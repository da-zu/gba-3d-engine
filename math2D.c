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
#include "math2D.h"
#include "Fix.h"

#define FP_BITS(fp) (*(long *)&(fp))
#define FP_ABS_BITS(fp) (FP_BITS(fp)&0x7FFFFFFF)
#define FP_SIGN_BIT(fp) (FP_BITS(fp)&0x80000000)
#define FP_ONE_BITS 0x3F800000

/******************************
*  Globals                    *
*******************************/
extern const u32 sqrttab[];

/******************************
*  Function Definitions       *
*******************************/

void FastIntDivide (s32 Numer, s32 Denom, s32 *Result, s32 *Remainder)
{
	asm volatile
		("
		mov   r0,%2
		mov   r1,%3
		swi   6
		ldr   r2,%0
		str   r0,[r2]
		ldr   r2,%1
		str   r1,[r2]
		"
		: "=m" (Result), "=m" (Remainder) // Outputs
		: "r" (Numer), "r" (Denom)        // Inputs
		: "r0","r1","r2","r3"             // Regs crushed & smushed
		);
}

/*
* fsqrt - fast square root by table lookup
*/  
s32 fsqrt(float n)
{  

  if (FP_BITS(n) == 0)
    return 0.0;                 // check for square root of 0
  
  FP_BITS(n) = sqrttab[(FP_BITS(n) >> 8) & 0xFFFF] | ((((FP_BITS(n) - 0x3F800000) >> 1) + 0x3F800000) & 0x7F800000);
  
  return n;
}

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
s32 Lines_Intersect( s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 x4, s32 y4, s32 *x, s32 *y )
{

   return ( DO_INTERSECT );
} /* lines_intersect */

/******************************************************************************************************************************
Function:		Get_Next_Line_Pos( u32 maxLength, s16 startX, s16 startY, s16 endX, s16 endY, s16 *pNextPosX, s16 *pNextPosY); *
                                                                                                                              *
Description:	Fills an X and Y posistion array with points along a line.										                        *
                                                                                                                              *
Args:		   maxLength:  Maximum number of positions to fill																				         *
            startX:     Start X posistion                                                                                     *
            startY:     Start Y posistion                                                                                     *
            endX:       End X posistion                                                                                       *
            endY:       End Y posistion                                                                                       *
            pNextPosX:  Pointer to the array to be filled with X posistions                                                   *
            pNextPosY:  Pointer to the array to be filled with Y posistions                                                   *
                                                                                                                              *
Returns:		none                                                                                                              *
******************************************************************************************************************************/
void linedraw(s32 x1,s32 y1,s32 x2,s32 y2,u16 color,
               u8 *screen) 
{
  int y_unit,x_unit; // Variables for amount of change
                     //  in x and y
  int xdiff;

  int error_term;

  int offset=y1*240+x1; // Calculate offset into video RAM

  int ydiff=y2-y1;   // Calculate difference between
                     //  y coordinates
  if (ydiff<0) {     // If the line moves in the negative
                     //  direction
    ydiff=-ydiff;    // ...get absolute value of difference
    y_unit=-240;     // ...and set negative unit in
                     //  y dimension
  }
  else y_unit=240;   // Else set positive unit in
                     //  y dimension

  xdiff=x2-x1;			// Calculate difference between
                        //  x coordinates
  if (xdiff<0) {				// If the line moves in the
                        //  negative direction
	  xdiff=-xdiff;				// ...get absolute value of
                        //  difference
	  x_unit=-1;					// ...and set negative unit
                        //  in x dimension
  }
  else x_unit=1;				// Else set positive unit in
                        //  y dimension

  error_term=0;			// Initialize error term
  if (xdiff>ydiff) {		// If difference is bigger in
                        //  x dimension
	int length, i;

	  length=xdiff+1;	// ...prepare to count off in
                        //  x direction
    for (i=0; i<length; i++) {  // Loop through points
                                    //  in x direction
      screen[offset]=color;	// Set the next pixel in the
                            //  line to COLOR
      offset+=x_unit;				// Move offset to next pixel
                            //  in x direction
      error_term+=ydiff;		// Check to see if move
                            // required in y direction
      if (error_term>xdiff) {	// If so...
        error_term-=xdiff;		// ...reset error term
        offset+=y_unit;				// ...and move offset to next
                              //  pixel in y dir.
		  }
	  }
  }
  else {								// If difference is bigger in
    int length, i;                    //  y dimension
    length=ydiff+1;	// ...prepare to count off in
                       //  y direction
	
    for (i=0; i<length; i++) {	// Loop through points
                                    //  in y direction
      screen[offset]=color;	// Set the next pixel in the
                            //  line to COLOR
      offset+=y_unit;				// Move offset to next pixel
                            //  in y direction
      error_term+=xdiff;    // Check to see if move
                            //  required in x direction
      if (error_term>0) {		// If so...
        error_term-=ydiff;	// ...reset error term
        offset+=x_unit;			// ...and move offset to next
                            //  pixel in x dir.
      }
    }
  }
}
