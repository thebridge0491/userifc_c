#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([Userifc_c-Aux], [0.1.0], [thebridge0491-codelab@yahoo.com])
# @PACKAGE_NAME@, @PACKAGE_VERSION@, @PACKAGE_BUGREPORT@
AC_SUBST([PACKAGE_URL], [bitbucket.org/thebridge0491/userifc_c])
AC_SUBST([PACKAGE_PARENT], [userifc_c])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([src/config.h])
AM_INIT_AUTOMAKE([1.12 silent-rules subdir-objects -Wall tar-pax dist-zip foreign])

AC_CANONICAL_BUILD
case $build in
	*-darwin*) AC_SUBST([SHLIBEXT], [dylib])
			is_darwin=true ;;
	*) AC_SUBST([SHLIBEXT], [so]) ;;
esac
AM_CONDITIONAL([DARWIN], [test x$is_darwin = xtrue])

AC_ARG_ENABLE([debug], [ --enable-debug   Turn on debugging],
[case "${enableval}" in
    yes) debug=true ;;
    no) debug=false ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
 esac], [debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

# Checks for programs.
AC_PROG_INSTALL
AC_PROG_CC_C_O
AC_PROG_CC
AM_PROG_AR

LT_INIT
AC_SUBST([LIBTOOL_DEPS])

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN"
    then AC_MSG_WARN([Doxygen not found - continuing without Doxygen])
else
    AC_MSG_WARN([Doxygen has been found - available to use])
fi

#AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
#AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES(Doxyfile_c.txt Doxyfile)])

# Checks for libraries.
AC_CHECK_LIB([m], [powf])

# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h stdint.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([clock_gettime gettimeofday memmove regcomp strtol powf])


PKG_CHECK_MODULES([TESTFRWK], [check])
PKG_CHECK_MODULES([PCRE], [libpcre])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([INTRO_CUTIL], [intro_c-util])
AC_CONFIG_SRCDIR([tests/ts_main.c])
#AC_CONFIG_LINKS([resources/log4crc.xml:resources/log4crc.xml])
#AC_CONFIG_FILES([resources/log4crc.xml:resources/log4crc.xml])
AC_CONFIG_FILES([Makefile src/${PACKAGE_TARNAME}.pc])
AC_OUTPUT
