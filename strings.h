/*
 *  The Regina Rexx Interpreter
 *  Copyright (C) 1992-1994  Anders Christensen <anders@pvv.unit.no>
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
#ifndef _STRINGS_ALREADY_DEFINED_
#define _STRINGS_ALREADY_DEFINED_

typedef struct strengtype {
   int len, max ;
#ifdef CHECK_MEMORY                     /* FGC: Test                         */
#ifdef REXX_8BIT_SUPPORT
   int converted;
   char *base_value;
#endif
   char *value;
#else
#ifdef REXX_8BIT_SUPPORT
   int converted;
   char *base_value;
#endif
   char value[4] ;
#endif
} streng ;

#if defined(REXXDEBUG_STRING) || defined(REXXDEBUG)
void print_stre(const FILE *fp, const char *chars, const streng *str);
#endif
/*
 * Some strange define's to allow constant strengs to be defined. They will
 * be accessible as pointers.
 */
#ifdef CHECK_MEMORY
#  define conststreng(name,value) const streng __regina__##name = { sizeof( value ) - 1, \
                                                        sizeof( value ) - 1,             \
                                                        value };                         \
                                  const streng *name = (const streng *) &__regina__##name
#  define staticstreng(name,value) static streng x_##name = { sizeof( value ) - 1,       \
                                                                sizeof( value ) - 1,     \
                                                                value };                 \
                                   const static streng *name = (const streng *) &x_##name
#else
#  define conststreng(name,value) const struct {                               \
                                     int len, max;                             \
                                     char content[sizeof( value )];            \
                                  } x__regina__##name = { sizeof( value ) - 1, \
                                                sizeof( value ) - 1,           \
                                                value };                       \
                                  const streng *name = (streng *) &x__regina__##name
#  define staticstreng(name,value) static struct {                     \
                                      int len, max;                    \
                                      char content[sizeof( value )];   \
                                   } x_##name = { sizeof( value ) - 1, \
                                                 sizeof( value ) - 1,  \
                                                 value };              \
                                   static const streng *name = (const streng *) &x_##name
#endif
#define STRENG_TYPEDEFED 1

//#define REXXDEBUG_STRING

#define Str_len(a) ((a)->len)
#define Str_max(a) ((a)->max)
#ifdef REXX_8BIT_SUPPORT
#define Str_origval(a) ((a)->value)
#define Str_val(a) ((a)->base_value)
#else
#define Str_val(a) ((a)->value)
#define Str_origval(a) ((a)->value)
#endif
#define Str_in(a,b) (Str_len(a)>(b))
#define Str_end(a) (Str_val((a))/*(a)->value*/+Str_len(a))
#define Str_origend(a) (Str_origval((a))/*(a)->value*/+Str_len(a))
#define Str_zero(a) ((Str_len(a)<Str_max(a)) && (Str_val((a))[(a)->len]/*(a)->value[(a)->len]*/==0x00))

#ifdef REXX_8BIT_SUPPORT
streng *streng_alloc_base_value(streng *ptr);
streng *streng_realloc_base_value(streng *ptr);
streng *streng_clone_value(streng *ptr);
streng *streng_clone_from_streng_base_value(streng *to, const streng *from);
streng *streng_clone_base_value(streng *ptr);

#define STRHEAD (1+(sizeof(int)<<3))

#define CLEAR_STRENG_CONVERTED(a)     ((a) &= ~0x1)
#define GET_STRENG_CONVERTED(a)       ((a) & 0x1)
#define SET_STRENG_CONVERTED(a)       ((a) |= 0x1)

#define CLEAR_STRENG_RESTORED(a)      ((a) &= ~0x2)
#define GET_STRENG_RESTORED(a)        ((a) & 0x2)
#define SET_STRENG_RESTORED(a)        ((a) |= 0x2)

#define CLEAR_STRENG_VARIABLE(a)      ((a) &= ~0x4)
#define GET_STRENG_VARIABLE(a)        ((a) & 0x4)
#define SET_STRENG_VARIABLE(a)        ((a) |= 0x4)

#else
#define STRHEAD (1+(sizeof(int)<<1))
#endif


typedef struct num_descr_type
{
   char *num ;      /* pointer to matissa of precision + 1 */
   int negative ;   /* boolean, true if negative number */
   int exp ;        /* value of exponent */
   int size ;       /* how much of num is actually used */
   int max ;        /* how much can num actually take */

   /*
    * The number has an absolute value of
    *   *********************
    *   * "0."<num>"E"<exp> *
    *   *********************
    * Only size byte of num are used.
    *
    * The number of used digits depends on its usage. In general, it's a good
    * idea to use the standard value. used_digits shall be reset after each
    * computation and may or may not be respected. It shall be respected, and
    * this is the intention, by str_norm and all other function which make
    * a string from the number accidently. Functions like string_add may
    * or may not use this value.
    * fixes bug 675395
    */
   int used_digits;
} num_descr ;


#endif /* _STRINGS_ALREADY_INCLUDED_ */
