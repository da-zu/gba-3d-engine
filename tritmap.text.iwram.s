@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void tritmap( vertexTMap *pv1,vertexTMap *pv2,vertexTMap *pv3 u16 *screen )
@	void inner(u32 dest,u32 *bitmap, u32 uv,u32 dudv,u32 width)
@
@	NOTEs:
@
@
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@
@	DEFINES			@
@@@@@@@@@@@@@@@@@@@@@	

@@@@@@@@@@@@@
@ sp offsets
@@@@@@@@@@@@@
LEFT_SECTION				=0
RIGHT_SECTION				=4
LEFT_SECTION_HEIGHT			=8
RIGHT_SECTION_HEIGHT		=12
DELTA_LEFT_X				=16
DELTA_LEFT_U				=20
DELTA_LEFT_V				=24
DELTA_RIGHT_X				=28
LEFT_U						=32
LEFT_V						=36
LEFT_X						=40
RIGHT_X						=44
LONGEST						=48
TEMP						=52
@@@@@@@@@@@@@@@@@@@@@

	.section	.iwram,"ax",%progbits
	.ARM
    .ALIGN 4		
	.EXTERN VideoBuffer 
	.EXTERN Draw_Poly_Span_Textured
    .GLOBAL tritmap 
	.GLOBAL inner 
	.GLOBAL inner2
	.GLOBAL inner3 
	.EXTERN	PrjTable
	.EXTERN fixedpointasm_div
	.EXTERN levelTmap
	.EXTERN Font_itoa
	.EXTERN Gba_Print
	.EXTERN VideoBuffer
	.EXTERN szp
	.GLOBAL longestpos



	 
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	Tirangle Texture Mapper 
@		void tritmap( vertexTMap *pv1,vertexTMap *pv2,vertexTMap *pv3 u16 *screen )
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
tritmap:   
@r0=pv1
@r1=pv2
@r2=pv3
@r3=screen
     
	stmfd sp!, {r7-r9,lr}
	
	sub		sp, sp, #60

 	ldr		lr, =PrjTable 

	@ get xy's
	ldmia	r0,{r4,r5}
	ldmia	r1,{r6,r7}
	ldmia	r2,{r8,r9}


@r0=pv1
@r1=pv2
@r2=pv3
@r3=screen
@r4=v1->x
@r5=v1->y
@r6=v2->x
@r7=v2->y
@r8=v3->x
@r9=v3->y

@----------------------------------------
@ Sort the triangle so that v1 points to 
@ the topmost, v2 to the middle and 
@ v3 to the bottom vertexTMap.
@---------------------------------------- 

	@if(v1->y > v2->y)
	cmp		r5, r7
	ble		tsty1y3
	@ swap v1,v2
	eor r0, r0, r1
	eor r1, r1, r0
	eor r0, r0, r1
	ldmia	r0,{r4,r5}
	ldmia	r1,{r6,r7}	

tsty1y3:
	@if(v1->y > v3->y)
	cmp		r5, r9
	ble		tsty2y3
	@ swap v1,v2
	eor r0, r0, r2
	eor r2, r2, r0
	eor r0, r0, r2
	ldmia	r0,{r4,r5}	
	ldmia	r2,{r8,r9}

tsty2y3:
	@if(v2->y > v3->y)
	cmp		r7, r9
	ble		noswap
	@ swap v1,v2
	eor r1, r1, r2
	eor r2, r2, r1
	eor r1, r1, r2	
	ldmia	r1,{r6,r7}
	ldmia	r2,{r8,r9}

noswap:
	add		r0, r0, #8
	add		r1, r1, #8
	add		r2, r2, #8

@r0=pv1+xy
@r1=pv2+xy
@r2=pv3+xy
@r3=screen
@r4=v1->x
@r5=v1->y
@r6=v2->x
@r7=v2->y
@r8=v3->x
@r9=v3->y

@----------------------------------------
@ We start out by calculating the length 
@ of the longest scanline.
@----------------------------------------
	@height = v3->y - v1->y;
    @if(height == 0) return;
	subs	r10, r9, r5	
	beq		ret
	 
@r10=height 
	@op1 = ((v2->y - v1->y));//<<16);
	@op2 = PrjTable[height];
	@temp = (op1 * op2);//>>16;
	sub		r11, r7, r5  	
	ldr		r12, [lr,r10,asl #2]
	mul		r10, r12, r11	
	str		r10, [sp, #TEMP]  
@r10=temp 
   
	@longest = (temp * (v3->x - v1->x)) + ((v1->x - v2->x) << 16);
	sub		r11, r8, r4
	sub		r12, r4, r6
	mov		r12, r12, asl #16
	mlas	r12, r10, r11, r12
	


    
@r12=longest

	 
@----------------------------------------
@ Now that we have the length of the 
@ longest scanline we can use that  
@ to tell us which is left and which is 
@ the right side of the triangle.  
@----------------------------------------

	@if(longest == 0) return;
	@cmp		r12, #0 
	bgt		longestpos
	beq		ret  
	
	
@----------------------------------------
@ If longest is neg. we have the middle  
@ vertex on the right side.
@ Store the pointers for the right and 
@ left edge of the triangle.
@----------------------------------------  
longestneg:  
@r0=pv1+xy
@r1=pv2+xy
@r2=pv3+xy    
@r3=screen     
@r4=v1->x     
@r5=v1->y   
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y    
@r12=longest

@----------------------------------------
@ Ugly compensation so that the dudx,dvdx 
@ divides won't overflow if the longest 
@ scanline is very short.
@----------------------------------------
    @if(longest > -0x1000)
    @longest = -0x1000;
	ldr		r10, =-0x1000	
	cmp		r12, r10	
	movgt	r12, r10
	str		r12, [sp, #LONGEST]

	

	@right_section  = 2;        
    @left_section   = 1;
	mov		r10, #1
	mov		r11, #2
	@add		r12, sp, #LEFT_SECTION
	stmia	sp, {r10,r11}

	@mov		r10, #2
	@str		r10,[sp,#RIGHT_SECTION]
	@mov		r10, #1
	@str		r10,[sp,#LEFT_SECTION]

@----------------------------------------  
@ Calculate initial left and right parameters
@----------------------------------------  
	@height = v3->y - v1->y;
	@if(height <= 0) return;
	subs	r10, r9, r5
	ble		ret

	 
@r10=height
	@left_section_height = height;
	str		r10, [sp, #LEFT_SECTION_HEIGHT]

@----------------------------------------  
@ Calculate the deltas along this section (left)	
@----------------------------------------  			
	@op1 = ((v3->x - v1->x));
	@op2 = PrjTable[height];
	@delta_left_x = (op1 * op2);
	sub		r11, r8, r4
	ldr		r10, [lr, r10, asl #2]
	mul		r11, r10, r11
	
	str		r11,[sp,#DELTA_LEFT_X]

	
	@left_x = v1->x << 16;
	mov		r11, r4, asl #16
	   						
	str		r11,[sp,#LEFT_X]	

 
@r0=pv1+xy
@r1=pv2+xy
@r2=pv3+xy    
@r3=screen    
@r4=v1->x     
@r5=v1->y   
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y 
@r10=op2

	@op1 = ((v3->u - v1->u));
	@delta_left_u = (op1 * op2)>>6;			
	
	ldmia	r0,{r4,r5}						
	ldmia	r2,{r11,r12}
		
@r0=pv1+xy//uv
@r1=pv2+xy
@r2=pv3+xy//uv    
@r3=screen    
@r4=v1->u
@r5=v1->v   
@r6=v2->x   
@r7=v2->y         
@r8=v3->x      
@r9=v3->y 
@r10=op2
@r11=v3->u
@r12=v3->v

	sub		r11, r11, r4
	mul		r11, r10, r11
	mov		r11, r11, asr #6 
	str		r11, [sp, #DELTA_LEFT_U]


 
	@left_u = v1->u << 10;	
	mov		r4, r4, asl #10 
	str		r4, [sp, #LEFT_U] 

							
	@op1 = ((v3->v - v1->v));
	@delta_left_v = (op1 * op2)>>6;
	sub		r11, r12, r5
	mul		r11, r10, r11
	mov		r11, r11, asr #6
	str		r11, [sp, #DELTA_LEFT_V]
	
					
	@left_v = v1->v << 10;
	mov		r5, r5, asl #10
	str		r5, [sp, #LEFT_V]
	
		
@r0=pv1+xy//uv
@r1=pv2+xy
@r2=pv3+xy//uv    
@r3=screen    
@r4=free
@r5=free 
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y 
@r10=free
@r11=free
@r12=free

@----------------------------------------  
@ Calculate the deltas along this section (right)	
@----------------------------------------  		
	sub		r0, r0, #8	
	ldmia	r0!,{r4,r5}
@	sub		r2, r2, #8

@r0=pv1+xy
@r1=pv2+xy
@r2=pv3+xy    
@r3=screen    
@r4=v1->x
@r5=v1->y
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y 
@r10=free
@r11=free
@r12=free

	@height = v2->y - v1->y;  
	subs 	r10, r7, r5 	 
	bgt		rsnzh1

@r10=height

@----------------------------------------
@ The first right section had zero height. 
@ Use the next section. 
@----------------------------------------
	ldr		r11, [sp,#RIGHT_SECTION]
	sub		r11, r11, #1
	str		r11, [sp,#RIGHT_SECTION]


	

	@height = v3->y - v2->y;
	@if(height <= 0) return;
	subs	r10, r9, r7 
	ble		ret

@----------------------------------------
@ Calculate the deltas along this section (zero height)
@----------------------------------------
	@right_section_height = height;
	str		r10, [sp, #RIGHT_SECTION_HEIGHT]

	@op1 = ((v3->x - v2->x));
	@op2 = PrjTable[height];
	@delta_right_x = (op1 * op2);
	sub		r11, r8, r6
	ldr		r12, [lr, r10, asl #2]
	mul		r11, r12, r11
	str		r11, [sp, #DELTA_RIGHT_X]

@r10=height
@r12=op2
		 
	@right_x = v2->x << 16;			
	mov		r11, r6, asl #16
	str		r11, [sp, #RIGHT_X]
	
	b		calcconstdeltasneg

@----------------------------------------
@ Calculate the deltas along this section (non zero height)
@----------------------------------------
rsnzh1:
@r0=pv1+xy
@r1=pv2+xy
@r2=pv3+xy    
@r3=screen    
@r4=v1->x
@r5=v1->y
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y 
@r10=height
@r11=free
@r12=free

	@right_section_height = height;
	str		r10, [sp, #RIGHT_SECTION_HEIGHT]

	@op1 = ((v2->x - v1->x));
	@op2 = PrjTable[height]; 
	@delta_right_x = (op1 * op2);
	sub		r11, r6, r4
	ldr		r12, [lr, r10, asl #2]
	mul		r11, r12, r11 
	str		r11, [sp, #DELTA_RIGHT_X]
 
						 
	@right_x = v1->x << 16;	
	mov		r12, r4, asl #16 
	str		r12, [sp, #RIGHT_X] 
  

@----------------------------------------
@ Now we calculate the constant deltas 
@ for u and v (dudx, dvdx)
@----------------------------------------
calcconstdeltasneg:
@r0=pv1+xy
@r1=pv2+xy
@r2=pv3+xy    
@r3=screen    
@r4=v1->x
@r5=v1->y
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y 
@r10=free
@r11=free
@r12=free

	@7 6 5 4
	@destptr = (u16 *) (v1->y * 120 + screen);
@	mov		r12, #240
@	mla		r12, r5, r12, r3
@	mov		r12, r5, lsl #7
@	add		r12, r12, r5, lsl #6
@	add		r12, r12, r5, lsl #5
@	add		r12, r12, r5, lsl #4
@	add		r12, r3, r12
	mov		r12, r5, lsl#8		@ lr = y * 256
	sub		r12,r12, r5,lsl#4	@ lr = ((y*256)-(y*16)) = y * 240
	add		r12, r3, r12

 
 
	@MOVE vertex pointers
	@mov		r10, r0
	@mov		r11, r1
	@mov		r12, r2
 
	ldmia	r0,{r4,r5}
	ldmia	r1,{r0,r1}
	ldmia	r2,{r10,r11} 

	ldr		r2, [sp, #TEMP]


@r2=temp
	
@r0=v2->u
@r1=v2->v
@r2=temp    
@r3=screen    
@r4=v1->u
@r5=v1->v
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y 
@r10=v3->u
@r11=v3->v
@r12=destptr	
@lr=PrjTable


@----------------------------------------
@ Now we calculate the constant deltas 
@ for u and v (dudx, dvdx)
@----------------------------------------

@r0=v2->u
@r1=v2->v
@r2=temp    
@r3=screen    
@r4=v1->u
@r5=v1->v
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y 
@r10=v3->u
@r11=v3->v
@r12=destptr	
@lr=PrjTable

	
	@(v3->u - v1->u)
	sub		r10, r10, r4
	sub		r4, r4, r0
	@((v1->u - v2->u)<<16)
	mov		r4, r4, asl #16

	@dvdx = fixedpointasm_div(temp*(v3->v - v1->v)+((v1->v - v2->v)<<16), longest)>>8;
	sub		r11, r11, r5
	sub		r1, r5, r1
	mov		r1, r1, asl #16
	mla		r0, r2, r11, r1

	ldr		r1, [sp, #LONGEST]
 
	mov		lr,pc
	bl		fixedpointasm_div
	@ldr		lr,=PrjTable

	mov		r5, r0, asr #8

@r5=dvdx

	@dudx = fixedpointasm_div(temp*(v3->u - v1->u)+((v1->u - v2->u)<<16), longest)>>8;		
	mla		r0, r2, r10, r4

	mov		lr,pc
	bl		fixedpointasm_div
	mov		r0, r0, asr #8
	ldr		lr,=PrjTable
	
@r0=dudx
@r1=free
@r2=free    
@r3=screen    
@r4=free
@r5=dvdx
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y 
@r10=free
@r11=free
@r12=destptr	
@lr=PrjTable

@----------------------------------------
@ Here starts the outer loop 
@ (for each scanline).
@----------------------------------------
	add		sp, sp, #LEFT_U
	ldmfd	sp,{r1,r2,r10,r11}
	sub		sp, sp, #LEFT_U
	
	@ldr		r1, [sp, #LEFT_U]
	@ldr		r2, [sp, #LEFT_V]
	@ldr		r10, [sp, #LEFT_X] 
	@ldr		r11, [sp, #RIGHT_X] 
   
@r1=left_u  
@r2=left_v 
  
@r10=left_x
@r11=right_x    
 
 
@r0=dudx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dvdx  
@r6=v2->x     
@r7=v2->y               
@r8=v3->x     
@r9=v3->y    
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable  
     
	@mov		sp, lr    
	stmfd	sp!,{r6-r9} 
       	   
@r4=free   
@r6=free   
@r7=free                
@r8=free       
@r9=free     
	
outterneg:  
	@ldr		r4, =0xffff 	
	mov		r4, #0xff00
	orr		r4, r4, #0x00ff
	@uv = (((left_u>>8) << 16)|((left_v>>8) & 0xffff));
	mov		r6, r1, asr #8	
	and		r9, r4, r2, asr #8
	orr		r9, r9, r6, asl #16 

@r9=uv
	
	@dudv = ((dudx << 16)|(dvdx & 0xffff));
	@mov		r6, r0, asl #16
	and		r8, r5, r4
	orr		r8, r8, r0, asl #16 
 
@r8=dudv


@r0=dudx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=0xffff   
@r5=dvdx  
@r6=free     
@r7=free               
@r8=dudv
@r9=uv
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable  

	@x1 = ceil(left_x);
	@x += 0xffff;
	@return x >> 16;	
	add		r7, r10, r4
	movs	r7, r7, asr #16
	

@r7=x1

@----------------------------------------
@ test for x clipping
@----------------------------------------
	@if (x1 < 0)
	@dx = 0- x1;	
	rsblt	r6, r7, #0
	

@r6=dx
  
@----------------------------------------
@ slide interpolants over if need to be  
@ clipped 
@----------------------------------------
	@uv+=dx*dudv;
	mlalt	r9, r6, r8, r9
				
	@x1 = 0;
	movlt	r7, #0
 

@r6=free
 
	@x2 = ceil(right_x);
	@x += 0xffff;
	@return x >> 16;	 
	add		r6, r11, r4
	mov		r6, r6, asr #16
	
@r6=x2
	

@----------------------------------------
@ test for x clipping RHS
@----------------------------------------
	@if (x2 > 240)
	@  x2 = 240;
	cmp		r6, #240
	movgt	r6, #240
	
			
	@width = x2 - x1;
	subs	r6, r6, r7 
	
@r6=width
@r7=x1


	
	@if(width > 0)
	ble		noinnerneg

	@u8 * dest = (u32)destptr + (u32)x1;
	add		r7, r12, r7

		
@r7=dest

@----------------------------------------
@ test for y clipping
@---------------------------------------- 
	@if ((dest >= screen) && (dest<= screen+0x05000))	
	cmp		r7, r3
	blt		noinnerneg
	add		r4, r3, #0x9600
	cmp		r7, r4
	bgt		noinnerneg 

@r4=free


	@inner(dest,&levelTmap,uv,dudv,width); 
	stmfd	sp!, {r0,r1,lr}@-r6}
	
	@mov		r0, r7
	ldr		r1, =levelTmap  
	@mov		r2, r9    
	@mov		r3, r8 
	@mov		r4, r6
	
@	mov		lr, pc
	bl		inner
	
	@ldr		lr, =PrjTable
	ldmfd	sp!, {r0,r1,lr}@-r6}
	
	

@r0=dudx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dvdx  
@r6=free     
@r7=free               
@r8=dudv
@r9=uv
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable 

@r0=dudx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=0xffff   
@r5=dvdx  
@r6=free     
@r7=free               
@r8=dudv
@r9=uv
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable  
noinnerneg:

	
	@destptr += 120;
	add		r12, r12, #240

	add		r4, sp, #LEFT_SECTION_HEIGHT+16
	ldmfd	r4,{r4,r6,r7,r8,r9}
	

@----------------------------------------
@ Interpolate along the left edge of the 
@ triangle
@----------------------------------------	
	@if(--left_section_height <= 0)  // At the bottom of this section?
	@	return;				
	@ldr		r4, [sp, #LEFT_SECTION_HEIGHT+16]
	subs	r4, r4, #1
	ldmmifd	sp!,{r6-r9}  
	bmi		ret
	str		r4, [sp, #LEFT_SECTION_HEIGHT+16]
		
	@else
@r0=dudx 
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dvdx  
@r6=free     
@r7=free               
@r8=dudv
@r9=uv
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable

	

	@left_x += delta_left_x;
	@ldr		r4, [sp, #DELTA_LEFT_X+16]
	add		r10, r10, r7

	@left_u += delta_left_u;
	@ldr		r4, [sp, #DELTA_LEFT_U+16]
	add		r1, r1, r8

	@left_v += delta_left_v;
	@ldr		r4, [sp, #DELTA_LEFT_V+16]
	add		r2, r2, r9

@----------------------------------------
@ Interpolate along the right edge of the 
@ triangle
@----------------------------------------
	@if(--right_section_height <= 0) // At the bottom of this section?
	@ldr		r4, [sp, #RIGHT_SECTION_HEIGHT+16]
	subs	r6, r6, #1
	
	ble		interreneg
		
	str		r6, [sp, #RIGHT_SECTION_HEIGHT+16]
	
	@right_x += delta_right_x;
	ldr		r4, [sp, #DELTA_RIGHT_X+16]
	add		r11, r11, r4 



	
	b		outterneg  
	 
interreneg:
@r0=dudx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dvdx  
@r6=free     
@r7=free              
@r8=free     
@r9=free   
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable
	
	@if (right_section == 2) // section did not initally have 0 height
	ldr		r4, [sp, #RIGHT_SECTION+16]
	cmp		r4, #2
	ldmnefd	sp!,{r6-r9} 
	bne		ret				@// section initally had 0 height

	

	@right_section--;
	sub		r4, r4, #1
	str		r4, [sp, #RIGHT_SECTION+16]

	
	ldmfd	sp,{r6-r9} 
	

@r0=dudx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dvdx  
@r6=v2->x     
@r7=v2->y               
@r8=v3->x     
@r9=v3->y    
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable

	@height = v3->y - v2->y;
	@if(height <= 0)	return;
	subs	r4, r9, r7
	ldmlefd	sp!,{r6-r9}
	ble		ret

@r4=height

@----------------------------------------
@ Calculate the deltas along this section
@----------------------------------------						
	
	
	
	@right_section_height = height;	
	str		r4, [sp, #RIGHT_SECTION_HEIGHT+16]

	@op1 = ((v3->x - v2->x));
	@op2 = PrjTable[height];
	@delta_right_x = (op1 * op2);
	
	ldr		r4, [lr, r4, asl #2]
	sub		lr, r8, r6

	

@r4=op2
@r5=op1	

	mul		r4, lr, r4
	str		r4, [sp, #DELTA_RIGHT_X+16] 



	@right_x = v2->x << 16;			
	mov		r11, r6, asl #16
 
	ldr		lr, =PrjTable
	
	@stmfd	sp!,{r6-r9}
	b	outterneg

@----------------------------------------
@ If longest is pos. we have the middle   
@ vertex on the left side.
@ Store the pointers for the left and   
@ right edge of the triangle.
@----------------------------------------

longestpos:
@r0=pv1+xy
@r1=pv2+xy
@r2=pv3+xy     
@r3=screen    
@r4=v1->x      
@r5=v1->y   
@r6=v2->x   
@r7=v2->y          
@r8=v3->x     
@r9=v3->y    
@r12=longest
@----------------------------------------
@ Ugly compensation so that the dudx,dvdx 
@ divides won't overflow if the longest 
@ scanline is very short.
@---------------------------------------- 
    @if(longest < 0x1000)
    @  longest = 0x1000;     
	@ldr		r10, =0x1000	
	@mov		r10, #0x1000
	cmp		r12, #0x1000	
	movlt	r12, #0x1000
	str		r12, [sp, #LONGEST]


		
 
	@left_section  = 2;        
    @right_section   = 1;
	mov		r10, #2
	mov		r11, #1
	@add		r12, sp, #LEFT_SECTION
	stmia	sp, {r10,r11}

	@mov		r10, #2
	@str		r10,[sp,#LEFT_SECTION]
	@mov		r10, #1
	@str		r10,[sp,#RIGHT_SECTION] ] 

	
@----------------------------------------  
@ Calculate initial left and right para meters  
@----------------------------------------  
	@height = v3->y - v1->y;
	@if(height <= 0) return;
	subs	r10, r9, r5 
	ble		ret

@r10=height
	@right_section_height = height;  
	str		r10, [sp, #RIGHT_SECTION_HEIGHT]

@----------------------------------------  
@ Calculate the deltas along this section (right)	
@----------------------------------------  			
	@op1 = ((v3->x - v1->x));
	@op2 = PrjTable[height];
	@delta_right_x = (op1 * op2);
	sub		r11, r8, r4
	ldr		r10, [lr, r10, asl #2]
	mul		r11, r10, r11
	
	str		r11,[sp,#DELTA_RIGHT_X]

	@right_x = v1->x << 16;
	mov		r11, r4, asl #16
	   						
	str		r11,[sp,#RIGHT_X]	


@----------------------------------------  
@ Calculate the deltas along this section (left)	
@----------------------------------------  	
@r0=pv1+xy
@r1=pv2+xy
@r2=pv3+xy    
@r3=screen    
@r4=v1->x     
@r5=v1->y   
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y    
@r12=longest
		
	@destptr = (u16 *) (v1->y * 120 + screen);
	@mov		r12, #240
	@mla		r12, r5, r12, r3
	mov		r12, r5, lsl#8		@ lr = y * 256
	sub		r12,r12, r5,lsl#4	@ lr = ((y*256)-(y*16)) = y * 240
	add		r12, r3, r12
	stmfd	sp!,{r12}

	@height = v2->y - v1->y;			
	@if(height <= 0)
	subs	r10, r7, r5

	bgt		lsechgt0
 
 
@r10=height

	@left_section--;
	ldr		r11, [sp, #LEFT_SECTION+4]
	sub		r11, r11, #1
	str		r11, [sp, #LEFT_SECTION+4]

	@height = v3->y - v2->y;
	@if(height <= 0) return;
	subs	r10, r9, r7
	ldmlefd	sp!,{r12}
	ble		ret

	@left_section_height = height;	
	str		r10, [sp, #LEFT_SECTION_HEIGHT+4]

	@op1 = ((v3->x - v2->x));
	@op2 = PrjTable[height];
	@delta_left_x = (op1 * op2);
	ldr		r10, [lr, r10, asl #2]
	sub		r11, r8, r6
	mul		r11, r10, r11

	str		r11, [sp, #DELTA_LEFT_X+4]
			
@r10=op2

	@left_x = v2->x << 16;
	mov		r11, r6, asl #16
	str		r11, [sp, #LEFT_X+4]

	
	ldmia	r1,{r4,r5}						
	ldmia	r2,{r11,r12}
	ldmia	r0,{r0,r1}

@r0=v1->u
@r1=v1->v
@r2=free    
@r3=screen    
@r4=v2->u
@r5=v2->v   
@r6=v2->x   
@r7=v2->y         
@r8=v3->x      
@r9=v3->y 
@r10=op2
@r11=v3->u
@r12=v3->v

	@op1 = ((v3->u - v2->u));
	@delta_left_u = (op1 * op2)>>6;
	sub		r2, r11, r4
	mul		r2, r10, r2
	mov		r2, r2, asr #6
	str		r2, [sp, #DELTA_LEFT_U+4]
	
	

	@left_u = v2->u << 10;
	mov		r2, r4, asl #10
	str		r2, [sp, #LEFT_U+4]	

	
	@op1 = ((v3->v - v2->v));
	@delta_left_v = (op1 * op2)>>6;
	sub		r2, r12, r5
	mul		r2, r10, r2
	mov		r2, r2, asr #6
	str		r2, [sp, #DELTA_LEFT_V+4]

	
	@left_v = v2->v << 10;
	mov		r2, r5, asl #10
	str		r2, [sp, #LEFT_V+4]

	b		calcconstdeltaspos

lsechgt0:
@r0=pv1+xy
@r1=pv2+xy
@r2=pv3+xy    
@r3=screen    
@r4=v1->x     
@r5=v1->y   
@r6=v2->x   
@r7=v2->y         
@r8=v3->x     
@r9=v3->y    
@r10=height 
@r12=longest

	@left_section_height = height;
	str		r10, [sp, #LEFT_SECTION_HEIGHT+4]


	@op1 = ((v2->x - v1->x));
	@op2 = PrjTable[height];
	@delta_left_x = (op1 * op2);
	ldr		r10, [lr, r10, asl #2] 
	sub		r11, r6, r4 
	mul		r11, r10, r11
	str		r11, [sp, #DELTA_LEFT_X+4]  
@r10=op2 
	    
	@left_x = v1->x << 16;
	mov		r11, r4, asl #16 
	str		r11, [sp, #LEFT_X+4]				  


	ldmia	r1,{r4,r5}						
	ldmia	r2,{r11,r12}
	ldmia	r0,{r0,r1}

@r0=v1->u
@r1=v1->v
@r2=free    
@r3=screen    
@r4=v2->u
@r5=v2->v   
@r6=v2->x   
@r7=v2->y         
@r8=v3->x      
@r9=v3->y 
@r10=op2
@r11=v3->u
@r12=v3->v

	@op1 = ((v2->u - v1->u)); 
	@delta_left_u = (op1 * op2)>>6;
	sub		r2, r4, r0		
	mul		r2, r10, r2
	mov		r2, r2, asr #6 
	
	str		r2, [sp, #DELTA_LEFT_U+4]
	

	@left_u = v1->u << 10;
	mov		r2, r0, asl #10
	str		r2, [sp, #LEFT_U+4]				
  
	@op1 = ((v2->v - v1->v));
	@delta_left_v = (op1 * op2)>>6;  
	sub		r2, r5, r1
	mul		r2, r10, r2 
	mov		r2, r2, asr #6  
	str		r2, [sp, #DELTA_LEFT_V+4]			
	

	@left_v = v1->v << 10;
	mov		r2, r1, asl #10
	str		r2, [sp, #LEFT_V+4]

	
@----------------------------------------  
@ Now we calculate the constant deltas 
@ for u and v (dudx, dvdx)
@----------------------------------------  
calcconstdeltaspos:
@r0=v1->u
@r1=v1->v
@r2=free    
@r3=screen    
@r4=v2->u
@r5=v2->v   
@r6=v2->x   
@r7=v2->y         
@r8=v3->x      
@r9=v3->y 
@r10=op2
@r11=v3->u
@r12=v3->v

	
	@destptr = (u16 *) (v1->y * 120 + screen);
	ldmfd	sp!,{r2}

	
@r2=destptr

	stmfd	sp!, {r4,r5,r11,r12}

		
	sub		r11, r11, r0
	sub		r4, r0, r4
	mov		r4, r4, asl #16	

	@dvdx = fixedpointasm_div(temp*(v3->v - v1->v)+((v1->v - v2->v)<<16), longest)>>8;
	sub		r12, r12, r1
	sub		r5, r1, r5
	mov		r1, r5, asl #16
	ldr		r5, [sp, #TEMP+16]
	mla		r4, r5, r11, r4
	mla		r0, r5, r12, r1

	ldr		r1, [sp, #LONGEST+16]

	mov		lr,pc
	bl		fixedpointasm_div
	@ldr		lr,=PrjTable

	mov		r5, r0, asr #8
@r5=dvdx

	@dudx = fixedpointasm_div(temp*(v3->u - v1->u)+((v1->u - v2->u)<<16), longest)>>8;
		
	mov		r0, r4
	
	mov		lr,pc
	bl		fixedpointasm_div
	ldr		lr,=PrjTable

	mov		r0, r0, asr #8   

@r0=dudx 
		  
	mov		r12, r2	



@r12=destptr

@r0=dudx
@r1=free
@r2=free    
@r3=screen    
@r4=free
@r5=dvdx  
@r6=v2->x   
@r7=v2->y         
@r8=v3->x      
@r9=v3->y 
@r10=free
@r11=free
@r12=destptr
@lr=PrjTable
@----------------------------------------
@ Here starts the outer loop 
@ (for each scanline).
@----------------------------------------
	add		r4, sp, #LEFT_U+16
	ldmia	r4, {r1,r2,r10,r11}

	@ldr		r1, [sp, #LEFT_U+16]
	@ldr		r2, [sp, #LEFT_V+16]
	@ldr		r10, [sp, #LEFT_X+16] 
	@ldr		r11, [sp, #RIGHT_X+16] 
	   
@r1=left_u  
@r2=left_v 
  
@r10=left_x
@r11=right_x    


	stmfd	sp!,{r6-r9} 

@r0=dudx
@r1=left_u
@r2=left_v  
@r3=screen    
@r4=free
@r5=dvdx  
@r6=v2->x   
@r7=v2->y         
@r8=v3->x      
@r9=v3->y 
@r10=left_x
@r11=right_x
@r12=destptr
@lr=PrjTable

@r4=free   
@r6=free   
@r7=free                
@r8=free       
@r9=free   
outterpos:

	@ldr		r4, =0xffff 	
	mov		r4, #0xff00
	orr		r4, r4, #0x00ff
	@uv = (((left_u>>8) << 16)|((left_v>>8) & 0xffff));
	mov		r6, r1, asr #8	
	and		r9, r4, r2, asr #8
	orr		r9, r9, r6, asl #16 

@r9=uv
	
	@dudv = ((dudx << 16)|(dvdx & 0xffff));
	@mov		r6, r0, asl #16
	and		r8, r5, r4
	orr		r8, r8, r0, asl #16 
 
@r8=dudv

@r0=dudx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=0xffff   
@r5=dvdx  
@r6=free     
@r7=free               
@r8=dudv
@r9=uv
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable  


	@x1 = ceil(left_x);
	@x += 0xffff;
	@return x >> 16;	
	add		r7, r10, r4
	movs	r7, r7, asr #16


@r7=x1

@----------------------------------------
@ test for x clipping
@----------------------------------------
	@if (x1 < 0)
	@dx = 0- x1; 	
	rsblt	r6, r7, #0

@r6=dx

@----------------------------------------
@ slide interpolants over if need to be 
@ clipped
@----------------------------------------
	@uv+=dx*dudv;
	mlalt	r9, r6, r8, r9
				
	@x1 = 0;
	movlt	r7, #0

 
@r6=free
 
	@x2 = ceil(right_x);
	@x += 0xffff;
	@return x >> 16;	 
	add		r6, r11, r4
	mov		r6, r6, asr #16

@r6=x2
	

@----------------------------------------
@ test for x clipping RHS
@----------------------------------------
	@if (x2 > 240)
	@  x2 = 240;
	cmp		r6, #240
	movgt	r6, #240
	 
			
	@width = x2 - x1; 
	subs	r6, r6, r7 

		
@r6=width
@r7=x1

	
	@if(width > 0)
	ble		noinnerpos 
 
	@u8 * dest = (u32)destptr + (u32)x1; 
	add		r7, r12, r7 

			 
@r7=dest

@----------------------------------------
@ test for y clipping
@----------------------------------------
	@if ((dest >= screen) && (dest<= screen+0x05000))	
	cmp		r7, r3
	blt		noinnerpos
	add		r4, r3, #0x9600
	cmp		r7, r4
	bgt		noinnerpos

@r4=free

	@inner(dest,&levelTmap,uv,dudv,width); 
	stmfd	sp!, {r0,r1,lr}@-r6}
	
	@mov		r0, r7
	ldr		r1, =levelTmap  
	@mov		r2, r9    
	@mov		r3, r8 
	@mov		r4, r6
	
@	mov		lr, pc
	bl		inner
	
	@ldr		lr, =PrjTable
	ldmfd	sp!, {r0,r1,lr}@-r6}



@r0=dvdx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dudx  
@r6=free     
@r7=free               
@r8=dudv
@r9=uv
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable  

noinnerpos:

	@destptr += 120; 
	add		r12, r12, #240


@----------------------------------------
@ Interpolate along the left edge of 
@ the triangle.
@----------------------------------------
	@if(--left_section_height <= 0)  // At the bottom of this section?
	ldr		r4, [sp, #LEFT_SECTION_HEIGHT+32]
	subs	r4, r4, #1
	str		r4, [sp, #LEFT_SECTION_HEIGHT+32]
  
	@no
	bgt		notbotpos

	@if (left_section == 2)
	ldr		r4, [sp, #LEFT_SECTION+32]
	cmp		r4, #2
	ldmnefd sp!,{r6-r9} 
	ldmnefd sp!,{r6-r9}
	bne		ret  



	@left_section--; 
	sub		r4, r4, #1
	str		r4, [sp, #LEFT_SECTION+32]

	 


@r0=dvdx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dudx
@r6=free     
@r7=free               
@r8=free
@r9=free
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable 

	ldmfd	sp,{r6-r9}	@ v2 & v3 xy's

@r6=v2->x     
@r7=v2->y              
@r8=v3->x
@r9=v3->y



	@height = v3->y - v2->y;
	@if(height <= 0) return;
	subs	r4, r9, r7	
	ldmlefd sp!,{r6-r9}
	ldmlefd sp!,{r6-r9}
	ble		ret  

@r4=height
 
	@left_section_height = height;			
	str		r4, [sp, #LEFT_SECTION_HEIGHT+32]

	@op1 = ((v3->x - v2->x));
	@op2 = PrjTable[height];
	@delta_left_x = (op1 * op2);     
	ldr		r4, [lr, r4, asl #2] 
	sub		r8, r8, r6     
	mul		r8, r4, r8 
	str		r8, [sp, #DELTA_LEFT_X+32]  

 
@r4=op2

@r0=dvdx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dudx
@r6=free     
@r7=free               
@r8=free
@r9=free
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable 

	@left_x = v2->x << 16;
	mov		r10, r6, asl #16
	@str		r6, [sp, #LEFT_X+32]

	add		sp, sp, #16
	ldmfd	sp, {r6-r9} @v2 & v3 uv's 
	sub		sp, sp, #16
 


@r6=v2->u     
@r7=v2->v              
@r8=v3->u
@r9=v3->v
  
	@op1 = ((v3->u - v2->u));  
	@delta_left_u = (op1 * op2)>>6;		  				
	sub		r8, r8, r6
	mul		r8, r4, r8
	mov		r8, r8, asr #6
	str		r8, [sp, #DELTA_LEFT_U+32] 
 
@r0=dvdx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dudx
@r6=free     
@r7=free               
@r8=free
@r9=free
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable

	@left_u = v2->u << 10;
	mov		r1, r6, asl #10
	@str		r6, [sp, #LEFT_U+32]


	@op1 = ((v3->v - v2->v));
	@delta_left_v = (op1 * op2)>>6;
	sub		r9, r9, r7
	mul		r9, r4, r9
	mov		r9, r9, asr #6
	str		r9, [sp, #DELTA_LEFT_V+32]


	@left_v = v2->v << 10;
	mov		r2, r7, asl #10
	@str		r7, [sp, #LEFT_V+32]

@r0=dvdx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dudx
@r6=free     
@r7=free               
@r8=free
@r9=free
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable

@----------------------------------------
@ Interpolate along the right edge of 
@ the triangle
@----------------------------------------
	@if(--right_section_height <= 0) // At the bottom of this section?
	@	return
	ldr		r4, [sp, #RIGHT_SECTION_HEIGHT+32]
	subs	r4, r4, #1

	ldmlefd sp!,{r6-r9}
 	ldmlefd sp!,{r6-r9} 
	ble		ret

	str		r4, [sp, #RIGHT_SECTION_HEIGHT+32]


	@right_x += delta_right_x;
	ldr		r4, [sp, #DELTA_RIGHT_X+32]
	add		r11, r11, r4  


	b		outterpos

notbotpos:
@r0=dvdx
@r1=left_u
@r2=left_v     
@r3=screen       
@r4=free   
@r5=dudx  
@r6=free     
@r7=free               
@r8=dudv
@r9=uv
@r10=left_x
@r11=right_x  
@r12=destptr	 
@lr=PrjTable

	add		r4, sp, #RIGHT_SECTION_HEIGHT+32
	ldmfd	r4, {r4,r6,r7,r8, r9}	
	

	@left_x += delta_left_x;
	@ldr		r4, [sp, #DELTA_LEFT_X+32]
	add		r10, r10, r6

	@left_u += delta_left_u;
	@ldr		r4, [sp, #DELTA_LEFT_U+32]
	add		r1, r1, r7

	@left_v += delta_left_v;  
	@ldr		r4, [sp, #DELTA_LEFT_V+32] 
	add		r2, r2, r8 

	

@----------------------------------------
@ Interpolate along the right edge of 
@ the triangle
@----------------------------------------	
	@if(--right_section_height <= 0) // At the bottom of this section?
	@	return
	@ldr		r4, [sp, #RIGHT_SECTION_HEIGHT+32]
 	subs	r4, r4, #1

	ldmlefd	sp!, {r6-r9}  
	ldmlefd	sp!, {r6-r9}
	ble		ret

	 
	str		r4, [sp, #RIGHT_SECTION_HEIGHT+32]
 
	@right_x += delta_right_x;
	@ldr		r4, [sp, #DELTA_RIGHT_X+32]
	add		r11, r11, r9
  
@  PRINT AND RETURN    
@	mov		r0, r11
@	ldr		r1, =szp     	        
@	mov		r2, #10         
@	ldr		r4, =Font_itoa+1       
@	mov		lr, pc   
@	bx		r4	  	    
@	ldmfd sp!,{r6-r9} 
@	ldmfd sp!,{r6-r9}    
@ 	b		ret  


	b		outterpos

ret:
	add		sp, sp, #60	 
	ldmfd	sp!, {r7-r9,lr}
	mov		pc, lr

	.pool	
	  
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@	void inner(u32 dest,u32 *bitmap, u32 uv,u32 dudv,u32 width)
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
inner:	
@ r0 = dest
@ r1 = bitmap
@ r2 = uv
@ r3 = dudv
@ r4 = width
@mov		r0, r7
@ldr		r1, =levelTmap  
@mov		r2, r9    
@mov		r3, r8 
@mov		r4, r6
 
@r4=free
@r0=free 

	@cmp		r6, #1 
	@beq		inner1pixel
	 
	tst		r7, #1
@	addeq	r6, r6, #1
	beq		anyleftovers 
	
@not aligned  

	and		r4,r9,#0xff00 	 
	add		r4,r4,r9, lsr #24  	   
	ldrb	r4,[r1,r4]      
	 
	bic		r7,r7,#1  
	ldrh	r0,[r7] 
	bic		r0,r0,#0xff00 
	orr		r0,r0,r4,lsl#8    

	strh	r0,[r7],#2
	  
	add		r9,r9,r8	@ uv+=dudv 

	subs	r6, r6, #1 
	@ble		innerfinished
	movle	pc, lr

anyleftovers:
	movs	r6, r6, lsr #1

inloop:	
 
	rsb	r6,r6,#120
	mov	r4,r6,lsl #5
	add	r4,r4,r6,lsl #3
	
	add	pc,pc,r4
	nop
	@nop 
	
	.REPT	120		
	and		r6,r9,#0xff00 	
	add		r6,r6,r9, lsr #24  	       
	ldrb	r6,[r1,r6]      	 	
	add		r9,r9,r8	@ uv+=dudv	
	and		r4,r9,#0xff00 
	add		r4,r4,r9, lsr #24  
	ldrb	r4,[r1,r4] 
	add		r9,r9,r8	@ uv+=dudv  
	orr		r6,r6,r4,lsl #8 	
	strh	r6,[r7],#2 		
	.ENDR 

	@bcc		innerfinished
	movcc	pc, lr

	and		r6,r9,#0xff00 	
	add		r6,r6,r9, lsr #24  	   
	ldrb	r6,[r1,r6] 
 
	ldrh	r4,[r7]
	bic		r4,r4,#0x00ff
	orr		r4,r4,r6@,lsr#8 
	strh	r4,[r7]  

	@ Done so exit
	mov		pc, lr


innerfinished:

	@ Done so exit
	mov		pc, lr		 

inner1pixel:

	and		r6,r9,#0xff00 	
	add		r6,r6,r9, lsr #24  	   
	ldrb	r6,[r1,r6]

	tst		r7, #1
	bne		noalign	

	@aligned
	ldrh	r4,[r7]
	bic		r4,r4,#0x00ff
	orr		r4,r4,r6
	
	strh	r4,[r7]

	@ Done so exit
	mov		pc, lr

noalign:
	@not aligned
	bic		r7,r7,#1
	ldrh	r4,[r7]
	bic		r4,r4,#0xff00
	orr		r4,r4,r6,lsl#8
	
	strh	r4,[r7]

	@ Done so exit
	mov		pc, lr	

	.pool


	.END

@  PRINT AND RETURN
@	mov		r0, r4 
@	ldr		r1, =szp     	  
@	mov		r2, #10      
@	ldr		r4, =Font_itoa+1     
@	mov		lr, pc 
@	bx		r4	
@	ldmfd	sp!,{r6-r9}
@	b		ret 	
