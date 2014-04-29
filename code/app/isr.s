	.global	irq_handler
irq_handler:
	sub	lr,	lr,	#4
	stmdb	sp!, { r0-r3, r12, lr }
	bl	isr
	ldmia	sp!, { r0-r3, r12, pc }^
	
	.global interrupt_enable
interrupt_enable:
	mrs	r0,	cpsr
	and	r0,	r0,	#~(1<<7)
	msr	cpsr_c,	r0
	mov	pc,	lr
