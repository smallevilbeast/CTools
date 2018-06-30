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
#include <unistd.h>
#include "cprocess.h"

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(cprocess_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(cprocess_fork), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(cprocess_kill), 0, 0, 0)
    ZEND_ARG_INFO(0, pid)
ZEND_END_ARG_INFO()


CTOOL_METHOD(CProcess, __construct)
{
    
}

/*{{{ proto CProcess::fork
 * unix fork system call, when fork return value is 0, it means in the child process
 * otherwise in parent proess.
 */
CTOOL_METHOD(CProcess, fork)
{
    pid_t pid;

    pid = fork();

    if (pid == -1)
    {
        CTOOL_INFO(E_ERROR, "fork system invoke error. error code: %d", errno);
        return;
    }
    ZVAL_LONG(return_value, pid);
}/*}}}*/

/* {{{ proto CProcess::kill
 * Kill the process with the process-id
 */
CTOOL_METHOD(CProcess, kill)
{
    int ret;
    zend_long pid;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &pid) == FAILURE)
    {
        return ;
    }

    ret = kill(pid, SIGKILL);
    if (ret != 0)
    {
        CTOOL_INFO(E_ERROR, "kill the process: %ld failed.", pid );
        return ;
    }
    ZVAL_LONG(return_value, ret);
}/*}}}*/

CTOOL_FUNCTIONS(cprocess)
    CTOOL_ME(CProcess, __construct, arginfo_cprocess_construct, ZEND_ACC_PUBLIC)
    CTOOL_ME(CProcess, fork, arginfo_cprocess_fork, ZEND_ACC_PUBLIC)
    CTOOL_ME(CProcess, kill, arginfo_cprocess_kill, ZEND_ACC_PUBLIC)
CTOOL_FUNCTIONS_END()

CTOOL_INIT(cprocess)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "CTool", "CProcess", cprocess_functions);

    cprocess_ce = zend_register_internal_class(&ce);
    cprocess_ce->ce_flags |= ZEND_ACC_FINAL;
}