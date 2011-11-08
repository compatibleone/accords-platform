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
AC_CONFIG_MACRO_DIR([m4])

AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AC_PROG_LN_S
AC_PROG_INSTALL
AC_HEADER_STDC

###
### Modules configuration
###
dnl AC_ARG_ENABLE([coxml],
dnl	      [AS_HELP_STRING([--disable-coxml],
dnl			      [Disable coxml @<:@default=enable@:>@])],
dnl	      [],
dnl	      [enable_coxml=yes])
enable_coxml=yes
AM_CONDITIONAL([ENABLE_COXML], [test "$enable_coxml" = "yes"])

enable_occi=no
AM_CONDITIONAL([ENABLE_OCCI], [test "$enable_occi" = "yes"])

enable_cords=no
AM_CONDITIONAL([ENABLE_CORDS], [test "$enable_cords" = "yes"])

enable_corest=no
AM_CONDITIONAL([ENABLE_COREST], [test "$enable_corest" = "yes"])

enable_cocci=no
AM_CONDITIONAL([ENABLE_COCCI], [test "$enable_cocci" = "yes"])

enable_cocarrier=no
AM_CONDITIONAL([ENABLE_COCARRIER], [test "$enable_cocarrier" = "yes"])

enable_pubocci=no
AM_CONDITIONAL([ENABLE_PUBOCCI], [test "$enable_pubocci" = "yes"])

enable_copabr=no
AM_CONDITIONAL([ENABLE_COPABR], [test "$enable_copabr" = "yes"])

enable_coss=no
AM_CONDITIONAL([ENABLE_COSS], [test "$enable_coss" = "yes"])

enable_coes=no
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

enable_broker=no
AM_CONDITIONAL([ENABLE_BROKER], [test "$enable_broker" = "yes"])

enable_procci=no
AM_CONDITIONAL([ENABLE_PROCCI], [test "$enable_procci" = "yes"])

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
