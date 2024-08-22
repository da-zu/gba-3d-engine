@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void ViewAlign(object_type *pObj, view_type *pView)
@
@	NOTE:
@			Should unroll the loop so as to save on ldr'ing curview.matrix etc etc
@		
@
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@
@	DEFINES			@
@@@@@@@@@@@@@@@@@@@@@	
CENTER_X				=120
CENTER_Y				=80
NUMVERTS_OFFSET			=0
OBJECT_VERTEX_OFFSET	=28@92
VERTEX_OFFSET			=24@28
VERTEX_AX				=0
VERTEX_AY				=2
VERTEX_AZ				=4 
VERTEX_WX				=12
VERTEX_WY				=14
VERTEX_WZ				=16
NUMSHIFTS				=8

		.section	.iwram,"ax",%progbits 
		.ARM
        .ALIGN 
		.EXTERN PrjTable24 
		.EXTERN Font_itoa
		.EXTERN Gba_Print
		.EXTERN VideoBuffer
		.EXTERN szp
        .GLOBAL ViewAlign
		.TEXT 

ViewAlign:         
@r0 = pObj
@r1 = pView 
   
		stmfd   sp!,{r0-r12,lr}				@ save stack 

		
		ldrh	r2, [r0, #NUMVERTS_OFFSET]	@ get number of object vertices

		
		ldr		r0, [r0, #OBJECT_VERTEX_OFFSET]@ get pointer to vertex array in object_type
		@add		r0, r0, #OBJECT_VERTEX_OFFSET


		ldr		lr, =PrjTable24 
 
		
@r0 = pObj->pVertexList
@r2 = numObjVerts 		 
numObjVerts:	@for ( i=0; i<pObj->number_of_vertices; i++) 

		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Align vertex with view													
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		ldrsh	r3, [r0, #VERTEX_WX]		@ wx = world.object[iobj].vertex[iobjvert].wx;
		ldrsh	r4, [r0, #VERTEX_WY]		@ wy = world.object[iobj].vertex[iobjvert].wy;
		ldrsh	r5, [r0, #VERTEX_WZ]		@ wz = world.object[iobj].vertex[iobjvert].wz;
		
		 

		@-------------------------------------------------
		@ Calculate new aligned x coordinate:			
		@
		@ world.object[iobj].vertex[iobjvert].ax=
		@			(wx*curView->matrix[0][0]
		@			+wy*curView->matrix[1][0]
		@			+wz*curView->matrix[2][0]
		@			+curView->matrix[3][0])>>NUM_SHIFTS;
		@-------------------------------------------------    
		ldr		r6, [r1]					@ curView->matrix[0][0]
		ldr		r7, [r1,#16]				@ curView->matrix[1][0]
		ldr		r8, [r1,#32]  				@ curView->matrix[2][0]
		ldr		r9, [r1,#48]				@ curView->matrix[3][0]

		mla		r10, r3, r6,r9 
		mla		r10, r4, r7, r10
		mla		r10, r5, r8, r10 
		@add		r10, r10, r9
		mov		r10, r10, asr #NUMSHIFTS
 
@ r10 = ax
		
		@-------------------------------------------------
		@ Calculate new aligned y coordinate:				
		@
		@ world.object[iobj].vertex[iobjvert].ay=
		@			(wx*curView->matrix[0][1]
		@			+wy*curView->matrix[1][1]
		@			+wz*curView->matrix[2][1]
		@			+curView->matrix[3][1])>>NUM_SHIFTS;
		@-------------------------------------------------
		ldr		r6, [r1,#4]					@ curView->matrix[0][1]
		ldr		r7, [r1,#20]				@ curView->matrix[1][1]
		ldr		r8, [r1,#36]				@ curView->matrix[2][1]
		ldr		r9, [r1,#52]		 		@ curView->matrix[3][1]

		mla		r11, r3, r6, r9 
		mla		r11, r4, r7, r11
		mla		r11, r5, r8, r11
		@add		r11, r11, r9
		mov		r11, r11, asr #NUMSHIFTS

@ r11 = ay

		@-------------------------------------------------
		@ Calculate new aligned z coordinate:		
		@		
		@		world.object[iobj].vertex[iobjvert].az=
		@			(wx*curView->matrix[0][2]
		@			+wy*curView->matrix[1][2]
		@			+wz*curView->matrix[2][2]
		@			+curView->matrix[3][2])>>NUM_SHIFTS;
		@-------------------------------------------------
		ldr		r6, [r1,#8]					@ curView->matrix[0][2]
		ldr		r7, [r1,#24]				@ curView->matrix[1][2]
		ldr		r8, [r1,#40]				@ curView->matrix[2][2]
		ldr		r9, [r1,#56]				@ curView->matrix[3][2]

		mla		r12, r3, r6,r9
		mla		r12, r4, r7, r12
		mla		r12, r5, r8, r12
		@add		r12, r12, r9
		mov		r12, r12, asr #NUMSHIFTS

@ r12 = az


@ r0 = pObj
@ r1 = pVertex
@ r2 = numObjVerts
@ r3 = wx = free
@ r4 = wy = free
@ r5 = wz = free
@ r6 = curView->matrix[0][2] = free
@ r7 = curView->matrix[1][2] = free
@ r8 = curView->matrix[2][2] = free
@ r9 = curView->matrix[3][2] = free
@ r10 = ax
@ r11 = ay
@ r12 = az
@ sp = free
@ lr = free

		@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@	Project the vertex
		@@@@@@@@@@@@@@@@@@@@@@@@@@@	

		
		@-------------------------
		@ vertex0
		@ r10 = ax1/az1		
		@-------------------------
		ldr		r3, [lr, r12, asl #2]
		@mov		r10, r10, asl #8		
		mul		r10, r3, r10
		mov		r10, r10, asr #16 
		add		r10, r10, #CENTER_X


		@-------------------------
		@ r11 = ay1/az1		
		@-------------------------
		@mov		r11, r11, asl #8		
		mul		r11, r3, r11 		
		mov		r11, r11, asr #16
		add		r11, r11, #CENTER_Y
		
		@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@	Store ax,ay,az
		@@@@@@@@@@@@@@@@@@@@@@@@@@@   
		strh	r10, [r0, #VERTEX_AX] 
		strh	r11, [r0, #VERTEX_AY]
		strh	r12, [r0, #VERTEX_AZ]
		
		
		@ LAST store or load increment r0 by VERTEX_OFFSET
		add		r0, r0, #VERTEX_OFFSET

		
 
		subs	r2,r2,#1
		bgt		numObjVerts 


		ldmfd   sp!,{r0-r12,lr}				@ restore stack

		mov		pc,lr

		.pool

		.END 
