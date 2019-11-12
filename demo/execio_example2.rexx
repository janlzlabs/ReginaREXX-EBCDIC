/***************************** REXX ********************************/
/* This exec scans a data set whose name and size are specified by */
/* a user. The user is given the option of changing each line as */
/* it appears. If there is no change to the line, the user presses*/
/* Enter key to indicate that there is no change. If there is a */
/* change to the line, the user types the entire line with the */
/* change and the new line is returned to the data set. */
/*******************************************************************/
PARSE ARG name numlines /* Get data set name and size from user */

"ALLOC DA("name") F(updatedd) OLD"
eof = 'NO' /* Initialize end-of-file flag */

DO i = 1 to numlines WHILE eof = 'NO'
    "EXECIO 1 DISKRU updatedd" /* Queue the next line on the stack */
    IF RC = 2 THEN /* Return code indicates end-of-file */
        eof = 'YES'
    ELSE
    DO
        PARSE PULL line
        SAY 'Please make changes to the following line.'
        SAY 'If you have no changes, press ENTER.'
        SAY line
        PARSE PULL newline
        IF newline = '' THEN NOP
        ELSE
        DO
            PUSH newline
            "EXECIO 1 DISKW updatedd"
        END
    END
END
