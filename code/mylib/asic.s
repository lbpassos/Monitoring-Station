	.global asic_delay
asic_delay:
	sub r0, r0, #1
	cmp r0, #0
	bne asic_delay
	mov pc, lr
