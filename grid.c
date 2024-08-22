/******************************************************************
*        Author:		David Zurillo                                 *
*        Filename:		grid.c                                    *
*        Creation Date:	20/07/01                                  *
*        Description:	Motion grid functions                     *
*                                                                 *
*                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/

#include "Agb.h"
#include "Types3D.h" 

//extern void Put2PixelArray32(u32 index, u32 colr, u32 screen_buf);
extern void gba_setpixel(u32 index, u32 c, u32 screen)  __attribute__ ((section (".iwram"), long_call));

/***************************
*  Defines                 * 
****************************/
#define ABS(X) (X<0?-X:X)

#define GRIDWIDTH		6
#define GRIDHEIGHT   6

#define XORIGIN      120
#define YORIGIN      96
#define DISTANCE     400

#define  STARFOX		0

typedef struct {
   s32 x;
   s32 y;
} POINT;

/***************************
*  Variables               *
****************************/
u32 MotionGrid[GRIDHEIGHT][GRIDWIDTH];		// Motion grid.

POINT startLoc[GRIDWIDTH];
POINT offsets[GRIDWIDTH];

extern world_type world  __attribute__ ((section (".ewram")));

/***************************
*  Functions               *
****************************/

/*****************************************************************
Function:		void InitGrid( void )                             *
Description:	Initialise the motion grid system.                *
Args:			None                                                 *
Returns:		None                                                 *
*****************************************************************/
void InitGrid( void )
{
   u32 j;

   startLoc[0].x = -50;
   startLoc[0].y = 0;
   startLoc[1].x = -30;
   startLoc[1].y = 0;
   startLoc[2].x = -10;
   startLoc[2].y = 0;
   startLoc[3].x = 10;
   startLoc[3].y = 0;
   startLoc[4].x = 30;
   startLoc[4].y = 0;
   startLoc[5].x = 50;
   startLoc[5].y = 0;


   offsets[0].x = -5;
   offsets[0].y = 1;
   offsets[1].x = -3;
   offsets[1].y = 1;
   offsets[2].x = -1;
   offsets[2].y = 1;
   offsets[3].x = 1;
   offsets[3].y = 1;
   offsets[4].x = 3;
   offsets[4].y = 1;
   offsets[5].x = 5;
   offsets[5].y = 1;
/*
   for (i = 0; i < GRIDHEIGHT; i++)
   {
      for (j = 0; j < GRIDWIDTH; j++)
      {
         MotionGrid[i][j] = 1 << (i+1);
      }
   }
*/

   for (j = 0; j < GRIDWIDTH; j++)
   {
      MotionGrid[0][j] = 0;
   }

   for (j = 0; j < GRIDWIDTH; j++)
   {
      MotionGrid[1][j] = 10;
   }

   for (j = 0; j < GRIDWIDTH; j++)
   {
      MotionGrid[2][j] = 20;
   }

   for (j = 0; j < GRIDWIDTH; j++)
   {
      MotionGrid[3][j] = 30;
   }

   for (j = 0; j < GRIDWIDTH; j++)
   {
      MotionGrid[4][j] = 40;
   }

   for (j = 0; j < GRIDWIDTH; j++)
   {
      MotionGrid[5][j] = 50;
   }

}

/*****************************************************************
Function:		void UpdateGrid( void )                           *
Description:	Update the motion grid system.                    *
Args:			None                                                 *
Returns:		None                                                 *
*****************************************************************/
void UpdateGrid( void )
{

   u32 i, j;

   s32 ytrans;

   ytrans = ((world.object[STARFOX].y + 120) >> 7);

   for (i = 0; i < GRIDHEIGHT; i++)
   {
      for (j = 0; j < GRIDWIDTH; j++)
      {
         MotionGrid[i][j] += 1;
      }
      
      if (MotionGrid[i][0] >= 56)
      {
			DmaClear(3, 0, MotionGrid[i], GRIDWIDTH << 2, 16);
         //memset( MotionGrid[i], 0, GRIDWIDTH << 2);   // GRIDWIDTH * sizeof(u32)
      }
   }

}

/*****************************************************************
Function:		void DisplayGrid( void )                          *
Description:	Display the motion grid system.                   *
Args:			None                                                 *
Returns:		None                                                 *
*****************************************************************/
void DisplayGrid( u16 *screen )
{
   u32 i, j;
   u32 offset;
   u32 x, y;

   s32 xtrans = ((world.object[STARFOX].y + 520) >> 6);
   s32 ytrans = ABS(world.object[STARFOX].y >> 9);

   for (i = 0; i < GRIDHEIGHT; i++)
   {
      for (j = 0; j < GRIDWIDTH; j++)
      {
         if (j > 2)
         {
            x = MotionGrid[i][j] * (offsets[j].x + xtrans) + startLoc[j].x;
            y = MotionGrid[i][j] * offsets[j].y;
         }
         else
         {
            x = MotionGrid[i][j] * (offsets[j].x - xtrans) + startLoc[j].x;
            y = MotionGrid[i][j] * offsets[j].y;
         }
         
         x += XORIGIN;// + (world.object[STARFOX].x);
         y += YORIGIN + (ytrans << 2);

         if (x >= 0 && x < 240)
         {
            offset = y * 240 + x;
            
            gba_setpixel(offset, 0x3A3A3A3A, screen);
         }
      }
   }
}
