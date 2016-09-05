AC_PREREQ(2.59)
AC_INIT([uni_t_dmm_lib], [1.0], [mike.ray@btinternet.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PREFIX_DEFAULT(/usr/local)
AC_CONFIG_SRCDIR([src/uni_t_dmm.c])
AC_CONFIG_MACRO_DIR([m4])
eval LIBTOOL_VERSION_INFO="1:0:0"
AC_SUBST(LIBTOOL_VERSION_INFO)

AC_DISABLE_STATIC

dnl Test for programs
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AM_PROG_AR
AM_PROG_LIBTOOL



test "x$prefix" = xNONE && prefix=$ac_default_prefix

dnl Do not build static and shared libraries together
if test "$enable_static" = "$enable_shared" -a "$enable_static" = "yes"; then
cat <<EOF
  Please, do not try to compile static and shared libraries together.
  See INSTALL file for more details (do not use --enable-shared=yes with
  --enable-static=yes).
EOF
  exit 1
fi

AC_CONFIG_HEADERS([src/config.h])

AC_ARG_WITH(pkgconfdir,
    AS_HELP_STRING([--with-pkgconfdir=dir],
	[path where pkgconfig files are stored]),
    pkgconfdir="$withval", pkgconfdir="")
if test -z "$pkgconfdir"; then
    eval dir="$libdir"
    case "$dir" in
    /*) ;;
    *) dir="$dir"
    esac
    pkgconfdir="$dir/pkgconfig"
fi
AC_DEFINE_UNQUOTED(UNITDMM_PKGCONF_DIR, "$pkgconfdir", [directory containing pkgconfig files])
UNITDMM_PKGCONF_DIR="$pkgconfdir"
AC_SUBST(UNITDMM_PKGCONF_DIR)

AC_CONFIG_FILES([
 Makefile
	include/Makefile
 src/Makefile
	utils/Makefile
	utils/uni_t_dmm.pc
])
AC_OUTPUT
