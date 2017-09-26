/*
  +----------------------------------------------------------------------+
  | Speed framework                                                      |
  +----------------------------------------------------------------------+
  | Copyright (c) 2017-2020 www.supjos.cn                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  Josin <774542602@qq.com|www.supjos.cn>                      |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_speed.h"

#include "kernel/tool/supjos_speed_di.h"

/* {{{ARG_INFO For class supjos\tool\Di
 */
SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_di_get, 0, 0, 1)
    SPEED_ARG_INFO(0, key)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_di_set, 0, 0, 1)
    SPEED_ARG_INFO(0, key)
SPEED_END_ARG_INFO()
/*}}}*/

/* {{{ proto supjos\tool\Di::set($key, $callback)
    Store the object with the given key
 */
SPEED_METHOD(Di, set)
{
    zend_string  *object_key;
    zval retval;
    zend_fcall_info zfi;
    zend_fcall_info_cache zfic;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sf*", &object_key, &zfi, &zfic, &zfi.param_count, &zfi.params) == FAILURE) {
        return ;
    }
    zfi.retval = &retval; /* The method calling return_value */

    /*zend_class_entry *scope, zval *object, const char *name, size_t name_length, zend_bool silent, zval *rv*/
    zval *containers = zend_read_property(speed_di_ce, getThis(), ZEND_STRL(SUPJOS_SPEED_DI_CONTAINER), 1, NULL);
    if (ZVAL_IS_NULL(containers)) {
        zval container;
        array_init(&container);
        zend_update_property(speed_di_ce, getThis(), ZEND_STRL(SUPJOS_SPEED_DI_CONTAINER), &container);
    }
    /* Call the user-defined function[anonymous] and store it into container */
    if ( (zend_call_function(&zfi, &zfic) == SUCCESS) && (Z_TYPE(retval) != IS_UNDEF) ) {
        /* store the return value into the container */
        add_assoc_zval_ex(containers, ZSTR_VAL(object_key), ZSTR_LEN(object_key), &retval);
    }
}
/* }}} */ 

/*{{{ proto supjos\tool\Di::get($key)
    Get the object with the assoc. key
 */
SPEED_METHOD(Di, get)
{
    zend_string *key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE) {
        return ;
    }
    /*zend_class_entry *scope, zval *object, const char *name, size_t name_length, zend_bool silent, zval *rv*/
    zval *object_cotainer = zend_read_property(speed_di_ce, getThis(), ZEND_STRL(SUPJOS_SPEED_DI_CONTAINER), 1, NULL);
    if (ZVAL_IS_NULL(object_cotainer)) {
        RETURN_NULL()
    } else {
        zval *object = zend_hash_find(Z_ARRVAL_P(object_cotainer), key);
        RETURN_ZVAL(object, 1, 0)
    }
}
/*}}}*/

/*{{{
    All functions that obtained by class supjos\tool\Di
 */
static const zend_function_entry speed_di_functions[] = {
    SPEED_ME(Di, get, arginfo_speed_di_get, ZEND_ACC_PUBLIC)
    SPEED_ME(Di, set, arginfo_speed_di_set, ZEND_ACC_PUBLIC)

    SPEED_FE_END
};
/*}}}*/

/* {{{
    Loading the di module when webserver start the speed extension
 */
SPEED_STARTUP_FUNCTION(di)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "supjos\\tool", "Di", speed_di_functions);
    speed_di_ce = zend_register_internal_class(&ce);

    /* {{{
        Default properties for Di class
        zend_class_entry *ce, const char *name, size_t name_length, zval *property, int access_type
    */
    zend_declare_property_null(speed_di_ce, ZEND_STRL(SUPJOS_SPEED_DI_CONTAINER), ZEND_ACC_PROTECTED);
    /*}}}*/
}
/*}}}*/