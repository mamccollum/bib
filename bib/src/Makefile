#	@(#)Makefile	2.8	6/7/93
#
# Modernized for ANSI C and contemporary Unix systems
# Requires: gcc or clang with POSIX extensions
#

CC = cc
CFLAGS = -DBASEDIR=\"$(DEST_BASE)\" -DINCORE -std=c99 -D__EXTENSIONS__ -Wno-int-conversion -Wno-unused-but-set-variable -Wno-unused-variable -Wno-unused-parameter -Wno-endif-labels -Wno-return-type -Wno-implicit-int -Wno-char-subscripts -Wno-parentheses
LDFLAGS =

.c.o:
	$(CC) $(CFLAGS) -c $*.c

INVo        = invert.o streams.o  makekey.o
INVc        = invert.c streams.c  makekey.c
LOOKo       = lookup.o locate.o alpha.seek.o streams.o makekey.o
LOOKc       = lookup.c locate.c alpha.seek.c streams.c makekey.c
BIBo        = bib.o bibargs.o locate.o alpha.seek.o streams.o makekey.o
BIBc        = bib.c bibargs.c locate.c alpha.seek.c streams.c makekey.c
LISTo       = listrefs.o bibargs.o streams.o makekey.o
LISTc       = listrefs.c bibargs.c streams.c makekey.c
BIBINCo	    = bibinc.o
BIBINCc     = bibinc.c

SRCS = \
	streams.h \
	bib.h \
	bib.c \
	bibargs.c \
	bibinc.c \
	alpha.seek.c \
	invert.c \
	listrefs.c \
	locate.c \
	lookup.c \
	makekey.c \
	streams.c \
	bib2tib

all:	invert lookup listrefs bib bibinc

install: all
	install -s -m 755 bib $(DEST_BIN)/bib
	install -s -m 755 invert $(DEST_BIN)/invert
	install -s -m 755 lookup $(DEST_BIN)/lookup
	install -s -m 755 listrefs $(DEST_BIN)/listrefs
	install -s -m 755 bibinc $(DEST_BIN)/bibinc
	install -m 755 bib2tib $(DEST_BIN)/bib2tib

invert:  $(INVo)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INVo) -o invert

invert.lint:  $(INVc) bib.h streams.h
	lint  $(INVc)

lookup: $(LOOKo)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LOOKo) -o lookup

lookup.lint: $(LOOKc) bib.h streams.h
	lint $(LOOKc)

bibinc: $(BIBINCc) streams.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(BIBINCc) -o bibinc

bib: $(BIBo)
	$(CC) $(CFLAGS) $(LDFLAGS) $(BIBo) -o bib

bib.lint: $(BIBc) bib.h streams.h
	lint $(BIBc)

listrefs: $(LISTo)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LISTo) -o listrefs

bib.o:		bib.h
bibargs.o:	bib.h
listrefs.o:	bib.h
invert.o:       streams.h bib.h

locate.o:       streams.h bib.h
makekey.o:	bib.h
alpha.seek.o:   streams.h
streams.o:      streams.h bib.h
bibinc.o:	streams.h

$(SRCS) :
	sccs get $@

clean:	
	-rm -f *.o bib invert listrefs lookup tags bibinc

depend:
	$(CC) -MM $(CFLAGS) $(BIBc) $(INVc) $(LOOKc) $(LISTc) $(BIBINCc) > .depend

#-include .depend

