@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	C Function:		void DrawPolyList32(polygon_type *pPolyList, u32 numPolys, u32 video)
@
@	Description:	Draws a list of polgons to video. A polygon can be either a:
@						Texture mapped (256x256)
@						Solid color filled
@						3D sprite (billboard)
@
@	Comment:		ATM only solid color filled polygons are drawn inline, need to 
@						to fix this so 3d sprites and texture mapped polygons are also 
@						drawn in line. Could save a few cycles per polygon once done.
@						(less register swapping, branching, stack manipulation)
@								
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@
@	DEFINES			@
@@@@@@@@@@@@@@@@@@@@@	

	.section	.iwram,"ax",%progbits
	.ARM
    .ALIGN		
	.EXTERN VideoBuffer
	.EXTERN Draw_Poly_Span_Textured
	.EXTERN Draw_Textured_Triangle
	.EXTERN tritmap
	.EXTERN RectStretch32
	.EXTERN Sprite3D
	.EXTERN Draw_Textured_Triangle_ARM
	.EXTERN Draw_Triangle_Texture_ARM
    .GLOBAL DrawPolyList32
	
DrawPolyList32:        

@--------------------------------
@ r0 = pPolyList
@ r1 = numPolys
@ r2 = video
@--------------------------------

	stmfd   sp!,{r4-r12,lr}		@ save stack 
	
	mov		r7, r2
	mov		r8, r1
	mov		r9, r0

