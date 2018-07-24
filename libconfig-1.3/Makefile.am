
lib_LTLIBRARIES = libconfig.la

if BUILDCXX
lib_LTLIBRARIES += libconfig++.la
endif

# Versioning rules ( C:R:A )
# 1. Start with version 0:0:0.
# 2. If the library source code has changed at all, incremement R.
# 3. If any interfaces have been added, removed, or changed, increment C and
#    set R to 0.
# 4. If any interfaces have been added, increment A.
# 5. If any interfaces have been removed, set A to 0.
# For more info see page 27 of the GNU Libtool Manual.

VERINFO = -version-info 6:0:0

PARSER_PREFIX = libconfig_yy

libconfig_la_LDFLAGS = $(VERINFO) -no-undefined

libconfig___la_LDFLAGS = $(VERINFO) -no-undefined

libsrc = libconfig.c scanner.l grammar.y private.h wincompat.h
libinc = libconfig.h

libsrc_cpp =  $(libsrc) libconfigcpp.c++
libinc_cpp =  $(libinc) libconfig.h++

BUILT_SOURCES = scanner.c scanner.h grammar.c grammar.h

libconfig_la_SOURCES = $(libsrc)

libconfig___la_SOURCES = $(libsrc_cpp)

libcppflags = -D_REENTRANT

if GNU_WIN
libcppflags += -DLIBCONFIG_EXPORTS
endif

libconfig_la_CPPFLAGS = -DPARSER_PREFIX=\"$(PARSER_PREFIX)\" $(libcppflags)
libconfig___la_CPPFLAGS = -DPARSER_PREFIX=\"$(PARSER_PREFIX)\" $(libcppflags)

include_HEADERS = $(libinc)

if BUILDCXX
include_HEADERS += $(libinc_cpp)
endif

AM_YFLAGS = -d -p $(PARSER_PREFIX)

AM_LFLAGS = --header-file=scanner.h --prefix=$(PARSER_PREFIX)

EXTRA_DIST = \
	$(BUILT_SOURCES) \
	libconfig++.vcproj \
	libconfig++_stub.vcproj \
	libconfig.vcproj \
	libconfig_stub.vcproj \
	libconfig.sln \
	test.cfg \
	TODO \
	debian/changelog \
	debian/compat \
	debian/control \
	debian/copyright \
	debian/libconfigduo2-dev.docs \
	debian/libconfigduo2-dev.install \
	debian/libconfigduo2.install \
	debian/libconfigduo2.postinst \
	debian/shlibs \
	debian/rules \
	libconfig.spec \
	libconfigcpp.cc libconfig.hh

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = libconfig.pc

if BUILDCXX
pkgconfig_DATA += libconfig++.pc
endif

SUBDIRS = . samples doc

.PHONY: dist-rpm

dist-rpm: distcheck
	rpmbuild -ta $(distdir).tar.gz


msvc7:
	find . -name '*\.vcproj' | grep -v '7' \
	| while read x; do \
		d=`dirname $$x`; \
		y=`basename $$x .vcproj`-msvc7.vcproj; \
		rm -f $$d/$$y; \
		perl -p -e 's/Version="8.00"/Version="7.10"/g' < $$x > $$d/$$y; \
	done; \
	rm -f libconfig-msvc7.sln libconfig-msvc7.sln.tmp; \
	perl -p -e 's/Version 9.00/Version 8.00/g' < libconfig.sln \
		> libconfig-msvc7.sln.tmp; \
	perl -p -e 's/\.vcproj/-msvc7.vcproj/g' \
		< libconfig-msvc7.sln.tmp > libconfig-msvc7.sln; \
	rm -f libconfig-msvc7.sln.tmp
