# Single package automake Makefile.am script.
ACLOCAL_AMFLAGS = -I m4

include $(srcdir)/targets.am
pkgincludedir = $(includedir)/$(PACKAGE_PARENT)

pkginclude_HEADERS = include/$(PACKAGE_PARENT)/observer.h include/$(PACKAGE_PARENT)/subject.h
pkgconfig_DATA = src/$(PACKAGE_TARNAME).pc

##lib_LIBRARIES = lib$(PACKAGE_TARNAME).a
lib_LTLIBRARIES = lib$(PACKAGE_TARNAME).la
lib__PACKAGE_TARNAME__la_CFLAGS = $(AM_CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) $(PCRE_CFLAGS) $(GLIB_CFLAGS)
lib__PACKAGE_TARNAME__la_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS) $(PCRE_LIBS) $(GLIB_LIBS) -lintro_c-util -version-number `echo $(PACKAGE_VERSION) | $(SED) 'y|.|:|'`
lib__PACKAGE_TARNAME__la_SOURCES = src/$(PACKAGE_PARENT)/observer.c src/$(PACKAGE_PARENT)/subject.c

check_PROGRAMS = ts_main$(EXEEXT)
TESTS = $(check_PROGRAMS)

ts_main_CFLAGS = $(AM_CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) $(TESTFRWK_CFLAGS) $(GLIB_CFLAGS)
ts_main_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS) $(TESTFRWK_LIBS) -lintro_c-util -l$(PACKAGE_TARNAME) #-l:lib$(PACKAGE_TARNAME).$(SHLIBEXT)
ts_main_SOURCES = tests/ts_main.c tests/tc_new.c tests/tc_new.h
