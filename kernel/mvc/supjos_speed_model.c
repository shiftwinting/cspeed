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

/*{{{ ARG_INFO
 */
SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_select, 0, 0, 0)
    SPEED_ARG_INFO(0, fields)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_from, 0, 0, 0)
    SPEED_ARG_INFO(0, table)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_where, 0, 0, 0)
    SPEED_ARG_INFO(0, conditions)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_groupby, 0, 0, 0)
    SPEED_ARG_INFO(0, by)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_having, 0, 0, 0)
    SPEED_ARG_INFO(0, having)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_orderby, 0, 0, 0)
    SPEED_ARG_INFO(0, orderby)
SPEED_END_ARG_INFO()

SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_model_limit, 0, 0, 0)
    SPEED_ARG_INFO(0, limit)
SPEED_END_ARG_INFO()
/*}}}*/

/*{{{ proto Model::select()
    Set the select fields for the SELECT query
 */
SPEED_METHOD(Model, select)
{

}
/*}}}*/

/*{{{ proto Model::from()
    Set the select fields for which table
 */
SPEED_METHOD(Model, from)
{

}
/*}}}*/

/*{{{ proto Model::where()
    Set the select fields on which conditions
 */
SPEED_METHOD(Model, where)
{

}
/*}}}*/

/*{{{ proto Model::groupBy()
    Set the select fields on which groupby conditions
 */
SPEED_METHOD(Model, groupBy)
{

}
/*}}}*/

/*{{{ proto Model::having()
    Set the select fields on which having conditions
 */
SPEED_METHOD(Model, having)
{

}
/*}}}*/

/*{{{ proto Model::orderBy()
    Set the select fields on which orderBy conditions
 */
SPEED_METHOD(Model, orderBy)
{

}
/*}}}*/

/*{{{ proto Model::limit()
    Set the select fields on which limit conditions
 */
SPEED_METHOD(Model, limit)
{

}
/*}}}*/

/*{{{
    zend_function_entry all methods for the Model class
 */
static const zend_function_entry speed_model_functions[] = {
    SPEED_ABSTRACT_ME(Model, select , arginfo_speed_model_select)
    SPEED_ABSTRACT_ME(Model, from , arginfo_speed_model_from)
    SPEED_ABSTRACT_ME(Model, where , arginfo_speed_model_where)
    SPEED_ABSTRACT_ME(Model, groupBy , arginfo_speed_model_groupby)
    SPEED_ABSTRACT_ME(Model, having , arginfo_speed_model_having)
    SPEED_ABSTRACT_ME(Model, orderBy , arginfo_speed_model_orderby)
    SPEED_ABSTRACT_ME(Model, limit , arginfo_speed_model_limit)

    SPEED_FE_END
};
/*}}}*/

/*{{{
    model initialise method
 */
SPEED_STARTUP_FUNCTION(model)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "supjos\\mvc", "Model", speed_model_functions);
    speed_model_ce = zend_register_internal_interface(&ce);
}
/*}}}*/

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */