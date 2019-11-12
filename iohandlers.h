/*
 *  The Regina Rexx Interpreter
 *  Copyright (C) 1992-2016  Jan Lisowiec <jan.lisowiec@lzlabs.com>
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
 * iohandlers.h
 *
 *  Created on: Nov 27, 2015
 *  This module provides interface definitions for callback mechanism
 *  to call user=provided I/O functions when Regina REXX is loaded as
 *  a shared object
 */

#ifndef IOHANDLERS_H_
#define IOHANDLERS_H_

#ifdef REXX_8BIT_SUPPORT
size_t handle_fwrite(const void *buf, size_t buf_len, size_t count, FILE *file);
int handle_fputc(int c, FILE *file);
int handle_fflush(FILE *file);
int handle_fclose(FILE *file);

#define FWRITE(buf, buf_len, count, file) handle_fwrite((buf), (buf_len), (count), (file))
#define FPUTC(c, file) handle_fputc((c), (file))
#define FFLUSH(file) handle_fflush((file))
#define FCLOSE(file) handle_fclose((file))
#else
#define FWRITE(buf, buf_len, count, file) fwrite((buf), (buf_len), (count), (file))
#define FPUTC(c, file) fputc((c), (file))
#define FFLUSH(file) fflush((file))
#define FCLOSE(file) fclose((file))
#endif

typedef int (*regina_io_handler)(void *, char *buf, unsigned int buf_len);
typedef int (*regina_conv_handler)(void *, unsigned int);
typedef const char *(*regina_prompt_handler)(void *);
typedef void (*regina_check_eof_handler)(void *, int *);
typedef void (*regina_clear_error_handler)(void *, int *);
typedef void (*regina_check_plugin_version_handler)(void *, const char *ver);
typedef char*(*regina_program_fetch_handler)(void* arg, const char* path, FILE** retfp);
typedef int  (*regina_stack_read_handler)(void* arg, char** buf, int* ret_len);

#define PLUGIN_VERSION_NUMBER   "1.1"

typedef struct _ReginaIOHandlers {
    regina_io_handler inputInitHandler;                  /* Initialization handler for YY input */
    void              *inputInitArg;                     /* Arg for initialization handler for YY input */

    regina_io_handler inputGetcHandler;                  /* Get char handler for YY input */
    void              *getcHandlerArg;                   /* Arg for get char handler for YY input */

    regina_io_handler outputInitHandler;                 /* Initialization handler for YY output */
    void              *outputInitArg;                    /* Arg for initialization handler for YY output */

    regina_io_handler outputPutcHandler;                 /* Put char handler for YY output */
    void             *putcHandlerArg;                    /* Arg for put char handler for YY output */

    regina_io_handler outputFwriteHandler;               /* Fwrite handler to replace fwrite() */
    void             *fwriteHandlerArg;                  /* fwrite handler arg */

    regina_io_handler outputFflushHandler;               /* Fflush handler to replace fflush() */
    void             *fflushHandlerArg;                  /* fflush handler arg */

    regina_io_handler outputFcloseHandler;               /* Fclose handler to replace fclose() */
    void             *fcloseHandlerArg;                  /* fclose handler arg */

    regina_prompt_handler outputPutPromptHandler;        /* Put prompt handler for YY output */
    void             *outputPutPromptHandlerArg;         /* Arg for put prompt handler for YY output */

    regina_conv_handler inputConvHandler;                 /* Conversion handler for YY input */
    void             *inputConvHandlerArg;                /* Arg for conversion handler for YY input */

    regina_conv_handler outputConvHandler;                /* Conversion handler for YY output */
    void             *outputConvHandlerArg;               /* Arg for conversion handler for YY output */

    regina_check_eof_handler checkEofHandler;             /* Conversion handler for YY output */
    void             *checkEofHandlerArg;                 /* Arg for conversion handler for YY output */

    regina_clear_error_handler clearFileErrorHandler;     /* Clear file error handler */
    void             *clearFileErrorHandlerArg;           /* Clear file error handler argument */

    regina_check_plugin_version_handler checkVerHandler;  /* Checking version numebr */
    void             *checkVerHandlerArg;                 /* Arg for checking verion number */

    regina_io_handler subcomSystemHandler;                /* Subcom handler for SYSTEM env */
    void             *subcomSystemHandlerArg;             /* Subcom handler arg */

    regina_program_fetch_handler programFetchHandler;     /* RRT 15Apr2016 pgm fetch extension */
    void             *programFetchHandlerArg;             /* RRT 15Apr2016 arg */

    regina_stack_read_handler stackReadHandler;           /* RRT 15Apr2016 stack reader extension */
    void             *stackReadHandlerArg;                /* RRT 15Apr2016 arg */

    regina_io_handler inputAltGetcHandler;                /* Get char alternate handler for YY input */
    void              *getcAltHandlerArg;                 /* Arg for get char alternate handler for YY input */

    int               initialized;                        /* if set to '1' the handlers have been initialzied */
} ReginaIOHandlers;


void regina_io_handlers_zero();
int regina_io_handlers_initialized_get();
void regina_io_handlers_initialized_set();

int regina_close_on_return_handler(FILE *file_handles[], int file_handles_max, int *file_handles_size);

void regina_default_io_handlers();

int regina_register_input_init_handler(regina_io_handler inp_init, void *arg);
int regina_register_getc_handler(regina_io_handler inp_getc, void *arg);
int regina_register_putc_handler(regina_io_handler out_putc, void *arg);
int regina_register_fwrite_handler(regina_io_handler out_fwrite, void *arg);
int regina_register_fclose_handler(regina_io_handler out_fclose, void *arg);
int regina_register_fflush_handler(regina_io_handler out_fflush, void *arg);

int regina_register_output_init_handler(regina_io_handler out_init, void *arg);
int regina_register_put_prompt_handler(regina_prompt_handler prompt_handler, void *arg);
int regina_register_check_eof_handler(regina_check_eof_handler check_eof_handler, void *arg);
int regina_register_clear_error_handler(regina_clear_error_handler clear_error_handler, void *arg);
int regina_register_plugin_version_handler(regina_check_plugin_version_handler check_ver_handler, void *arg);
int regina_register_subcom_system_handler(regina_io_handler subcom_system_handler, void *arg);

int regina_register_input_conv_handler(regina_conv_handler inp_conv_handler, void *arg);
int regina_register_output_conv_handler(regina_conv_handler out_conv_handler, void *arg);

regina_conv_handler regina_input_conv_handler_get();
regina_conv_handler regina_output_conv_handler_get();
regina_prompt_handler regina_put_prompt_handler_get();
regina_check_eof_handler regina_check_eof_handler_get();
regina_clear_error_handler regina_clear_error_handler_get();

void dup_check_con(streng *ptr);
void dup_check_res(streng *ptr);
streng *streng_alloc_base_value(streng *ptr);
streng *streng_realloc_base_value(streng *ptr);
streng *streng_clone_value(streng *str);
streng *streng_clone_from_streng_base_value(streng *to, const streng *from);
streng *streng_clone_from_streng_nbase_value(streng *to, const streng *from, int offset);
streng *streng_clone_base_value(streng *ptr);
streng *Str_convert_TSD(streng *str);
streng *Str_restore_TSD(streng *str);
streng *streng_clone_and_restore(streng *str);
streng *streng_convert_printable(streng *str);
char Char_convert(const char c);
char Char_restore(const char c);
char Char_Toupper_8bit(const char c);
char Char_Tolower_8bit(const char c);
int Check_eof(int c);
void verify_sync_out();

extern ReginaIOHandlers io_handlers;

#endif /* IOHANDLERS_H_ */
