/*REXX*/ 
/**********************************************************************/
/*Source :<mypds>(BTLN)                          Language: REXX       */
/*                                                                    */
/*Author :Al Nichols alan@nichols.de             Date    : 19/07/96   */
/*                                                                    */
/*Purpose:A Simple client for the Batch Telnet Client                 */
/*                                                                    */
/*Input  :The file TELIN contains the expect and send strings         */
/*        The Parm specifies the server and optionaly the port        */
/*        You thus might call the program in JCL like this            */
/*        //STEP1 EXEC PGM=IRXJCL,PARM='192.168.1.1 23'               */
/*        and your TELIN script might look like this                  */
/*        gin:                                                        */
/*        myuser                                                      */
/*        ord:                                                        */
/*        mypass                                                      */
/*        >                                                           */
/*        date                                                        */
/*        >                                                           */
/*        ps                                                          */
/*        >                                                           */
/*        ls -lisa                                                    */
/*        >                                                           */
/*        exit                                                        */
/*                                                                    */
/*Output :The debug on standard out                                   */
/*        The session in TELOUT                                       */
/*                                                                    */
/*Copyright: GNU copyright applies to this code.                      */
/**********************************************************************/
 
 
trace o
signal on halt
signal on syntax
 
/* Set error code values                                              */
 
initialized = 0
 
/***************/
/* Translation */
/*    table    */
/***************/
 
/********************************/
/* Row for row plain and shift  */
/* (hint) look at your keyboard */
/* Do this so you can change the*/
/* table really easily and not  */
/* have to write a new IP       */
/* translation table.           */
/********************************/
 
ebcdic.1 = '^1234567890·Ô'
ascii.1  = '5E31323334353637383930DFB4'X
ebcdic.2 = '¯!"@$%&/()=?`'
ascii.2  = 'B02122A72425262F28293D3F60'X
ebcdic.3 = 'qwertzuiopÅ+'
ascii.3  = '71776572747A75696F70FC2B'X
ebcdic.4 = 'QWERTZUIOPö*'
ascii.4  = '51574552545A55494F50DC2A'X
ebcdic.5 = 'asdfghjklîÑ#'
ascii.5  = '6173646667686A6B6CF6E423'X
ebcdic.6 = "ASDFGHJKLôé'"
ascii.6  = '4153444647484A4B4CD6C427'X
ebcdic.7 = "<yxcvbnm,.-"
ascii.7  = '3C79786376626E6D2C2E2D'X
ebcdic.8 = ">YXCVBNM;:_"
ascii.8  = '3E59584356424E4D3B3A5F'X
/* Now All the ALTS and blank */
ebcdic.9 = "{[]}\@~| "
ascii.9  = '7B5B5D7D5C407E7C20'X
ascii = ''
ebcdic = ''
 
/* Put it in one string to use REXX translate function */
 
do i = 1 to 9
   ascii = ascii||ascii.i
   ebcdic = ebcdic||ebcdic.i
end
 
/**********************/
/* Deal with input    */
/* parms: help and    */
/* server are dealt   */
/* with at the moment */
/**********************/
 
parse arg argstring
argstring = strip(argstring)
if substr(argstring,1,1) = '?' | ,
   substr(argstring,1,4) = 'HELP',
  then do
  say 'Here is the help text '
  exit 100
end
 
/* Split arguments into parameters and options                        */
 
parse upper var argstring server port
 
 
/* Initialize control information                                     */
 
if port = '' then port = 23
 
if server = '' then exit 100
 
/************************/
/* Now read the input   */
/* file, it should have */
/* an even number of    */
/* lines, aranged as    */
/* expect and response  */
/* pairs                */
/* First expect then    */
/* response, the logon  */
/* prompt counts.       */
/************************/
 
"EXECIO * DISKR TELIN ( STEM PAIRS. FINIS"
 
if pairs.0//2 <> 0 then do
  say "Wrong number of lines in input"
  exit 200
end
 
do i = 1 to pairs.0;pairs.i=strip(pairs.i);end
 
