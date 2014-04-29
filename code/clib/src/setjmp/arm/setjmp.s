	.text
	.global setjmp

setjmp:

	/* Save all the callee-preserved registers into the jump buffer.  */
	stmea		a1!, { v1 - v7, fp, ip, sp, lr }
	
	/* When setting up the jump buffer return 0.  */
	mov			a1, #0

	movs		pc, lr


	.global longjmp

longjmp:

	/* Restore the registers, retrieving the state when setjmp() was called.  */
	ldmfd		a1!, { v1 - v7, fp, ip, sp, lr }
	
	/* Put the return value into the integer result register.
	   But if it is zero then return 1 instead.  */	
	movs		a1, a2
	moveq		a1, #1

	movs		pc, lr
