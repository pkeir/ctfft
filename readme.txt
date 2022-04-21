This project implements the fast fourier transform (FFT) at compile-time
in C++. C++11 features, expecially generalised constant expressions
(constexpr) are used extensively.

Should your version of g++, clang++, or gfortran have an alternative name, the
GXX, CLANGXX, and FORTRAN variables may be modified at the top of the Makefile.

The size of the FFT may too be altered in the Makefile using FFTSIZE.
Note that large values here may require additional use of both the
-ftemplate-depth and -fconstexpr-depth compiler flags.

To build under Linux or Cygwin, simply type:

make

This will build 8 C++ executables (cfft_clangxx_cm_ct, cfft_gxx_ct,
cfft_clangxx_recarr_cm, cfft_gxx_recarr, cfft_gxx_recarr_cm, cfft_gxx,
cfft_gxx_cm and cfft_gxx_cm_ct), and one Fortran reference version (sac_fft).

Assuming the numdiff program is installed, the output of the C++
executables can be tested, by comparing against the Fortran reference with:

make test

Individual projects can be built using their names:

make cfft_gxx_cm_ct
./cfft_gxx_cm_ct

A blog post introducing the project can be found here:
https://pkeir.github.io/blog/2012/09/02/compile-time-fft
