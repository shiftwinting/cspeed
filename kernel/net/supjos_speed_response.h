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
#ifndef SUPJOS_SPEED_RESPONSE_H
#define SUPJOS_SPEED_RESPONSE_H

#define SPEED_RESPONSE_DATA   "_response_data"    /* the variable to store the response data */
#define SPEED_RESPONSE_HEADER "_response_header"  /* the response header */

#define SPEED_HTTP_HEADER(sapi_h_line, key, value, rcode) do {                        \
        sapi_h_line.line_len = spprintf(&(sapi_h_line.line), 0, "%s:%s", key, value); \
        sapi_h_line.response_code = rcode;                                            \
        sapi_header_op(SAPI_HEADER_REPLACE, &sapi_h_line);                            \
        efree(sapi_h_line.line);                                                      \
    } while(0)

#define SPEED_DELETE_HTTP_HEADER(sapi_h_line, key, value, rcode) do {                 \
        sapi_h_line.line_len = spprintf(&(sapi_h_line.line), 0, "%s:%s", key, value); \
        sapi_h_line.response_code = rcode;                                            \
        sapi_header_op(SAPI_HEADER_DELETE, &sapi_h_line);                             \
        efree(sapi_h_line.line);                                                      \
    } while(0)

/* {{{ The supjos\net\Response class
 */
zend_class_entry *speed_response_ce;
/*}}}*/

#endif