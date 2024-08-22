@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Fixed point arithmetic routines
@	
@	NOTE:
@			
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
         .ARM
		.code 32
        .ALIGN 4
        .GLOBL  fixedpointasm_div
		.GLOBL  fixedpointasm_div2
		.EXTERN _binary_oneby_bin_start
@		.EXTERN	sndInterruptCounter



@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ long fixedpointasm_div(long r0,long r1);   // r0 = value , r1 = divisor
fixedpointasm_div:
	stmfd   sp!,{r1-r7}	@ push some registers

	@ r4 = signflag
	mov	r4,#0
	tst	r1,#0x80000000		@ see if neg bit is set
	subne r1,r4,r1			@ negate r1 if r1 is negative
	subeq r4,r4,#1			@ set r4 to -1 if unsigned else 0 if signed

@ r2 = maskreg
	ldr		r2,=0xfff00000
	mov		r3,#0
	tst		r1,r2
	bne		fixedpointasmdiv_highbitvalue
	tst		r1,#0x0f000
	movne	r3,#4
	tst		r1,#0xf0000
	movne	r3,#8
	b	fixedpointasmdiv_knowbits
fixedpointasmdiv_highbitvalue:
	mov		r3,#12
	tst		r1,#0x0f000000
	movne	r3,#16
	tst		r1,#0xf0000000
	movne	r3,#20
@@@@@@@@@@
fixedpointasmdiv_knowbits:
	@ r3 = divisior rightshifter
	mov		r1,r1,lsr r3
	ldr		r5,=_binary_oneby_bin_start
	ldr		r5,[r5,r1,lsl #2] 
	mov		r5,r5,lsr r3 
	smull	r1,r0,r5,r0
@ 0shift  : 22:42 
@ 4shift  : 22:42 * 16
@ 12shift : 22:42 * 4096
	mov		r0,r0,lsl #17
	add		r0,r0,r1,lsr #15

	tst		r4,r4
	subeq	r0,r4,r0			@ negate result if r4=0 (ie signed number)
 
	@@@@@@@@@@@@@@@@@@@@
	@ Added for tmapper
	@@@@@@@@@@@@@@@@@@@@
	mov		r0,r0,lsr #6

	ldmfd   sp!,{r1-r7} @ pop some registers
	@bx	lr 
	mov		pc, lr 
	  
  
@		stmfd   sp!,{r4-r5}
@		ldmfd   sp!,{r4-r5}  

@@$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
        .ARM
		.code 32
        .ALIGN 4
        .GLOBL  fixedpointasm_mul_t
        .GLOBL  fixedpointasm_div_t   
		.EXTERN _binary_oneby_bin_start
@		.EXTERN	sndInterruptCounter


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ long fixedpointasm_div(long r0,long r1);   // r0 = value , r1 = divisor
fixedpointasm_div_t:
	stmfd   sp!,{r1-r7}	@ push some registers

@ r4 = signflag
	mov	r4,#0
	tst	r1,#0x80000000		@ see if neg bit is set
	subne r1,r4,r1			@ negate r1 if r1 is negative
	subeq r4,r4,#1			@ set r4 to -1 if unsigned else 0 if signed

@ 2211
@ 00224000  downshifting
@ aabbcddd  bits used
@ 211
@ 06284000
@ abcdefff

@ r2 = maskreg
@@@@@@@@@
@	ldr		r2,=0xffff0000
@	mov		r3,#0
@	tst		r1,r2
@	bne		fixedpointasmdiv_highbitvalue
@	tst		r1,#0xf000
@	movne	r3,#4
@	b	fixedpointasmdiv_knowbits
@fixedpointasmdiv_highbitvalue:
@	mov		r3,#12
@	tst		r1,#0xff000000
@	movne	r3,#20
@@@@@@@@@
	ldr		r2,=0xfff00000
	mov		r3,#0
	tst		r1,r2
	bne		fixedpointasmdiv_highbitvalue_t
	tst		r1,#0x0f000
	movne	r3,#4
	tst		r1,#0xf0000
	movne	r3,#8
	b	fixedpointasmdiv_knowbits_t
fixedpointasmdiv_highbitvalue_t:
	mov		r3,#12
	tst		r1,#0x0f000000
	movne	r3,#16
	tst		r1,#0xf0000000
	movne	r3,#20
@@@@@@@@@@
fixedpointasmdiv_knowbits_t:
@ r3 = divisior rightshifter
	mov		r1,r1,lsr r3
	ldr		r5,=_binary_oneby_bin_start
	ldr		r5,[r5,r1,lsl #2]
	mov		r5,r5,lsr r3
	smull	r1,r0,r5,r0
@ 0shift  : 22:42
@ 4shift  : 22:42 * 16
@ 12shift : 22:42 * 4096
	mov		r0,r0,lsl #17
	add		r0,r0,r1,lsr #15

	tst		r4,r4
	subeq	r0,r4,r0			@ negate result if r4=0 (ie signed number)

	ldmfd   sp!,{r1-r7} @ pop some registers
	bx	lr
	

@		stmfd   sp!,{r4-r5}
@		ldmfd   sp!,{r4-r5}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ long fixedpointasm_div(long r0,long r1);   // r0 = value , r1 = divisor
fixedpointasm_div2:
	stmfd   sp!,{r1-r7}	@ push some registers

@ r4 = signflag
	mov	r4,#0
	tst	r1,#0x80000000		@ see if neg bit is set
	subne r1,r4,r1			@ negate r1 if r1 is negative
	subeq r4,r4,#1			@ set r4 to -1 if unsigned else 0 if signed

	ldr		r2,=0xfff00000
	mov		r3,#0
	tst		r1,r2
	bne		fixedpointasmdiv_highbitvalue2
	tst		r1,#0x0f000
	movne	r3,#4
	tst		r1,#0xf0000
	movne	r3,#8
	b	fixedpointasmdiv_knowbits2
fixedpointasmdiv_highbitvalue2:
	mov		r3,#12
	tst		r1,#0x0f000000
	movne	r3,#16
	tst		r1,#0xf0000000
	movne	r3,#20
@@@@@@@@@@
fixedpointasmdiv_knowbits2:
@ r3 = divisior rightshifter
	mov		r1,r1,lsr r3
	ldr		r5,=_binary_oneby_bin_start
	ldr		r5,[r5,r1,lsl #2] 
	mov		r5,r5,lsr r3
	smull	r1,r0,r5,r0
@ 0shift  : 22:42
@ 4shift  : 22:42 * 16
@ 12shift : 22:42 * 4096
	mov		r0,r0,lsl #17
	add		r0,r0,r1,lsr #15

	tst		r4,r4
	subeq	r0,r4,r0			@ negate result if r4=0 (ie signed number)

	ldmfd   sp!,{r1-r7} @ pop some registers
	@bx	lr
	mov		pc, lr
	

@		stmfd   sp!,{r4-r5}
@		ldmfd   sp!,{r4-r5}

	.pool
	.end