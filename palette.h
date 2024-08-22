/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: palette.h                                      *
*        Creation Date: 21/04/01                                  *
*        Description: Palette and color utilities and manipulation*
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
#ifndef PALETTEINC
#define PALETTEINC

#include <Agb.h>
/***************************
*  Defines                 *
****************************/


/***************************
*  Variables               *
****************************/


/****************************
*  Function Prototypes      *
*****************************/

// Function that returns a RGB color in 15bit format 
u16 RGB_Color( u8 R, u8 G, u8 B );

// Loads a palette from a file: First argument is a pointer to a palette and second, the 
// palette you want to write to.
void Load_Palette_CHARs( const u32 *palette );
void Load_BKG_Palette_Mode4( const u32 *palette );
void Load_BKG_Palette_Mode0( const u32 *palette );


#endif
