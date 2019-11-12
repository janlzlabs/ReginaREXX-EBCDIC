#ifndef _DEFS_DEBUG_H_
#define _DEFS_DEBUG_H_

#include <stdio.h>

#define STRINGIFY_ENUM( name ) # name

typedef struct _RexxTokenPair {
    int type;
    const char *type_text;
} RexxTokenPair;

const char *get_rexx_type_text(int type);

void debug_print_hex_buf(FILE *fp,
    const char buf[],
    unsigned int len);

#endif /* _DEFS_DEBUG_H_ */
