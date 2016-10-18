#ifndef _PRINT_TUPLE_HPP_
#define _PRINT_TUPLE_HPP_

/* 
 Insertion operator (<<) and helper struct for a C++11-style tuple.
 Copyright Paul Keir 2012-2016
 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file license.txt or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include <iostream>

using std::cout; using std::ostream;

template <typename Tup, std::size_t N>
struct print_tuple {
  static void _(const Tup &t, ostream &o = cout) {
    print_tuple<Tup,N-1>::_(t,o);
    o << "," << get<N-1>(t);
  }
};
 
template <typename Tup>
struct print_tuple<Tup,1> {
  static void _(const Tup &t, ostream &o = cout) { o << get<0>(t); }
};

template <typename Tup>
struct print_tuple<Tup,0> {
  static void _(const Tup &, ostream &) {}
};
 
template <typename ...Ts>
inline ostream &operator << (ostream &o, const tuple<Ts...> &t) {
  cout << "(";
  print_tuple<tuple<Ts...>, sizeof...(Ts)>::_(t,o);
  cout << ")";
  return o;
}

#endif // _PRINT_TUPLE_HPP_
