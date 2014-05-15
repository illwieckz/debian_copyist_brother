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
libsanedir = $(libdir)/sane
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include
configdir = ${sysconfdir}/sane.d

V_MAJOR = 1
V_MINOR = 0
V_REV = 7
DLL_PRELOAD = 
DLL_PRELOAD_EXTRAS = $(foreach be,$(DLL_PRELOAD),$($(addprefix EXTRA_,$(be))))\


MKDIR = $(top_srcdir)/mkinstalldirs
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
RANLIB = ranlib
LN_S = ln -s

CC = gcc
INCLUDES = -I. -I$(srcdir) -I$(top_builddir)/include -I$(top_srcdir)/include\
           -I$(top_srcdir)/libbrscandec -I$(top_srcdir)/libbrcolm

CPPFLAGS =  -D_GNU_SOURCE -DPATH_SANE_CONFIG_DIR=$(configdir) 	  -DPATH_SANE_DATA_DIR=$(datadir) 	  -DV_MAJOR=1 -DV_MINOR=0
CFLAGS =  -O0 -W -Wall
TARGETSUFFIX = -DBRSANESUFFIX=1
#LDFLAGS = 
LDFLAGS = -lpthread
BACKENDLIBS = -lnsl -lusb -lm  -ldl
DEFS = -DHAVE_CONFIG_H

LIBTOOL = ../libtool
MCOMP	= --mode=compile
MLINK	= --mode=link
MINST	= --mode=install

COMPILE = $(CC) -c $(CFLAGS) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(LDFLAGS) $(TARGETSUFFIX)
#COMPILE = $(CC) -c $(CFLAGS) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(LDFLAGS)
#COMPILE = $(CC) -c $(CFLAGS) $(DEFS) $(INCLUDES) $(CPPFLAGS)



PRELOADABLE_BACKENDS = brother
#PRELOADABLE_BACKENDS = abaton agfafocus apple artec as6e avision bh canon \
#        coolscan dc25   dmc epson  hp m3096g \
#	microtek microtek2 mustek mustek_pp mustek_usb nec net pie  \
#	plustek  qcam ricoh s9036 sharp sm3600 snapscan sp15c \
#	st400 tamarack umax umax_pp umax1220u v4l
ALL_BACKENDS = $(PRELOADABLE_BACKENDS) dll

LIBS = $(addprefix libsane-,$(addsuffix .la,$(ALL_BACKENDS)))
BRLIBS = $(addprefix libsane-,$(addsuffix .la,$(PRELOADABLE_BACKENDS)))
CONFIGS = $(addsuffix .conf,$(ALL_BACKENDS)) saned.conf

EXTRA = sane_strstatus.lo ../sanei/sanei_init_debug.lo ../sanei/sanei_config.lo

# With libtool-1.0, we have to mention each library object explicitly... ;-(
LIBLIB_FUNCS = alloca getenv getopt getopt1 isfdtype sigprocmask snprintf \
        strdup strndup strsep usleep vsyslog inet_ntop
LIBOBJS = $(addprefix ../lib/,$(addsuffix .lo,$(LIBLIB_FUNCS)))

DISTFILES = Makefile.in \
  dll.aliases dll.c dll.conf dll.desc sane_strstatus.c stubs.c\
  brother_cmatch.c brother_cmatch.h\
  brother_devaccs.c brother_devaccs.h brother_devinfo.c brother_devinfo.h\
  brother_dtype.h brother_log.c brother_log.h brother_mfccmd.c brother_mfccmd.h\
  brother_mfcinfo.h brother_misc.c brother_misc.h brother_modelinf.c\
  brother_modelinf.h brother_scanner.c brother_scanner.h\
  brother.c brother.h brother_netdev.c brother_netdev.h brother_advini.c brother_advini.h

.PHONY: all clean depend dist distclean install uninstall

libsane-%.la: %.lo %-s.lo $(EXTRA) $(LIBOBJS)  *.c *.h
	@$(LIBTOOL) $(MLINK) $(CC) -export-dynamic -o $@ $($*_LIBS) \
	$(LDFLAGS) $(BACKENDLIBS) $^ -rpath $(libsanedir) \
	-version-info $(V_MAJOR):$(V_REV):$(V_MINOR)

