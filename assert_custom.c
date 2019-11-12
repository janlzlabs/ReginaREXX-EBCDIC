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
 * assert_custom.c
 *
 *  Created on: Jan 27, 2016
 */

#include <stdint.h>

#include "rexx.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "iohandlers.h"

extern FILE *yyout_io;
void assert_custom(const char *file, int line, int cond)
{
    const char *assert_cond = "Something bad happened in %s at line: %d due to bad syntax that should normally terminate script, fix the syntax and try again\n";
    char msg[128];
    if ( cond ) {
        return;
    }
    sprintf(msg, assert_cond, file, line);
    streng *cond_str = Str_convert_TSD(Str_cre_TSD(__regina_get_tsd(), msg));
    FWRITE( Str_origval(cond_str), Str_len(cond_str), 1, yyout_io);
}
