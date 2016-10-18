/* 
 Main program to calculate at compile-time the fast fourier transform (FFT).
  
 Copyright Paul Keir 2012-2016
 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file license.txt or copy at http://boost.org/LICENSE_1_0.txt)
*/

#include <iostream>
#ifdef RECARR
#include "cfft_recarr.hpp"
#else
#include "cfft.hpp"
#endif

constexpr size_t n =         SZ;

typedef double           fptype;
typedef cxns::cx<fptype> cxtype;

int main(int argc, char *argv[])
{
  constexpr auto si = cxns::sin<fptype>;
  constexpr auto in = Init<cxtype>(n);
  constexpr auto ca = StaticCast<cxtype,size_t>();

  constexpr auto x = map( compose( si, in, ca ), iota<n>() );
  
  std::cout << x << std::endl;

  constexpr auto res = fft(x);

  std::cout << map( in, res ) << std::endl;

  return 0;
}
