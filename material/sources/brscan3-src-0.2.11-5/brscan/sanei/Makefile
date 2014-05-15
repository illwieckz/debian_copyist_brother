SHELL = /bin/sh


srcdir = .
top_srcdir = ..
top_builddir = ..

PACKAGE = sane-backends
VERSION = 1.0.7
distdir = $(top_srcdir)/$(PACKAGE)-$(VERSION)

prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include
configdir = ${sysconfdir}/sane.d

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
RANLIB = ranlib

CC = gcc
INCLUDES = -I. -I$(srcdir) -I$(top_builddir)/include -I$(top_srcdir)/include
CPPFLAGS =  -D_GNU_SOURCE -DPATH_SANE_CONFIG_DIR=$(configdir) 	  -DPATH_SANE_DATA_DIR=$(datadir) 	  -DV_MAJOR=1 -DV_MINOR=0
CFLAGS = -g -O2 -W -Wall
LDFLAGS = 
LIBS = -lnsl -lusb -lm  -ljpeg
DEFS = -DHAVE_CONFIG_H

 COMPILE = $(CC) -c $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LIBTOOL = ../libtool
MCOMP	= --mode=compile
MLINK	= --mode=link



LIBSANEI_OBJS = sanei_ab306.o sanei_constrain_value.o sanei_init_debug.o \
	sanei_net.o sanei_wire.o sanei_codec_ascii.o sanei_codec_bin.o \
	sanei_save_values.o sanei_load_values.o \
	sanei_scsi.o sanei_config.o sanei_config2.o sanei_pio.o sanei_pa4s2.o \
	sanei_auth.o sanei_usb.o sanei_thread.o sanei_pv8630.o sanei_lm983x.o

LIBSANEI_LTOBJS = sanei_ab306.lo sanei_constrain_value.lo sanei_init_debug.lo \
	sanei_net.lo sanei_wire.lo sanei_codec_ascii.lo sanei_codec_bin.lo \
	sanei_save_values.lo sanei_load_values.lo \
	sanei_scsi.lo sanei_config.lo sanei_config2.lo sanei_pio.lo \
	sanei_pa4s2.lo sanei_auth.lo sanei_usb.lo sanei_thread.lo \
	sanei_pv8630.lo sanei_lm983x.lo

TARGETS = libsanei.a
TESTPROGRAMS = test_wire

DISTFILES = Makefile.in linux_sg3_err.h os2_srb.h sanei_DomainOS.c \
  sanei_DomainOS.h sanei_ab306.c sanei_auth.c sanei_codec_ascii.c \
  sanei_codec_bin.c sanei_config.c sanei_config2.c \
  sanei_constrain_value.c sanei_init_debug.c sanei_load_values.c \
  sanei_net.c sanei_pa4s2.c sanei_pio.c sanei_pv8630.c sanei_lm983x.c \
  sanei_save_values.c sanei_scsi.c sanei_thread.c sanei_usb.c \
  sanei_wire.c test_wire.c

.PHONY: all check clean depend dist distclean install uninstall

.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
	@$(LIBTOOL) $(MCOMP) $(COMPILE) $<
	@test -f $@ || $(COMPILE) $<

all:	$(TARGETS)

libsanei.a: $(LIBSANEI_OBJS)
	ar r $@ $(LIBSANEI_OBJS)
	$(RANLIB) $@

install:

uninstall:

check: test_wire
	./test_wire

test_wire: test_wire.o libsanei.a
	@$(LIBTOOL) $(MLINK) $(CC) -o $@ test_wire.o $(LDFLAGS) libsanei.a

test_wire.o: test_wire.c
	$(COMPILE) $<

depend:
	makedepend -I. -I../include *.c

clean:
	rm -f *.out *.o *.lo *~ *.a *.bak $(OBJS) $(TESTPROGRAMS)
	rm -rf .libs

distclean: clean
	rm -f Makefile

dist: $(DISTFILES)
	for file in $(DISTFILES); do \
	  ln $$file $(distdir)/sanei 2> /dev/null \
	    || cp -p $$file $(distdir)/sanei ; \
	done

