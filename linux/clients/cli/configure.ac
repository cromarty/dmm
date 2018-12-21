AC_PREREQ([2.69])
AC_INIT([ut60a], [1.0.0], [mike.ray@btinternet.com])
AM_INIT_AUTOMAKE
AC_PREFIX_DEFAULT(/usr/local)
AC_CONFIG_SRCDIR([src/ut60a.c])
AC_CONFIG_HEADERS([config.h])

dnl Checks for programs.

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_SED
AC_PROG_MAKE_SET

dnl Checks for libraries.

AC_CHECK_LIB([espeak], [espeak_Initialize], [], [AC_MSG_ERROR([espeak_Initialize was not found in libespeak])])
AC_CHECK_LIB([uni_t_dmm], [dmm_set_callback], [], [AC_MSG_ERROR([dmm_set_callback was not found in libuni_t_dmm])])
AC_CHECK_LIB([pthread], [pthread_mutex_init], [], [AC_MSG_ERROR([pthread_mutex_init was not found in libpthread])])

dnl Checks for header files.

AC_CHECK_HEADERS([fcntl.h stddef.h stdlib.h string.h unistd.h termios.h])

dnl Checks for typedefs, structures, and compiler characteristics.

AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

dnl Checks for library functions.

AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove memset select strdup strrchr])

AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
