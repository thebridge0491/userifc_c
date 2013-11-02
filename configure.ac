#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([Userifc_c], [0.1.0], [thebridge0491-codelab@yahoo.com])
# @PACKAGE_NAME@, @PACKAGE_VERSION@, @PACKAGE_BUGREPORT@
AC_SUBST([PACKAGE_URL], [bitbucket.org/thebridge0491/userifc_c])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([1.12 silent-rules subdir-objects -Wall tar-pax dist-zip foreign])

AC_ARG_ENABLE([debug], [ --enable-debug   Turn on debugging],
[case "${enableval}" in
    yes) debug=true ;;
    no) debug=false ;;
    *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
 esac], [debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR


AC_CONFIG_FILES([Makefile])
AC_CONFIG_SUBDIRS([aux app])
AC_OUTPUT
