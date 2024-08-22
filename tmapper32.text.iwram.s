@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void tmapper32( vertexTMap *pv1,vertexTMap *pv2,vertexTMap *pv3 u16 *screen )
@	void outtertmapper32(s32 y1,u8 *levelTmap, u32 uv, u32 dudv, s32 y2, u32 xsxe, u32 deltalr, u32 deltat, u32 *dest)
@	NOTEs:
@
@
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@
@	DEFINES			@
@@@@@@@@@@@@@@@@@@@@@	
XCLIPMIN			=0
XCLIPMAX			=239
YCLIPMIN			=0
YCLIPMAX			=159

@@@@@@@@@@@@@
@ sp offsets
@@@@@@@@@@@@@


	.section	.iwram,"ax",%progbits  
	.ARM
    .ALIGN 4		
	.GLOBAL tmapper32 
	.GLOBAL outtertmapper32
	
	.GLOBAL inner2	
	.GLOBAL inner3 	
	.EXTERN VideoBuffer 
	.EXTERN	PrjTable
	.EXTERN levelTmap
	.EXTERN Font_itoa
	.EXTERN Gba_Print
	.EXTERN VideoBuffer
	.EXTERN szp
	
    
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	Tirangle Texture Mapper 
@		void tmapper32( u32 *screen, vertexTMap *pv1,vertexTMap *pv2,vertexTMap *pv3 )
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
tmapper32:   
@r0=screen
@r1=pv1
@r2=pv2
@r3=pv3
     
	stmfd sp!, {r4-r12,lr}
	
@-------------------------------------
@	get xy's
@-------------------------------------
	ldmia	r1,{r4,r5}
	ldmia	r2,{r6,r7}
	ldmia	r3,{r8,r9}

@r4 = pv1->x
@r5 = pv1->y
 
@r6 = pv2->x
@r7 = pv2->y

@r8 = pv3->x
@r9 = pv3->y

@-------------------------------------
@	make sure vertices are in order
@-------------------------------------
	@ pv2->y < pv1->y ? 
	cmp		r7, r5
	bge		y3y1

	@ swap pv2 pv1
	eor	r1,r1,r2	
	eor	r2,r2,r1
	eor	r1,r1,r2
	ldmia	r1,{r4,r5}
	ldmia	r2,{r6,r7}

y3y1:
	@pv3->y < pv1->y ?
	cmp		r9, r5
	bge		y3y2

	@ swap pv3 pv1
	eor	r1,r1,r3	
	eor	r3,r3,r1
	eor	r1,r1,r3
	ldmia	r1,{r4,r5}
	ldmia	r3,{r8,r9}

y3y2:	
	@pv3->y < pv2->y ?
	cmp		r9, r7
	bge		noyswap

	@ swap pv3 pv2
	eor	r2,r2,r3	
	eor	r3,r3,r2
	eor	r2,r2,r3
	ldmia	r2,{r6,r7}     
	ldmia	r3,{r8,r9}

noyswap: 
 
	ldr		lr, =PrjTable  

@-------------------------------------
@	test if triangle need to be x clipped
@------------------------------------- 
@r1 = pv1
@r2 = pv2
@r3 = pv3
@r4 = x1
@r5 = y1
@r6 = x2
@r7 = y2
@r8 = x3
@r9 = y3

@	b		xcliptri

	cmp		r4, #0
	blt		xcliptri
	cmp		r6, #0
	blt		xcliptri
	cmp		r8, #0
	blt		xcliptri
	
	cmp		r4, #XCLIPMAX
	bgt		xcliptri
	cmp		r6, #XCLIPMAX
	bgt		xcliptri
	cmp		r8, #XCLIPMAX
	bgt		xcliptri

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	NON X CLIPPED TRI
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@-------------------------------------
@	compute pixel incremental ratio 
@------------------------------------- 
@r1 = pv1
@r2 = pv2
@r3 = pv3
@r4 = x1
@r5 = y1
@r6 = x2
@r7 = y2
@r8 = x3
@r9 = y3
		
	@ y2miny1 = y2-y1;	
	sub		r10, r7, r5	

