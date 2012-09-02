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

template <size_t I, size_t N>
struct iota_helper {
  static
  constexpr
  recarr<size_t,N-I> _() {
    return recarr<size_t,N-I>(I,iota_helper<I+1,N>::_());
  }
};

template <size_t C>
struct iota_helper<C,C> {
  static
  constexpr
  recarr<size_t,0> _() {
    return recarr<size_t,0>();
  }
};

template <size_t N> // , typename T = typename enable_if<N>0>::type>
constexpr
recarr<size_t,N> iota() {
  return iota_helper<0,N>::_();
}

template <typename F, typename T, size_t N>
struct map_helper {
  static
  constexpr
  auto _(F f, recarr<T,N> a) ->
    recarr<decltype(f(T())),N> {
      return recarr_cons(f(a.head()),map_helper<F,T,N-1>::_(f,a.tail()));
  }
};

template <typename F, typename T>
struct map_helper<F,T,0> {
  static
  constexpr
  auto _(F f, recarr<T,0> a) ->
    recarr<decltype(f(T())),0> {
      return recarr<decltype(f(T())),0>();
  }
};

template <typename F, typename T, size_t N>
constexpr
auto map(F f, recarr<T,N> a) ->
  decltype(map_helper<F,T,N>::_(f,a)) {
  return   map_helper<F,T,N>::_(f,a);
};

template <typename F, typename T, size_t N>
struct zipWith_helper {
  static
  constexpr
  auto _(F f, recarr<T,N> a1, recarr<T,N> a2) ->
    recarr<decltype(f(T(),T())),N> {
      return recarr_cons(f(a1.head(),a2.head()),
                         zipWith_helper<F,T,N-1>::_(f,a1.tail(),a2.tail()));
  }
};

template <typename F, typename T>
struct zipWith_helper<F,T,0> {
  static
  constexpr
  auto _(F f, recarr<T,0> a1, recarr<T,0> a2) ->
           recarr<decltype(f(T(),T())),0> {
    return recarr<decltype(f(T(),T())),0>();
  }
};

template <typename F, typename T, size_t N>
constexpr
auto zipWith(F f, recarr<T,N> a1, recarr<T,N> a2) ->
  decltype(zipWith_helper<F,T,N>::_(f,a1,a2)) {
  return   zipWith_helper<F,T,N>::_(f,a1,a2);
}

template <typename T, size_t N>
struct condense_helper {
  static
  constexpr
  recarr<T,N/2> _(recarr<T,N> arr) {
    return recarr_cons(arr.head(),condense_helper<T,N-2>::_(arr.tail().tail()));
  }
};

template <typename T>
struct condense_helper<T,0> {
  static
  constexpr
  recarr<T,0> _(recarr<T,0> arr) { return arr; }
};

template <typename T, size_t N>
constexpr
recarr<T,N/2> condense(recarr<T,N> arr) {
  return condense_helper<T,N>::_(arr);
}

// cat (x:xs) ys = x : (cat xs ys)
// cat []     ys = ys
template <typename T, size_t N1, size_t N2>
struct cat_helper {
  static
  constexpr
  recarr<T,N1+N2> _(recarr<T,N1> a1, recarr<T,N2> a2) {
    return recarr_cons(a1.head(),cat_helper<T,N1-1,N2>::_(a1.tail(),a2));
  }
};

template <typename T, size_t N2>
struct cat_helper<T,0,N2> {
  static
  constexpr
  recarr<T,N2> _(recarr<T,0>, recarr<T,N2> a2) { return a2; }
};

template <typename T, size_t N1, size_t N2>
constexpr
recarr<T,N1+N2> cat(recarr<T,N1> a1, recarr<T,N2> a2) {
  return cat_helper<T,N1,N2>::_(a1,a2);
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

template <typename T, size_t N>
struct fft_helper {
  static constexpr recarr<T,N> _(recarr<T,N> v, recarr<T,N/2> rofu) {
    return cat(zipWith(Sum<T>(),
                         fft_helper<T,N/2>::_(condense(v),
                                              condense(rofu)),
                         zipWith(Product<T>(),
                                   fft_helper<T,N/2>::_(condense(cshift1(v)),
                                                        condense(rofu)),
                                   rofu)),
               zipWith(Sub<T>(),
                         fft_helper<T,N/2>::_(condense(v),
                                              condense(rofu)),
                         zipWith(Product<T>(),
                                   fft_helper<T,N/2>::_(condense(cshift1(v)),
                                                        condense(rofu)),
                                   rofu)));
  }
};

template <typename T>
struct fft_helper<T,2> {
  static constexpr recarr<T,2> _(recarr<T,2> v, recarr<T,1>) {
    return make_recarr(v.head()+v.tail().head(),
                       v.head()-v.tail().head());
  }
};

template <typename T, size_t N>
constexpr recarr<T,N> fft(recarr<T,N> v) {
  return fft_helper<T,N>::_(v,map(InitRofu<T>(N),iota<N/2>()));
}

}; // recarrconst

using recarrconst::fft;
using recarrconst::map;
using recarrconst::iota;

#endif // _CONSTFFT_RECARR_HPP_
