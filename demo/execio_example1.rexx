/***************************** REXX ********************************/
/* This exec reads from the data set allocated to INDD to find the */
/* first occurrence of the string "Jones". Upper and lowercase */
/* distinctions are ignored. */
/*******************************************************************/
done = 'no'
lineno = 0

DO WHILE done = 'no'
    "EXECIO 1 DISKR indd"
    IF RC = 0 THEN /* Record was read */
    DO
        PULL record
        lineno = lineno + 1 /* Count the record */
        IF INDEX(record,'JONES') \= 0 THEN
        DO
            SAY 'Found in record' lineno
            done = 'yes'
            SAY 'Record = ' record
        END
        ELSE NOP
    END
    ELSE
        done = 'yes'
END

EXIT 0
