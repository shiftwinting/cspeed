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

#include <string.h>
#include "kernel/net/supjos_speed_request.h"
#include "kernel/mvc/supjos_speed_app.h"
#include "kernel/mvc/supjos_speed_controller.h"

/* Judge the given value was match or not
    -1 means not match the given pattern, 0 means match and the procure was invoked
 */
int deal_with_request(char *url, zval *object, char *method_name, INTERNAL_FUNCTION_PARAMETERS)
{
    zval retval;

    /* Getting the current URL PATH-INFO from the server_server function */
    zval *path_info = request_path_info();
    if (Z_ISNULL_P(path_info)) {
        php_error_docref(0, E_ERROR, "The server must support the PATH_INFO routine.");
    }

    /* Use the regular expression to match the url-path-info */
    zval is_match_path;
    SPEED_CALL_FUNCTION(NULL, "preg_match", &is_match_path)
        uint32_t param_count = 2;
        zval pattern, subject;
        zend_string *result_url = strpprintf(0, "#%s#", url);
        ZVAL_STRING(&pattern, ZSTR_VAL(result_url));
        ZVAL_STRING(&subject, Z_STRVAL_P(path_info));
        zval params[] = {
            pattern,
            subject
        };
    SPEED_END_CALL_FUNCTION();
    /* If the given url starts with slash or match the given pattern, continue */
    if ( ( strlen(url) == 1 && (strncmp(url, "/", 1) == 0)) /* The url was the only slash */
        || Z_LVAL(is_match_path) > 0                        /* Match the given url with pattern */
    ) {
        zval obj;
        object_init_ex(&obj, speed_controller_ce);
        zval params[] = {
            obj
        };
        if (method_name == NULL) {
            call_user_function(CG(function_table), NULL, object, &retval, 1, params);
        } else {
            zval function_name;
            ZVAL_STRING(&function_name, method_name);
            call_user_function(NULL, object, &function_name, &retval, 1, params);
        }
        if (Z_TYPE_P(&retval) != IS_UNDEF) {
            return 0;
        } else {
            return 0;
        }
    } else {
        return -1;
    }
}

/* {{{ deal with the method into object-calling or anonymous invoke
 */
void deal_with_init_method(char *url, zval *function, INTERNAL_FUNCTION_PARAMETERS)
{
    if (Z_TYPE_P(function) == IS_OBJECT) {
        if ( SPEED_METHOD_IN_OBJECT(function, "init") ) {
            /* Call the object with the given method name */
            if (!deal_with_request(url, function, "init", INTERNAL_FUNCTION_PARAM_PASSTHRU)) {
                return ;
            }
        } else {
            /* Call the anonymous function*/
            if (!deal_with_request(url, function, NULL, INTERNAL_FUNCTION_PARAM_PASSTHRU)) {
                return ;
            }
        }
    } else {
        php_printf("Parameter two must be anonymous function or object implement supjos\\mvc\\Callback .");
    }
    zval_ptr_dtor(function);
}
/* }}}*/

/* {{{ ARG_INFO
 */
SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_app_contructor, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_app_getversion, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_app_get, 0, 0, 2)
    SPEED_ARG_INFO(0, path)
    SPEED_ARG_INFO(0, callback)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_app_post, 0, 0, 2)
    SPEED_ARG_INFO(0, path)
    SPEED_ARG_INFO(0, callback)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_app_delete, 0, 0, 2)
    SPEED_ARG_INFO(0, path)
    SPEED_ARG_INFO(0, callback)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_app_put, 0, 0, 2)
    SPEED_ARG_INFO(0, path)
    SPEED_ARG_INFO(0, callback)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_app_options, 0, 0, 2)
    SPEED_ARG_INFO(0, path)
    SPEED_ARG_INFO(0, callback)
SPEED_END_ARG_INFO()
/* }}}*/

/* {{{ proto $app = new supjos\App()
    The constructor of the supjos\App class
 */
SPEED_METHOD(App, __construct)
{

}
/* }}} */

/* {{{ proto $app->getVersion()
    Return the current version of the Speed framework
 */
SPEED_METHOD(App, getVersion)
{
    RETURN_STRING(PHP_SPEED_VERSION);
}
/* }}} */

/* {{{ proto $app->get($url, function(){  })
    To get the Web GET request from internet
 */
SPEED_METHOD(App, get)
{
    if (!request_is("GET")){
        RETURN_FALSE
    }

    char *url = NULL;
    size_t url_len;

    /* The function passed by the user-level */
    zval *function;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &url, &url_len, &function) == FAILURE) {
        return ;
    }
    
    deal_with_init_method(url, function, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto App::post('/index$', function(){ echo 'Yes'; })
 */
SPEED_METHOD(App, post)
{
    if (!request_is("POST")){
        RETURN_FALSE
    }

    char *url = NULL;
    size_t url_len;

    /* The function passed by the user-level */
    zval *function;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &url, &url_len, &function) == FAILURE) {
        return ;
    }

    deal_with_init_method(url, function, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/*}}}*/

/* {{{ proto App::delete($url, callback);
    Deal with the delete request.
 */
SPEED_METHOD(App, delete)
{
    if (!request_is("DELETE")){
        RETURN_FALSE
    }

    char *url = NULL;
    size_t url_len;

    /* The function passed by the user-level */
    zval *function;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &url, &url_len, &function) == FAILURE) {
        return ;
    }

    deal_with_init_method(url, function, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/*}}}*/

/* {{{ proto App::put($url, callback);
    Deal with the PUT request.
 */
SPEED_METHOD(App, put)
{
    if (!request_is("PUT")){
        RETURN_FALSE
    }

    char *url = NULL;
    size_t url_len;

    /* The function passed by the user-level */
    zval *function;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &url, &url_len, &function) == FAILURE) {
        return ;
    }

    deal_with_init_method(url, function, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/*}}}*/

/* {{{ proto App::options($url, callback);
    Deal with the OPTIONS request.
 */
SPEED_METHOD(App, options)
{
    if (!request_is("OPTIONS")){
        RETURN_FALSE
    }

    char *url = NULL;
    size_t url_len;

    /* The function passed by the user-level */
    zval *function;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &url, &url_len, &function) == FAILURE) {
        return ;
    }

    deal_with_init_method(url, function, INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/*}}}*/

/* {{{
    The function entry of the class supjos\App
 */
static const zend_function_entry speed_app_functions[] = {
    SPEED_ME(App, __construct, arginfo_speed_app_contructor, ZEND_ACC_PUBLIC)
    SPEED_ME(App, getVersion, arginfo_speed_app_getversion, ZEND_ACC_PUBLIC)
    SPEED_ME(App, get, arginfo_speed_app_get, ZEND_ACC_PUBLIC)
    SPEED_ME(App, post, arginfo_speed_app_post, ZEND_ACC_PUBLIC)
    SPEED_ME(App, delete, arginfo_speed_app_delete, ZEND_ACC_PUBLIC)
    SPEED_ME(App, put, arginfo_speed_app_put, ZEND_ACC_PUBLIC)
    SPEED_ME(App, options, arginfo_speed_app_options, ZEND_ACC_PUBLIC)
    SPEED_FE_END
};
/* }}} */

/* {{{
    Initialise the class entry to the module
 */
SPEED_STARTUP_FUNCTION(app)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "supjos\\mvc", "App", speed_app_functions);
    speed_app_ce = zend_register_internal_class(&ce);
}
/*}}}*/


