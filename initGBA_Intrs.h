/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: initGBA_Intrs.h                                *
*        Creation Date: 21/04/01                                  *
*        Description: GBA initilization functions,                *
*                     Setup Interupts,                            *                                                                 
*                     Interupt Routines                           *
*        Modification:                                            *
*        -----------------------                                  *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/ 
#ifndef INITINC
#define INITINC

#include <Agb.h>
#include "share.h"
/***************************
*  Defines                 *
****************************/

/****************************
*  Function Prototypes      *
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
void InitGBA( void );


#endif