@r10 = y2miny1
		
	@ divtmp = div_table[(y3-y1)];	
	sub		r11, r9, r5
	ldr		r11, [lr, r11, lsl #2]

@r11 = divtmp

	mul		r10, r11, r10

@r10 = y2miny1*divtmp

	@ right_x = ((x1<<16)+(y2miny1*(x3-x1)*divtmp)); 
	sub		r12, r8, r4			@ (x3-x1)	
	mov		r4, r4, asl #16		@ save x1<<16 as from now on thats how we treat it
	mla		r12, r10, r12, r4	@ ((x1<<16)+(y2miny1*(x3-x1)*divtmp))

@r12 = right_x
@r4 = x1<<16



	@ left_x = x2<<16; 
	mov		r6, r6, asl #16

@r6 = left_x

@-------------------------------------
@ compute texture incremental ratio
@-------------------------------------
@r1 = pv1 = can be free
@r2 = pv2 = can be free
@r3 = pv3 = KEEP
@r4 = x1<<16 = xs,xe 
@r5 = y1 = KEEP
@r6 = left_x
@r7 = y2 = KEEP
@r8 = x3 = free
@r9 = y3 = free
@r10 = y2miny1 * divtmp
@r11 = divtmp = free  
@r12 = right_x  
  	
	add		r1, r1, #8
	ldmia	r1, {r1,r11}

	add		r3, r3, #8
	ldmia	r3, {r8,r9}	
	
@r1 = tx1
@r11 = ty1
@r8 = tx3
@r9 = ty3
		  
	@right_tx = ((tx1<<16) + (y2miny1*(tx3-tx1)*divtmp)); 
	sub		r8, r8, r1
	mov		r1, r1, asl #16

	@ Save tx1
	stmfd	sp!,{r1}
	mla		r1, r8, r10, r1

@r1	= right_tx

	@left_tx = tx2<<16;
	@left_ty = ty2<<16;
	add		r2, r2, #8
	ldmia	r2, {r2, r8}

	mov		r2, r2, asl #16
	mov		r8, r8, asl #16

@r2 = left_tx
@r8 = left_ty

	@right_ty = ((ty1<<16) + (y2miny1*(ty3-ty1)*divtmp)); 
	sub		r9, r9, r11
	mov		r11, r11, asl #16
	@ Save ty1
	stmfd	sp!,{r11}
	mla		r11, r9, r10, r11
 
@r11 = right_ty


@r1 = right_tx
@r2 = left_tx
@r3 = pv3 + 8 = KEEP
@r4 = x1<<16 = xs,xe 
@r5 = y1 = KEEP
@r6 = left_x
@r7 = y2 = KEEP
@r8 = left_ty
@r9 = free
@r10 = y2miny1 * divtmp
@r11 = right_ty
@r12 = right_x  

	@if (right_x < left_x) messes up if right is on left 
	@cmp		r12, r6
	subs	r9, r12, r6
	bge		nomess
	
	@ Swap Em

	@ right_x = left_x
	eor	r12,r12,r6	
	eor	r6,r6,r12
	eor	r12,r12,r6

	@ right_tx = left_tx
	eor	r1,r1,r2	
	eor	r2,r2,r1
	eor	r1,r1,r2

	@ right_ty = left_ty;
	eor	r11,r11,r8	
	eor	r8,r8,r11
	eor	r11,r11,r8
			
	sub	r9, r12, r6
nomess:

@r1 = right_tx
@r2 = left_tx
@r3 = pv3 + 8 = KEEP
@r4 = x1<<16 = xs,xe 
@r5 = y1 = KEEP
@r6 = left_x
@r7 = y2 = KEEP
@r8 = left_ty
@r9 = right_x - left_x
@r10 = y2miny1 * divtmp = free
@r11 = right_ty
@r12 = right_x  

	@divtmp = div_table[((right_x - left_x)>>16)];
	
	@????????
	mov		r9, r9, asr #16
	ldr		r9, [lr, r9, lsl #2]
		
@r9 = divtmp

	@constant_ty = (((right_ty - left_ty)>>10) * divtmp)>>6;  	
	sub		r11, r11, r8
	mov		r11, r11, asr #10
	mul		r11, r9, r11
	@mov		r11, r11, asr #14 @#6
 
@r11 = constant_ty

	@constant_tx = (((right_tx - left_tx)>>10) * divtmp)>>6;  
	sub		r1, r1, r2
	mov		r1, r1, asr #10
	mul		r1, r9, r1
	mov		r1, r1, asr #14 @#6
	
@r1 = constant_tx


	@dudv = ((constant_tx>>8)<<16) | ((constant_ty>>8) & 0xffff);
	@ldr		r9, =0xffff
	mov		r9, #0xff00
	orr		r9, r9, #0x00ff
	and		r11, r9, r11, asr #14
	orr		r1, r11, r1, asl #16

@r1 = dudv	

	

@^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
@ atm treating left_x,eight_x left_ty, right_ty and PV3!
@ as can be free.. but when coding bottom i will
@ need to stack these value or figure out a better way
@^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	@ save vars for bottom
	stmfd	sp!,{r0,r1,r2,r3,r6,r7,r8,r12}
	

@r1 = dudv
@r2 = left_tx
@r3 = pv3 + 8 = KEEP
@r4 = x1<<16 = xs,xe 
@r5 = y1 = KEEP
@r6 = left_x
@r7 = y2 = KEEP
@r8 = left_ty
@r9 = free
@r10 = y2miny1 * divtmp = free
@r11 = free
@r12 = right_x 
 
@-------------------------------------
@ draw the no clip triangle top 
@ if need too
@-------------------------------------
	@if ((height = y2 - y1) != 0) 
	subs	r9, r7, r5
	beq		noclipbottom

@r9 = height

	@divtmp = div_table[height];
	ldr		r9, [lr, r9, lsl #2]

@r9 = divtmp

	@ compute edge ratios 				

	@dx_left = (((left_x - xs)>>10) * divtmp)>>6; 
	sub		r6, r6, r4
	mov		r6, r6, asr #10
	mul		r6, r9, r6
	mov		r6, r6, asr #6

@r6 = dx_left

	@dx_right = (((right_x - xs)>>10) * divtmp)>>6; 
	sub		r12, r12, r4
	mov		r12, r12, asr #10
	mul		r12, r9, r12
	mov		r12, r12, asr #6

@r12 = dx_right

	@txs = tx1<<16; 
	@tys = ty1<<16;	
	add		r10, sp, #32
	ldmfd r10, {r3,r10}
	@sub		sp, sp, #32

@r3 = tys
@r10 = txs

	@tx_left = (((left_tx - txs)>>10) * divtmp)>>6; 				
	sub		r2, r2, r10
	mov		r2, r2, asr #10
	mul		r2, r9, r2
	mov		r2, r2, asr #6
	
@r2 = tx_left

	@ty_left = (((left_ty - tys)>>10) * divtmp)>>6; 
	sub		r8, r8, r3
	mov		r8, r8, asr #10
	mul		r8, r9, r8
	mov		r8, r8, asr #6

@r8 = ty_left

@r1 = dudv
@r2 = tx_left
@r3 = tys
@r4 = x1<<16 = xs,xe 
@r5 = y1 = KEEP
@r6 = dx_left
@r7 = y2 = KEEP
@r8 = ty_left
@r9 = divtmp
@r10 = txs
@r11 = free
@r12 = dx_right 

	mov		r11, #0xff00
	orr		r11, r11, #0x00ff

@ compute starting points for u,v interpolants								
	@u32 deltat = ((tx_left>>8)<<16) | ((ty_left>>8) & 0xffff);
	mov		r2, r2, asr #8
	and		r8, r11, r8, asr #8
	orr		r8, r8, r2, asl #16

@r8 = deltat

	@uv = ((txs>>8)<<16) | ((tys>>8) & 0xffff);					
	mov		r10, r10, asr #8	
	and		r3, r11, r3, asr #8
	orr		r2, r3, r10, asl #16
	
@r2 = uv
			
	@u32 xsxe = ((xs>>8)<<16) | (((xe+0xffff)>>8) & 0xffff);
	mov		r10, r4, asr #8	
	add		r4, r4, r11	
	and		r4, r11, r4, asr #8
	orr		r10, r4, r10, asl #16

@r10 = xsxe

	@u32 deltalr = ((dx_left>>8)<<16) | ((dx_right>>8) & 0xffff); 
	mov		r6, r6, asr #8	
	and		r12, r11, r12, asr #8
	orr		r12, r12, r6, asl #16

@r12 = deltalr

@r1 = dudv
@r2 = uv
@r3 = free
@r4 = x1<<16 = xs,xe = free
@r5 = y1 = KEEP
@r6 = free
@r7 = y2 = KEEP
@r8 = deltat
@r9 = free
@r10 = xsxe
@r11 = free
@r12 = deltalr

	@ clip ymax
	@if (y2 > 160)
	@	y2 = 160;
	cmp		r7, #YCLIPMAX
	movgt	r7, #YCLIPMAX

	@ clip ymin
	@if (y1 < 0)
	cmp		r5, #0
	bge		noycliptop	

	@s32 dy = ABS(0-y1);
	rsb		r11, r5, #0
	cmp		r11, #0
	sublt	r11, r11, r11, lsl #1	

	@xsxe = xsxe+(deltalr*dy);
	mla		r10, r12, r11, r10

	@uv = uv+(deltat*dy);
	mla		r2, r8, r11, r2
				
	@y1 = 0;
	mov		r5, #0
	
noycliptop:

	@outtertmapper32(dest,levelTmap,uv,dudv, y2,xsxe, deltalr, deltat, y1);

	@if (y1 >= y2)nodraw
	cmp		r5, r7
	bge		noclipbottom
			
	mov		r11, r8
	
	mov		r9, r7
	mov		r7, r11

	mov		r6, r12

	mov		r8, r5
	mov		r5, r10

	mov		r4, r9

	mov		r3, r1

	ldr		r1, =levelTmap
		
	stmfd sp!, {lr}	
	bl		outtertmapper32
	ldmfd sp!, {lr}
	

@-------------------------------------
@ draw the no clip triangle bottom 
@ if need too
@-------------------------------------
noclipbottom:

	
	@load vars saved at top
	ldmfd	sp!,{r0,r1,r2,r3,r6,r7,r8,r12}
	
	
	@if ((height = y3 - y2) != 0) 
	sub		r3, r3, #8
	ldmia	r3, {r4,r5,r9,r10}
	
	subs	r11, r5, r7	
	beq		donetmapper32

@r1 = dudv
@r2 = left_tx
@r3 = pv3 + 8 = KEEP
@r4 = x3
@r5 = y3
@r6 = left_x
@r7 = y2 = KEEP
@r8 = left_ty
@r9 = tx3
@r10 = ty3
@r11 = height
@r12 = right_x 

@-------------------------------------
@ now recompute slope of shorter edge 
@ to finish triangle bottom 
@-------------------------------------

	@divtmp = PrjTable[height];
	ldr		r11, [lr, r11, lsl #2]

@r11 = divtmp

	@dx_right = ((((x3<<16) - right_x)>>10) * divtmp)>>6;  
	rsb		r3, r12, r4, asl #16
	mov		r3, r3, asr #10
	mul		r3, r11, r3
	mov		r3, r3, asr #6

@r3 = dx_right

	@dx_left = ((((x3<<16) - left_x)>>10) * divtmp)>>6; 
	rsb		r4, r6, r4, asl #16
	mov		r4, r4, asr #10
	mul		r4, r11, r4
	mov		r4, r4, asr #6

@r4 = dx_left

	@xs = left_x;
	@xe = right_x; 

@r6 = xs
@r12 = xe

	@tx_left = ((((tx3<<16) - left_tx)>>10) * divtmp)>>6; 
	rsb		r9, r2, r9, asl #16
	mov		r9, r9, asr #10
	mul		r9, r11, r9
	mov		r9, r9, asr #6

@r9 = tx_left

	@ty_left = ((((ty3<<16) - left_ty)>>10) * divtmp)>>6; 
	rsb		r10, r8, r10, asl #16
	mov		r10, r10, asr #10
	mul		r10, r11, r10
	mov		r10, r10, asr #6

@r10 = ty_left

	@txs = left_tx; 
	@tys = left_ty;

@r2 = txs
@r8 = tys

	mov		r11, #0xff00
	orr		r11, r11, #0x00ff

	@uv = ((txs>>8)<<16) | ((tys>>8) & 0xffff);	
	mov		r2, r2, asr #8		
	and		r8, r11, r8, asr #8
	orr		r2, r8, r2, asl #16

@r2 = uv
					
	@u32 deltat = ((tx_left>>8)<<16) | ((ty_left>>8) & 0xffff);
	mov		r9, r9, asr #8
	and		r10, r11, r10, asr #8
	orr		r9, r10, r9, asl #16
			
@r9 = deltat

	@u32 xsxe = ((xs>>8)<<16) | (((xe+0xffff)>>8) & 0xffff);
	mov		r6, r6, asr #8
	add		r12, r12, r11
	and		r12, r11, r12, asr #8
	orr		r6, r12, r6, asl #16

@r6 = xsxe

	@u32 deltalr = ((dx_left>>8)<<16) | ((dx_right>>8) & 0xffff);
	mov		r4, r4, asr #8
	and		r3, r11, r3, asr #8
	orr		r4, r3, r4, asl #16

@r4 = deltalr

@r1 = dudv
@r2 = uv
@r3 = free
@r4 = deltalr
@r5 = y3
@r6 = xsxe
@r7 = y2 = KEEP
@r8 = free
@r9 = deltat
@r10 = free
@r11 = free
@r12 = free 

	@ clip ymax
	@if (y3 > 160)
	@	y3 = 160;
	cmp		r5, #YCLIPMAX
	movgt	r5, #YCLIPMAX

	@ clip ymin
	@if (y2 < 0)
	cmp		r7, #0
	bge		noyclipbottom

	@s32 dy = ABS(0-y2);
	rsb		r11, r7, #0
	cmp		r11, #0
	sublt	r11, r11, r11, lsl #1	

	@xsxe += deltalr*dy;
	mla		r6, r4, r11, r6

	@uv += deltat*dy;
	mla		r2, r9, r11, r2

	@y2 = 0;
	mov		r7, #0
	
noyclipbottom:

	@outtertmapper32(dest,levelTmap,uv,dudv, y3,xsxe, deltalr, deltat, y2);
	@mov		r8, r0
	
	@if (y2 >= y3)nodraw
	cmp		r7, r5
	bge		donetmapper32

	mov		r8, r7

	mov		r3, r1
	ldr		r1, =levelTmap

	mov		r12, r4
	mov		r4, r5
	mov		r5, r6
	mov		r6, r12
	mov		r7, r9

		
	@stmfd sp!, {lr}
	bl		outtertmapper32
	@ldmfd sp!, {lr}
	
donetmapper32:

	add		sp, sp, #8
	ldmfd sp!, {r4-r12,lr}
	@mov		pc, lr
	bx		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	X CLIPPED TRI
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
xcliptri:

@-------------------------------------
@	compute pixel incremental ratio 
@------------------------------------- 
@r1 = pv1
@r2 = pv2
@r3 = pv3
@r4 = x1
@r5 = y1
@r6 = x2
@r7 = y2
@r8 = x3
@r9 = y3
		
	@ y2miny1 = y2-y1;	
	sub		r10, r7, r5	

@r10 = y2miny1
		
	@ divtmp = div_table[(y3-y1)];	
	sub		r11, r9, r5
	ldr		r11, [lr, r11, lsl #2]

@r11 = divtmp

	mul		r10, r11, r10

@r10 = y2miny1*divtmp

	@ right_x = ((x1<<16)+(y2miny1*(x3-x1)*divtmp)); 
	sub		r12, r8, r4			@ (x3-x1)	
	mov		r4, r4, asl #16		@ save x1<<16 as from now on thats how we treat it
	mla		r12, r10, r12, r4	@ ((x1<<16)+(y2miny1*(x3-x1)*divtmp))

@r12 = right_x
@r4 = x1<<16

	@ left_x = x2<<16; 
	mov		r6, r6, asl #16

@r6 = left_x

@-------------------------------------
@ compute texture incremental ratio
@-------------------------------------
@r1 = pv1 = can be free
@r2 = pv2 = can be free
@r3 = pv3 = KEEP
@r4 = x1<<16 = xs,xe 
@r5 = y1 = KEEP
@r6 = left_x
@r7 = y2 = KEEP
@r8 = x3 = free
@r9 = y3 = free
@r10 = y2miny1 * divtmp
@r11 = divtmp = free  
@r12 = right_x  
  	
	add		r1, r1, #8
	ldmia	r1, {r1,r11}

	add		r3, r3, #8
	ldmia	r3, {r8,r9}	
	
@r1 = tx1
@r11 = ty1
@r8 = tx3
@r9 = ty3
		  
	@right_tx = ((tx1<<16) + (y2miny1*(tx3-tx1)*divtmp)); 
	sub		r8, r8, r1
	mov		r1, r1, asl #16
	@ Save tx1
	stmfd	sp!,{r1}
	mla		r1, r8, r10, r1

@r1	= right_tx

	@left_tx = tx2<<16;
	@left_ty = ty2<<16;
	add		r2, r2, #8
	ldmia	r2, {r2, r8}

	mov		r2, r2, asl #16
	mov		r8, r8, asl #16

@r2 = left_tx
@r8 = left_ty

	@right_ty = ((ty1<<16) + (y2miny1*(ty3-ty1)*divtmp)); 
	sub		r9, r9, r11
	mov		r11, r11, asl #16
	@ Save ty1
	stmfd	sp!,{r11}
	mla		r11, r9, r10, r11

@r11 = right_ty

@r1 = right_tx
@r2 = left_tx
@r3 = pv3 + 8 = KEEP
@r4 = x1<<16 = xs,xe 
@r5 = y1 = KEEP
@r6 = left_x
@r7 = y2 = KEEP
@r8 = left_ty
@r9 = free
@r10 = y2miny1 * divtmp
@r11 = right_ty
@r12 = right_x  

	@if (right_x < left_x) messes up if right is on left 
	@cmp		r12, r6
	subs	r9, r12, r6
	bge		nomessxclip
	
	@ Swap Em

	@ right_x = left_x
	eor	r12,r12,r6	
	eor	r6,r6,r12
	eor	r12,r12,r6

	@ right_tx = left_tx
	eor	r1,r1,r2	
	eor	r2,r2,r1
	eor	r1,r1,r2

	@ right_ty = left_ty;
	eor	r11,r11,r8	
	eor	r8,r8,r11
	eor	r11,r11,r8
			
	sub	r9, r12, r6
nomessxclip:

@r1 = right_tx
@r2 = left_tx
@r3 = pv3 + 8 = KEEP
@r4 = x1<<16 = xs,xe 
@r5 = y1 = KEEP
@r6 = left_x
@r7 = y2 = KEEP
@r8 = left_ty
@r9 = right_x - left_x
@r10 = y2miny1 * divtmp = free
@r11 = right_ty
@r12 = right_x  

	@divtmp = div_table[((right_x - left_x)>>16)];
	
	@????????
	mov		r9, r9, asr #16
	ldr		r9, [lr, r9, lsl #2]
		
@r9 = divtmp

	@constant_ty = (((right_ty - left_ty)>>10) * divtmp)>>6;  	
	sub		r11, r11, r8
	mov		r11, r11, asr #10
	mul		r11, r9, r11
	@mov		r11, r11, asr #14 @#6
 
@r11 = constant_ty

	@constant_tx = (((right_tx - left_tx)>>10) * divtmp)>>6;  
	sub		r1, r1, r2
	mov		r1, r1, asr #10
	mul		r1, r9, r1
	mov		r1, r1, asr #14 @#6
	
@r1 = constant_tx

	@dudv = ((constant_tx>>8)<<16) | ((constant_ty>>8) & 0xffff);
	@ldr		r9, =0xffff
	mov		r9, #0xff00
	orr		r9, r9, #0x00ff
	and		r11, r9, r11, asr #14
	orr		r1, r11, r1, asl #16

@r1 = dudv	


@^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
@ atm treating left_x,eight_x left_ty, right_ty and PV3!
@ as can be free.. but when coding bottom i will
@ need to stack these value or figure out a better way
@^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
	@ save vars for bottom
	stmfd	sp!,{r0-r12,lr}@r0,r1,r2,r3,r6,r7,r8,r12}
	

@r1 = dudv
@r2 = left_tx
@r3 = pv3 + 8 = KEEP
@r4 = x1<<16 = xs,xe 
@r5 = y1 = KEEP
@r6 = left_x
@r7 = y2 = KEEP
@r8 = left_ty
@r9 = free
@r10 = y2miny1 * divtmp = free
@r11 = free
@r12 = right_x 
 
@-------------------------------------
@ draw the x clip triangle top 
@ if need too
@-------------------------------------
	@if ((height = y2 - y1) != 0) 
	subs	r9, r7, r5
	beq		bottomxclip

@r9 = height

	@divtmp = div_table[height];
	ldr		r9, [lr, r9, lsl #2]

@r9 = divtmp

	@ compute edge ratios 				

	@dx_left = (((left_x - xs)>>10) * divtmp)>>6; 
	sub		r6, r6, r4
	mov		r6, r6, asr #10
	mul		r6, r9, r6
	mov		r6, r6, asr #6

@r6 = dx_left


	@dx_right = (((right_x - xs)>>10) * divtmp)>>6; 
	sub		r12, r12, r4
	mov		r12, r12, asr #10
	mul		r12, r9, r12
	mov		r12, r12, asr #6

@r12 = dx_right

 
	@txs = tx1<<16; 
	@tys = ty1<<16;	
	add		r10, sp, #56 @#32
	ldmfd r10, {r3,r10}
	@sub		sp, sp, #32 

@r3 = tys
@r10 = txs



	@tx_left = (((left_tx - txs)>>10) * divtmp)>>6; 				
	sub		r2, r2, r10
	mov		r2, r2, asr #10
	mul		r2, r9, r2
	mov		r2, r2, asr #6
	
@r2 = tx_left

	@ty_left = (((left_ty - tys)>>10) * divtmp)>>6; 
	sub		r8, r8, r3
	mov		r8, r8, asr #10
	mul		r8, r9, r8
	mov		r8, r8, asr #6

@r8 = ty_left

@r1 = dudv
@r2 = tx_left
@r3 = tys
@r4 = x1<<16 = xs,xe 
@r5 = y1 = KEEP
@r6 = dx_left
@r7 = y2 = KEEP
@r8 = ty_left
@r9 = divtmp
@r10 = txs
@r11 = free
@r12 = dx_right 

	mov		r9, r4
@r9 = xe

	@ clip ymax
	@if (y2 > 160)
	@	y2 = 160;
	cmp		r7, #YCLIPMAX
	movgt	r7, #YCLIPMAX

	@ clip ymin
	@if (y1 < 0)
	cmp		r5, #0
	bge		noycliptopxclip

	@s32 dy = ABS(0-y1);
	rsb		r11, r5, #0
	cmp		r11, #0
	sublt	r11, r11, r11, lsl #1	

@r11 = dy
	

	@xs += (dx_left*dy); 
	mla		r4, r6, r11, r4

	@xe += (dx_right*dy); 
	mla		r9, r12, r11, r9
				
	@txs += (tx_left*dy); 
	mla		r10, r2, r11, r10

	@tys += (ty_left*dy); 
	mla		r3, r8, r11, r3
				
	@y1 = 0;
	mov		r5, #0
	
noycliptopxclip:
@r1 = dudv
@r2 = tx_left
@r3 = tys
@r4 = xs
@r5 = y1 = KEEP
@r6 = dx_left
@r7 = y2 = KEEP
@r8 = ty_left
@r9 = xe
@r10 = txs
@r11 = free
@r12 = dx_right 
@stmfd	sp!,{r0,r1,r2,r3,r6,r7,r8,r12}

@-------------------------------------
@ draw the X clip triangle top
@ if need too
@-------------------------------------
	
	cmp		r5, r7
	bge		bottomxclip

y1y2xcliploop:	@for (y = y1; y < y2; y++)
		
	@xstart = xs>>16;				
	@xend = ceil2(xe);	@x += 0xffff;
						@return x >> 16;
	
	mov		lr, #0xff00
	orr		lr, lr, #0x00ff

	@compute starting points for u,v interpolants								
	@uv = ((txs>>8)<<16) | ((tys>>8) & 0xffff);			
	and		r11, lr, r3, asr#8
	mov		lr, r10, asr #8
	orr		r11, r11, lr, asl #16			
	
@r11=uv

	stmfd	sp!, {r2,r4}
	mov		r2, r4, asr #16

	mov		lr, #0xff00
	orr		lr, lr, #0x00ff

	@s32 width = xend-xstart;	
	add		lr, r9, lr
	mov		lr, lr, asr #16		@xend = ceil2(xe);
	
	@test for x clipping RHS
	@if (xend > 240)
	@	xend = 240;
	cmp		lr, #XCLIPMAX
	movgt	lr, #XCLIPMAX
	
	@test for x clipping LHS
	@if (xstart < 0)
	@{
	@	compute x overlap
	@	s32 dx = 0-xstart;	
	@	xstart = 0;
	@	slide interpolants over					
	@	uv += dx*dudv;					
	@}
	cmp		r2, #0 
	bge		noxclip0	
	rsb		r4, r2, #0
	mov		r2, #0
	mla		r11, r4, r1, r11

noxclip0:

	subs	lr, lr, r2			
	ble		nodrawtop

@lr = width

	@stmfd sp!,{r0-r4,r8,r10}
	stmfd sp!,{r0-r12,lr}

	@if (width > 0)
	@{
	@	draw span		
	@	u32 screen_ptr = dest+xstart+((y<<8)-(y<<4));//y*240;
	add		r8, r0, r2
	mov		r2, r5, asl #8
	sub		r2, r2, r5, asl #4
	add		r8, r8, r2

	mov		r3, r1
	ldr		r1, =levelTmap
	mov		r2, r11
	mov		r4, lr	

@ r8 = dest 
@ r1 = bitmap
@ r2 = uv
@ r3 = dudv 
@ r4 = width 
@r10,r11,r4,r2,r8

	@stmfd sp!,{lr}
	bl	inpoo
	@ldmfd sp!,{lr}

	@	inner3(screen_ptr,levelTmap,uv,dudv,width);	
	@}

	@ldmfd sp!,{r0-r4,r8,r10}
	ldmfd sp!,{r0-r12,lr}

nodrawtop:

	ldmfd	sp!, {r2,r4}

	@adjust starting and ending point 
	@xs += dx_left; 
	add		r4, r4, r6
	@xe += dx_right; 
	add		r9, r9, r12
	
	@txs += tx_left; 
	add		r10, r10, r2

	@tys += ty_left; 				
	add		r3, r3, r8

	add		r5, r5, #1
	cmp		r5, r7
	blt		y1y2xcliploop
	

@-------------------------------------
@ draw the X clip triangle bottom 
@ if need too
@-------------------------------------
bottomxclip:

	@load vars saved at top
	ldmfd	sp!,{r0-r12,lr}@{r0,r1,r2,r3,r6,r7,r8,r12}
	
	@TEMP!!!
	@b		donetmapper32xclip

	@if ((height = y3 - y2) != 0) 
	sub		r3, r3, #8
	ldmia	r3, {r4,r5,r9,r10}
	
	subs	r11, r5, r7	
	beq		donetmapper32xclip

@r1 = dudv
@r2 = left_tx
@r3 = pv3 + 8 = KEEP
@r4 = x3
@r5 = y3
@r6 = left_x
@r7 = y2 = KEEP
@r8 = left_ty
@r9 = tx3
@r10 = ty3
@r11 = height
@r12 = right_x 



@-------------------------------------
@ now recompute slope of shorter edge 
@ to finish triangle bottom 
@-------------------------------------

	@divtmp = PrjTable[height];
	ldr		r11, [lr, r11, lsl #2]

@r11 = divtmp

	@dx_right = ((((x3<<16) - right_x)>>10) * divtmp)>>6;  
	rsb		r3, r12, r4, asl #16
	mov		r3, r3, asr #10
	mul		r3, r11, r3
	mov		r3, r3, asr #6

@r3 = dx_right

	@dx_left = ((((x3<<16) - left_x)>>10) * divtmp)>>6; 
	rsb		r4, r6, r4, asl #16
	mov		r4, r4, asr #10
	mul		r4, r11, r4
	mov		r4, r4, asr #6

@r4 = dx_left

	@xs = left_x;
	@xe = right_x; 

@r6 = xs
@r12 = xe

	@tx_left = ((((tx3<<16) - left_tx)>>10) * divtmp)>>6; 
	rsb		r9, r2, r9, asl #16
	mov		r9, r9, asr #10
	mul		r9, r11, r9
	mov		r9, r9, asr #6

@r9 = tx_left

	@ty_left = ((((ty3<<16) - left_ty)>>10) * divtmp)>>6; 
	rsb		r10, r8, r10, asl #16
	mov		r10, r10, asr #10
	mul		r10, r11, r10
	mov		r10, r10, asr #6

@r10 = ty_left

	@txs = left_tx; 
	@tys = left_ty;

@r2 = txs
@r8 = tys


@r1 = dudv
@r2 = txs
@r3 = dx_right
@r4 = dx_left
@r5 = y3
@r6 = xs
@r7 = y2 = KEEP
@r8 = tys
@r9 = tx_left
@r10 = ty_left
@r11 = free
@r12 = xe 

	@ clip ymax
	@if (y3 > 160)
	@	y3 = 160;
	cmp		r5, #YCLIPMAX
	movgt	r5, #YCLIPMAX

	@ clip ymin
	@if (y2 < 0)
	cmp		r7, #0
	bge		noyclipbottomxclip

	@s32 dy = ABS(0-y2);
	rsb		r11, r7, #0
	cmp		r11, #0
	sublt	r11, r11, r11, lsl #1	

@r11 = dy

	@xs += (dx_left*dy); 
	mla		r6, r4, r11, r6

	@xe += (dx_right*dy); 
	mla		r12, r3, r11, r12
				
	@txs += (tx_left*dy); 
	mla		r2, r9, r11, r2

	@tys += (ty_left*dy); 
	mla		r8, r10, r11, r8


	@y2 = 0;
	mov		r7, #0

noyclipbottomxclip:

@r1 = dudv
@r2 = txs
@r3 = dx_right
@r4 = dx_left
@r5 = y3
@r6 = xs
@r7 = y2 = KEEP
@r8 = tys
@r9 = tx_left
@r10 = ty_left
@r11 = free
@r12 = xe 

	cmp		r7, r5
	@blt		y2y3xcliploop
	bge		donetmapper32xclip

y2y3xcliploop:	@for (y = y2; y < y3; y++) 

	@xstart = xs>>16;				
	@xend = ceil2(xe);

	@compute starting points for u,v interpolants		
	@uv = ((txs>>8)<<16) | ((tys>>8) & 0xffff);					
	mov		lr, #0xff00
	orr		lr, lr, #0x00ff

	stmfd sp!, {r2,r4}

	and		r11, lr, r8, asr #8
	mov		r2, r2, asr #8
	orr		r11, r11, r2, asl #16

@r11 = uv		

	mov		r2, r6, asr #16

@r2 = xstart

	@test for x clipping LHS
	@if (xstart < 0)
	cmp		r2, #0
	bge		noxclipbottomxclip

	@	compute x overlap
	@	s32 dx = 0-xstart;					
	rsb		r4, r2, #0

	@	xstart = 0;					
	mov		r2, #0

	@	slide interpolants over					
	@	uv += dx*dudv;
	mla		r11,r4,r1,r11
	
noxclipbottomxclip:

	@test for x clipping RHS
	@if (xend > 240)
	@	xend = 240;
	add		lr, lr, r12
	mov		lr, lr, asr #16

	cmp		lr, #XCLIPMAX
	movgt	lr, #XCLIPMAX

	subs	r4, lr, r2
	ble		noinnerbottomxclip
@r4 = width

	stmfd sp!,{r0-r12,lr}

	@s32 width = xend-xstart;				
	@if (width > 0)
	@	draw span		
	@	u32 screen_ptr = dest+xstart+((y<<8)-(y<<4));//y*240;
	add		r8, r0, r2
	mov		lr, r7, asl #8
	sub		lr, lr, r7, asl #4
	add		r8, r8, lr

	mov		r3, r1
	ldr		r1, =levelTmap
	mov		r2, r11

@ r8 = dest 
@ r1 = bitmap
@ r2 = uv
@ r3 = dudv 
@ r4 = width 
@r10,r11,r4,r2,r8
	@	inner3(screen_ptr,levelTmap,uv,dudv,width);	
	
	bl		inpoo
	
	ldmfd sp!,{r0-r12,lr}

noinnerbottomxclip:

	ldmfd sp!, {r2,r4}

	@adjust starting and ending point 
	@xs += dx_left; 
	add		r6, r6, r4
	
	@xe += dx_right; 
	add		r12, r12, r3
				
	@txs += tx_left; 
	add		r2, r2, r9

	@tys += ty_left; 
	add		r8, r8, r10
	
	add		r7, r7, #1
	cmp		r7, r5
	blt		y2y3xcliploop

donetmapper32xclip:

	add		sp, sp, #8
	ldmfd sp!, {r4-r12,lr}
	@mov		pc, lr
	bx		lr


	.pool




@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	Tirangle Texture Mapper 
@		void outtertmapper32(u32 *dest,u8 *levelTmap, u32 uv, u32 dudv, s32 y2, u32 xsxe, u32 deltalr, u32 deltat, s32 y1)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
outtertmapper32:   
@r0=dest
@r1=levelTmap
@r2=uv
@r3=dudv 
@r4=y2
@r5=xsxe
@r6=deltalr
@r7=deltat
@r8=y1
      
	 
@	mov	r12, sp

	stmfd sp!, {lr}
 
@ 	ldmia	r12, {r4-r8}
		
	cmp		r8, r4
	bge		doneoutter

	@mov		r9, r8
	@mov		r12, r4 

	mov		r12, #240
	mul		r9,r8,r12
	mul		r12, r4, r12


	@ save yloop uv
	mov lr, r2 
 
yloop: 	@for (y = y2; y < y3; y++)

	@xend = (xsxe & 0xff00)>>8;
	and		r11, r5, #0xff00
	mov		r11, r11, lsr #8
		
	@width = xend-(xstart>>24)
	subs	r4, r11, r5, lsr #24
	ble		noinner4 

	@ draw span		
	@u32 screen_ptr = dest+xstart+((y<<8)-(y<<4));//y*240;
	@mov		r11, r9, lsl #8
	@sub		r11, r11, r9, lsl #4
	add		r11, r0, r5, lsr #24
	add		r8, r11, r9 @r11
	
	mov		r2, lr
	
	@------------------------------
	@ INNER START
	@------------------------------	
	stmfd sp!,{lr}
	bl		inpoo
	ldmfd sp!,{lr}

		
noinner4:

	@restore yloop uv
	@mov		r2, lr

	@ adjust starting and ending point 
	@xsxe += deltalr;
	@uv += deltat;		
	add		r5, r5, r6
	add		lr, lr, r7
	  
	@for (y = y2; y < y3; y++)
	add		r9, r9, #240 @#1
	cmp		r9, r12
	blt		yloop

doneoutter:

	ldmfd sp!, {lr}
	@bx lr
	mov pc, lr
	
	.pool

inpoo:

@ r8 = dest 
@ r1 = bitmap
@ r2 = uv
@ r3 = dudv 
@ r4 = width 

@r10,r11,r4,r2,r8

	tst		r8, #1
	beq		anyleftovers4
	
@not aligned  
	and		r10,r2,#0xff00 	 
	add		r10,r10,r2, lsr #24  	   
	ldrb	r10,[r1,r10]      
	 
	bic		r8,r8,#1  
	ldrh	r11,[r8] 
	bic		r11,r11,#0xff00 
	orr		r11,r11,r10,lsl#8    

	strh	r11,[r8],#2
	  
	add		r2,r2,r3	@ uv+=dudv 

	subs	r4, r4, #1 
	ble		noinner2	

anyleftovers4:

@	movs	r4, r4, lsr #1

@	rsb	r4,r4,#120
@	mov	r11,r4,lsl #5
@	add	r11,r11,r4,lsl #3
	
@	add	pc,pc,r11
@	nop
		
@	.REPT	120		
@	and		r11,r2,#0xff00 	
@	add		r11,r11,r2, lsr #24  	       
@	ldrb	r11,[r1,r11]      	 	
@	add		r2,r2,r3	@ uv+=dudv	
@	and		r4,r2,#0xff00 
@	add		r4,r4,r2, lsr #24  
@	ldrb	r4,[r1,r4] 
@	add		r2,r2,r3	@ uv+=dudv  
@	orr		r11,r11,r4,lsl #8 
@	strh	r11,[r8],#2 		
@	.ENDR 


	movs	r4, r4, lsr #1

	rsb	r4,r4,#120
	mov	r11,r4,lsl #4
	add	r11,r11,r4,lsl #3
	 
	add	pc,pc,r11
	nop

	.REPT	120		
	and		r11,r2,#0xff00 	
	add		r11,r11,r2, lsr #24  	       
	ldrb	r4,[r1,r11]      	 	
	add		r2,r2,r3, lsl #1	@ uv+=dudv	
	orr		r11,r4,r4,lsl #8 	
	strh	r11,[r8],#2 		
	.ENDR  

	bcc		noinner2

 	and		r11,r2,#0xff00 	
	add		r11,r11,r2, lsr #24  	   
	ldrb	r11,[r1,r11] 
 
	ldrh	r4,[r8]
	bic		r4,r4,#0x00ff
	orr		r4,r4,r11
	strh	r4,[r8]  
	@------------------------------
	@ INNER END
	@------------------------------	

noinner2:

	mov pc, lr
	
	.pool

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	void inner2(u32 dest,u32 *bitmap, u32 uv,u32 dudv,u32 width)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
inner2:	
@ r0 = dest 
@ r1 = bitmap
@ r2 = uv
@ r3 = dudv 
@ r4 = width 
 
@r4=free
@r0=free  

@ @	mov r12, sp
@	stmfd sp!,{r1-r7}
@ @ 	ldmfd r12,{r4}
@
@	tst		r0, #1
@	beq		anyleftovers2
@	
@   @not aligned  
@	and		r5,r2,#0xff00 	 
@	add		r5,r5,r2, lsr #24  	   
@	ldrb	r5,[r1,r5]      
@	 
@	bic		r0,r0,#1  
@	ldrh	r6,[r0] 
@	bic		r6,r6,#0xff00 
@	orr		r6,r6,r5,lsl#8    
@
@	strh	r6,[r0],#2
@	  
@	add		r2,r2,r3	@ uv+=dudv 
@
@	subs	r4, r4, #1 
@	ble		done2	
@
@anyleftovers2:
@
@	movs	r4, r4, lsr #1
@
@inloop2:	
@
@	rsb	r4,r4,#120
@	mov	r6,r4,lsl #5
@	add	r6,r6,r4,lsl #3
@	
@	add	pc,pc,r6
@	nop
@	@nop 
@	
@	.REPT	120		
@	and		r6,r2,#0xff00 	
@	add		r6,r6,r2, lsr #24  	       
@	ldrb	r6,[r1,r6]      	 	
@	add		r2,r2,r3	@ uv+=dudv	
@	and		r4,r2,#0xff00 
@	add		r4,r4,r2, lsr #24  
@	ldrb	r4,[r1,r4] 
@	add		r2,r2,r3	@ uv+=dudv  
@	orr		r6,r6,r4,lsl #8 	
@	strh	r6,[r0],#2 		
@	.ENDR 
@
@	bcc		done2
@
@	and		r6,r2,#0xff00 	
@	add		r6,r6,r2, lsr #24  	   
@	ldrb	r6,[r1,r6] 
@
@	ldrh	r4,[r0]
@	bic		r4,r4,#0x00ff
@	orr		r4,r4,r6@,lsr#8 
@	strh	r4,[r0]  
@
@	@ Done so exit
@	b	done2
@
@done2:
@
@	ldmfd sp!,{r1-r7}
@	@ Done so exit
@	@bx lr	 
@	mov pc, lr
 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	void inner3(u32 dest,u32 *bitmap, u32 uv,u32 dudv,u32 width)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
inner3:	
 	mov pc, lr

	.pool

	.END
	
	
 	
