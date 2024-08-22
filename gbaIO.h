/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: gbaIO.h                                        *
*        Creation Date: 27/07/01                                  *
*        Description: Standard display IO functions.              *
*                                                                 *                                                                 
*                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/ 
#ifndef __GBAIOINC__
#define __GBAIOINC__

#include <Agb.h>
#include "HBDefines.h"
/***************************
*  Defines                 *
****************************/

/***************************
*  Globals                 *
****************************/

/***************************
*  Types		               *
****************************/

/****************************
*  Function Prototypes      *
*****************************/
void Font_itoa ( u32 number, u8 *pBuffer, s32 size );
/******************************************************************************************
Function:		void Gba_Print( s32 x, s32 y, u8 *text, u8 *screen,  BOB_PTR pBob )        *
                                                                                          *
Description:	Prints a string of text to the gba screen. Terminated by '\0'              *
                                                                                          *
Args:			x:       X coordinate to start output.                                        *
            y:       Y coordinate to start output.                                        *
            *text:   Pointer to a string of text to display.                              *
            *screen: Pointer to a display buffer, used for output.                        *
            *fontset:Pointer to a BOB fontset used to display the numbers.                *
                                                                                          *
Returns:		u32   :  Returns OK on success or ERROR on failure                            *
*******************************************************************************************/
void Gba_Print( s32 x, s32 y, u8 *text, u8 *screen);
#endif
