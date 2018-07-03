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
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sharememory.h"

/**
 * {{{ All arg info for all functions in CTool\CIpc\ShareMemory class
 */
ZEND_BEGIN_ARG_INFO_EX(ARGINFO(sharememory_construct), 0, 0, 0)
    ZEND_ARG_INFO(0, memoryId)
    ZEND_ARG_INFO(0, memorySize)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(sharememory_write_data), 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(sharememory_read_data), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(sharememory_truncate), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(sharememory_unmap), 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ARGINFO(sharememory_unlink), 0, 0, 0)
ZEND_END_ARG_INFO()
/*}}}*/

/*{{{ proto ShareMemory::__construct(id, size)
 * To create the sharememory from the given id & size
 */
CTOOL_METHOD(ShareMemory, __construct)
{
    int shmfd;
    zend_long id = 1, memory_size = 5 * 1024;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ll", &id, &memory_size) == FAILURE)
    {
        return ;
    }

    shmfd = shm_open(ZSTR_VAL(strpprintf(0, "/ctools_memory%lld", id)), O_CREAT | O_RDWR, S_IRWXU );
    if (shmfd == -1)
    {
        CTOOL_INFO(E_ERROR, "Can't use the Posix sharememory!");
        return ;
    }

    zend_update_property_long(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMFDID"), id);
    zend_update_property_long(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMFD"), shmfd);
    zend_update_property_long(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMSIZE"), memory_size);
}/*}}}*/

/**
 * {{{ proto ShareMemory::writeData($data)
 * Write data into the shared-memory, note the size of the data must less than or equals to the 
 * size given in __construct .
 */
CTOOL_METHOD(ShareMemory, writeData)
{
    zend_string *data;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &data) == FAILURE)
    {
        return ;
    }

    zval *shmfd = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMFD"), 1, NULL);
    zval *mem_size = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMSIZE"), 1, NULL);

    ftruncate(Z_LVAL_P(shmfd), Z_LVAL_P(mem_size));

    char *memory = (char *)mmap(NULL, Z_LVAL_P(mem_size), PROT_WRITE, MAP_SHARED, Z_LVAL_P(shmfd), 0);
    bzero(memory, sizeof(char) * Z_LVAL_P(mem_size));
    if ( memory == MAP_FAILED )
    {
        CTOOL_INFO(E_ERROR, "Memory can't be mapped.");
        return ;
    }
    memcpy(memory, ZSTR_VAL(data), sizeof(char) * ( (ZSTR_LEN(data) > Z_LVAL_P(mem_size)) ? Z_LVAL_P(mem_size) : ZSTR_LEN(data)) );

    RETURN_TRUE;
}/*}}}*/

/**
 * {{{ proto ShareMemory::readData()
 * Read data from the shared memory
 */
CTOOL_METHOD(ShareMemory, readData)
{
    zval *id = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMFDID"), 1, NULL);
    zval *mem_size = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMSIZE"), 1, NULL);
    int shm_fd = shm_open(ZSTR_VAL(strpprintf(0, "/ctools_memory%lld", Z_LVAL_P(id))), O_RDONLY | O_RDWR, 0666);                  
    if (shm_fd == -1)
    {
        CTOOL_INFO(E_ERROR, "SharedMemory not created.");
        return ;
    }    
    char *ret_val = (char *)mmap(NULL, sizeof(char) * Z_LVAL_P(mem_size), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ret_val == MAP_FAILED)
    {
        CTOOL_INFO(E_ERROR, "Memory can't shared.");
        return ;
    }
    
    ZVAL_STRING(return_value, ret_val);
    close(shm_fd);
}/*}}}*/

/**
 * {{{ proto ShareMemory::truncate()
 * Truncate the sharememory to the given size
 */
CTOOL_METHOD(ShareMemory, truncate)
{
    int ret;
    zval *shmfd = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMFD"), 1, NULL);
    zval *mem_size = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMSIZE"), 1, NULL);

    ret = ftruncate(Z_LVAL_P(shmfd), Z_LVAL_P(mem_size));

    if (ret == -1)
    {
        CTOOL_INFO(E_ERROR, "ftruncate the shm_open failed.");
        return ;
    }
    RETURN_TRUE
}/*}}}*/

/**
 * {{{ proto ShareMemory::unmap()
 * Unmaping the memory from the shared memory.
 */
CTOOL_METHOD(ShareMemory, unmap)
{
    int ret;
    zval *shmfd = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMFD"), 1, NULL);
    zval *mem_size = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMSIZE"), 1, NULL);
    char *memory = (char *)mmap(NULL, Z_LVAL_P(mem_size), PROT_WRITE, MAP_SHARED, Z_LVAL_P(shmfd), 0);
    ret = munmap(memory, sizeof(char) * Z_LVAL_P(mem_size));
    if (ret == -1)
    {
        CTOOL_INFO(E_ERROR, "munmap operation failed.");
        return ;
    }
    RETURN_TRUE
}/*}}}*/

/**
 * {{{ proto ShareMemory::unlink()
 * unlink the data from the shared memory
 */
CTOOL_METHOD(ShareMemory, unlink)
{
    int ret;
    zval *id = zend_read_property(CTOOL_ENTRY_OBJ(getThis()), CTOOL_STRL("SHMFDID"), 1, NULL);
    zend_string *shm_filename  = strpprintf(0, "/ctools_memory%lld", Z_LVAL_P(id));
    ret = shm_unlink(ZSTR_VAL(shm_filename));
    if (ret == -1)
    {
        CTOOL_INFO(E_ERROR, "unlink operation failed.");
        return ;
    }
    RETURN_TRUE
}/*}}}*/

CTOOL_FUNCTIONS(sharememory)
    CTOOL_ME(ShareMemory, __construct, arginfo_sharememory_construct,  ZEND_ACC_PUBLIC)
    CTOOL_ME(ShareMemory, writeData,   arginfo_sharememory_write_data, ZEND_ACC_PUBLIC)
    CTOOL_ME(ShareMemory, readData,    arginfo_sharememory_read_data,  ZEND_ACC_PUBLIC)
    CTOOL_ME(ShareMemory, truncate,    arginfo_sharememory_truncate,   ZEND_ACC_PUBLIC)
    CTOOL_ME(ShareMemory, unmap,       arginfo_sharememory_unmap,      ZEND_ACC_PUBLIC)
    CTOOL_ME(ShareMemory, unlink,      arginfo_sharememory_unlink,     ZEND_ACC_PUBLIC)
CTOOL_FUNCTIONS_END()

/**
 * Load the class: CTool\CIpc\ShareMemory class to the Zend engine.
 */
CTOOL_INIT(sharememory)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "CTool\\CIpc", "ShareMemory", sharememory_functions);

    sharememory_ce = zend_register_internal_class(&ce);
    sharememory_ce->ce_flags |= ZEND_ACC_FINAL;

    CTOOL_PR_LONG(sharememory_ce, "SHMFD", 0, ZEND_ACC_PRIVATE);
    CTOOL_PR_LONG(sharememory_ce, "SHMFDID", 1, ZEND_ACC_PRIVATE);
    CTOOL_PR_LONG(sharememory_ce, "SHMSIZE", 5 * 1024, ZEND_ACC_PRIVATE);
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
