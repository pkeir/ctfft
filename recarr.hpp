#ifndef _RECARR_HPP_
#define _RECARR_HPP_

/*
 A recursive array defined by its type and length; with associated functions.
 Copyright Paul Keir 2012-2016
 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file license.txt or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include <iostream>

// Add recarr namespace here as in cfft_recarr.hpp?

using std::cout; using std::ostream;

template <typename T, size_t N>
struct recarr {
  constexpr
  recarr(T x, recarr<T,N-1> a) : m_x(x), m_a(a) {}

  constexpr T             head() const { return m_x; }
  constexpr recarr<T,N-1> tail() const { return m_a; }

private:
  T m_x;
  recarr<T,N-1> m_a;
};

template <typename T>
struct recarr<T,0> {};

template <typename T>
constexpr
recarr<T,0> make_recarr()    { return recarr<T,0>(); }

#ifdef __clang__
// needed only by clang (crash)
template <typename T>
constexpr
recarr<T,1> make_recarr(T x) { return recarr<T,1>(x,recarr<T,0>()); }
#endif

// The call to make_recarr needs the T template argument:
// 1. make_recarr() needs this explicitly as there's not arg. to infer from.
// 2. make_recarr() has only one template argument.
// 3. Other calls to make_recarr(x,xs...) are content as template arguments
//    can all be inferred anyway; any prefix provided is optional.
template <typename T, typename ...Ts>
constexpr
recarr<T,sizeof...(Ts)+1> make_recarr(T x, Ts ...xs) {
#ifdef __clang__
  return recarr<T,sizeof...(Ts)+1>(x,make_recarr(xs...));
#else
  return recarr<T,sizeof...(Ts)+1>(x,make_recarr<T>(xs...));
#endif
}

template <typename T, size_t N>
constexpr recarr<T,N+1> recarr_cons(T x, recarr<T,N> xs) {  // N and N-1 fails.
  return recarr<T,N+1>(x,xs);
}

template <typename T>
void print_recarr(recarr<T,0> a, ostream &) {} // n.b. a has no head member

template <typename T>
void print_recarr(recarr<T,1> a, ostream &o = cout) { o << a.head(); }

template <typename T, size_t N>
void print_recarr(recarr<T,N> a, ostream &o = cout) {
  o << a.head() << ",";
  print_recarr(a.tail(),o);
}

template <typename T, size_t N>
inline ostream &operator << (ostream &o, const recarr<T,N> &arr) {
  o << "(";
  print_recarr(arr,o);
  o << ")";
  return o;
}

#endif // _RECARR_HPP_
