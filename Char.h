 /******************************************************************
*        Author: David Zurillo                                    *
*        Filename: sprite.h                                       *
*        Creation Date: 21/04/01                                  *
*        Description: CHAR functions and object structs				*
*                                                                 *                                                                 
*                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*        CHAR has a CREATURE type and CHAR is a SPRITE type         *
*			CREATURE->CHAR->SPRITE												*
*							                                             *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/ 
#ifndef SPRITEINC
#define SPRITEINC

#include <Agb.h>
//#include "palette.h"
#include "HBDefines.h"
#include "HBTypes.h"
#include "Trig.h"

/***************************
*  Defines                 *
****************************/

/****************************
*  Function Prototypes      *
*****************************/

/******************************************************************************************
Function:		void Init_CHARs( void )                                                     *
                                                                                          *
Description:	Initialise the Oam Manager. Init_CHARs must be                              *
called before any CHAR creation.					                                             *
                                                                                          *
Args:			None                                                                          *
                                                                                          *
Returns:		None                                                                          *
*******************************************************************************************/
void Init_CHARs ( void );

/***********************************************************************************************************************************
Function:		s32 Create_CHAR(CHAR_PTR pCHAR, u8 numFrames, u16 attr, u16 oamAttrib0State, u16 oamAttrib1State, u16 oamAttrib2State)   *
                                                                                                                                    *
Description:	Creates and Initializes CHAR.																					                           *
               Setting up oam attributes and retreiving a oam ID and a oam NAME.												                  *
                                                                                                                                    * 	
Args:			pCHAR:		Pointer to the CHAR user wants to create, which is previsouly declared in user code.			                     *
       numFrames:		The total number of frames the CHAR has.														                              *
            attr:		Intial CHAR attributes, defined in sprite.h CHAR_ATTR_XXXXXX									                           *
 oamAttrib0State:    Internal oam attribute 0. Generally a user will only need to set the color palette size.	                     *
                     Refer to oamMan.h for furthur defines for oam attribute 0									                           *
 oamAttrib1State:	   Internal oam attribute 1. Generally a user will only need to set the size of the CHAR.		                     *
                     Refer to oamMan.h for furthur defines for oam attribute 1.									                           *
 oamAttrib2State:	   Internal oam attribute 2. Generally a user will only need to set the priority of the CHAR.	                     *    	
                     Refer to oamMan.h for furthur defines for oam attribute 2.									                           *
                                                                                                                                    *
Returns:		s32:		Upon successful creation of CHAR Create_CHAR will return 1, else failure to will return 0		                  *                                             *
***********************************************************************************************************************************/
s32 Create_CHAR( CHAR_TYP *pCHAR, const CREATURE_TYP *pCreature );              


/******************************************************************************************
Function:		s32 Destroy_CHAR( CHAR_PTR pCHAR )                                  		      *
                                                                                          *
Description:	NOT YET IMPLEMENTED.														               *
                                                                                          *
Args:			pCHAR:	Pointer to the CHAR to be destroyed									            *
                                                                                          *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR						            *
*******************************************************************************************/
s32 Destroy_CHAR( CHAR_TYP *pCHAR );

/******************************************************************************************
Function:		s32 Flip_CHAR( CHAR_PTR pCHAR, u16 oamAttrib )					   	    	      *
                                                                                          *
Description:	Flips a CHAR either horizontally or vertically								      *
                                                                                          *
Args:			pCHAR:		   Pointer to CHAR to be flipped									            *
            oamAttrib:	Which axis to flip CHAR. Either FLIP_HORZ or FLIP_VERT			      *
                                                                                          *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR							         *
*******************************************************************************************/
s32 Flip_CHAR( CHAR_TYP *pCHAR, u16 oamAttrib );


/******************************************************************************************
Function:		Flip_Reset_CHAR( CHAR_PTR pCHAR, u16 oamAttrib )				   	    	      *
                                                                                          *
Description:	Resets a Flipped CHAR either horizontally or vertically					      *
                                                                                          *
Args:			pCHAR:		   Pointer to CHAR to be reset flipped								         *
            oamAttrib:	Which axis to reset flip CHAR. Either FLIP_HORZ or FLIP_VERT	      *
                                                                                          *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR							         *
*******************************************************************************************/
s32 Flip_Reset_CHAR( CHAR_TYP *pCHAR, u16 oamAttrib );


/******************************************************************************************
Function:		s32 Move_CHAR( CHAR_PTR pCHAR )									   	    	         *
                                                                                          *
Description:	This function moves the CHAR based on its current velocity				      *
               also, the function test for various motion attributes of the			      *
               CHAR and takes the appropriate actions.										         *
                                                                                          *
Args:			pCHAR:		Pointer to CHAR to be moved										               *
                                                                                          *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR							         *
*******************************************************************************************/
s32 Move_CHAR( CHAR_TYP *pCHAR );


/******************************************************************************************
Function:		s32 Set_Pos_CHAR( CHAR_PTR pCHAR, s16 x, s16 y )				   	    	         *
	                                                                                       *
Description:	This functions sets the postion of a CHAR									         *
	                                                                                       *
Args:			pCHAR:		Pointer to CHAR to set posistion for								            *
				x:			x coordinate													                  *
				y:			y coordinate													                  *																							
				                                                                              *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR							         *
*******************************************************************************************/
s32 Set_Pos_CHAR( CHAR_TYP *pCHAR, s32 x, s32 y);

/******************************************************************************************
Function:		s32 Set_Level_Pos_CHAR( CHAR_PTR pCHAR, s32 x, s32 y )				   	    	         *
	                                                                                       *
Description:	This functions intializes the postion of a CHAR in relation to the level.	*
               Therefore should only be called on initalization of CHAR, as other functions*
               convert to level position automatically based on values user sets in this  *
               function.                                                                  *
	                                                                                       *
Args:			pCHAR:		Pointer to CHAR to set posistion for								            *
				x:			x coordinate													                  *
				y:			y coordinate													                  *																							
				                                                                              *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR							         *
*******************************************************************************************/
s32 Set_Level_Pos_CHAR( CHAR_TYP *pCHAR, s32 x, s32 y );

/******************************************************************************************
Function:		s32 Set_Vel_CHAR( CHAR_PTR pCHAR,s8 xv, s8 yv )					      	         *
	                                                                                       *
Description:	This function sets the X and Y velocity of a CHAR.				    	         *
	                                                                                       *
Args:			pCHAR:	Pointer to CHAR to set the velocity for.							            *
				xv:		Velocity along X axis												            *
				xy:		Velocity along Y axis												            *
				                                                                              *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR						            *
*******************************************************************************************/
s32 Set_Vel_CHAR( CHAR_TYP *pCHAR, FIXED xv, FIXED yv);


/******************************************************************************************
Function:		s32 Hide_CHAR( CHAR_PTR pCHAR )									   	               *
	                                                                                       *
Description:	This functions hides a CHAR.					 							            *
	                                                                                       *
Args:			pCHAR:	Pointer to CHAR to hide.												               *
                                                                                          *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR						            *
*******************************************************************************************/
s32 Hide_CHAR( CHAR_TYP *pCHAR );


/******************************************************************************************
Function:		s32 Show_CHAR( CHAR_PTR pCHAR )									   	               *
	                                                                                       *
Description:	This functions shows a hidden CHAR.					 					            *
	                                                                                       *
Args:			pCHAR:	Pointer to CHAR to hide.												               *
	                                                                                       *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR						            *
*******************************************************************************************/
s32 Show_CHAR( CHAR_TYP *pCHAR );

/******************************************************************************************
Function:		s32 Clone_CHAR( CHAR_PTR pSrcCHAR, CHAR_PTR pDestCHAR )									*
																														*
Description:	This function clones a CHAR, the cloned CHAR posistion, velocity, animtaion	*
					sequences etc ( ALL ) states and attributes are replicated in newly cloned	*
					CHAR																								*
																														*
Args:			pSrcCHAR:	Pointer to source CHAR.																*										
				pDestCHAR:	Pointer to destinatation CHAR that will be the clone.					*
																														*
Returns:		s32:		Returns 1 on success or 0 if invalid CHARs										*		
*******************************************************************************************/
s32 Clone_CHAR( CHAR_TYP *pSrcCHAR, CHAR_TYP *pDestCHAR );

/********************************************************************************************************
Function:		s32 Set_Collision_Rect_CHAR(CHAR_PTR CHAR, u16 xoffset, u16 yoffset, u16 width, u16 height)   *
	                                                                                                      *
Description:	This sets a CHARs collision rectangle to any given dimension and offset to CHARs			   *
				   posistion.					 															                     *
				                                                                                             *
Args:			pCHAR:				Pointer to CHAR to hide.	
				numBoundBox:	The bounding box ID(number defined in the creatures header file)
			                                                                                                *
Returns:		s32:		Returns OK on success or ERROR if invalid CHAR										               *	
********************************************************************************************************/
s32 Set_Bounding_Box_CHAR( CHAR_TYP *pCHAR, const RECT_TYP *pBoundRect );

#endif
