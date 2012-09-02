#ifndef _CONST_TUPLE_HPP_
#define _CONST_TUPLE_HPP_

/* 
  C++11 constexpr versions of the C++11 tuple, and functions needed for the FFT.
  Copyright (C) 2012 Paul Keir
  Distributed under the GNU General Public License. See license.txt for details.
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

template <typename Tup, size_t I>
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

template <size_t I, typename Tup>
constexpr
auto get(Tup tup) ->
  decltype(get_helper<Tup,I>::_(tup)) {
  return   get_helper<Tup,I>::_(tup);
}

template  <typename Tup1, typename Tup2, size_t ...Is, size_t ...Js>
constexpr
auto tuple_cat_helper(Tup1 t1, Tup2 t2, indices<Is...>, indices<Js...>) ->
  decltype(make_tuple(get<Is>(t1)...,get<Js>(t2)...)) {
  return   make_tuple(get<Is>(t1)...,get<Js>(t2)...);
}

// This tuple_cat only defined for two parameters.
template <typename ...Ts, typename ...Us>
constexpr
tuple<Ts...,Us...>
tuple_cat(tuple<Ts...> t1, tuple<Us...> t2) {
  return tuple_cat_helper(
    t1,t2,
    typename make_indices<0,1,Ts...>::type(),
    typename make_indices<0,1,Us...>::type()
  );
}

}; // namespace ctup

#endif //  _CONST_TUPLE_HPP_
