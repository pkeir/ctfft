#ifndef _COMPLEX_HPP_
#define _COMPLEX_HPP_

/* 
  C++11 constexpr versions of the complex type and functions needed for the FFT.
  Copyright (C) 2012 Paul Keir
  Distributed under the GNU General Public License. See license.txt for details.
*/

// Complex number formulae from     http://www.clarku.edu/~djoyce/complex/
// and        http://en.literateprograms.org/Complex_numbers_(C_Plus_Plus)

#include <iostream>    // ostream and ostream used by << and >>

#ifdef CONSTMATH
#include "const_math.hpp"
#else
#include <cmath>
#endif

namespace cxns {

template <typename T>
struct cx {
  explicit constexpr cx(T r_, T i_=0) : r(r_), i(i_) {}
           constexpr cx()             : r(0),  i(0)  {}
  inline cx &operator=(const cx &c)  { r=c.r;  i=c.i;  return *this; }
  inline cx &operator+=(const cx &c) { r+=c.r; i+=c.i; return *this; }
  inline cx &operator*=(const cx &c) {
    T r_ = r*c.r - i*c.i;
    T i_ = r*c.i + i*c.r;
    r = r_; i = i_;
    return *this;
  }

  friend inline std::ostream &operator << (std::ostream &o, const cx &c) {
    o << "(" << c.r << "," << c.i << ")";
    return o;
  }

  friend inline std::istream &operator >> (std::istream &i,       cx &c) {
    i >> c.r >> c.i; // ? GCC expects both parts, and the comma; spaces ignored.
    return i;
  }

  T r,i;
};

template <typename T>
inline
constexpr
cx<T> operator+(const cx<T> &lhs, const cx<T> &rhs) {
  return cx<T>(lhs.r+rhs.r,lhs.i+rhs.i);
}

template <typename T>
inline
constexpr
cx<T> operator-(const cx<T> &lhs, const cx<T> &rhs) {
  return cx<T>(lhs.r-rhs.r,lhs.i-rhs.i);
}

template <typename T>
inline
constexpr
cx<T> operator*(const cx<T> &lhs, const cx<T> &rhs) {
  return cx<T>(lhs.r*rhs.r - lhs.i*rhs.i, lhs.r*rhs.i + lhs.i*rhs.r);
}

// Unused by fft
template <typename T>
inline
constexpr
cx<T> operator*(const T &lhs, const cx<T> &rhs) {
  return cx<T>(lhs*rhs.r, lhs*rhs.i);
}

// Yes, used by fft
template <typename T>
inline
constexpr cx<T> operator*(const int &lhs, const cx<T> &rhs) {
  return cx<T>(lhs*rhs.r, lhs*rhs.i);
}

// Unused by fft
template <typename T>
inline
constexpr
cx<T> operator/(const cx<T> &lhs, const T &rhs) {
  return cx<T>(lhs.r/rhs, lhs.i/rhs);
}

// Yes, used by fft
template <typename T>
inline
constexpr
cx<T> operator/(const cx<T> &lhs, const int &rhs) {
  return cx<T>(lhs.r/rhs, lhs.i/rhs);
}

template <typename T>
inline
constexpr
T abs(const cx<T> &z) { return sqrt(z.r*z.r + z.i*z.i); }

template <typename T>
inline
constexpr
T arg(const cx<T> &z) { return atan2(z.i,z.r); }

template <typename T>
inline
constexpr
cx<T> log(const cx<T> &z) {
  return cx<T>(::log(abs(z)), arg(z));
//  return cx<T>(log(abs(z)), arg(z));
}

template <typename T>
inline
constexpr
cx<T> pow(const cx<T> &x, const int y) {
  return exp(y*log(x));
}

/*
// More elegant(?), but less accurate than above?
template <typename T>
inline constexpr cx<T> pow(const cx<T> &x, const unsigned int y) {
  return y == 0 ? cx<T>(1) : y == 1 ? x : (x * pow(x,y-1));
}*/

template <typename T>
inline
constexpr
cx<T> polar(const T &rho, const T &theta) {
  return cx<T>(rho * cos(theta), rho * sin(theta));
}

template <typename T>
inline
constexpr
cx<T> exp(const cx<T> &z) {
  return polar(::exp(z.r), z.i);
}

template <typename T>
inline
constexpr
cx<T> sin(const cx<T> &z) {
  return cx<T>(::sin(z.r)*cosh(z.i), cos(z.r)*sinh(z.i));
}

template <typename T>
inline
constexpr
T real (cx<T> &c) { return c.r; }

template <typename T>
inline
constexpr
T imag(cx<T> &c) { return c.i; }

}; // namespace cxns

#endif // _COMPLEX_HPP_
