@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@																		
@ void setpixel32(int index, int c)						
@																	 	
@																		
@	r0 = index	this is index+screen e.g &screen[y*240+x]													
@	r1 = color													
@	r2 = screen														 	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
				
		@.section	.iwram,"ax",%progbits
		@.arm
		@.align
		@.text
 
		.section	.iwram,"ax",%progbits
		.ARM
        .ALIGN
        .GLOBAL  setpixel32		

setpixel32:

	@ Check alignment	
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
	
	strh	r2,[r0]
  
	@ Return.
	bx	lr 
	.pool

	.END
