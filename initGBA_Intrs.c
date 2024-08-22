/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: initGBA_Intrs.c                                *
*        Creation Date: 21/04/01                                  *
*        Description: GBA initilization functions,                *
*                     Setup Interupts,                            *                                                                 
*                     Interupt Routines							      *                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/ 

#include "initGBA_Intrs.h"
/****************************
*		  Globals			*
*****************************/


/****************************
*  Function Definitions      *
*****************************/
/********************************************************************************************
Function:		void InitGBA( void )														*
																							*
Description:	Initialze GBA and setup interupts request flags.							*
				User configurable ATM.														*
																							*
Args:			None																		*
																							*	
Returns:		None																		*
*********************************************************************************************/
void InitGBA( void )
{
	//ClearRamAll();
	
	// 3-1 Setup wait access
	*(vu16 *)REG_WAITCNT = CST_ROM0_1ST_3WAIT | CST_ROM0_2ND_1WAIT | CST_PREFETCH_ENABLE;
	
	/*// Setup interupt main buffer
	InitInterrupts();
	
	// Set IME 
	*(vu16 *)REG_IME  = 1;                      
	// V Blank interrupt request flag enabled 
    *(vu16 *)REG_STAT = STAT_V_BLANK_IF_ENABLE;// | STAT_H_BLANK_IF_ENABLE;	
	*(vu16 *)REG_IE = V_BLANK_INTR_FLAG;// | H_BLANK_INTR_FLAG;	*/
}


