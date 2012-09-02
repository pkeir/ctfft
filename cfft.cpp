/* 
  Main program to calculate at compile-time the fast fourier transform (FFT).
  Copyright (C) 2012 Paul Keir
  Distributed under the GNU General Public License. See license.txt for details.
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
