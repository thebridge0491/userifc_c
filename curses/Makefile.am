# Single package automake Makefile.am script.
ACLOCAL_AMFLAGS = -I m4

include $(srcdir)/targets.am
pkgincludedir = $(includedir)/$(PACKAGE_PARENT)

pkginclude_HEADERS = include/$(PACKAGE_PARENT)/curseshello_model.h include/$(PACKAGE_PARENT)/curseshello_view.h include/$(PACKAGE_PARENT)/curseshello_controller.h
pkgconfig_DATA = src/$(PACKAGE_TARNAME).pc

##lib_LIBRARIES = lib$(PACKAGE_TARNAME).a
lib_LTLIBRARIES = lib$(PACKAGE_TARNAME).la
lib__PACKAGE_TARNAME__la_CFLAGS = $(AM_CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) $(GLIB_CFLAGS) $(INTRO_CUTIL_CFLAGS) $(USERIFC_CAUX_CFLAGS) $(NCURSES_CFLAGS)
lib__PACKAGE_TARNAME__la_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS) $(GLIB_LIBS) $(INTRO_CUTIL_LIBS) $(USERIFC_CAUX_LIBS) $(NCURSES_LIBS) -lncurses -lpanel -version-number `echo $(PACKAGE_VERSION) | $(SED) 'y|.|:|'`
lib__PACKAGE_TARNAME__la_SOURCES = src/$(PACKAGE_PARENT)/curseshello_model.c src/$(PACKAGE_PARENT)/curseshello_view.c src/$(PACKAGE_PARENT)/curseshello_controller.c

check_PROGRAMS = ts_main$(EXEEXT)
TESTS = $(check_PROGRAMS)

ts_main_CFLAGS = $(AM_CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) $(PCRE_CFLAGS) $(TESTFRWK_CFLAGS) $(GLIB_CFLAGS) $(INTRO_CUTIL_CFLAGS) $(USERIFC_CAUX_CFLAGS) $(NCURSES_CFLAGS)
ts_main_LDFLAGS = $(AM_LDFLAGS) $(LDFLAGS) $(PCRE_LIBS) $(TESTFRWK_LIBS) $(GLIB_LIBS) $(INTRO_CUTIL_LIBS) $(USERIFC_CAUX_LIBS) $(NCURSES_LIBS) -lncurses -lpanel -l$(PACKAGE_TARNAME) #-l:lib$(PACKAGE_TARNAME).$(SHLIBEXT)
ts_main_SOURCES = tests/ts_main.c tests/tc_viewcontroller.c
