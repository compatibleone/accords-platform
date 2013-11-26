dnl configure.in for CompatibleOne ACCORDS components.
dnl Copyright 2011 Jean Parpaillon <jparpaillon@mandriva.com>
dnl
dnl Process this file with autoconf to produce a configure script.

dnl autoconf version
AC_PREREQ(2.59)

dnl mandatory stuff
AC_INIT([accords], [2.0], [compatible-accords@ow2.org])

dnl check host and target
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# tar-pax is needed for having paths with more than 99 chars
# which happens for java paprocci
AM_INIT_AUTOMAKE([1.9 -Wno-portability tar-pax])
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

AC_CHECK_HEADERS([ldap.h],
                [break],
		[AC_MSG_ERROR([You are missing ldap development files])])

if test "$(uname -s)" = 'Linux' ; then
	AC_SUBST([LDFLAGS_LDAP], ["-lldap"])
fi

AC_CHECK_HEADERS([mysql/mysql.h],
                [break],
		[AC_MSG_ERROR([You are missing mysql development files])])

if test "$(uname -s)" = 'Linux' ; then
	AC_SUBST([LDFLAGS_MYSQL], ["-lmysqlclient_r"])
fi

AC_CHECK_HEADERS([uuid/uuid.h uuid.h],
                [break],
		[AC_MSG_ERROR([You are missing uuid development files])])
AC_SUBST([CFLAGS_UUID], ["-I/usr/include/uuid"])
if test "$(uname -s)" = 'Linux' ; then
	AC_SUBST([LDFLAGS_UUID], ["-luuid"])
fi
if test "$(uname -s)" = 'OpenBSD' ; then
	AC_SUBST([LDFLAGS_UUID], ["-le2fs-uuid"])
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

# Some targets requires rpm
AC_CHECK_PROG([HAVE_RPM],[rpm],[yes],[no])
AM_CONDITIONAL([ENABLE_RPM],[test "$HAVE_RPM" = "yes"])

# Variables missing from some versions of autoconf
abs_top_srcdir=$(pwd)/${top_srcdir}
AC_SUBST([abs_top_srcdir])
abs_top_builddir=$(pwd)/${top_buildir}
AC_SUBST([abs_top_builddir])

###
### Modules configuration
###
AC_ARG_ENABLE([cosql],
      [AS_HELP_STRING([--disable-cosql],
		      [Disable cosql @<:@default=enable@:>@])],
      [],
      [enable_cosql=yes])
AM_CONDITIONAL([ENABLE_COSQL], [test "$enable_cosql" = "yes"])
AC_SUBST([CFLAGS_COSQL], [-I\${abs_top_srcdir}/cosql])
AC_SUBST([LD_COSQL], ["-L\${abs_top_builddir}/cosql -lcosql"])

AC_ARG_ENABLE([coxml],
      [AS_HELP_STRING([--disable-coxml],
		      [Disable coxml @<:@default=enable@:>@])],
      [],
      [enable_coxml=yes])
AM_CONDITIONAL([ENABLE_COXML], [test "$enable_coxml" = "yes"])
AC_SUBST([CFLAGS_COXML], [-I\${abs_top_srcdir}/coxml])
AC_SUBST([LD_COXML], ["-L\${abs_top_builddir}/coxml -lcoxml"])

AC_ARG_ENABLE([occi],
      [AS_HELP_STRING([--disable-occi],
		      [Disable occi @<:@default=enable@:>@])],
      [],
      [enable_occi=yes])
AM_CONDITIONAL([ENABLE_OCCI], [test "$enable_occi" = "yes"])
AC_SUBST([CFLAGS_OCCI], [-I\${abs_top_srcdir}/occi])
AC_SUBST([LD_OCCI], ["-L\${abs_top_builddir}/occi -locci"])

AC_ARG_ENABLE([cords],
      [AS_HELP_STRING([--disable-cords],
		      [Disable cords @<:@default=enable@:>@])],
      [],
      [enable_cords=yes])
AM_CONDITIONAL([ENABLE_CORDS], [test "$enable_cords" = "yes"])
AC_SUBST([CFLAGS_CORDS], ["-I\${abs_top_srcdir}/cords -I\${abs_top_builddir}/cords"])
AC_SUBST([LD_CORDS], ["-L\${abs_top_builddir}/cords -lcords"])

AC_ARG_ENABLE([corest],
      [AS_HELP_STRING([--disable-corest],
		      [Disable corest @<:@default=enable@:>@])],
      [],
      [enable_corest=yes])
AM_CONDITIONAL([ENABLE_COREST], [test "$enable_corest" = "yes"])
AC_SUBST([CFLAGS_COREST], [-I\${abs_top_srcdir}/corest])
AC_SUBST([LD_COREST], ["-L\${abs_top_builddir}/corest -lcorest"])

AC_ARG_ENABLE([cocci],
      [AS_HELP_STRING([--disable-cocci],
		      [Disable cocci @<:@default=enable@:>@])],
      [],
      [enable_cocci=yes])
