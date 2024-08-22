/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: math2D.c                                       *
*        Creation Date: 19/10/01                                  *
*        Description:	Utility set for 2D related Math.          *
*                                                                 *
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
/* NOTES:
*			
*
* TODO:
*			
********************************************************************/
#ifndef MATH2DINC
#define MATH2DINC

#include <Agb.h>
#include "HBDefines.h"
/***************************
*  Defines                 *
****************************/
#define	DONT_INTERSECT    0
#define	DO_INTERSECT      1
#define  COLLINEAR         2
/***************************
*  Macros                  *
****************************/
#define SAME_SIGNS( a, b )	(((s32) ((u32) a ^ (u32) b)) >= 0 )

/***************************
*  Types		               *
****************************/

/****************************
*  Function Prototypes      *
*****************************/
inline s32 fsqrt(float n);

/*********************************************************************************************
Function:		s32 Lines_Intersect( s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 x4, s32 y4, s32 *pX, s32 *pY );											                  
                                                                                          
Description:	Test if 2 lines intersect and calculates the intersection point on the line			
                                                                                          
Args:			x1, y1, x2, y2:   Line 1 start and end xy points
            x3, y3, x4, y4:   Line 2 start and end xy points
            *pX:              Pointer to variable to be given the x coordinate of inersection
            *pY:              Pointer to variable to be given the y coordinate of inersection
                                                                                          
Returns:		s32:        DONT_INTERSECT    Lines dont intersect
                        DO_INTERSECT		Lines do intersect			
                        COLLINEAR         Lines are colinear
*********************************************************************************************/
s32 Lines_Intersect( s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 x4, s32 y4, s32 *x, s32 *y );

void linedraw(s32 x1,s32 y1,s32 x2,s32 y2,u16 color, u8 *screen);
#endif
