dnl $Id$
dnl config.m4 for extension jlog

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(jlog, for jlog support,
dnl Make sure that the comment is aligned:
dnl [  --with-jlog             Include jlog support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(jlog, whether to enable jlog support,
Make sure that the comment is aligned:
[  --enable-jlog           Enable jlog support])

jlog_src="jlog.c storage/jlog_storage.c"

if test "$PHP_JLOG" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-jlog -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/jlog.h"  # you most likely want to change this
  dnl if test -r $PHP_JLOG/$SEARCH_FOR; then # path given as parameter
  dnl   JLOG_DIR=$PHP_JLOG
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for jlog files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       JLOG_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$JLOG_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the jlog distribution])
  dnl fi

  dnl # --with-jlog -> add include path
  dnl PHP_ADD_INCLUDE($JLOG_DIR/include)

  dnl # --with-jlog -> check for lib and symbol presence
  dnl LIBNAME=jlog # you may want to change this
  dnl LIBSYMBOL=jlog # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $JLOG_DIR/$PHP_LIBDIR, JLOG_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_JLOGLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong jlog lib version or lib not found])
  dnl ],[
  dnl   -L$JLOG_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  PHP_SUBST(JLOG_SHARED_LIBADD)

  PHP_NEW_EXTENSION(jlog, ${jlog_src}, $ext_shared)
fi
