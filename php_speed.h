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

/* $Id$ */

#ifndef PHP_SPEED_H
#define PHP_SPEED_H

extern zend_module_entry speed_module_entry;
#define phpext_speed_ptr &speed_module_entry

#define PHP_SPEED_VERSION "1.0.0"

#ifdef PHP_WIN32
# define PHP_SPEED_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHP_SPEED_API __attribute__ ((visibility("default")))
#else
# define PHP_SPEED_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* Always refer to the globals in your function as SPEED_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define SPEED_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(speed, v)

#if defined(ZTS) && defined(COMPILE_DL_SPEED)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

/* {{{
  The macros of the Speed framework
  */
#define SPEED_METHOD            PHP_METHOD
#define SPEED_ME                PHP_ME
#define SPEED_ABSTRACT_ME       PHP_ABSTRACT_ME
#define SPEED_STARTUP_FUNCTION  PHP_MINIT_FUNCTION
#define SPEED_FE_END            PHP_FE_END

#define SPEED_STARTUP(module)   ZEND_MODULE_STARTUP_N(module)(INIT_FUNC_ARGS_PASSTHRU)

#define SPEED_BEGIN_ARG_INFO_EX ZEND_BEGIN_ARG_INFO_EX
#define SPEED_ARG_INFO          ZEND_ARG_INFO
#define SPEED_END_ARG_INFO      ZEND_END_ARG_INFO

#define SPEED_STRL(str)            (str), (strlen(str)) /* to repalce the ZEND_STRL */

#define STRING_IS_EMPTY(str)       ((*str) == '\0') /* Judge the char *str is emtpy or not. */
#define STRING_IS_EQUAL(dest, src) ( (dest == src) || ( !memcmp(dest, src, strlen(src)) ) ) /* Judge two char *string is equal or not. */
#define SPEED_PATH_IS_ABSOLUTE(path) ((*path) == '/') /* Absolute path or not. */

#define SPEED_METHOD_IN_OBJECT(object, method_name) ( ( Z_OBJ_HT_P(object)->get_method(&Z_OBJ_P(object), zend_string_init(ZEND_STRL(method_name), 0), NULL)) != NULL ) /* Judge whether the method is in object */
/* }}} */

/* macros to easy invoke the call_user_function */
#define SPEED_CALL_FUNCTION(obj_ptr, func_name, retval)     do { \
    zval function_name;                                          \
    ZVAL_STRING(&function_name, func_name);                      \
    zval *temp_val = retval;                                     \
    zval *obj = obj_ptr;

#define SPEED_END_CALL_FUNCTION()                                \
        call_user_function(CG(function_table), obj, &function_name, temp_val, param_count, params);  \
    } while(0)

#define SPEED_INVOKE_FUNCTION(obj_ptr, func_name, retval, param_count, params)                  do { \
        zval function_name;                                                                          \
        ZVAL_STRING(&function_name, func_name);                                                      \
        call_user_function(CG(function_table), obj_ptr, &function_name, retval, param_count, params);\
    } while(0)

#endif  /* PHP_SPEED_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
