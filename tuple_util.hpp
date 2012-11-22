#ifndef _TUPLE_UTIL_HPP_
#define _TUPLE_UTIL_HPP_

/* 
  Insertion operator (<<), tuple_tail and tuple_reverse for a
  C++11-style tuple. Needs tuple, make_tuple, and get defined.
  Copyright (C) 2012 Paul Keir
  Distributed under the GNU General Public License. See license.txt for details.
*/

template <typename ...Ts, size_t ...Is>
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
  return select(t, mk_index_range<1,sizeof...(Ts)>()); // n.b T,Ts; not Ts
}

template <typename ...Ts>
constexpr
tuple<Ts...>
tuple_reverse(tuple<Ts...> t) {
  return select(t, mk_index_range<sizeof...(Ts)-1,0,-1>());
}

void print_tuple(const tuple<>  &t, std::ostream &o = std::cout) {}

template <typename T>
void print_tuple(const tuple<T> &t, std::ostream &o = std::cout) {
  o << get<0>(t);
}

template <typename T1, typename T2, typename ...Ts>
void print_tuple(const tuple<T1,T2,Ts...> &t, std::ostream &o = std::cout) {
  o << get<0>(t) << ",";
  print_tuple(tuple_tail(t),o);
}

template <typename ...Ts>
inline std::ostream &operator << (std::ostream &o, const tuple<Ts...> &t) {
  std::cout << "(";
  print_tuple(t,o);
  std::cout << ")";
  return o;
}

#endif // _TUPLE_UTIL_HPP_
