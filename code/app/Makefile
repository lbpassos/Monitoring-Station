# Makefile
#
#      Author: lpassos
#  Version: 1.1

LIBGCC = 'arm-eabi-gcc --print-libgcc-file-name'

ifdef ROM
program-flash.elf: event.o init.o program.o startosc.o lpc2106.o \
	i2c.o isr.o 32k_memmory.o hih6130.o mem_manager.o spi.o nokia_6610.o \
	manager_display.o button.o buffer.o date.o asic_asm.o asic.o \
	wiznet.o snmp.o
	arm-eabi-ld event.o lpc2106.o init.o program.o startosc.o \
	i2c.o isr.o 32k_memmory.o hih6130.o mem_manager.o spi.o nokia_6610.o \
	manager_display.o button.o buffer.o date.o asic_asm.o asic.o wiznet.o snmp.o \
	-o program-flash.elf -Tldscript_rom -L../clib/lib -lc -L/opt/gnutools/arm-eabi/lib/gcc/arm-eabi/4.3.2 -lgcc 
else
program-ram.elf: event.o init.o program.o startosc.o lpc2106.o i2c.o isr.o 32k_memmory.o hih6130.o mem_manager.o spi.o nokia_6610.o manager_display.o button.o buffer.o date.o asic_asm.o asic.o wiznet.o snmp.o
	arm-eabi-ld -o program-ram.elf event.o lpc2106.o init.o program.o startosc.o \
	i2c.o isr.o 32k_memmory.o hih6130.o mem_manager.o spi.o nokia_6610.o manager_display.o \
	button.o buffer.o date.o asic_asm.o asic.o wiznet.o snmp.o -nostdlib -lc -L../clib/lib \
	-L/opt/gnutools/arm-eabi/lib/gcc/arm-eabi/4.3.2 -lgcc -T ldscript_ram
	
	
endif

	
lpc2106.o: ../mylib/lpc2106.c
	arm-eabi-gcc -c -g  -o lpc2106.o ../mylib/lpc2106.c
	
program.o: main.c
	arm-eabi-gcc -c -g -mlong-calls -o program.o main.c


startosc.o: startosc.c
	arm-eabi-gcc -c -g -o startosc.o startosc.c
	
init.o: init.S
	arm-eabi-as --gstabs -o init.o init.S
	
i2c.o: ../mylib/i2c.c
	arm-eabi-gcc -c -g  -o i2c.o ../mylib/i2c.c
	
isr.o: isr.s
	arm-eabi-as --gstabs -o isr.o isr.s

32k_memmory.o: ../mylib/32k_memmory.c
	arm-eabi-gcc -c -g  -o 32k_memmory.o ../mylib/32k_memmory.c
	
hih6130.o: ../mylib/hih6130.c
	arm-eabi-gcc -c -g  -o hih6130.o ../mylib/hih6130.c
	
mem_manager.o: ../mylib/mem_manager.c
	arm-eabi-gcc -c -g  -o mem_manager.o ../mylib/mem_manager.c
	
spi.o: ../mylib/spi.c
	arm-eabi-gcc -c -g -mlong-calls -o spi.o ../mylib/spi.c
	
nokia_6610.o: ../mylib/nokia_6610.c
	arm-eabi-gcc -c -g -mlong-calls -o nokia_6610.o ../mylib/nokia_6610.c

manager_display.o: ../mylib/manager_display.c
	arm-eabi-gcc -c -g -mlong-calls -o manager_display.o ../mylib/manager_display.c
	
button.o: ../mylib/button.c
	arm-eabi-gcc -c -g  -o button.o ../mylib/button.c

buffer.o: ../mylib/buffer.c
	arm-eabi-gcc -c -g  -o buffer.o ../mylib/buffer.c
	
date.o: ../mylib/date.c
	arm-eabi-gcc -c -g  -o date.o ../mylib/date.c

event.o: ../mylib/event.c
	arm-eabi-gcc -c -g  -o event.o ../mylib/event.c
	
asic_asm.o: ../mylib/asic.s
	arm-eabi-as --gstabs -o asic_asm.o ../mylib/asic.s

wiznet.o: ../mylib/wiznet.c
	arm-eabi-gcc -c -g -mlong-calls -o wiznet.o ../mylib/wiznet.c
	
asic.o: ../mylib/asic.c
	arm-eabi-gcc -c -g -mlong-calls -o asic.o ../mylib/asic.c -I../clib/inc

snmp.o: ../mylib/snmp.c
	arm-eabi-gcc -c -g -mlong-calls -o snmp.o ../mylib/snmp.c -I../clib/inc


clean:
	rm -f *.o *.elf
