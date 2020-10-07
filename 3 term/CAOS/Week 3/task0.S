	.data
	.global R
R:  .space 4

	.text
	.global calculate
calculate:
	push {r4}
	ldr r0, .Lexterns
	ldr r0, [r0]
	ldr r1, .Lexterns+4
	ldr r1, [r1]
	ldr r2, .Lexterns+8
	ldr r2, [r2]
	ldr r3, .Lexterns+12
	ldr r3, [r3]
	ldr r4, .Lexterns+16
	mul r0, r1
	mul r2, r3
	add r0, r2
	str r0, [r4]
	pop {r4}
	bx lr

.Lexterns:
	.word A
	.word B
	.word C
	.word D
	.word R