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
  | Author:  Josin                                                       |
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

CTOOL_INIT(cthread);
CTOOL_INIT(cprocess);
CTOOL_INIT(carray);

/* If you declare any globals in php_ctools.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(ctools)
*/

/* True global resources - no need for thread safety here */
static int le_ctools;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("ctools.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_ctools_globals, ctools_globals)
    STD_PHP_INI_ENTRY("ctools.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_ctools_globals, ctools_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_ctools_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(get_ctools_version)
{
	RETURN_STRING( PHP_CTOOLS_VERSION );
}
/* }}} */

/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_ctools_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_ctools_init_globals(zend_ctools_globals *ctools_globals)
{
	ctools_globals->global_value = 0;
	ctools_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ctools)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/

	cthread_init();
	cprocess_init();
	carray_init();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ctools)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(ctools)
{
#if defined(COMPILE_DL_CTOOLS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(ctools)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ctools)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ctools support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ ctools_functions[]
 *
 * Every user visible function must have an entry in ctools_functions[].
 */
const zend_function_entry ctools_functions[] = {
	PHP_FE(get_ctools_version,	NULL) 		/* Get the ctools version */
	PHP_FE_END								/* Must be the last line in ctools_functions[] */
};
/* }}} */

/* {{{ ctools_module_entry
 */
zend_module_entry ctools_module_entry = {
	STANDARD_MODULE_HEADER,
	"ctools",
	ctools_functions,
	PHP_MINIT(ctools),
	PHP_MSHUTDOWN(ctools),
	PHP_RINIT(ctools),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(ctools),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(ctools),
	PHP_CTOOLS_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_CTOOLS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(ctools)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
