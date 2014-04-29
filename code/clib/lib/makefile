ROOT        =   ..
SRC         =   $(ROOT)/src

CFLAGS        = -c -g -save-temps -Wa,-a=$*.lst -O -I$(ROOT)/inc

CXXFLAGS      = $(CFLAGS)
EXTRACFLAGS   = -Wall -ffunction-sections -fdata-sections

VPATH	=	$(SRC)/init:$(SRC)/ctype:$(SRC)/string/c:$(SRC)/assert:$(SRC)/errno:$(SRC)/stdio:$(SRC)/stdlib:$(SRC)/string:$(SRC)/time:$(SRC)/setjmp/arm

include objects.mak

%.o:%.c
	arm-eabi-gcc -o $*.o $(CFLAGS) $(EXTRACFLAGS) $<

%.o:%.s
	arm-eabi-as -a=$*.lst -o $*.o $<

lib: $(OBJS)
	arm-eabi-ar rcs $(ROOT)/lib/libc.a $?

clean:
	-rm -f *.o *.lst *.bak *.map *.ii *.i *.s

