AC_PREREQ([2.57])
AC_INIT([xlib], [1.0.0], [brian.molnar@gmail.com])

AC_CONFIG_AUX_DIR([config-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([unistd.h errno.h sys/uio.h sys/types.h sys/stat.h fcntl.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AM_CONDITIONAL([ENABLE_XSTREAM_FILE], [false])
AM_CONDITIONAL([ENABLE_XSTREAM_STDIO], [false])

AC_CONFIG_FILES([Makefile])
#AC_CONFIG_SUBDIRS([])
AC_OUTPUT
