/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: gbaIO.c                                        *
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
#include "gbaIO.h"

/****************************
*		  Globals			    *
*****************************/
extern const u8 font8x8_IndexBitmap[80*8 + 0];
/****************************
*  Function Definitions     *
*****************************/

void ConsolePrint_T(char *s)
{
    asm volatile("mov r0, %0;"
		"swi 0xff;"
		: // no ouput
	: "r" (s)
		: "r0");
}
/*
// ARM code
void ConsolePrint_A(char *s)
{
    asm volatile("mov r0, %0;"
		"swi 0xff0000;"
		: // no ouput
	: "r" (s)
		: "r0");
}*/

void Font_itoa ( u32 number, u8 *pBuffer, s32 size )
{
   s32 i;

   for( i=size; i>0; i-- )
   {
      pBuffer[i] = (number%10) + 0x30;	  
      number /= 10;

	  //number = (( (number<<7) - (number<<5) + (number<<2) + (number <<1)  + (number >> 1) - (number>>3)) >> 10);
   }

}
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
Returns:		none                                                                          *
*******************************************************************************************/
void Gba_Print( s32 x, s32 y, u8 *text, u8 *screen)
{   	
   s32 i, xdim, ydim;
   u16 curChar;

   i = 0;

   do
	{
		curChar = text[i];
      switch ( curChar )
      {
      case '0':     
       for ( ydim = 0; ydim < 8; ydim++ )
         {
            for ( xdim = 0; xdim < 8; xdim+=2 )
            {
               screen[(y+ydim)*240+(x+xdim+(i*8))] = font8x8_IndexBitmap[ydim*80+xdim];
            }
         }
      break;
      case '1':
         for ( ydim = 0; ydim < 8; ydim++ )
         {
            for ( xdim = 0; xdim < 8; xdim+=2 )
            {
               screen[(y+ydim)*240+(x+xdim+(i*8))] = font8x8_IndexBitmap[ydim*80+(xdim+8)];
            }
         }
         break;
      case '2':
         for ( ydim = 0; ydim < 8; ydim++ )
         {
            for ( xdim = 0; xdim < 8; xdim+=2 )
            {
               screen[(y+ydim)*240+(x+xdim+(i*8))] = font8x8_IndexBitmap[ydim*80+(xdim+16)];
            }
         }
         break;
      case '3':
         for ( ydim = 0; ydim < 8; ydim++ )
         {
            for ( xdim = 0; xdim < 8; xdim+=2 )
            {
               screen[(y+ydim)*240+(x+xdim+(i*8))] = font8x8_IndexBitmap[ydim*80+(xdim+24)];
            }
         }
         break;
      case '4':
         for ( ydim = 0; ydim < 8; ydim++ )
         {
            for ( xdim = 0; xdim < 8; xdim+=2 )
            {
               screen[(y+ydim)*240+(x+xdim+(i*8))] = font8x8_IndexBitmap[ydim*80+(xdim+32)];
            }
         }
         break;
      case '5':
         for ( ydim = 0; ydim < 8; ydim++ )
         {
            for ( xdim = 0; xdim < 8; xdim+=2 )
            {
               screen[(y+ydim)*240+(x+xdim+(i*8))] = font8x8_IndexBitmap[ydim*80+(xdim+40)];
            }
         }
         break;
      case '6':
         for ( ydim = 0; ydim < 8; ydim++ )
         {
            for ( xdim = 0; xdim < 8; xdim+=2 )
            {
               screen[(y+ydim)*240+(x+xdim+(i*8))] = font8x8_IndexBitmap[ydim*80+(xdim+48)];
            }
         }
         break;
      case '7':
         for ( ydim = 0; ydim < 8; ydim++ )
         {
            for ( xdim = 0; xdim < 8; xdim+=2 )
            {
               screen[(y+ydim)*240+(x+xdim+(i*8))] = font8x8_IndexBitmap[ydim*80+(xdim+56)];
            }
         }
         break;
      case '8':
         for ( ydim = 0; ydim < 8; ydim++ )
         {
            for ( xdim = 0; xdim < 8; xdim+=2 )
            {
               screen[(y+ydim)*240+(x+xdim+(i*8))] = font8x8_IndexBitmap[ydim*80+(xdim+64)];
            }
         }
         break;
      case '9':
         for ( ydim = 0; ydim < 8; ydim++ )
         {
            for ( xdim = 0; xdim < 8; xdim+=2 )
            {
               screen[(y+ydim)*240+(x+xdim+(i*8))] = font8x8_IndexBitmap[ydim*80+(xdim+72)];
            }
         }
         break;
      
      }
		
		i++;

	} while( text[i] != '\0' );
}
