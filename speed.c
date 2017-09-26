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

/* {{{ proto string confirm_speed_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_speed_compiled)
{
    smart_str sql = {0};
    smart_str_appends(&sql, "hello");
    smart_str_appends(&sql, " speed framework");
    php_printf("SMART_STR: %s", ZSTR_VAL(sql.s));
    smart_str_free(&sql);

    zval object;
    object_init_ex(&object, php_pdo_get_dbh_ce());

    zval rretval;
    SPEED_CALL_FUNCTION(&object, "__construct", &rretval)
        uint32_t param_count = 3;
        zval dsn, username, password;
        ZVAL_STRING(&dsn, "mysql:host=localhost;dbname=supjos");
        ZVAL_STRING(&username, "root");
        ZVAL_STRING(&password, "Root@localhost");
        zval params[] = { dsn, username, password};
    SPEED_END_CALL_FUNCTION();

    zval p_retval;
    SPEED_CALL_FUNCTION(&object, "prepare", &p_retval)
        uint32_t param_count = 1;
        zval prepare_sql;
        ZVAL_STRING(&prepare_sql, "SELECT * FROM www_product LIMIT 1000");
        zval params[] = {
            prepare_sql
        };
    SPEED_END_CALL_FUNCTION();

    zval e_retval;
    SPEED_CALL_FUNCTION(&p_retval, "execute", &e_retval)
        uint32_t param_count = 0;
        zval *params = NULL;
    SPEED_END_CALL_FUNCTION();

    zval f_retval;
    SPEED_CALL_FUNCTION(&p_retval, "fetchAll", &f_retval)
        uint32_t param_count = 1;
        zval fetch_style;
        ZVAL_LONG(&fetch_style, 2);
        zval params[] = {
            fetch_style
        };
    SPEED_END_CALL_FUNCTION();

    RETURN_ZVAL(&f_retval, 1, 0);

    zval *func;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &func) == FAILURE) {
        return ;
    }

    /* function_table, object, function_name, retval_ptr, param_count, params */
    if (Z_TYPE_P(func) == IS_OBJECT) {
        php_printf("Yes, this is object");
    }
    zend_function *funcs;
    if ( SPEED_METHOD_IN_OBJECT(func, "init") ) {
        php_printf("Yes, contains the init method, it was a object.");
    } else {
        php_printf("Not, it was only a closure");
    }

    zval returnval;
    /*call_user_function(CG(function_table), NULL, func, &returnval, 0, NULL);*/
    zval func_name;
    ZVAL_STRING(&func_name, "init");
    call_user_function(NULL, func, &func_name ,&returnval, 0, NULL);

    RETURN_NULL()

    zval result;

    ZVAL_UNDEF(&result);

    zend_op_array *op_array;
    zend_file_handle file_handle;

    char *tpl = "test.phtml";

    file_handle.filename = (const char *)tpl;
    file_handle.opened_path = NULL;
    file_handle.free_filename = 0;
    file_handle.type = ZEND_HANDLE_FILENAME;
    file_handle.handle.fp = NULL;

    op_array = zend_compile_file(&file_handle, ZEND_INCLUDE);

    if (op_array) {
        if (file_handle.handle.stream.handle) {
            if (!file_handle.opened_path) {
                file_handle.opened_path = zend_string_init(ZEND_STRL(tpl), 0);
            }
            zend_hash_add_empty_element(&EG(included_files), file_handle.opened_path);
        }
    }

    zend_execute_data *call;

    call = zend_vm_stack_push_call_frame(ZEND_CALL_NESTED_CODE | ZEND_CALL_HAS_SYMBOL_TABLE,
        (zend_function*)op_array, 0, 
        NULL, NULL);

    zend_array *symbol_tables;
    symbol_tables = emalloc(sizeof(zend_array));

    zend_hash_init(symbol_tables, 8, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_real_init(symbol_tables, 0);
    zval pdata;
    ZVAL_STRING(&pdata, "Speed, Josin");
    zend_hash_add_new(symbol_tables, zend_string_init(ZEND_STRL("name"), 0), &pdata);

    zval persons;
    array_init(&persons);
    add_assoc_string_ex(&persons, "a", 1, "aaa" );
    add_assoc_string_ex(&persons, "b", 1, "bbb" );
    add_assoc_string_ex(&persons, "c", 1, "ccc" );
    add_assoc_string_ex(&persons, "d", 1, "ddd" );
    zend_hash_add_new(symbol_tables, zend_string_init(ZEND_STRL("person"), 0), &persons);

    call->symbol_table = symbol_tables;

    zend_init_execute_data(call, op_array, &result);

    ZEND_ADD_CALL_FLAG(call, ZEND_CALL_TOP);
    zend_execute_ex(call);
    zend_vm_stack_free_call_frame(call);

    char reap_path[MAXPATHLEN];
    char *real = VCWD_REALPATH("Html/index.php", reap_path);
    php_printf("Real Path: %s\n", reap_path);


    zend_fcall_info zfi;
    zend_fcall_info_cache zfic;
    zval retuval;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "f*", &zfi, &zfic, &zfi.params, &zfi.param_count) == FAILURE) {
        return ;
    }

    zfi.retval = &retuval;
    if (zend_call_function(&zfi, &zfic) == SUCCESS) {
        php_printf("Yes, call the anonymouse function.\n");

        if (Z_TYPE(retuval) != IS_UNDEF) {
            php_printf("Return value : [[ %s ]]\n", Z_STRVAL(retuval));
        }
    }

    /* function_table, object, function_name, retval_ptr, param_count, params */
    zval retval;
    SPEED_CALL_FUNCTION(NULL, "file_exists", &retval)
        uint32_t param_count = 1;
        zval file_name;
        ZVAL_STRING(&file_name, "/home/josin/datas/../Lambda.java");
        zval params[] = {
            file_name
        };
    SPEED_END_CALL_FUNCTION();

    zval ret_val;
    SPEED_CALL_FUNCTION(NULL, "file_exists", &ret_val)
        uint32_t param_count = 1;
        zval file_name;
        ZVAL_STRING(&file_name, "/home/josin/datas/../Mains.java");
        zval params[] = {
            file_name
        };
    SPEED_END_CALL_FUNCTION();

    if (Z_TYPE(ret_val) == IS_TRUE) {
        php_printf("Yes, find the file Mains.java\n");
    } else {
        php_printf("Sorry, can't find the Mains.java\n");
    }

    RETURN_ZVAL(&retval, 0, 0);
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
    PHP_FE(confirm_speed_compiled,  NULL)
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
