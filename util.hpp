#ifndef _UTIL_HPP_
#define _UTIL_HPP_

/* 
  Function objects, FFT helpers, and utility functions for variadic templates.
  Copyright (C) 2012 Paul Keir
  Distributed under the GNU General Public License. See license.txt for details.
*/

#ifdef CONSTMATH
#include "const_math.hpp"
#else
#include <cmath>
#endif

#include "mk_index_range.hpp"

template <typename ...Ts>
  struct are_same;

template <typename T, typename ...Ts>
struct are_same<T,T,Ts...> {
  static const bool value = true && are_same<T,Ts...>::value;
};

template <typename T>
struct are_same<T,T> {
  static const bool value = true;
};

template <typename T, typename ...Ts>
struct pack_head {
  typedef T type;
};

template <typename T>
struct Init {
  constexpr Init(std::size_t n_) : n(n_) {}
  constexpr T operator()(T y) {
    return 2 * M_PI / n * y;
  }
  std::size_t n;
};

template <typename T>
struct InitRofu {
  constexpr InitRofu(std::size_t n_) : n(n_) {}
  constexpr T operator()(std::size_t i) {
    return pow(exp(-2 * M_PI * T(0,1) / n),i);
  }
  std::size_t n;
};

template <typename T, typename U>
struct StaticCast {
  constexpr T operator()(U u) { return static_cast<T>(u); }
};

struct Succ    {
  constexpr int operator()(int x)  { return x+1; }
};

template <typename T>
struct Sum {               // On the road to FoldR and FoldL ?

  constexpr T operator()() { return T(); }

  template <typename ...Ts>
  constexpr
  T operator()(T x, Ts ...xs) { return x+operator()(xs...); }

};

template <typename T> constexpr T sum(T x, T y) { return x+y; } // concise

template <typename F, typename T>
struct FoldL {

  constexpr FoldL(F f, T z) : m_f(f), m_z(z) {}

  constexpr T operator()() { return m_z; }

  template <typename ...Ts>
  constexpr
  T operator()(T x, Ts ...xs) { return m_f(x,operator()(xs...)); }

  F m_f;
  T m_z;

};

template <typename F, typename T>
constexpr
FoldL<F,T> make_foldl(F f, T z) { return FoldL<F,T>(f,z); }

template <typename T>
struct Sub {
  constexpr T operator()(T x, T y) { return x-y; }
};

template <typename T> constexpr T sub(T x, T y) { return x-y; } // concise

template <typename T>
struct Product {
  constexpr T operator()(T x, T y) { return x*y; }
};

template <typename T> constexpr T product(T x, T y) { return x*y; } // concise

template <typename T>
struct Id {
  constexpr T operator()(T x) { return x; }
};

template <typename Fa, typename Fb>
struct Comp {
  Fa fa;
  Fb fb;
  constexpr Comp(Fa fa_, Fb fb_) : fa(fa_), fb(fb_) {}

  template <typename ... Ts>
  constexpr auto operator()(Ts ... ts) -> decltype(fa(fb(ts...))) {
    return fa(fb(ts...));
  };
};

template <typename Fa, typename Fb>
constexpr
Comp<Fa,Fb> compose(Fa fa, Fb fb) { return Comp<Fa,Fb>(fa,fb); };

template <typename Fa, typename Fb, typename Fc, typename ... Fs>
constexpr
auto compose(Fa fa, Fb fb, Fc fc, Fs ... fs) ->
  decltype(Comp<Fa,decltype(compose(fb,fc,fs...))>(fa,compose(fb,fc,fs...))) {
  return   Comp<Fa,decltype(compose(fb,fc,fs...))>(fa,compose(fb,fc,fs...));
};

#endif // _UTIL_HPP_
