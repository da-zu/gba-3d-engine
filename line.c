#include "line.h"

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
/*Replace the part (or line of code) where the pixel is drawn 
to a section that checks the current offset against the left/right 
offsets of the current Y line and updates if necessary. 
When the Y changes (in the major for Y-biased lines (abs(slope) > 1)
 and in the minor for X-biased lines (abs(slope) < 1) ), update the 
 current Y line in the buffer so you know which left/right offsets 
 to check against. The first Y will be the Y value from the first 
 point, at the start of the line. 
*/
void Get_Next_Line_Pos( u32 maxLength, s16 startX, s16 startY, s16 endX, s16 endY, s16 *pNextPosX, s16 *pNextPosY)
{	
   u32 i;
	s16 x_unit, y_unit, y_diff, x_diff, x_count, y_count;		// varibales for amount of change in x and y
	s16 error_term;
	s16 length;
	s16 loop_counter;
	
	y_diff = endY - startY;		// difference between y coordinates
	x_diff = endX - startX;		// difference between x coordinates
	
	error_term = 0;

	if ( y_diff < 0 )		// if the line moves in the negative direction
	{
		y_diff = -y_diff;	// ...get absolute value of the difference
		y_unit = -1;		// ...and set negative unit in y dimension
      y_count = -1;
	}
	else					// else set positive unit in the y dimension
	{
		y_unit = 1;
      y_count = 1;
	}

	if ( x_diff < 0 )		// if the line moves in the negative direction
	{
		x_diff = -x_diff;	// ...get absolute value of the difference
		x_unit = -1;		// ...and set negative unit in y dimension
      x_count = -1;
	}
	else					// else set positive unit in the y dimension
	{
		x_unit = 1;
      x_count = 1;
	}

   
	if ( x_diff > y_diff )	// if difference is bigger in x dimension
	{
		length = x_diff + 1;//...prepare to count off in x direction
		//length = length >> 1;
    
      for ( i = 0; i < length; i++ )
      {
         pNextPosX[i] = startX;
         pNextPosY[i] = startY;
      }

      for ( loop_counter = 1; loop_counter < length; loop_counter++ )	// loop through points in x direction
		{         
			pNextPosX[loop_counter] += (x_unit+=x_count);		// move offset to next pixel in x direction
		     
	      pNextPosY[loop_counter] = pNextPosY[loop_counter-1];
			error_term += y_diff;	// check to see if move required in y direction

			if ( error_term > x_diff )	// if so...
			{
				error_term -= x_diff;	//...reset error term
				pNextPosY[loop_counter] += y_unit;		//...and move offset to next pixekl in y direction
			}
		}
	}
	else					// if difference is bigger in y dimension
	{
		length = y_diff + 1;//... prepare to count off in the direction
     
      for ( i = 0; i < length; i++ )
      {
         pNextPosX[i] = startX;
         pNextPosY[i] = startY;
      }

      for ( loop_counter = 1; loop_counter < length; loop_counter++ )	// loop through points in y direction
		{

		   pNextPosY[loop_counter] += (y_unit += y_count);		// move offset to next pixel in y direction
			
         pNextPosX[loop_counter] = pNextPosX[loop_counter-1];

			error_term += x_diff;	// check to see if move required in x direction

			if ( error_term > 0 )	// if so...
			{
				error_term -= y_diff;	//...reset error term
				pNextPosX[loop_counter] += x_unit;		//...and move offset to next pixekl in y direction
			}
		}
	}

}




