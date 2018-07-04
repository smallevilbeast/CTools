/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Josin                                                        |
  +----------------------------------------------------------------------+
*/

/* $Id$ */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ctools.h"
#include <sys/time.h>
#include "alarmtimer.h"

/**
 * {{{ All arg info for all functions in CTool\Timer class
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(alarmtimer_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(alarmtimer_add), 0, 0, 2)
    ZEND_ARG_INFO(0, time)
    ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

/*}}}*/

/*{{{ proto Timer::__construct()
 * To create the sharememory from the given id & size
 */
CTOOL_METHOD(AlarmTimer, __construct)
{
}/*}}}*/

/**
 * {{{ proto Timer::add()
 * Add the timer job to the timer listen list
 */
CTOOL_METHOD(AlarmTimer, add)
{
    zval *callback;
    zend_long time;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &time, &callback) == FAILURE)
    {
        return ;
    }
}/*}}}*/

CTOOL_FUNCTIONS(AlarmTimer)
    CTOOL_ME(AlarmTimer, __construct, arginfo_alarmtimer_construct,  ZEND_ACC_PUBLIC)
    CTOOL_ME(AlarmTimer, add, arginfo_alarmtimer_add,  ZEND_ACC_PUBLIC)
CTOOL_FUNCTIONS_END()

/**
 * Load the class: CTool\CIpc\ShareMemory class to the Zend engine.
 */
CTOOL_INIT(alarmtimer)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "CTool", "AlarmTimer", AlarmTimer_functions);

    alarmtimer_ce = zend_register_internal_class(&ce);
    alarmtimer_ce->ce_flags |= ZEND_ACC_FINAL;

    /* All timers which need to be runned */
    CTOOL_PR_NULL(alarmtimer_ce, "TASKS", ZEND_ACC_PRIVATE);
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
