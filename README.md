Welcome to version 0.07a of the Regina Rexx interpreter for 8-bit encoded input.

This is a fork from the original Regina Rexx interpreter with the added
suppport for 8-bit encoded characters, including EBCDIC. 
To execute the interpreter in EBCDIC mode a plugin is required. 
The function of the plugin is to provide character translation from utf-8 
to EBCDIC and vice-versa. Example plugin will be provided soon.....
Without the plugin, the interpreter will work in standard mode, i.e. utf-8.

NOTE: Flex 2.6.0 with 8-bit modifications is required to build Regina Rexx
interpreter. It'll be provided soon.....

But first, the copyright and copying conditions. The code is
'copylefted', which means that it is copyrighted, but that you are
allowed to use, copy, modify, redistribute and sell the code, under
the conditions listed in the "GNU Library General Public License". See
the file COPYING-LIB for more information.

Note that this has changed since version 0.06, which was covered by
the 'ordinary' GPL. The reason for the change is simply that Regina is
becoming more and more library-like, and the LGPL seemed more
appropriate than the GPL.

The source has been built and tested on several platforms. Generally,
'all' you need a Standard C compiler (i.e. ANSI C), a 'lex' program
and a 'yacc' program. In addition, your machine should be POSIX
compliant. The latest version of Regina is stored on the anonymous ftp
site ftp.pvv.unit.no (129.241.36.200), in the '/pub/regina' directory.

To build the interpreter:

1) Edit the Makefiles (in particular the one in the 'src' subdirectory,
   and uncomment a suitable definition of the macros CC and CEXTRA.
   You may want to make a new one if none of the existing ones fit
   your exact needs.

2) Run the command   make

   If you like to make the dynamic library curses.rxlib, then you must
   issue the command  make curses.rxlib  while in the src subdirectory.
   Note that dynamic loading of libraries will not work on all systems.

3) The Makefile does not install targets. There is a make-target
   'install', which installs programs. Check the settings of macros in
   the makefile before running make for target 'install'.

4) You might want to run the documentation through the LaTeX
   textformatter.  If you don't have the 'block' and 'a4' style
   options for tex (these are not standard LaTeX options), just
   edit them out, letting the first line become:

      \documentstyle{report}

   Preformatted version of the documentation is supposed to be
   available from flipper in the directory 'pub/rexx/doc', there is
   one file which contains the whole documentation as a PostScript
   document. In addition, there is a set of ASCII files, which is the
   preformatted version, divided by chapters.

There are sometimes problems when compiling Regina under SunOS. The
main problem is that SunOS is one of few Unix systems still in use which
is neither POSIX nor ANSI. Note that Solaris is OK. But SunOS 4.1.1 has
the following problems:

 * Lacks the memmove() function; use bcopy() instead
 * Lacks the strerror() function; access sys_errlist[] directrly for
   circumvent this.

There are several other omisions and bugs in the include file of SunOS,
but the rest of them are likely to 'just' generate warnings. The two
above, however, _must_ be fixed if compilation shall succeed. If the
preprocessor symbol SunKludges is defined, these two problems are
automatically fixed in config.h; however, that fix may not suit all
situations.

In general, this depends on the interpreter you are using. Some
versions of gcc (at least 2.5.2) will declare strerror() and
memmove(); thus an error is provoked if compiled with SunKludges.
Also, there are some other problems, like the SEEK_{SET,END,CUR}
macros not being defined.

Also, SunOS has a bug, the _file member of FILE has type char, but
fileno() should return an integer or -1 if error. However, if your
max-number-of-files limit exceeds 128, fileno() will return -128 as
file descriptor. Obviously, this is incorrect, and obviously, this can
not be fixed by casting fileno() to unsigned char (it would be
incompatible and then -1 and 255 would overlap). You should (a)
complain about this to your SunOS vendor, and (b) never set the max
open files limit higher than 128. I've tried to circumvent it by
casting the return value from fileno to unsigned char. But it doesn't
seem to work, probably because other I/O functions use _file without
taking any precautions. Thus, you should never use max open files
greater than 128 under SunOS; else, bad things will happen, both in
Regina and in other programs. (Use limit or ulimit to display/set
this max limit.)

If you want to compile it for VMS, you need a lex and a yacc program.
You can get the GNU versions of these tools, or you can run the lex
and yacc on another machine, and copy the results of these programs
from that machine. Unfortunately, I've not been able to test the VMS
version for some time now, so it might need some 'help' to compile
correctly.

I have built it on the following platforms:

   - gcc, lex and yacc on a Uniline 88
   - gcc, lex and yacc on Sun 3
   - c89 (unbundled), lex and yacc on Decstation 5000, under Ultrix 4.2
   - cc, lex and yacc on Irix Indigo and Onyx, under IRIX 4 and 5
   - acc (unbundled), lex and yacc on Sun Sparc, under Sunos 4.2
   - cc, lex and yacc on Decstation 3100, under OSF/1
   - c89, lex and yacc on HP 9000, under HP/UX 8.05
   - cc, lex and yacc on IBM RS6000 under AIX
   - cc, lex and yacc on Cray under UNICOS
   - cc, lex and yacc on Linux 0.99
   - cc on a VAX/VMS, getting yacc and lex output from a Sun.
   - cc, lex and yacc on a DEC alpha running OSF/1

Read the file README.VMS to get more information about how to build
Regina on a computer running VMS, and what functionality is present in
the VMS version of Regina. In addition to VMS and Unix, this version
of Regina has also been successfully built for OS/2 and MS-DOS, using
the EMX development environment under OS/2. However, Regina does not
yet have any specific support for these systems.

This version of Regina also has support for dynamic loading of object
files. I.e. that you can load an object module containing definitions
for extra 'built-in' functions. This interface uses the same interface
as the RXFUNCADD(), RXFUNCDROP() and RXFUNCQUERY() functions in SAA
Rexx. Only one library of extra functions are available, it is a
curses interface library (although not all curses functions are
implemented in it yet.) To load these function into the interpreter,
use:

   lib = './curses.rxlib'
   if rxfuncadd('CURSES', '/path/curses.rxlib', 'curses')  then do
      say 'An error occured when trying to load the library: "'lib'"'
      say 'The error message returned was: "'rxfuncerrmsg()'"'
      say 'Exiting...'
      exit
      end
   else
      call curses('CUR_')

Actually, this 'just' loads and executes a function CURSES() in the
library. However, this special function will load all the other
functions in the library. The parameter given to CURSES() is the
prefix to prepend on all function names. See the file hanoi.rexx in
the code directory for an example.

Note that dynamic linking is only supported for those. Also note the
the normal 'make' setup will not by default compile the curses
library. To do that, do:

   make curses.rxlib

after having made the rest of Regina.

If you are unable to get Regina to work on your computer, because you
don't have one or several of the programs needed to build it, you can
try to build the GNU tools on your computer. Regina will compile with
these tools (and using them is a good idea anyway). You can get them
by anonymous ftp from prep.ai.mit.edu, ugle.unit.no and several other
places.

The areas where it is not POSIX compliant are:

   - It uses "struct timeval" and gettimeofday().  You can control
     this through the HAS_TIMEVAL option in config.h.  Comment it out
     if your machine does not eat the code.  Consequences: The
     interpreter will have a time granularity of 1 second.

   - It uses ftruncate().  If you do not have that, comment out the
     HAS_FTRUNCATE option on config.h.  Consequences: When writing to
     the middle of a file with lineout(), the file will not be
     truncated after the line written.

   - It uses putenv() to set an environment variable. While POSIX do
     have the getenv() call, it does not have a way of setting
     environment variables. Fortunately, most machines have a way of
     doing this, but you might have to link in an extra library or
     something. If you can't find a putenv() on your machine, you can
     undefine the HAS_PUTENV macro in config.h.  Consequences: Trying
     to set en environment variable will cause a syntax error

The interpreter has bugs and missing feature. I consider this release
of Regina a beta release, and I will continue to do so, until:

   - Regina is a full implementation of Rexx as described in the book:
     "The Rexx Language", by Mike Cowlishaw;

   - the most important extensions from CMS have been implemented;

   - a full implementation of the Rexx SAA API, as documented for
     OS/2, has been implemented;

   - all important extensions suggested by the ANSI Rexx committee
     have been implemented;

   - an extensive test suite which test most parts of the features
     listed above have been implemented, and Regina is able to run it
     without problems; and

   - the documentation for Regina has been completed.

If you find bugs or incompatibilities, please send me e-mail
describing the bug. It is important that your bugreport contains:

   - Description of what equipment you used, i.e. hardware
     platform, operating system, compiler version, compiler
     options used, version of Regina etc.

   - A description of the buggy behavior (and the behavior that
     you expected to see).

   - Preferably a program that demonstrates the behavior.  The
     program should be as small as possible, preferably not more
     than 10 lines or so.

   - If you have already fixed the bug, please append a context
     diff of the changes you made to the source.

Please make sure that the bug is really a bug in the interpreter, not
just a bug in your program. And also make sure that you've got the
latest release of Regina. If possible, run your program on other REXX
interpreters to see how they behave, and check with a REXX manual if
you have access to one. Bugreports are welcome, as well as other types
of comments. You can send bugreports directly to my email address,
listed below.

There are four subdirectories.

   - The first is 'trip' which is the start of a "triptest" for a REXX
     interpreter, which is supposed to test most REXX interpreter
     features.  If enough tests are gathered in this triptest, any
     REXX interpreter that manages to run it would be fairly REXX
     compatible. Such a trip test is not a perfect way of ensuring
     compatibility, but it is far better than none.

   - The next subdirectory is the 'demo', which contains some small
     demo programs for REXX, mainly demonstrations of special
     programming features in REXX. Also included are some common test
     programs and routines distributed on the net. Note that the
     programs in this directory are not covered by the GPL.

   - The src directory, containing the source code for Regina.

   - The 'doc' subdirectory, containing the documentation for Regina.
     Unfortunately, this documentation is not complete, nor it is
     completely correct at the moment, since I want to finish the
     coding before continuing the work in the doc.

Some of the things the interpreter does not have:

   - The documentation is far from finished. It has lower priority
     for the moment, until I have finished the interpreter itself.

   - For the (external) commands, there is not a consistent shell in
     the background to which commands are sent.  Instead either there
     is a shell that is started up each time (ADDRESS SYSTEM), or the
     command is run directly (ADDRESS PATH and COMMAND).  This will
     be fixed ...

   - I still need to test a lot of functionality that has been
     implemented and are supposed to work.

   - Lots of other things ...

There is a mailing list for announcements of new versions of Regina.
If you want to be on that list, send a request to be put on the list
to the address: <rexx-request@pvv.unit.no>

Jan Lisowiec                           e-mail: jan.lisowiec@lzlabs.com
Lzlabs GmbH
Richtiarkade 16
CH-8304 Wallisellen
Switzerland

Anders Christensen                     e-mail: anders@pvv.unit.no
SINTEF RUNIT
Hogskoleringen 7i
N-7034 Trondheim, Norway