AM_CONDITIONAL([ENABLE_COCCI], [test "$enable_cocci" = "yes"])
AC_SUBST([CFLAGS_COCCI], [-I\${abs_top_srcdir}/cocci])
AC_SUBST([LD_COCCI], ["-L\${abs_top_builddir}/cocci -lcocci"])

AC_ARG_ENABLE([cocarrier],
      [AS_HELP_STRING([--disable-cocarrier],
		      [Disable cocarrier @<:@default=enable@:>@])],
      [],
      [enable_cocarrier=yes])
AM_CONDITIONAL([ENABLE_COCARRIER], [test "$enable_cocarrier" = "yes"])
AC_SUBST([CFLAGS_COCARRIER], [-I\${abs_top_srcdir}/cocarrier])
AC_SUBST([LD_COCARRIER], ["-L\${abs_top_builddir}/cocarrier -lcocarrier"])

AC_ARG_ENABLE([copabr],
      [AS_HELP_STRING([--disable-copabr],
		      [Disable copabr @<:@default=enable@:>@])],
      [],
      [enable_copabr=yes])
AM_CONDITIONAL([ENABLE_COPABR], [test "$enable_copabr" = "yes"])
AC_SUBST([CFLAGS_COPABR], [-I\${abs_top_srcdir}/copabr])
AC_SUBST([LD_COPABR], ["-L\${abs_top_builddir}/copabr -lcopabr"])

AC_ARG_ENABLE([cocspi],
      [AS_HELP_STRING([--disable-cocspi],
		      [Disable cocspi @<:@default=enable@:>@])],
      [],
      [enable_cocspi=yes])
AM_CONDITIONAL([ENABLE_COCSPI], [test "$enable_cocspi" = "yes"])
AC_SUBST([CFLAGS_COCSPI], [-I\${abs_top_srcdir}/cocspi])
AC_SUBST([LD_COCSPI], ["-L\${abs_top_builddir}/cocspi -lcocspi"])

AC_ARG_ENABLE([pubocci],
      [AS_HELP_STRING([--disable-pubocci],
		      [Disable pubocci @<:@default=enable@:>@])],
      [],
      [enable_pubocci=yes])
AM_CONDITIONAL([ENABLE_PUBOCCI], [test "$enable_pubocci" = "yes"])
AC_SUBST([CFLAGS_PUBOCCI], [-I\${abs_top_srcdir}/pubocci])
AC_SUBST([LD_PUBOCCI], ["-L\${abs_top_builddir}/pubocci -lpubocci"])

AC_ARG_ENABLE([coss],
      [AS_HELP_STRING([--disable-coss],
		      [Disable coss @<:@default=enable@:>@])],
      [],
      [enable_coss=yes])
AM_CONDITIONAL([ENABLE_COSS], [test "$enable_coss" = "yes"])
AC_SUBST([CFLAGS_COSS], [-I\${abs_top_srcdir}/coss])
AC_SUBST([PATH_COSS], [${abs_top_srcdir}/coss])

AC_ARG_ENABLE([ezvm],
      [AS_HELP_STRING([--disable-ezvm],
		      [Disable ezvm @<:@default=enable@:>@])],
      [],
      [enable_ezvm=yes])
AM_CONDITIONAL([ENABLE_EZVM], [test "$enable_ezvm" = "yes"])
AC_SUBST([CFLAGS_EZVM], [-I\${abs_top_srcdir}/ezvm])
AC_SUBST([PATH_EZVM], [${abs_top_srcdir}/ezvm])

AC_ARG_ENABLE([cosacs],
      [AS_HELP_STRING([--disable-cosacs],
		      [Disable cosacs @<:@default=enable@:>@])],
      [],
      [enable_cosacs=yes])
AM_CONDITIONAL([ENABLE_COSACS], [test "$enable_cosacs" = "yes"])
AC_SUBST([CFLAGS_COSACS], [-I\${abs_top_srcdir}/cosacs])
AC_SUBST([PATH_COSACS], [${abs_top_srcdir}/cosacs])

AC_ARG_ENABLE([coips],
      [AS_HELP_STRING([--disable-coips],
		      [Disable coips @<:@default=enable@:>@])],
      [],
      [enable_coips=yes])
AM_CONDITIONAL([ENABLE_COIPS], [test "$enable_coips" = "yes"])
AC_SUBST([CFLAGS_COIPS], [-I\${abs_top_srcdir}/coips])
AC_SUBST([PATH_COIPS], [${abs_top_srcdir}/coips])

AC_ARG_ENABLE([cosched],
      [AS_HELP_STRING([--disable-cosched],
		      [Disable cosched @<:@default=enable@:>@])],
      [],
      [enable_cosched=yes])
AM_CONDITIONAL([ENABLE_COSCHED], [test "$enable_cosched" = "yes"])
AC_SUBST([CFLAGS_COSCHED], [-I\${abs_top_srcdir}/cosched])
AC_SUBST([PATH_COSCHED], [${abs_top_srcdir}/cosched])

AC_ARG_ENABLE([cops],
      [AS_HELP_STRING([--disable-cops],
		      [Disable cops @<:@default=enable@:>@])],
      [],
      [enable_cops=yes])
AM_CONDITIONAL([ENABLE_COPS], [test "$enable_cops" = "yes"])
AC_SUBST([CFLAGS_COPS], [-I\${abs_top_srcdir}/cops])
AC_SUBST([PATH_COPS], [${abs_top_srcdir}/cops])

AC_ARG_ENABLE([coees],
      [AS_HELP_STRING([--disable-coees],
		      [Disable coees @<:@default=enable@:>@])],
      [],
      [enable_coees=yes])
AM_CONDITIONAL([ENABLE_COEES], [test "$enable_coees" = "yes"])
AC_SUBST([CFLAGS_COEES], [-I\${abs_top_srcdir}/coees])
AC_SUBST([PATH_COEES], [${abs_top_srcdir}/coees])

AC_ARG_ENABLE([coobas],
      [AS_HELP_STRING([--disable-coobas],
		      [Disable coobas @<:@default=enable@:>@])],
      [],
      [enable_coobas=yes])
AM_CONDITIONAL([ENABLE_COOBAS], [test "$enable_coobas" = "yes"])
AC_SUBST([CFLAGS_COOBAS], [-I\${abs_top_srcdir}/coobas])
AC_SUBST([PATH_COOBAS], [${abs_top_srcdir}/coobas])

AC_ARG_ENABLE([comons],
      [AS_HELP_STRING([--disable-comons],
		      [Disable comons @<:@default=enable@:>@])],
      [],
      [enable_comons=yes])
AM_CONDITIONAL([ENABLE_COMONS], [test "$enable_comons" = "yes"])
AC_SUBST([CFLAGS_COMONS], [-I\${abs_top_srcdir}/comons])
AC_SUBST([PATH_COMONS], [${abs_top_srcdir}/comons])

AC_ARG_ENABLE([conets],
      [AS_HELP_STRING([--disable-conets],
		      [Disable conets @<:@default=enable@:>@])],
      [],
      [enable_conets=yes])
AM_CONDITIONAL([ENABLE_CONETS], [test "$enable_conets" = "yes"])
AC_SUBST([CFLAGS_CONETS], [-I\${abs_top_srcdir}/conets])
AC_SUBST([PATH_CONETS], [${abs_top_srcdir}/conets])

AC_ARG_ENABLE([publisher],
      [AS_HELP_STRING([--disable-publisher],
		      [Disable publisher @<:@default=enable@:>@])],
      [],
      [enable_publisher=yes])
AM_CONDITIONAL([ENABLE_PUBLISHER], [test "$enable_publisher" = "yes"])
AC_SUBST([PATH_PUBLISHER], [${abs_top_srcdir}/publisher])

AC_ARG_ENABLE([parser],
      [AS_HELP_STRING([--disable-parser],
		      [Disable parser @<:@default=enable@:>@])],
      [],
      [enable_parser=yes])
AM_CONDITIONAL([ENABLE_PARSER], [test "$enable_parser" = "yes"])
AC_SUBST([CFLAGS_PARSER], [-I\${abs_top_srcdir}/parser])
AC_SUBST([PATH_PARSER], [${abs_top_srcdir}/parser])

AC_ARG_ENABLE([broker],
      [AS_HELP_STRING([--disable-broker],
		      [Disable broker @<:@default=enable@:>@])],
      [],
      [enable_broker=yes])
AM_CONDITIONAL([ENABLE_BROKER], [test "$enable_broker" = "yes"])
AC_SUBST([CFLAGS_BROKER], [-I\${abs_top_srcdir}/broker])
AC_SUBST([PATH_BROKER], [${abs_top_srcdir}/broker])

AC_ARG_ENABLE([procci],
      [AS_HELP_STRING([--disable-procci],
		      [Disable procci @<:@default=enable@:>@])],
      [],
      [enable_procci=yes])
AM_CONDITIONAL([ENABLE_PROCCI], [test "$enable_procci" = "yes"])
AC_SUBST([CFLAGS_PROCCI], [-I\${abs_top_srcdir}/procci])
AC_SUBST([PATH_PROCCI], [${abs_top_srcdir}/procci])

AC_ARG_ENABLE([osprocci],
      [AS_HELP_STRING([--disable-osprocci],
		      [Disable osprocci @<:@default=enable@:>@])],
      [],
      [enable_osprocci=yes])
AM_CONDITIONAL([ENABLE_OSPROCCI], [test "$enable_osprocci" = "yes"])
AC_SUBST([CFLAGS_OSPROCCI], [-I\${abs_top_srcdir}/osprocci])
AC_SUBST([PATH_OSPROCCI], [${abs_top_srcdir}/osprocci])

AC_ARG_ENABLE([osocciprocci],
      [AS_HELP_STRING([--disable-osocciprocci],
		      [Disable osocciprocci @<:@default=enable@:>@])],
      [],
      [enable_osocciprocci=yes])
AM_CONDITIONAL([ENABLE_OSOCCIPROCCI], [test "$enable_osocciprocci" = "yes"])
AC_SUBST([CFLAGS_OSOCCIPROCCI], [-I\${abs_top_srcdir}/osocciprocci])
AC_SUBST([PATH_OSOCCIPROCCI], [${abs_top_srcdir}/osocciprocci])

AC_ARG_ENABLE([testosocci],
      [AS_HELP_STRING([--disable-testosocci],
		      [Disable testosocci @<:@default=enable@:>@])],
      [],
      [enable_testosocci=yes])
AM_CONDITIONAL([ENABLE_TESTOSOCCI], [test "$enable_testosocci" = "yes"])
AC_SUBST([CFLAGS_TESTOSOCCI], [-I\${abs_top_srcdir}/testosocci])
AC_SUBST([PATH_TESTOSOCCI], [${abs_top_srcdir}/testosocci])

AC_ARG_ENABLE([slam],
      [AS_HELP_STRING([--disable-slam],
		      [Disable slam @<:@default=enable@:>@])],
      [],
      [enable_slam=yes])
AM_CONDITIONAL([ENABLE_SLAM], [test "$enable_slam" = "yes"])
AC_SUBST([CFLAGS_SLAM], [-I\${abs_top_srcdir}/slam])
AC_SUBST([PATH_SLAM], [${abs_top_srcdir}/slam])

AC_ARG_ENABLE([copaas],
      [AS_HELP_STRING([--disable-copaas],
		      [Disable copaas @<:@default=enable@:>@])],
      [],
      [enable_copaas=yes])
AM_CONDITIONAL([ENABLE_COPAAS], [test "$enable_copaas" = "yes"])
AC_SUBST([CFLAGS_COPAAS], [-I\${abs_top_srcdir}/copaas])
AC_SUBST([PATH_COPAAS], [${abs_top_srcdir}/copaas])

AC_ARG_ENABLE([testorga],
      [AS_HELP_STRING([--disable-testorga],
		      [Disable testorga @<:@default=enable@:>@])],
      [],
      [enable_testorga=yes])
AM_CONDITIONAL([ENABLE_TESTORGA], [test "$enable_testorga" = "yes"])
AC_SUBST([CFLAGS_TESTORGA], [-I\${abs_top_srcdir}/testorga])
AC_SUBST([PATH_TESTORGA], [${abs_top_srcdir}/testorga])

AC_ARG_ENABLE([orgabas],
      [AS_HELP_STRING([--disable-orgabas],
		      [Disable orgabas @<:@default=enable@:>@])],
      [],
      [enable_orgabas=yes])
AM_CONDITIONAL([ENABLE_ORGABAS], [test "$enable_orgabas" = "yes"])
AC_SUBST([CFLAGS_ORGABAS], [-I\${abs_top_srcdir}/orgabas])
AC_SUBST([PATH_ORGABAS], [${abs_top_srcdir}/orgabas])

AC_ARG_ENABLE([conagios],
      [AS_HELP_STRING([--disable-conagios],
		      [Disable conagios @<:@default=enable@:>@])],
      [],
      [enable_conagios=yes])
AM_CONDITIONAL([ENABLE_CONAGIOS], [test "$enable_conagios" = "yes"])
AC_SUBST([CFLAGS_CONAGIOS], [-I\${abs_top_srcdir}/conagios])
AC_SUBST([PATH_CONAGIOS], [${abs_top_srcdir}/conagios])

AC_ARG_ENABLE([paasprocci],
      [AS_HELP_STRING([--disable-paasprocci],
		      [Disable paasprocci @<:@default=enable@:>@])],
      [],
      [enable_paasprocci=yes])
AM_CONDITIONAL([ENABLE_PAASPROCCI], [test "$enable_paasprocci" = "yes"])
AC_SUBST([CFLAGS_PAASPROCCI], [-I\${abs_top_srcdir}/paasprocci])
AC_SUBST([PATH_PAASPROCCI], [${abs_top_srcdir}/paasprocci])

AC_ARG_ENABLE([cnprocci],
      [AS_HELP_STRING([--disable-cnprocci],
		      [Disable cnprocci @<:@default=enable@:>@])],
      [],
      [enable_cnprocci=yes])
AM_CONDITIONAL([ENABLE_CNPROCCI], [test "$enable_cnprocci" = "yes"])
AC_SUBST([CFLAGS_CNPROCCI], ["-I\${abs_top_srcdir}/cnprocci -I\${abs_top_builddir}/cnprocci"])
AC_SUBST([PATH_CNPROCCI], [${abs_top_srcdir}/cnprocci])

AC_ARG_ENABLE([eziprocci],
      [AS_HELP_STRING([--disable-eziprocci],
		      [Disable eziprocci @<:@default=enable@:>@])],
      [],
      [enable_eziprocci=yes])
AM_CONDITIONAL([ENABLE_EZIPROCCI], [test "$enable_eziprocci" = "yes"])
AC_SUBST([CFLAGS_EZIPROCCI], [-I\${abs_top_srcdir}/eziprocci])
AC_SUBST([PATH_EZIPROCCI], [${abs_top_srcdir}/eziprocci])

AC_ARG_ENABLE([coeasy],
      [AS_HELP_STRING([--disable-coeasy],
		      [Disable coeasy @<:@default=enable@:>@])],
      [],
      [enable_coeasy=yes])
AM_CONDITIONAL([ENABLE_COEASY], [test "$enable_coeasy" = "yes"])
AC_SUBST([CFLAGS_COEASY], [-I\${abs_top_srcdir}/coeasy])
AC_SUBST([PATH_COEASY], [${abs_top_srcdir}/coeasy])

AC_ARG_ENABLE([easigsm],
      [AS_HELP_STRING([--disable-easigsm],
		      [Disable easigsm @<:@default=enable@:>@])],
      [],
      [enable_easigsm=yes])
AM_CONDITIONAL([ENABLE_EASIGSM], [test "$enable_easigsm" = "yes"])
AC_SUBST([CFLAGS_EASIGSM], [-I\${abs_top_srcdir}/easigsm])
AC_SUBST([PATH_EASIGSM], [${abs_top_srcdir}/easigsm])

AC_ARG_ENABLE([oaprocci],
      [AS_HELP_STRING([--disable-oaprocci],
		      [Disable oaprocci @<:@default=enable@:>@])],
      [],
      [enable_oaprocci=yes])
AM_CONDITIONAL([ENABLE_OAPROCCI], [test "$enable_oaprocci" = "yes"])
AC_SUBST([CFLAGS_OAPROCCI], [-I\${abs_top_srcdir}/oaprocci])
AC_SUBST([PATH_OAPROCCI], [${abs_top_srcdir}/oaprocci])

AC_ARG_ENABLE([dummyprocci],
      [AS_HELP_STRING([--disable-dummyprocci],
		      [Disable dummyprocci @<:@default=enable@:>@])],
      [],
      [enable_dummyprocci=yes])
AM_CONDITIONAL([ENABLE_DUMMYPROCCI], [test "$enable_dummyprocci" = "yes"])
AC_SUBST([CFLAGS_DUMMYPROCCI], [-I\${abs_top_srcdir}/dummyprocci])
AC_SUBST([PATH_DUMMYPROCCI], [${abs_top_srcdir}/dummyprocci])

AC_ARG_ENABLE([comodel],
      [AS_HELP_STRING([--disable-comodel],
		      [Disable comodel @<:@default=enable@:>@])],
      [],
      [enable_comodel=yes])
AM_CONDITIONAL([ENABLE_COMODEL], [test "$enable_comodel" = "yes"])
AC_SUBST([CFLAGS_COMODEL], [-I\${abs_top_srcdir}/comodel])
AC_SUBST([PATH_COMODEL], [${abs_top_srcdir}/comodel])

AC_ARG_ENABLE([cool],
      [AS_HELP_STRING([--disable-cool],
		      [Disable cool @<:@default=enable@:>@])],
      [],
      [enable_cool=yes])
AM_CONDITIONAL([ENABLE_COOL], [test "$enable_cool" = "yes"])
AC_SUBST([CFLAGS_COOL], [-I\${abs_top_srcdir}/cool])
AC_SUBST([PATH_COOL], [${abs_top_srcdir}/cool])

AC_ARG_ENABLE([example],
      [AS_HELP_STRING([--disable-example],
		      [Disable example @<:@default=enable@:>@])],
      [],
      [enable_example=yes])
AM_CONDITIONAL([ENABLE_EXAMPLE], [test "$enable_example" = "yes"])
AC_SUBST([CFLAGS_EXAMPLE], [-I\${abs_top_srcdir}/example])
AC_SUBST([PATH_EXAMPLE], [${abs_top_srcdir}/example])

AC_ARG_ENABLE([dcprocci],
      [AS_HELP_STRING([--disable-dcprocci],
		      [Disable dcprocci @<:@default=enable@:>@])],
      [],
      [enable_dcprocci=yes])
AM_CONDITIONAL([ENABLE_DCPROCCI], [test "$enable_dcprocci" = "yes"])
AC_SUBST([CFLAGS_DCPROCCI], [-I\${abs_top_srcdir}/dcprocci])
AC_SUBST([PATH_DCPROCCI], [${abs_top_srcdir}/dcprocci])

AC_ARG_ENABLE([onprocci],
      [AS_HELP_STRING([--disable-onprocci],
		      [Disable onprocci @<:@default=enable@:>@])],
      [],
      [enable_onprocci=yes])
AM_CONDITIONAL([ENABLE_ONPROCCI], [test "$enable_onprocci" = "yes"])
AC_SUBST([CFLAGS_ONPROCCI], [-I\${abs_top_srcdir}/onprocci])
AC_SUBST([PATH_ONPROCCI], [${abs_top_srcdir}/onprocci])

AC_ARG_ENABLE([azprocci],
      [AS_HELP_STRING([--disable-azprocci],
		      [Disable azprocci @<:@default=enable@:>@])],
      [],
      [enable_azprocci=yes])
AM_CONDITIONAL([ENABLE_AZPROCCI], [test "$enable_azprocci" = "yes"])
AC_SUBST([CFLAGS_AZPROCCI], [-I\${abs_top_srcdir}/azprocci])
AC_SUBST([PATH_AZPROCCI], [${abs_top_srcdir}/azprocci])

AC_ARG_ENABLE([paprocci],
      [AS_HELP_STRING([--enable-paprocci],
		      [Enable paprocci and get as argument the Sun JDK path (preferred jdk1.6.0_25) @<:@default=disable@:>@])],
      [enable_paprocci=yes;paprocci_jdk=$enableval],
      [enable_paprocci=no;paprocci_jdk=jdk_not_used])
AM_CONDITIONAL([ENABLE_PAPROCCI], [test "$enable_paprocci" = "yes"])

if test "x$enable_paprocci" = "xyes"; then
      case $target_cpu in
           x86_64) JVM_ARCH=amd64 ;;
           i?86)   JVM_ARCH=i386 ;;
           *)      JVM_ARCH=$target_cpu ;;
      esac
      AC_SUBST([JVM_ARCH])
      JDK_ROOT=${paprocci_jdk}/jre/lib/${JVM_ARCH}
      AC_SUBST([JDK_ROOT])
      CFLAGS_JNI="-I${paprocci_jdk}/include -I${paprocci_jdk}/include/linux"
      AC_SUBST([CFLAGS_JNI])

       # Checks for jni.
      save_CFLAGS=$CFLAGS
      CFLAGS="$CFLAGS_JNI $CFLAGS"
      AC_CHECK_HEADERS([${paprocci_jdk}/include/jni.h], [
          save_LDFLAGS=$LDFLAGS
          LDFLAGS="-L$JDK_ROOT/client -L$JDK_ROOT/server $LDFLAGS"
          AC_CHECK_LIB(
              [jvm],
              [JNI_CreateJavaVM],
              [LDFLAGS_JNI_JVM="-ljvm"],
              [AC_MSG_ERROR(The JDK jvm library is required in order to build the program!)]
          )
#          AC_CHECK_LIB(
#              [java],
#              [JNI_CreateJavaVM],
#              [LDFLAGS_JNI_JAVA="-ljava"],
#              [AC_MSG_ERROR(The JDK java library is required in order to build the program!)]
#          )
#          AC_CHECK_LIB(
#              [verify],
#              [JNI_CreateJavaVM],
#              [LDFLAGS_JNI_VERIFY="-lverify"],
#              [AC_MSG_ERROR(The JDK verify library is required in order to build the program!)]
#          )
          LDFLAGS=$save_LDFLAGS
          CFLAGS=$save_CFLAGS
      ], AC_MSG_ERROR(The JVM header is required in order to build the program!))

      AC_SUBST([CFLAGS_PAPROCCI], ["-I\${abs_top_srcdir}/paprocci/src ${CFLAGS_JNI}"])
      AC_SUBST([LDFLAGS_PAPROCCI], ["-L${JDK_ROOT} -L${JDK_ROOT}/server -L${JDK_ROOT}/client -R${JDK_ROOT} -R${JDK_ROOT}/server -R${JDK_ROOT}/client ${LDFLAGS_JNI_JVM} ${LDFLAGS_JNI_JAVA} ${LDFLAGS_JNI_VERIFY}"])
      AC_SUBST([PATH_PAPROCCI], ["${abs_top_srcdir}/paprocci/src"])
      #AC_SUBST([CFLAGS_JNI], ["-I${paprocci_jdk}/include/ -I${paprocci_jdk}/include/linux/"])
      #AC_SUBST([LDFLAGS_JNI], ["-L${paprocci_jdk}/jre/lib/i386 -L${paprocci_jdk}/jre/lib/i386/client -ljvm -ljava -lverify"])
fi

AC_ARG_ENABLE([coidms],
      [AS_HELP_STRING([--enable-coidms],
		      [Enable coidms @<:@default=disable@:>@])],
      [enable_coidms=yes],
      [enable_coidms=no] 
      )
AM_CONDITIONAL([ENABLE_COIDMS], [test "$enable_coidms" = "yes"])
AC_SUBST([CFLAGS_COIDMS], [-I\${abs_top_srcdir}/coidms])
AC_SUBST([PATH_COIDMS], [${abs_top_srcdir}/coidms])
# XXX FIXME: Autodetect libcurl presence
AC_SUBST([LDFLAGS_COIDMS], ["-L/usr/include/curl -lcurl"])

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
AC_SUBST([CFLAGS_TESTOS], [-I\${abs_top_srcdir}/testos])

AC_ARG_ENABLE([testdc],
      [AS_HELP_STRING([--disable-testdc],
		      [Disable testdc @<:@default=enable@:>@])],
      [],
      [enable_testdc=yes])
