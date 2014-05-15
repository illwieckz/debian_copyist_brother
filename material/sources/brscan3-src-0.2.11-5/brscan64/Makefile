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



SUBDIRS	= backend_brscan3

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

all:    brscan3

brscan3: include\
        backend_src\
        backend \
        lib\
	make_brscan64_configure_date \
        po
	(cd backend_brscan3 && make )
	(cd netconfig3 && sh mk_brsaneconfig3 3)
	(cd mk_package3/G8R2  && make )


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
	(cd backend_brscan3 && make clean)
	(cd mk_package  && make clean)


	rm -fR include
	rm -fR backend_src
	rm -fR backend
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

