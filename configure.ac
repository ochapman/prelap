#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(prelap, 0.1, ochapman.cn@gmail.com)
AM_INIT_AUTOMAKE([-Wall])
AC_PROG_LIBTOOL
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([strrchr strtol])
AC_CONFIG_FILES([Makefile])

AC_CONFIG_MACRO_DIR([m4])
AC_OUTPUT
