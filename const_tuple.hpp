#ifndef _CONST_TUPLE_HPP_
#define _CONST_TUPLE_HPP_

/* 
 C++11 constexpr versions of the C++11 tuple, and functions needed for the FFT.
 Copyright Paul Keir 2012-2016
 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file license.txt or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include "util.hpp"

namespace ctup {

template <typename...>
  struct tuple;

template <typename T, typename ...Ts>
struct tuple<T, Ts...> {
  constexpr tuple() {}
  constexpr tuple(T h, Ts ...t) : head(h), tail(t...) {}
  T head;
  tuple<Ts...> tail;
};

template <>
struct tuple<> {};

template <typename ...Ts>
constexpr
tuple<Ts...> make_tuple(Ts ...ts) { return tuple<Ts...>(ts...); }

template <typename Tup>
  struct tuple_size;

template <typename ...Ts>
struct tuple_size<tuple<Ts...>> {
  static const std::size_t value = sizeof...(Ts);
};

template <typename Tup, std::size_t I>
  struct tuple_element;

template <typename T, typename ...Ts, std::size_t I>
struct tuple_element<tuple<T,Ts...>,I> {
  typedef typename tuple_element<tuple<Ts...>,I-1>::type type;
};

template <typename T, typename ...Ts>
struct tuple_element<tuple<T,Ts...>,0> {
  typedef T type;
};

template <typename Tup, std::size_t I>
struct get_helper {
  static
  constexpr
  auto _(Tup tup) ->
    decltype(get_helper<decltype(tup.tail),I-1>::_(tup.tail)) { 
    return   get_helper<decltype(tup.tail),I-1>::_(tup.tail);
  }
};

template <typename Tup>
struct get_helper<Tup,0> {
  static
  constexpr
  auto _(Tup tup) ->
    decltype(tup.head) { 
    return   tup.head;
  }
};

template <std::size_t I, typename Tup>
constexpr
auto get(Tup tup) ->
  decltype(get_helper<Tup,I>::_(tup)) {
  return   get_helper<Tup,I>::_(tup);
}

template  <typename Tup1, typename Tup2, std::size_t ...Is, std::size_t ...Js>
constexpr
auto tuple_cat_helper(Tup1 t1, Tup2 t2, indices<Is...>, indices<Js...>) ->
  decltype(make_tuple(get<Is>(t1)...,get<Js>(t2)...)) {
  return   make_tuple(get<Is>(t1)...,get<Js>(t2)...);
}

template <typename ...Ts>
constexpr
tuple<>
tuple_cat() { return tuple<>(); }       // Clang gives an error for 0 arg.

template <typename ...Ts>
constexpr
tuple<Ts...>
tuple_cat(tuple<Ts...> t) { return t; } // Clang gives an error for 1 arg.

// cat (x:xs) ys = x : (cat xs ys)
// cat []     ys = ys

template <typename ...Ts, typename ...Us, typename ...Tups>
constexpr
auto
tuple_cat(tuple<Ts...> t1, tuple<Us...> t2, Tups ...ts) ->
  decltype(tuple_cat(
             tuple_cat_helper(
               t1,t2,
               mk_index_range<0,sizeof...(Ts)-1>(),
               mk_index_range<0,sizeof...(Us)-1>()
             ),
             ts...
           )
          )
{
  return   tuple_cat(
             tuple_cat_helper(
               t1,t2,
               mk_index_range<0,sizeof...(Ts)-1>(),
               mk_index_range<0,sizeof...(Us)-1>()
             ),
             ts...
           );
}

}; // namespace ctup

#endif //  _CONST_TUPLE_HPP_
