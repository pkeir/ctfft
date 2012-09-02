GXX=g++-4.7
CLANGXX=clang++
FORTRAN=gfortran-4.7
CFLAGS=-std=c++11
FFLAGS=-cpp
FFTSIZE=8
CM=-DCONSTMATH
CT=-DCONSTTUP

all: sac_fft cfft_gxx_recarr cfft_gxx_recarr_cm cfft_clangxx_recarr_cm \
     cfft_gxx cfft_gxx_cm cfft_gxx_ct cfft_gxx_cm_ct cfft_clangxx_cm_ct

sac_fft: sac_fft.f95
	$(FORTRAN) $(FFLAGS) sac_fft.f95 -DSZ=$(FFTSIZE) -o $@

cfft_gxx: cfft.cpp
	    $(GXX) $(CFLAGS) cfft.cpp -DSZ=$(FFTSIZE) -o $@

cfft_gxx_cm: cfft.cpp
	    $(GXX) $(CFLAGS) cfft.cpp -DSZ=$(FFTSIZE) $(CM) -o $@

cfft_gxx_ct: cfft.cpp
	    $(GXX) $(CFLAGS) cfft.cpp -DSZ=$(FFTSIZE) $(CT) -o $@

cfft_gxx_cm_ct: cfft.cpp
	    $(GXX) $(CFLAGS) cfft.cpp -DSZ=$(FFTSIZE) $(CM) $(CT) -o $@

cfft_clangxx_cm_ct: cfft.cpp
	$(CLANGXX) $(CFLAGS) cfft.cpp -DSZ=$(FFTSIZE) $(CM) $(CT) -o $@

cfft_gxx_recarr: cfft.cpp
	    $(GXX) $(CFLAGS) cfft.cpp -DSZ=$(FFTSIZE) -DRECARR -o $@

cfft_gxx_recarr_cm: cfft.cpp
	    $(GXX) $(CFLAGS) cfft.cpp -DSZ=$(FFTSIZE) -DRECARR $(CM) -o $@

cfft_clangxx_recarr_cm: cfft.cpp
	$(CLANGXX) $(CFLAGS) cfft.cpp -DSZ=$(FFTSIZE) -DRECARR $(CM) -o $@

test: sac_fft cfft_gxx_cm_ct cfft_clangxx_cm_ct cfft_gxx_recarr_cm cfft_clangxx_recarr_cm
	rm -f /tmp/cfft_ref_out /tmp/cfft_gxx_out /tmp/cfft_clangxx_out
	./sac_fft                > /tmp/cfft_ref_out
	./cfft_gxx_cm_ct         > /tmp/cfft_gxx_out
	./cfft_clangxx_cm_ct     > /tmp/cfft_clangxx_out
	./cfft_gxx_recarr_cm     > /tmp/cfft_gxx_recarr_out
	./cfft_clangxx_recarr_cm > /tmp/cfft_clangxx_recarr_out
	numdiff -s ",() \t\n" -a 0.001 /tmp/cfft_ref_out /tmp/cfft_gxx_out
	numdiff -s ",() \t\n" -a 0.001 /tmp/cfft_ref_out /tmp/cfft_clangxx_out
	numdiff -s ",() \t\n" -a 0.001 /tmp/cfft_ref_out /tmp/cfft_gxx_recarr_out
	numdiff -s ",() \t\n" -a 0.001 /tmp/cfft_ref_out /tmp/cfft_clangxx_recarr_out

clean:
	rm -rf *o *.mod
	rm -rf sac_fft cfft_gxx cfft_gxx_cm cfft_gxx_ct cfft_gxx_cm_ct \
         cfft_clangxx_cm_ct cfft_gxx_recarr cfft_gxx_recarr_cm \
         cfft_clangxx_recarr_cm
