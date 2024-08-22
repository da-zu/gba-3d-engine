  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@********************************************************************	@
@*        Author:		David Zurillo								*	@
@*        Filename:		DrawPoly32.s								*	@
@*        Creation		Date: 18/09/01								*	@
@*        Description:	Fills a 16bit buffer (screen) with a		*	@
@*						value(colr), with padding for start and end.*	@
@*																	*   @
@*																	*	@
@*      Modification:												*	@
@*      -----------------------										*	@
@*		18/09/01 : David Zurillo									*	@
@*			Removed one sub from start plot odd						*	@
@*				@subcs		r0,r0,#1								*	@
@*			Removed one add from end plot even						*	@
@*				@add		r2,r2,#1								*	@
@*																	*	@
@*			I used the write back feature of str/ldr				*	@
@*																	*	@
@*																	*	@
@*										(c)2001 Halfbrick Studios	*	@
@********************************************************************	@
@																		@
@	void Fill2Array32 (u32 col, u32 screen, u32 length, u32 start);	@
@																	 	@
@																		@
@	r0 = index,col														@
@	r1 = colr,screen															@
@	r2 = length														 	@
@	r3 = screen,index															@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				
		@.section	.iwram,"ax",%progbits
		@.arm
		@.align
		@.text
 
		.section	.iwram,"ax",%progbits
		.ARM
        .ALIGN
        .GLOBAL  gba_setpixel
		



@ void gba_setpixel(int index, int c, u32 screen)@
@ where 0 < x < 240, 0 < y < 160, 0 < c < 256
gba_setpixel:
	
	@ Generate screen address
	
		
	@ Calculate pixel address.
	 @add		r0, r0, r2
	
	tst		r0, #1	

@aligned
	ldreqh	r2,[r0]
	biceq	r2,r2,#0x00ff
	orreq	r2,r2,r1
	
@not aligned
	bicne	r0,r0,#1
	ldrneh	r2,[r0]
	bicne	r2,r2,#0xff00
	orrne	r2,r2,r1,lsl#8
	
	strh		r2,[r0]
  
	@ Return.
	@mov	pc, lr
	
	bx	lr 
	.pool

	.END