AM_CONDITIONAL([ENABLE_TESTDC], [test "$enable_testdc" = "yes"])
AC_SUBST([CFLAGS_TESTDC], [-I\${abs_top_srcdir}/testdc])

AC_ARG_ENABLE([teston],
      [AS_HELP_STRING([--disable-teston],
		      [Disable teston @<:@default=enable@:>@])],
      [],
      [enable_teston=yes])
AM_CONDITIONAL([ENABLE_TESTON], [test "$enable_teston" = "yes"])

AC_ARG_ENABLE([testhdfs],
      [AS_HELP_STRING([--disable-testhdfs],
		      [Disable testhdfs @<:@default=enable@:>@])],
      [],
      [enable_testhdfs=yes])
AM_CONDITIONAL([ENABLE_TESTHDFS], [test "$enable_testhdfs" = "yes"])

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

AC_ARG_ENABLE([testcn],
      [AS_HELP_STRING([--disable-testcn],
		      [Disable testcn @<:@default=enable@:>@])],
      [],
      [enable_testcn=yes])
AM_CONDITIONAL([ENABLE_TESTCN], [test "$enable_testcn" = "yes"])

AC_ARG_ENABLE([fileserver],
      [AS_HELP_STRING([--disable-fileserver],
		      [Disable fileserver @<:@default=enable@:>@])],
      [],
      [enable_fileserver=yes])
AM_CONDITIONAL([ENABLE_FILESERVER], [test "$enable_fileserver" = "yes"])
AC_SUBST([PATH_FILESERVER], [${abs_top_srcdir}/fileserver])

AC_ARG_ENABLE([testresolver],
      [AS_HELP_STRING([--disable-testresolver],
		      [Disable testresolver @<:@default=enable@:>@])],
      [],
      [enable_testresolver=yes])
AM_CONDITIONAL([ENABLE_TESTRESOLVER], [test "$enable_testresolver" = "yes"])

AC_ARG_ENABLE([testocci],
      [AS_HELP_STRING([--disable-testocci],
          [Disable testocci @<:@default=enable@:>@])],
      [],
      [enable_testocci=yes])
AM_CONDITIONAL([ENABLE_TESTOCCI], [test "$enable_testocci" = "yes"])

AC_ARG_ENABLE([testrest],
      [AS_HELP_STRING([--disable-testrest],
		      [Disable testrest @<:@default=enable@:>@])],
      [],
      [enable_testrest=yes])
AM_CONDITIONAL([ENABLE_TESTREST], [test "$enable_testrest" = "yes"])

AC_ARG_ENABLE([testsuite],
      [AS_HELP_STRING([--enable-testsuite],
		      [Enable testsuite @<:@default=disable@:>@])],
      [],
      [enable_testsuite=no])
AM_CONDITIONAL([ENABLE_TESTSUITE], [test "$enable_testsuite" = "yes"])

AC_ARG_ENABLE([pyaccords],
      [AS_HELP_STRING([--enable-pyaccords],
		      [Enable pyaccords @<:@default=disable@:>@])],
      [],
      [enable_pyaccords=no])
AM_CONDITIONAL([ENABLE_PYACCORDS], [test "$enable_pyaccords" = "yes"])

