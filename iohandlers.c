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
 * iohandlers.c
 *
 *  Created on: Jan 7, 2016
 *  This module provides functions required for 8BIT support
 */


#include "rexx.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#include "iohandlers.h"
#include "strings.h"


#define yyin __reginain
#define yyout __reginaout

extern FILE *yyin;
extern FILE *yyout;
extern FILE *yyin_io;
extern FILE *yyout_io;

void __attribute__ ((constructor)) io_handlers_init(void)
{
    memset(&io_handlers, '\0', sizeof(io_handlers));
    //fprintf(stderr, "REGINA IO HANDLERS INITIALIZED\n");
}

ReginaIOHandlers io_handlers;

int regina_output_message_handler(const char *msg, int msg_len)
{
    char buf[512];
    int ret;

    if ( !yyout_io ) {
        return 0;
    }
#define MIN(a,b) ((a) < (b) ? (a) : (b))
    memcpy(buf, msg, MIN(sizeof(buf), msg_len));
    if ( regina_output_conv_handler_get() ) {
        regina_output_conv_handler_get()(buf, MIN(sizeof(buf), msg_len));
    }

    ret = FWRITE(buf, msg_len, 1, yyout_io);
    FPUTC( REGINA_EOL, yyout_io);
    FFLUSH(yyout_io);

    return ret;
}

static int defaultGetcHandler(void *arg)
{
    FILE *stream = (FILE *)arg;

    int c = getc(stream);
    //if ( debug_getc )
        //fprintf(stderr, "%c", (char)c);
    return c;
}

void regina_io_handlers_zero()
{
    memset(&io_handlers, '\0', sizeof(io_handlers));
}

int regina_io_handlers_initialized_get()
{
    return io_handlers.initialized;
}

void regina_io_handlers_initialized_set()
{
    io_handlers.initialized = 1;
}

int regina_register_input_init_handler(regina_io_handler inp_init, void *arg)
{
    io_handlers.inputInitHandler = inp_init;
    io_handlers.inputInitArg = arg;
    return 0;
}

int regina_register_getc_handler(regina_io_handler inp_getc, void *arg)
{
    io_handlers.inputGetcHandler = inp_getc;
    io_handlers.getcHandlerArg = arg;
    yyin = (FILE *)io_handlers.getcHandlerArg;
    yyin_io = yyin;
    return 0;
}

int regina_register_output_init_handler(regina_io_handler out_init, void *arg)
{
    io_handlers.outputInitHandler = out_init;
    io_handlers.outputInitArg = arg;
    return 0;
}

int regina_register_putc_handler(regina_io_handler out_putc, void *arg)
{
    FILE *yy_tmp;

    io_handlers.outputPutcHandler = out_putc;
    io_handlers.putcHandlerArg = arg;

    // we get yyout and yyout_io from fd passed in, in case soem code still relies on it
    if ( io_handlers.putcHandlerArg && *(int *)io_handlers.putcHandlerArg >= 0 ) {
        yy_tmp = fdopen(*(int *)io_handlers.putcHandlerArg, "w");
        if ( !yy_tmp ) {
            fprintf(stderr, "Get FILE* from fd failed errno=%d\n", errno);
            return -1;
        }

        yyout = yy_tmp;
        yyout_io = yyout;
    }
    return 0;
}

int regina_register_fwrite_handler(regina_io_handler out_fwrite, void *arg)
{
    io_handlers.outputFwriteHandler = out_fwrite;
    io_handlers.fwriteHandlerArg = arg;
    return 0;
}

int regina_register_fclose_handler(regina_io_handler out_fclose, void *arg)
{
    io_handlers.outputFcloseHandler = out_fclose;
    io_handlers.fcloseHandlerArg = arg;
    return 0;
}

int regina_register_fflush_handler(regina_io_handler out_fflush, void *arg)
{
    io_handlers.outputFflushHandler = out_fflush;
    io_handlers.fflushHandlerArg = arg;
    return 0;
}

int regina_register_input_conv_handler(regina_conv_handler inp_conv_handler, void *arg)
{
    io_handlers.inputConvHandlerArg = arg;
    io_handlers.inputConvHandler = inp_conv_handler;
    return 0;
}

int regina_register_output_conv_handler(regina_conv_handler out_conv_handler, void *arg)
{
    io_handlers.outputConvHandlerArg = arg;
    io_handlers.outputConvHandler = out_conv_handler;
    return 0;
}

int regina_register_put_prompt_handler(regina_prompt_handler prompt_handler, void *arg)
{
    io_handlers.outputPutPromptHandlerArg = arg;
    io_handlers.outputPutPromptHandler = prompt_handler;
    return 0;
}

