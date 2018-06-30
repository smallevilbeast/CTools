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
#include <ctype.h>
#include <stdlib.h>
#include "array.h"

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(array_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(array_combinekeyvalue), 0, 0, 1)
    ZEND_ARG_INFO(0, assocArray)
    ZEND_ARG_INFO(0, convertBool)
ZEND_END_ARG_INFO()

/*{{{ proto CArray::__construct()
 * The function which to construct the CArray class
 */
CTOOL_METHOD(CArray, __construct)
{
}/*}}}*/

/*{{{ proto CArray::combineKeyValue()
 * Combine the array's key & value with converting the bool value or not.
 */
CTOOL_METHOD(CArray, combineKeyValue)
{
    zend_bool convert_bool = 0;
    zval *assoc_array;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a|b", &assoc_array, &convert_bool) == FAILURE)
    {
        return ;
    }
    if ( !zend_hash_num_elements(Z_ARRVAL_P(assoc_array)) )
    {
        RETURN_TRUE;
    }

    zval result;
    zend_string *new_key;
    Bucket *bucket;
    array_init(&result);

    ZEND_HASH_FOREACH_BUCKET(Z_ARRVAL_P(assoc_array), bucket)
    {
        if (Z_TYPE(bucket->val) == IS_OBJECT)
        {
            continue;
        }
        if ( (Z_TYPE_INFO(bucket->val) != IS_FALSE) && (Z_TYPE_INFO(bucket->val) != IS_TRUE) )
        {
            convert_to_string(&(bucket->val));
        }

        if ( convert_bool )
        {
            convert_to_string(&(bucket->val));
        }
        
        if ( !bucket->key )
        {
            new_key = strpprintf(0, "%d%s", bucket->h, (Z_TYPE_INFO(bucket->val) == IS_FALSE) ? "false" 
            : ((Z_TYPE_INFO(bucket->val) == IS_TRUE) ? "true": Z_STRVAL(bucket->val) ));
        }
        else
        {
            new_key = strpprintf(0, "%s%s", ZSTR_VAL(bucket->key), (Z_TYPE_INFO(bucket->val) == IS_FALSE) ? "false" 
            : ((Z_TYPE_INFO(bucket->val) == IS_TRUE) ? "false": Z_STRVAL(bucket->val) ));
        }

        add_next_index_str(&result, new_key);

    }ZEND_HASH_FOREACH_END();

    RETURN_ZVAL(&result, 1, NULL);
}/*}}}*/

/*{{{ All functions for the CTool\CArray class */
CTOOL_FUNCTIONS(array)
    CTOOL_ME(CArray, __construct, arginfo_array_construct, ZEND_ACC_PUBLIC)
    CTOOL_ME(CArray, combineKeyValue, arginfo_array_combinekeyvalue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
CTOOL_FUNCTIONS_END()
/*}}}*/

CTOOL_INIT(carray)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "CTool", "CArray", array_functions);

    array_ce = zend_register_internal_class(&ce);
}