@--------------------------------
@ r9 = pPolyList
@ r8 = numPolys
@ r7 = video
@--------------------------------		 
numPolysLoop:
		
	ldr		r10, [r9], #4		@ derefence polygon_list_type (pPolyList)
	
 	ldrsh	r6, [r10, #16]		@ get isTextured  
	cmp		r6, #1
	beq		drawTextured 

	cmp		r6, #-1
	beq		drawSprite

	ldmia	r10, {r0,r2,r4}		@ dereference pPolyList->polygon[i].vertex[x]
	
	@ldmia	r0, {r0,r1}			@ get sx,sy (pPolyList->polygon[i].vertex[0]->)
	ldrsh	r1,[r0,#20]
	ldrsh	r0,[r0,#18]
	
	@ldmia	r2, {r2,r3}			@ get sx,sy (pPolyList->polygon[i].vertex[1]->)
	ldrsh	r3,[r2,#20]
	ldrsh	r2,[r2,#18]
	
	@ldmia	r4, {r4,r5} 		@ get sx,sy (pPolyList->polygon[i].vertex[2]->)
	ldrsh	r5,[r4,#20]
	ldrsh	r4,[r4,#18] 

	ldrh	r6, [r10, #12]		@ get polygon color 


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Start Drawing Filled Polygon
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	 
 
@--------------------------------
@ save some registers
@-------------------------------- 	
	stmfd	sp!,{r7-r9,lr}

@--------------------------------
@ r4 = x2
@ r5 = y2
@ r6 = color
@ r7 = video
@--------------------------------	

@--------------------------------
@ preset color word
@--------------------------------
	orr		r6,r6,r6,lsl#8
@--------------------------------
@ sort by y
@-------------------------------- 
	cmp		r1,r3				@ y0>y1 ?
	blt		y0y2
	eor		r0,r0,r2			@ swap x
	eor		r2,r2,r0
	eor		r0,r0,r2
	
	eor		r1,r1,r3			@ swap y
	eor		r3,r3,r1	
	eor		r1,r1,r3

y0y2:	
	cmp		r1,r5				@ y0>y2 ?
	blt		y1y2
	eor		r0,r0,r4			@ swap x
	eor		r4,r4,r0
	eor		r0,r0,r4
	
	eor		r1,r1,r5			@ swap y
	eor		r5,r5,r1	
	eor		r1,r1,r5
y1y2:	
	cmp		r3,r5				@ y2>y3 ?
	blt		ysorted
	eor		r2,r2,r4			@ swap x
	eor		r4,r4,r2
	eor		r2,r2,r4
	
	eor		r3,r3,r5			@ swap y
	eor		r5,r5,r3	
	eor		r3,r3,r5

@--------------------------------
@ needed fixes for x0 and x1
@--------------------------------
ysorted:
	cmp		r1,r3
	bne		nox0x1
	cmp		r0,r2
	eorgt	r0,r0,r2
	eorgt	r2,r2,r0
	eorgt	r0,r0,r2

nox0x1:
@--------------------------------
@ calculate delta x
@--------------------------------
	
	sub		r8,r2,r0			@ delta x B-A	( upper tri height)
	sub		r9,r4,r2			@ delta x C-B	( lower tri height)
	sub		r10,r4,r0			@ delta x C-A	( whole tri height)
@--------------------------------
@ setup video y position
@--------------------------------
	mov		lr,r1,lsl#8			@ lr = y * 256
	sub		lr,lr,r1,lsl#4		@ lr = ((y*256)-(y*16)) = y * 240
	add		r7,r7,lr			@ first y scanline
@--------------------------------
@ calculate delta y
@--------------------------------	
	sub		r11,r3,r1			@ delta y B-A
	sub		lr,r5,r1			@ delta y C-A
	sub		r12,r5,r3			@ delta y C-B
@--------------------------------
@ load divs value
@--------------------------------
	ldr		r1,=PrjTable
		
	ldr		r4,[r1,r11,lsl #2]	@ 1 / delta y B-A
	
	ldr		r5,[r1,lr,lsl#2]	@ 1 / delta y C-A
	
	ldr		r3,[r1,r12,lsl#2]	@ 1 / delta y C-B
@--------------------------------
@ precalc slopes
@--------------------------------	
	mul		r4,r8,r4			@ dABx/dABy
	mul		r5,r10,r5			@ dACx/dACy
	mul		r3,r9,r3			@ dBCx/dBCy

@--------------------------------
@ choose kind of tri
@--------------------------------	
	subs	r8,r4,r5		
	bmi		leftside
	
	mov		r8,r3	
	mov		r9,r5
	bal		goon
leftside:	
	mov		r8,r5
	mov		r9,r3

	eor		r4,r4,r5	
	eor		r5,r5,r4
	eor		r4,r4,r5
@--------------------------------
@ initialization and drawing
@--------------------------------		
goon:
	add		r12,r12,#1
	
	mov		r1,r0,asl#16		@ start x << 16
	mov		r0,r1				@ start x << 16
	
	cmp		r11,#0				@ up tri have zero height ?
	beq		adjustdowntri		@ yes then it's only down
	
outerloop:	
	mov		r2,r0,asr#16	
	rsb		r3,r2,r1,asr#16		@ r3 = delta x , r2 = start x
		
	add		r2,r7,r2			@ begin of line

@--------------------------------
@ CLIP X MIN
@--------------------------------
	cmp		r2, r7
	sublt	r10, r7, r2
	movlt	r2, r7
	sublt	r3, r3, r10

@--------------------------------
@ CLIP X MAX	
@--------------------------------
	add		r10, r7, #239
	add		lr, r2, r3
	cmp		lr, r10
	subgt	lr, lr, r10
	subgt	r3, r3, lr
	cmp		r3, #0	
	ble		nextscanline
	
	ldr		lr, =VideoBuffer
	ldr		lr, [lr]

@--------------------------------
@ CLIP Y MIN
@--------------------------------
	cmp		r2, lr	
	blt		nextscanline

@--------------------------------
@ CLIP Y MAX
@--------------------------------
	add		lr, lr, #0x9600
	cmp		r2, lr
	ldmgtfd	sp!,{r7-r9,lr}
	bgt		nofill
	

@--------------------------------
@ tricky startpixel
@--------------------------------
	tst		r2,#1
	addeq	r3,r3,#1
	beq		alignedwrites
	
	bic		r2,r2,#1
	ldrh	r10,[r2]
	bic		r10,r10,#0xff00
	orr		r10,r10,r6,lsl#8
	strh	r10,[r2],#2
	
@--------------------------------
alignedwrites:
	movs 	r3,r3,lsr#1
	rsb		r3,r3,#119
	
	add		pc,pc,r3,lsl#2
	
	.REPT	120
	strh	r6,[r2],#2
	.ENDR
	
	bcc		nextscanline
 
 	ldrh	r10,[r2]
	bic		r10,r10,#0x00ff
	orr		r10,r10,r6,lsr#8
	strh	r10,[r2]
	
nextscanline:
	add		r7,r7,#240			@ nextline
	add		r0,r0,r5
adjustdowntri:
	add		r1,r1,r4
	subs	r11,r11,#1
	bgt		outerloop

	cmp		r0,r1
	eorgt	r0,r0,r1
	eorgt	r1,r1,r0
	eorgt	r0,r0,r1

	mov		r4,r8
	mov		r5,r9
	mov		r11,r12
	mov		r12,#-1
	
	subs	r11,r11,#1 
	bge		outerloop

 	ldmfd	sp!,{r7-r9,lr}		

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ End Drawing Filled Polygon
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

nofill:
@--------------------------------
@ Loop number of polygons
@--------------------------------
	subs	r8, r8, #1
	bgt		numPolysLoop 

	ldmfd   sp!,{r4-r12,lr}		@ restore stack 

@--------------------------------
@ EXIT
@--------------------------------
	mov		pc, lr  

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Start Drawing Textured Polygon
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

drawTextured:

@--------------------------------
@ r9 = pPolyList
@ r8 = numPolys
@ r7 = video
@ r10 = polygon = free
@--------------------------------
	ldmia	r10, {r0,r2,r4}		@ dereference pPolyList->polygon[i].vertex[x]

	ldrsh	r1,[r0,#20]			@ get sx,sy (pPolyList->polygon[i].vertex[0]->)
	ldrsh	r0,[r0,#18]	
	
	ldrsh	r3,[r2,#20]			@ get sx,sy (pPolyList->polygon[i].vertex[1]->)
	ldrsh	r2,[r2,#18]	
	
	ldrsh	r5,[r4,#20]			@ get sx,sy (pPolyList->polygon[i].vertex[2]->)
	ldrsh	r4,[r4,#18]
	 
@--------------------------------
@ r0 = v0->x
@ r1 = v0->y
@ r2 = v1->x
@ r3 = v1->y
@ r4 = v2->x
@ r5 = v2->y
@--------------------------------
	add		r10, r10, #18
	
	ldrsh	r6,[r10],#2			@ Get uv's
	ldrsh	r11,[r10],#2
	ldrsh	r12,[r10],#2
	ldrsh	lr,[r10],#2
	
@--------------------------------
@ Push polygon onto the stack
@--------------------------------
	stmfd	sp!, {r0,r1,r6,r11} 
	mov		r1, sp

	stmfd	sp!, {r2,r3,r12,lr} 
	mov		r2, sp 
		
	ldrsh	r6,[r10],#2
	ldrsh	r11,[r10],#2
	stmfd	sp!, {r4,r5,r6,r11}  
	mov		r3, sp 
	
	mov		r0, r7 

@--------------------------------	  
@ Branch to tmapper32 function
@ tmapper32( vertexTMap *pv1,vertexTMap *pv2,vertexTMap *pv3 u16 *screen )
@--------------------------------
	bl		tmapper32

@--------------------------------
@ Pop polygon off the stack
@ Instead just change stack pointer
@--------------------------------
	add		sp, sp, #48
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ End Drawing Textured Polygon
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@--------------------------------
@ Loop number of polygons
@--------------------------------
	subs	r8, r8, #1
	bgt		numPolysLoop

	ldmfd   sp!,{r4-r12,lr}		@ restore stack 

@--------------------------------
@ EXIT
@--------------------------------
	mov		pc, lr

 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Start Drawing Sprite Polygon
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
drawSprite:

	mov		r0, r10
	mov		r1, r7

@--------------------------------
@ Branch to draw sprite function
@ void RectStretch( polygon_type *poly, u16 *screen )
@--------------------------------
	bl		RectStretch32  

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ End Drawing Sprite Polygon
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@--------------------------------
@ Loop number of polygons
@--------------------------------
	subs	r8, r8, #1
	bgt		numPolysLoop

	ldmfd   sp!,{r4-r12,lr}		@ restore stack 

@--------------------------------
@ EXIT
@--------------------------------
	mov		pc, lr 

	.pool

	.END
	  
	
	

