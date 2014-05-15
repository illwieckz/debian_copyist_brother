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
INCLUDES = -I. -I$(srcdir) \
	-I$(top_builddir)/include/sane -I$(top_srcdir)/include
CPPFLAGS =  -D_GNU_SOURCE -DPATH_SANE_CONFIG_DIR=$(configdir) 	  -DPATH_SANE_DATA_DIR=$(datadir) 	  -DV_MAJOR=1 -DV_MINOR=0
CFLAGS = -g -O2 -W -Wall
LDFLAGS = 
LIBS = -lnsl -lusb -lm  -ljpeg
DEFS = -DHAVE_CONFIG_H

COMPILE = $(CC) -c $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LIBTOOL = ../libtool
MCOMP	= --mode=compile
MLINK	= --mode=link



LIBLIB_OBJS     = alloca.o getopt.o getopt1.o sigprocmask.o strndup.o \
                  strdup.o strsep.o snprintf.o usleep.o getenv.o \
                  isfdtype.o vsyslog.o md5.o inet_ntop.o
LIBLIB_LTOBJS   = alloca.lo getopt.lo getopt1.lo sigprocmask.lo strndup.lo \
                  strdup.lo strsep.lo snprintf.lo usleep.lo getenv.lo \
                  isfdtype.lo vsyslog.lo md5.lo inet_ntop.lo

TARGETS = $(LIBLIB_OBJS)
TARGETS = liblib.a

DISTFILES = Makefile.in alloca.c getenv.c getopt.c getopt1.c \
  inet_ntop.c isfdtype.c md5.c sigprocmask.c snprintf.c strdup.c \
  strndup.c strsep.c usleep.c vsyslog.c

.PHONY: all check clean depend dist distclean install uninstall

.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
	@$(LIBTOOL) $(MCOMP) $(COMPILE) $<
	@test -f $@ || $(COMPILE) $<

all:	$(TARGETS)

liblib.a: $(LIBLIB_OBJS)
	ar r $@ $(LIBLIB_OBJS)
	$(RANLIB) $@

install:

uninstall:

check:

depend:
	makedepend -I. -I../include *.c

clean:
	rm -f *.out *.o *.lo *~ *.a *.bak $(TESTPROGRAMS)
	rm -rf .libs

distclean: clean
	rm -f Makefile

dist: $(DISTFILES)
	for file in $(DISTFILES); do \
	  ln $$file $(distdir)/lib 2> /dev/null \
	    || cp -p $$file $(distdir)/lib ; \
	done

