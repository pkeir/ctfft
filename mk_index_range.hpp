#ifndef _MK_INDEX_RANGE_HPP_
#define _MK_INDEX_RANGE_HPP_

/*
 Provides mk_index_range struct to generate indices for tuple manipulation.
 Copyright Paul Keir 2012-2016
 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file license.txt or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include <cstddef>      // std::size_t
#include <type_traits>  // std::remove_reference

template <typename T, T...>
struct indicesT {};

template <std::size_t ...Is>
using indices = indicesT<std::size_t, Is...>;

template <typename T, typename U>
constexpr 
auto max(T x, U y) ->
  typename std::remove_reference<decltype(x > y ? x : y)>::type {
  return   x > y ? x : y;
}

// Problem: if Tlu is unsigned, and u<l, u-l *should* be negative.
// Solution: Cast to a signed type. n.b. Ts is signed when the problem occurs.
template <typename Tlu, typename Ts>
constexpr Ts niters(Tlu l, Tlu u, Ts s) {
  return max((static_cast<Ts>(u)-static_cast<Ts>(l)+s)/s,0);
}

// The base case (the specialisation) would be negative for n,0,-s
template <typename Tlu, typename Ts, Tlu L, Tlu U, Ts S, Tlu ...Is>
struct MkIndicesH {
  typedef typename MkIndicesH<Tlu,Ts,L+S,U,S,Is...,L>::type type;
};

template <typename Tlu, typename Ts,        Tlu U, Ts S, Tlu ...Is>
struct MkIndicesH<Tlu,Ts,U,U,S,Is...> {
  typedef indicesT<Tlu,Is...> type;
};

template <typename Tlu, typename Ts, Tlu Fl, Tlu Fu, Ts Fs=1>
using MkIndices =
  typename MkIndicesH<Tlu,Ts,Fl,Fl+niters(Fl,Fu,Fs)*Fs,Fs>::type;

// See usage example in map function from cfft.cpp
// e.g. mk_index_range<1,5,2> == indicesT<size_t,1,3,5> == indices<1,3,5>
template <std::size_t Fl, std::size_t Fu, int Fs=1>
using mk_index_range  = MkIndices<decltype(Fl),decltype(Fs),Fl,Fu,Fs>;

template <int Fl,    int Fu,    int Fs=1>
using mk_index_rangei = MkIndices<decltype(Fl),decltype(Fs),Fl,Fu,Fs>;

// The "old" way :)
template <typename ...Ts>
using mk_indices = mk_index_range<0,sizeof...(Ts)-1>;

#endif // _MK_INDEX_RANGE_HPP_
