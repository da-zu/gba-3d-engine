  @@@@@@@@@@@@@@@@@@@@@
@@@@@@@ DEFINES @@@@@@@@@
  @@@@@@@@@@@@@@@@@@@@@	

	CLIP		=0
	SCREEN		=4
	COLOR		=8
	COUNT1		=12			
	COUNT2		=16
	YDIFF1		=20
	YDIFF2		=24
	XDIFF1		=28
	XDIFF2		=32
	ERROR_TERM1	=36
	ERROR_TERM2	=40
	OFFSET1		=44
	OFFSET2		=48
	XUNIT1		=52
	XUNIT2		=56
	EDGE_COUNT	=60	
	FIRST_VERT	=64
	MIN_AMT		=68
	START_VERT1	=72
	START_VERT2	=76
	XSTART1		=80
	YSTART1		=84
	XSTART2		=88
	YSTART2		=92
	END_VERT1	=96
	END_VERT2	=100
	XEND1		=104
	XEND2		=108
	YEND1		=112
	YEND2		=116
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		.section	.iwram,"ax",%progbits
		.ARM
        .ALIGN
        .GLOBAL  x1GTy1_x2GTy2

x1GTy1_x2GTy2:  
		stmfd   sp!,{r4-r12,lr}		@ save stack
		
		ldr		r1, [r0, #XDIFF1]	@ count1=xdiff1	
		str		r1, [r0, #COUNT1]
		
		ldr		r2, [r0, #XDIFF2]	@ count2=xdiff2
		str		r2, [r0, #COUNT2]
		
		cmp		r1, #0
		beq		18f
		cmp		r2, #0
		beq		18f		
		
11:		@ START while(count1 && count2)

		ldr		r3, [r0, #ERROR_TERM1]
		ldr		r4, [r0, #XDIFF1]
		ldr		r5, [r0, #OFFSET1]

		ldr		r10, [r0, #OFFSET2]
		ldr		r11, [r0, #ERROR_TERM2]
		ldr		r12, [r0, #XDIFF2]
		
		cmp		r3, r4
		bge		13f
		cmp		r1, #0
		ble		13f		
		ldr		r6, [r0, #XUNIT1]
		ldr		r8, [r0, #YDIFF1]
		
		stmfd	sp!, {r7}
		ldr		r7, [r0, #XSTART1]

12:		@ START while(error_term1<xdiff1)&&(count1>0))		
		cmp		r1, #0				@ if (count1)		
		addne	r5, r5, r6			@ offset1 += xunit1		
		addne	r7, r7, r6			@ xstart1 += xunit1		
		
		add		r3, r3, r8			@ error_term1 += ydiff1

		@ plot a pixel if its time too
		@cmp		r3, r4				@ f (error_term1< xdiff1)
		@blt		19f
		
		subs	r1, r1, #1			@ count1--
		ble		13f
		cmp		r3, r4
		blt		12b
		@cmp		r1, #0
		@bgt		12b
		

13:		@ SKIP while(error_term1<xdiff1)&&(count1))		
		str		r7, [r0, #XSTART1]
		ldmfd	sp!, {r7}		

		sub		r3, r3, r4			@ error_term1 -= xdiff1	
		
		cmp		r11, r12
		bge		15f
		cmp		r2, #0
		ble		15f	
		ldr		r6, [r0, #XUNIT2]		
		ldr		r8, [r0, #YDIFF2]

		stmfd	sp!, {r7}		
		ldr		r7, [r0, #XSTART2]
		
14:		@ START while(error_term2<xdiff2)&&(count2>0))
	
		cmp		r2, #0				@ if (count2)
		addne	r10, r10, r6		@ offset2 += xunit2
		addne	r7, r7, r6			@ xstart2 += xunit2
		

		add		r11, r11, r8		@ error_term2 += ydiff2

		@ plot a pixel if its time too
		@cmp		r11, r12			@ if (error_term2< xdiff2)
		@blt		20f

		
		subs	r2, r2, #1			@ count2--
		ble		15f	
		cmp		r11, r12
		blt		14b
		@cmp		r2, #0
		

15:		@ Draw line from edge 1 to edge 2
		str		r7, [r0, #XSTART2]
		ldmfd	sp!, {r7}		
		

		sub		r11, r11, r12		@ error_term2 -= xdiff2
		sub		r6, r10, r5			@ length = offset2 - offset1
		
		stmfd	sp!,{r0}

		cmp		r6, #0				@ if (length < 0)

		sublt	r6, r6, r6, lsl #1	@	length = -length
		movlt	r7, r10				@	start = offset2
									@ else
		movge	r7, r5				@	start = offset1
		
		@ do the actual pixel plotting
		add		r6, r6, #2			@ length += 1
		ldr		r8, [r0, #CLIP]		
		ldr		r8, [r8]			@clip->color
		ldr		r9,	[r0, #SCREEN]

				
		add		r7, r7, r9			@ start += screen
		add		r6, r6, r7			@ length += start
		
	
		movs	r0, r7, rrx		
		ldrcsh	r0, [r7, #-1]!
		andcs	r0, r0, #0x00FF
		orrcs	r0, r0, r8, lsl #8
		strcsh	r0, [r7], #2

		subs	r0, r6, r7	
		@ldmeqfd	sp!,{r0}
		beq		17f

		movs	r0, r6, rrx		@ how many 2pixels? one left after?
		ldrcsh	r0, [r6, #-1]!			@ grab halfword
		biccs	r0, r0, #0x00ff		@ clear left pixel
		orrcs	r0, r0, r8, lsr	#24	@ insert left pixel
		strcsh	r0, [r6]			@ plot
		
		subs	r0, r6, r7	
		@ldmeqfd	sp!,{r0}
		beq		17f
	
16:		@ loop until all pixels are drawen
		strh	r8, [r7], #2
		subs	r0, r6, r7		
		ble		17f

		cmp		r0, #8
		strge	r8, [r7], #4
		strge	r8, [r7], #2

		subs	r0, r6, r7		
		ble		17f
		
		bgt		16b

						
		
17:

		ldmfd	sp!,{r0}
		add		r5, r5, #240		@ offset1 += 240
		str		r5, [r0, #OFFSET1]

		add		r10, r10, #240		@ offset2 += 240
		str		r10, [r0, #OFFSET2]

		ldr		r6, [r0, #YSTART1]
		add		r6, r6, #1
		str		r6, [r0, #YSTART1]

		ldr		r6, [r0, #YSTART2]
		add		r6, r6, #1
		str		r6, [r0, #YSTART2]

		str		r1, [r0, #COUNT1]		
		str		r2, [r0, #COUNT2]
		str		r3, [r0, #ERROR_TERM1]		
		str		r11, [r0, #ERROR_TERM2]

		cmp		r1, #0
		beq		18f
		cmp		r2, #0
		bne		11b

18:		@ END while(count1 && count2)

		ldmfd   sp!,{r4-r12,lr}		@ restore stack
		@bx lr
		mov		pc,lr
		
19:		@ plot a pixel a offset1
		stmfd	sp!, {r7,r8}
		
		ldr		r7, [r0, #CLIP]		
		ldr		r7, [r7]			@clip->color
		ldr		r9,	[r0, #SCREEN]
		add		r9, r9, r5			@ screen += offset1

		@ odd or even?
		tst		r9, #0x1		

		ldreqh	r8, [r9]			@ even
		biceq	r8, r8, #0x00FF
		orreq	r8, r8, r7, lsr #24

		ldrneh	r8, [r9, #-1]!		@ odd
		andne	r8, r8, #0x00FF		
		orrne	r8, r8, r7, lsl #8

		strh	r8, [r9]

		ldmfd	sp!, {r7,r8}

		sub		r1, r1, #1			@ count1--

		cmp		r3, r4
		bge		13b
		cmp		r1, #0
		bgt		12b
		bge		13b

20:		@ plot a pixel a offset2
		stmfd	sp!, {r7,r8}
		
		ldr		r7, [r0, #CLIP]		
		ldr		r7, [r7]			@clip->color
		ldr		r9,	[r0, #SCREEN]
		add		r9, r9, r10			@ screen += offset1

		@ odd or even?
		tst		r9, #0x1		

		ldreqh	r8, [r9]			@ even
		biceq	r8, r8, #0x00FF
		orreq	r8, r8, r7, lsr #24

		ldrneh	r8, [r9, #-1]!		@ odd
		andne	r8, r8, #0x00FF		
		orrne	r8, r8, r7, lsl #8

		ldmfd	sp!, {r7,r8}

		sub		r2, r2, #1			@ count2--

		cmp		r11, r12
		bge		15b
		cmp		r2, #0
		bgt		14b
		bge		15b	


		.pool
		
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		.section	.iwram,"ax",%progbits
		.ARM
        .ALIGN
        .GLOBAL  x1GTy1_x2LEy2

x1GTy1_x2LEy2:  
		stmfd   sp!,{r4-r12,lr}		@ save stack
		
		ldr		r1, [r0, #XDIFF1]	@ count1=xdiff1	
		str		r1, [r0, #COUNT1]
		ldr		r2, [r0, #YDIFF2]	@ count2=ydiff2
		str		r2, [r0, #COUNT2]
		
		cmp		r1, #0
		beq		27f
		cmp		r2, #0
		beq		27f
21:		@ START while(count1 && count2)

		ldr		r3, [r0, #ERROR_TERM1]
		ldr		r4, [r0, #XDIFF1]
		ldr		r5, [r0, #OFFSET1]

		ldr		r10, [r0, #OFFSET2]
		ldr		r11, [r0, #ERROR_TERM2]
		ldr		r12, [r0, #YDIFF2]
		
		cmp		r3, r4
		bge		23f
		cmp		r1, #0
		ble		23f		
		ldr		r6, [r0, #XUNIT1]		
		ldr		r8, [r0, #YDIFF1]

		stmfd	sp!, {r7}
		ldr		r7, [r0, #XSTART1]
22:		@ START while(error_term1<xdiff1)&&(count1>0))
		
		cmp		r1, #0				@ if (count1)		
		addne	r5, r5, r6			@	offset1 += xunit1		
		addne	r7, r7, r6			@	xstart1 += xunit1		
		
		add		r3, r3, r8			@ error_term1 += ydiff1

		@ plot a pixel if its time too
		@cmp		r3, r4			@ if (error_term1< xdiff1)
		@blt		29f

		subs	r1, r1, #1			@ count1--
		ble		23f

		cmp		r3, r4
		blt		22b
		@cmp		r1, #0
		@bgt		22b		

23:		@ SKIP while(error_term1<xdiff1)&&(count1>0))		
		str		r7, [r0, #XSTART1]
		ldmfd	sp!, {r7}
		

		sub		r3, r3, r4			@ error_term1 -= xdiff1	
		ldr		r9, [r0, #XDIFF2]
		add		r11, r11, r9		@ error_term2 += xdiff2
		
		@ if (error_term2 >= ydiff2)		
		cmp		r11, r12			@ if (error_term2 >= ydiff2)
		subge	r11, r11, r12		@	error_term2 -= ydiff2
		ldrge	r6, [r0, #XUNIT2]
		addge	r10, r10, r6		@	offset2 += xunit2
		ldrge	r7, [r0, #XSTART2]	
		addge	r7, r7, r6			@	xstart2 += xunit2
		strge	r7, [r0, #XSTART2]		

		sub		r2, r2, #1			@ count2--

24:		@ Draw line from edge 1 to edge 2
		sub		r6, r10, r5			@ length = offset2 - offset1
		
		stmfd	sp!,{r0}

		cmp		r6, #0				@ if (length < 0)
		
		sublt	r6, r6, r6, lsl #1	@	length = -length
		movlt	r7, r10				@	start = offset2
									@ else
		movge	r7, r5				@	start = offset1
		
		@ do the actual pixel plotting
		add		r6, r6, #2			@ length += 1
		ldr		r8, [r0, #CLIP]		
		ldr		r8, [r8]		@clip->color
		ldr		r9,	[r0, #SCREEN]

			
		add		r7, r7, r9			@ start += screen
		add		r6, r6, r7			@ length += start
		
	
		movs	r0, r7, rrx		
		ldrcsh	r0, [r7, #-1]!
		andcs	r0, r0, #0x00FF
		orrcs	r0, r0, r8, lsl #8
		strcsh	r0, [r7], #2

		subs	r0, r6, r7	
		@ldmeqfd	sp!,{r0}
		beq		26f

		movs	r0, r6, rrx		@ how many 2pixels? one left after?
		ldrcsh	r0, [r6, #-1]!			@ grab halfword
		biccs	r0, r0, #0x00ff		@ clear left pixel
		orrcs	r0, r0, r8, lsr	#24	@ insert left pixel
		strcsh	r0, [r6]			@ plot
		
		subs	r0, r6, r7
		@ldmeqfd	sp!,{r0}
		beq		26f
		

25:		@ loop until all pixels are drawen
		
		strh	r8, [r7], #2
		subs	r0, r6, r7		
		ble		26f

		cmp		r0, #8
		strge	r8, [r7], #4
		strge	r8, [r7], #2

		subs	r0, r6, r7		
		ble		26f
		
		bgt		25b



26:
		ldmfd	sp!,{r0}

		@ Advance edges and SAVE RUNNING VARS
		add		r5, r5, #240		@ offset1 += 240
		str		r5, [r0, #OFFSET1]

		add		r10, r10, #240		@ offset2 += 240
		str		r10, [r0, #OFFSET2]

		ldr		r6, [r0, #YSTART1]
		add		r6, r6, #1
		str		r6, [r0, #YSTART1]

		ldr		r6, [r0, #YSTART2]
		add		r6, r6, #1
		str		r6, [r0, #YSTART2]

		str		r1, [r0, #COUNT1]		
		str		r2, [r0, #COUNT2]
		str		r3, [r0, #ERROR_TERM1]		
		str		r11, [r0, #ERROR_TERM2]

		cmp		r1, #0
		beq		27f
		cmp		r2, #0		
		bne		21b
27:		@ END while(count1 && count2)
		ldmfd   sp!,{r4-r12,lr}		@ restore stack
		@bx lr
		mov		pc,lr

29:
		@ plot a pixel a offset1
		stmfd	sp!, {r7,r8}

		ldr		r7, [r0, #CLIP]		
		ldr		r7, [r7]			@clip->color
		ldr		r9,	[r0, #SCREEN]
		add		r9, r9, r5			@ screen += offset1

		@ odd or even?
		tst		r9, #0x1		

		ldreqh	r8, [r9]			@ even
		biceq	r8, r8, #0x00FF
		orreq	r8, r8, r7, lsr #24

		ldrneh	r8, [r9, #-1]!		@ odd
		andne	r8, r8, #0x00FF		
		orrne	r8, r8, r7, lsl #8


		ldmfd	sp!, {r7,r8}

		sub		r1, r1, #1			@ count1--

		cmp		r3, r4
		bge		23b
		cmp		r1, #0
		bgt		22b
		bge		23b

		.pool

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		.section	.iwram,"ax",%progbits
		.ARM
        .ALIGN
        .GLOBAL  x1LEy1_x2GTy2

x1LEy1_x2GTy2:  
		stmfd   sp!,{r4-r12,lr}		@ save stack
		
		ldr		r1, [r0, #YDIFF1]	@ count1=ydiff1	
		str		r1, [r0, #COUNT1]
		ldr		r2, [r0, #XDIFF2]	@ count2=xdiff2
		str		r2, [r0, #COUNT2]
		
		cmp		r1, #0
		beq		38f
		cmp		r2, #0
		beq		38f
31:		@ START while(count1 && count2)

		ldr		r3, [r0, #ERROR_TERM1]
		ldr		r4, [r0, #YDIFF1]
		ldr		r5, [r0, #OFFSET1]
		
		ldr		r10, [r0, #OFFSET2]
		ldr		r11, [r0, #ERROR_TERM2]
		ldr		r12, [r0, #XDIFF2]
		
				
		ldr		r8, [r0, #XDIFF1]		
		add		r3, r3, r8			@ error_term1 += xdiff1

32:		@ START if (error_term1 >= ydiff1)
		cmp		r3, r4				@ if (error_term1 >= ydiff1)
		blt		33f

		sub		r3, r3, r4			@	error_term1 -= ydiff1
		ldr		r6, [r0, #XUNIT1]
		add		r5, r5, r6			@	offset1 += xunit1
		ldr		r7, [r0, #XSTART1]	
		add		r7, r7, r6			@	xstart1 += xunit1
		str		r7, [r0, #XSTART1]


33:		@ 
		sub		r1, r1, #1			@	count1--		
		
		cmp		r11, r12			
		bge		35f
		cmp		r2, #0
		ble		35f
			
		ldr		r8, [r0, #YDIFF2]

		stmfd	sp!, {r6, r7}
		ldr		r7, [r0, #XSTART2]
		ldr		r6, [r0, #XUNIT2]

34:		@ START while((error_term2 < xdiff2)&&(count2 > 0))
		
		cmp		r2, #0				@ if (count2)		
		addne	r10, r10, r6		@ offset2 += xunit2	
		addne	r7, r7, r6			@ xstart2 += xunit2
		
		add		r11, r11, r8		@ error_term2 += ydiff2

		@ plot a pixel if its time too
		@cmp		r11, r12			@ if (error_term2< xdiff2)
		@blt		39f

		sub		r2, r2, #1			@ count2--

		cmp		r11, r12			
		bge		35f
		cmp		r2, #0
		bgt		34b

35:		@ Draw line from edge 1 to edge 2
		str		r7, [r0, #XSTART2]
		ldmfd	sp!, {r6, r7}
		
		
		sub		r11, r11, r12		@ error_term2 -= xdiff2

		sub		r6, r10, r5			@ length = offset2 - offset1
		
		stmfd	sp!,{r0}

		cmp		r6, #0				@ if (length < 0)
		
		sublt	r6, r6, r6, lsl #1	@	length = -length
		movlt	r7, r10				@	start = offset2
									@ else
		movge	r7, r5				@	start = offset1
		
		@ do the actual pixel plotting
		add		r6, r6, #2			@ length += 1
		ldr		r8, [r0, #CLIP]		
		ldr		r8, [r8]		@clip->color
		ldr		r9,	[r0, #SCREEN]

				
		add		r7, r7, r9			@ start += screen
		add		r6, r6, r7			@ length += start
		
		movs	r0, r7, rrx		
		ldrcsh	r0, [r7, #-1]!
		andcs	r0, r0, #0x00FF
		orrcs	r0, r0, r8, lsl #8
		strcsh	r0, [r7], #2

		subs	r0, r6, r7
		@ldmeqfd	sp!,{r0}
		beq		37f

		movs	r0, r6, rrx		@ how many 2pixels? one left after?
		ldrcsh	r0, [r6, #-1]!			@ grab halfword
		biccs	r0, r0, #0x00ff		@ clear left pixel
		orrcs	r0, r0, r8, lsr	#24	@ insert left pixel
		strcsh	r0, [r6]			@ plot
		
		subs	r0, r6, r7
		@ldmeqfd	sp!,{r0}
		beq		37f
		
		
36:		@ loop until all pixels are drawen
		strh	r8, [r7], #2
		subs	r0, r6, r7		
		ble		37f

		cmp		r0, #8
		strge	r8, [r7], #4
		strge	r8, [r7], #2

		subs	r0, r6, r7		
		ble		37f
		
		bgt		36b
		
		

37:
		ldmfd	sp!,{r0}

		@ Advance edges and SAVE RUNNING VARS
		add		r5, r5, #240		@ offset1 += 240
		str		r5, [r0, #OFFSET1]

		add		r10, r10, #240		@ offset2 += 240
		str		r10, [r0, #OFFSET2]

		ldr		r6, [r0, #YSTART1]
		add		r6, r6, #1
		str		r6, [r0, #YSTART1]

		ldr		r6, [r0, #YSTART2]
		add		r6, r6, #1
		str		r6, [r0, #YSTART2]

		
		str		r3, [r0, #ERROR_TERM1]		
		str		r11, [r0, #ERROR_TERM2]

		str		r1, [r0, #COUNT1]		
		str		r2, [r0, #COUNT2]

		cmp		r1, #0
		beq		38f
		cmp		r2, #0		
		bne		31b
38:		@ END while(count1 && count2)
		ldmfd   sp!,{r4-r12,lr}		@ restore stack
		@bx lr
		mov		pc,lr

39:
		@ plot a pixel a offset2
		stmfd	sp!, {r7,r8}

		ldr		r7, [r0, #CLIP]		
		ldr		r7, [r7]			@clip->color
		ldr		r9,	[r0, #SCREEN]
		add		r9, r9, r10			@ screen += offset2

		@ odd or even?
		tst		r9, #0x1		

		ldreqh	r8, [r9]			@ even
		biceq	r8, r8, #0x00FF
		orreq	r8, r8, r7, lsr #24

		ldrneh	r8, [r9, #-1]!		@ odd
		andne	r8, r8, #0x00FF		
		orrne	r8, r8, r7, lsl #8

		ldmfd	sp!, {r7,r8}

		sub		r2, r2, #1			@ count2--

		cmp		r11, r12
		bge		35b
		cmp		r2, #0
		bgt		34b
		bge		35b


		.pool		
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		.section	.iwram,"ax",%progbits
		.ARM
        .ALIGN
        .GLOBAL  x1LEy1_x2LEy2

x1LEy1_x2LEy2:  
		stmfd   sp!,{r4-r12,lr}		@ save stack
		
		ldr		r1, [r0, #YDIFF1]	@ count1=ydiff1	
		str		r1, [r0, #COUNT1]

		ldr		r2, [r0, #YDIFF2]	@ count2=ydiff2
		str		r2, [r0, #COUNT2]
		
		cmp		r1, #0
		beq		48f
		cmp		r2, #0
		beq		48f
41:		@ START while(count1 && count2)

		ldr		r3, [r0, #ERROR_TERM1]
		ldr		r4, [r0, #YDIFF1]
		ldr		r5, [r0, #OFFSET1]
		
		ldr		r10, [r0, #OFFSET2]
		ldr		r11, [r0, #ERROR_TERM2]
		ldr		r12, [r0, #YDIFF2]
		
				
		ldr		r8, [r0, #XDIFF1]		
		add		r3, r3, r8			@ error_term1 += xdiff1

42:		@ START if (error_term1 >= ydiff1)
		cmp		r3, r4				@ if (error_term1 >= ydiff1)
		blt		43f

		sub		r3, r3, r4			@	error_term1 -= ydiff1
		ldr		r6, [r0, #XUNIT1]
		add		r5, r5, r6			@	offset1 += xunit1
		ldr		r7, [r0, #XSTART1]	
		add		r7, r7, r6			@	xstart1 += xunit1
		str		r7, [r0, #XSTART1]


43:		@ 
		sub		r1, r1, #1			@	count1--		
		
		ldr		r8, [r0, #XDIFF2]		
		add		r11, r11, r8		@ error_term2 += xdiff2

		cmp		r11, r12			@ if (error_term2 >= ydiff2)
		blt		45f

		sub		r11, r11, r12		@	error_term2 -= ydiff2
		ldr		r6, [r0, #XUNIT2]
		add		r10, r10, r6		@	offset2 += xunit2
		ldr		r7, [r0, #XSTART2]	
		add		r7, r7, r6			@	xstart2 += xunit2
		str		r7, [r0, #XSTART2]
		
45:		@ Draw line from edge 1 to edge 2		
		sub		r2, r2, #1			@	count2--		

		sub		r6, r10, r5			@ length = offset2 - offset1
		
		stmfd	sp!,{r0}

		cmp		r6, #0				@ if (length < 0)
				
		sublt	r6, r6, r6, lsl #1	@	length = -length
		movlt	r7, r10				@	start = offset2
									@ else
		movge	r7, r5				@	start = offset1
		
		@ do the actual pixel plotting
		add		r6, r6, #2			@ length += 1
		ldr		r8, [r0, #CLIP]		
		ldr		r8, [r8]		@clip->color
		ldr		r9,	[r0, #SCREEN]

				
		add		r7, r7, r9			@ start += screen
		add		r6, r6, r7			@ length += start

		movs	r0, r7, rrx		
		ldrcsh	r0, [r7, #-1]!
		andcs	r0, r0, #0x00FF
		orrcs	r0, r0, r8, lsl #8
		strcsh	r0, [r7], #2

		subs	r0, r6, r7
		@ldmeqfd	sp!,{r0}
		beq		47f

		movs	r0, r6, rrx		@ how many 2pixels? one left after?
		ldrcsh	r0, [r6, #-1]!			@ grab halfword
		biccs	r0, r0, #0x00ff		@ clear left pixel
		orrcs	r0, r0, r8, lsr	#24	@ insert left pixel
		strcsh	r0, [r6]			@ plot
		
		
		subs	r0, r6, r7	
		@ldmeqfd	sp!,{r0}
		beq		47f
			
		
46:		@ loop until all pixels are drawen
@		strh	r8, [r7], #2
@		subs	r0, r6, r7		
@		ble		47f

@		cmp		r0, #8
@		strge	r8, [r7], #4
@		strge	r8, [r7], #2

@		subs	r0, r6, r7		
@		ble		47f
		
@		bgt		46b

		ldr		r0, .L1		
		ldr		r8, [r0, #CLIP]		

		ldr		r0, .L1+32		@dmasrc
		str		r8, [r0]		@dmasrc = &clip->color

		ldr		r0, .L1+36		@dmadest
		str		r7, [r0]		@dmadest

		ldr		r0, .L1+40		@dmaflags
		sub		r6, r6, r7
		orr		r0, r0, r6, lsr #1

		ldr		r6, .L1+28
		str		r0, [r6]
			
47:
		ldmfd	sp!,{r0}

		@ Advance edges and SAVE RUNNING VARS
		add		r5, r5, #240		@ offset1 += 240
		str		r5, [r0, #OFFSET1]

		add		r10, r10, #240		@ offset2 += 240
		str		r10, [r0, #OFFSET2]

		ldr		r6, [r0, #YSTART1]
		add		r6, r6, #1
		str		r6, [r0, #YSTART1]

		ldr		r6, [r0, #YSTART2]
		add		r6, r6, #1
		str		r6, [r0, #YSTART2]

		
		str		r3, [r0, #ERROR_TERM1]		
		str		r11, [r0, #ERROR_TERM2]

		str		r1, [r0, #COUNT1]		
		str		r2, [r0, #COUNT2]

		cmp		r1, #0
		beq		48f
		cmp		r2, #0		
		bne		41b
48:		@ END while(count1 && count2)
		ldmfd   sp!,{r4-r12,lr}		@ restore stack
		@bx lr
		mov		pc,lr
				
		.pool

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		.section	.iwram,"ax",%progbits
		.ARM
        .ALIGN
        .GLOBAL  AnotherEdge_Count1

AnotherEdge_Count1:  
		
		ldr		r1, [r0, #COUNT1]
		cmp		r1, #0
		bxne lr
				
		ldr		r1, [r0, #EDGE_COUNT]
		sub		r1, r1, #1
		str		r1, [r0, #EDGE_COUNT]

		ldr		r1, [r0, #END_VERT1]
		str		r1, [r0, #START_VERT1]

		sub		r1, r1, #1

		cmp		r1, #0
		@ldrlt	r2, [r0, #NUMVERTS]
		@movlt	r2, #3
		movlt	r1, #2

		str		r1, [r0, #END_VERT1]

		ldr		r2, [r0, #CLIP]
		add		r2, r2, #4
		ldr		r3, [r2, r1, lsl #4]!
		bics	r3, r3, r3, asr #31
		cmp		r3, #239	
		movgt	r3, #239

		str		r3, [r0, #XEND1]

		ldr		r3, [r2, #4]
		bics	r3, r3, r3, asr #31
		cmp		r3, #160	
		movgt	r3, #160

		str		r3, [r0, #YEND1]

		
		@bx lr
		mov		pc,lr

		.pool

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	 
		.section	.iwram,"ax",%progbits
		.ARM
        .ALIGN
        .GLOBAL  AnotherEdge_Count2

AnotherEdge_Count2:  

		ldr		r1, [r0, #COUNT2]
		cmp		r1, #0
		bxne lr
				
		ldr		r1, [r0, #EDGE_COUNT]
		sub		r1, r1, #1
		str		r1, [r0, #EDGE_COUNT]

		ldr		r1, [r0, #END_VERT2]
		str		r1, [r0, #START_VERT2]

		add		r1, r1, #1

		@ldr		r2, [r0, #NUMVERTS]
		@mov		r2, #3
		cmp		r1, #3
		moveq	r1, #0

		str		r1, [r0, #END_VERT2]
		
		ldr		r2, [r0, #CLIP]
		add		r2, r2, #4
		ldr		r3, [r2, r1, lsl #4]!
		bics	r3, r3, r3, asr #31
		cmp		r3, #239	
		movgt	r3, #239

		str		r3, [r0, #XEND2]

		ldr		r3, [r2, #4]
		bics	r3, r3, r3, asr #31
		cmp		r3, #160	
		movgt	r3, #160

		str		r3, [r0, #YEND2]

		@bx lr
		mov		pc,lr

		.pool

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ void Draw_Poly ( clipped_polygon_type *clip, u32 *screen )
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	 
		.section	.iwram,"ax",%progbits
		.ARM
        .ALIGN
        .GLOBAL  Draw_Poly32
		
Draw_Poly32:  
		
		stmfd   sp!,{r0-r12,lr}		@ save stack

		ldr		r2, .L1

@ r0 = *clip
@ r1 = *screen
@ r2 = vars
		
		@ vars.clip = clip;
		str		r0, [r2]

		@ vars.screen = screen;
		str		r1, [r2, #4]

		@ vars.edge_count = 2;
		mov		r3, #2
		str		r3, [r2, #60]

		@ vars.first_vert = 0;
		mov		r3, #0
@ r3 = first_vert

		@ vars.min_amt = (clip->vertex[0].y);
		ldr		r4, [r0, #8]
@ r4 = min_amt

		ldr		r5, [r0, #24]

		cmp		r5, r4
		movlt	r3, #1
		movlt	r4, r5

		ldr		r5, [r0, #40]

		cmp		r5, r4
		movlt	r3, #2
@		movlt	r4, r5

		@str		r4, [r2, #68]		@@@MAY NOT BE REQUIRED!
@ r0 = *clip
@ r1 = *screen
@ r2 = vars
@ r3 = first_vert !NOT stored yet!
@ r4 FREE
@ r5 FREE

		mov		r5, r3, lsl #4
		add		r5, r5, #4
		@ vars.xstart1 = clip->vertex[vars.start_vert1].x;
		@ vars.xstart2 = clip->vertex[vars.start_vert2].x;
		ldr		r4, [r0, r5]
		bics	r4, r4, r4, asr #31
		cmp		r4, #239
		movgt	r4, #239
		

@ r4 = xstart1 and xstart2 !NOT stored yet!

		add		r5, r5, #4

		@ vars.ystart1 = clip->vertex[vars.start_vert1].y;
		@ vars.ystart2 = clip->vertex[vars.start_vert2].y;
		ldr		r5, [r0, r5]
		bics	r5, r5, r5, asr #31
		cmp		r5, #160	
		movgt	r5, #160
				
@ r5 = ystart1 and ystart2 !NOT stored yet!
		
		str		r3, [r2, #64]		@ store first_vert
		str		r3, [r2, #72]		@ store start_vert1
		str		r3, [r2, #76]		@ store start_vert2
		mov		r8, r3

@ r8 = start_vert2

		@ vars.end_vert1 = vars.start_vert1 - 1;
		subs	r3, r3, #1		
		@ if ( vars.end_vert1 < 0 )
		@		vars.end_vert1 = 2;
		movlt	r3, #2
		
		str		r3, [r2, #96]		@ store end_vert1

@ r0 = *clip
@ r1 = *screen
@ r2 = vars
@ r3 = end_vert1 
@ r4 = xstart1 & xstart2 !NOT stored yet!
@ r5 = ystart1 & ystart2 !NOT stored yet!
@ r8 = start_vert2 !KILLABLE!

		@ vars.xend1 = clip->vertex[vars.end_vert1].x;
		mov		r3, r3, lsl #4
		add		r3, r3, #4
		ldr		r6, [r0, r3]

		bics	r6, r6, r6, asr #31
		cmp		r6, #239	
		movgt	r6, #239

@ r6 = vars.xend1 !NOT stored yet!

		@ vars.yend1 = clip->vertex[vars.end_vert1].y;
		add		r3, r3, #4
		ldr		r7, [r0, r3]
		
		bics	r7, r7, r7, asr #31
		cmp		r7, #160	
		movgt	r7, #160

@ r7 = vars.yend1 !NOT stored yet!
		
		@ vars.end_vert2 = vars.start_vert2 + 1;
		add		r8, r8, #1
		@ if ( vars.end_vert2 == 3)
		cmp		r8, #3
		@		vars.end_vert2 =0;
		moveq	r8, #0

		str		r8, [r2, #100]

@ r0 = *clip
@ r1 = *screen
@ r2 = vars
@ r3 = FREE
@ r4 = xstart1 & xstart2 !NOT stored yet!
@ r5 = ystart1 & ystart2 !NOT stored yet!
@ r6 = vars.xend1 !NOT stored yet!
@ r7 = vars.yend1 !NOT stored yet!
@ r8 = vars.end_vert2 !KILLABLE!

		mov		r3, r8, lsl #4
		add		r3, r3, #4
		@ vars.xend2 = clip->vertex[vars.end_vert2].x;
		ldr		r8, [r0, r3]

		bics	r8, r8, r8, asr #31
		cmp		r8, #239	
		movgt	r8, #239

@ r8 = vars.xend2

		add		r3, r3, #4

		@ vars.yend2 = clip->vertex[vars.end_vert2].y;
		ldr		r9, [r0, r3]

		bics	r9, r9, r9, asr #31
		cmp		r9, #160	
		movgt	r9, #160

@ r9 = vars.yend2
		
		@ vars.color = clip->color;
		ldr		r3, [r0]
		str		r3, [r2, #8]

@ r0 = *clip
@ r1 = *screen
@ r2 = vars
@ r3 = FREE
@ r4 = xstart1 & xstart2 !NOT stored yet!
@ r5 = ystart1 & ystart2 !NOT stored yet!
@ r6 = vars.xend1 !NOT stored yet!
@ r7 = vars.yend1 !NOT stored yet!
@ r8 = vars.xend2 !NOT stored yet!
@ r9 = vars.yend2 !NOT stored yet!

		str		r4, [r2, #80]		@ store xstart1
		str		r4, [r2, #88]		@ store xstart2

		str		r5, [r2, #84]		@ store ystart1
		str		r5, [r2, #92]		@ store ystart2

		str		r6, [r2, #104]		@ store xend1
		str		r7, [r2, #112]		@ store yend1
		str		r8, [r2, #108]		@ store xend2
		str		r9, [r2, #116]		@ store yend2


1:		@ while (vars.edge_count>0) 
@ r0 = *clip
@ r1 = *screen
@ r2 = vars
@ r3 = FREE
@ r4 = FREE
@ r5 = FREE
@ r6 = FREE
@ r7 = FREE
@ r8 = FREE
@ r9 = FREE
		
		ldr		r3, [r2, #84]
		bics	r3, r3, r3, asr #31
		cmp		r3, #160
		movgt	r3, #160

		ldr		r4, [r2, #80]
		bics	r4, r4, r4, asr #31
		cmp		r4, #239
		movgt	r4, #239

@ r3 = ystart1
@ r4 = xstart1
		
		@ vars.offset1= ( (vars.ystart1<<7) + (vars.ystart1<<6) + (vars.ystart1<<5) + (vars.ystart1<<4) )+ vars.xstart1
		mov		r5, r3, lsl #7
		add		r5, r5, r3, lsl #6
		add		r5, r5, r3, lsl #5
		add		r5, r5, r3, lsl #4

		add		r5, r5, r4

@ r5 = offset1
		str		r5, [r2, #44]



		ldr		r5, [r2, #92]
		bics	r5, r5, r5, asr #31
		cmp		r5, #160
		movgt	r5, #160

		ldr		r6, [r2, #88]
		bics	r6, r6, r6, asr #31
		cmp		r6, #239
		movgt	r6, #239

@ r5 = ystart2
@ r6 = xstart2
		
		@ vars.offset2= ( (vars.ystart2<<7) + (vars.ystart2<<6) + (vars.ystart2<<5) + (vars.ystart2<<4) )+ vars.xstart2
		mov		r7, r5, lsl #7
		add		r7, r7, r5, lsl #6
		add		r7, r7, r5, lsl #5
		add		r7, r7, r5, lsl #4

		add		r7, r7, r6

@ r7 = offset2
		str		r7, [r2, #48]


@ r0 = *clip
@ r1 = *screen
@ r2 = vars
@ r3 = ystart1 !KILLABLE!
@ r4 = xstart1 !KILLABLE!
@ r5 = ystart2 !KILLABLE!
@ r6 = xstart2 !KILLABLE!
@ r7 = offset2 !KILLABLE!
@ r8 = FREE
@ r9 = FREE

			mov		r8, #0
			@ vars.error_term1=0;
			str		r8, [r2, #36]

			@ vars.error_term2=0;
			str		r8, [r2, #40]


			@ vars.ydiff1=vars.yend1-vars.ystart1;		
			ldr		r8, [r2, #112]

@ r8 = yend1 !KILLABLE!

			subs	r8, r8, r3			
			@ if (vars.ydiff1<0) 
			@		vars.ydiff1=-vars.ydiff1
			sublt	r8, r8, r8, lsl #1

@ r8 = ydfiff1 !NOT stored yet!

			@ vars.ydiff2=vars.yend2-vars.ystart2;
			ldr		r9, [r2, #116]
@ r9 = yend2 !KILLABLE!

			subs	r9, r9, r5			
			@ if (vars.ydiff2<0) 
			@		vars.ydiff2=-vars.ydiff2
			sublt	r9, r9, r9, lsl #1

@ r9 = ydiff2 !NOT stored yet!

			str		r8, [r2, #20]		@ store ydiff1
			str		r9, [r2, #24]		@ store ydiff2

@ r0 = *clip
@ r1 = *screen
@ r2 = vars
@ r3 = ystart1 !KILLABLE!
@ r4 = xstart1 !KILLABLE!
@ r5 = ystart2 !KILLABLE!
@ r6 = xstart2 !KILLABLE!
@ r7 = offset2 !KILLABLE!
@ r8 = ydiff1 !KILLABLE! try too keep
@ r9 = ydiff2 !KILLABLE! try too keep

		
			@ vars.xdiff1=vars.xend1-vars.xstart1;
			ldr		r7, [r2, #104]

			subs		r7, r7, r4			
			@ if (vars.xdiff1<0) 
			@		vars.xunit1=-1;
			movlt	r4, #-1
			@		vars.xdiff1=-vars.xdiff1;
			sublt	r7, r7, r7, lsl #1
			@ else 
			@		vars.xunit1=1;
			movge	r4, #1
			
@ r7 = xdiff1 !NOT saved yet!
			str		r4, [r2, #52]		@ store xunit1
@ r4 = xunit1

			
			@ vars.xdiff2=vars.xend2-vars.xstart2;
			ldr		r5, [r2, #108]
			subs	r6, r5, r6			
			@ if (vars.xdiff2<0)
			movlt	r4, #-1
			sublt	r6, r6, r6, lsl #1
			movge	r4, #1

			str		r4, [r2, #56]		@ store xunit2
@ r4 = xunit2
@ r6 = xdiff2 !NOT saved yet!

		
			str		r6, [r2, #32]		@ store xdiff2
			str		r7, [r2, #28]		@ store xdiff1

@ r0 = FREE
@ r1 = FREE
@ r2 = vars
@ r3 = FREE
@ r4 = FREE
@ r5 = FREE
@ r6 = xdiff2 !KILLABLE! try too keep
@ r7 = xdiff1 !KILLABLE! try too keep
@ r8 = ydiff1 !KILLABLE! try too keep
@ r9 = ydiff2 !KILLABLE! try too keep

			mov		r0, r2

			cmp		r7, r8		@ if (vars.xdiff1>vars.ydiff1) 
			ble		2f

			stmfd   sp!,{r0-r3,lr}		@ restore stack
			
			cmp		r6, r9		@		if (vars.xdiff2>vars.ydiff2) 	
			ldrgt	r3, .L1+4
			movgt	lr, pc
			bxgt	r3
			
			ldmfd   sp!,{r0-r3,lr}		@ restore stack
			
			cmp		r6, r9

			stmfd   sp!,{r0-r3,lr}		@ restore stack

			ldrle	r3, .L1+16
			movle	lr, pc
			bxle	r3

			ldmfd   sp!,{r0-r3,lr}		@ restore stack

@			blgt	x1GTy1_x2GTy2
@			blle	x1GTy1_x2LEy2
			
			mov		r0, r2

			b		3f
2: 			
			mov		r0, r2

			stmfd   sp!,{r0-r3,lr}		@ restore stack

			cmp		r6, r9		@ if (vars.xdiff2>vars.ydiff2) 
			ldrgt	r3, .L1+20
			movgt	lr, pc
			bxgt	r3
			
			ldmfd   sp!,{r0-r3,lr}		@ restore stack
						
			cmp		r6, r9

			stmfd   sp!,{r0-r3,lr}		@ restore stack

			ldrle	r3, .L1+24
			movle	lr, pc
			bxle	r3

@			blgt	x1LEy1_x2GTy2
@			blle	x1LEy1_x2LEy2

			ldmfd   sp!,{r0-r3,lr}		@ restore stack

3:
@			mov		r0, r2

			stmfd   sp!,{r0-r3,lr}		@ restore stack
			ldr	r3, .L1+8
			mov	lr, pc
			bx	r3
			ldmfd   sp!,{r0-r3,lr}		@ restore stack
			
			mov		r0, r2

			stmfd   sp!,{r0-r3,lr}		@ restore stack
			ldr	r3, .L1+12
			mov	lr, pc
			bx	r3
			ldmfd   sp!,{r0-r3,lr}		@ restore stack

			ldr		r3, [r2, #60]
			cmp		r3, #0
			bgt		1b

			ldmfd   sp!,{r0-r12,lr}		@ restore stack

			@mov		pc,lr
			bx		lr

.L1:
	.word	vars
	.word	x1GTy1_x2GTy2
	.word	AnotherEdge_Count1
	.word	AnotherEdge_Count2
	.word	x1GTy1_x2LEy2
	.word	x1LEy1_x2GTy2
	.word	x1LEy1_x2LEy2
	.word	0x040000DC		@dmacnt
	.word	0x040000D4		@dma src
	.word	0x040000D8		@dmadest
	.word	0x81000000		@dmaflags

		.pool

		.END

@ #define REG_DMA3        (REG_BASE + 0xd4)   // DMA 3
@ #define REG_DMA3SAD     (REG_BASE + 0xd4)   // (Game Pak/Internal Memory <--> 
                                            // Internal Memory)
@ #define REG_DMA3SAD_L   (REG_BASE + 0xd4)
@ #define REG_DMA3SAD_H   (REG_BASE + 0xd6)
@ #define REG_DMA3DAD     (REG_BASE + 0xd8)
@ #define REG_DMA3DAD_L   (REG_BASE + 0xd8)
@ #define REG_DMA3DAD_H   (REG_BASE + 0xda)
@ #define REG_DMA3CNT     (REG_BASE + 0xdc)
@ #define REG_DMA3CNT_L   (REG_BASE + 0xdc)
@ #define REG_DMA3CNT_H   (REG_BASE + 0xde)
@(                            
@DMA_ENABLE         | DMA_TIMMING_IMM |         
@DMA_SRC_FIX        | DMA_DEST_INC    |         
@DMA_##Bit##BIT_BUS | ((Size)/(Bit/8))));
