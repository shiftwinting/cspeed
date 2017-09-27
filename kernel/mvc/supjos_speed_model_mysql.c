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

#include "kernel/mvc/supjos_speed_model.h"
#include "kernel/mvc/supjos_speed_model_mysql.h"

#include "ext/pdo/php_pdo.h"                /* To use the PDO driver */
#include "ext/pdo/php_pdo_driver.h"         /* To user the driver struct */ 
#include "zend_smart_str.h"                 /* use the smart_str */

/* {{{ ARG_INFO */
SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_construct, 0, 0, 1)
    SPEED_ARG_INFO(0, data)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_select, 0, 0, 0)
    SPEED_ARG_INFO(0, fields)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_from, 0, 0, 0)
    SPEED_ARG_INFO(0, table)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_where, 0, 0, 0)
    SPEED_ARG_INFO(0, conditions)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_andwhere, 0, 0, 0)
    SPEED_ARG_INFO(0, conditions)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_groupby, 0, 0, 0)
    SPEED_ARG_INFO(0, by)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_having, 0, 0, 0)
    SPEED_ARG_INFO(0, having)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_orderby, 0, 0, 0)
    SPEED_ARG_INFO(0, orderby)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_limit, 0, 0, 0)
    SPEED_ARG_INFO(0, limit)
SPEED_END_ARG_INFO()
/*}}}*/

/* {{{ proto MySql::__construct($dsn, $username, $password [, $options = []])
 */
SPEED_METHOD(MySql, __construct)
{
    zval *connect_data = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &connect_data) == FAILURE) {
        return ;
    }

    if (Z_TYPE_P(connect_data) != IS_ARRAY) {
        php_error_docref("pdo.connect", E_ERROR, "Parameter must be an array");
        RETURN_FALSE;
    }

    zval *dsn, *username, *password, *options;

    dsn = zend_hash_find(Z_ARRVAL_P(connect_data), zend_string_init(SPEED_STRL("dsn"), 0));
    if (!dsn) {
        php_error_docref("pdo.dsn", E_ERROR, "PDO need a [dsn] key value");
        RETURN_FALSE;
    }

    username = zend_hash_find(Z_ARRVAL_P(connect_data), zend_string_init(SPEED_STRL("username"), 0));
    if (!username) {
        php_error_docref("pdo.username", E_ERROR, "PDO need a [username] key value");
        RETURN_FALSE;
    }

    password = zend_hash_find(Z_ARRVAL_P(connect_data), zend_string_init(SPEED_STRL("password"), 0));
    if (!password) {
        php_error_docref("pdo.password", E_ERROR, "PDO need a [password] key value");
        RETURN_FALSE;
    }

    options = zend_hash_find(Z_ARRVAL_P(connect_data), zend_string_init(SPEED_STRL("options"), 0));

    zval pdo_object;
    object_init_ex(&pdo_object, php_pdo_get_dbh_ce());
    zval rretval;
    if (!options) {
        SPEED_CALL_FUNCTION(&pdo_object, "__construct", &rretval)
            uint32_t param_count = 3;
            zval params[] = { *dsn, *username, *password};
        SPEED_END_CALL_FUNCTION();
    } else {
        SPEED_CALL_FUNCTION(&pdo_object, "__construct", &rretval)
            uint32_t param_count = 4;
            zval params[] = { *dsn, *username, *password, *options};
        SPEED_END_CALL_FUNCTION();
    }
}
/*}}}*/

/* {{{ proto MySql::select($fields) 
    SET THE deault select fields for the data
 */
