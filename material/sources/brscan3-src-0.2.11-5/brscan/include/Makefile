SHELL = /bin/sh


srcdir = .
top_srcdir = ..
top_builddir = .

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

MKDIR = $(top_srcdir)/mkinstalldirs
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644



INSTALLED_INCLUDES = $(addprefix $(top_srcdir)/include/sane/,sane.h \
	saneopts.h)

SANE_INCLUDES = $(INSTALLED_INCLUDES) \
                $(addprefix $(top_srcdir)/include/sane/,config.h.in sanei.h \
                sanei_ab306.h sanei_auth.h sanei_backend.h \
                sanei_codec_ascii.h sanei_codec_bin.h sanei_config.h \
                sanei_debug.h sanei_lm983x.h sanei_net.h sanei_pa4s2.h \
                sanei_pio.h sanei_pv8630.h sanei_scsi.h sanei_thread.h \
                sanei_usb.h sanei_wire.h)

OTHER_INCLUDES = $(addprefix $(top_srcdir)/include/,Makefile.in getopt.h \
                lalloca.h lassert.h md5.h )

DISTFILES = $(SANE_INCLUDES) $(OTHER_INCLUDES)

.PHONY: all clean depend dist distclean install uninstall 

all:

clean:

distclean: clean
	rm -f Makefile
	rm -f *~ sane/*~ sane/config.h

install:
	$(MKDIR) $(includedir)/sane
	@list="$(INSTALLED_INCLUDES)"; for file in $$list; do \
	  echo installing $${file} in $(includedir)/sane...; \
	  $(INSTALL_DATA) $${file} $(includedir)/sane || exit 1; \
	done

uninstall:
	rm -rf $(includedir)/sane

depend:

dist: $(DISTFILES)
	$(MKDIR) $(distdir)/include/sane
	for file in $(OTHER_INCLUDES); do \
	  ln $$file $(distdir)/include 2> /dev/null \
	    || cp -p $$file $(distdir)/include; \
	done
	for file in $(SANE_INCLUDES); do \
	  ln $$file $(distdir)/include/sane 2> /dev/null \
	    || cp -p $$file $(distdir)/include/sane; \
	done
