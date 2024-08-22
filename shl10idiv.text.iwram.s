@ int shl10idiv(int x, int y)

		.text
		.section	.iwram,"ax",%progbits		
		.ARM
        .ALIGN	
        .GLOBAL  shl10idiv

shl10idiv:  
@r0 = eax = x
@r1 = ebx = y

		stmfd   sp!,{r4-r12,lr}		@ restore stack

		mov	r4, r0
		mov	r4, r4, asr #22

		mov	r5,	r1

		mov	r0, r0, lsl #10
				
		@ r0 = x
		@ r1 = z
		@ div swi destroys r0,r1,r3

		swi	0x60000	

		mov	r6, r0
		
		mov	r0, r4
		mov	r1, r5

		swi	0x60000	

		add	r0,	r0,	r6

		ldmfd   sp!,{r4-r12,lr}		@ restore stack
		bx		lr
		
		.pool
		.END

