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

#ifndef SUPJOS_SPEED_DI_H
#define SUPJOS_SPEED_DI_H

/* {{{
  container to store the objects
*/
#define SUPJOS_SPEED_DI_CONTAINER "_speed_container"
/*}}}*/

/* {{{
  The supjos\tool\di class entry
 */
zend_class_entry *speed_di_ce;
/* }}}*/
#endif