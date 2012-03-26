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
AM_CFLAGS=
AC_ARG_ENABLE([debug],
      [AS_HELP_STRING([--enable-debug],
		      [Enable debug @<:@default=disable@:>@])],
      [],
      [enable_debug=no])
if test "x$enable_debug" = "xyes"; then
   AM_CFLAGS="$AM_CFLAGS -g -ggdb -O0"
fi
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_LDFLAGS])

AC_CHECK_SSL

AC_CHECK_HEADERS([uuid/uuid.h uuid.h],
                [break],
		[AC_MSG_ERROR([You are missing uuid development files])])
AC_SUBST([CFLAGS_UUID], ["-I/usr/include/uuid"])
if test "$(uname -s)" = 'Linux' ; then
	AC_SUBST([LDFLAGS_UUID], ["-luuid"])
fi

# Checks for pthread.
AC_CHECK_HEADERS([pthread.h], [
    AC_CHECK_LIB(pthread, pthread_create, [
    LIBS="$LIBS -lpthread"
    ], AC_MSG_ERROR(The pthread library is required in order to build the program!))
], AC_MSG_ERROR(The pthread header is required in order to build the program!))

AC_DEFINE([UNIX])
AC_DEFINE([unix])
AC_DEFINE([CONE])

# Variables missing from some versions of autoconf
abs_top_srcdir=$(pwd)/${top_srcdir}
AC_SUBST([abs_top_srcdir])
abs_top_builddir=$(pwd)/${top_buildir}
AC_SUBST([abs_top_builddir])

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
AC_SUBST([LD_COXML], ["-L\${abs_top_builddir}/coxml/src -lcoxml"])

AC_ARG_ENABLE([occi],
      [AS_HELP_STRING([--disable-occi],
		      [Disable occi @<:@default=enable@:>@])],
      [],
      [enable_occi=yes])
AM_CONDITIONAL([ENABLE_OCCI], [test "$enable_occi" = "yes"])
AC_SUBST([CFLAGS_OCCI], [-I\${abs_top_srcdir}/occi/src])
AC_SUBST([LD_OCCI], ["-L\${abs_top_builddir}/occi/src -locci"])

AC_ARG_ENABLE([cords],
      [AS_HELP_STRING([--disable-cords],
		      [Disable cords @<:@default=enable@:>@])],
      [],
      [enable_cords=yes])
AM_CONDITIONAL([ENABLE_CORDS], [test "$enable_cords" = "yes"])
AC_SUBST([CFLAGS_CORDS], [-I\${abs_top_srcdir}/cords/src])
AC_SUBST([LD_CORDS], ["-L\${abs_top_builddir}/cords/src -lcords"])

AC_ARG_ENABLE([corest],
      [AS_HELP_STRING([--disable-corest],
		      [Disable corest @<:@default=enable@:>@])],
      [],
      [enable_corest=yes])
AM_CONDITIONAL([ENABLE_COREST], [test "$enable_corest" = "yes"])
AC_SUBST([CFLAGS_COREST], [-I\${abs_top_srcdir}/corest/src])
AC_SUBST([LD_COREST], ["-L\${abs_top_builddir}/corest/src -lcorest"])

AC_ARG_ENABLE([cocci],
      [AS_HELP_STRING([--disable-cocci],
		      [Disable cocci @<:@default=enable@:>@])],
      [],
      [enable_cocci=yes])
AM_CONDITIONAL([ENABLE_COCCI], [test "$enable_cocci" = "yes"])
AC_SUBST([CFLAGS_COCCI], [-I\${abs_top_srcdir}/cocci/src])
AC_SUBST([LD_COCCI], ["-L\${abs_top_builddir}/cocci/src -lcocci"])

AC_ARG_ENABLE([cocarrier],
      [AS_HELP_STRING([--disable-cocarrier],
		      [Disable cocarrier @<:@default=enable@:>@])],
      [],
      [enable_cocarrier=yes])
AM_CONDITIONAL([ENABLE_COCARRIER], [test "$enable_cocarrier" = "yes"])
AC_SUBST([CFLAGS_COCARRIER], [-I\${abs_top_srcdir}/cocarrier/src])
AC_SUBST([LD_COCARRIER], ["-L\${abs_top_builddir}/cocarrier/src -lcocarrier"])

AC_ARG_ENABLE([copabr],
      [AS_HELP_STRING([--disable-copabr],
		      [Disable copabr @<:@default=enable@:>@])],
      [],
      [enable_copabr=yes])
AM_CONDITIONAL([ENABLE_COPABR], [test "$enable_copabr" = "yes"])
AC_SUBST([CFLAGS_COPABR], [-I\${abs_top_srcdir}/copabr/src])
AC_SUBST([LD_COPABR], ["-L\${abs_top_builddir}/copabr/src -lcopabr"])

AC_ARG_ENABLE([pubocci],
      [AS_HELP_STRING([--disable-pubocci],
		      [Disable pubocci @<:@default=enable@:>@])],
      [],
      [enable_pubocci=yes])
AM_CONDITIONAL([ENABLE_PUBOCCI], [test "$enable_pubocci" = "yes"])
AC_SUBST([CFLAGS_PUBOCCI], [-I\${abs_top_srcdir}/pubocci/src])
AC_SUBST([LD_PUBOCCI], ["-L\${abs_top_builddir}/pubocci/src -lpubocci"])

AC_ARG_ENABLE([coss],
      [AS_HELP_STRING([--disable-coss],
		      [Disable coss @<:@default=enable@:>@])],
      [],
      [enable_coss=yes])
AM_CONDITIONAL([ENABLE_COSS], [test "$enable_coss" = "yes"])
AC_SUBST([CFLAGS_COSS], [-I\${abs_top_srcdir}/coss/src])
AC_SUBST([PATH_COSS], [${abs_top_srcdir}/coss/src])

AC_ARG_ENABLE([ezvm],
      [AS_HELP_STRING([--disable-ezvm],
		      [Disable ezvm @<:@default=enable@:>@])],
      [],
      [enable_ezvm=yes])
AM_CONDITIONAL([ENABLE_EZVM], [test "$enable_ezvm" = "yes"])
AC_SUBST([CFLAGS_EZVM], [-I\${abs_top_srcdir}/ezvm/src])
AC_SUBST([PATH_EZVM], [${abs_top_srcdir}/ezvm/src])

AC_ARG_ENABLE([cosacs],
      [AS_HELP_STRING([--disable-cosacs],
		      [Disable cosacs @<:@default=enable@:>@])],
      [],
      [enable_cosacs=yes])
AM_CONDITIONAL([ENABLE_COSACS], [test "$enable_cosacs" = "yes"])
AC_SUBST([CFLAGS_COSACS], [-I\${abs_top_srcdir}/cosacs/src])
AC_SUBST([PATH_COSACS], [${abs_top_srcdir}/cosacs/src])

AC_ARG_ENABLE([coips],
      [AS_HELP_STRING([--disable-coips],
		      [Disable coips @<:@default=enable@:>@])],
      [],
      [enable_coips=yes])
AM_CONDITIONAL([ENABLE_COIPS], [test "$enable_coips" = "yes"])
AC_SUBST([CFLAGS_COIPS], [-I\${abs_top_srcdir}/coips/src])
AC_SUBST([PATH_COIPS], [${abs_top_srcdir}/coips/src])

AC_ARG_ENABLE([cosched],
      [AS_HELP_STRING([--disable-cosched],
		      [Disable cosched @<:@default=enable@:>@])],
      [],
      [enable_cosched=yes])
AM_CONDITIONAL([ENABLE_COSCHED], [test "$enable_cosched" = "yes"])
AC_SUBST([CFLAGS_COSCHED], [-I\${abs_top_srcdir}/cosched/src])
AC_SUBST([PATH_COSCHED], [${abs_top_srcdir}/cosched/src])

AC_ARG_ENABLE([coes],
      [AS_HELP_STRING([--disable-coes],
		      [Disable coes @<:@default=enable@:>@])],
      [],
      [enable_coes=yes])
AM_CONDITIONAL([ENABLE_COES], [test "$enable_coes" = "yes"])
AC_SUBST([CFLAGS_COES], [-I\${abs_top_srcdir}/coes/src])
AC_SUBST([PATH_COES], [${abs_top_srcdir}/coes/src])

AC_ARG_ENABLE([coees],
      [AS_HELP_STRING([--disable-coees],
		      [Disable coees @<:@default=enable@:>@])],
      [],
      [enable_coees=yes])
AM_CONDITIONAL([ENABLE_COEES], [test "$enable_coees" = "yes"])
AC_SUBST([CFLAGS_COEES], [-I\${abs_top_srcdir}/coees/src])
AC_SUBST([PATH_COEES], [${abs_top_srcdir}/coees/src])

AC_ARG_ENABLE([coobas],
      [AS_HELP_STRING([--disable-coobas],
		      [Disable coobas @<:@default=enable@:>@])],
      [],
      [enable_coobas=yes])
AM_CONDITIONAL([ENABLE_COOBAS], [test "$enable_coobas" = "yes"])
AC_SUBST([CFLAGS_COOBAS], [-I\${abs_top_srcdir}/coobas/src])
AC_SUBST([PATH_COOBAS], [${abs_top_srcdir}/coobas/src])

AC_ARG_ENABLE([comons],
      [AS_HELP_STRING([--disable-comons],
		      [Disable comons @<:@default=enable@:>@])],
      [],
      [enable_comons=yes])
AM_CONDITIONAL([ENABLE_COMONS], [test "$enable_comons" = "yes"])
AC_SUBST([CFLAGS_COMONS], [-I\${abs_top_srcdir}/comons/src])
AC_SUBST([PATH_COMONS], [${abs_top_srcdir}/comons/src])

AC_ARG_ENABLE([conets],
      [AS_HELP_STRING([--disable-conets],
		      [Disable conets @<:@default=enable@:>@])],
      [],
      [enable_conets=yes])
AM_CONDITIONAL([ENABLE_CONETS], [test "$enable_conets" = "yes"])
AC_SUBST([CFLAGS_CONETS], [-I\${abs_top_srcdir}/conets/src])
AC_SUBST([PATH_CONETS], [${abs_top_srcdir}/conets/src])

AC_ARG_ENABLE([publisher],
      [AS_HELP_STRING([--disable-publisher],
		      [Disable publisher @<:@default=enable@:>@])],
      [],
      [enable_publisher=yes])
AM_CONDITIONAL([ENABLE_PUBLISHER], [test "$enable_publisher" = "yes"])
AC_SUBST([PATH_PUBLISHER], [${abs_top_srcdir}/publisher/src])

AC_ARG_ENABLE([parser],
      [AS_HELP_STRING([--disable-parser],
		      [Disable parser @<:@default=enable@:>@])],
      [],
      [enable_parser=yes])
AM_CONDITIONAL([ENABLE_PARSER], [test "$enable_parser" = "yes"])
AC_SUBST([CFLAGS_PARSER], [-I\${abs_top_srcdir}/parser/src])
AC_SUBST([PATH_PARSER], [${abs_top_srcdir}/parser/src])

AC_ARG_ENABLE([broker],
      [AS_HELP_STRING([--disable-broker],
		      [Disable broker @<:@default=enable@:>@])],
      [],
      [enable_broker=yes])
AM_CONDITIONAL([ENABLE_BROKER], [test "$enable_broker" = "yes"])
AC_SUBST([CFLAGS_BROKER], [-I\${abs_top_srcdir}/broker/src])
AC_SUBST([PATH_BROKER], [${abs_top_srcdir}/broker/src])

AC_ARG_ENABLE([procci],
      [AS_HELP_STRING([--disable-procci],
		      [Disable procci @<:@default=enable@:>@])],
      [],
      [enable_procci=yes])
AM_CONDITIONAL([ENABLE_PROCCI], [test "$enable_procci" = "yes"])
AC_SUBST([CFLAGS_PROCCI], [-I\${abs_top_srcdir}/procci/src])
AC_SUBST([PATH_PROCCI], [${abs_top_srcdir}/procci/src])

AC_ARG_ENABLE([osprocci],
      [AS_HELP_STRING([--disable-osprocci],
		      [Disable osprocci @<:@default=enable@:>@])],
      [],
      [enable_osprocci=yes])
AM_CONDITIONAL([ENABLE_OSPROCCI], [test "$enable_osprocci" = "yes"])
AC_SUBST([CFLAGS_OSPROCCI], [-I\${abs_top_srcdir}/osprocci/src])
AC_SUBST([PATH_OSPROCCI], [${abs_top_srcdir}/osprocci/src])

AC_ARG_ENABLE([onprocci],
      [AS_HELP_STRING([--disable-onprocci],
		      [Disable onprocci @<:@default=enable@:>@])],
      [],
      [enable_onprocci=yes])
AM_CONDITIONAL([ENABLE_ONPROCCI], [test "$enable_onprocci" = "yes"])
AC_SUBST([CFLAGS_ONPROCCI], [-I\${abs_top_srcdir}/onprocci/src])
AC_SUBST([PATH_ONPROCCI], [${abs_top_srcdir}/onprocci/src])

AC_ARG_ENABLE([azprocci],
      [AS_HELP_STRING([--disable-azprocci],
		      [Disable azprocci @<:@default=enable@:>@])],
      [],
      [enable_azprocci=yes])
AM_CONDITIONAL([ENABLE_AZPROCCI], [test "$enable_azprocci" = "yes"])
AC_SUBST([CFLAGS_AZPROCCI], [-I\${abs_top_srcdir}/azprocci/src])
AC_SUBST([PATH_AZPROCCI], [${abs_top_srcdir}/azprocci/src])

AC_ARG_ENABLE([command],
      [AS_HELP_STRING([--disable-command],
		      [Disable command @<:@default=enable@:>@])],
      [],
      [enable_command=yes])
AM_CONDITIONAL([ENABLE_COMMAND], [test "$enable_command" = "yes"])

AC_ARG_ENABLE([testos],
      [AS_HELP_STRING([--disable-testos],
		      [Disable testos @<:@default=enable@:>@])],
      [],
      [enable_testos=yes])
AM_CONDITIONAL([ENABLE_TESTOS], [test "$enable_testos" = "yes"])
AC_SUBST([CFLAGS_TESTOS], [-I\${abs_top_srcdir}/testos/src])

AC_ARG_ENABLE([teston],
      [AS_HELP_STRING([--disable-teston],
		      [Disable teston @<:@default=enable@:>@])],
      [],
      [enable_teston=yes])
AM_CONDITIONAL([ENABLE_TESTON], [test "$enable_teston" = "yes"])

AC_ARG_ENABLE([testcp],
      [AS_HELP_STRING([--disable-testcp],
		      [Disable testcp @<:@default=enable@:>@])],
      [],
      [enable_testcp=yes])
AM_CONDITIONAL([ENABLE_TESTCP], [test "$enable_testcp" = "yes"])

AC_ARG_ENABLE([testcb],
      [AS_HELP_STRING([--disable-testcb],
		      [Disable testcb @<:@default=enable@:>@])],
      [],
      [enable_testcb=yes])
AM_CONDITIONAL([ENABLE_TESTCB], [test "$enable_testcb" = "yes"])

AC_ARG_ENABLE([testaz],
      [AS_HELP_STRING([--disable-testaz],
		      [Disable testaz @<:@default=enable@:>@])],
      [],
      [enable_testaz=yes])
AM_CONDITIONAL([ENABLE_TESTAZ], [test "$enable_testaz" = "yes"])

AC_ARG_ENABLE([fileserver],
      [AS_HELP_STRING([--disable-fileserver],
		      [Disable fileserver @<:@default=enable@:>@])],
      [],
      [enable_fileserver=yes])
AM_CONDITIONAL([ENABLE_FILESERVER], [test "$enable_fileserver" = "yes"])
AC_SUBST([PATH_FILESERVER], [${abs_top_srcdir}/fileserver/src])

AC_ARG_ENABLE([testresolver],
      [AS_HELP_STRING([--disable-testresolver],
		      [Disable testresolver @<:@default=enable@:>@])],
      [],
      [enable_testresolver=yes])
AM_CONDITIONAL([ENABLE_TESTRESOLVER], [test "$enable_testresolver" = "yes"])

dnl files to generate with automake
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([scripts/Makefile])
AC_CONFIG_FILES([coxml/Makefile coxml/src/Makefile])
AC_CONFIG_FILES([occi/Makefile occi/src/Makefile])
AC_CONFIG_FILES([cords/Makefile cords/src/Makefile])
AC_CONFIG_FILES([corest/Makefile corest/src/Makefile])
AC_CONFIG_FILES([cocci/Makefile cocci/src/Makefile])
AC_CONFIG_FILES([cocarrier/Makefile cocarrier/src/Makefile])
AC_CONFIG_FILES([copabr/Makefile copabr/src/Makefile])
AC_CONFIG_FILES([pubocci/Makefile pubocci/src/Makefile])
AC_CONFIG_FILES([coss/Makefile coss/src/Makefile])
AC_CONFIG_FILES([ezvm/Makefile ezvm/src/Makefile])
AC_CONFIG_FILES([coes/Makefile coes/src/Makefile])
AC_CONFIG_FILES([coees/Makefile coees/src/Makefile])
AC_CONFIG_FILES([coobas/Makefile coobas/src/Makefile])
AC_CONFIG_FILES([comons/Makefile comons/src/Makefile])
AC_CONFIG_FILES([conets/Makefile conets/src/Makefile])
AC_CONFIG_FILES([publisher/Makefile publisher/src/Makefile])
AC_CONFIG_FILES([parser/Makefile parser/src/Makefile])
AC_CONFIG_FILES([broker/Makefile broker/src/Makefile])
AC_CONFIG_FILES([procci/Makefile procci/src/Makefile])
AC_CONFIG_FILES([cosacs/Makefile cosacs/src/Makefile])
AC_CONFIG_FILES([coips/Makefile coips/src/Makefile])
AC_CONFIG_FILES([cosched/Makefile cosched/src/Makefile])
AC_CONFIG_FILES([osprocci/Makefile osprocci/src/Makefile])
AC_CONFIG_FILES([onprocci/Makefile onprocci/src/Makefile])
AC_CONFIG_FILES([azprocci/Makefile azprocci/src/Makefile])
AC_CONFIG_FILES([command/Makefile command/src/Makefile])
AC_CONFIG_FILES([testos/Makefile testos/src/Makefile])
AC_CONFIG_FILES([teston/Makefile teston/src/Makefile])
AC_CONFIG_FILES([testcp/Makefile testcp/src/Makefile])
AC_CONFIG_FILES([testcb/Makefile testcb/src/Makefile])
AC_CONFIG_FILES([testaz/Makefile testaz/src/Makefile])
AC_CONFIG_FILES([fileserver/Makefile fileserver/src/Makefile])
AC_CONFIG_FILES([testresolver/Makefile testresolver/src/Makefile])

dnl write all stuff
AC_OUTPUT

###
### Display results
###
echo ""
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
echo "    - ezvm                     : $enable_ezvm"
echo "    - coes                     : $enable_coes"
echo "    - coees                    : $enable_coees"
echo "    - coobas                   : $enable_coobas"
echo "    - comons                   : $enable_comons"
echo "    - conets                   : $enable_conets"
echo "    - publisher                : $enable_publisher"
echo "    - parser                   : $enable_parser"
echo "    - broker                   : $enable_broker"
echo "    - procci                   : $enable_procci"
echo "    - cosacs                   : $enable_cosacs"
echo "    - coips                    : $enable_coips"
echo "    - cosched                  : $enable_cosched"
echo "    - osprocci                 : $enable_osprocci"
echo "    - onprocci                 : $enable_onprocci"
echo "    - azprocci                 : $enable_azprocci"
echo "    - command                  : $enable_command"
echo "    - testos                   : $enable_testos"
echo "    - teston                   : $enable_teston"
echo "    - testcp                   : $enable_testcp"
echo "    - testcb                   : $enable_testcb"
echo "    - testaz                   : $enable_testaz"
echo "    - fileserver               : $enable_fileserver"
echo "    - testresolver             : $enable_testresolver"
echo ""
