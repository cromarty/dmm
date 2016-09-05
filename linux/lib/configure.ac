AC_INIT([uni_t_dmm_lib], [1.0], [mike.ray@btinternet.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_MACRO_DIR([m4])
eval LIBTOOL_VERSION_INFO="1:0:0"
AC_SUBST(LIBTOOL_VERSION_INFO)
AC_PROG_CC
AM_PROG_AR
AC_PROG_LIBTOOL
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
])
AC_OUTPUT
