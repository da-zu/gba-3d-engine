@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void Sprite3D32(polygon_type *poly, u16 *screen )
@	void StretchSprite32(u8 *spriteMap, u8 *screen, s32 x1, s32 x2, s32 y1, s32 y2, s32 yr, s32 yw)
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@
@	DEFINES			@
@@@@@@@@@@@@@@@@@@@@@	

	.section	.iwram,"ax",%progbits
	.ARM
    .ALIGN			
    .GLOBAL Sprite3D32
	.GLOBAL StretchSprite32

StretchSprite32:
@r0=*spriteMap
@r1=*screen
@r2=x1
@r3=x2
@r4=y1
@r5=y2
@r6=yr
@r7=yw
 
	mov		r12, sp

	stmfd	sp!,{r0-r12,lr}

	ldmfd	r12,{r4-r7}

	@dx = ABS(x2-x1);
	subs	r3, r3, r2
	sublt	r3, r3, r3, lsl #1
	mvnle	r8, #0
	movgt	r8, #1

@r3=dx
@r8=sx

	cmp		r3, #1
	ldmlefd	sp!,{r0-r12,lr}
	movle		pc,lr 

	@dy = ABS(y2-y1);
	subs	r5, r5, r4
	sublt	r5, r5, r5, lsl #1
	mvnle	r9, #0
	movgt	r9, #1

@r5=dy
@r9=sy

	
	@e = (dy)-dx;
	sub		r10, r5, r3

@r10=e

	@dx2 = dx;
	mov		r11, r3

@r11=dx2

	@for(d=dx; d>=0; d--)
loopdx:
	
	@----------------------- 
	@ extract texture map	
	@-----------------------	
	@color = spriteMap[y1+yr*32];
	add		lr, r4, r6, asl #5	
	ldrb	lr, [r0, lr]

	@if (color != 0)
	cmp		lr, #0
	beq		transpix

@lr=color
	
	@-----------------------
	@ plot the pixel
	@-----------------------	
	@gba_setpixel(screen+x1+yw*240,color);	
	mov		r12, #240 
	mla		r12, r7, r12, r2
	add		r12, r12, r1
	@strb	lr, [r1, r12]
	stmfd	sp!, {r11}

	tst		r12, #1	
	bne		notaligned

	ldrh	r11,[r12]
	bic		r11,r11,#0x00ff
	orr		r11,r11,lr
	strh	r11,[r12]

	ldmfd	sp!, {r11}
	b		transpix
	
notaligned:
	bic		r12,r12,#1
	ldrh	r11,[r12]
	bic		r11,r11,#0xff00
	orr		r11,r11,lr,lsl#8
	
	strh	r11,[r12]
	ldmfd	sp!, {r11}
	
transpix: 
    
	cmp		r10, #0 
	blt		negt 
 
egt:  
	@while(e >= 0)  
	@{
	@	y1 += sy;
	@	e -= dx2;
	@}
	add		r4, r4, r9 
	subs	r10, r10, r11
	bge		egt

negt:
  
	@x1 += sx;
	add		r2, r2, r8

	@e += dy;
	add		r10, r10, r5
	

	subs	r3, r3, #1
	bge		loopdx

	@EXIT 
	ldmfd	sp!,{r0-r12,lr}
	mov		pc,lr
 

Sprite3D32:        

@ r0 = pPolyList
@ r1 = numPolys
@ r2 = video
 

	stmfd   sp!,{r4-r12,lr}		@ save stack 

	ldmfd   sp!,{r4-r12,lr}		@ save stack 
	mov		pc, lr