%-s.lo:	%-s.c
	@$(LIBTOOL) $(MCOMP) $(COMPILE) -DSTUBS -DBACKEND_NAME=$* $<

%.lo:	%.c
	@$(LIBTOOL) $(MCOMP) $(COMPILE) -DBACKEND_NAME=$* \
		-DLIBDIR=$(libsanedir) $<

%-s.c:	$(srcdir)/stubs.c
	rm -f $@
	$(LN_S) $(srcdir)/stubs.c $@

# Don't delete any intermediate files.
.PRECIOUS: %-s.c %-s.lo %.lo dll-preload.c

brscan:
	sh mk_backend release

all:	$(LIBS) libsane.la libsane-brother.la

install:
	$(MKDIR) $(libdir) $(libsanedir) $(configdir)
	@list="$(BRLIBS)"; for be in $$list; do \
	  echo installing $${be} in $(libsanedir)...; \
	  $(LIBTOOL) $(MINST) $(INSTALL_PROGRAM) $${be} $(libsanedir)/$${be} \
	    || exit 1; \
	done

uninstall:
	rm -rf $(libsanedir) $(configdir)
	rm -f $(libdir)/libsane.*

dll.lo: dll-preload.c

dll-preload.c:
	rm -f $@
	list="$(DLL_PRELOAD)"; for be in $$list; do \
	  echo "PRELOAD_DECL($$be)" >> $@; \
	done
	echo "static struct backend preloaded_backends[] = {" >> $@
	sep=""; \
	list="$(DLL_PRELOAD)"; \
	if test -z "$${list}"; then \
	  echo { 0 } >> $@; \
	else \
	  for be in $$list; do \
	    echo "$${sep}PRELOAD_DEFN($$be)" >> $@; \
	    sep=","; \
	  done; \
	fi
	echo "};" >> $@

libsane.la: dll.lo dll-s.lo $(EXTRA) $(addsuffix .lo,$(DLL_PRELOAD)) $(LIBOBJS)
	@$(LIBTOOL) $(MLINK) $(CC) -o $@ $(LDFLAGS) $(BACKENDLIBS)  $^ \
		$(addsuffix .lo,$(DLL_PRELOAD_EXTRAS)) \
		-rpath $(libdir) -version-info $(V_MAJOR):$(V_REV):$(V_MINOR)

# additional dependencies

# When preloading dll, we need to add in all preloaded objects:
libsane-dll.la: $(addsuffix .lo,$(DLL_PRELOAD))
libsane-dll.la: $(addsuffix .lo,$(DLL_PRELOAD_EXTRAS))

# We must not build SANE backend libraries that contain unresolved references
# to any of the sanei routines.  These explicit dependencies take care of
# this:

libsane-brother.la: ../sanei/sanei_constrain_value.lo

ifneq ($(DLL_PRELOAD),)
# need to make dll dependent on all sanei files:
libsane-dll.la libsane.la: ../sanei/sanei_config.lo
libsane-dll.la libsane.la: ../sanei/sanei_config2.lo
libsane-dll.la libsane.la: ../sanei/sanei_codec_bin.lo
libsane-dll.la libsane.la: ../sanei/sanei_constrain_value.lo
libsane-dll.la libsane.la: ../sanei/sanei_net.lo
libsane-dll.la libsane.la: ../sanei/sanei_scsi.lo
libsane-dll.la libsane.la: ../sanei/sanei_wire.lo
libsane-dll.la libsane.la: ../sanei/sanei_ab306.lo
libsane-dll.la libsane.la: ../sanei/sanei_pio.lo
libsane-dll.la libsane.la: ../sanei/sanei_pa4s2.lo
libsane-dll.la libsane.la: ../sanei/sanei_usb.lo
libsane-dll.la libsane.la: ../sanei/sanei_pv8630.lo
libsane-dll.la libsane.la: ../sanei/sanei_lm983x.lo
endif

depend:
	makedepend $(INCLUDES) *.c

clean:
	rm -f *.lo *.o *~ *.la libsane.la *.bak dll-preload.c
	find . -type l -name \*-s.c | xargs rm -f
	rm -rf .libs

distclean: clean
	rm -f Makefile libsane.so

dist: $(DISTFILES)
	for file in $(DISTFILES); do \
	  ln $$file $(distdir)/backend 2> /dev/null \
	    || cp -p $$file $(distdir)/backend ; \
	done
