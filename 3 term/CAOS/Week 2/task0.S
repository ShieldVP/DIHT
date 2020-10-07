	.text
	.global f
f:
	mul r0, r3
	add r0, r1
	mul r0, r3
	add r0, r2

	mov r1, r0
	bx lr