/* Initialize the socket                                              */
 
call Socket 'Initialize', 'BTELNET'
if src=0 then initialized = 1
else call error 'E', 200, 'Unable to initialize SOCKET'
 
k=0
i=0
ipaddress = server
 
/* Initialize for receiving lines sent by the server                  */
 
s = Socket('Socket')
if src^=0 then call error 'E', 32, 'SOCKET(SOCKET) rc='src
call Socket 'Connect', s, 'AF_INET' port ipaddress
if src^=0 then call error 'E', 32, 'SOCKET(CONNECT) rc='src
 
/* Nancy Regan negotiation, remember, just say NO ! */
 
do forever
   i=i+1 
   parse value Socket('Read', s) with len newline
   if len=0 then exit
   say 'read  pre-prompt ' i C2X(newline)
   say 'read  pre-prompt ' i translate(newline,ebcdic,ascii)
   response=translate(newline,'FEFCFEFC'x,'FBFDFCFE'x)
   start=pos(translate(pairs.1,ascii,ebcdic),response)
   if start <> 0 then leave
   if pos('FB'x,newline) = 0 & pos('FD'x,newline) = 0 & ,
      pos('FC'x,newline) = 0 & pos('FE'x,newline) = 0 then iterate
   call Socket 'Write', s, response
   say 'wrote pre-prompt' i C2X(response)
   say 'wrote pre-prompt ' i translate(response,ebcdic,ascii)
end
 
/* Found the prompt string , and negotiation is over */
 
say 'Found Prompt'
holdline=''
newline=''
 
do index = 2 to pairs.0 by 2
 rindex=index+1
 response = translate(pairs.index,ascii,ebcdic)||'0A'x
 call Socket 'Write', s, response
 say 'Wrote' i C2X(response)
 say 'Wrote' i pairs.index
 do forever
    if newline <> '' then holdline=newline
    else holdline=''
    parse value Socket('Read', s) with len newline
    if len=0 then do;say 'len=0';leave;end
    say 'Read Mainloop' index C2X(newline)
    say 'Read Mainloop' index translate(newline,ebcdic,ascii)
    orgline=holdline||newline
    do forever
       if pos('FFF9'x,newline) <> 0 then say 'IAC GoAhead '
       if pos('0A'x,newline)=0 then leave
       parse var newline nextline '0A'x newline
       k = k + 1
       output.k = holdline||nextline
       say translate(output.k,ebcdic,ascii)
       holdline=''
    end
    if pos(translate(pairs.rindex,ascii,ebcdic),orgline) <> 0 then leave
  end
end
 
/* Terminate The socket */
call Socket 'Terminate'
 
/* Format and Print the responses */
  do j = 1 to k
  output.j = translate(output.j,ebcdic,ascii)
  end
 
"EXECIO " k " DISKW TELOUT ( STEM OUTPUT. FINIS"
 
exit 0
 
/* Calling the real SOCKET function                                   */
socket: procedure expose src
  a0 = arg(1)
  a1 = arg(2)
  a2 = arg(3)
  a3 = arg(4)
  a4 = arg(5)
  a5 = arg(6)
  a6 = arg(7)
  a7 = arg(8)
  a8 = arg(9)
  a9 = arg(10)
  parse value 'SOCKET'(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) with src res
return res
 
/* Syntax error routine                                               */
syntax:
  call error 'E', rc, '==> REXX Error No.' 20000+rc
return
 
 
/* Halt processing routine                                            */
halt:
  call error 'E', 4, '==> REXX Interrupted'
return
 
/* Error message and exit routine                                     */
error:
  type = arg(1)
  retc = arg(2)
  text = arg(3)
  ecretc = right(retc,3,'0')
  ectype = translate(type)
  ecfull = 'NSCLIE' || ecretc || ectype
  say '===> Error:' ecfull text
  if type^='E' then return
  if initialized
     then do
       parse value Socket('SocketSetStatus') with . status severreason
       if status^='Connected'
          then say 'The status of the socket set is' status severreason
     end 
  call Socket 'Terminate'
exit retc
