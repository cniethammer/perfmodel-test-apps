CXX = mpicxx


.PHONY: clean all

all: streamapp

streamapp: streamapp.cpp cmdline.c cmdline.h

cmdline.c cmdline.h: cmdline.ggo
	gengetopt < $<

clean:
	$(RM) streamapp
	$(RM) cmdline.h cmdline.c
	$(RM) *.o
