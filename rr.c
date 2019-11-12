#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>

#define INCL_RXSHV
#define INCL_RXFUNC
#define INCL_RXSUBCOM
#define INCL_RXDSTK         /* supporting RexxDataStack */

#include "rexxsaa.h"

void testStackPopulate()  {
    char        buffer[256];
    DSTKPARMS   stkparms;
    const int   lines = 20;
    int         rc;
    int         i;

    memset(&stkparms, 0, sizeof(stkparms));
    
    for (i = 1; i <= lines; ++i)  {
        sprintf(buffer, "This is stacked line # %d", i);
        stkparms.dstkline.strptr = buffer;
        stkparms.dstkline.strlength = strlen(buffer);
        stkparms.dstkcode = RXDSTK_QUEUE;

        rc = RexxDataStack(&stkparms);

        if (rc || stkparms.dstkret)
            printf("stack QUEUE error: %d - &d \n", rc, stkparms.dstkret);
    }

    sprintf(buffer, "%d", lines);
    stkparms.dstkline.strptr = buffer;
    stkparms.dstkline.strlength = strlen(buffer);
    stkparms.dstkcode = RXDSTK_PUSH;

    rc = RexxDataStack(&stkparms);

    if (rc || stkparms.dstkret)
        printf("stack PUSH error: %d - %d \n", rc, stkparms.dstkret);
}

void testVarPopulate(char* org, int len, int stemvar) {
    SHVBLOCK    shvblk;
    APIRET      rxrc;
    char        value[256];
    char        name[64];
    char*       isrt;
    char*       p;
    int         pfxlen = len;
    const int   lines = 10;
    int         i;

    printf("stem: %d len: %d str: %.*s \n", stemvar, len, len, org);

    memset(&shvblk, 0, sizeof(shvblk));

    memcpy(name, org, len);
    isrt = name + len;

    if (stemvar) {
        *isrt++ = '.';
        ++pfxlen;
    }

    for (*isrt = 0, p = name; *p; ++p) 
        *p = toupper(*p);

    shvblk.shvname.strptr = name;
    shvblk.shvnamelen = sizeof(name);
    shvblk.shvvalue.strptr = value;
    shvblk.shvvaluelen = sizeof(value);

    for ( i = 0; i <= lines; ++i)  {
        sprintf(isrt, "%d", i);
        shvblk.shvname.strlength = pfxlen + strlen(isrt);
   
        if (i == 0)
            sprintf(value, "%d", lines);
        else 
            sprintf(value, "%2d - This is the contents of variable: %s", i, name);


        printf("count: %d value: %s \n", i, value);

        shvblk.shvvalue.strlength = strlen(value);

        shvblk.shvcode = RXSHV_SET;
        rxrc = RexxVariablePool(&shvblk);

        shvblk.shvret &= ~RXSHV_NEWV;

        if (shvblk.shvret) {
            printf("RexxVariablePool error: %.2X - %u \n", shvblk.shvret, rxrc);
            break;
        }
    }
}

void testStackRetrieve() 
{   
    char        buffer[256];
    DSTKPARMS   stkparms;
    int         count;
    int         rc;
    int         i;

    memset(&stkparms, 0, sizeof(stkparms));
        
    stkparms.dstkcode = RXDSTK_QUEUED;

    rc = RexxDataStack(&stkparms);

    if (rc)  {
        printf("stack QUEUED error: %d \n", rc);
        return;
    }    
    
    count = stkparms.dstkret;
    printf("RexxDataStack: QUEUED value is: %d \n", count);

    stkparms.dstkline.strptr = buffer;
    stkparms.dstkmaxline = sizeof(buffer);

    for ( i = 1; i <= count; ++i)  {
        stkparms.dstkcode = RXDSTK_PULL;
        if (i == count)  {
            printf("forcing truncation to five chars \n");
            stkparms.dstkmaxline = 5;
        } 
        
        rc = RexxDataStack(&stkparms);
        
        if (rc != 0)
            printf("unexpected PULL rc: %d \n", rc);
            
//        printf("PULLED: %3d length %2d aux: %d - %.*s \n", i, stkparms.dstkline.strlength, 
//            stkparms.dstkret, stkparms.dstkline.strlength, stkparms.dstkline.strptr);
   
        stkparms.dstkcode = RXDSTK_QUEUED;
        rc = RexxDataStack(&stkparms);
//      printf("stack now contains %d entries \n", stkparms.dstkret);
   }
}

/*----------------------------*/
/* SUBCOM handler             */
/*----------------------------*/

APIRET SubcomHandler(PRXSTRING command, USHORT* flags, PRXSTRING retstring)
{
    char    bStem;
    char    *org, *end;
    int     len;
    char    execiow[] = "execiow";

    printf("SubcomHandler passed control for: %.*s \n", command->strlength, command->strptr);
  
/* stack PULL and QUEUED tests */

    if (strncmp(command->strptr, execiow, strlen(execiow)) == 0)  {
        testStackRetrieve();      
        return 0;
    }

/* original stem / variable access, stack PUSH and QUEUE tests */

    end = command->strptr + command->strlength - 1;

    if (*end != '.')
        bStem = 0;
    else {
        bStem = 1;
        --end;
    }

    for (org = end; *org != ' ' && org != command->strptr; --org);

    if (org == command->strptr)  {
        *flags = RXSUBCOM_FAILURE;
        return 0; /* ???????? */
    }

    len = end - org;
    org++;
   
    if (strcmp("stack", org) == 0)
        testStackPopulate();
    else 
        testVarPopulate(org, len, bStem); 
    
    *flags = RXSUBCOM_OK;
}

/*----------------------------*/
/* Entry: start a rexx script */
/*----------------------------*/

int main(int argc, char** argv)  {
    RXSTRING    rxstring;
    APIRET      rxrc;
    short       src;

    rxrc = RexxRegisterSubcomExe(
        "CMD",             
        &SubcomHandler,
        NULL
    );
    
    printf("RexxRegisterSubcomExe return code is: %d \n", rxrc);

    rxrc = RexxStart(
        0,
        NULL,
        "rexxmain",
        NULL,
        "CMD",
        RXSUBROUTINE,
        NULL,
        &src,
        NULL
    );

    printf("RexxStart return code is: %d, return code value is: %d \n", rxrc, src);

    return 0;
}

