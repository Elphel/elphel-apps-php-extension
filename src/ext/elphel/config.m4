dnl $Id$
dnl config.m4 for extension elphel

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(elphel, for elphel support,
dnl Make sure that the comment is aligned:
dnl [  --with-elphel             Include elphel support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(elphel, whether to enable elphel support,
Make sure that the comment is aligned:
[  --enable-elphel           Enable elphel support])

if test "$PHP_ELPHEL" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-elphel -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/elphel.h"  # you most likely want to change this
  dnl if test -r $PHP_ELPHEL/$SEARCH_FOR; then # path given as parameter
  dnl   ELPHEL_DIR=$PHP_ELPHEL
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for elphel files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ELPHEL_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ELPHEL_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the elphel distribution])
  dnl fi

  dnl # --with-elphel -> add include path
  dnl PHP_ADD_INCLUDE($ELPHEL_DIR/include)

  dnl # --with-elphel -> check for lib and symbol presence
  dnl LIBNAME=elphel # you may want to change this
  dnl LIBSYMBOL=elphel # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ELPHEL_DIR/$PHP_LIBDIR, ELPHEL_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ELPHELLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong elphel lib version or lib not found])
  dnl ],[
  dnl   -L$ELPHEL_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ELPHEL_SHARED_LIBADD)

  PHP_NEW_EXTENSION(elphel, elphel_php.c, $ext_shared)
fi
