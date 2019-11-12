/*
 *  The Regina Rexx Interpreter
 *  Copyright (C) 1992-2019  Jan Lisowiec <jan.lisowiec@lzlabs.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * userhandlers.c
 *
 *  Created on: Feb 5, 2019
 *  This module provides interface definitions for callback mechanism
 *  to call user-provided application functions when Regina REXX is loaded as
 *  a shared object
 */


#include "rexx.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "iohandlers.h"
#include "userhandlers.h"
#include "strings.h"


void __attribute__ ((constructor)) user_handlers_init(void)
{
    memset(&user_handlers, '\0', sizeof(user_handlers));
}

ReginaUserHandlers user_handlers;

void regina_user_handlers_zero()
{
    memset(&user_handlers, '\0', sizeof(user_handlers));
}

int regina_user_handlers_initialized_get()
{
    return user_handlers.initialized;
}

void regina_user_handlers_initialized_set()
{
    user_handlers.initialized = 1;
}

int regina_register_user_parse_cmd_handler(regina_user_handler handler, void *arg)
{
    user_handlers.ParseCmdHandler = handler;
    user_handlers.ParseCmdArg = arg;
    return 0;
}

int regina_deregister_user_parse_cmd_handler()
{
    user_handlers.ParseCmdHandler = 0;
    user_handlers.ParseCmdArg = 0;
    return 0;
}

int regina_register_user_parse_source_handler(regina_user_handler handler, void *arg)
{
    user_handlers.ParseSourceHandler = handler;
    user_handlers.ParseSourceArg = arg;
    return 0;
}

int regina_register_user_parse_version_handler(regina_user_handler handler, void *arg)
{
    user_handlers.ParseVersionHandler = handler;
    user_handlers.ParseVersionArg = arg;
    return 0;
}

int regina_register_user_parse_numeric_handler(regina_user_handler handler, void *arg)
{
    user_handlers.ParseNumericHandler = handler;
    user_handlers.ParseNumericArg = arg;
    return 0;
}

int regina_register_user_get_error_line_num_handler(regina_user_handler1 handler, void *arg)
{
    user_handlers.GetErrorLineNumHandler = handler;
    user_handlers.GetErrorLineNumArg = arg;
    return 0;
}

int regina_register_user_get_error_file_name_handler(regina_user_handler2 handler, void *arg)
{
    user_handlers.GetErrorFileNameHandler = handler;
    user_handlers.GetErrorFileNameArg = arg;
    return 0;
}

int regina_register_user_get_error_fun_name_handler(regina_user_handler2 handler, void *arg)
{
    user_handlers.GetErrorFunNameHandler = handler;
    user_handlers.GetErrorFunNameArg = arg;
    return 0;
}

int regina_register_user_get_error_proc_name_handler(regina_user_handler2 handler, void *arg)
{
    user_handlers.GetErrorProcNameHandler = handler;
    user_handlers.GetErrorProcNameArg = arg;
    return 0;
}

int regina_register_user_tso_pull_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoPullHandler = handler;
    user_handlers.TsoPullArg = arg;
    return 0;
}

int regina_register_user_tso_push_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoPushHandler = handler;
    user_handlers.TsoPushArg = arg;
    return 0;
}

int regina_register_user_tso_queue_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoQueueHandler = handler;
    user_handlers.TsoQueueArg = arg;
    return 0;
}

int regina_register_user_tso_queued_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoQueuedHandler = handler;
    user_handlers.TsoQueuedArg = arg;
    return 0;
}

int regina_register_user_tso_executil_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoExecutilHandler = handler;
    user_handlers.TsoExecutilArg = arg;
    return 0;
}

int regina_register_user_tso_newstack_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoNewstackHandler = handler;
    user_handlers.TsoNewstackArg = arg;
    return 0;
}

int regina_register_user_tso_delstack_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoDelstackHandler = handler;
    user_handlers.TsoDelstackArg = arg;
    return 0;
}

int regina_register_user_tso_qstack_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoQstackHandler = handler;
    user_handlers.TsoQstackArg = arg;
    return 0;
}

int regina_register_user_tso_dropbuf_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoDropbufHandler = handler;
    user_handlers.TsoDropbufArg = arg;
    return 0;
}

int regina_register_user_tso_makebuf_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoMakebufHandler = handler;
    user_handlers.TsoMakebufArg = arg;
    return 0;
}

int regina_register_user_tso_he_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoHeHandler = handler;
    user_handlers.TsoHeArg = arg;
    return 0;
}

int regina_register_user_tso_hi_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoHiHandler = handler;
    user_handlers.TsoHiArg = arg;
    return 0;
}

int regina_register_user_tso_ht_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoHtHandler = handler;
    user_handlers.TsoHtArg = arg;
    return 0;
}

int regina_register_user_tso_qbuf_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoQbufHandler = handler;
    user_handlers.TsoQbufArg = arg;
    return 0;
}

int regina_register_user_tso_qelem_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoQelemHandler = handler;
    user_handlers.TsoQelemArg = arg;
    return 0;
}

int regina_register_user_tso_rt_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoRtHandler = handler;
    user_handlers.TsoRtArg = arg;
    return 0;
}

int regina_register_user_tso_te_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoTeHandler = handler;
    user_handlers.TsoTeArg = arg;
    return 0;
}

int regina_register_user_tso_ts_handler(regina_user_handler handler, void *arg)
{
    user_handlers.TsoTsHandler = handler;
    user_handlers.TsoTsArg = arg;
    return 0;
}

