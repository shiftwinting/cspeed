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

#ifndef SUPJOS_SPEED_MODEL_MYSQL_H
#define SUPJOS_SPEED_MODEL_MYSQL_H

#define SPEED_MYSQL_PDO_OBJECT      "__speed_mysql_pdo_object"    /* To store the PDO Object */

#define SPEED_MYSQL_SELECT_DATA     "_speed_mysql_select_data"    /* To store the SQL SELECT */
#define SPEED_MYSQL_FROM_DATA       "_speed_mysql_from_data"      /* To store the SQL FROM */
#define SPEED_MYSQL_WHERE_DATA      "_speed_mysql_where_data"     /* To store the SQL WHERE */
#define SPEED_MYSQL_GROUPBY_DATA    "_speed_mysql_group_by_data"  /* To store the SQL GROUP BY */
#define SPEED_MYSQL_HAVING_DATA     "_speed_mysql_having_data"    /* To store the SQL HAVING */
#define SPEED_MYSQL_ORDERBY_DATA    "_speed_mysql_orderby_data"   /* To store the SQL ORDERBY */
#define SPEED_MYSQL_LIMIT_DATA      "_speed_mysql_limit_data"     /* To store the SQL LIMIT */

/* {{{ The class entry for zend-engine */
zend_class_entry *speed_mysql_ce;
/*}}}*/

#endif


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */