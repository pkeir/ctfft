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

template <size_t...>
  struct indices{};

template <size_t N, size_t CurStep, size_t Step,
          typename IntTuple, typename...Types>
  struct make_indices_impl;

template <size_t N,                 size_t Step, size_t ...Indices,
          typename T, typename...Types>
struct make_indices_impl<N, N,       Step, indices<Indices...>, T, Types...>
{
  typedef typename make_indices_impl<N+1, N+Step, Step, indices<Indices...,N>,
    Types...>::type type;
};

template <size_t N, size_t CurStep, size_t Step, size_t... Indices,
          typename T, typename...Types>
struct make_indices_impl<N, CurStep, Step, indices<Indices...>, T, Types...>
{
  typedef typename make_indices_impl<N+1, CurStep, Step, indices<Indices...>,
    Types...>::type type;
};

template <size_t N, size_t CurStep, size_t Step, size_t... Indices>
struct make_indices_impl<N, CurStep, Step, indices<Indices...> > {
  typedef indices<Indices...> type;
};

//template <size_t Step, typename... Types>
//struct make_indices : make_indices_impl<0, 0, Step, indices<>, Types...> {};
template <size_t Start, size_t Step, typename... Types>
struct make_indices :
       make_indices_impl<Start, Start, Step, indices<>, Types...> {};

template <typename>
  struct make_indices_tail_helper;

template <size_t I, size_t ...Indices>
struct make_indices_tail_helper<indices<I,Indices...>> {
  typedef indices<Indices...> type;
};

template <typename T, typename ...Ts>
struct make_indices_tail {
  typedef typename make_indices_tail_helper<
//                     typename make_indices<1,T,Ts...>::type
                     typename make_indices<0,1,T,Ts...>::type
                   >::type type;
};

template <typename ...Ts>
  struct are_same;

template <typename T, typename ...Ts>
struct are_same<T,T,Ts...> {
  static const bool value = true && are_same<T,Ts...>::value;
};

template <typename t>
struct are_same<t,t> {
  static const bool value = true;
};

template <typename T, typename ...Ts>
struct pack_head {
  typedef T type;
};

template <typename T>
struct Init {
  constexpr Init(size_t n_) : n(n_) {}
  constexpr T operator()(T y) {
    return 2 * M_PI / n * y;
  }
  size_t n;
};

template <typename T>
struct InitRofu {
  constexpr InitRofu(size_t n_) : n(n_) {}
  constexpr T operator()(size_t i) {
    return pow(exp(-2 * M_PI * T(0,1) / n),i);
  }
  size_t n;
};

template <typename T, typename U>
struct StaticCast {
  constexpr T operator()(U u) { return static_cast<T>(u); }
};

struct Succ    {
  constexpr int operator()(int x)  { return x+1; }
};

template <typename T>
struct Sum {
  constexpr T operator()(T x, T y) { return x+y; }
};

template <typename T>
struct Sub {
  constexpr T operator()(T x, T y) { return x-y; }
};

template <typename T>
struct Product {
  constexpr T operator()(T x, T y) { return x*y; }
};

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
constexpr Comp<Fa,Fb> compose(Fa fa, Fb fb) { return Comp<Fa,Fb>(fa,fb); };

template <typename Fa, typename Fb, typename Fc, typename ... Fs>
constexpr auto compose(Fa fa, Fb fb, Fc fc, Fs ... fs) ->
  decltype(Comp<Fa,decltype(compose(fb,fc,fs...))>(fa,compose(fb,fc,fs...))) {
  return   Comp<Fa,decltype(compose(fb,fc,fs...))>(fa,compose(fb,fc,fs...));
};

#endif // _UTIL_HPP_