int regina_register_check_eof_handler(regina_check_eof_handler check_eof_handler, void *arg)
{
    io_handlers.checkEofHandlerArg = arg;
    io_handlers.checkEofHandler = check_eof_handler;
    return 0;
}

int regina_register_clear_error_handler(regina_clear_error_handler clear_error_handler, void *arg)
{
    io_handlers.clearFileErrorHandlerArg = arg;
    io_handlers.clearFileErrorHandler = clear_error_handler;
    return 0;
}

int regina_register_plugin_version_handler(regina_check_plugin_version_handler check_ver_handler, void *arg)
{
    io_handlers.checkVerHandlerArg = arg;
    io_handlers.checkVerHandler = check_ver_handler;
    return 0;
}

int regina_register_subcom_system_handler(regina_io_handler subcom_system_handler, void *arg)
{
    io_handlers.subcomSystemHandlerArg = arg;
    io_handlers.subcomSystemHandler = subcom_system_handler;
    return 0;
}

int regina_register_program_fetch_handler(regina_program_fetch_handler program_fetch_handler, void *arg)
{
    io_handlers.programFetchHandlerArg = arg;
    io_handlers.programFetchHandler = program_fetch_handler;
    return 0;
}

int regina_register_stack_read_handler(regina_stack_read_handler stack_read_handler, void *arg)
{
    io_handlers.stackReadHandlerArg = arg;
    io_handlers.stackReadHandler = stack_read_handler;
    return 0;
}

int regina_register_alt_getc_handler(regina_io_handler inp_getc, void *arg)
{
    io_handlers.inputAltGetcHandler = inp_getc;
    io_handlers.getcAltHandlerArg = arg;
    return 0;
}

regina_conv_handler regina_input_conv_handler_get()
{
    return io_handlers.inputConvHandler;
}

regina_conv_handler regina_output_conv_handler_get()
{
    return io_handlers.outputConvHandler;
}

regina_prompt_handler regina_put_prompt_handler_get()
{
    return io_handlers.outputPutPromptHandler;
}

regina_check_eof_handler regina_check_eof_handler_get()
{
    return io_handlers.checkEofHandler;
}

regina_clear_error_handler regina_clear_error_handler_get()
{
    return io_handlers.clearFileErrorHandler;
}


int regina_close_on_return_handler(FILE *file_handles[], int file_handles_max, int *file_handles_size)
{
    return 0;
}

void regina_default_io_handlers()
{
#ifdef REXXDEBUG
   fprintf(stderr, "func=%s\n", __FUNCTION__);
#endif

    memset(&io_handlers, '\0', sizeof(io_handlers));
    io_handlers.inputGetcHandler = defaultGetcHandler;

    io_handlers.getcHandlerArg = yyin;
    yyin_io = io_handlers.getcHandlerArg;

    io_handlers.putcHandlerArg = yyout;
    yyout_io = io_handlers.putcHandlerArg;

    io_handlers.initialized = 1;
}

void verify_sync_out()
{
    regina_default_io_handlers();
}

size_t handle_fwrite(const void *buf, size_t buf_len, size_t count, FILE *file)
{
    if ( io_handlers.outputFwriteHandler ) {
        return io_handlers.outputFwriteHandler(io_handlers.fwriteHandlerArg, buf, buf_len*count);
    }
    return fwrite(buf, buf_len, count, file);
}

int handle_fputc(int c, FILE *file)
{
    if ( io_handlers.outputPutcHandler ) {
        return io_handlers.outputPutcHandler(io_handlers.putcHandlerArg, &c, 1);
    }
    return fputc(c, file);
}

int handle_fflush(FILE *file)
{
    if ( io_handlers.outputFflushHandler ) {
        return io_handlers.outputFflushHandler(io_handlers.fflushHandlerArg, (char *)file, 0);
    }
    return fflush(file);
}

int handle_fclose(FILE *file)
{
    if ( io_handlers.outputFcloseHandler ) {
        return io_handlers.outputFcloseHandler(io_handlers.fcloseHandlerArg, (char *)file, 0);
    }
    return fclose(file);
}

#if defined(REXXDEBUG_STRING) || defined(REXXDEBUG)

static unsigned char a2e[] =
        {
                /*      .0  .1  .2  .3  .4  .5  .6  .7  .8  .9  .A  .B  .C  .D  .E  .F*/
                /*0.*/"\x00\x01\x02\x03\x37\x2D\x2E\x2F\x16\x05\x25\x0B\x0C\x0D\x0E\x0F"
                /*1.*/"\x10\x11\x12\x13\x3C\x3D\x32\x26\x18\x19\x1A\x27\x22\x1D\x35\x1F"
                /*2.*/"\x40\x5A\x7F\x7B\x5B\x6C\x50\x7D\x4D\x5D\x5C\x4E\x6B\x60\x4B\x61"
                /*3.*/"\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\x7A\x5E\x4C\x7E\x6E\x6F"
                /*4.*/"\x7C\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xD1\xD2\xD3\xD4\xD5\xD6"
                /*5.*/"\xD7\xD8\xD9\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xAD\xE0\xBD\x5F\x6D"
                /*6.*/"\x79\x81\x82\x83\x84\x85\x86\x87\x88\x89\x91\x92\x93\x94\x95\x96"
                /*7.*/"\x97\x98\x99\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xC0\x6A\xD0\xA1\x07"
                /*8.*/"\x68\xDC\x51\x42\x43\x44\x47\x48\x52\x53\x54\x57\x56\x58\x63\x67"
                /*9.*/"\x71\x9C\x9E\xCB\xCC\xCD\xDB\xDD\xDF\xEC\xFC\xB0\xB1\xB2\xB3\xB4"
                /*A.*/"\x45\x55\xCE\xDE\x49\x69\x04\x06\xAB\x08\xBA\xB8\xB7\xAA\x8A\x8B"
                /*B.*/"\x09\x0A\x14\xBB\x15\xB5\xB6\x17\x1B\xB9\x1C\x1E\xBC\x20\xBE\xBF"
                /*C.*/"\x21\x23\x24\x28\x29\x2A\x2B\x2C\x30\x31\xCA\x33\x34\x36\x38\xCF"
                /*D.*/"\x39\x3A\x3B\x3E\x41\x46\x4A\x4F\x59\x62\xDA\x64\x65\x66\x70\x72"
                /*E.*/"\x73\xE1\x74\x75\x76\x77\x78\x80\x8C\x8D\x8E\xEB\x8F\xED\xEE\xEF"
                /*F.*/"\x90\x9A\x9B\x9D\x9F\xA0\xAC\xAE\xAF\xFD\xFE\xFB\x3F\xEA\xFA\xFF" };

static unsigned char e2a[] =
        {
                /*      .0  .1  .2  .3  .4  .5  .6  .7  .8  .9  .A  .B  .C  .D  .E  .F*/
                /*0.*/"\x00\x01\x02\x03\xA6\x09\xA7\x7F\xA9\xB0\xB1\x0B\x0C\x0D\x0E\x0F"
                /*1.*/"\x10\x11\x12\x13\xB2\x0A\x08\xB7\x18\x19\x1A\xB8\xBA\x1D\xBB\x1F"
                /*2.*/"\xBD\xC0\x1C\xC1\xC2\x0A\x17\x1B\xC3\xC4\xC5\xC6\xC7\x05\x06\x07"
                /*3.*/"\xC8\xC9\x16\xCB\xCC\x1E\xCD\x04\xCE\xD0\xD1\xD2\x14\x15\xD3\xFC"
                /*4.*/"\x20\xD4\x83\x84\x85\xA0\xD5\x86\x87\xA4\xD6\x2E\x3C\x28\x2B\xD7"
                /*5.*/"\x26\x82\x88\x89\x8A\xA1\x8C\x8B\x8D\xD8\x21\x24\x2A\x29\x3B\x5E"
                /*6.*/"\x2D\x2F\xD9\x8E\xDB\xDC\xDD\x8F\x80\xA5\x7C\x2C\x25\x5F\x3E\x3F"
                /*7.*/"\xDE\x90\xDF\xE0\xE2\xE3\xE4\xE5\xE6\x60\x3A\x23\x40\x27\x3D\x22"
                /*8.*/"\xE7\x61\x62\x63\x64\x65\x66\x67\x68\x69\xAE\xAF\xE8\xE9\xEA\xEC"
                /*9.*/"\xF0\x6A\x6B\x6C\x6D\x6E\x6F\x70\x71\x72\xF1\xF2\x91\xF3\x92\xF4"
                /*A.*/"\xF5\x7E\x73\x74\x75\x76\x77\x78\x79\x7A\xAD\xA8\xF6\x5B\xF7\xF8"
                /*B.*/"\x9B\x9C\x9D\x9E\x9F\xB5\xB6\xAC\xAB\xB9\xAA\xB3\xBC\x5D\xBE\xBF"
                /*C.*/"\x7B\x41\x42\x43\x44\x45\x46\x47\x48\x49\xCA\x93\x94\x95\xA2\xCF"
                /*D.*/"\x7D\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\xDA\x96\x81\x97\xA3\x98"
                /*E.*/"\x5C\xE1\x53\x54\x55\x56\x57\x58\x59\x5A\xFD\xEB\x99\xED\xEE\xEF"
                /*F.*/"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\xFE\xFB\x9A\xF9\xFA\xFF" };

static void debug_ebcdic_to_ascii_convert(char *ab, unsigned int ab_size, const char *eb, unsigned int eb_size)
{
    char *ptr;
    for ( ptr = eb ; *ptr && ptr - eb < ab_size ; ptr++ ) {
        ab[ptr - eb] = e2a[eb[ptr - eb]];
    }
    ab[ptr - eb] = '\0';
}

void print_stre(const FILE *fp, const char *chars, const streng *str)
{
    char buf[1024];
    char ascii_buf[1024];
    char *buf_ptr = buf;

    memset(buf, '\0', sizeof(buf));
    if ( str ) {
        memcpy(buf, str->value, str->len);
        if ( chars ) {
            if ( !strcmp(chars, "EBCDIC") ) {
                fprintf(fp, " (EBCDIC) ");
                debug_ebcdic_to_ascii_convert(ascii_buf, sizeof(ascii_buf), buf, sizeof(buf));
                buf_ptr = ascii_buf;
            } else if ( !strcmp(chars, "ASCII") ) {
                fprintf(fp, " (ASCII) ");
            } else {
                fprintf(fp, " (UNKNOWN) ");
            }
        }
#ifdef REXXDEBUG_STRING
        {
            int ii;
            fprintf(fp, " s->value=%s s->len=%d hex=(", buf_ptr, str->len);
            for ( ii = 0 ; ii < str->len ; ii++ ) {
                fprintf(fp, "%02x", buf_ptr[ii]);
            }
            fprintf(fp, ") ");
        }
#else
        fprintf(fp, "s->value=%s s->len=%d ", buf_ptr, str->len);
#endif
#ifdef REXX_8BIT_SUPPORT
#ifdef REXXDEBUG_STRING
        {
            int ii;
            fprintf(fp, "con=%d s->base_value=%s hex=(", str->converted, str->base_value);
            for ( ii = 0 ; ii < str->len ; ii++ ) {
                fprintf(fp, "%02x", str->base_value[ii]);
            }
            fprintf(fp, ") ");
        }
#else
        fprintf(fp, "con=%d s->base_value=%s ", str->converted, str->base_value);
#endif
#endif
    } else {
        fprintf(fp, "s->name=<NULL> ");
    }
}
#endif

#ifdef REXX_8BIT_SUPPORT

void dup_check_con(streng *ptr)
{
#ifdef REXXDEBUG_STRING
    if ( ptr->value && ptr->base_value && ptr->len && !memcmp(ptr->value, ptr->base_value, ptr->len) ) {
        fprintf(stderr, "DUPEK_CON value=%s len=%d\n", ptr->value, ptr->len);
    }
#endif
}

void dup_check_res(streng *ptr)
{
#ifdef REXXDEBUG_STRING
    if ( ptr->value && ptr->base_value && ptr->len && !memcmp(ptr->value, ptr->base_value, ptr->len) ) {
        fprintf(stderr, "DUPEK_RES value=%s len=%d\n", ptr->value, ptr->len);
    }
#endif
}

streng *streng_alloc_base_value(streng *ptr)
{
    int size;
    if ( !ptr || ptr->base_value ) {
        return ptr;
    }
    size = ptr->max < ptr->len ? ptr->len + 4 : ptr->max + 4;
    ptr->base_value = malloc(size);
    memset(ptr->base_value, '\0', size);

    return ptr;
}

streng *streng_realloc_base_value(streng *ptr)
{
    int size;
    if ( !ptr ) {
        return ptr;
    }
    if ( ptr->base_value ) {
        free(ptr->base_value);
    }
    size = ptr->max < ptr->len ? ptr->len + 4 : ptr->max + 4;
    ptr->base_value = malloc(size);
    memset(ptr->base_value, '\0', size);

    return ptr;
}

streng *streng_clone_value(streng *str)
{
    if ( !str || !str->base_value || !str->value ) {
        return str;
    }
    memcpy(str->base_value, str->value, str->len );
    CLEAR_STRENG_RESTORED(str->converted);

    return str;
}

streng *streng_clone_from_streng_base_value(streng *to, const streng *from)
{
    if ( !to || !from || !to->base_value || !from->base_value ) {
        return to;
    }
    memcpy(to->base_value, from->base_value, from->len );

    return to;
}

streng *streng_clone_from_streng_nbase_value(streng *to, const streng *from, int offset)
{
    if ( !to || !from || !to->base_value || !from->base_value || to->len < from->len ) {
        return to;
    }
    memcpy(to->base_value, from->base_value + offset, from->len );

    return to;
}

streng *streng_clone_base_value(streng *ptr)
{
    if ( !ptr || !ptr->value || !ptr->base_value ) {
        return ptr;
    }
    if ( GET_STRENG_VARIABLE(ptr->converted) ) {
        return ptr;
    }
    memcpy(ptr->value, ptr->base_value, ptr->len );
    CLEAR_STRENG_CONVERTED(ptr->converted);

    return ptr;
}

streng *streng_convert_printable(streng *ptr)
{
    if ( !ptr || !ptr->value || !ptr->base_value ) {
        return ptr;
    }
    if ( GET_STRENG_VARIABLE(ptr->converted) ) {
        return ptr;
    }
    if ( regina_output_conv_handler_get() && !GET_STRENG_CONVERTED(ptr->converted) ) {
        int i;
        for ( i = 0 ; i < ptr->len ; i++ ) {
            if ( isprint(ptr->base_value[i]) ) {
                ptr->value[i] = Char_convert(ptr->base_value[i]);
            } else {
                ptr->value[i] = ptr->base_value[i];
            }
        }
        SET_STRENG_CONVERTED(ptr->converted);
    }

    return ptr;
}
#endif

streng *Str_convert_TSD(streng *str)
{
#ifdef REXX_8BIT_SUPPORT
   if ( !str || str == NOFUNC ) {
       return str;
   }
   if ( !str->base_value ) {
       streng_alloc_base_value(str);
       if ( !GET_STRENG_CONVERTED(str->converted) ) {
           streng_clone_value(str);
       }
   }
   if ( regina_output_conv_handler_get() && !GET_STRENG_CONVERTED(str->converted) ) {
       regina_output_conv_handler_get()(str->value, str->len);
       SET_STRENG_CONVERTED(str->converted);
       dup_check_con(str);
   }
#endif
#ifdef REXXDEBUG
   fprintf(stderr, "func=%s ", __FUNCTION__);
   print_stre(stderr, "ASCII", str);
   fprintf(stderr, "\n");
#endif
   return str;
}

streng *Str_restore_TSD(streng *str)
{
#ifdef REXX_8BIT_SUPPORT
   if ( !str || str == NOFUNC ) {
       return str;
   }
   if ( !str->base_value ) {
       streng_alloc_base_value(str);
       if ( GET_STRENG_RESTORED(str->converted) ) {
           streng_clone_value(str);
       }
   }
   if ( regina_input_conv_handler_get() && !GET_STRENG_RESTORED(str->converted) && GET_STRENG_CONVERTED(str->converted) ) {
       regina_input_conv_handler_get()(str->base_value, str->len);
       SET_STRENG_RESTORED(str->converted);
       dup_check_res(str);
   }
#endif
#ifdef REXXDEBUG
   fprintf(stderr, "func=%s ", __FUNCTION__);
   print_stre(stderr, "ASCII", str);
   fprintf(stderr, "\n");
#endif
   return str;
}

char Char_convert(const char c)
{
#ifdef REXX_8BIT_SUPPORT
   if ( regina_output_conv_handler_get() ) {
       regina_output_conv_handler_get()(&c, 1);
   }
#endif
   return c;
}

char Char_restore(const char c)
{
#ifdef REXX_8BIT_SUPPORT
   if ( regina_input_conv_handler_get() ) {
       regina_input_conv_handler_get()(&c, 1);
   }
#endif
   return c;
}

char Char_Toupper_8bit(const char c)
{
    char cc = c;
#ifdef REXX_8BIT_SUPPORT
   cc = Char_restore(cc);
   cc = rx_toupper(cc);
   cc = Char_convert(cc);
#endif
   return cc;
}

char Char_Tolower_8bit(const char c)
{
    char cc = c;
#ifdef REXX_8BIT_SUPPORT
    cc = Char_restore(cc);
    cc = rx_tolower(cc);
    cc = Char_convert(cc);
#endif
   return cc;
}

int Check_eof(int c)
{
#ifdef REXX_8BIT_SUPPORT
   if ( regina_check_eof_handler_get() ) {
       regina_check_eof_handler_get()(io_handlers.checkEofHandlerArg, &c);
   }
#endif
   return c;
}

streng *streng_clone_and_restore(streng *str)
{
    streng_alloc_base_value(str);
    if ( !GET_STRENG_RESTORED(str->converted) ) {
        streng_clone_value(str);
        Str_restore_TSD(str);
    }

    return str;
}

