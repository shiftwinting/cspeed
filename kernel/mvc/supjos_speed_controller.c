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

#include "kernel/mvc/supjos_speed_controller.h"
#include "kernel/mvc/supjos_speed_view.h"

/* {{{ supjos\Controller ARG_INFO
 */
SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_controller_getView, 0, 0, 0)
SPEED_END_ARG_INFO()
/* }}} */

/* {{{ proto supjos\Controller::render($templ, $vars)
    Rendering the template with the given array variables
*/
SPEED_METHOD(Controller, getView)
{
    zval *speed_view_object = zend_read_property(speed_controller_ce, getThis(), ZEND_STRL(SPEED_CONTROLLER_VIEW_SETS), 1, NULL);
    if (Z_TYPE_P(speed_view_object) == IS_NULL) {
        zval speed_view_object_t;
        object_init_ex(&speed_view_object_t, speed_view_ce);
        /* zend_class_entry *scope, zval *object, const char *name, size_t name_length, zval *value */
        zval_add_ref(&speed_view_object_t);
        zend_update_property(speed_controller_ce, getThis(), ZEND_STRL(SPEED_CONTROLLER_VIEW_SETS), &speed_view_object_t);
        if (Z_TYPE(speed_view_object_t) == IS_OBJECT) {
            RETURN_ZVAL(&speed_view_object_t, 1, 0)
        }
    } else {
        RETURN_ZVAL(speed_view_object, 1, 0);
    }
}
/*}}}*/

/* {{{
    The supjos\Controller class's function collections
*/
static const zend_function_entry speed_controller_functions[] = {
    SPEED_ME(Controller, getView, arginfo_speed_controller_getView, ZEND_ACC_PUBLIC)
    SPEED_FE_END
};
/* }}}*/

/* {{{
    The module which will be loaded before the current class can be used in user-level
*/
SPEED_STARTUP_FUNCTION(controller)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "supjos\\mvc", "Controller", speed_controller_functions);
    speed_controller_ce = zend_register_internal_class(&ce);

    /* Set the default property null to store the View object */
    zend_declare_property_null(speed_controller_ce, ZEND_STRL(SPEED_CONTROLLER_VIEW_SETS),  ZEND_ACC_PROTECTED);
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