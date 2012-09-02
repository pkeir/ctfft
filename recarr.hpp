#ifndef _RECARR_HPP_
#define _RECARR_HPP_

/* 
  A recursive array defined by its type and length; with associated functions.
  Copyright (C) 2012 Paul Keir
  Distributed under the GNU General Public License. See license.txt for details.
*/

#include <iostream>

using std::cout; using std::ostream;

template <typename T, size_t N>
struct recarr {
  constexpr recarr(T x, recarr<T,N-1> a) : m_x(x), m_a(a) {}

  constexpr T             head()   { return m_x; }
  constexpr recarr<T,N-1> tail()   { return m_a; }

private:
  T m_x;
  recarr<T,N-1> m_a;
};

template <typename T>
struct recarr<T,0> {};

template <typename T>
constexpr recarr<T,0> make_recarr()    { return recarr<T,0>(); }

template <typename T>
constexpr recarr<T,1> make_recarr(T x) { return recarr<T,1>(x,recarr<T,0>()); }

template <typename T, typename ... Ts>
constexpr auto make_recarr(T x, Ts ... xs) -> recarr<T,sizeof...(Ts)+1> {
  return recarr<T,sizeof...(Ts)+1>(x,make_recarr(xs...));
}

template <typename T, size_t N>
constexpr recarr<T,N+1> recarr_cons(T x, recarr<T,N> xs) {  // N and N-1 fails.
  return recarr<T,N+1>(x,xs);
}

template <typename T, size_t N>
struct print_recarr {
  static void _(recarr<T,N> a, ostream &o = cout) {
    o << a.head() << ",";
    print_recarr<T,N-1>::_(a.tail(),o);
  }
};

template <typename T>
struct print_recarr<T,1> {
  static void _(recarr<T,1> a, ostream &o = cout) { o << a.head(); }
};

template <typename T>
struct print_recarr<T,0> {
  static void _(recarr<T,0> a, ostream &) {}
};

template <typename T, size_t N>
inline ostream &operator << (ostream &o, const recarr<T,N> &arr) {
  o << "(";
  print_recarr<T,N>::_(arr,o);
  o << ")";
  return o;
}

#endif // _RECARR_HPP_
