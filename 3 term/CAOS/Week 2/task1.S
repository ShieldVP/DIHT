	.text
	.global summ
summ:
	ldr r3, [r2]
	add r0, r3
	sub r1, #1
	add r2, #4

	cmp r1, #0
	bgt summ

	mov r1, r0
	bx lr