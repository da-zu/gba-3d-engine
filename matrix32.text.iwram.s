@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void inittrans32(void)
@
@	NOTE:
@			
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@
@	DEFINES			@
@@@@@@@@@@@@@@@@@@@@@	
		
NUM_SHIFTS				=8
ONE						=256		@#define ONE	((FIXED)1<<NUM_SHIFTS)

OBJECT_VERTEX_OFFSET	=28@92
VERTEX_OFFSET			=24@28

VERTEX_LX				=6
VERTEX_LY				=8
VERTEX_LZ				=10 

VERTEX_WX				=12
VERTEX_WY				=14 
VERTEX_WZ				=16
 
NUMVERTS_OFFSET			=0
NUMPOLYS_OFFSET			=2
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		.section	.iwram,"ax",%progbits		
		.ARM
        .ALIGN				 
		.GLOBAL matmult32
        .GLOBAL inittrans32 
		.GLOBAL rotate32
		.GLOBAL translate32
		.GLOBAL scale32
		.GLOBAL transform32
		.GLOBAL inittrans_view32
		.GLOBAL setcam32
		.EXTERN  matrix	
		.EXTERN cos_table
		.EXTERN sin_table
		.EXTERN fixedpointasm_div
		.TEXT	

	
inittrans32:  
		stmfd sp!, {lr}
		
		ldr		lr, =matrix

		mov		r0, #ONE
		mov		r1, #0			@ ONE (fixed Point)
		mov		r2, #0			@ 0
		mov		r3, #0			@ 0
		 		
		stmia	lr!, {r0-r3}
		mov		r1, #ONE
		mov		r0, #0

		stmia	lr!, {r0-r3}
		mov		r2, #ONE
		mov		r1, #0

		stmia	lr!, {r0-r3}
		mov		r3, #ONE
		mov		r2, #0

		stmia	lr, {r0-r3}
		
		
		ldmfd	sp!, {lr}		
		bx		lr

		.pool


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void inittrans_view32(view_type *pView)
@
@	NOTE:
@			
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
inittrans_view32:
		
		stmfd	sp!,{r0}

		@Initialize master transformation matrix to the identity matrix
		@view->matrix[0][0]=ONE; view->matrix[0][1]=0;   view->matrix[0][2]=0;   view->matrix[0][3]=0;
		@view->matrix[1][0]=0;   view->matrix[1][1]=ONE; view->matrix[1][2]=0;   view->matrix[1][3]=0;
		@view->matrix[2][0]=0;   view->matrix[2][1]=0;   view->matrix[2][2]=ONE; view->matrix[2][3]=0;
		@view->matrix[3][0]=0;   view->matrix[3][1]=0;   view->matrix[3][2]=0;   view->matrix[3][3]=ONE;

		mov		r1, #ONE
		mov		r2, #0			@ ONE (fixed Point)
		mov		r3, #0			@ 0
		mov		r4, #0			@ 0
		 		
		stmia	r0!, {r1-r4}
		mov		r2, #ONE
		mov		r1, #0

		stmia	r0!, {r1-r4}
		mov		r3, #ONE
		mov		r2, #0

		stmia	r0!, {r1-r4}
		mov		r4, #ONE
		mov		r3, #0

		stmia	r0, {r1-r4}

		ldmfd	sp!,{r0}

		bx		lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void rotate32(s32 ax, s32 ay, s32 az)
@
@	NOTE:
@			
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ r0 = ax
@ r1 = ay
@ r2 = az
rotate32:
		stmfd sp!, {r0-r12,lr}
		
		sub		sp, sp, #192

		@ABS(ax)&255
		cmp		r0, #0
		rsblt	r0, r0, #0
		and		r0, r0, #255

		@ABS(ay)&255
		cmp		r1, #0
		rsblt	r1, r1, #0
		and		r1, r1, #255

		@ABS(az)&255
		cmp		r2, #0
		rsblt	r2, r2, #0
		and		r2, r2, #255
		
		ldr		r3, =sin_table
		ldr		r4, =cos_table

		@------------------------------
		@ Initialize X rotation matrix:
		@------------------------------
		@rotmat[0][0]=ONE;  rotmat[0][1]=0;        rotmat[0][2]=0;       rotmat[0][3]=0;
		@rotmat[1][0]=0;    rotmat[1][1]=COS(ax);  rotmat[1][2]=SIN(ax); rotmat[1][3]=0;
		@rotmat[2][0]=0;    rotmat[2][1]=-SIN(ax); rotmat[2][2]=COS(ax); rotmat[2][3]=0;
		@rotmat[3][0]=0;    rotmat[3][1]=0;        rotmat[3][2]=0;       rotmat[3][3]=ONE;

		mov		r5, #ONE
		mov		r6, #0
		mov		r7, #0
		mov		r8, #0
		mov		r9, #0
		ldr		r10, [r4, r0, lsl #2]	@COS(ax)
		ldr		r11, [r3, r0, lsl #2]	@SIN(ax)	
		mov		r12, #0

		stmia	sp!,{r5-r12}

		mov		r5, #0
		rsb		r6, r11, #0
		mov		r7, r10
		mov		r10, #0
		mov		r11, #0
		mov		r12, #ONE
		
		stmia	sp!,{r5-r12}

		mov		r5, r0
		mov		r7, r1
		mov		r10, r2
		
		@ Concatenate this matrix with yrot matrix:
		@ matmult(mat1,matrix,rotmat);
		mov		r0, sp
		ldr		r1, =matrix
		sub		sp, sp, #64
		mov		r2, sp

		bl		matmult32

		mov		r0, r5
		mov		r1, r7
		mov		r2, r10
		@------------------------------
		@ Initialize Y rotation matrix:
		@------------------------------
		@rotmat[0][0]=COS(ay); rotmat[0][1]=0;   rotmat[0][2]=-SIN(ay); rotmat[0][3]=0;
		@rotmat[1][0]=0;       rotmat[1][1]=ONE; rotmat[1][2]=0;        rotmat[1][3]=0;
		@rotmat[2][0]=SIN(ay); rotmat[2][1]=0;   rotmat[2][2]=COS(ay);  rotmat[2][3]=0;
		@rotmat[3][0]=0;       rotmat[3][1]=0;   rotmat[3][2]=0;        rotmat[3][3]=ONE;

		ldr		r5, [r4, r1, lsl #2]		@ COS(ay)
		mov		r6, #0
		ldr		lr, [r3, r1, lsl #2]		@ lr = SIN(ay)
		rsb		r7, lr, #0					@ -SIN(ay)
		mov		r8, #0
		mov		r9, #0
		mov		r10, #ONE
		mov		r11, #0
		mov		r12, #0

		stmia	sp!,{r5-r12}

		mov		r7, r5
		mov		r5, lr
		mov		r10, #0
		mov		r12, #ONE
		
		stmia	sp!,{r5-r12}

		mov		r5, r0
		mov		r7, r1
		mov		r10, r2
		
		@ Concatenate this matrix with master matrix:
		@ matmult(mat2,mat1,rotmat);
		add		r0, sp, #64
		mov		r1, sp		
		sub		sp, sp, #64
		mov		r2, sp

		bl		matmult32

		mov		r0, r5
		mov		r1, r7
		mov		r2, r10		

		@------------------------------
		@ Initialize Z rotation matrix:
		@------------------------------
		@rotmat[0][0]=COS(az);  rotmat[0][1]=SIN(az);  rotmat[0][2]=0;    rotmat[0][3]=0;
		@rotmat[1][0]=-SIN(az); rotmat[1][1]=COS(az);  rotmat[1][2]=0;    rotmat[1][3]=0;
		@rotmat[2][0]=0;        rotmat[2][1]=0;        rotmat[2][2]=ONE;  rotmat[2][3]=0;
		@rotmat[3][0]=0;        rotmat[3][1]=0;        rotmat[3][2]=0;    rotmat[3][3]=ONE;


		ldr		r5, [r4, r2, lsl #2]	@COS(az)
		ldr		r6, [r3, r2, lsl #2]	@SIN(az)			
		mov		r7, #0
		mov		r8, #0
		rsb		r9, r6, #0
		mov		r10, r5
		mov		r11, #0 
		mov		r12, #0
				
		stmia	sp!,{r5-r12}

		mov		r5, #0
		mov		r6, #0
		mov		r7, #ONE
		@mov		r8, #0
		mov		r9, #0
		mov		r10, #0
		@mov		r11, #0
		mov		r12, #ONE
		
		stmia	sp!,{r5-r12}
		
		
		@ Concatenate this matrix with yrot matrix:
		@ matmult(matrix,mat2,rotmat);
		ldr		r0, =matrix 
		add		r1, sp, #64
		sub		sp, sp, #64
		mov		r2, sp

		bl		matmult32

		
		add		sp, sp, #192

		ldmfd sp!, {r0-r12,lr}		
		bx lr
		
		.pool

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void translate32( s32 xt, s32 yt, s32 zt)
@
@	NOTE:
@			
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@r0=xt
@r1=yt
@r2=zt

translate32:

	stmfd sp!, {r0-r7,lr}
	
	sub		sp, sp, #64
	mov		r3, sp

	@ Create translation matrix

	mov		r4, #ONE
	mov		r5, #0			
	mov		r6, #0			
	mov		r7, #0				 		
	stmia	r3!, {r4-r7}

	mov		r4, #0
	mov		r5, #ONE
	stmia	r3!, {r4-r7}

	mov		r5, #0
	mov		r6, #ONE
	stmia	r3!, {r4-r7} 

	mov		r4, r0, asl #NUM_SHIFTS
	mov		r5, r1, asl #NUM_SHIFTS
	mov		r6, r2, asl #NUM_SHIFTS
	mov		r7, #ONE	
	stmia	r3!, {r4-r7}
  
	@ Concatenate with master matrix:
	@ matmult32 (matrix,matrix,tmat);

	ldr		r0, =matrix
	mov		r1, r0
	sub		r2, r3, #64 
	
	bl		matmult32	

	add		sp, sp, #64

	ldmfd sp!, {r0-r7,lr}

	bx		lr

	.pool

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void scale32( s32 sx, s32 sy, s32 sz)
@
@	NOTE:
@			
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@r0=sx
@r1=sy
@r2=sz

scale32:

	cmp		r0, #256
	cmpeq	r1, #256
	cmpeq	r2, #256
	bxeq	lr

	stmfd sp!, {r0-r12,lr} 
	
	sub		sp, sp, #64
	mov		lr, sp

	@Initialize scaling matrix:
	@smat[0][0] = (s32)xs; smat[0][1] = 0;	smat[0][2] = 0;		smat[0][3] = 0;
	@smat[1][0] = 0;		smat[1][1] = (s32)ys; smat[1][2] = 0;	smat[1][3] = 0;
	@smat[2][0] = 0;		smat[2][1] = 0;		smat[2][2] = (s32)zs; smat[2][3] = 0;
	@smat[3][0] = 0;		smat[3][1] = 0;		smat[3][2] = 0;		smat[3][3] = ONE;

	mov		r5, r1
	mov		r10, r2
	mov		r1, #0
	mov		r2, #0
	mov		r3, #0
	mov		r4, #0
	mov		r6, #0
	mov		r7, #0
	mov		r8, #0
	mov		r9, #0
	mov		r11, #0
	mov		r12, #0
	
	stmia	lr!,{r0-r12}
			
	mov		r9, #ONE

	stmia	lr!,{r7-r9}

	@Concatenate with master matrix:
	@matmult(matrix,smat,matrix);
	ldr		r0, =matrix
	mov		r2, r0
	sub		r1, lr, #64

	bl		matmult32

	add		sp, sp, #64
	
	ldmfd	sp!, {r0-r12,lr}

	bx		lr

	.pool
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void transform32(object_type *pObject)
@
@	NOTE:
@			
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@ r0 = pObject
transform32:
		stmfd sp!, {r0-r12,lr} 
		
		@ get the global matrix instance
		ldr		lr, =matrix
		ldmia	lr,{r1,r2,r3}
		add		lr, lr, #16
		ldmia	lr,{r4,r5,r6}
		add		lr, lr, #16
		ldmia	lr,{r7,r8,r9}
		add		lr, lr, #16
		
		ldrh	r12, [r0, #NUMVERTS_OFFSET]		

		@add		r0, r0, #OBJECT_VERTEX_OFFSET
		ldr		r0, [r0, #OBJECT_VERTEX_OFFSET]

@ lr = free		
numverts: 

		@----------------
		@ WX
		@----------------
		ldrsh	r10, [r0, #VERTEX_LX]
		mul		r10, r1, r10					@ object->vertex[v].lx*matrix[0][0]
		ldrsh	r11, [r0, #VERTEX_LY]
		mla		r10, r11, r4, r10				@ +object->vertex[v].ly*matrix[1][0]
		ldrsh	r11, [r0, #VERTEX_LZ]
		mla		r10, r11, r7, r10				@ +object->vertex[v].lz*matrix[2][0]
		ldr		r11, [lr]						
		add		r10, r10, r11					@ +matrix[3][0]
		mov		r10, r10, asr #NUM_SHIFTS

		strh	r10, [r0, #VERTEX_WX]		

		@----------------
		@ WY
		@----------------
		ldrsh	r10, [r0, #VERTEX_LX]
		mul		r10, r2, r10					@ object->vertex[v].lx*matrix[0][0]
		ldrsh	r11, [r0, #VERTEX_LY]
		mla		r10, r11, r5, r10				@ +object->vertex[v].ly*matrix[1][0]
		ldrsh	r11, [r0, #VERTEX_LZ]
		mla		r10, r11, r8, r10				@ +object->vertex[v].lz*matrix[2][0]
		ldr		r11, [lr,#4]						
		add		r10, r10, r11					@ +matrix[3][0]
		mov		r10, r10, asr #NUM_SHIFTS

		strh	r10, [r0, #VERTEX_WY]		

		@----------------
		@ WZ
		@----------------
		ldrsh	r10, [r0, #VERTEX_LX]
		mul		r10, r3, r10					@ object->vertex[v].lx*matrix[0][0]
		ldrsh	r11, [r0, #VERTEX_LY]
		mla		r10, r11, r6, r10				@ +object->vertex[v].ly*matrix[1][0]
		ldrsh	r11, [r0, #VERTEX_LZ]
		mla		r10, r11, r9, r10				@ +object->vertex[v].lz*matrix[2][0]
		ldr		r11, [lr,#8]						
		add		r10, r10, r11					@ +matrix[3][0]
		mov		r10, r10, asr #NUM_SHIFTS

		strh	r10, [r0, #VERTEX_WZ]		

		
		@ LAST store or load increment r0 by VERTEX_OFFSET
		add		r0, r0, #VERTEX_OFFSET

		subs	r12, r12, #1
		bgt		numverts
		

		@ EXIT		
		ldmfd sp!, {r0-r12,lr}		
		bx lr
		
		.pool


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void matmult32( s32 *dest_mat[4][4], s32 *mat1[4][4], s32 *mat2[4][4])
@
@	NOTE:
@			mat[3][3] is always #ONE
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@r0=dest_mat
@r1=mat1
@r2=mat2

matmult32:
		
		
		stmfd	sp!,{r0-r12,lr}
		
		@-------------------
		@ Loop i=0, j=0,1,2,3
		@-------------------
		ldmia	r1!, {r3,r4,r5}
		
		mov		lr, r2
		ldmia	lr, {r6,r7}
		add		lr, lr, #16
		ldmia	lr, {r8,r9}
		add		lr, lr, #16
		ldmia	lr, {r10,r11}
		add		lr, lr, #16
		ldmia	lr, {r12,lr}
		
		mul		r6, r3, r6
		mla		r6, r4, r8, r6
		mla		r6, r5, r10, r6
		ldr		r8, [r1]
		mla		r6, r8, r12, r6

@r6=result[0][0]

		mul		r7, r3, r7
		mla		r7, r4, r9, r7
		mla		r7, r5, r11, r7
		mla		r7, r8, lr, r7

@r7=result[0][1]
		mov		r6, r6, asr #NUM_SHIFTS
		mov		r7, r7, asr #NUM_SHIFTS
		stmia	r0!,{r6,r7}

		add		lr, r2, #8
		ldmia	lr, {r6,r7}
		add		lr, lr, #16
		ldmia	lr, {r8,r9}
		add		lr, lr, #16
		ldmia	lr, {r10,r11}
		add		lr, lr, #16
		ldmia	lr, {r12,lr}

		mul		r6, r3, r6
		mla		r6, r4, r8, r6
		mla		r6, r5, r10, r6
		ldr		r8, [r1],#4
		mla		r6, r8, r12, r6

@r6=result[0][2]

		mul		r7, r3, r7
		mla		r7, r4, r9, r7
		mla		r7, r5, r11, r7
		mla		r7, r8, lr, r7

@r7=result[0][3]
		mov		r6, r6, asr #NUM_SHIFTS
		mov		r7, r7, asr #NUM_SHIFTS
		stmia	r0!,{r6,r7}

		@-------------------
		@ Loop i=1, j=0,1,2,3
		@-------------------
		ldmia	r1!, {r3,r4,r5}
		
		mov		lr, r2
		ldmia	lr, {r6,r7}
		add		lr, lr, #16
		ldmia	lr, {r8,r9}
		add		lr, lr, #16
		ldmia	lr, {r10,r11}
		add		lr, lr, #16
		ldmia	lr, {r12,lr}
		
		mul		r6, r3, r6
		mla		r6, r4, r8, r6
		mla		r6, r5, r10, r6
		ldr		r8, [r1]
		mla		r6, r8, r12, r6

@r6=result[1][0]

		mul		r7, r3, r7
		mla		r7, r4, r9, r7
		mla		r7, r5, r11, r7
		mla		r7, r8, lr, r7

@r7=result[1][1]
		mov		r6, r6, asr #NUM_SHIFTS
		mov		r7, r7, asr #NUM_SHIFTS
		stmia	r0!,{r6,r7}

		add		lr, r2, #8
		ldmia	lr, {r6,r7}
		add		lr, lr, #16
		ldmia	lr, {r8,r9}
		add		lr, lr, #16
		ldmia	lr, {r10,r11}
		add		lr, lr, #16
		ldmia	lr, {r12,lr}

		mul		r6, r3, r6
		mla		r6, r4, r8, r6
		mla		r6, r5, r10, r6
		ldr		r8, [r1],#4
		mla		r6, r8, r12, r6

@r6=result[1][2]

		mul		r7, r3, r7
		mla		r7, r4, r9, r7
		mla		r7, r5, r11, r7
		mla		r7, r8, lr, r7

@r7=result[1][3]
		mov		r6, r6, asr #NUM_SHIFTS
		mov		r7, r7, asr #NUM_SHIFTS
		stmia	r0!,{r6,r7}
		

		@-------------------
		@ Loop i=2, j=0,1,2,3
		@-------------------
		ldmia	r1!, {r3,r4,r5}
		
		mov		lr, r2
		ldmia	lr, {r6,r7}
		add		lr, lr, #16
		ldmia	lr, {r8,r9}
		add		lr, lr, #16
		ldmia	lr, {r10,r11}
		add		lr, lr, #16
		ldmia	lr, {r12,lr}
		
		mul		r6, r3, r6
		mla		r6, r4, r8, r6
		mla		r6, r5, r10, r6
		ldr		r8, [r1]
		mla		r6, r8, r12, r6

@r6=result[2][0]

		mul		r7, r3, r7
		mla		r7, r4, r9, r7
		mla		r7, r5, r11, r7
		mla		r7, r8, lr, r7

@r7=result[2][1]
		mov		r6, r6, asr #NUM_SHIFTS
		mov		r7, r7, asr #NUM_SHIFTS
		stmia	r0!,{r6,r7}

		add		lr, r2, #8
		ldmia	lr, {r6,r7}
		add		lr, lr, #16
		ldmia	lr, {r8,r9}
		add		lr, lr, #16
		ldmia	lr, {r10,r11}
		add		lr, lr, #16
		ldmia	lr, {r12,lr}

		mul		r6, r3, r6
		mla		r6, r4, r8, r6
		mla		r6, r5, r10, r6
		ldr		r8, [r1],#4
		mla		r6, r8, r12, r6

@r6=result[2][2]

		mul		r7, r3, r7
		mla		r7, r4, r9, r7
		mla		r7, r5, r11, r7
		mla		r7, r8, lr, r7

@r7=result[2][3]
		mov		r6, r6, asr #NUM_SHIFTS
		mov		r7, r7, asr #NUM_SHIFTS
		stmia	r0!,{r6,r7}


		@-------------------
		@ Loop i=3, j=0,1,2,3
		@-------------------
		ldmia	r1!, {r3,r4,r5}
		
		mov		lr, r2
		ldmia	lr, {r6,r7}
		add		lr, lr, #16
		ldmia	lr, {r8,r9}
		add		lr, lr, #16
		ldmia	lr, {r10,r11}
		add		lr, lr, #16
		ldmia	lr, {r12,lr}
		
		mul		r6, r3, r6
		mla		r6, r4, r8, r6
		mla		r6, r5, r10, r6
		ldr		r8, [r1]
		mla		r6, r8, r12, r6

@r6=result[3][0]

		mul		r7, r3, r7
		mla		r7, r4, r9, r7
		mla		r7, r5, r11, r7
		mla		r7, r8, lr, r7

@r7=result[3][1]
		mov		r6, r6, asr #NUM_SHIFTS
		mov		r7, r7, asr #NUM_SHIFTS
		stmia	r0!,{r6,r7}

		add		lr, r2, #8
		ldmia	lr, {r6,r7}
		add		lr, lr, #16
		ldmia	lr, {r8,r9}
		add		lr, lr, #16
		ldmia	lr, {r10,r11}
		add		lr, lr, #16
		ldmia	lr, {r12,lr}

		mul		r6, r3, r6
		mla		r6, r4, r8, r6
		mla		r6, r5, r10, r6
		ldr		r8, [r1],#4
		mla		r6, r8, r12, r6

@r6=result[3][2]

		mul		r7, r3, r7
		mla		r7, r4, r9, r7
		mla		r7, r5, r11, r7
		mla		r7, r8, lr, r7

@r7=result[3][3]
		mov		r6, r6, asr #NUM_SHIFTS
		mov		r7, r7, asr #NUM_SHIFTS
		stmia	r0,{r6,r7}

		@ EXIT
		ldmfd	sp!,{r0-r12,lr} 

		mov		pc, lr

		.pool


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void setcam32(Vector3 *pLookAt, view_type *pView, s32 roll)
@
@	NOTE:
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@r0=pLookAt
@r1=pView
@r2=roll 

setcam32:

		stmfd	sp!,{r0-r8,lr} 
		
		@ABS(roll)&255
		cmp		r2, #0
		rsblt	r2, r2, #0
		and		r2, r2, #255

		@up.x = SIN(roll);  
		@up.y = -COS(roll); 
		@up.z = 0;
		ldr		r3, =sin_table
		ldr		r3, [r3, r2, lsl #2]
		mov		r5, #0
		ldr		r4, =cos_table
		ldr		r4, [r4, r2, lsl #2]
		rsb		r4, r4, #0
		
		sub		sp, sp, #12
		mov		r2, sp
@r2=up		

		@store up vector
		stmia	r2,{r3-r5}

		@ get pLookAt xyz
		ldmia	r0,{r3-r5}				

		@ get pView xyz
		ldmia	r1,{r6-r8}				

		@forward.x = (pLookAt->x - pView->copx)<<8;
		sub		r3, r3, r6
		mov		r3, r3, lsl #8

		@forward.y = (pLookAt->y - pView->copy)<<8;
		sub		r4, r4, r7
		mov		r4, r4, lsl #8

		@forward.z = (pLookAt->z - pView->copz)<<8;
		sub		r5, r5, r8
		mov		r5, r5, lsl #8
	
		sub		sp, sp, #12
		mov		r0, sp

		mov		r3, r1
		sub		sp, sp, #12
		mov		r1, sp

		@store forward vector
		stmia	r0,{r3-r5}

@r0=forward
@r1=right
@r2=up
@r3=pView

		@V3Normalize(&forward);
		bl		V3Normalize32
		
		@V3Cross(&up, &forward,&right);
		mov		r4, r0
		mov		r0, r2
		mov		r2, r1
		mov		r1, r4

		bl		V3Cross32
		
		@V3Normalize(&right);
		eor	r0,r0,r2	@ swap right and up
		eor	r2,r2,r0
		eor	r0,r0,r2

		bl		V3Normalize32

		@V3Cross(&right, &forward, &up); 
		bl		V3Cross32
 		
		@V3Normalize(&up);
		eor	r0,r0,r2	@ swap right and up
		eor	r2,r2,r0
		eor	r0,r0,r2

		bl		V3Normalize32

		@cammatrix(&up,&right,&forward, pView);
		eor	r1,r1,r2	@ swap right and forward 
		eor	r2,r2,r1
		eor	r1,r1,r2 
 
		bl		cammatrix32

		@ EXIT
		add		sp, sp, #36
		ldmfd	sp!,{r0-r8,lr} 

		bx		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void V3SquaredLength32(Vector3 *pVec3)
@
@	NOTE:
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@r0=pVec3

V3SquaredLength32:
		mov	ip, r0
	ldr	r2, [ip, #4]
	smull	r0, r1, r2, r2
	ldr	r3, [ip, #0]
	smlal	r0, r1, r3, r3
	ldr	r2, [ip, #8]
	smlal	r0, r1, r2, r2
	bx	lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void V3Length32(Vector3 *pVec3)
@
@	NOTE:
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@r0=pVec3

V3Length32:
		str	lr, [sp, #-4]!
	
		bl	V3SquaredLength32
	
		bl	fsqrt
		
		@ EXIT
		ldmfd	sp!, {lr}
		bx	lr
		
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void V3Normalize32(Vector3 *pVec3)
@
@	NOTE:
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@r0=pVec3
@r1=pView
@r2=roll

V3Normalize32:

		stmfd	sp!, {r4-r5, lr}

		@s32 len = V3Length32(v)>>8;
		mov		r4, r0	
		bl		V3Length32

		movs	r5, r0, asr #8
		mov		r1, r5
		beq		zerolen

@r1=len
@r4=pVec3

		ldr		r0, [r4, #0]
		bl		fixedpointasm_div

		ldr	ip, [r4, #4]
		mov	r3, r0, asr #16
		str	r3, [r4, #0]

		mov	r0, ip
		mov	r1, r5
		bl	fixedpointasm_div
		ldr	r1, [r4, #8]
		mov	r2, r0, asr #16
		str	r2, [r4, #4]
		mov	r0, r1
		mov	r1, r5
		bl	fixedpointasm_div
		mov	r1, r0, asr #16
		str	r1, [r4, #8]

zerolen:

		@ EXIT
		mov		r0, r4
		ldmfd	sp!,{r4-r5,lr} 
		bx		lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void V3Cross32(Vector3 *pVec3_A, Vector3 *pVec3_B, Vector3 *pVec3_Res)
@
@	NOTE:
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@r0=pVec3_A
@r1=pVec3_B
@r2=pVec3_Res

V3Cross32:

		stmfd	sp!, {r4, r5, r6, lr}
	ldr	r6, [r1, #8]
	ldr	lr, [r0, #4]
	ldr	ip, [r0, #8]
	ldr	r5, [r1, #4]
	mul	r3, r6, lr
	mul	r6, r5, ip
	rsb	r5, r6, r3
	str	r5, [r2, #0]
	ldr	r6, [r0, #8]
	ldr	ip, [r1, #0]
	ldr	r5, [r0, #0]
	ldr	lr, [r1, #8]
	mul	r3, ip, r6
	mul	ip, lr, r5
	rsb	r4, ip, r3
	str	r4, [r2, #4]
	ldr	r4, [r0, #4]
	ldr	ip, [r1, #0]
	ldr	lr, [r0, #0]
	ldr	r0, [r1, #4]
	mul	r3, ip, r4
	mul	ip, r0, lr
	rsb	r0, r3, ip
	str	r0, [r2, #8]
	mov	r0, r2
	ldmfd	sp!, {r4, r5, r6, lr}
	bx	lr
				
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void cammatrix32(Vector3 *up, Vector3 *right, Vector3 *forward, view_type *pView)
@
@	NOTE:
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@r0=pVec3_A
@r1=pVec3_B
@r2=pVec3_Res

cammatrix32:

		stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
	mov	r7, r3
	ldr	sl, [r7, #64]
	ldr	r9, [r7, #68]
	sub	sp, sp, #140
	mov	r6, r0
	mov	r8, r1
	mov	r5, r2
	ldr	r3, [r7, #72]
	mov	r0, sp
	mov	r1, sl
	mov	r2, r9
	ldr	r4, =V3Init 
	mov	lr, pc
	bx	r4
	ldr	r3, [r8, #8]
	ldr	r1, [r8, #0]
	ldr	r0, [r8, #4]
	ldr	r9, [r5, #8]
	ldr	ip, [r6, #0]
	ldr	lr, [r5, #0]
	ldr	r4, [r6, #4]
	ldr	r8, [r5, #4]
	ldr	sl, [r6, #8]
	mov	r5, #256
	rsb	r2, r1, #0
	rsb	r1, r0, #0
	rsb	r0, r3, #0
	mov	r3, #0
	str	r2, [sp, #76]
	str	ip, [sp, #80]
	str	lr, [sp, #84]
	str	r1, [sp, #92]
	str	r4, [sp, #96]
	str	r0, [sp, #108]
	str	r8, [sp, #100]
	str	sl, [sp, #112]
	str	r9, [sp, #116]
	str	r3, [sp, #88]
	str	r3, [sp, #104]
	str	r3, [sp, #120]
	str	r3, [sp, #124]
	str	r3, [sp, #128]
	str	r3, [sp, #132]
	str	r5, [sp, #136]
	str	r5, [sp, #12]
	str	r3, [sp, #16]
	str	r3, [sp, #20]
	str	r3, [sp, #24]
	str	r3, [sp, #28]
	str	r5, [sp, #32]
	str	r3, [sp, #36]
	str	r3, [sp, #40]
	str	r3, [sp, #44]
	ldr	lr, [r7, #64]
	add	r0, r7, #68
	ldmia	r0, {r0, r4}	@ phole ldm
	rsb	r2, lr, #0
	rsb	r1, r0, #0
	rsb	r0, r4, #0
	mov	ip, r2, asl #8
	mov	lr, r1, asl #8
	mov	r4, r0, asl #8
	add	r1, sp, #12
	mov	r0, r7
	add	r2, sp, #76
	str	r3, [sp, #56]
	str	ip, [sp, #60]
	str	lr, [sp, #64]
	str	r4, [sp, #68]
	str	r5, [sp, #72]
	str	r3, [sp, #48]
	str	r5, [sp, #52]
	bl	matmult32
	add	sp, sp, #140
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
	bx	lr
			
		.END

		
	
