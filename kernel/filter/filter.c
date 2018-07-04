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
#include "ext/standard/php_string.h"
#include "filter.h"

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(filter_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(filter_filter), 0, 0, 2)
    ZEND_ARG_INFO(0, value)
    ZEND_ARG_INFO(0, closure)
ZEND_END_ARG_INFO()

/**
 * {{{ proto Filter::__construct()
 * The construct function for the filter class
 */
CTOOL_METHOD(Filter, __construct)
{

}/*}}}*/

/**
 * {{{ proto CFilter::filter($value, $closure)
 * To filter the value with the callback or global funtion which can be find in runtime or compile time
 * for example:
 *     CFileter::filter("hello", "strtoupper|strtolower");
 * which will return the raw string. 
 */
CTOOL_METHOD(Filter, filter)
{
    zval *value, *closure, *func_name, ret_val, func_array, temp_val;
    zend_string *error_handler = NULL;

    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &value, &closure) == FAILURE )
    {
        return ;
    }

    if ( Z_TYPE_P(closure) == IS_STRING )
    {
        ZVAL_NULL(&temp_val);
        array_init(&func_array);
        /* The closure can be more function together */
        php_explode(strpprintf(0, "|"), Z_STR_P(closure), &func_array, ZEND_LONG_MAX );

        ZEND_HASH_FOREACH_VAL(Z_ARRVAL(func_array), func_name)
        {
            if ( Z_STRLEN_P(func_name) )
            {
                zend_function *func= zend_hash_find_ptr( EG(function_table), zend_string_tolower(Z_STR_P(func_name)));
                if ( func && (func->type != ZEND_INTERNAL_FUNCTION || func->internal_function.handler != zif_display_disabled_function) )
                {
                    call_user_function( EG(function_table), NULL, func_name, &ret_val, 1, ZVAL_IS_NULL(&temp_val) ? value : &temp_val );
                    ZVAL_COPY_VALUE(&temp_val, &ret_val);
                }
                else
                {
                    CTOOL_INFO(E_ERROR, "Function: %s not exists.", Z_STRVAL_P(func_name));
                    RETURN_NULL();
                }
            }
        } ZEND_HASH_FOREACH_END();

        zval_ptr_dtor(&temp_val);
        zend_array_destroy(Z_ARRVAL(func_array));
    }
    else
    {
        if ( !zend_is_callable(closure, 0, &error_handler) )
        {
            CTOOL_INFO(E_ERROR, "Parameter must be valid function name or a callable.");
            RETURN_FALSE;
        }
        call_user_function( EG(function_table), NULL, closure, &ret_val, 1, value );
    }

    RETURN_ZVAL(&ret_val, 1, NULL);
}/*}}}*/

CTOOL_FUNCTIONS(filter)
    CTOOL_ME(Filter, __construct, arginfo_filter_construct, ZEND_ACC_PUBLIC)
    CTOOL_ME(Filter, filter, arginfo_filter_filter, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
CTOOL_FUNCTIONS_END()

/**
 * The filter_init() function to load the filter extension
 */
CTOOL_INIT(filter)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "CTool", "CFilter", filter_functions);

    filter_ce = zend_register_internal_class(&ce);
}/*}}}*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
