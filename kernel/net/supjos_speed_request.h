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
#ifndef SUPJOS_SPEED_REQUEST_H
#define SUPJOS_SPEED_REQUEST_H


/* {{{ 
  The supjos\net\Request class entry
 */
zend_class_entry *speed_request_ce;
/*}}}*/

/* Get the $_GET[getkey] value*/
zval *request_get(zend_string *get_key);

/* Get the $_POST[postkey] value*/
zval *request_post(zend_string *post_key);

/* Get the $_SERVER[serverkey] value*/
zval *request_server(zend_string *server_key);

/* Get the PATH_INFO or REQUEST_PATH_INFO from $_SERVER */
zval *request_path_info();

/* Get the server-info with char key */
zval *get_server_info(char *key);

/* Whether the current request is THE GIVEN METHOD REQUEST */
zend_bool request_is(char *r_method);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */