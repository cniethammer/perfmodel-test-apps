CXX = mpicxx


.PHONY: clean all

all: amdahlapp

amdahlapp: amdahlapp.cpp cmdline.c cmdline.h

cmdline.c cmdline.h: cmdline.ggo
	gengetopt < $<

clean:
	$(RM) amdahlapp
	$(RM) cmdline.h cmdline.c
	$(RM) *.o
