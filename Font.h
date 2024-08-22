/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: Font.h                                         *
*        Creation Date: 17/08/01                                  *
*        Description: Font loading and managemenet.               *
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
#ifndef FONTINC
#define FONTINC

#include <Agb.h>
#include "Char.h"
#include "HBDefines.h"
#include "HBTypes.h"
#include "oamMan.h"
/***************************
*  Defines                 *
****************************/
#define FONT_SPEEDX 0
#define FONT_SPEEDY 0

/***************************
*  Globals                 *
****************************/

/***************************
*  Types		           *
****************************/

/****************************
*  Function Prototypes      *
*****************************/
/******************************************************************************************
Function:		void Font_Init(void)                                                       *
                                                                                          *
Description:	Initializes CHAR's used to represent chracters                              *
                                                                                          *
Args:			none                                                                          *
                                                                                          *
Returns:		none                                                                          *
*******************************************************************************************/
void Init_Font(void);

/******************************************************************************************
Function:		void Font_itoa ( u32 number, u8 *pBuffer, s32 size )                       *
                                                                                          *
Description:	Converts a u32 number into a ascii string.                                 *
                                                                                          *
Args:			number:  The number to be converted.                                          *
            pBuffer: Pointer to the character buffer to store the ascii string.           *
            size:    Size of string. If total size is not used padding will occur.        *
Returns:		none                                                                          *
*******************************************************************************************/
void Font_itoa ( u32 number, u8 *pBuffer, s32 size );

/******************************************************************************************
Function:		void Font_Print( s32 x, s32 y, u8 *text, s32 levelPosY )                   *
                                                                                          *
Description:	Prints a string of text to the gba screen. Terminated by '\0'              *
               ATM only prints numeric ascii characters.                                  *
                                                                                          *
Args:			x:          X coordinate to start output.                                     *
            y:          Y coordinate to start output.                                     *
            *text:      Pointer to a string of text to display.                           *
            levelPosY:  Current level posistion of main character                         *
                                                                                          *
Returns:		none                                                                          *
*******************************************************************************************/
void Font_Print( s32 x, s32 y, u8 *text );

void ColResponce_Font_PLAYER_TO_NPC( CHAR_TYP *pCHAR, u32 isCollide );

void ColResponce_Font_NPC_TO_NPC( CHAR_TYP *pCHAR, u32 isCollide );

void ColResponce_Font_LEVEL( CHAR_TYP *pCHAR, u32 isCollide, s32 impactX, s32 impactY );
#endif
