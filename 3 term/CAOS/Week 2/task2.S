	.text
	.global solve
solve:
	push {r4, r5}
	mov r4, #0

find_root:
	mul r5, r0, r4
	add r5, r1
	mul r5, r4
	add r5, r2
	mul r5, r4
	add r5, r3
	add r4, #1

	cmp r5, #0
	bne find_root

come_back:
	sub r4, #1
	mov r1, r4
	mov r0, r4
	pop {r4, r5}
	bx lr