SPEED_METHOD(MySql, select)
{
    zval p_fields;
    ZVAL_STRING(&p_fields, "*");
    zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_SELECT_DATA), &p_fields);
    
    zval *fields = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|z", &fields) == FAILURE) {
        return ;
    }
    if ( (Z_TYPE_P(fields) == IS_ARRAY) && fields) {
        smart_str sql_fields = {0};
        zend_string *var_name;
        zval *value;
        ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(fields), var_name, value){
            zend_string *str = strpprintf(0, "`%s`,", Z_STRVAL_P(value));
            smart_str_appends(&sql_fields, ZSTR_VAL(str));
            zend_string_release(str);
        } ZEND_HASH_FOREACH_END();
        
        /* Here has a little bug. for the memory leak */
        char *ss = ZSTR_VAL(sql_fields.s);
        ss[ZSTR_LEN(sql_fields.s) - 1] = '\0';
        
        ZVAL_STRING(&p_fields, ss);
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_SELECT_DATA), &p_fields);
        smart_str_free(&sql_fields);
    }
    RETURN_ZVAL(getThis(), 1, 0);
}
/*}}}*/


/*{{{ proto MySql::from()
    Set the select fields for which table
 */
SPEED_METHOD(MySql, from)
{
    zval *table = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &table) == FAILURE) {
        return ;
    }
    if (Z_TYPE_P(table) == IS_STRING && table) {
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_FROM_DATA), table);
        RETURN_ZVAL(getThis(), 1, 0);
    } else {
        php_error_docref("sql.from", E_ERROR, "Parameter want type string.");
        RETURN_FALSE;
    }
}
/*}}}*/

/*{{{ proto MySql::where()
    Set the select fields on which conditions
 */
SPEED_METHOD(MySql, where)
{
    zval *where = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &where) == FAILURE) {
        return ;
    }
    if ( (Z_TYPE_P(where) == IS_ARRAY)  && where ) {
        /* array data */
        zend_string *var_name;
        zval *value;
        smart_str where_str = {0};
        smart_str_appends(&where_str, " WHERE ");
        ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(where), var_name, value) {
            if (Z_TYPE_P(value) == IS_STRING) {
                zend_string *str = strpprintf(0, "`%s`='%s'", ZSTR_VAL(var_name), Z_STRVAL_P(value));
                smart_str_appends(&where_str, ZSTR_VAL(str));
                smart_str_appends(&where_str, " AND ");
                zend_string_release(str);
            } else if (Z_TYPE_P(value) == IS_LONG) {
                zend_string *str = strpprintf(0, "`%s`='%d'", ZSTR_VAL(var_name), Z_STRVAL_P(value));
                smart_str_appends(&where_str, ZSTR_VAL(str));
                smart_str_appends(&where_str, " AND ");
                zend_string_release(str);
            }
        } ZEND_HASH_FOREACH_END();

        char *ss = (char *)malloc(sizeof(char) * (ZSTR_LEN(where_str.s) - 4) );
        memcpy(ss, ZSTR_VAL(where_str.s), ZSTR_LEN(where_str.s) - 5);
        ss[ZSTR_LEN(where_str.s) - 5] = '\0';

        ZVAL_STRING(where, ss);
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_WHERE_DATA), where);
        free(ss);
        RETURN_ZVAL(getThis(), 1, 0);
    } else if ( (Z_TYPE_P(where) == IS_STRING) && where ) {
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_WHERE_DATA), where);
        RETURN_ZVAL(getThis(), 1, 0);
    } else {
        php_error_docref("sql.where", E_ERROR, "Parameter need array or string value");
        RETURN_FALSE;
    }
}
/*}}}*/

/*{{{ proto MySql::andWhere()
    Set the select fields on which conditions
 */
SPEED_METHOD(MySql, andWhere)
{
    zval *previous_where = zend_read_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_WHERE_DATA), 1, NULL);

    zval *where = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &where) == FAILURE) {
        return ;
    }
    if ( (Z_TYPE_P(where) == IS_ARRAY)  && where ) {
        /* array data */
        zend_string *var_name;
        zval *value;
        smart_str where_str = {0};
        if (Z_ISNULL_P(previous_where)) {
            smart_str_appends(&where_str, " WHERE ");
        } else {
            smart_str_appends(&where_str, Z_STRVAL_P(previous_where));
            smart_str_appends(&where_str, " AND ");
        }
        ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(where), var_name, value) {
            if (Z_TYPE_P(value) == IS_STRING) {
                zend_string *str = strpprintf(0, "`%s`='%s'", ZSTR_VAL(var_name), Z_STRVAL_P(value));
                smart_str_appends(&where_str, ZSTR_VAL(str));
                smart_str_appends(&where_str, " AND ");
                zend_string_release(str);
            } else if (Z_TYPE_P(value) == IS_LONG) {
                zend_string *str = strpprintf(0, "`%s`='%d'", ZSTR_VAL(var_name), Z_STRVAL_P(value));
                smart_str_appends(&where_str, ZSTR_VAL(str));
                smart_str_appends(&where_str, " AND ");
                zend_string_release(str);
            }
        } ZEND_HASH_FOREACH_END();

        char *ss = (char *)malloc(sizeof(char) * (ZSTR_LEN(where_str.s) - 4) );
        memcpy(ss, ZSTR_VAL(where_str.s), ZSTR_LEN(where_str.s) - 5);
        ss[ZSTR_LEN(where_str.s) - 5] = '\0';

        ZVAL_STRING(where, ss);
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_WHERE_DATA), where);
        free(ss);
        RETURN_ZVAL(getThis(), 1, 0);
    } else if ( (Z_TYPE_P(where) == IS_STRING) && where ) {
        zend_string *temp_where;
        if (Z_ISNULL_P(previous_where)) {
            temp_where = strpprintf(0, " %s ", where);
        } else {
            temp_where = strpprintf(0, " %s AND %s ", Z_STRVAL_P(previous_where), Z_STRVAL_P(where));
        }
        ZVAL_STRING(where, ZSTR_VAL(temp_where));
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_WHERE_DATA), where);
        RETURN_ZVAL(getThis(), 1, 0);
    } else {
        php_error_docref("sql.where", E_ERROR, "Parameter need array or string value");
        RETURN_FALSE;
    }
}
/*}}}*/

/*{{{ proto MySql::groupBy()
    Set the select fields on which groupby conditions
 */
SPEED_METHOD(MySql, groupBy)
{

}
/*}}}*/

/*{{{ proto MySql::having()
    Set the select fields on which having conditions
 */
SPEED_METHOD(MySql, having)
{

}
/*}}}*/

/*{{{ proto MySql::orderBy()
    Set the select fields on which orderBy conditions
 */
SPEED_METHOD(MySql, orderBy)
{

}
/*}}}*/

/*{{{ proto MySql::limit()
    Set the select fields on which limit conditions
 */
SPEED_METHOD(MySql, limit)
{

}
/*}}}*/


/* {{{ All methods for the MySql class
 */
static const zend_function_entry speed_mysql_functions[] = {
    SPEED_ME(MySql, __construct, arginfo_speed_model_mysql_construct, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, select, arginfo_speed_model_mysql_select, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, from, arginfo_speed_model_mysql_from, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, where, arginfo_speed_model_mysql_where, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, andWhere, arginfo_speed_model_mysql_andwhere, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, groupBy, arginfo_speed_model_mysql_groupby, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, having, arginfo_speed_model_mysql_having, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, orderBy, arginfo_speed_model_mysql_orderby, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, limit, arginfo_speed_model_mysql_limit, ZEND_ACC_PUBLIC)

    SPEED_FE_END
};
/*}}}*/

/* {{{ Loading the module into memory
 */
SPEED_STARTUP_FUNCTION(mysql)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "supjos\\mvc", "MySql", speed_mysql_functions);
    speed_mysql_ce = zend_register_internal_class(&ce);

    /* Implement the interface from Model */
    zend_class_implements(speed_mysql_ce, 1, speed_model_ce);

    /* Declare some property to store the data */
    zend_declare_property_null(speed_mysql_ce, SPEED_STRL(SPEED_MYSQL_SELECT_DATA), ZEND_ACC_PROTECTED);
    zend_declare_property_null(speed_mysql_ce, SPEED_STRL(SPEED_MYSQL_FROM_DATA), ZEND_ACC_PROTECTED);
    zend_declare_property_null(speed_mysql_ce, SPEED_STRL(SPEED_MYSQL_WHERE_DATA), ZEND_ACC_PROTECTED);
}
/*}}}*/