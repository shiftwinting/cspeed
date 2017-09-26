dnl $Id$
dnl config.m4 for extension speed

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(speed, for speed support,
Make sure that the comment is aligned:
[  --with-speed             Include speed support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(speed, whether to enable speed support,
dnl Make sure that the comment is aligned:
dnl [  --enable-speed           Enable speed support])

if test "$PHP_SPEED" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-speed -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/speed.h"  # you most likely want to change this
  dnl if test -r $PHP_SPEED/$SEARCH_FOR; then # path given as parameter
  dnl   SPEED_DIR=$PHP_SPEED
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for speed files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SPEED_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SPEED_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the speed distribution])
  dnl fi

  dnl # --with-speed -> add include path
  dnl PHP_ADD_INCLUDE($SPEED_DIR/include)

  dnl # --with-speed -> check for lib and symbol presence
  dnl LIBNAME=speed # you may want to change this
  dnl LIBSYMBOL=speed # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SPEED_DIR/$PHP_LIBDIR, SPEED_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SPEEDLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong speed lib version or lib not found])
  dnl ],[
  dnl   -L$SPEED_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SPEED_SHARED_LIBADD)

  dnl PHP_NEW_EXTENSION(speed, speed.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
  PHP_NEW_EXTENSION(speed, 
    speed.c                              \
    kernel/net/supjos_speed_request.c    \
    kernel/net/supjos_speed_response.c   \
    kernel/mvc/supjos_speed_controller.c \
    kernel/mvc/supjos_speed_callback.c   \
    kernel/tool/supjos_speed_di.c        \
    kernel/mvc/supjos_speed_view.c       \
    kernel/mvc/supjos_speed_model.c      \
    kernel/mvc/supjos_speed_app.c,
  $ext_shared)
  PHP_ADD_BUILD_DIR([$ext_builddir/kernel/mvc])
  PHP_ADD_BUILD_DIR([$ext_builddir/kernel/net])
  PHP_ADD_BUILD_DIR([$ext_builddir/kernel/tool])
fi
