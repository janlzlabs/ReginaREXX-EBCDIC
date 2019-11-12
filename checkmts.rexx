/*
 * This program checks consistency of Regina source language files.
 * All native language files are checked against the en.mts; its the
 * master file
 * 93.3 es
 */
Parse Arg srcdir
If srcdir = '' Then Call Abort 'General', 0, 'Source directory not supply as 1st parameter'
langs = "ca cs da de el es fi fr he hu is it ja ko lt nl no pl pt ru sk sl sv th tr zh"
en. = ''
enp. = ''
Call reader srcdir'/en.mts', 1

/*
 * Now we have the number of messages from the en.mts file, check this
 * against the #define in rexxmsg.h
 */
Call CheckHeader
Do j = 1 To Words( langs )
   fn = Word( langs, j )'.mts'
   If Stream( srcdir'/'fn, 'C', 'QUERY EXISTS' ) \= '' Then
      Do
         other. = ''
         otherp. = ''
         Call reader srcdir'/'fn, 0
         If en.0 \= other.0 Then Call Abort fn, 1, 'Number of messages inconsistent'
         Do i = 1 To en.0
            If en.i \== other.i Then Call Abort fn, i, 'Mismatch of messages between:' '<'en.i'>' 'and' '<'other.i'>'
         End
         If enp.0 \= otherp.0 Then Call Abort fn, 1, 'Number of prefix messages inconsistent'
         Do i = 1 To enp.0
            If enp.i \== otherp.i Then Call Abort fn, i, 'Mismatch of prefix messages between:' '<'enp.i'>' 'and' '<'otherp.i'>'
         End
         Say fn 'is clean'
      End
End
Return 0

reader: Procedure Expose en. other. enp. otherp.
Parse Arg fn, en
lineno = 0
idx = 0
idxp = 0
If Stream( fn, 'C', 'QUERY EXISTS' ) = '' Then Call Abort fn, 0, 'Unable to find source file:' fn
Do While Lines(fn) > 0
   lineno = lineno + 1
   line = Linein( fn )
   If Strip( line ) = '' | Left( line, 1 ) = '#' Then Iterate
   Parse Var line err ',' suberr ',' text '|' inserts
   inserts = Strip( inserts )
   If ( Datatype( err ) \= 'NUM' & err \= 'P' ) | DataType( suberr ) \= 'NUM' Then Call Abort fn, lineno, 'Invalid line format'
   sub = ''
   /*
    * parse the error text looking for constructs like %x
    * if the 'x' is one of "sdcx', then include it in the further checking
    */
   Do Forever
      Parse Var text '%' text
      If text = '' Then Leave
      subs = Left( text, 1 )
      Select
         When subs = '-' Then
            Do
               sub = sub || ' '
               say 'found %- at' lineno text
            End
         When subs = '%' Then text = Overlay( ' ', text, 1 )
         When subs = ' ' Then Nop
         Otherwise sub = sub || Left( text, 1 )
      End
   End
   /*
    * Now we have all of the substitutes, ensure that for each %x construct, there is
    * an insert in inserts
    */
   If inserts \= '' & Countstr( ',', inserts ) \= Length( sub ) - 1 Then Call Abort fn, lineno, 'Incorrect number of inserts:' 1 + Countstr( ',', inserts ) 'for substitutions:' '<'sub'>'
   If err = 'P' Then idxp = idxp + 1
   Else idx = idx + 1
   If en Then
      Do
         If err = 'P' Then
            Do
               enp.idxp = err'.'suberr sub
               enp.0 = idxp
            End
         Else
            Do
               en.idx = err'.'suberr sub
               en.0 = idx
            End
      End
   Else
      Do
         If err = 'P' Then
            Do
               otherp.idxp = err'.'suberr sub
               otherp.0 = idxp
            End
         Else
            Do
               other.idx = err'.'suberr sub
               other.0 = idx
            End
      End
End
Return 0

CheckHeader:
fn = srcdir'/rexxmsg.h'
ok = 0
okp = 0
Do While Lines( fn ) > 0
   line = Linein( fn )
   Parse Var line def name num
   If def = '#define' & name = 'NUMBER_ERROR_MESSAGES' & Datatype( num ) = 'NUM' Then
      Do
         If num \= en.0 Then Call Abort fn, 0, 'Number of error messages defined in' fn '('num') is not the same as in message source files ('en.0')'
         ok = 1
         Iterate
      End
   If def = '#define' & name = 'NUMBER_PREFIX_MESSAGES' & Datatype( num ) = 'NUM' Then
      Do
         If num \= enp.0 Then Call Abort fn, 0, 'Number of prefix messages defined in' fn '('num') is not the same as in message source files ('enp.0')'
         okp = 1
         Iterate
      End
End
If \ok Then Call Abort fn, 0, 'Unable to find definition of number of error messages in' fn
If \okp Then Call Abort fn, 0, 'Unable to find definition of number of prefix messages in' fn
Return

Abort: Procedure
Parse Arg fn, lineno, msg
Say 'Error processing' fn 'at line:' lineno':' msg
Exit 1
