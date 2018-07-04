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

#ifndef PHP_CTOOLS_H
#define PHP_CTOOLS_H

extern zend_module_entry ctools_module_entry;
#define phpext_ctools_ptr &ctools_module_entry

#define PHP_CTOOLS_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_CTOOLS_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_CTOOLS_API __attribute__ ((visibility("default")))
#else
#	define PHP_CTOOLS_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:
*/
ZEND_BEGIN_MODULE_GLOBALS(ctools)
    zend_long  value;
ZEND_END_MODULE_GLOBALS(ctools)

/* Always refer to the globals in your function as CTOOLS_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define CTOOLS_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(ctools, v)

#if defined(ZTS) && defined(COMPILE_DL_CTOOLS)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#define CTOOL_ME                    PHP_ME
#define CTOOL_AB_ME                 PHP_ABSTRACT_ME
#define CTOOL_METHOD                PHP_METHOD
#define CTOOL_INIT(name)            void name##_init()
#define ARGINFO(name)                 arginfo_##name

#define CTOOL_FUNCTIONS(name)       \
    static const zend_function_entry name##_functions[] = {
#define CTOOL_FUNCTIONS_END()       PHP_FE_END };
#define CTOOL_THREAD_FUNC(name)     void *thread_##name(void *arg)

#define CTOOL_INFO(type, info...)   php_error_docref(NULL, type, ##info)
#define CTOOL_ENTRY_OBJ(obj)             (Z_OBJCE_P(obj)),(obj)
#define CTOOL_CC_STRING(entry, name, string_value)         zend_declare_class_constant_string(entry, CTOOL_STRL(name), string_value)
#define CTOOL_CC_LONG(entry, name, long_value)             zend_declare_class_constant_long(entry, CTOOL_STRL(name), long_value)
#define CTOOL_CC_BOOL(entry, name, bool_value)             zend_declare_class_constant_bool(entry, CTOOL_STRL(name), bool_value)
#define CTOOL_CC_DOUBLE(entry, name, double_value)         zend_declare_class_constant_double(entry, CTOOL_STRL(name), double_value)
#define CTOOL_CC_ZVAL(entry, name, zval_value)             zend_declare_class_constant(entry, CTOOL_STRL(name), zval_value)
#define CTOOL_CC_NULL(entry, name)                         zend_declare_class_constant_null(entry, CTOOL_STRL(name))
#define CTOOL_PR_NULL(entry, name, type)                   zend_declare_property_null(entry, CTOOL_STRL(name), type)
#define CTOOL_PR_BOOL(entry, name, bool_value, type)       zend_declare_property_bool(entry, CTOOL_STRL(name), bool_value, type)
#define CTOOL_PR_LONG(entry, name, long_value, type)       zend_declare_property_long(entry, CTOOL_STRL(name), long_value, type)
#define CTOOL_PR_DOUBLE(entry, name, double_value, type)   zend_declare_property_double(entry, CTOOL_STRL(name), double_value, type)
#define CTOOL_PR_STRING(entry, name, string_value, type)   zend_declare_property_string(entry, CTOOL_STRL(name), string_value, type)
#define CTOOL_STRL(str)                  (str),(sizeof(str)-1)
#define CTOOL_STRS(str)                  (str),(sizeof(str))

extern ZEND_DECLARE_MODULE_GLOBALS(ctools);

#endif	/* PHP_CTOOLS_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
