ROOT			=		c:\rtl
TMP			 	=		tmp
CLIB			=		$(ROOT)\clib

INCLUDE1		=		$(ROOT)\inc
INCLUDE3		=		$(ROOT)\inc
LIB				=		$(ROOT)\lib

MODEL			=		l	 # s, m, c, l, h

LIBS			=		c$(MODEL).lib

STARTUP			=		$(TMP)\cstart.obj

CFLAGS			=		-x- -v -r- -c -m$(MODEL) -I. -I$(INCLUDE1);$(INCLUDE2) -n$(TMP)
AFLAGS			=		/zd /mx /d__$(MODEL)__ /i$(INCLUDE3)
LFLAGS			=		/v /c /L$(LIB)

.path.asm		=		.;$(CLIB)\string;$(CLIB)\borland;$(CLIB)\startup\pc;$(CLIB)\x86
.path.c			=		.;pc;$(CLIB)\ctype;$(CLIB)\string;$(CLIB)\assert;$(CLIB)\stdio;$(CLIB)\conio\generic;$(CLIB)\conio\pc
.path.cpp		=		.

.autodepend
.suffixes:	.c .asm .cpp

.c.obj:
   bcc $<

.asm.obj:
   tasm $(AFLAGS) $<, $(TMP)\$(<B).obj

OBJS	=	$(TMP)\ttime.obj \
			$(TMP)\time.obj \
			$(TMP)\clock.obj \
			$(TMP)\mktime.obj \
			$(TMP)\difftime.obj \
			$(TMP)\asctime.obj \
			$(TMP)\strftime.obj \
			$(TMP)\localtim.obj \
			$(TMP)\gmtime.obj \
			$(TMP)\ctime.obj \
			$(TMP)\_tinfo.obj \
			$(TMP)\_time.obj \
			$(TMP)\_strftim.obj \
			$(TMP)\_isdst.obj \
			$(TMP)\_tzoff.obj \
			$(TMP)\memcpy.obj \
			$(TMP)\strcmp.obj \
			$(TMP)\strcpy.obj \
			$(TMP)\strncpy.obj \
			$(TMP)\strlen.obj \
			$(TMP)\strchr.obj \
			$(TMP)\strtok.obj \
			$(TMP)\assert.obj \
			$(TMP)\ctype.obj \
			$(TMP)\cprintf.obj \
			$(TMP)\cputs.obj \
			$(TMP)\vsprintf.obj \
			$(TMP)\putch.obj \
			$(TMP)\getch.obj \
			$(TMP)\kbhit.obj \
			$(TMP)\_printf.obj \
			$(TMP)\int86.obj \
			$(TMP)\n_lxmul.obj \
			$(TMP)\h_ldiv.obj

all:   turboc.cfg ttime.exe 

ttime.exe:	   $(STARTUP) $(OBJS)
	tlink $(LFLAGS) @&&!
	$** 
	$(TMP)\$*.exe
	$(TMP)\$*.map
	$(LIBS) 
!

turboc.cfg:		makefile
	copy &&!
	$(CFLAGS)
!		turboc.cfg
