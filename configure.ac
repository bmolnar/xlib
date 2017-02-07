AC_PREREQ([2.57])
AC_INIT([xlib], [1.0.0], [brian(dot)molnar(at)gmail(dot)com])

AC_CONFIG_AUX_DIR([config-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])


# Set default CFLAGS/CXXFLAGS
AS_IF([test "x${ac_cv_env_CFLAGS_set}" = "x"],
    [CFLAGS="-Wall -ggdb"])
AS_IF([test "x${ac_cv_env_CXXFLAGS_set}" = "x"],
    [CXXFLAGS="-Wall -ggdb"])


AM_INIT_AUTOMAKE([1.9 tar-ustar subdir-objects])

# Checks for programs.
AC_PROG_CC
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdio.h unistd.h errno.h sys/uio.h sys/types.h sys/stat.h fcntl.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AM_CONDITIONAL([ENABLE_XSTREAM_FILE], [false])
AM_CONDITIONAL([ENABLE_XSTREAM_STDIO], [false])

AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])
#AC_CONFIG_SUBDIRS([])
AC_OUTPUT
