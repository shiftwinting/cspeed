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

#include "main/SAPI.h" /* Use the sapi */

#include "kernel/net/supjos_speed_response.h"

/* {{{ ARG_INFO */
SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_response_setjsondata, 0, 0, 1)
    SPEED_ARG_INFO(0, vars)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_response_setrawdata, 0, 0, 1)
    SPEED_ARG_INFO(0, vars)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_response_send, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_response_setheader, 0, 0, 2)
    SPEED_ARG_INFO(0, key)
    SPEED_ARG_INFO(0, value)
SPEED_END_ARG_INFO()
/* }}}*/

/* {{{ proto Response::setJsonData(array $data)
    Set the array data into response, when output it will change into JSON format
 */
SPEED_METHOD(Response, setJsonData)
{
    zval *data;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &data) == FAILURE) {
        return ;
    }
    if (Z_TYPE_P(data) != IS_ARRAY) { /* The data must be array type data */
        RETURN_FALSE
    }
    /* For the reason that the data which you want to send were JSON, so turn the data into JSON format */
    zval retval;
    SPEED_CALL_FUNCTION(NULL, "json_encode", &retval)
        uint32_t param_count = 1;
        zval params[] = {
            *data
        };
    SPEED_END_CALL_FUNCTION();

    /* setJsonData will store the JSON data into the RESPONSE_DATA */
    zend_update_property(speed_response_ce, getThis(), ZEND_STRL(SPEED_RESPONSE_DATA), &retval);

    /* Set the content http header into application/json;charset=utf-8 */
    zval *headers = zend_read_property(speed_response_ce, getThis(), SPEED_STRL(SPEED_RESPONSE_HEADER), 1, NULL);
    if (Z_ISNULL_P(headers)) {
        zval header;
        array_init(&header);
        zend_update_property(speed_response_ce, getThis(), SPEED_STRL(SPEED_RESPONSE_HEADER), &header);
    }
    add_assoc_string_ex(headers, SPEED_STRL("Content-Type"), "application/json;chatset=UTF-8");
    RETURN_TRUE
}
/* }}}*/

/* {{{ proto Response::setRawData($data)
    Set the RAW formatter data
 */
SPEED_METHOD(Response, setRawData)
{
    zval *raw_data = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &raw_data) == FAILURE) {
        return ;
    }
    zend_update_property(speed_response_ce, getThis(), SPEED_STRL(SPEED_RESPONSE_DATA), raw_data);
    RETURN_TRUE
}
/*}}}*/

/* {{{ proto Response::send()
    Send the content data in the Response to the user-client, usually browser
 */
SPEED_METHOD(Response, send)
{
    zval *data = zend_read_property(speed_response_ce, getThis(), ZEND_STRL(SPEED_RESPONSE_DATA), 0, NULL);
    if (!ZVAL_IS_NULL(data)) {
        sapi_header_line ctr = {0};
        /* Reading the property from this class */
        zval *headers = zend_read_property(speed_response_ce, getThis(), SPEED_STRL(SPEED_RESPONSE_HEADER), 1, NULL);
        if (!ZVAL_IS_NULL(headers)) {
            zend_string *key;
            zval *value;
            ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(headers), key, value) {
                SPEED_HTTP_HEADER(ctr, ZSTR_VAL(key), Z_STRVAL_P(value), 200);
            } ZEND_HASH_FOREACH_END();
        }
        php_write(Z_STRVAL_P(data), Z_STRLEN_P(data));
        efree(ctr.line);
    }
}
/*}}}*/

/*{{{ proto Response::setHeader($headerKey, $headerValue) 
    For example Response::setHeader('Content-Type', 'application/json;charset=UTF-8');
*/
SPEED_METHOD(Response, setHeader)
{
    zend_string *key, *value;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "SS", &key, &value) == FAILURE) {
        return ;
    }
    zval *response_headers = zend_read_property(speed_response_ce, getThis(), ZEND_STRL(SPEED_RESPONSE_HEADER), 1, NULL);
    if (Z_ISNULL_P(response_headers)) {
        /* Update the property into array */
        zval headers;
        array_init(&headers);
        zend_update_property(speed_response_ce, getThis(), SPEED_STRL(SPEED_RESPONSE_HEADER), &headers);
    }
    add_assoc_string_ex(response_headers, SPEED_STRL(ZSTR_VAL(key)), ZSTR_VAL(value));
}
/* }}}*/

/* {{{ 
    The class's function of the supjos\Response class
 */
static const zend_function_entry speed_response_functions[] = {
    SPEED_ME(Response, setJsonData, arginfo_speed_response_setjsondata, ZEND_ACC_PUBLIC)
    SPEED_ME(Response, setRawData, arginfo_speed_response_setrawdata, ZEND_ACC_PUBLIC)
    SPEED_ME(Response, send, arginfo_speed_response_send, ZEND_ACC_PUBLIC)
    SPEED_ME(Response, setHeader, arginfo_speed_response_setheader, ZEND_ACC_PUBLIC)
    SPEED_FE_END
};
/*}}}*/

/* {{{ 
    Register the module into the speed extension
 */
SPEED_STARTUP_FUNCTION(response)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "supjos\\net", "Response", speed_response_functions);
    speed_response_ce = zend_register_internal_class(&ce);

    /* declare one property to store the data */
    zend_declare_property_null(speed_response_ce, ZEND_STRL(SPEED_RESPONSE_DATA), ZEND_ACC_PROTECTED);
    zend_declare_property_null(speed_response_ce, ZEND_STRL(SPEED_RESPONSE_HEADER), ZEND_ACC_PROTECTED);
}
/*}}}*/