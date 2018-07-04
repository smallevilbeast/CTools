PHP_ARG_WITH(ctools, for ctools support,
Make sure that the comment is aligned:
[  --with-ctools             Include ctools support])

dnl PHP_ARG_ENABLE(ctools, whether to enable ctools support,
dnl Make sure that the comment is aligned:
dnl [  --enable-ctools           Enable ctools support])

if test "$PHP_CTOOLS" != "no"; then
  dnl Write more examples of tests here...

  dnl CTools need NTS environment
  AC_MSG_CHECKING([for NTS])
  if test "$PHP_THREAD_SAFETY" != "no"; then
		AC_MSG_RESULT([CTools need your environment to be NTS, ZTS will not work.])
	else
		AC_MSG_RESULT([OK])
	fi

  dnl # --with-ctools -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/ctools.h"  # you most likely want to change this
  dnl if test -r $PHP_CTOOLS/$SEARCH_FOR; then # path given as parameter
  dnl   CTOOLS_DIR=$PHP_CTOOLS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for ctools files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       CTOOLS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$CTOOLS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the ctools distribution])
  dnl fi

  dnl # --with-ctools -> add include path
  PHP_ADD_INCLUDE($CTOOLS_DIR/include)

  dnl # --with-ctools -> check for lib and symbol presence
  dnl LIBNAME=ctools # you may want to change this
  dnl LIBSYMBOL=ctools # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $CTOOLS_DIR/$PHP_LIBDIR, CTOOLS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_CTOOLSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong ctools lib version or lib not found])
  dnl ],[
  dnl   -L$CTOOLS_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  PHP_SUBST(CTOOLS_SHARED_LIBADD)
  CFLAGS="-Wall -pthread $CFLAGS"
  LDFLAGS="$LDFLAGS -lpthread"
  PHP_ADD_LIBRARY(pthread, 1, CTOOLS_SHARED_LIBADD)
  PHP_NEW_EXTENSION(
    ctools,
    ctools.c                            \
    kernel/class_entries.c              \
    kernel/array/array.c                \
    kernel/filter/filter.c              \
    kernel/process/cprocess.c           \
    kernel/timer/timer.c                \
    kernel/process/ipc/sharememory.c    \
    kernel/thread/cthread.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
