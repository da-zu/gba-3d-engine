@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ ProcPolyList2(u32 count, polygon_list_type *pPolyList, u32 *screen_buf )
@  	 
@	(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		.text
		.section	.iwram,"ax",%progbits		
		.ARM
        .ALIGN			
		.EXTERN PrjTable	
		.EXTERN TriFillARM
        .GLOBAL  ProcPolyList2  
 	 
		 
ProcPolyList2:    
@r0 = count
@r1 = pPolyList 
@r2 = screen_buf 

		
		cmp		r0, #0					@ if count == 0 no need to process so return
		moveq	pc, lr

		stmfd   sp!,{r4-r12,lr}			@ push reg onto stack
	
		@ rearange regs to save moving them inside loop when calling triFillARM
		mov		r7,  r2					@ move screen_buf into param7
		mov		r9, r0					@ move count into spare reg		
		mov		r10, r1					@ move pPolyLisy into spare reg

		ldr		r12, =PrjTable

1:

		ldr		r8, [r10], #4			@ derefence polygon_list_type (pPolyList)
		@dont need to add #8 anymore as i rearranged polygon type to have vertex list at top
		@add		r8, r8, #8
		ldmia	r8, {r0,r3,r6}			@ dereference pPolyList->polygon[i].vertex[x]

@@@@@@@@@@@@@@@@@@@@@@@@@@
@		VERTEX 0
@@@@@@@@@@@@@@@@@@@@@@@@@@
		
		add		r0, r0, #24
		ldmia	r0, {r0-r2}				@ get ax,ay,az (pPolyList->polygon[i].vertex[0]->ax)
		
		@r0 = ax/az		
		mov		r0, r0, lsl #8
		ldr		r11, [r12, r2, asl #2]
		mul		r0, r11, r0
		mov		r0, r0, asr #16
		add		r0, r0, #120

		@r1 = ay/az
		mov		r1, r1, lsl #8
		ldr		r11, [r12, r2, asl #2]
		mul		r1, r11, r1
		mov		r1, r1, asr #16		
		add		r1, r1, #80

@@@@@@@@@@@@@@@@@@@@@@@@@@
@		VERTEX 1
@@@@@@@@@@@@@@@@@@@@@@@@@@
		
		add		r3, r3, #24
		ldmia	r3, {r2-r4}			@ get ax,ay,az (pPolyList->polygon[i].vertex[0]->ax)

		@r2 = ax/az
		mov		r2, r2, lsl #8
		ldr		r11, [r12, r4, asl #2]
		mul		r2, r11, r2
		mov		r2, r2, asr #16		
		add		r2, r2, #120
		
		@r3 = ay/az
		mov		r3, r3, lsl #8
		ldr		r11, [r12, r4, asl #2]
		mul		r3, r11, r3
		mov		r3, r3, asr #16		
		add		r3, r3, #80

@@@@@@@@@@@@@@@@@@@@@@@@@@
@		VERTEX 2
@@@@@@@@@@@@@@@@@@@@@@@@@@
		
		add		r6, r6, #24
		ldmia	r6, {r4-r6}			@ get ax,ay,az (pPolyList->polygon[i].vertex[0]->ax)

		@r4 = ax/az
		mov		r4, r4, lsl #8
		ldr		r11, [r12, r6, asl #2]
		mul		r4, r11, r4
		mov		r4, r4, asr #16		
		add		r4, r4, #120
		
		@r5 = ay/az		
		mov		r5, r5, lsl #8
		ldr		r11, [r12, r6, asl #2]
		mul		r5, r11, r5
		mov		r5, r5, asr #16		
		add		r5, r5, #80
		

		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ BACKFACE CULL IN SCREENSPACE		
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@visible = (vx2-vx1)*(vy3-vy2);
		@visible-= (vy2-vy1)*(vx3-vx2);
		@if (visible < 0) DRAW IT!

		@ DONT USE r8!!!!!!!!
		
		@sub		r8, r2, r0
		@sub		r11, r5, r3
		@mul		r8, r11, r8

		@sub		r11, r3, r1
		@sub		r6, r4, r2
		@mul		r6, r11, r6

		@subs	r8, r8, r6
		@bge		noFill


		@ldr		r6, [r10, #-4]			@ retrieve polygon color
		ldr		r6, [r8, #12]			@ retrieve polygon color
		@ldr		r6, [r6]


		@HACK FOR NOW TO CLIP TRIANGLE!!!!!
		@ X1
		cmp		r0, #1
		movlt	r0, #1
		cmp		r0, #239
		movgt	r0, #239

		@ Y1
		cmp		r1, #1
		movlt	r1, #1
		cmp		r1, #159
		movgt	r1, #159

		@ X2
		cmp		r2, #1
		movlt	r2, #1
		cmp		r2, #239
		movgt	r2, #239

		@ Y2
		cmp		r3, #1
		movlt	r3, #1
		cmp		r3, #159
		movgt	r3, #159

		@ X3
		cmp		r4, #1
		movlt	r4, #1
		cmp		r4, #239
		movgt	r4, #239

		@ Y3
		cmp		r5, #1
		movlt	r5, #1
		cmp		r5, #159
		movgt	r5, #159

@ void TriFillARM(ULONG x0,ULONG y0,ULONG x1,ULONG y1,ULONG x2,ULONG y2,ULONG col,ULONG vid);				
TriFillARM:
@--------------------------------
@ save sp to get params
@--------------------------------
 	@mov	r12,sp		 
@--------------------------------
@ save some registers
@-------------------------------- 	
	stmfd	sp!,{r0-r12,lr}
@--------------------------------
@ get params :
@	r4 = x2
@	r5 = y2
@	r6 = color
@	r7 = video
@--------------------------------	
	@ldmfd	r12,{r4-r7}	@ ??????????????????????????????????????????

@--------------------------------
@ preset color word
@--------------------------------
	orr	r6,r6,r6,lsl#8
@--------------------------------
@ sort by y
@-------------------------------- 
	cmp	r1,r3		@ y0>y1 ?
	blt	.y0y2
	eor	r0,r0,r2	@ swap x
	eor	r2,r2,r0
	eor	r0,r0,r2
	
	eor	r1,r1,r3	@ swap y
	eor	r3,r3,r1	
	eor	r1,r1,r3

.y0y2:	
	cmp	r1,r5		@ y0>y2 ?
	blt	.y1y2
	eor	r0,r0,r4	@ swap x
	eor	r4,r4,r0
	eor	r0,r0,r4
	
	eor	r1,r1,r5	@ swap y
	eor	r5,r5,r1	
	eor	r1,r1,r5
.y1y2:	
	cmp	r3,r5		@ y2>y3 ?
	blt	.ysorted
	eor	r2,r2,r4	@ swap x
	eor	r4,r4,r2
	eor	r2,r2,r4
	
	eor	r3,r3,r5	@ swap y
	eor	r5,r5,r3	
	eor	r3,r3,r5

@--------------------------------
@ needed fixes for x0 and x1
@--------------------------------
.ysorted:
	cmp	r1,r3
	bne	.nox0x1
	cmp	r0,r2
	eorgt	r0,r0,r2
	eorgt	r2,r2,r0
	eorgt	r0,r0,r2
.nox0x1:
@--------------------------------
@ calculate delta x
@--------------------------------
	sub	r8,r2,r0	@ delta x B-A	( upper tri height)
	sub	r9,r4,r2	@ delta x C-B	( lower tri height)
	sub	r10,r4,r0	@ delta x C-A	( whole tri height)
@--------------------------------
@ setup video y position
@--------------------------------
	mov	lr,r1,lsl#8	@ lr = y * 256
	sub	lr,lr,r1,lsl#4	@ lr = ((y*256)-(y*16)) = y * 240
	add	r7,r7,lr	@ first y scanline
@--------------------------------
@ calculate delta y
@--------------------------------	
	sub	r11,r3,r1	@ delta y B-A
	sub	lr,r5,r1	@ delta y C-A
	sub	r12,r5,r3	@ delta y C-B
@--------------------------------
@ load divs value
@--------------------------------
	ldr	r1,=div_table_here
		
	add	r3,r1,r11,lsl#2
	ldr	r4,[r3]		@ 1 / delta y B-A
	
	add	r3,r1,lr,lsl#2
	ldr	r5,[r3]		@ 1 / delta y C-A
	
	add	r3,r1,r12,lsl#2
	ldr	r3,[r3]		@ 1 / delta y C-B
@--------------------------------
@ precalc slopes
@--------------------------------
	mul	r4,r8,r4	@ dABx/dABy
	mul	r5,r10,r5	@ dACx/dACy
	mul	r3,r9,r3	@ dBCx/dBCy
@--------------------------------
@ choose kind of tri
@--------------------------------	
	subs	r8,r4,r5		
	bmi	.leftside
	
	mov	r8,r3	
	mov	r9,r5
	bal	.goon
.leftside:	
	mov	r8,r5
	mov	r9,r3

	eor	r4,r4,r5	
	eor	r5,r5,r4
	eor	r4,r4,r5
@--------------------------------
@ initialization and drawing
@--------------------------------		
.goon:
	add	r12,r12,#1
	
	mov	r1,r0,lsl#16	@ start x << 16
	mov	r0,r1		@ start x << 16
	
	cmp	r11,#0		@ up tri have zero height ?
	beq	.adjustdowntri	@ yes then it's only down
	
.outerloop:	
	mov	r2,r0,lsr#16
	rsb	r3,r2,r1,lsr#16	@ r3 = delta x , r2 = start x
	
	add	r2,r7,r2	@ begin of line
	
@--------------------------------
@ tricky startpixel
@--------------------------------
	tst	r2,#1
	addeq	r3,r3,#1
	beq	.alignedwrites
	
	bic	r2,r2,#1
	ldrh	r10,[r2]
	bic	r10,r10,#0xff00
	orr	r10,r10,r6,lsl#8
	strh	r10,[r2],#2
	
@-------------------------------
.alignedwrites:
	movs 	r3,r3,lsr#1
	rsb	r3,r3,#119
	
	add	pc,pc,r3,lsl#2
	
	.REPT	120
	strh	r6,[r2],#2
	.ENDR

	bcc	.nextscanline
 
 	ldrh	r10,[r2]
	bic	r10,r10,#0x00ff
	orr	r10,r10,r6,lsr#8
	strh	r10,[r2]
	
.nextscanline:
	add	r7,r7,#240	@ nextline
	add	r0,r0,r5
.adjustdowntri:
	add	r1,r1,r4
	subs	r11,r11,#1
	bgt	.outerloop

	cmp	r0,r1
	eorgt	r0,r0,r1
	eorgt	r1,r1,r0
	eorgt	r0,r0,r1

	mov	r4,r8
	mov	r5,r9
	mov	r11,r12
	mov	r12,#-1
	
	subs	r11,r11,#1 
	bge	.outerloop

@	nop
 	ldmfd	sp!,{r0-r12,lr}		
@	mov		pc, lr

		
noFill:
		subs	r9, r9, #1
		bne		1b

		@ EXIT
		ldmfd   sp!,{r4-r12,lr}			@ pop regs from stack
		bx lr
		
		.pool

			.ALIGN
div_table_here:
	.word	0x00010000	@ 1/0 << 16
	.word	0x00010000	@ 1/1 << 16
	.word	0x00008000	@ 1/2 << 16
	.word	0x00005555	@ 1/3 << 16
	.word	0x00004000	@ 1/4 << 16
	.word	0x00003333	@ 1/5 << 16
	.word	0x00002aab	@ 1/6 << 16
	.word	0x00002492	@ 1/7 << 16
	.word	0x00002000	@ 1/8 << 16
	.word	0x00001c72	@ 1/9 << 16
	.word	0x0000199a	@ 1/10 << 16
	.word	0x00001746	@ 1/11 << 16
	.word	0x00001555	@ 1/12 << 16
	.word	0x000013b1	@ 1/13 << 16
	.word	0x00001249	@ 1/14 << 16
	.word	0x00001111	@ 1/15 << 16
	.word	0x00001000	@ 1/16 << 16
	.word	0x00000f0f	@ 1/17 << 16
	.word	0x00000e39	@ 1/18 << 16
	.word	0x00000d79	@ 1/19 << 16
	.word	0x00000ccd	@ 1/20 << 16
	.word	0x00000c31	@ 1/21 << 16
	.word	0x00000ba3	@ 1/22 << 16
	.word	0x00000b21	@ 1/23 << 16
	.word	0x00000aab	@ 1/24 << 16
	.word	0x00000a3d	@ 1/25 << 16
	.word	0x000009d9	@ 1/26 << 16
	.word	0x0000097b	@ 1/27 << 16
	.word	0x00000925	@ 1/28 << 16
	.word	0x000008d4	@ 1/29 << 16
	.word	0x00000889	@ 1/30 << 16
	.word	0x00000842	@ 1/31 << 16
	.word	0x00000800	@ 1/32 << 16
	.word	0x000007c2	@ 1/33 << 16
	.word	0x00000788	@ 1/34 << 16
	.word	0x00000750	@ 1/35 << 16
	.word	0x0000071c	@ 1/36 << 16
	.word	0x000006eb	@ 1/37 << 16
	.word	0x000006bd	@ 1/38 << 16
	.word	0x00000690	@ 1/39 << 16
	.word	0x00000666	@ 1/40 << 16
	.word	0x0000063e	@ 1/41 << 16
	.word	0x00000618	@ 1/42 << 16
	.word	0x000005f4	@ 1/43 << 16
	.word	0x000005d1	@ 1/44 << 16
	.word	0x000005b0	@ 1/45 << 16
	.word	0x00000591	@ 1/46 << 16
	.word	0x00000572	@ 1/47 << 16
	.word	0x00000555	@ 1/48 << 16
	.word	0x00000539	@ 1/49 << 16
	.word	0x0000051f	@ 1/50 << 16
	.word	0x00000505	@ 1/51 << 16
	.word	0x000004ec	@ 1/52 << 16
	.word	0x000004d5	@ 1/53 << 16
	.word	0x000004be	@ 1/54 << 16
	.word	0x000004a8	@ 1/55 << 16
	.word	0x00000492	@ 1/56 << 16
	.word	0x0000047e	@ 1/57 << 16
	.word	0x0000046a	@ 1/58 << 16
	.word	0x00000457	@ 1/59 << 16
	.word	0x00000444	@ 1/60 << 16
	.word	0x00000432	@ 1/61 << 16
	.word	0x00000421	@ 1/62 << 16
	.word	0x00000410	@ 1/63 << 16
	.word	0x00000400	@ 1/64 << 16
	.word	0x000003f0	@ 1/65 << 16
	.word	0x000003e1	@ 1/66 << 16
	.word	0x000003d2	@ 1/67 << 16
	.word	0x000003c4	@ 1/68 << 16
	.word	0x000003b6	@ 1/69 << 16
	.word	0x000003a8	@ 1/70 << 16
	.word	0x0000039b	@ 1/71 << 16
	.word	0x0000038e	@ 1/72 << 16
	.word	0x00000382	@ 1/73 << 16
	.word	0x00000376	@ 1/74 << 16
	.word	0x0000036a	@ 1/75 << 16
	.word	0x0000035e	@ 1/76 << 16
	.word	0x00000353	@ 1/77 << 16
	.word	0x00000348	@ 1/78 << 16
	.word	0x0000033e	@ 1/79 << 16
	.word	0x00000333	@ 1/80 << 16
	.word	0x00000329	@ 1/81 << 16
	.word	0x0000031f	@ 1/82 << 16
	.word	0x00000316	@ 1/83 << 16
	.word	0x0000030c	@ 1/84 << 16
	.word	0x00000303	@ 1/85 << 16
	.word	0x000002fa	@ 1/86 << 16
	.word	0x000002f1	@ 1/87 << 16
	.word	0x000002e9	@ 1/88 << 16
	.word	0x000002e0	@ 1/89 << 16
	.word	0x000002d8	@ 1/90 << 16
	.word	0x000002d0	@ 1/91 << 16
	.word	0x000002c8	@ 1/92 << 16
	.word	0x000002c1	@ 1/93 << 16
	.word	0x000002b9	@ 1/94 << 16
	.word	0x000002b2	@ 1/95 << 16
	.word	0x000002ab	@ 1/96 << 16
	.word	0x000002a4	@ 1/97 << 16
	.word	0x0000029d	@ 1/98 << 16
	.word	0x00000296	@ 1/99 << 16
	.word	0x0000028f	@ 1/100 << 16
	.word	0x00000289	@ 1/101 << 16
	.word	0x00000283	@ 1/102 << 16
	.word	0x0000027c	@ 1/103 << 16
	.word	0x00000276	@ 1/104 << 16
	.word	0x00000270	@ 1/105 << 16
	.word	0x0000026a	@ 1/106 << 16
	.word	0x00000264	@ 1/107 << 16
	.word	0x0000025f	@ 1/108 << 16
	.word	0x00000259	@ 1/109 << 16
	.word	0x00000254	@ 1/110 << 16
	.word	0x0000024e	@ 1/111 << 16
	.word	0x00000249	@ 1/112 << 16
	.word	0x00000244	@ 1/113 << 16
	.word	0x0000023f	@ 1/114 << 16
	.word	0x0000023a	@ 1/115 << 16
	.word	0x00000235	@ 1/116 << 16
	.word	0x00000230	@ 1/117 << 16
	.word	0x0000022b	@ 1/118 << 16
	.word	0x00000227	@ 1/119 << 16
	.word	0x00000222	@ 1/120 << 16
	.word	0x0000021e	@ 1/121 << 16
	.word	0x00000219	@ 1/122 << 16
	.word	0x00000215	@ 1/123 << 16
	.word	0x00000211	@ 1/124 << 16
	.word	0x0000020c	@ 1/125 << 16
	.word	0x00000208	@ 1/126 << 16
	.word	0x00000204	@ 1/127 << 16
	.word	0x00000200	@ 1/128 << 16
	.word	0x000001fc	@ 1/129 << 16
	.word	0x000001f8	@ 1/130 << 16
	.word	0x000001f4	@ 1/131 << 16
	.word	0x000001f0	@ 1/132 << 16
	.word	0x000001ed	@ 1/133 << 16
	.word	0x000001e9	@ 1/134 << 16
	.word	0x000001e5	@ 1/135 << 16
	.word	0x000001e2	@ 1/136 << 16
	.word	0x000001de	@ 1/137 << 16
	.word	0x000001db	@ 1/138 << 16
	.word	0x000001d7	@ 1/139 << 16
	.word	0x000001d4	@ 1/140 << 16
	.word	0x000001d1	@ 1/141 << 16
	.word	0x000001ce	@ 1/142 << 16
	.word	0x000001ca	@ 1/143 << 16
	.word	0x000001c7	@ 1/144 << 16
	.word	0x000001c4	@ 1/145 << 16
	.word	0x000001c1	@ 1/146 << 16
	.word	0x000001be	@ 1/147 << 16
	.word	0x000001bb	@ 1/148 << 16
	.word	0x000001b8	@ 1/149 << 16
	.word	0x000001b5	@ 1/150 << 16
	.word	0x000001b2	@ 1/151 << 16
	.word	0x000001af	@ 1/152 << 16
	.word	0x000001ac	@ 1/153 << 16
	.word	0x000001aa	@ 1/154 << 16
	.word	0x000001a7	@ 1/155 << 16
	.word	0x000001a4	@ 1/156 << 16
	.word	0x000001a1	@ 1/157 << 16
	.word	0x0000019f	@ 1/158 << 16
	.word	0x0000019c	@ 1/159 << 16
	.word	0x0000019a	@ 1/160 << 16		

		.end

 