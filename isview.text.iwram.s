@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	u32 IsView(polygon_type *pPolygons, polygon_type *pPolyList, u32 numPolys)
@
@	return count in r0
@
@
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@
@	DEFINES			@
@@@@@@@@@@@@@@@@@@@@@	
ZMIN			=100
ZMAX			=4000
POLYGON_OFFSET	=56@48@72@88@64@52@60@52@44@36@24@32
DISTANCE_OFFSET	=14@16 
CENTER_X		=120
CENTER_Y		=80 


		.section	.iwram,"ax",%progbits 
		.ARM
        .ALIGN
		.EXTERN PrjTable 
		.EXTERN TriFillARM 
		.EXTERN VideoBuffer   
        .GLOBAL IsView
		.TEXT  

IsView:        
 
@ r0 = pObjPolygons
@ r1 = pPolyList
@ r2 = numObjPolys

		
		stmfd   sp!,{r0-r12,lr}		@ save stack 

		
numObjPolys:		@ for (ipoly=0; ipoly<numObjPolys; ipoly++)
		
 
		ldmia	r0,{r3,r4,r5}		@ get the 3 vertex pointers for the polygon


		@ get ax,ay,az for vertex[0]
		@ldmia	r3,{r6,r7,r8}
		ldrsh	r6,[r3]
		ldrsh	r7,[r3,#2] 
		ldrsh	r8,[r3,#4]
		
		@ if az not in front and not too far from view plane then don't bother going any further with this polyogn
		cmp		r8, #ZMIN
		ble		rejectPoly
		cmp		r8, #ZMAX
		bge		rejectPoly

		@ get ax,ay,az for vertex[1] 
		@ldmia	r4,{r9,r10,r11}
		ldrsh	r9,[r4] 
		ldrsh	r10,[r4,#2] 
		ldrsh	r11,[r4,#4]
		
		@ if az not in front and not too far from view plane then don't bother going any further with this polyogn
		cmp		r11, #ZMIN
		ble		rejectPoly
		cmp		r11, #ZMAX 
		bge		rejectPoly 

		@ get ax,ay,az for vertex[2]
		@ldmia	r5,{r5,r12,lr}
		ldrsh	lr,[r5,#4]
		ldrsh	r12,[r5,#2]
		ldrsh	r5,[r5]
		
		@ if az not in front and not too far from view plane then don't bother going any further with this polyogn
		cmp		lr, #ZMIN
		ble		rejectPoly
		cmp		lr, #ZMAX
		bge		rejectPoly
				
		

@r0 = pObjPolygons
@r1 = pPolyList
@r2 = numObjPolys
@r3 = vertex[0] = free
@r4 = vertex[1] = free
@r5 = ax3
@r6 = ax1
@r7 = ay1
@r8 = az1
@r9 = ax2
@r10 = ay2
@r11 = az2
@r12 = ay3
@lr = az3

		@r3 = z1+z2+z3 = A
		add	r3, r8, r11
		add	r3, r3, lr


@		ldr		r8, [r0, #BACKFACE_OFFSET]
@		tst		r8, #0
@		beq		noBackFace
		@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@	Backface the polygon	
		@@@@@@@@@@@@@@@@@@@@@@@@@@@	
		@visible = (ax2-ax1)*(ay3-ay2);
		@visible-= (ay2-ay1)*(ax3-ax2);
		@if (visible < 0) DRAW IT!
						
		sub		r8, r9, r6 
		sub		r11, r12, r10
		mul		r8, r11, r8

		sub		r11, r10, r7
		sub		lr, r5, r9
		mul		r11, lr, r11

		subs	r8, r8, r11 
		bge		rejectPoly  

@noBackFace:
		
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@	Discard full out of view polygons X Y
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		
		@ X < 0
		mov		r8, #0
		
		cmp		r6, #0
		addle	r8,r8,#1
		cmp		r9, #0
		addle	r8,r8,#1
		cmp		r5, #0
		addle	r8,r8,#1

		cmp		r8, #3
		bge		rejectPoly

		@ X > 239
		mov		r8, #0
		
		cmp		r6, #239
		addge	r8,r8,#1
		cmp		r9, #239
		addge	r8,r8,#1
		cmp		r5, #239
		addge	r8,r8,#1

		cmp		r8, #3
		bge		rejectPoly

		@ Y < 0
		mov		r8, #0
		
		cmp		r7, #0
		addle	r8,r8,#1
		cmp		r10, #0
		addle	r8,r8,#1
		cmp		r12, #0
		addle	r8,r8,#1

		cmp		r8, #3
		bge		rejectPoly

		@ Y > 159
		mov		r8, #0
		
		cmp		r7, #159
		addge	r8,r8,#1
		cmp		r10, #159
		addge	r8,r8,#1
		cmp		r12, #159
		addge	r8,r8,#1

		cmp		r8, #3
		bge		rejectPoly

		@ is viewed 
		@mov r8, #1
		@strb r8, [r0, #ISVIEW_OFFSET]
		@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@	Store sx and sy
		@@@@@@@@@@@@@@@@@@@@@@@@@@@
		ldmia	r0,{r8,r11,lr}		@ get the 3 vertex pointers for the polygon
		
		@add		r8, r8, #18@#24@#30
		@add		r11, r11, #18@#24@#30
		@add		lr, lr, #18@#24@#30

		@stmia	r8, {r6,r7}  
		strh	r6,[r8,#18]
		strh	r7,[r8,#20]
 		@stmia	r11, {r9,r10} 
		strh	r9,[r11,#18]
		strh	r10,[r11,#20]
		@stmia 	lr, {r5,r12}
		strh	r5,[lr,#18]
		strh	r12,[lr,#20]

		 
@r0 = pObjPolygons
@r1 = pPolyList
@r2 = numObjPolys
@r3 = vertex[0] = free
@r4 = vertex[1] = free
@r5 = projected ax3
@r6 = projected ax1
@r7 = projected ay1
@r8 = az1
@r9 = projected ax2
@r10 = projected ay2 
@r11 = az2
@r12 = projected ay3
@lr = az3
		@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@	calculate distance of polygon	
		@@@@@@@@@@@@@@@@@@@@@@@@@@@	
	
		@/* approximate A/3 */
        @Q = ((A >>  2) + A) >> 2; /* Q = A*0.0101 */
        @Q = ((Q >>  0) + A) >> 2; /* Q = A*0.010101 */
        @Q = ((Q >>  0) + A) >> 2; /* Q = A*0.01010101 */
        @Q = ((Q >>  0) + A) >> 2; /* Q = A*0.0101010101 */
        @Q = ((Q >>  0) + A) >> 1; /* Q = A*0.10101010101 */
        @Q = ((Q >> 12) + Q) >> 0; /* Q = A*0.10101010101010101010101 */
        @Q = ((Q >> 24) + Q) >> 1; /* Q = A*0.010101010101001010101010101 ... */
        @/* either Q = A/3 or Q+1 = A/3 for all 32-bit unsigned A */
		@return Q+1;
		
		@r4 = A >> 2
		@mov		r4, r3, asr #2
		@add		r4, r4, r3
		@mov		r4, r4, asr #2  

		@r4 = Q = ((A >>  2) + A) >> 2; /* Q = A*0.0101 */

		@add		r4, r4, r3	
		@mov		r4, r4, asr #2

		@r4 = Q = ((Q >>  0) + A) >> 2; /* Q = A*0.010101 */

		@add		r4, r4, r3	
		@mov		r4, r4, asr #2

		@r4 = Q = ((Q >>  0) + A) >> 2; /* Q = A*0.01010101 */

		@add		r4, r4, r3	
		@mov		r4, r4, asr #2

		@r4 = Q = ((Q >>  0) + A) >> 2; /* Q = A*0.0101010101 */

		@add		r4, r4, r3	
		@mov		r4, r4, asr #1
		
		@r4 = Q = ((Q >>  0) + A) >> 1; /* Q = A*0.10101010101 */

		@add		r4, r4, r4, asr #12

		@r4 = Q = ((Q >> 12) + Q) >> 0; /* Q = A*0.10101010101010101010101 */

		@add		r4, r4, r4, asr #24
		@mov		r4, r4, asr #1
		@add		r4, r4, #1 
		
		
		@ r4 = distance}
		
		@ store distance in polygon[ipoly].distance for use in zsorting later
		mov		r4,r3,lsr #2
		strh	r4, [r0, #DISTANCE_OFFSET]
		

		

 
@r0 = pObjPolygons
@r1 = pPolyList
@r2 = numObjPolys 
@r3 = vertex[0] = free
@r4 = vertex[1] = free
@r5 = projected ax3
@r6 = projected ax1
@r7 = projected ay1
@r8 = az1
@r9 = projected ax2
@r10 = projected ay2
@r11 = az2
@r12 = projected ay3
@lr = az3


nextPoly:
				
		str	r0, [r1],#4

		add	r0, r0, #POLYGON_OFFSET		@ incremenet polygon pointer to next polygon
		
		subs	r2, r2, #1		
		bgt		numObjPolys
		
		mov		lr, r1
		ldmfd   sp!,{r0-r12}		@ restore stack
		sub		r0, lr, r1
		mov		r0, r0, asr #2

		ldmfd   sp!,{lr}			@ restore stack
		
		@ Return count in r0
		@bx lr
		mov		pc, lr

rejectPoly:
		@not in view
		@mov r8, #0
		@strb r8, [r0, #ISVIEW_OFFSET]
		
		add	r0, r0, #POLYGON_OFFSET		@ incremenet polygon pointer to next polygon

		subs	r2, r2, #1		
		bgt		numObjPolys
  
		mov		lr, r1
		ldmfd   sp!,{r0-r12}		@ restore stack
		
		sub		r0, lr, r1
		mov		r0, r0, asr #2

		ldmfd   sp!,{lr}			@ restore stack

		@ Return count in r0
		@bx lr
		mov		pc, lr

		
		.pool
		
		.END
