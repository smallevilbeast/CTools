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
#include <pthread.h>
#include "cthread.h"

/**
 * The function which to be used to run the Async-task.
 * @param arg
 * @return
 */
void *worker(void *arg)
{
    zend_long index;
    zval ret, *object = (zval *)arg;
    zval *value, *handlers = zend_read_property(CTOOL_ENTRY_OBJ(object), CTOOL_STRL("HANDLERS"), 1, NULL);

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(handlers), index, value)
    {
        if ( (long)pthread_self() == index )
        {
            call_user_function( EG(function_table), NULL, value, &ret, 0, NULL );
            break;
        }
    } ZEND_HASH_FOREACH_END();
    zval_ptr_dtor(&ret);
    return NULL;
}

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(cthread_constructor), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(cthread_async_run), 0, 0, 1)
   ZEND_ARG_INFO(0, threadCallback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(cthread_kill_main_thread), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(cthread_destruct), 0, 0, 0)
ZEND_END_ARG_INFO()
/**
 * The following where the function definitions
 */
CTOOL_METHOD(CThread, __construct)
{
    zval *handlers = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("HANDLERS"), 1, NULL);
    if ( !handlers || ZVAL_IS_NULL(handlers) )
    {
        array_init(handlers);
    }
}

/*{{{ proto CThread::asyncRun
 * Run the task with a new thread, after the task done, the thread will be exit automatically.
 */
CTOOL_METHOD(CThread, asyncRun)
{
    int ret_val;
    zval *async_run;
    pthread_t thread_id = NULL;

    zval *handlers = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("HANDLERS"), 1, NULL);

    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &async_run) == FAILURE )
    {
        return ;
    }

    ret_val = pthread_create( &thread_id, NULL, worker, getThis() );
    if (ret_val != 0)
    {
        CTOOL_INFO(E_ERROR, "Can't create thread to do the job.");
    }

    Z_TRY_ADDREF_P(async_run);
    SEPARATE_ZVAL(async_run);

    add_index_zval(handlers, (long)thread_id, async_run);

    ZVAL_LONG(return_value, (long)thread_id);
}/*}}}*/

/* {{{ proto CThread::killMainThread
 * kill the main thread by hand. otherwise the kernel will  automatically exit the main thread.
 */
CTOOL_METHOD(CThread, killMainThread)
{
    zend_update_property_bool( CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("KILLMAIN"), IS_TRUE );
    pthread_exit(NULL);
}/*}}}*/

CTOOL_METHOD(CThread, __destruct)
{
    zval *kill_main = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("KILLMAIN"), 1, NULL);
    if ( kill_main && Z_TYPE_INFO_P(kill_main) ==   IS_FALSE )
    {
        pthread_exit(NULL);
    }
}

/* function definitions end */

CTOOL_FUNCTIONS(cthread)
    CTOOL_ME(CThread, __construct,    ARGINFO(cthread_constructor),      ZEND_ACC_PUBLIC)
    CTOOL_ME(CThread, asyncRun,       ARGINFO(cthread_async_run),        ZEND_ACC_PUBLIC)
    CTOOL_ME(CThread, killMainThread, ARGINFO(cthread_kill_main_thread), ZEND_ACC_PUBLIC)
    CTOOL_ME(CThread, __destruct,     ARGINFO(cthread_destruct),         ZEND_ACC_PUBLIC)
CTOOL_FUNCTIONS_END();

CTOOL_INIT(cthread)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "CTool", "CThread", cthread_functions);

    cthread_ce = zend_register_internal_class(&ce);
    cthread_ce->ce_flags |= ZEND_ACC_FINAL;
    
    CTOOL_PR_NULL(cthread_ce, "HANDLERS", ZEND_ACC_PRIVATE);
    CTOOL_PR_BOOL(cthread_ce, "KILLMAIN", 0, ZEND_ACC_PRIVATE);
}




/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
