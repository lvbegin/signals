AC_PREREQ([2.69])
AC_INIT([signals], [1.0], [laurent.vanbegin76@gmail.com])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADERS([config.h])
# Checks for programs.
AC_LANG([C++])

# Checks for libraries.
AC_PROG_RANLIB
AM_INIT_AUTOMAKE()
# Checks for header files.
# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CONFIG_FILES([Makefile])
AC_CHECK_LIB(pthread, pthread_sigmask)
AC_OUTPUT
