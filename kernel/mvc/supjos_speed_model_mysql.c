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
SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_construct, 0, 0, 0)
    SPEED_ARG_INFO(0, data)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_select, 0, 0, 0)
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

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_findone, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_findall, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_query, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_execute, 0, 0, 0)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_mysql_insert, 0, 0, 0)
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

    zval pdo_object, rretval;
    object_init_ex(&pdo_object, php_pdo_get_dbh_ce());
    
    SPEED_CALL_FUNCTION(&pdo_object, "__construct", &rretval)
        uint32_t param_count = 3;
        zval params[] = { *dsn, *username, *password};
    SPEED_END_CALL_FUNCTION();

    zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_PDO_OBJECT), &pdo_object);

    zval p_fields;
    ZVAL_STRING(&p_fields, "*");
    zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_SELECT_DATA), &p_fields);
}
/*}}}*/

/* {{{ proto MySql::select($fields) 
    SET THE deault select fields for the data
 */
SPEED_METHOD(MySql, select)
{
    zval p_fields;
    
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
                zend_string *str = strpprintf(0, "`%s`='%d'", ZSTR_VAL(var_name), Z_LVAL_P(value));
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
        zend_string *temp_where = strpprintf(0, " WHERE %s ", Z_STRVAL_P(where));
        ZVAL_STRING(where, ZSTR_VAL(temp_where));
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_WHERE_DATA), where);
        zend_string_release(temp_where);
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
                zend_string *str = strpprintf(0, "`%s`='%d'", ZSTR_VAL(var_name), Z_LVAL_P(value));
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
            temp_where = strpprintf(0, " WHERE %s ", where);
        } else {
            temp_where = strpprintf(0, " %s AND %s ", Z_STRVAL_P(previous_where), Z_STRVAL_P(where));
        }
        ZVAL_STRING(where, ZSTR_VAL(temp_where));
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_WHERE_DATA), where);
        zend_string_release(temp_where);
        RETURN_ZVAL(getThis(), 1, 0);
    } else {
        php_error_docref("sql.where", E_ERROR, "Parameter need array or string value");
        RETURN_FALSE;
    }
}
/*}}}*/

/*{{{ proto MySql::groupBy(string $by_conditions)
    Set the select fields on which groupby conditions
 */
SPEED_METHOD(MySql, groupBy)
{
    zval *group_by = NULL;
    if ( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &group_by ) == FAILURE) {
        return ;
    }
    if ( Z_TYPE_P(group_by) == IS_STRING && group_by ) {
        zend_string *temp_group = strpprintf(0, " GROUP BY %s ", Z_STRVAL_P(group_by));
        ZVAL_STRING(group_by, ZSTR_VAL(temp_group));
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_GROUPBY_DATA), group_by);
        zend_string_release(temp_group);
        RETURN_ZVAL(getThis(), 1, 0);
    } else {
        php_error_docref("sql.groupBy", E_ERROR, "Parameter need string value");
        RETURN_FALSE
    }
}
/*}}}*/

/*{{{ proto MySql::having(array $having)
    Set the select fields on which having conditions
 */
SPEED_METHOD(MySql, having)
{
    zval *having = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &having) == FAILURE) {
        return ;
    }

    if ( Z_TYPE_P(having) == IS_ARRAY && having ) {
        /* array data */
        zend_string *var_name;
        zval *value;
        smart_str having_str = {0};
        smart_str_appends(&having_str, " HAVING ");
        ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(having), var_name, value) {
            if (Z_TYPE_P(value) == IS_STRING) {
                zend_string *str = strpprintf(0, "`%s`='%s'", ZSTR_VAL(var_name), Z_STRVAL_P(value));
                smart_str_appends(&having_str, ZSTR_VAL(str));
                smart_str_appends(&having_str, " AND ");
                zend_string_release(str);
            } else if (Z_TYPE_P(value) == IS_LONG) {
                zend_string *str = strpprintf(0, "`%s`='%d'", ZSTR_VAL(var_name), Z_STRVAL_P(value));
                smart_str_appends(&having_str, ZSTR_VAL(str));
                smart_str_appends(&having_str, " AND ");
                zend_string_release(str);
            }
        } ZEND_HASH_FOREACH_END();

        char *ss = (char *)malloc(sizeof(char) * (ZSTR_LEN(having_str.s) - 4) );
        memcpy(ss, ZSTR_VAL(having_str.s), ZSTR_LEN(having_str.s) - 5);
        ss[ZSTR_LEN(having_str.s) - 5] = '\0';
        ZVAL_STRING(having, ss);
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_HAVING_DATA), having);
        free(ss);
        RETURN_ZVAL(getThis(), 1, 0);

    } else if ( Z_TYPE_P(having) == IS_STRING && having ) {

        zend_string *temp_having = strpprintf(0, " HAVING %s ", Z_STRVAL_P(having));
        ZVAL_STRING(having, ZSTR_VAL(temp_having));
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_HAVING_DATA), having);
        zend_string_release(temp_having);
        RETURN_ZVAL(getThis(), 1, 0);

    } else {
        php_error_docref("sql.having", E_ERROR, "Parameter need array or string value");
        RETURN_FALSE
    }


}
/*}}}*/

/*{{{ proto MySql::orderBy()
    Set the select fields on which orderBy conditions
 */
SPEED_METHOD(MySql, orderBy)
{
    zval *order_by = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &order_by) == FAILURE) {
        return ;
    }
    if (Z_TYPE_P(order_by) == IS_STRING && order_by) {
        zend_string *temp_order = strpprintf(0, " ORDER BY %s ", Z_STRVAL_P(order_by));
        ZVAL_STRING(order_by, ZSTR_VAL(temp_order));
        zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_ORDERBY_DATA), order_by);
        zend_string_release(temp_order);
        RETURN_ZVAL(getThis(), 1, 0);
    } else {
        php_error_docref("sql.order_by", E_ERROR, "Parameter need string value");
        RETURN_FALSE
    }
}
/*}}}*/

/*{{{ proto MySql::limit()
    Set the select fields on which limit conditions
 */
SPEED_METHOD(MySql, limit)
{
    zval *count = NULL, *offset = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &count, &offset ) == FAILURE) {
        return ;
    }
    
    zend_string *temp_limit = NULL;

    if ( (Z_TYPE_P(count) == IS_LONG && count) && ( !offset ) ) {
        temp_limit = strpprintf( 0, " LIMIT %d ", Z_LVAL_P(count) );
    }
    if ( (Z_TYPE_P(count) == IS_LONG && count) && ( offset && ( Z_TYPE_P(offset) == IS_LONG ) ) ) {
        temp_limit = strpprintf( 0, " LIMIT %d, %d ", Z_LVAL_P(count), Z_LVAL_P(offset) );
    }
    if (!temp_limit) {
        php_error_docref("sql.limit", E_ERROR, "Parameter need string value for both.");
        RETURN_FALSE
    }
    zval temp_limit_str;
    ZVAL_STRING(&temp_limit_str, ZSTR_VAL(temp_limit));
    zend_update_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_LIMIT_DATA), &temp_limit_str);
    zend_string_release(temp_limit);
    RETURN_ZVAL(getThis(), 1, 0);
}
/*}}}*/

/*{{{ Return the build query sql */
zend_string *build_query_sql(zval *object)
{
    zval *select, *from, *where, *group_by, *having, *order_by, *limit;
    select      = zend_read_property(speed_mysql_ce, object, SPEED_STRL(SPEED_MYSQL_SELECT_DATA), 1, NULL);
    from        = zend_read_property(speed_mysql_ce, object, SPEED_STRL(SPEED_MYSQL_FROM_DATA), 1, NULL);
    where       = zend_read_property(speed_mysql_ce, object, SPEED_STRL(SPEED_MYSQL_WHERE_DATA), 1, NULL);
    group_by    = zend_read_property(speed_mysql_ce, object, SPEED_STRL(SPEED_MYSQL_GROUPBY_DATA), 1, NULL);
    having      = zend_read_property(speed_mysql_ce, object, SPEED_STRL(SPEED_MYSQL_HAVING_DATA), 1, NULL);
    order_by    = zend_read_property(speed_mysql_ce, object, SPEED_STRL(SPEED_MYSQL_ORDERBY_DATA), 1, NULL);
    limit       = zend_read_property(speed_mysql_ce, object, SPEED_STRL(SPEED_MYSQL_LIMIT_DATA), 1, NULL);

    zend_string *temp_query_sql = strpprintf(0, "SELECT %s FROM %s%s%s%s%s%s",
        Z_STRVAL_P(select),
        Z_STRVAL_P(from), 
        Z_ISNULL_P(where) ? "" : Z_STRVAL_P(where),
        Z_ISNULL_P(group_by) ? "" : Z_STRVAL_P(group_by),
        Z_ISNULL_P(having) ? "" : Z_STRVAL_P(having),
        Z_ISNULL_P(order_by) ? "" : Z_STRVAL_P(order_by),
        Z_ISNULL_P(limit) ? "" : Z_STRVAL_P(limit)
    );
    return temp_query_sql;
}
/*}}}*/

