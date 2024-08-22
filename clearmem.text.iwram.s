@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@	void ClearMem(void)
@
@	NOTE:
@			
@	
@			(c)2002 David Zurillo
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@
@	DEFINES			@
@@@@@@@@@@@@@@@@@@@@@	

	.SECTION .iwram,"ax",%progbits 
 	.ARM
	.ALIGN
	.GLOBL	ClearMem
 	
ClearMem:
	stmfd	sp!,{r4-r12,lr}

	
	mov	r3,r2
	mov	r4,r2
	mov	r5,r2
	mov	r6,r2
	mov	r7,r2
	mov	r8,r2
	mov	r9,r2
	mov	r10,r2
	mov	r11,r2
	mov	r12,r2
	
.clear:
	.REPT	6
	stmia	r1!,{r3-r12}
	.ENDR
	
	
	subs	r0,r0,#1
	bne	.clear	

 	ldmfd	sp!,{r4-r12,lr}	
	bx	lr
	