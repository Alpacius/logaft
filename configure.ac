#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(logaft, 0.1, sceneviper@hotmail.com)
AC_CONFIG_SRCDIR([src/logger.c])
AC_CONFIG_MACRO_DIRS([m4])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PROG_AR

LT_INIT

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB(pthread, pthread_create)

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([fcntl.h stddef.h stdint.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([clock_gettime localtime_r memset])

AC_OUTPUT([Makefile src/Makefile])
