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
#include "kernel/net/supjos_speed_request.h"

#include <string.h>

/* {{{ 
    Return the value from the global variables $_GET, note that you must judge NULL   
 */
zval *request_get(zend_string *get_key)
{
    /* Get the given server path-info value */
    zval *get_variables = &PG(http_globals)[TRACK_VARS_GET];
    /* const HashTable *ht, zend_string *key */
    return zend_hash_find(Z_ARRVAL_P(get_variables), get_key);
}
/* }}} */

/* {{{ 
    Return the value from the global variables $_POST   
 */
zval *request_post(zend_string *post_key)
{
    /* Get the given server path-info value */
    zval *post_variables = &PG(http_globals)[TRACK_VARS_POST];
    /* const HashTable *ht, zend_string *key */
    return zend_hash_find(Z_ARRVAL_P(post_variables), post_key);
}
/*}}}*/

/* {{{ 
    Return the value from the global variables $_SERVER
    Notice that, In PHP module $_SERVER must be initialise before get value from it.   
 */
zval *request_server(zend_string *server_key)
{
    if (PG(auto_globals_jit)) {
        zend_string *server_str = zend_string_init(ZEND_STRL("_SERVER"), 0);
        zend_is_auto_global(server_str);
        zend_string_release(server_str);
    }
    /* Get the given server path-info value */
    zval *server_variables = &PG(http_globals)[TRACK_VARS_SERVER];
    /* const HashTable *ht, zend_string *key */
    return zend_hash_find(Z_ARRVAL_P(server_variables), server_key);
}
/*}}}*/

/* Get the PATH-INFO */
zval *request_path_info() /* {{{ */
{
    zval *path_info;
    path_info = request_server(zend_string_init(ZEND_STRL("PATH_INFO"), 0));
    if (Z_ISNULL_P(path_info)) {
        path_info = request_server(zend_string_init(ZEND_STRL("REQUEST_PATH_INFO"), 0));
        if (Z_ISNULL_P(path_info)) {
            return NULL;
        }
    }
    return path_info;
}
/*}}}*/

/* {{{ Get the server_info with the given key
 */
zval *get_server_info(char *key)
{
    return request_server(zend_string_init(SPEED_STRL(key), 0));
}
/*}}}*/

/*{{{ Judge the request is GET|POST|DELETE|OPTIONS or not HTTP_X_HTTP_METHOD_OVERRIDE
 */
zend_bool request_is(char *r_method)
{
    zval *request_method = get_server_info("REQUEST_METHOD");
    if (ZVAL_IS_NULL(request_method)) {
        request_method = get_server_info("HTTP_X_HTTP_METHOD_OVERRIDE");
        if (ZVAL_IS_NULL(request_method)) {
            if (STRING_IS_EQUAL(r_method, "GET")){
                return 1;  /* default is GET request */
            } else {
                return 0;  /* otherwise false */
            }
        }
    }
    return STRING_IS_EQUAL(Z_STRVAL_P(request_method), r_method);
}
/*}}}*/

/* {{{ ARGINFO FOR THE CLASS supjos\Request
 */
SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_reqeust_constructor, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_isget, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_ispost, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_isdelete, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_isput, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_ishead, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_isoptions, 0, 0, 0)
SPEED_END_ARG_INFO()
/* }}} */

/* {{{ proto $request = new Request()
  Return the instance of the class supjos\Request
 */
SPEED_METHOD(Request, __construct)
{

}
/* }}} */


/* {{{ proto $request->isGet()
    Return whether the current request were GET or not
 */
SPEED_METHOD(Request, isGet)
{
    RETURN_BOOL(request_is("GET"));
}
/* }}} */

/* {{{ proto $request->isPost()
    Return whether the current request were POST or not
 */
SPEED_METHOD(Request, isPost)
{
    RETURN_BOOL(request_is("POST"));
}
/* }}} */

/* {{{ proto $request->isDelete()
    Return whether the current request were DELETE or not
 */
SPEED_METHOD(Request, isDelete)
{
    RETURN_BOOL(request_is("DELETE"));
}
/* }}} */

/* Determine whether the current request is PUT reqeust */
SPEED_METHOD(Request, isPut) /* {{{ */
{
    RETURN_BOOL(request_is("PUT"));
}
/* }}} */

/* Determine whether the current request is HEAD reqeust */
SPEED_METHOD(Request, isHead) /* {{{ */
{
    RETURN_BOOL(request_is("HEAD"));
}
/* }}} */

/* Determine whether the current request is OPTIONS reqeust */
SPEED_METHOD(Request, isOptions) /* {{{ */
{
    RETURN_BOOL(request_is("OPTIONS"));
}
/* }}} */

/* {{{ The function entries of the class : supjos\Request
 */
static const zend_function_entry speed_request_fucntions[] = {
    SPEED_ME(Request, __construct, arginfo_speed_reqeust_constructor, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, isGet, arginfo_speed_request_isget, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, isPost, arginfo_speed_request_ispost, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, isDelete, arginfo_speed_request_isdelete, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, isPut, arginfo_speed_request_isput, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, isHead, arginfo_speed_request_ishead, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, isOptions, arginfo_speed_request_isoptions, ZEND_ACC_PUBLIC)
    SPEED_FE_END
};
/* }}} */

/* {{{
    Initialise the module's components named request
 */
SPEED_STARTUP_FUNCTION(request)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "supjos\\net", "Request", speed_request_fucntions);
    speed_request_ce = zend_register_internal_class(&ce);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */