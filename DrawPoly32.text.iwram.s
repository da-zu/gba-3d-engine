@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@********************************************************************	@
@*        Author:		David Zurillo								*	@
@*        Filename:		DrawPoly32.s								*	@
@*        Creation		Date: 18/09/01								*	@
@*        Description:	Fills a 16bit buffer (screen) with a		*	@
@*						value(colr), with padding for start and end.*	@
@*																	*   @
@*		NOTES:														*	@
@*		------														*	@
@*			An ldr ( e.g ldr r3, =xend1 )							*	@
@*			Then data at memory address manipulation				*	@
@*			When done a str ( e.g str r4, [r3] )					*	@
@*																	*	@
@*			The load then store may not be required if can find a	*	@
@*			more effecient and lower register use way.				*	@
@*																	*	@
@*      Modification:												*	@
@*      -------------												*	@
@*																	*	@
@*																	*	@
@*																	*	@
@********************************************************************	@
@																		@
@	void DrawPoly32 (clipped_polygon_type *clip, u32 screen);			@
@																		@
@																		@
@	r0 = *clip															@
@	r1 = screen															@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	.INCLUDE    "AgbDefine.s"
    .INCLUDE    "AgbMemoryMap.s"
    .INCLUDE    "AgbSyscallDefine.s"
    .INCLUDE    "AgbMacro.s"
@************************************
@*	Defines 						*
@************************************
CLIP_ENTRY				=8
CLIP_X_ENTRY			=8
CLIP_Y_ENTRY			=12
CLIP_Z_ENTRY			=16

CLIP_OFFSET				=32

	
@************************************
@*	Globals 						*
@************************************
	.align 4
	.bss

	.comm	ydiff1,		4			@ difference between starting x and ending x
	.comm	ydiff2,		4			@
	.comm	xdiff1,		4			@ difference between starting y and ending y
	.comm	xdiff2,		4			@
	
	.comm	start,		4			@ starting offset of line between edges
	.comm	length,		4			@ distance from edge1 to edge2
	
	.comm	error_term1,4			@ error terms for edges 1 and 2
	.comm	error_term2,4			@

	.comm	offset1,	4			@ offset of current pixel in edges 1 and 2
	.comm	offset2,	4			@
	
	.comm	count1,		4			@ increment count for edges 1 and 2
	.comm	count2,		4			@
	
	.comm	xunit1,		4			@ unit to advance x offset in edges 1 and 2
	.comm	xunit2,		4			@
	
	.comm	edge_count,	4			@ number of edges to be drawn
	
	.comm	start_vert1,4
	.comm	end_vert1,	4
	.comm	start_vert2,4 
	.comm	end_vert2,	4
	.comm	xstart1,	4
	.comm	ystart1,	4
	.comm	xstart2,	4 
	.comm	ystart2,	4
	.comm	xend1,		4
	.comm	yend1,		4 
	.comm	xend2,		4
	.comm	yend2,		4
	.comm	numverts,	4
	.comm	buffer,		40	
	.comm	temp,		4

@************************************
@*	Functions						*
@************************************

	.text

	.extern	ydiff1
	.extern	ydiff2
	.extern	xdiff1
	.extern	xdiff2	
	.extern	start
	.extern	length	
	.extern	error_term1
	.extern	error_term2
	.extern	offset1
	.extern	offset2	
	.extern	count1
	.extern	count2	
	.extern	xunit1
	.extern	xunit2	
	.extern	edge_count		
	.extern	start_vert1
	.extern	end_vert1
	.extern	start_vert2
	.extern	end_vert2
	.extern	xstart1
	.extern	ystart1
	.extern	xstart2
	.extern	ystart2
	.extern	xend1
	.extern	yend1
	.extern	xend2
	.extern	yend2	
	.extern numverts

    .EXTERN     Font_itoa	 @( u32 number, u8 *pBuffer, s32 size );
	.EXTERN		Gba_Print	 @( s32 x, s32 y, u8 *text, u8 *screen, const u8 *fontset);	
	
	 .section	.iwram,"ax",%progbits
	.ARM
	.ALIGN
    .GLOBAL  Draw_Poly32

