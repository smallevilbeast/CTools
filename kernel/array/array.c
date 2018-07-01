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

zend_bool compare_zval(zval *one, zval *two);

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(array_construct), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(array_combinekeyvalue), 0, 0, 1)
    ZEND_ARG_INFO(0, assocArray)
    ZEND_ARG_INFO(0, convertBool)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(array_column_sum), 0, 0, 2)
    ZEND_ARG_INFO(0, arrayValue)
    ZEND_ARG_INFO(0, arrayColumnName)
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

/**
 * {{{ proto CArray::columnSum($array, "id")
 * Only support one or two dimension array
 */
CTOOL_METHOD(CArray, columnSum)
{
    zval *data;
    Bucket *bucket, *nest_bucket;
    zend_string *column;

    double result = 0.00;

    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zS", &data, &column) == FAILURE )
    {
        return ;
    }

    ZEND_HASH_FOREACH_BUCKET( Z_ARRVAL_P(data), bucket )
    {
        if (Z_TYPE(bucket->val) == IS_ARRAY)
        {
            ZEND_HASH_FOREACH_BUCKET(Z_ARRVAL(bucket->val), nest_bucket)
            {
                if ( zend_string_equals(nest_bucket->key, column) )
                {
                    convert_to_double(&(nest_bucket->val));
                    result += Z_DVAL(nest_bucket->val);
                }
            } ZEND_HASH_FOREACH_END();
        }
        else
        {
            if ( zend_string_equals(bucket->key, column) )
            {
                convert_to_double(&(bucket->val));
                result += Z_DVAL(bucket->val);
            }
        }
    } ZEND_HASH_FOREACH_END();

    RETURN_DOUBLE(result);
}/*}}}*/

/*{{{ All functions for the CTool\CArray class */
CTOOL_FUNCTIONS(array)
    CTOOL_ME(CArray, __construct, arginfo_array_construct, ZEND_ACC_PUBLIC)
    CTOOL_ME(CArray, combineKeyValue, arginfo_array_combinekeyvalue, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    CTOOL_ME(CArray, columnSum, arginfo_array_column_sum, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
CTOOL_FUNCTIONS_END()
/*}}}*/

CTOOL_INIT(carray)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "CTool", "CArray", array_functions);

    array_ce = zend_register_internal_class(&ce);
}

/**
 * The function compare the two zval to tell wheather the two zval was equals or not.
 * if equals return 1 otherwise 0 returned.
 */
zend_bool compare_zval( zval *one, zval *two )
{
    switch ( Z_TYPE_P(one) )
    {
        case IS_STRING :
            convert_to_string(two);
            if ( zend_string_equals( Z_STR_P(one), Z_STR_P(two) ) )
            {
                return 1;
            }
            else
            {
                return 0;
            }
            break;
        case IS_LONG :
            convert_to_long(two);
            if ( Z_LVAL_P(one) == Z_LVAL_P(two) )
            {
                return 1;
            }
            else
            {
                return 0;
            }
            break;
        case IS_DOUBLE :
            convert_to_double(two);
            if ( Z_DVAL_P(one) * 1000000 == Z_DVAL_P(two) * 1000000 )
            {
                return 1;
            }
            else
            {
                return 0;
            }
            break;
        default:
            return 0;
    }
}




/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