dnl files to generate with automake
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([scripts/Makefile])
AC_CONFIG_FILES([tools/Makefile tools/strukt/Makefile])
AC_CONFIG_FILES([manifests/Makefile])
AC_CONFIG_FILES([manpages/Makefile])
AC_CONFIG_FILES([coxml/Makefile])
AC_CONFIG_FILES([cosql/Makefile])
AC_CONFIG_FILES([occi/Makefile])
AC_CONFIG_FILES([cords/Makefile])
AC_CONFIG_FILES([corest/Makefile])
AC_CONFIG_FILES([cocci/Makefile])
AC_CONFIG_FILES([cocarrier/Makefile])
AC_CONFIG_FILES([copabr/Makefile])
AC_CONFIG_FILES([cocspi/Makefile])
AC_CONFIG_FILES([pubocci/Makefile])
AC_CONFIG_FILES([coss/Makefile])
AC_CONFIG_FILES([ezvm/Makefile])
AC_CONFIG_FILES([cops/Makefile])
AC_CONFIG_FILES([coees/Makefile])
AC_CONFIG_FILES([coobas/Makefile])
AC_CONFIG_FILES([comons/Makefile])
AC_CONFIG_FILES([conets/Makefile])
AC_CONFIG_FILES([publisher/Makefile])
AC_CONFIG_FILES([parser/Makefile])
AC_CONFIG_FILES([broker/Makefile])
AC_CONFIG_FILES([procci/Makefile])
AC_CONFIG_FILES([cosacs/Makefile])
AC_CONFIG_FILES([coips/Makefile])
AC_CONFIG_FILES([cosched/Makefile])
AC_CONFIG_FILES([osprocci/Makefile])
AC_CONFIG_FILES([osocciprocci/Makefile])
AC_CONFIG_FILES([slam/Makefile])
AC_CONFIG_FILES([copaas/Makefile])
AC_CONFIG_FILES([testorga/Makefile])
AC_CONFIG_FILES([orgabas/Makefile])
AC_CONFIG_FILES([paasprocci/Makefile])
AC_CONFIG_FILES([cnprocci/Makefile])
AC_CONFIG_FILES([eziprocci/Makefile])
AC_CONFIG_FILES([coeasy/Makefile])
AC_CONFIG_FILES([easigsm/Makefile])
AC_CONFIG_FILES([oaprocci/Makefile])
AC_CONFIG_FILES([dummyprocci/Makefile])
AC_CONFIG_FILES([comodel/Makefile])
AC_CONFIG_FILES([cool/Makefile])
AC_CONFIG_FILES([conagios/Makefile])
AC_CONFIG_FILES([example/Makefile])
AC_CONFIG_FILES([coidms/Makefile])
AC_CONFIG_FILES([testosocci/Makefile])
AC_CONFIG_FILES([onprocci/Makefile])
AC_CONFIG_FILES([azprocci/Makefile])
AC_CONFIG_FILES([dcprocci/Makefile])
AC_CONFIG_FILES([paprocci/Makefile paprocci/src/Makefile paprocci/java/Makefile])
AC_CONFIG_FILES([command/Makefile])
AC_CONFIG_FILES([testdc/Makefile])
AC_CONFIG_FILES([testos/Makefile])
AC_CONFIG_FILES([teston/Makefile])
AC_CONFIG_FILES([testhdfs/Makefile])
AC_CONFIG_FILES([testcp/Makefile])
AC_CONFIG_FILES([testcb/Makefile])
AC_CONFIG_FILES([testaz/Makefile])
AC_CONFIG_FILES([testcn/Makefile])
AC_CONFIG_FILES([fileserver/Makefile])
AC_CONFIG_FILES([testresolver/Makefile])
AC_CONFIG_FILES([testocci/Makefile])
AC_CONFIG_FILES([testrest/Makefile])
AC_CONFIG_FILES([testsuite/Makefile testsuite/basic/Makefile testsuite/platform/Makefile])
AC_CONFIG_FILES([pyaccords/Makefile pyaccords/pysrc/Makefile pyaccords/pysrc/pypacksrc/Makefile])
AC_CONFIG_FILES([pyaccordsSDK/Makefile pyaccordsSDK/pycompdev/Makefile pyaccordsSDK/pycompmanager/Makefile])

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
echo "    - cosql                    : $enable_cosql"
echo "    - occi                     : $enable_occi"
echo "    - cords                    : $enable_cords"
echo "    - corest                   : $enable_corest"
echo "    - cocci                    : $enable_cocci"
echo "    - cocarrier                : $enable_cocarrier"
echo "    - pubocci                  : $enable_pubocci"
echo "    - copabr                   : $enable_copabr"
echo "    - cocspi                   : $enable_cocspi"
echo "    - coss                     : $enable_coss"
echo "    - ezvm                     : $enable_ezvm"
echo "    - cops                     : $enable_cops"
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
echo "    - dcprocci                 : $enable_dcprocci"
echo "    - osocciprocci             : $enable_osocciprocci"
echo "    - slam                     : $enable_slam"
echo "    - copaas                   : $enable_copaas"
echo "    - testorga                 : $enable_testorga"
echo "    - orgabas                  : $enable_orgabas"
echo "    - conagios                 : $enable_conagios"
echo "    - paasprocci               : $enable_paasprocci"
echo "    - cnprocci                 : $enable_cnprocci"
echo "    - eziprocci                : $enable_eziprocci"
echo "    - coeasy                   : $enable_coeasy"
echo "    - easigsm                  : $enable_easigsm"
echo "    - oaprocci                 : $enable_oaprocci"
echo "    - dummyprocci              : $enable_dummyprocci"
echo "    - comodel                  : $enable_comodel"
echo "    - cool                     : $enable_cool"
echo "    - example                  : $enable_example"
echo "    - coidms                   : $enable_coidms"
echo "    - testosocci               : $enable_testosocci"
echo "    - onprocci                 : $enable_onprocci"
echo "    - azprocci                 : $enable_azprocci"
echo "    - paprocci                 : $enable_paprocci"
echo "    - JDK used                 : $paprocci_jdk"
echo "    - command                  : $enable_command"
echo "    - testos                   : $enable_testos"
echo "    - testdc                   : $enable_testdc"
echo "    - teston                   : $enable_teston"
echo "    - testhdfs                 : $enable_testhdfs"
echo "    - testcp                   : $enable_testcp"
echo "    - testcb                   : $enable_testcb"
echo "    - testaz                   : $enable_testaz"
echo "    - testcn                   : $enable_testcn"
echo "    - fileserver               : $enable_fileserver"
echo "    - testresolver             : $enable_testresolver"
echo "    - testocci                 : $enable_testocci"
echo "    - testrest                 : $enable_testrest"
echo "    - testsuite                : $enable_testsuite"
echo "    - pyaccords                : $enable_pyaccords"
echo ""

