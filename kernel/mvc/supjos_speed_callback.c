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

#include "kernel/mvc/supjos_speed_callback.h"

/* {{{ ARG_INFO */
SPEED_BEGIN_ARG_INFO_EX(arginfo_speed_callback_init, 0, 0, 1)
    SPEED_ARG_INFO(0, controller)
SPEED_END_ARG_INFO()
/*}}}*/

/* {{{
    init method for the supjos\mvc\Callback function
*/
SPEED_METHOD(Callback, init)
{

}
/*}}}*/

/* {{{ 
    All methods for the interface of the supjos\mvc\Callback
*/
static const zend_function_entry speed_callback_functions[] = {
    SPEED_ABSTRACT_ME(Callback, init, arginfo_speed_callback_init)
    SPEED_FE_END
};
/*}}}*/

/* {{{
  Initailise function before this module can be used
 */
SPEED_STARTUP_FUNCTION(callback)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "supjos\\mvc", "Callback", speed_callback_functions);
    speed_callback_ce = zend_register_internal_interface(&ce);
}
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */