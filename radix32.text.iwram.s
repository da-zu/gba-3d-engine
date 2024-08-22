@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ Radix_Sort(polygon_list_type *source, polygon_list_type *temp, u32 N, u32 byte)
@  
@	(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		.text
		.section	.iwram,"ax",%progbits		
		.ARM
        .ALIGN						
        .GLOBAL  Radix_Sort32  
		.GLOBAL  Radix32  

DISTANCE_OFFSET	=14 
 	   
Radix32:

@r0 = *source
@r1 = *temp
@r2 = N
@r3 = byte
 
		stmfd   sp!,{r0-r12,lr}		@ push reg onto stack
		
		sub		sp, sp, r2, asl #2
		mov		r12, sp
		
		mov		r3, r3, asl #3		@ byte*8

		mov		r4, #0
		mov		r5, #0
		mov		r6, #0
		mov		r7, #0
		mov		r8, #0
		mov		r9, #0
		mov		r10, #0
		mov		r11, #0

		@for (i=0; i<256; i++)
		@{
		@	count[i] = 0;
		@}		
		.REPT	32
		stmfd	sp!, {r4-r11}
		.ENDR

		

		@for(i=N-1; i>=0; i--)
		@{ 
		@	count[((-source->polygon[i]->distance)>>(byte*8))&0xff]++;
		@}
		sub		r4, r2, #1
		mov		r7, r4, asr #2			@k=((n-1)>>2)
		
		mov		r8,r0
		mov		r10,r12

setupCount0:
		
		ldmia	r8!, {r5,r6,r9,lr}
		
		@1				
		@ldr		r5, [r5, #16]				@ get polygon->distance
		ldrsh	r5, [r5, #DISTANCE_OFFSET]				@ get polygon->distance
		rsb		r5, r5, #0					@ negate polygon->distance		
		mov		r5, r5, asr r3
		and		r5, r5, #0xFF		
		ldr		r11, [sp, r5, asl #2]		@ get the current value at count[r5]
		add		r11, r11, #1			
		str		r11, [sp, r5, asl #2]		@ put the incremented value back

		@2
		@ldr		r6, [r6, #16]				@ get polygon->distance
		ldrsh	r6, [r6, #DISTANCE_OFFSET]				@ get polygon->distance
		rsb		r6, r6, #0					@ negate polygon->distance		
		mov		r6, r6, asr r3
		and		r6, r6, #0xFF
		ldr		r11, [sp, r6, asl #2]		@ get the current value at count[r5]
		add		r11, r11, #1			
		str		r11, [sp, r6, asl #2]		@ put the incremented value back
				
		@3
		@ldr		r9, [r9, #DISTANCE_OFFSET]				@ get polygon->distance
		ldrsh	r9, [r9, #DISTANCE_OFFSET]				@ get polygon->distance
		rsb		r9, r9, #0					@ negate polygon->distance		
		mov		r9, r9, asr r3
		and		r9, r9, #0xFF
		ldr		r11, [sp, r9, asl #2]		@ get the current value at count[r5]
		add		r11, r11, #1			
		str		r11, [sp, r9, asl #2]		@ put the incremented value back

		@4		
		@ldr		lr, [lr, #DISTANCE_OFFSET]				@ get polygon->distance
		ldrsh	lr, [lr, #DISTANCE_OFFSET]				@ get polygon->distance
		rsb		lr, lr, #0					@ negate polygon->distance		
		mov		lr, lr, asr r3
		and		lr, lr, #0xFF
		ldr		r11, [sp, lr, asl #2]		@ get the current value at count[r5]
		add		r11, r11, #1			
		str		r11, [sp, lr, asl #2]		@ put the incremented value back
				
		
		stmia	r10!, {r5,r6,r9,lr}		
				
		subs	r7, r7, #1
		bgt		setupCount0

		
@r4 = (n-1)
		mov		r7, r4, asr #2
		sub		r7, r4, r7, asl #2	
		blt		poo

leftovers1:
		
		ldr		r5, [r8], #4			@ derefrence polgony_list_type (*source)		
		@ldr		r5, [r5, #4]				@ get polygon->distance
		ldrsh		r5, [r5, #DISTANCE_OFFSET]				@ get polygon->distance
		rsb		r5, r5, #0					@ negate polygon->distance		
		mov		r5, r5, asr r3
		and		r5, r5, #0xFF

		str		r5, [r10], #4

		ldr		r6, [sp, r5, asl #2]		@ get the current value at count[r5]
		add		r6, r6, #1			
		str		r6, [sp, r5, asl #2]		@ put the incremented value back

		subs	r7, r7, #1
		bpl		leftovers1

		
@r0 = *source
@r1 = *temp
@r2 = N
@r3 = byte
@r4-lr = free
@r7 = 0		

		
poo:
		mov		r7, #0

		mov		r4, sp				@ *count (sp - 1024) size 1024
		sub		sp, sp, #1024		
		mov		r5, sp				@ *index (sp - 2048) size 1024

		str		r7, [r5]			@ index[0] = 0;
		mov		r7, #254
		
		mov		r9, r4
		mov		r10, r5		
		
		@index[0] = 0;
		@for(i=1; i<256; i++)
		@{
		@	index[i]=index[i-1]+count[i-1];
		@}
setupIndex0:
		
		ldr		r6, [r5], #4
		ldr		r8, [r4], #4
		add		r6, r6, r8
		str		r6, [r5]
				
		subs		r7, r7, #1
		bge		setupIndex0
				
		mov		r4, r9
		mov		r5, r10

@r0 = *source
@r1 = *temp
@r2 = N
@r3 = byte
@r4 = *count
@r5 = *index
@r6-lr = free

		@for(i=0; i<N; i++)
		@{
		@	dest->polygon[index[((-source->polygon[i]->distance)>>(byte*8))&0xff]++] =  source->polygon[i];
		@}

		mov		r9, r2
		sub		r4, r2, #1				@r2 = N-1		
		mov		r3, r4, asr #2			@k=((n-1)>>2)
		
		stmfd	sp!, {r4,r9}

NLoop:	

		ldmia	r0!,{r6,r10,r11,lr}		@ derefrence polgony_list_type (pPolyList) and writeback for next loop iteration
		ldmia	r12!,{r2,r4,r7,r9}
@1				
		ldr		r8, [r5, r2, asl #2]	@ get the current value at index[r7]		
		str		r6, [r1, r8, asl #2]	@ dest->polygon[r8] = source->polygon[i]		
		add		r8, r8, #1	
		str		r8, [r5, r2, asl #2]	@ set the incremented value at index[r7]

@2		
		ldr		r8, [r5, r4, asl #2]	@ get the current value at index[r7]		
		str		r10, [r1, r8, asl #2]	@ dest->polygon[r8] = source->polygon[i]		
		add		r8, r8, #1	
		str		r8, [r5, r4, asl #2]		@ set the incremented value at index[r7]

@3
		ldr		r8, [r5, r7, asl #2]	@ get the current value at index[r7]		
		str		r11, [r1, r8, asl #2]	@ dest->polygon[r8] = source->polygon[i]		
		add		r8, r8, #1	
		str		r8, [r5, r7, asl #2]		@ set the incremented value at index[r7]

@4
		ldr		r8, [r5, r9, asl #2]	@ get the current value at index[r7]		
		str		lr, [r1, r8, asl #2]	@ dest->polygon[r8] = source->polygon[i]		
		add		r8, r8, #1	
		str		r8, [r5, r9, asl #2]		@ set the incremented value at index[r7]

		subs	r3, r3, #1
		bgt		NLoop
			
@r3 = k = ((n-1)>>2)
@r4 = (n-1)
		ldmfd	sp!, {r4,r9}

		mov		r3, r4, asr #2
		subs	r3, r4, r3, asl #2	
		blt		poo2

leftovers2:		
		
		ldr		r6, [r0],#4

		ldr		r7, [r12],#4		
		ldr		r8, [r5, r7, asl #2]	@ get the current value at index[r7]		
		str		r6, [r1, r8, asl #2]	@ dest->polygon[r8] = source->polygon[i]		

		add		r8, r8, #1	
		str		r8, [r5, r7, asl #2]		@ set the incremented value at index[r7]

		subs	r3, r3, #1
		bpl		leftovers2
poo2:

		
		@EXIT
		add		sp, sp, #2048
		add		sp, sp, r9, asl #2
		
						
		ldmfd   sp!,{r0-r12,lr}		@ push reg onto stack
		
		mov		pc, lr

		.pool


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ Radix(polygon_list_type *source, polygon_list_type *dest, u32 N)
@  
@	(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		 	 		 
Radix_Sort32:    

@r0 = *source
@r1 = *temp
@r2 = N	 

		stmfd   sp!,{lr}		@ push reg onto stack
		
		mov		r3, #0
		bl		Radix32
	
		eor r0, r0, r1
		eor r1, r1, r0
		eor r0, r0, r1
		
		mov		r3, #1
		bl		Radix32

		eor r0, r0, r1
		eor r1, r1, r0
		eor r0, r0, r1

		mov		r3, #2
		bl		Radix32

		eor r0, r0, r1
		eor r1, r1, r0
		eor r0, r0, r1

		mov		r3, #3
		bl		Radix32

		
		ldmfd   sp!,{lr}		@ push reg onto stack

		bx lr
		 
		.end
	
 