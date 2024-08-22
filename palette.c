/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: palette.c                                      *
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

#include "palette.h"


/****************************
*  Function Definitions     *
*****************************/

// Function that returns a RGB color in 15bit format 
u16 RGB_Color( u8 R, u8 G, u8 B )          // Uses u8 because it only needs 5 bits. Int is waste of speed.
{ 
   return ((R)|(G<<5)|(B<<10));
}

// Loads a palette from a file: First argument is a pointer to a palette and second, the 
// palette you want to write to.
void Load_Palette_CHARs( const u32 *palette )
{
	const u32 *pal = palette;
							// 512 bytes
	DmaCopy(3, pal, OBJ_PLTT, 512, 32);//DMA3Call((u32)pal, (u32)REG_PALOBJ, 0x84000080);	// Setup the sprites palette   
} 

void Load_BKG_Palette_Mode0( const u32 *palette )
{
	const u32 *pal = palette;
   DmaCopy(3, pal, BG_PLTT, 512, 32);//DMA3Call((u32)pal, (u32)REG_PALBG, 128 | DMA_ENABLE | DMA_32BIT);				// Setup the background palette
} 

void Load_BKG_Palette_Mode4( const u32 *palette )
{
   const u32 *pal = palette;
   DmaCopy(3, pal, BG_PLTT, 512, 32);
}
