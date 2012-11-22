#ifndef _CONSTFFT_RECARR_HPP_
#define _CONSTFFT_RECARR_HPP_

/* 
  Header providing FFT implementation using a recursive array type.
  Copyright (C) 2012 Paul Keir
  Distributed under the GNU General Public License. See license.txt for details.
*/

#ifndef SZ
#error Please #define SZ macro. Try 8: -DSZ=8
#define SZ 8
#endif

#include "const_complex.hpp"
#include "recarr.hpp"
#include "util.hpp"

namespace recarrconst {

template <size_t N>
constexpr
recarr<size_t,N> iota_helper(size_t i) {
  return recarr<size_t,N>(i,iota_helper<N-1>(i+1));
}

template <>
constexpr
recarr<size_t,0> iota_helper<0>(size_t) {
  return recarr<size_t,0>();
};

template <size_t N>
constexpr
recarr<size_t,N> iota() {
  return iota_helper<N>(0);
}

template <typename F, typename T>
constexpr
auto map(F f, recarr<T,0>) ->
         recarr<decltype(f(T())),0> {
  return recarr<decltype(f(T())),0>();
};

template <typename F, typename T, size_t N>
constexpr
auto map(F f, recarr<T,N> a) ->
  recarr<decltype(f(T())),N> {
  return recarr_cons(f(a.head()),map(f,a.tail()));
};

template <typename F, typename T>
constexpr
auto zipWith(F f, recarr<T,0>, recarr<T,0>) ->
         recarr<decltype(f(T(),T())),0> {
  return recarr<decltype(f(T(),T())),0>();
}

template <typename F, typename T, size_t N>
constexpr
auto zipWith(F f, recarr<T,N> a1, recarr<T,N> a2) ->
  recarr<decltype(f(T(),T())),N> {
  return recarr_cons(f(a1.head(),a2.head()),
                     zipWith(f,a1.tail(),a2.tail()));
}

template <typename T>
constexpr
recarr<T,0> condense(recarr<T,0> arr) {
  return arr; 
}

template <typename T, size_t N>
constexpr
recarr<T,N/2> condense(recarr<T,N> arr) {
  return recarr_cons(arr.head(),condense(arr.tail().tail()));
}

// Can't rely on one func definition alone: recarr<T,0> has no head!
template <typename T, size_t N2>
constexpr
recarr<T,N2> cat(recarr<T,0>, recarr<T,N2> a2) {
  return a2;
}

template <typename T, size_t N1, size_t N2>
constexpr
recarr<T,N1+N2> cat(recarr<T,N1> a1, recarr<T,N2> a2) {
  return recarr_cons(a1.head(),cat(a1.tail(),a2));
}

template <typename T, size_t N>
constexpr
recarr<T,N> cshift1(recarr<T,N> arr) {
  return cat(arr.tail(),recarr<T,1>(arr.head(),recarr<T,0>()));
}

template <typename T>
constexpr
recarr<T,0> cshift1(recarr<T,0> arr) {
  return arr;
}

template <typename T>
constexpr
recarr<T,2> fft_helper(recarr<T,2> v, recarr<T,1>) {
  return make_recarr(v.head()+v.tail().head(),
                     v.head()-v.tail().head());
}

template <typename T, size_t N, size_t N_2>
constexpr
typename std::enable_if<N==N_2*2,recarr<T,N>>::type
fft_helper(recarr<T,N> v, recarr<T,N_2> rofu) {
  return cat(zipWith(sum<T>,fft_helper(condense(v), condense(rofu)),
                            zipWith(product<T>,fft_helper(condense(cshift1(v)),
                                                          condense(rofu)),
                                               rofu)),
             zipWith(sub<T>,fft_helper(condense(v), condense(rofu)),
                            zipWith(product<T>,fft_helper(condense(cshift1(v)),
                                                          condense(rofu)),
                                               rofu)));
}

template <typename T, size_t N>
constexpr recarr<T,N> fft(recarr<T,N> v) {
  return fft_helper(v,map(InitRofu<T>(N),iota<N/2>()));
}

}; // recarrconst

using recarrconst::fft;
using recarrconst::map;
using recarrconst::iota;

#endif // _CONSTFFT_RECARR_HPP_
