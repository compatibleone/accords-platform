dnl @synopsis AC_CHECK_SSL
dnl
dnl This macro will check various standard spots for OpenSSL.
dnl User can supply --with-ssl=DIR to setup its own dir.
dnl
dnl If OpenSSL is found the include directory gets added to CFLAGS and
dnl CXXFLAGS as well as '-DHAVE_SSL', '-lssl' & '-lcrypto' get added to
dnl LIBS, and the libraries location gets added to LDFLAGS. Finally
dnl 'HAVE_SSL' gets set to 'yes' for use in your Makefile.in I use it
dnl like so (valid for gmake):
dnl
dnl     HAVE_SSL = @HAVE_SSL@
dnl     ifeq ($(HAVE_SSL),yes)
dnl         SRCS+= @srcdir@/my_file_that_needs_ssl.c
dnl     endif
dnl
dnl For bsd 'bmake' use:
dnl
dnl     .if ${HAVE_SSL} == "yes"
dnl         SRCS+= @srcdir@/my_file_that_needs_ssl.c
dnl     .endif
dnl
AC_DEFUN([AC_CHECK_SSL],
[AC_MSG_CHECKING(for SSL library)
_ssl_dirs=

AC_ARG_WITH([ssl],
	    [AC_HELP_STRING([--with-ssl=DIR],
			    [set user defined dir for SSL library])],
	    [ssldirs=$withval], 
	    [ssldirs="/usr/local/ssl /usr/lib/ssl /usr/ssl /usr/pkg /usr/local /usr"])

for dir in $ssldirs; do
  ssldir="$dir"
  if test -f "$dir/include/openssl/ssl.h"; then
    found_ssl="yes";
    CFLAGS="$CFLAGS -I$ssldir/include/openssl -DHAVE_SSL";
    CXXFLAGS="$CXXFLAGS -I$ssldir/include/openssl -DHAVE_SSL";
    break;
  fi
  if test -f "$dir/include/ssl.h"; then
    found_ssl="yes";
    CFLAGS="$CFLAGS -I$ssldir/include/ -DHAVE_SSL";
    CXXFLAGS="$CXXFLAGS -I$ssldir/include/ -DHAVE_SSL";
    break
  fi
done
if test x_$found_ssl != x_yes; then
  AC_MSG_ERROR(Cannot find ssl libraries)
else
  AC_MSG_RESULT([$ssldir])
  LIBS="$LIBS -lssl -lcrypto";
  LDFLAGS="$LDFLAGS -L$ssldir/lib";
  HAVE_SSL=yes
fi
AC_SUBST(HAVE_SSL)
])dnl
