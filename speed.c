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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_speed.h"

#include "kernel/net/supjos_speed_request.h"

#include "ext/pdo/php_pdo.h"
#include "ext/pdo/php_pdo_driver.h"

#include "zend_smart_str.h" /* use the smart_str */

/* True global resources - no need for thread safety here */
static int le_speed;

/* {{{ proto string speed_say_hi()
   Printf a string to the output client. */
PHP_FUNCTION(speed_say_hi)
{

/*    char *cwd = VCWD_GETCWD(NULL, 0);
    zend_printf("Current Directory: %s", cwd);*/

    zend_file_handle include_file_handle;
    include_file_handle.handle.fp     = NULL;
    include_file_handle.filename      = "Test.php";
    include_file_handle.opened_path   = NULL;
    include_file_handle.type          = ZEND_HANDLE_FILENAME;
    include_file_handle.free_filename = 0;

    zend_op_array *op_array;
    op_array = zend_compile_file(&include_file_handle, ZEND_REQUIRE);
    zend_execute_data *require = zend_vm_stack_push_call_frame(ZEND_CALL_NESTED_CODE | ZEND_CALL_HAS_SYMBOL_TABLE, (zend_function *)op_array, 0, NULL, NULL);

    zval result;
    ZVAL_UNDEF(&result);
    zend_init_execute_data(require, op_array, &result);
    ZEND_ADD_CALL_FLAG(require, ZEND_CALL_TOP);
    zend_execute_ex(require);
    zend_vm_stack_free_call_frame(require);

    /*zend_printf("CSpeed %s", PHP_SPEED_VERSION);*/
    RETURN_NULL()
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(speed)
{
    /* Start the following module before Apache or Nginx loading the speed PHP extension */
    SPEED_STARTUP(app);
    SPEED_STARTUP(request);
    SPEED_STARTUP(response);
    SPEED_STARTUP(model);
    SPEED_STARTUP(mysql);
    SPEED_STARTUP(view);
    SPEED_STARTUP(controller);
    SPEED_STARTUP(callback);
    SPEED_STARTUP(di);

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(speed)
{
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(speed)
{
#if defined(COMPILE_DL_SPEED) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(speed)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(speed)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "speed support", "enabled");
    php_info_print_table_end();
}
/* }}} */

/* {{{ speed_functions[]
 */
const zend_function_entry speed_functions[] = {
    PHP_FE(speed_say_hi,  NULL)
    PHP_FE_END
};
/* }}} */

/* {{{ speed_module_entry
 */
zend_module_entry speed_module_entry = {
    STANDARD_MODULE_HEADER,
    "speed",
    speed_functions,
    PHP_MINIT(speed),
    PHP_MSHUTDOWN(speed),
    PHP_RINIT(speed),     /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(speed), /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(speed),
    PHP_SPEED_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SPEED
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(speed)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
