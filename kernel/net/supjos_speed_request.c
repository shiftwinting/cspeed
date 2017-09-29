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
    if (!get_key) {
        return get_variables;
    } else {
        /* const HashTable *ht, zend_string *key */
        return zend_hash_find(Z_ARRVAL_P(get_variables), get_key);
    }
}
/* }}} */

/* {{{ 
    Return the value from the global variables $_POST   
 */
zval *request_post(zend_string *post_key)
{
    /* Get the given server path-info value */
    zval *post_variables = &PG(http_globals)[TRACK_VARS_POST];
    if (!post_key) {
        return post_variables;
    } else {
        /* const HashTable *ht, zend_string *key */
        return zend_hash_find(Z_ARRVAL_P(post_variables), post_key);
    }
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
    if (!server_key) {
        return server_variables;
    } else {
        /* const HashTable *ht, zend_string *key */
        return zend_hash_find(Z_ARRVAL_P(server_variables), server_key);
    }
}
/*}}}*/

/* Get the PATH-INFO */
zval *request_path_info() /* {{{ */
{
    return request_server(zend_string_init(ZEND_STRL("PATH_INFO"), 0));
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
    if ( request_method && ZVAL_IS_NULL(request_method)) {
        request_method = get_server_info("HTTP_X_HTTP_METHOD_OVERRIDE");
        if (request_method && ZVAL_IS_NULL(request_method)) {
            if (STRING_IS_EQUAL(r_method, "GET")){
                return 1;  /* default is GET request */
            } else {
                return 0;  /* otherwise false */
            }
        }
    }
    return STRING_IS_EQUAL(r_method, Z_STRVAL_P(request_method));
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

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_get, 0, 0, 0)
    SPEED_ARG_INFO(0, key)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getpost, 0, 0, 0)
    SPEED_ARG_INFO(0, key)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getquery, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_gethttphost, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getuseragent, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getservername, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getserverport, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getremoteport, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getgateway, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getscheme, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getprotocol, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getrequesturi, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_request_getscriptname, 0, 0, 0)
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

/*{{{ proto Request::get($key)
    Get the value from $_GET with the given key $key
 */
SPEED_METHOD(Request, get)
{
    zend_string *key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|S", &key) == FAILURE) {
        return ;
    }
    RETURN_ZVAL(request_get(key), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getPost($key)
    Get the value from the $_POST with the given key $key
 */
SPEED_METHOD(Request, getPost)
{
    zend_string *key;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|S", &key) == FAILURE){
        return ;
    }
    RETURN_ZVAL(request_post(key), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getQuery()
    Return the QueryString in the $_SERVER surper globals
 */
SPEED_METHOD(Request, getQuery)
{
    RETURN_ZVAL(get_server_info("QUERY_STRING"), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getHost()
    Return $_SERVER['HTTP_HOST']
 */
SPEED_METHOD(Request, getHost)
{
    RETURN_ZVAL(get_server_info("HTTP_HOST"), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getHost()
    Return $_SERVER['HTTP_USER_AGENT']
 */
SPEED_METHOD(Request, getUserAgent)
{
    RETURN_ZVAL(get_server_info("HTTP_USER_AGENT"), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getServerName()
    Return $_SERVER['SERVER_NAME']
 */
SPEED_METHOD(Request, getServerName)
{
    RETURN_ZVAL(get_server_info("SERVER_NAME"), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getServerPort()
    Return $_SERVER['SERVER_PORT']
 */
SPEED_METHOD(Request, getServerPort)
{
    RETURN_ZVAL(get_server_info("SERVER_PORT"), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getRemotePort()
    Return $_SERVER['REMOTE_PORT']
 */
SPEED_METHOD(Request, getRemotePort)
{
    RETURN_ZVAL(get_server_info("REMOTE_PORT"), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getGateway()
    Return $_SERVER['GATEWAY_INTERFACE']
 */
SPEED_METHOD(Request, getGateway)
{
    RETURN_ZVAL(get_server_info("GATEWAY_INTERFACE"), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getScheme()
    Return $_SERVER['REQUEST_SCHEME']
 */
SPEED_METHOD(Request, getScheme)
{
    RETURN_ZVAL(get_server_info("REQUEST_SCHEME"), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getProtocol()
    Return $_SERVER['SERVER_PROTOCOL']
 */
SPEED_METHOD(Request, getProtocol)
{
    RETURN_ZVAL(get_server_info("SERVER_PROTOCOL"), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getRequestUri()
    Return $_SERVER['REQUEST_URI']
 */
SPEED_METHOD(Request, getRequestUri)
{
    RETURN_ZVAL(get_server_info("REQUEST_URI"), 1, 0);
}
/*}}}*/

/* {{{ proto Request::getScriptName()
    Return $_SERVER['SCRIPT_NAME']
 */
SPEED_METHOD(Request, getScriptName)
{
    RETURN_ZVAL(get_server_info("SCRIPT_NAME"), 1, 0);
}
/*}}}*/



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
    SPEED_ME(Request, get, arginfo_speed_request_get, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getPost, arginfo_speed_request_getpost, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getQuery, arginfo_speed_request_getquery, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getHost, arginfo_speed_request_gethttphost, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getUserAgent, arginfo_speed_request_getuseragent, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getServerName, arginfo_speed_request_getservername, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getServerPort, arginfo_speed_request_getserverport, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getRemotePort, arginfo_speed_request_getremoteport, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getGateway, arginfo_speed_request_getgateway, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getScheme, arginfo_speed_request_getscheme, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getRequestUri, arginfo_speed_request_getrequesturi, ZEND_ACC_PUBLIC)
    SPEED_ME(Request, getScriptName, arginfo_speed_request_getscriptname, ZEND_ACC_PUBLIC)
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