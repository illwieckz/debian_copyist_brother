SHELL = /bin/sh


srcdir = .
top_srcdir = .
top_builddir = .

PACKAGE = sane-backends
VERSION = 1.0.7
distdir = $(PACKAGE)-$(VERSION)

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



SUBDIRS	= backend_brscan  backend_brscan2

DISTFILES = AUTHORS COPYING ChangeLog ChangeLog-1.0.0 ChangeLog-1.0.1 \
  ChangeLog-1.0.2 ChangeLog-1.0.3 ChangeLog-1.0.4 ChangeLog-1.0.5 LEVEL2 \
  LICENSE Makefile.in NEWS \
  PROBLEMS PROJECTS README README.aix README.hp-ux README.linux README.netbsd \
  README.openbsd README.os2 README.solaris README.unixware2 README.unixware7 \
  TODO acinclude.m4 aclocal.m4 config.guess config.sub configure \
  configure.in configure.os2 install-sh ltconfig ltmain.sh mkinstalldirs \
  sane-backends.lsm

.PHONY: all all-recursive clean clean-recursive depend \
  depend-recursive dist install install-recursive libcheck lsm \
  sane-backends sort-cvsignore uninstall uninstall-recursive

all:    brscan  brscan2 

brscan: include\
        backend_src\
        backend \
        sanei\
        lib\
	make_brscan64_configure_date \
        po
	(cd libbrcolm && make)
	(cd libbrscandec && make)
	(cd backend_brscan && make brscan)
	(cd mk_package  && make brscan)

brscan2:include\
        backend_src\
        backend \
        sanei\
        lib\
	make_brscan64_configure_date \
        po
	(cd libbrcolm2 && make)
	(cd libbrscandec2 && make)
	(cd backend_brscan2 && make brscan2)
	(cd mk_package  && make brscan2)

make_brscan64_configure_date:
	ln -s ../brscan/acinclude.m4
	ln -s ../brscan/aclocal.m4
	ln -s ../brscan/config.cache
	ln -s ../brscan/config.guess
	ln -s ../brscan/config.status
	ln -s ../brscan/config.sub
	ln -s ../brscan/configure
	ln -s ../brscan/configure.in
	ln -s ../brscan/install-sh
	ln -s ../brscan/libtool
	ln -s ../brscan/ltconfig
	ln -s ../brscan/ltmain.sh
	ln -s ../brscan/mkinstalldirs
	date  > make_brscan64_configure_date

include:
	ln -s ../brscan/include
backend_src:
	ln -s ../brscan/backend_src
backend:
	ln -s ../brscan/backend_src   backend
sanei: make_brscan64_configure_date
	mkdir sanei
	ln -s ../../brscan/sanei/linux_sg3_err.h      sanei
	ln -s ../../brscan/sanei/os2_srb.h      sanei
	ln -s ../../brscan/sanei/sanei_DomainOS.h      sanei
	ln -s ../../brscan/sanei/sanei_ab306.c     sanei
	ln -s ../../brscan/sanei/sanei_auth.c     sanei
	ln -s ../../brscan/sanei/sanei_codec_ascii.c     sanei
	ln -s ../../brscan/sanei/sanei_codec_bin.c     sanei
	ln -s ../../brscan/sanei/sanei_config2.c     sanei
	ln -s ../../brscan/sanei/sanei_config.c     sanei
	ln -s ../../brscan/sanei/sanei_constrain_value.c     sanei
	ln -s ../../brscan/sanei/sanei_DomainOS.c     sanei
	ln -s ../../brscan/sanei/sanei_init_debug.c     sanei
	ln -s ../../brscan/sanei/sanei_lm983x.c     sanei
	ln -s ../../brscan/sanei/sanei_load_values.c     sanei
	ln -s ../../brscan/sanei/sanei_net.c     sanei
	ln -s ../../brscan/sanei/sanei_pa4s2.c     sanei
	ln -s ../../brscan/sanei/sanei_pio.c     sanei
	ln -s ../../brscan/sanei/sanei_pv8630.c     sanei
	ln -s ../../brscan/sanei/sanei_save_values.c     sanei
	ln -s ../../brscan/sanei/sanei_scsi.c     sanei
	ln -s ../../brscan/sanei/sanei_thread.c     sanei
	ln -s ../../brscan/sanei/sanei_usb.c     sanei
	ln -s ../../brscan/sanei/sanei_wire.c     sanei
	ln -s ../../brscan/sanei/test_wire.c     sanei
	ln -s ../../brscan/sanei/Makefile     sanei
	ln -s ../../brscan/sanei/Makefile.in     sanei
	(cd sanei && make)

po:
	ln -s ../brscan/po

lib: make_brscan64_configure_date
	(mkdir lib && \
	cd    lib && \
	ln -s   ../../brscan/lib/*.c  .      && \
	ln -s   ../../brscan/lib/Makefile    && \
	ln -s   ../../brscan/lib/Makefile.in && \
	make)


clean: clean-recursive
	(cd libbrcolm && make clean)
	(cd libbrscandec && make clean)
	(cd backend_brscan && make clean)
	(cd libbrcolm2 && make clean)
	(cd libbrscandec2 && make clean)
	(cd backend_brscan2 && make clean)
	(cd mk_package  && make clean)


	rm -fR include
	rm -fR backend_src
	rm -fR backend
	rm -fR sanei
	rm -fR po
	rm -fR lib
	
	rm -f acinclude.m4
	rm -f aclocal.m4
	rm -f config.cache
	rm -f config.guess
	rm -f config.status
	rm -f config.sub
	rm -f configure
	rm -f configure.in
	rm -f install-sh
	rm -f libtool
	rm -f ltconfig
	rm -f ltmain.sh
	rm -f mkinstalldirs

	rm -f make_brscan64_configure_date
	rm config.log

distclean: clean distclean-recursive
	rm -f *~ *.log *.bak libtool
	rm -f Makefile config.cache config.status
	rm -rf $(distdir)

depend: depend-recursive

rpm-install: rpm-install1 rpm-install2

rpm-install1:
	rpm -ihv  mk_package/linux*/brscan-*.rpm

rpm-install2:
	rpm -ihv  mk_package/linux*/brscan2-*.rpm

rpm-uninstall: rpm-uninstall1 rpm-uninstall2

rpm-uninstall1:
	rpm -e brscan-*

rpm-uninstall2:
	rpm -e brscan2-*

all-recursive install-recursive clean-recursive distclean-recursive \
depend-recursive uninstall-recursive:
	@for subdir in $(SUBDIRS); do		\
	  target=`echo $@ | sed s/-recursive//`; \
	  echo making $$target in $$subdir;	\
	  (cd $$subdir && $(MAKE) $$target)	\
	   || case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

dist: $(DISTFILES)
	rm -fr $(distdir)
	$(MKDIR) $(distdir)
	for file in $(DISTFILES); do \
	  ln $$file $(distdir) 2>/dev/null || cp -p $$file $(distdir); \
	done
#	for subdir in $(SUBDIRS) japi testsuite ; do \
#	  mkdir $(distdir)/$$subdir || exit 1; \
#	  chmod 777 $(distdir)/$$subdir; \
#	  (cd $$subdir && $(MAKE) $@) || exit 1; \
#	done
	tar chzf $(distdir).tar.gz $(distdir)
	rm -fr $(distdir)

