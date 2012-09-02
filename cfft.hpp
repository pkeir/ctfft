#ifndef _CONSTFFT_HPP_
#define _CONSTFFT_HPP_

/* 
  Header providing FFT implementation using C++11 tuples.
  Copyright (C) 2012 Paul Keir
  Distributed under the GNU General Public License. See license.txt for details.
*/

#ifndef SZ
#error Please #define SZ macro. Try 8: -DSZ=8
#define SZ 8
#endif

#include "const_complex.hpp"

#ifdef CONSTTUP
#include "const_tuple.hpp"
using ctup::tuple;
using ctup::get;
using ctup::make_tuple;
using ctup::tuple_cat;
#else
#include <tuple>
using std::tuple;
using std::get;
using std::make_tuple;
using std::tuple_cat;
#endif
#include "util.hpp"
#include "print_tuple.hpp"
using std::enable_if;

namespace tupconst {

template <size_t I, size_t N>
struct iota_helper {
  static
  constexpr
  auto _() ->
    decltype(tuple_cat(make_tuple(I),iota_helper<I+1,N>::_())) {
    return   tuple_cat(make_tuple(I),iota_helper<I+1,N>::_());
  }
};

template <size_t I>
struct iota_helper<I,I> {
  static
  constexpr
  tuple<> _() {
    return tuple<>();
  }
};

template <size_t N>
constexpr
auto
iota() ->
  decltype(iota_helper<0,N>::_()) {
  return   iota_helper<0,N>::_();
}

template<typename F, typename ...Ts, size_t ...Is>
constexpr
auto
map_helper(F f, tuple<Ts...> t, indices<Is...>) ->
  decltype(make_tuple( f(get<Is>(t))... )) {
  return   make_tuple( f(get<Is>(t))... );
}

template<typename F, typename ...Ts>
constexpr
auto
map(F f, tuple<Ts...> t) ->
  decltype(map_helper(f,t,typename make_indices<0,1,Ts...>::type())) {
  return   map_helper(f,t,typename make_indices<0,1,Ts...>::type());
}

template<typename F, typename ...Ts, typename ...Us, size_t ...Is>
constexpr
auto
zipWith_helper(F f, tuple<Ts...> t1, tuple<Us...> t2, indices<Is...>) -> 
  decltype(make_tuple( f(get<Is>(t1),get<Is>(t2))... )) {
  return   make_tuple( f(get<Is>(t1),get<Is>(t2))... );
}

template<typename F, typename ...Ts, typename ...Us>
constexpr
auto
zipWith(F f, tuple<Ts...> t1, tuple<Us...> t2) ->
  decltype(zipWith_helper(f,t1,t2,typename make_indices<0,1,Ts...>::type())) {
  return   zipWith_helper(f,t1,t2,typename make_indices<0,1,Ts...>::type());
}

template<typename ...Ts, size_t ...Is>
constexpr
auto
select(tuple<Ts...> t, indices<Is...>) ->
  decltype(make_tuple( get<Is>(t)... )) {
  return   make_tuple( get<Is>(t)... );
}

template <typename T, typename ...Ts>
constexpr
tuple<Ts...>
tuple_tail(tuple<T,Ts...> t) {
  return select(t,typename make_indices_tail<T,Ts...>::type());
}

template <size_t N, typename ...Ts>
constexpr
auto
condenseN(tuple<Ts...> t) ->
  decltype(select(t,typename make_indices<0,N,Ts...>::type())) {
  return   select(t,typename make_indices<0,N,Ts...>::type());
}

template <typename ...Ts>
constexpr
auto
condense(tuple<Ts...> t) ->
  decltype(condenseN<2>(t)) {
  return   condenseN<2>(t);
}

template <typename ...Ts, typename ...Us>
constexpr
tuple<Ts...,Us...>
cat(tuple<Ts...> t1, tuple<Us...> t2) {
  return tuple_cat(t1,t2);
}

template <typename T, typename ...Ts>
constexpr
tuple<Ts...,T>
cshift1(tuple<T,Ts...> t) {
  return tuple_cat(tuple_tail(t),make_tuple(get<0>(t)));
}

constexpr
tuple<>
cshift1(tuple<> t) {
  return t;
}

template <typename ...Ts>
struct fft_helper {
  typedef typename pack_head<Ts...>::type T; // T is usually cx<double>
  template <typename ...Us>
  static constexpr
  typename enable_if<sizeof...(Ts)==(2*sizeof...(Us)),tuple<Ts...>>::type
  _(tuple<Ts...> v, tuple<Us...> rofu) {
    return cat(zipWith(Sum<T>(),
                         fft_helper<Us...>::_(condense(v),
                                              condense(rofu)),
                         zipWith(Product<T>(),
                                   fft_helper<Us...>::_(condense(cshift1(v)),
                                                        condense(rofu)),
                                   rofu)),
               zipWith(Sub<T>(),
                         fft_helper<Us...>::_(condense(v),
                                              condense(rofu)),
                         zipWith(Product<T>(),
                                   fft_helper<Us...>::_(condense(cshift1(v)),
                                                        condense(rofu)),
                                   rofu)));
  }
};

template <typename T>
struct fft_helper<T,T> {
  static constexpr tuple<T,T> _(tuple<T,T> v, tuple<T>) {
    return make_tuple(get<0>(v)+get<1>(v),
                      get<0>(v)-get<1>(v));
  }
};

template <typename T, typename ...Ts> // For now; move to is_same
constexpr
typename enable_if<are_same<T,Ts...>::value,tuple<T,Ts...>>::type
fft(tuple<T,Ts...> v) {
  return fft_helper<T,Ts...>::_(v,map(InitRofu<T>(sizeof...(Ts)+1),
                                      iota<(sizeof...(Ts)+1)/2>()));
}

}; // tupconst

using tupconst::fft;
using tupconst::map;
using tupconst::iota;

#endif//  _CONSTFFT_HPP_
