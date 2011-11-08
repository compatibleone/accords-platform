dnl configure.in for CompatibleOne ACCORDS components.
dnl Copyright 2011 Jean Parpaillon <jparpaillon@mandriva.com>
dnl
dnl Process this file with autoconf to produce a configure script.

dnl autoconf version
AC_PREREQ(2.59)

dnl mandatory stuff
AC_INIT([accords], [1.0], [compatible-accords@ow2.org])

dnl check host and target
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([-Wno-portability])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES(yes)])
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AC_PROG_LN_S
AC_PROG_INSTALL
AC_HEADER_STDC

# Some global compilation flags
# http://www.gnu.org/software/autoconf/manual/automake.html#Flag-Variables-Ordering
AC_SUBST([CFLAGS])
AC_SUBST([CPPFLAGS])
AC_SUBST([LDFLAGS])

AC_ARG_ENABLE([debug],
      [AS_HELP_STRING([--enable-debug],
		      [Enable debug @<:@default=disable@:>@])],
      [],
      [enable_debug=no])
if test "x$enable_debug" = "xyes"; then
   AC_SUBST([CFLAGS], [-g -ggdb])
fi

AC_CHECK_SSL

AC_CHECK_HEADERS_ONCE([uuid/uuid.h uuid.h],
                [break],
		[AC_MSG_ERROR([You're missing uuid development files])])
AC_SUBST([CFLAGS_UUID], [`pkg-config --cflags uuid`])
AC_SUBST([LDFLAGS_UUID], [`pkg-config --libs uuid`])

# Checks for pthread.
AC_CHECK_HEADERS([pthread.h], [
    AC_CHECK_LIB(pthread, pthread_create, [
    LIBS="$LIBS -lpthread"
    ], AC_MSG_ERROR(The pthread library is required in order to build the program!))
], AC_MSG_ERROR(The pthread header is required in order to build the program!))

AC_DEFINE([UNIX])
AC_DEFINE([unix])
AC_DEFINE([CONE])

###
### Modules configuration
###
AC_ARG_ENABLE([coxml],
      [AS_HELP_STRING([--disable-coxml],
		      [Disable coxml @<:@default=enable@:>@])],
      [],
      [enable_coxml=yes])
AM_CONDITIONAL([ENABLE_COXML], [test "$enable_coxml" = "yes"])
AC_SUBST([CFLAGS_COXML], [-I\${abs_top_srcdir}/coxml/src])
AC_SUBST([LD_COXML], [\${abs_top_srcdir}/coxml/src/libcoxml.la])

AC_ARG_ENABLE([occi],
      [AS_HELP_STRING([--disable-occi],
		      [Disable occi @<:@default=enable@:>@])],
      [],
      [enable_occi=yes])
AM_CONDITIONAL([ENABLE_OCCI], [test "$enable_occi" = "yes"])
AC_SUBST([CFLAGS_OCCI], [-I\${abs_top_srcdir}/occi/src])
AC_SUBST([LD_OCCI], [\${abs_top_srcdir}/occi/src/libocci.la])

AC_ARG_ENABLE([cords],
      [AS_HELP_STRING([--disable-cords],
		      [Disable cords @<:@default=enable@:>@])],
      [],
      [enable_cords=yes])
AM_CONDITIONAL([ENABLE_CORDS], [test "$enable_cords" = "yes"])
AC_SUBST([CFLAGS_CORDS], [-I\${abs_top_srcdir}/cords/src])
AC_SUBST([LD_CORDS], [\${abs_top_srcdir}/cords/src/libcords.la])

AC_ARG_ENABLE([corest],
      [AS_HELP_STRING([--disable-corest],
		      [Disable corest @<:@default=enable@:>@])],
      [],
      [enable_corest=yes])
AM_CONDITIONAL([ENABLE_COREST], [test "$enable_corest" = "yes"])
AC_SUBST([CFLAGS_COREST], [-I\${abs_top_srcdir}/corest/src])
AC_SUBST([LD_COREST], [\${abs_top_srcdir}/corest/src/libcorest.la])

AC_ARG_ENABLE([cocci],
      [AS_HELP_STRING([--disable-cocci],
		      [Disable cocci @<:@default=enable@:>@])],
      [],
      [enable_cocci=yes])
AM_CONDITIONAL([ENABLE_COCCI], [test "$enable_cocci" = "yes"])
AC_SUBST([CFLAGS_COCCI], [-I\${abs_top_srcdir}/cocci/src])
AC_SUBST([LD_COCCI], [\${abs_top_srcdir}/cocci/src/libcocci.la])

AC_ARG_ENABLE([cocarrier],
      [AS_HELP_STRING([--disable-cocarrier],
		      [Disable cocarrier @<:@default=enable@:>@])],
      [],
      [enable_cocarrier=yes])
AM_CONDITIONAL([ENABLE_COCARRIER], [test "$enable_cocarrier" = "yes"])
AC_SUBST([CFLAGS_COCARRIER], [-I\${abs_top_srcdir}/cocarrier/src])
AC_SUBST([LD_COCARRIER], [\${abs_top_srcdir}/cocarrier/src/libcocarrier.la])

AC_ARG_ENABLE([pubocci],
      [AS_HELP_STRING([--disable-pubocci],
		      [Disable pubocci @<:@default=enable@:>@])],
      [],
      [enable_pubocci=yes])
AM_CONDITIONAL([ENABLE_PUBOCCI], [test "$enable_pubocci" = "yes"])
AC_SUBST([CFLAGS_PUBOCCI], [-I\${abs_top_srcdir}/pubocci/src])
AC_SUBST([LD_PUBOCCI], [\${abs_top_srcdir}/pubocci/src/libpubocci.la])

AC_ARG_ENABLE([copabr],
      [AS_HELP_STRING([--disable-copabr],
		      [Disable copabr @<:@default=enable@:>@])],
      [],
      [enable_copabr=yes])
AM_CONDITIONAL([ENABLE_COPABR], [test "$enable_copabr" = "yes"])
AC_SUBST([CFLAGS_COPABR], [-I\${abs_top_srcdir}/copabr/src])
AC_SUBST([LD_COPABR], [\${abs_top_srcdir}/copabr/src/libcopabr.la])

AC_ARG_ENABLE([coss],
      [AS_HELP_STRING([--disable-coss],
		      [Disable coss @<:@default=enable@:>@])],
      [],
      [enable_coss=yes])
AM_CONDITIONAL([ENABLE_COSS], [test "$enable_coss" = "yes"])
AC_SUBST([CFLAGS_COSS], [-I\${abs_top_srcdir}/coss/src])

AC_ARG_ENABLE([coes],
      [AS_HELP_STRING([--disable-coes],
		      [Disable coes @<:@default=enable@:>@])],
      [],
      [enable_coes=yes])
AM_CONDITIONAL([ENABLE_COES], [test "$enable_coes" = "yes"])

enable_coobas=no
AM_CONDITIONAL([ENABLE_COOBAS], [test "$enable_coobas" = "yes"])

enable_comons=no
AM_CONDITIONAL([ENABLE_COMONS], [test "$enable_comons" = "yes"])

enable_conets=no
AM_CONDITIONAL([ENABLE_CONETS], [test "$enable_conets" = "yes"])

enable_publisher=no
AM_CONDITIONAL([ENABLE_PUBLISHER], [test "$enable_publisher" = "yes"])

enable_parser=no
AM_CONDITIONAL([ENABLE_PARSER], [test "$enable_parser" = "yes"])
AC_SUBST([CFLAGS_PARSER], [-I\${abs_top_srcdir}/parser/src])

enable_broker=no
AM_CONDITIONAL([ENABLE_BROKER], [test "$enable_broker" = "yes"])
AC_SUBST([CFLAGS_BROKER], [-I\${abs_top_srcdir}/broker/src])

enable_procci=no
AM_CONDITIONAL([ENABLE_PROCCI], [test "$enable_procci" = "yes"])
AC_SUBST([CFLAGS_PROCCI], [-I\${abs_top_srcdir}/procci/src])

enable_osprocci=no
AM_CONDITIONAL([ENABLE_OSPROCCI], [test "$enable_osprocci" = "yes"])

enable_onprocci=no
AM_CONDITIONAL([ENABLE_ONPROCCI], [test "$enable_onprocci" = "yes"])

enable_azprocci=no
AM_CONDITIONAL([ENABLE_AZPROCCI], [test "$enable_azprocci" = "yes"])

enable_testos=no
AM_CONDITIONAL([ENABLE_TESTOS], [test "$enable_testos" = "yes"])

enable_teston=no
AM_CONDITIONAL([ENABLE_TESTON], [test "$enable_teston" = "yes"])

enable_testcp=no
AM_CONDITIONAL([ENABLE_TESTCP], [test "$enable_testcp" = "yes"])

enable_testcb=no
AM_CONDITIONAL([ENABLE_TESTCB], [test "$enable_testcb" = "yes"])

enable_testaz=no
AM_CONDITIONAL([ENABLE_TESTAZ], [test "$enable_testaz" = "yes"])

enable_fileserver=no
AM_CONDITIONAL([ENABLE_FILESERVER], [test "$enable_fileserver" = "yes"])

enable_testresolver=no
AM_CONDITIONAL([ENABLE_TESTRESOLVER], [test "$enable_testresolver" = "yes"])

dnl files to generate with automake
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([coxml/Makefile coxml/src/Makefile])
AC_CONFIG_FILES([occi/Makefile occi/src/Makefile])
AC_CONFIG_FILES([cords/Makefile cords/src/Makefile])
AC_CONFIG_FILES([corest/Makefile corest/src/Makefile])
AC_CONFIG_FILES([cocci/Makefile cocci/src/Makefile])
AC_CONFIG_FILES([cocarrier/Makefile cocarrier/src/Makefile])
AC_CONFIG_FILES([pubocci/Makefile pubocci/src/Makefile])
AC_CONFIG_FILES([copabr/Makefile copabr/src/Makefile])
AC_CONFIG_FILES([coss/Makefile coss/src/Makefile])
AC_CONFIG_FILES([coes/Makefile coes/src/Makefile])

dnl write all stuff
AC_OUTPUT

###
### Display results
###
echo "********************************************************************"
echo "  CompatibleOne ACCORDS configuration is now complete"
echo "********************************************************************"
echo ""
echo "    - coxml                    : $enable_coxml"
echo "    - occi                     : $enable_occi"
echo "    - cords                    : $enable_cords"
echo "    - corest                   : $enable_corest"
echo "    - cocci                    : $enable_cocci"
echo "    - cocarrier                : $enable_cocarrier"
echo "    - pubocci                  : $enable_pubocci"
echo "    - copabr                   : $enable_copabr"
echo "    - coss                     : $enable_coss"
echo "    - coes                     : $enable_coes"
echo "    - coobas                   : $enable_coobas"
echo "    - comons                   : $enable_comons"
echo "    - conets                   : $enable_conets"
echo "    - publisher                : $enable_publisher"
echo "    - parser                   : $enable_parser"
echo "    - broker                   : $enable_broker"
echo "    - procci                   : $enable_procci"
echo "    - osprocci                 : $enable_osprocci"
echo "    - onprocci                 : $enable_onprocci"
echo "    - azprocci                 : $enable_azprocci"
echo "    - testos                   : $enable_testos"
echo "    - teston                   : $enable_teston"
echo "    - testcp                   : $enable_testcp"
echo "    - testcb                   : $enable_testcb"
echo "    - testaz                   : $enable_testaz"
echo "    - fileserver               : $enable_fileserver"
echo "    - testresolver             : $enable_testresolver"
echo ""