/* {{{ proto MySql::findOne()
 */
SPEED_METHOD(MySql, findOne)
{
    zval *pdo_object = zend_read_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_PDO_OBJECT), 1, NULL);

    zval p_retval;
    SPEED_CALL_FUNCTION(pdo_object, "prepare", &p_retval)
        uint32_t param_count = 1;
        zval prepare_sql;
        ZVAL_STRING(&prepare_sql, ZSTR_VAL(build_query_sql(getThis())));
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
    SPEED_CALL_FUNCTION(&p_retval, "fetch", &f_retval)
        uint32_t param_count = 1;
        zval fetch_style;
        ZVAL_LONG(&fetch_style, 2);
        zval params[] = {
            fetch_style
        };
    SPEED_END_CALL_FUNCTION();

    RETURN_ZVAL(&f_retval, 1, 0);
}
/*}}}*/

/* {{{ proto MySql::findAll()
 */
SPEED_METHOD(MySql, findAll)
{
    zval *pdo_object = zend_read_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_PDO_OBJECT), 1, NULL);

    zval p_retval;
    SPEED_CALL_FUNCTION(pdo_object, "prepare", &p_retval)
        uint32_t param_count = 1;
        zval prepare_sql;
        ZVAL_STRING(&prepare_sql, ZSTR_VAL(build_query_sql(getThis())));
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
}
/*}}}*/

/* {{{ proto MySql::query($rawSql, $bindParams)
    Run the raw sql select query
 */
SPEED_METHOD(MySql, query)
{
    zval *raw_sql = NULL;
    zval *bind_params = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &raw_sql, &bind_params) == FAILURE) {
        return ;
    }

    if ( raw_sql && Z_TYPE_P(raw_sql) == IS_STRING ) {

        zval *pdo_object = zend_read_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_PDO_OBJECT), 1, NULL);

        zval p_retval;
        SPEED_CALL_FUNCTION(pdo_object, "prepare", &p_retval)
            uint32_t param_count = 1;
            zval prepare_sql;
            ZVAL_STRING(&prepare_sql, Z_STRVAL_P(raw_sql));
            zval params[] = {
                prepare_sql
            };
        SPEED_END_CALL_FUNCTION();

        zval e_retval;
        if ( bind_params && Z_TYPE_P(bind_params) == IS_ARRAY ) {
            SPEED_CALL_FUNCTION(&p_retval, "execute", &e_retval)
                uint32_t param_count = 1;
                zval params[] = {
                    *bind_params
                };
            SPEED_END_CALL_FUNCTION();
        } else {
            SPEED_CALL_FUNCTION(&p_retval, "execute", &e_retval)
                uint32_t param_count = 0;
                zval *params = NULL;
            SPEED_END_CALL_FUNCTION();
        }
        
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
    } else {
        php_error_docref("sql.query", E_ERROR, "Parameter one need a string type value");
        RETURN_FALSE
    }

}
/*}}}*/

/* {{{ proto MySql::execute()
 */
SPEED_METHOD(MySql, execute)
{
    zval *raw_sql = NULL;
    zval *bind_params = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &raw_sql, &bind_params) == FAILURE) {
        return ;
    }

    if ( raw_sql && Z_TYPE_P(raw_sql) == IS_STRING ) {

        zval *pdo_object = zend_read_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_PDO_OBJECT), 1, NULL);

        zval p_retval;
        SPEED_CALL_FUNCTION(pdo_object, "prepare", &p_retval)
            uint32_t param_count = 1;
            zval prepare_sql;
            ZVAL_STRING(&prepare_sql, Z_STRVAL_P(raw_sql));
            zval params[] = {
                prepare_sql
            };
        SPEED_END_CALL_FUNCTION();

        zval e_retval;
        if ( bind_params && Z_TYPE_P(bind_params) == IS_ARRAY ) {
            SPEED_CALL_FUNCTION(&p_retval, "execute", &e_retval)
                uint32_t param_count = 1;
                zval params[] = {
                    *bind_params
                };
            SPEED_END_CALL_FUNCTION();
        } else {
            SPEED_CALL_FUNCTION(&p_retval, "execute", &e_retval)
                uint32_t param_count = 0;
                zval *params = NULL;
            SPEED_END_CALL_FUNCTION();
        }
        
        zval f_retval;
        SPEED_CALL_FUNCTION(&p_retval, "rowCount", &f_retval)
            uint32_t param_count = 0;
            zval *params = NULL;
        SPEED_END_CALL_FUNCTION();

        RETURN_ZVAL(&f_retval, 1, 0);
    } else {
        php_error_docref("sql.execute", E_ERROR, "Parameter one need a string type value");
        RETURN_FALSE
    }
}
/*}}}*/

/* {{{ proto MySql::insert($table, $fields = [], $data = [])
 */
SPEED_METHOD(MySql, insert)
{
    zval *table  = NULL;
    zval *fields = NULL;
    zval *data   = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &table, &fields, &data) == FAILURE) {
        return ;
    }

    if (!table || Z_TYPE_P(table) != IS_STRING ) {
        php_error_docref("sql.insert", E_ERROR, "Parameter one need a string type value");
        RETURN_FALSE
    }
    if (!fields || Z_TYPE_P(fields) != IS_ARRAY) {
        php_error_docref("sql.execute", E_ERROR, "Parameter two need a array value");
        RETURN_FALSE
    }
    if (!data || Z_TYPE_P(data) != IS_ARRAY) {
        php_error_docref("sql.execute", E_ERROR, "Parameter three need a array value");
        RETURN_FALSE
    }

    smart_str insert_sql = {0};
    smart_str_appends(&insert_sql, "INSERT INTO ");
    smart_str_appends(&insert_sql, Z_STRVAL_P(table));
    smart_str_appends(&insert_sql, " (");

    smart_str fields_sql = {0};
    zend_string *var_name;
    zval *value;
    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(fields), var_name, value) {
        zend_string *temp_fields = strpprintf(0, "`%s`,", Z_STRVAL_P(value));
        smart_str_appends(&fields_sql, ZSTR_VAL(temp_fields));
        zend_string_release(temp_fields);
    } ZEND_HASH_FOREACH_END();

    char *temp_t_fields = (char *)malloc(sizeof(char) * (ZSTR_LEN(fields_sql.s)));
    memcpy(temp_t_fields, ZSTR_VAL(fields_sql.s), ZSTR_LEN(fields_sql.s) - 1);
    temp_t_fields[ZSTR_LEN(fields_sql.s) - 1] = '\0';
    smart_str_free(&fields_sql);
    smart_str_appends(&insert_sql, temp_t_fields);
    free(temp_t_fields);
    smart_str_appendc(&insert_sql, ')');

    smart_str_appends(&insert_sql, " VALUES ");

    zend_string *data_name;
    zval *d_value;
    smart_str data_value = {0};

    smart_str_appends(&data_value, "(");
    ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(data), data_name, d_value){
        if (d_value && Z_TYPE_P(d_value) == IS_ARRAY) {
            /* If the value was array */
            zend_string *nest_name;
            zval *nest_value;
            smart_str temp_array_value = {0};

            smart_str_appendc(&temp_array_value, '(');
            zend_string *temp_array_data;
            ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(d_value), nest_name, nest_value) {
                if (Z_TYPE_P(nest_value) == IS_STRING) {
                    temp_array_data = strpprintf(0, "'%s',", Z_STRVAL_P(nest_value));
                } else if (Z_TYPE_P(nest_value) == IS_LONG) {
                    temp_array_data = strpprintf(0, "'%d',", Z_LVAL_P(nest_value));
                }
                smart_str_appends(&temp_array_value, ZSTR_VAL(temp_array_data));
            } ZEND_HASH_FOREACH_END();
            zend_string_release(temp_array_data);
            smart_str_appendl(&insert_sql, ZSTR_VAL(temp_array_value.s), ZSTR_LEN(temp_array_value.s) - 1);
            smart_str_appends(&insert_sql, "),");
            smart_str_free(&temp_array_value);
        } else if (Z_TYPE_P(d_value) == IS_STRING) {
            zend_string *temp_string_data = strpprintf(0, "'%s',", Z_STRVAL_P(d_value));
            smart_str_appends(&data_value, ZSTR_VAL(temp_string_data));
            zend_string_release(temp_string_data);
        } else if (Z_TYPE_P(d_value) == IS_LONG) {
            zend_string *temp_string_data = strpprintf(0, "'%d',", Z_LVAL_P(d_value));
            smart_str_appends(&data_value, ZSTR_VAL(temp_string_data));
            zend_string_release(temp_string_data);
        }
    } ZEND_HASH_FOREACH_END();

    char *temp_string_data_value = (char *)malloc(sizeof(char) * (ZSTR_LEN(data_value.s)));
    memcpy(temp_string_data_value, ZSTR_VAL(data_value.s), ZSTR_LEN(data_value.s) - 1);
    temp_string_data_value[ZSTR_LEN(data_value.s) - 1] = '\0';
    smart_str_appends(&insert_sql, temp_string_data_value);

    if ( *( ZSTR_VAL(insert_sql.s) + ZSTR_LEN(insert_sql.s) - 1 ) != ',') {
        smart_str_appendc(&insert_sql, ')');
    } else {
        *( ZSTR_VAL(insert_sql.s) + ZSTR_LEN(insert_sql.s) - 1 ) = '\0';
    }
    smart_str_0(&insert_sql);
    free(temp_string_data_value);
    smart_str_free(&data_value);

    zval *pdo_object = zend_read_property(speed_mysql_ce, getThis(), SPEED_STRL(SPEED_MYSQL_PDO_OBJECT), 1, NULL);

    zval p_retval;
    SPEED_CALL_FUNCTION(pdo_object, "prepare", &p_retval)
        uint32_t param_count = 1;
        zval prepare_sql;
        ZVAL_STRING(&prepare_sql, ZSTR_VAL(insert_sql.s));
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
    SPEED_CALL_FUNCTION(&p_retval, "rowCount", &f_retval)
        uint32_t param_count = 0;
        zval *params = NULL;
    SPEED_END_CALL_FUNCTION();

    RETURN_ZVAL(&f_retval, 1, 0);
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
    SPEED_ME(MySql, findOne, arginfo_speed_model_mysql_findone, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, findAll, arginfo_speed_model_mysql_findall, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, query, arginfo_speed_model_mysql_query, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, execute, arginfo_speed_model_mysql_execute, ZEND_ACC_PUBLIC)
    SPEED_ME(MySql, insert, arginfo_speed_model_mysql_insert, ZEND_ACC_PUBLIC)

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
    zend_declare_property_null(speed_mysql_ce, SPEED_STRL(SPEED_MYSQL_GROUPBY_DATA), ZEND_ACC_PROTECTED);
    zend_declare_property_null(speed_mysql_ce, SPEED_STRL(SPEED_MYSQL_HAVING_DATA), ZEND_ACC_PROTECTED);
    zend_declare_property_null(speed_mysql_ce, SPEED_STRL(SPEED_MYSQL_ORDERBY_DATA), ZEND_ACC_PROTECTED);
    zend_declare_property_null(speed_mysql_ce, SPEED_STRL(SPEED_MYSQL_LIMIT_DATA), ZEND_ACC_PROTECTED);
    zend_declare_property_null(speed_mysql_ce, SPEED_STRL(SPEED_MYSQL_PDO_OBJECT), ZEND_ACC_PROTECTED);
}
/*}}}*/