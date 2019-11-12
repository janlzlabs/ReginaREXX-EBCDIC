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
 * userhandlers.h
 *
 *  Created on: Feb 5, 2019
 *  This module provides interface definitions for callback mechanism
 *  to call user-provided application functions when Regina REXX is loaded as
 *  a shared object
 */

#ifndef USERHANDLERS_H_
#define USERHANDLERS_H_

typedef int (*regina_user_handler)(void *, const char *cmd, char *out_buf, unsigned int out_buf_len);
typedef int (*regina_user_handler1)(void *, unsigned int);
typedef int (*regina_user_handler2)(void *, const char *);

typedef struct _ReginaUserHandlers {
    regina_user_handler   ParseCmdHandler;                  /* PARSE <CMD> handler */
    void                 *ParseCmdArg;                      /* PARSE <CMD> handler arg */

    regina_user_handler   ParseSourceHandler;               /* PARSE SOURCE handler */
    void                 *ParseSourceArg;                   /* PARSE SOURCE handler arg */

    regina_user_handler   ParseVersionHandler;              /* PARSE VERSION handler */
    void                 *ParseVersionArg;                  /* PARSE VERSION handler arg */

    regina_user_handler   ParseNumericHandler;              /* PARSE NUMERIC handler*/
    void                 *ParseNumericArg;                  /* PARSE NUMERIC handler arg */

    regina_user_handler1  GetErrorLineNumHandler;           /* Get line number where an error occurred */
    void                 *GetErrorLineNumArg;

    regina_user_handler2  GetErrorFileNameHandler;          /* Get file name where an error occurred */
    void                 *GetErrorFileNameArg;

    regina_user_handler2  GetErrorFunNameHandler;           /* Get function name where an error occurred */
    void                 *GetErrorFunNameArg;

    regina_user_handler2  GetErrorProcNameHandler;          /* Get procedure name where an error occurred */
    void                 *GetErrorProcNameArg;

    regina_user_handler TsoPullHandler;                   /* TSO PULL handler*/
    void               *TsoPullArg;                       /* TSO PULL handler arg */

    regina_user_handler TsoPushHandler;                   /* TSO PUSH handler*/
    void               *TsoPushArg;                       /* TSO PUSH handler arg */

    regina_user_handler TsoQueueHandler;                  /* TSO QUEUE handler*/
    void               *TsoQueueArg;                      /* TSO QUEUE handler arg */

    regina_user_handler TsoQueuedHandler;                 /* TSO build-in QUEUED() handler*/
    void               *TsoQueuedArg;                     /* TSO build-in QUEUED() arg */

    regina_user_handler TsoExecutilHandler;               /* TSO EXECUTIL handler*/
    void               *TsoExecutilArg;                   /* TSO EXECUTIL handler arg */

    regina_user_handler TsoNewstackHandler;               /* TSO NEWSTACK handler*/
    void               *TsoNewstackArg;                   /* TSO NEWSTACK handler arg */

    regina_user_handler TsoDelstackHandler;               /* TSO DELSTACK handler*/
    void               *TsoDelstackArg;                   /* TSO DELSTACK handler arg */

    regina_user_handler TsoQstackHandler;                 /* TSO QSTACK handler*/
    void               *TsoQstackArg;                     /* TSO QSTACK handler arg */

    regina_user_handler TsoDropbufHandler;                /* TSO DROPBUF handler*/
    void               *TsoDropbufArg;                    /* TSO DROPBUF handler arg */

    regina_user_handler TsoMakebufHandler;                /* TSO MAKEBUF handler*/
    void               *TsoMakebufArg;                    /* TSO MAKEBUF handler arg */

    regina_user_handler TsoHeHandler;                     /* TSO HE handler*/
    void               *TsoHeArg;                         /* TSO HE handler arg */

    regina_user_handler TsoHiHandler;                     /* TSO HI handler*/
    void               *TsoHiArg;                         /* TSO HI handler arg */

    regina_user_handler TsoHtHandler;                     /* TSO HT handler*/
    void               *TsoHtArg;                         /* TSO HT handler arg */

    regina_user_handler TsoQbufHandler;                   /* TSO QBUF handler*/
    void               *TsoQbufArg;                       /* TSO QBUF handler arg */

    regina_user_handler TsoQelemHandler;                  /* TSO QELEM handler*/
    void               *TsoQelemArg;                      /* TSO QELEM handler arg */

    regina_user_handler TsoRtHandler;                     /* TSO RT handler*/
    void               *TsoRtArg;                         /* TSO RT handler arg */

    regina_user_handler TsoTeHandler;                     /* TSO TE handler*/
    void               *TsoTeArg;                         /* TSO TE handler arg */

    regina_user_handler TsoTsHandler;                     /* TSO TS handler*/
    void               *TsoTsArg;                         /* TSO TS handler arg */

    int               initialized;                        /* if set to '1' the handlers have been initialized */
} ReginaUserHandlers;


void regina_user_handlers_zero();
int regina_user_handlers_initialized_get();
void regina_user_handlers_initialized_set();

int regina_register_user_parse_cmd_handler(regina_user_handler handler, void *arg);
int regina_deregister_user_parse_cmd_handler();
int regina_register_user_parse_source_handler(regina_user_handler handler, void *arg);
int regina_register_user_parse_version_handler(regina_user_handler handler, void *arg);
int regina_register_user_parse_numeric_handler(regina_user_handler handler, void *arg);

int regina_register_user_get_error_line_num_handler(regina_user_handler1 handler, void *arg);
int regina_register_user_get_error_file_name_handler(regina_user_handler2 handler, void *arg);
int regina_register_user_get_error_fun_name_handler(regina_user_handler2 handler, void *arg);
int regina_register_user_get_error_proc_name_handler(regina_user_handler2 handler, void *arg);

int regina_register_user_tso_pull_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_push_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_queue_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_queued_handler(regina_user_handler handler, void *arg);

int regina_register_user_tso_executil_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_newstack_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_delstack_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_qstack_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_dropbuf_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_makebuf_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_he_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_hi_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_ht_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_qbuf_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_qelem_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_rt_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_te_handler(regina_user_handler handler, void *arg);
int regina_register_user_tso_ts_handler(regina_user_handler handler, void *arg);

extern ReginaUserHandlers user_handlers;

#endif /* USERHANDLERS_H_ */
