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
#include "tuple_util.hpp"
using std::enable_if;

namespace tupconst {

template<size_t ...Is>
constexpr
auto iota_helper(indices<Is...>) ->
  decltype(make_tuple(Is...)) {
  return   make_tuple(Is...);
}

template <size_t N>
constexpr
auto
iota() ->
  decltype(iota_helper(mk_index_range<0,N-1>())) {
  return   iota_helper(mk_index_range<0,N-1>());
}

template<typename F, typename ...Ts, size_t ...Is>
constexpr
auto
map_helper(F f, tuple<Ts...> t, indices<Is...>) ->
  decltype(make_tuple( f(get<Is>(t))... )) {
  return   make_tuple( f(get<Is>(t))... );
}

template <typename F, typename ...Ts>
constexpr
auto
map(F f, tuple<Ts...> t) ->
  decltype(map_helper(f,t, mk_index_range<0,sizeof...(Ts)-1>())) {
  return   map_helper(f,t, mk_index_range<0,sizeof...(Ts)-1>());
}

template <typename F, typename ...Ts, typename ...Us, size_t ...Is>
constexpr
auto
zipWith_helper(F f, tuple<Ts...> t1, tuple<Us...> t2, indices<Is...>) -> 
  decltype(make_tuple( f(get<Is>(t1),get<Is>(t2))... )) {
  return   make_tuple( f(get<Is>(t1),get<Is>(t2))... );
}

template <typename F, typename ...Ts, typename ...Us>
constexpr
auto
zipWith(F f, tuple<Ts...> t1, tuple<Us...> t2) ->
  decltype(zipWith_helper(f,t1,t2,mk_index_range<0,sizeof...(Ts)-1>())) {
  return   zipWith_helper(f,t1,t2,mk_index_range<0,sizeof...(Ts)-1>());
}

// Crashes clang 3.2 snapshot 
/*
template <size_t N, typename ...Ts>
constexpr
auto
condenseN(tuple<Ts...> t) ->
  decltype(select(t,mk_index_range<0,sizeof...(Ts)-1,N>())) {
  return   select(t,mk_index_range<0,sizeof...(Ts)-1,N>());
}*/

template <typename ...Ts>
constexpr
auto
condense2(tuple<Ts...> t) ->
  decltype(select(t,mk_index_range<0,sizeof...(Ts)-1,2>())) {
  return   select(t,mk_index_range<0,sizeof...(Ts)-1,2>());
}

template <typename ...Ts>
constexpr
auto
condense(tuple<Ts...> t) ->
  decltype(condense2(t)) { // Avoids clang problem.
  return   condense2(t);
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

template <typename T>
constexpr
tuple<T,T> fft_helper(tuple<T,T> v, tuple<T>) {
  return make_tuple(get<0>(v)+get<1>(v), get<0>(v)-get<1>(v));
}

template <typename ...Ts, typename ...Us>
constexpr
typename enable_if<sizeof...(Ts)==(2*sizeof...(Us)),tuple<Ts...>>::type
fft_helper(tuple<Ts...> v, tuple<Us...> rofu) {
  typedef typename pack_head<Ts...>::type T;   // T is usually cx<double>
  return cat(zipWith(sum<T>,fft_helper(condense(v), condense(rofu)),
                            zipWith(product<T>,fft_helper(condense(cshift1(v)),
                                                          condense(rofu)),
                                               rofu)),
             zipWith(sub<T>,fft_helper(condense(v), condense(rofu)),
                            zipWith(product<T>,fft_helper(condense(cshift1(v)),
                                                          condense(rofu)),
                                               rofu)));
}

template <typename ...Ts>
constexpr
typename enable_if<are_same<Ts...>::value,tuple<Ts...>>::type
fft(tuple<Ts...> v) {
  typedef typename pack_head<Ts...>::type T;
  return fft_helper(v,map(InitRofu<T>(sizeof...(Ts)),
                          iota<(sizeof...(Ts))/2>()));
}

}; // tupconst

using tupconst::fft;
using tupconst::map;
using tupconst::iota;

#endif//  _CONSTFFT_HPP_
