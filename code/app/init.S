
/*
 * init.S
 *
 *  Created on: 17 Mar 2013
 *      Author: lpassos
 */

/*****************************************************************
	Module includes the interrupt vectors and start-up code.
 ******************************************************************/

.set  	MODE_SYS, 0x1F 		/* System Running Priviledged Operating System Tasks  Mode	*/
.set  	I_BIT, 0x80   		/* when I bit is set, IRQ is disabled (program status registers) */
.set  	F_BIT, 0x40    		/* when F bit is set, FIQ is disabled (program status registers) */

/******************************************************************
	Module includes the interrupt vectors and startup code.
 ******************************************************************/
.section .startup, "ax"

.global _start

# Exception Vectors
_vectors:
	ldr     pc, ResetAddr
    ldr     pc, UndefAddr
    ldr     pc, SWIAddr
    ldr     pc, PAbtAddr
    ldr     pc, DAbtAddr
	.word	0xB8A06F58
    ldr     pc, IRQAddr
    ldr     pc, FIQAddr

ResetAddr:  .word   ResetHandler
UndefAddr:  .word   UndefHandler
SWIAddr:    .word   SWIHandler
PAbtAddr:   .word   PAbortHandler
DAbtAddr:   .word   DAbortHandler
	    .word   0
IRQAddr:    .word   irq_handler
FIQAddr:    .word   FIQHandler

ResetHandler:
_start:
	ldr   	r0, =__stack_top__
	/* Alterado para permitir IRQ	*/
	/*	************************************************************ */
	mov	r1,	#0xd2
	msr	cpsr_c,	r1
	mov	sp,	r0
	
	sub	r0,	r0, #2048	/* metade do stack */
	/*	************************************************************ */
	msr   	cpsr_c, #MODE_SYS|I_BIT|F_BIT 	/* System Mode */
	mov   	sp, r0

/* inicializar fastcode */
	ldr r1, = __fastcode_start__
	ldr r2, = __fastcode_load
	ldr r3, = __fastcode_end__
1:
	cmp r1, r3
	ldrne r4, [r2], #4
	strne r4, [r1], #4
	bne 1b

	/* Clear .bss section */
	ldr   	r1, =__bss_start__
	ldr   	r2, =__bss_end__
	ldr   	r3, =0
bss_clear_loop:
	cmp   	r1, r2
	strne 	r3, [r1], #+4
	bne   	bss_clear_loop

	
	/* Initialise .data section */
	ldr	r1, =__data_start__     	/* Start of memory block */
	ldr	r2, =__rom_data_start__	   	/* End of memory block */
	ldr	r3, =__data_end__
	cmp	r1, r2
	beq	end_set_loop
set_loop:
	cmp	r1, r3
	ldrne	r4, [r2], #4
	strne	r4, [r1], #4
	bne	set_loop
end_set_loop:
	
   	ldr   	r10, =StartOsc
    mov   	lr, pc
   	bx    	r10

    ldr   	r10, =main
    mov   	lr, pc
    bx    	r10                       /* enter main() */

    b	.

/****************************************************************************/
/*                         Stack area                                       */
/****************************************************************************/
.section .stack
	.space	1024 * 4
__stack_top__:

/****************************************************************************/
/*                         Default interrupt handler                        */
/****************************************************************************/

UndefHandler:
   b UndefHandler

SWIHandler:
   b SWIHandler

PAbortHandler:
   b PAbortHandler

DAbortHandler:
   b DAbortHandler

/*
IRQHandler:
   b =irq_handler */

FIQHandler:
   b FIQHandler

.end