Draw_Poly32:
		stmfd   sp!,{r4-r12,lr}				@ save stack

		@r2 =numverts
		@r3 =first_vert
		@r4 =min_amt
		@r7 =clip		
		
		mov		r7, r0						@ save clip address
		ldr		r2, [r7]					@ put number_of_vertices in numverts. number_of_vertices is first element in clip
		ldr		r8, =numverts
		str		r2, [r8]
		mov		r3, #0						@ first_vert = 0
		
		ldr		r4, [r7, #CLIP_Y_ENTRY]!	@ load clip->vertex[0].y
											@ r4 IS FIRST clip->vertex ( [0] ).y
											@ min_amt = clip->vertex[0].y

		mov		r5, #1						@ counter = 1
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Print stats. adds 4000 ticks!!!!!!
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ldr		r8, =xend2
		@ldr		r8, [r8]
		@mov		r10, r2						@ save r2
		@mov		r2, r11						@ move into r2 value to be displayed
		@bl		PrintStats
		@mov		r2, r10						@ restore r2
		@ldmfd   sp!,{r4-r12,lr}			@ restore stack
		@bx		lr
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Find top of polygon, start by assuming vertex 0 is at top
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
1: @ for ( counter = 1; counter < numverts; counter++ )
				
		ldr		r6, [r7, #CLIP_OFFSET]!		@ get clip-vertex[1].y ( 2nd clip_type inside clip_polygon )
		
		cmp		r6, r4						@ if ( clip->vertex[counter].y < min_amt )
		movlt	r3, r5						@ firstvert = counter		
		movlt	r4, r6						@ min_amt = clip->vertex[counter].y
		
		cmp		r5, r2						@ counter < numverts
		add		r5, r5, #1
		blt		1b
		
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@		
		@ Find starting and ending of first 2 edges
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@r2 =numverts
		@r3 =first_vert				
		@r4	=start_vert1
		@r5	=start_vert2
		@r6	=xstart1
		@r7 =ystart1
		@r8 =xstart2
		@r9 =ystart2
		@r10 =clip->vertex[0].x

		add		r10, r0, #CLIP_ENTRY		@ move clip pointer to first vertex type
											
		mov		r4, r3						@ start_vert1 = first_vert
		mov		r5, r3						@ start_vert2 = first_vert
		
		ldr		r6, [r10, r4, lsl #5]		@ xstart1 = clip->vertex[start_vert1].x
		ldr		r8, [r10, r5, lsl #5]		@ xstart2 = clip->vertex[start_vert2].x

		add		r10, r10, #4				@ move r10 to point to y elements
		ldr		r7, [r10, r4, lsl #5]		@ ystart1 = clip->vertex[start_vert1].y
		ldr		r9, [r10, r5, lsl #5]		@ ystart2 = clip->vertex[start_vert2].y
				
		ldr		r10, =xstart1				@ save xstart1
		str		r6, [r10]
		ldr		r10, =ystart1				@ save ystart1
		str		r7, [r10]
		ldr		r10, =xstart2				@ save xstart2
		str		r8, [r10]
		ldr		r10, =ystart2				@ save ystart2
		str		r9, [r10]
	

		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@	Get end of edge 1 and check for wrap at vertex
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@r2 =numverts	
		@r3 =end_vert1	
		@r4	=start_vert1		
		@r5	=start_vert2
		@r6	=xend1
		@r7 =yend1
		@r8 =temp
		@r9 =clip

		sub		r3, r4, #1					@ end_vert1 = start_vert1 - 1
		
		cmp		r3, #0						@ if ( end_vert1 < 0 )
		sublt	r3, r2, #1					@ end_vert1 = numverts - 1
		
		add		r9, r0, #CLIP_ENTRY			@ move clip pointer to first vertex type

		ldr		r6, [r9, r3, lsl #5]		@ xend1 = clip->vertext[end_vert1].x
		add		r9, r9, #4					@ move r9 to point to y elements
		ldr		r7, [r9, r3, lsl #5]		@ yend1 = clip->vertext[end_vert1].y
		
		ldr		r8, =xend1					@ save xend1
		str		r6, [r8]
		ldr		r8, =yend1					@ save yend1
		str		r7, [r8]
		ldr		r8, =end_vert1				@ save end_vert1 for later as not needed until check for more edges to draw
		str		r3, [r8]

		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@	Get end of edge 2 and check for wrap at vertex
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@r2 =numverts	
		@r3 =end_vert2	
		@r4	=start_vert1		
		@r5	=start_vert2
		@r6	=xend2
		@r7 =yend2
		@r8 =temp
		@r9 =clip

		add		r3, r5, #1					@ end_vert2 = startvert2 + 1
		@mov	r3, r8

		cmp		r3, r2						@ if ( end_vert2 == clip->number_of_vertices ) < r2, numverts >
		moveq	r3, #0						@ end_vert2 = 0

		add		r9, r0, #CLIP_ENTRY			@ move clip pointer to first vertex type

		ldr		r6, [r9, r3, lsl #5]		@ xend2 = clip->vertext[end_vert2].x
		add		r9, r9, #4					@ move r9 to point to y elements
		ldr		r7, [r9, r3, lsl #5]		@ yend2 = clip->vertext[end_vert2].y
		
		ldr		r8, =xend2					@ save xend2
		str		r6, [r8]
		ldr		r8, =yend2					@ save yend2
		str		r7, [r8]
		ldr		r8, =end_vert2				@ save end_vert2 for later as not needed until check for more edges to draw
		str		r3, [r8]
		
		
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@			*******************
@				DRAW POLYGON 
@			*******************
@	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@r2 =edge_count	
		@r3 =end_vert2	
		@r4	=start_vert1		
		@r5	=xstart1
		@r6	=ystart1
		@r7 =xstart2
		@r8 =ystart2
		@r9 =clip

		sub		r2, r2, #1					@ edge_count = numverts - 1
		
		ldr		r5, =edge_count				@ store edge_count
		str		r2, [r5]
		
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Continue drawing until all edges done
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
2: @ while (edge_count > 0)
		@r2 =edge_count	
		@r3 =&offset1	
		@r4	=&offset2		
		@r5	=xstart1
		@r6	=ystart1
		@r7 =xstart2
		@r8 =ystart2
		@r9 =temp

		ldr		r9, =xstart1
		ldr		r5, [r9]
		
		ldr		r9, =ystart1
		ldr		r6, [r9]
		
		ldr		r9, =xstart2
		ldr		r7, [r9]
		
		ldr		r9, =ystart2
		ldr		r8, [r9]	
		

		@ offset1 = ( (ystart1<<7) + (ystart1<<6) + (ystart1<<5) + (ystart1<<4) )+ xstart1
		mov		r9, r6, lsl #7
		add		r9, r9, r6, lsl #6
		add		r9, r9, r6, lsl #5
		add		r9, r9, r6, lsl #4

		add		r9, r9, r5
		ldr		r3, =offset1
		str		r9, [r3]

		@ offset2 = ( (ystart2<<7) + (ystart2<<6) + (ystart2<<5) + (ystart2<<4) )+ xstart2
		mov		r9, r8, lsl #7
		add		r9, r9, r8, lsl #6
		add		r9, r9, r8, lsl #5
		add		r9, r9, r8, lsl #4

		add		r9, r9, r7
		ldr		r4, =offset2
		str		r9, [r4]

		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Initalize error terms
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@r2 =edge_count	
		@r3 =&offset1	
		@r4	=&offset2		
		@r5	=xstart1
		@r6	=ystart1
		@r7 =xstart2
		@r8 =ystart2
		@r9 =temp
		@r10 =temp

		ldr		r10, =error_term1		
		mov		r9, #0
		str		r9, [r10]

		ldr		r10, =error_term2		
		str		r9, [r10]

		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@	Setup diffs ( xdiff1, xdiff2 etc )
		@		Note:
		@		-----
		@			Arm asm doesnt know about signed types
		@			
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@r2 =edge_count	
		@r3 =&offset1	
		@r4	=&offset2		
		@r5	=xstart1
		@r6	=ystart1
		@r7 =xstart2
		@r8 =ystart2
		@r9 =temp
		@r10 =temp

		ldr		r10, =yend1					@ ydiff1 = yend1 - ystart1
		ldr		r9, [r10]
		sub		r6, r9, r6
		cmp		r6, #0						@ if ( ydiff1 < 0 )
		sublt	r6, r6, r6, lsl #1
		@mvnlt	r6, r6						@ get absolute value of ydiff1 (ydiff1 = -ydiff1)
		@r6  =ydiff1

		ldr		r10, =yend2					@ ydiff2 = yend2 - ystart2
		ldr		r9, [r10]
		sub		r8, r9, r8
		cmp		r8, #0						@ if ( ydiff2 < 0 )
		sublt	r8, r8, r8, lsl #1
		@mvnlt	r8, r8						@ get absolute value of ydiff2 (ydiff2 = -ydiff2)
		@r8  =ydiff2

		
		ldr		r10, =xend1					@ xdiff1 = xend1 - xstart1
		ldr		r9, [r10]
		ldr		r10, =xunit1
		sub		r5, r9, r5
		cmp		r5, #0						@ if ( xdiff1 < 0 )
		movlt	r11, #-1					@ xunit1 = -1
		sublt	r5, r5, r5, lsl #1
		@mvnlt	r5, r5						@ get absolute value of xdiff1 (xdiff1 = -xdiff1)
		movge	r11, #1						@ else xunit1 = 1
		strlt	r11, [r10]					@ store xunit1
		@r5  =xdiff1
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Print stats. adds 4000 ticks!!!!!!
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ldr		r8, =xend2
		@ldr		r8, [r8]
		@mov		r10, r2						@ save r2
		@mov		r2, r11						@ move into r2 value to be displayed
		@bl		PrintStats
		@mov		r2, r10						@ restore r2
		

		
		ldr		r10, =xend2					@ xdiff2 = xend2 - xstart2
		ldr		r9, [r10]
		ldr		r10, =xunit2
		sub		r7, r9, r7
		cmp		r7, #0						@ if ( xdiff2 < 0 )
		movlt	r11, #-1					@ xunit2 = -1
		sub		r7, r7, r7, lsl #1
		@mvnlt	r7, r7						@ get absolute value of xdiff2 (xdiff2 = -xdiff2)
		movge	r11, #1						@ else xunit2 = 1
		strlt	r11, [r10]					@ store xunit2
		@r7  =xdiff2


		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Choose which if the 4 routines to use
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@		
		@r2 =edge_count	
		@r3 =&offset1	
		@r4	=&offset2		
		@r5  =xdiff1
		@r6  =ydiff1
		@r7  =xdiff2
		@r8  =ydiff2
		@r9 =xend2
		@r10 =temp

		cmp		r5, r6						@ if ( xdiff1 > ydiff1 )
		ble		3f							@ else
		cmp		r7, r8						@   if ( xdiff2 > ydiff2 )
		bgt		4f
		b		5f							@   else
3:
		cmp		r7, r8						@ if ( xdiff2 > ydiff2 )
		bgt		6f
		b		7f							@ else

4:	@ if (xdiff1 > ydiff1) && (xdiff2 > ydiff2)
		
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Print stats. adds 4000 ticks!!!!!!
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ldr		r8, =xend2
		@ldr		r8, [r8]
		@mov		r10, r2						@ save r2
		@mov		r2, #4						@ move into r2 value to be displayed
		@bl		PrintStats
		@mov		r2, r10						@ restore r2
		
		b		8f

5:	@ if (xdiff1 > ydiff1) && !(xdiff2 > ydiff2)
	@r0  =clip
	@r1  =screen
	@------------
	@r3  =offset1
	@r4  =offset2
	@r5  =xdiff1
	@r6  =ydiff1
	@r7  =xdiff2
	@r8  =ydiff2
	
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Print stats. adds 4000 ticks!!!!!!
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ldr		r8, =xend2
		@ldr		r8, [r8]
		@mov		r10, r2						@ save r2
		@mov		r2, #5						@ move into r2 value to be displayed
		@bl		PrintStats
		@mov		r2, r10						@ restore r2
		
		b		8f

6:	@ if !(xdiff1 > ydiff1) && (xdiff2 > ydiff2)
	@r0  =clip
	@r1  =screen
	@-------------
	@r3  =offset1
	@r4  =offset2
	@r5  =xdiff1
	@r6  =ydiff1
	@r7  =xdiff2
	@r8  =ydiff2
	
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Print stats. adds 4000 ticks!!!!!!
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ldr		r8, =xend2
		@ldr		r8, [r8]
		@mov		r10, r2						@ save r2
		@mov		r2, #6						@ move into r2 value to be displayed
		@bl		PrintStats
		@mov		r2, r10						@ restore r2
		
		b		8f

7:	@ if !(xdiff1 > ydiff1) && !(xdiff2 > ydiff2)
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Print stats. adds 4000 ticks!!!!!!
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ldr		r8, =xend2
		@ldr		r8, [r8]
		@mov		r10, r2						@ save r2
		@mov		r2, r11						@ move into r2 value to be displayed
		@bl		PrintStats
		@mov		r2, r10						@ restore r2
	@r0  =clip
	@r1  =screen
	@-------------
	@-------------
	@-------------
	@r5  =offset1
	@r6  =ydiff1
	@r7  =offset2
	@r8  =ydiff2
	@r9	 =count1
	@r10 =count2
	@r11 =error_term1
	@r12 =error_term2

		@r2 =edge_count	
		@r3 =&offset1	
		@r4	=&offset2		
		@r5  =xdiff1
		@r6  =ydiff1
		@r7  =xdiff2
		@r8  =ydiff2
		@r9 =xend2
		@r10 =temp

		ldr		r10, =offset1
		ldr		r5, [r10]

		ldr		r10, =offset2
		ldr		r7, [r10]

		ldr		r10, =error_term1
		ldr		r11, [r10]

		ldr		r10, =error_term2
		ldr		r12, [r10]

		mov		r9, r6						@ count1 = ydiff1
		mov		r10, r8						@ count2 = ydiff2
		
		@r2 =edge_count	
		@r3 =&offset1	
		@r4	=&offset2		
		@r5  =offset1
		@r6  =ydiff1
		@r7  =offset2
		@r8  =ydiff2
		@r9 =count1
		@r10 =count2
		@r11 =error_term1
		@r12 =error_term2

	9:	@ while ( count1 && count 2)
		
		@ calculate edge 1:
		ldr		r3, =xdiff1
		ldr		r2, [r3]
		add		r11, r11, r2				@ error_term1 += xdiff1
		cmp		r11, r6						@ if time to increment Y. if (error_term1 >= ydiff1)
		bge		12f
		@r2 =xdiff1
	10:
		@ calculate edge 2:
		ldr		r3, =xdiff2
		ldr		r2, [r3]
		add		r12, r12, r2				@ error_term2 += xdiff2
		cmp		r12, r8						@ if time to increment Y. if (error_term2 >= ydiff2)
		bge		13f
		@r2 =xdiff2
	
	11:
		@r2 =temp
		@r3 =temp
		@r4 =temp
		@r5  =offset1
		@r6  =ydiff1
		@r7  =offset2
		@r8  =ydiff2
		@r9 =count1
		@r10 =count2
		@r11 =error_term1
		@r12 =error_term2

		sub		r9, r9, #1					@ count1--
		sub		r10, r10, #1				@ count2--

		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Draw line from edge 1 to edge 2
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@r2 =length
		@r3 =start
		sub		r2, r7, r5					@ length = offset2 - offset1
		
		cmp		r2, #0						@ if length < 0
		sub		r2, r2, r2, lsl #1
		@mvnlt	r2, r2						@ length = -length
		movlt	r3, r7						@ start = offset2
		
		movge	r3, r5						@ start = offset1
				
		add		r0, r0, #4					@ clip->color
		@ldr		r4, =Fill2Array32
		@mov     lr, pc
		
        @bx      r4							@ draw the line


Fill2Array32:        
		stmfd   sp!,{r0-r3}
		@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ TEST PIXEL START LOCATION
		@@@@@@@@@@@@@@@@@@@@@@@@@@@
		add		r3,r3,r1			@ add index to screen base
		add		r2,r2,r3			@ add length to index + screen base

		movs	r1,r3,rrx			@ test or even/odd pixel location
	
		@@@@@@@@@@@@@@@@@@
		@  START PLOT ODDD
		@@@@@@@@@@@@@@@@@@	
		@subcs		r3,r3,#1
			
		ldrcsh		r1,[r3],#-1			@ get 2 pixels
		movcs		r1,r1,lsr #8
		orrcs		r1,r1,r0,lsl #8	@ merge to 16bit plot ( 2 pixels at a time )

		strcsh		r1,[r3],#2 		@ plot 2 pixels	

92:		
		@@@@@@@@@@@@@@@@@@@@@
		@ PLOT MIDDLE OF LINE
		@@@@@@@@@@@@@@@@@@@@@
		strh	r0,[r3],#2			@ Plot the 2 pixels with colr at screen address + index
									@ Write back next pixel along the hline
		cmp		r3,r2
		blt		92b
		
	
		@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ TEST PIXEL END LOCATION
		@@@@@@@@@@@@@@@@@@@@@@@@@@@		

		movs	r1,r2,rrx		@ test or even/odd pixel location
		bcs		93f
		
		@@@@@@@@@@@@@@@@@@
		@  END PLOT EVEN
		@@@@@@@@@@@@@@@@@@
		@add		r3,r3,#1
		ldrh	r1,[r3],#1			@ get 2 pixels
		mov		r1,r1,lsl #8
		and		r0,r0,#0x00FF
		orr		r1,r1,r0			@ merge to 16bit plot ( 2 pixels at a time )

		strh	r1,[r3]				@ plot the 2 pixels

93: @ exit were done drawing
		
		ldmfd   sp!,{r0-r3}
		@bx      lr					@ exit were done drawing
	
		@r2 =length
		@r3 =start
		@r4 =temp
		@r5  =offset1
		@r6  =ydiff1
		@r7  =offset2
		@r8  =ydiff2
		@r9 =count1
		@r10 =count2
		@r11 =error_term1
		@r12 =error_term2

		sub		r0, r0, #4

		add		r5, r5, #240				@ advance edge 1 offfset to next line
		ldr		r3, =ystart1				@ ystart1++
		ldr		r4, [r3]
		add		r4, r4, #1
		str		r4, [r3]
		add		r7, r7, #240				@ advance edge 1 offfset to next line
		ldr		r3, =ystart2				@ ystart2++
		ldr		r4, [r3]
		add		r4, r4, #1
		str		r4, [r3]

		ldr		r4, =offset1
		str		r5, [r4]
		ldr		r4, =offset2
		str		r7, [r4]

		cmp		r9, #0						@ while count1 && count 2
		cmpne	r10, #0
		bne		9b							@ still drawing
		b		8f							@ else check for another edge

	12: @ if (error_term1 >= ydiff1) 
		@r2 =xdiff1	
		@r3 =temp
		@r4	=temp
		@r5  =offset1
		@r6  =ydiff1
		@r7  =offset2
		@r8  =ydiff2
		@r9 =count1
		@r10 =count2
		@r11 =error_term1
		@r12 =error_term2
		sub		r11, r11, r6				@ restore error term. error_term1 -= ydiff1
		ldr		r3, =xunit1
		ldr		r2, [r3]
		add		r5, r5, r2					@ offset1 += xunit1
		ldr		r4, =offset1
		str		r5, [r4]
		ldr		r3, =xstart1
		ldr		r4, [r3]
		add		r4, r4, r2					@ xstart1 += xunit1
		str		r4, [r3]
		b		10b
		@r2 =xunit1
		@r3 =&xstart1
		@r4 =xstart1

	13: @ if (error_term2 >= ydiff2) 
		@r2 =xdiff2	
		@r3 =temp
		@r4	=temp	
		@r5  =offset1
		@r6  =ydiff1
		@r7  =offset2
		@r8  =ydiff2
		@r9 =count1
		@r10 =count2
		@r11 =error_term1
		@r12 =error_term2
		sub		r12, r12, r8				@ restore error term. error_term2 -= ydiff2
		ldr		r3, =xunit2
		ldr		r2, [r3]		
		add		r7, r7, r2					@ offset2 += xunit2
		ldr		r4, =offset2
		str		r7, [r4]
		ldr		r3, =xstart2
		ldr		r4, [r3]
		add		r4, r4, r2					@ xstart2 += xunit2
		str		r4, [r3]
		b		11b
		@r2 =xunit2
		@r3 =&xstart2
		@r4 =xstart2

		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Print stats. adds 4000 ticks!!!!!!
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ldr		r8, =xend2
		@ldr		r8, [r8]
		@mov		r10, r2						@ save r2
		@mov		r2, #7						@ move into r2 value to be displayed
		@bl		PrintStats
		@mov		r2, r10						@ restore r2
		
		b		8f
	
		
8:	@ Another edge check if (!count1) and if (!count2) checks
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@	Start next edge if any
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@r2 =temp
	@r3 =temp
	@r4 =temp
	@r5  =offset1
	@r6  =ydiff1
	@r7  =offset2
	@r8  =ydiff2
	@r9 =count1
	@r10 =count2
	@r11 =error_term1
	@r12 =error_term2

		ldr		r4, =count1
		str		r9, [r4]
		ldr		r4, =count2
		str		r10, [r4]

		cmp		r9, #0							@ if (!count1)
		beq		14f @bne		14f
		b		15f

	14:	@ if (!count1) if edge 1 is complete
		@r2 =temp
		@r3 =temp
		@r4 =temp
		@r5  =offset1
		@r6  =ydiff1
		@r7  =offset2
		@r8  =ydiff2
		@r9 =count1
		@r10 =count2
		@r11 =error_term1
		@r12 =error_term2

		ldr		r3, =edge_count						@ edge_count--
		ldr		r4, [r3]
		sub		r4, r4, #1
		str		r4, [r3]
		
		ldr		r3, =end_vert1						@ start_vert1 = end_vert1
		ldr		r5, [r3]
		ldr		r4, =start_vert1
		str		r5, [r4]

		sub		r5, r5, #1							@ end_vert1--
		str		r5, [r3]

		cmp		r5, #0							@ if (end_vert1 < 0 )
		ldrlt	r11, =numverts
		ldrlt	r4, [r11]
		sublt	r4, r4, #1								
		strlt	r4, [r3]						@ end_vert1 = numverts - 1
		
		@ldr		r3, [r3]
		mov		r3, r5
		@r3 =end_vert1
		add		r5, r0, #CLIP_ENTRY				@ move clip pointer to first vertex type
				
		ldr		r4, [r5, r3, lsl #5]			@ xend1 = clip->vertex[end_vert1].x
		ldr		r3, =xend1
		str		r4, [r3]

		add		r5, r5, #4
		ldr		r4, [r5]
		ldr		r3, =yend1
		str		r4, [r3]						@ yend1 = clip->vertex[end_vert1].y
		
	15:	@ if (!count2)
		@r2 =temp
		@r3 =temp
		@r4 =temp
		@r5  =offset1
		@r6  =ydiff1
		@r7  =offset2
		@r8  =ydiff2
		@r9 =count1
		@r10 =count2
		@r11 =error_term1
		@r12 =error_term2
@**************** DRAMA!!!!!!!!!!!!!!
		cmp		r10, #0								@ if (!count2)
		bne		16f @beq 16f

		ldr		r3, =edge_count						@ edge_count--
		ldr		r4, [r3]
		sub		r4, r4, #1
		str		r4, [r3]

		ldr		r3, =end_vert2						@ start_vert1 = end_vert1
		ldr		r5, [r3]
		ldr		r4, =start_vert2
		str		r5, [r4]

		add		r5, r5, #1							@ end_vert2++
		@str		r5, [r3]

		
		ldr		r4, [r0]							@ if (end_vert2 == numverts)
		cmp		r5, r4
		moveq	r5, #0								@ end_vert2 = 0
		
		str		r5, [r3]							

		@ldr		r3, [r3]		
		add		r11, r0, #CLIP_ENTRY				@ move clip pointer to first vertex type
				
		ldr		r4, [r11, r5, lsl #5]			@ xend1 = clip->vertex[end_vert1].x
		ldr		r3, =xend2
		str		r4, [r3]

		add		r11, r11, #4
		ldr		r4, [r11]
		ldr		r3, =yend2
		str		r4, [r3]						@ yend1 = clip->vertex[end_vert1].y


	16:	@ while (edge_count > 0)
		ldr		r3, =edge_count
		ldr		r4, [r3]
		cmp		r4, #0
		bgt		2b

		@ fall thru to exit draw_poly32 else


		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ Print stats. adds 4000 ticks!!!!!!
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ldr		r8, =xend2
		@ldr		r8, [r8]
		@mov		r10, r2						@ save r2
		@mov		r2, r4						@ move into r2 value to be displayed
		@bl		PrintStats
		@mov		r2, r10						@ restore r2
		@ldmfd   sp!,{r4-r12,lr}				@ restore stack
		@bx		lr
		
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		@ DONE ALL, now exit to caller
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		ldmfd   sp!,{r4-r12,lr}				@ restore stack

		bx		lr

		
PrintStats:
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@		
		@ Print to std display r2
		@ !!!!WARNING all used regs are destroyed!!!!!!!!!!!!!!!!
		@ need to implement a stack save and restore
		@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@		
		mov		r7, r0						@ save clip
		mov		r8,	r1						@ save screen
		mov		r9,	lr						@ save return address

		ldr     r5, =Font_itoa				
        mov		r0, r2						@ r2 numverts
		ldr		r1, =buffer
		mov		r2,	#10
		mov     lr, pc
        bx      r5							@ ( u32 number, u8 *pBuffer, s32 size );
													
		mov		r0, #0		
		ldr		r2,	=buffer
		mov		r1, #0
		mov		r3, r8
		ldr		r5, =Gba_Print
		mov		lr, pc
		bx		r5							@ ( s32 x, s32 y, u8 *text, u8 *screen, const u8 *fontset);

		mov		r0, r7						@ restore clip
		mov		r1, r8						@ restore screen
		mov		lr, r9
		bx		lr							@ return to caller
