/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: sprite.c                                       *
*        Creation Date: 21/04/01                                  *
*        Description: Sprite functions and object structs         *
*                                                                 *                                                                 
*                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*        26/07/01 - David Zurillo											*
*			Renamed variables and functions to coinside with HB		*
*			Programming Conventions												*
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/ 
#include "Char.h"

//u8 animCountMax __attribute ((section(".intwram0"))) = 0;

/****************************
*         Globals           *
*****************************/
//OAMMANAGER oamManager;

u32 curDataPointer;

/****************************
*  Function Definitions      *
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
void Init_CHARs ( void )
{
	
}// End Init_CHARs


/***********************************************************************************************************************************
Function:		s32 Create_CHAR(CHAR_PTR pCHAR, u8 numFrames, u16 attr, u16 oamAttrib0State, u16 oamAttrib1State, u16 oamAttrib2State)   *
                                                                                                                                    *
Description:	Creates and Initializes CHAR.																					                           *
               Setting up oam attributes and retreiving a oam ID and a oam NAME.												                  *
                                                                                                                                    * 	
Args:			pCHAR:		Pointer to the CHAR user wants to create, which is previsouly declared in user code.			                     *
       pCreature:		Pointer to the CREATURE that this CHAR is.														                              *
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
s32 Create_CHAR( CHAR_TYP *pCHAR, const CREATURE_TYP *pCreature )
{  
	// Set creature type
 	pCHAR->pCreature = pCreature;

   // Set CHARs default Oam Attribs from its CREATURE type
	pCHAR->oamAttrib0State = pCreature->oamAttrib0State;
	pCHAR->oamAttrib1State = pCreature->oamAttrib1State;
	pCHAR->oamAttrib2State = pCreature->oamAttrib2State;

	pCHAR->attr					= pCreature->attr;

	pCHAR->BoundingBox.left		= 0;
	pCHAR->BoundingBox.top		= 0;
	pCHAR->BoundingBox.right		= 0;
	pCHAR->BoundingBox.bottom		= 0;
	
	pCHAR->animState			= ANIM_STATE_STOP;	
	pCHAR->curFrame				= 0;		
	pCHAR->curAnimationSeq	= 0;
	pCHAR->curAnimFrameIndex = 0;
	pCHAR->animDelay			= 0;
	pCHAR->animDelayCount		= 0;

	pCHAR->x						= 0;
	pCHAR->y						= 0;
	pCHAR->oldX					= 0;
	pCHAR->oldY					= 0;
  	pCHAR->xv						= 0;
	pCHAR->yv						= 0;
	pCHAR->angle					= 0;
	pCHAR->scaleX				= 1<<8;
   pCHAR->scaleY				= 1<<8;

	pCHAR->direction			= CHAR_DIR_STOP;

	pCHAR->name					= 0;
	// old shite must check it
//	STATE_ON( pCHAR->state, CHAR_STATE_ALIVE );

   // return success
	return ( OK );
	
} // end Create_CHAR

/******************************************************************************************
Function:		s32 Destroy_CHAR( CHAR_PTR pCHAR )                                  		      *
                                                                                          *
Description:	NOT YET IMPLEMENTED.														               *
                                                                                          *
Args:			pCHAR:	Pointer to the CHAR to be destroyed									            *
                                                                                          *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR						            *
*******************************************************************************************/
s32 Destroy_CHAR( CHAR_TYP *pCHAR )
{	
  // is this CHAR valid
  if (!pCHAR)
	  return ( ERROR );
	  
  // NOT YET IMPLEMENTED
	  
  return ( OK );			
} // end Destroy_CHAR
  

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
s32 Flip_CHAR( CHAR_TYP *pCHAR, u16 oamAttrib )
{
  // is this a valid CHAR
  if (!pCHAR)
	  return( ERROR );
	  
  if ( oamAttrib & FLIP_HORZ )
  {   
	  pCHAR->oamAttrib1State |= oamAttrib;
  }
	  
  if ( oamAttrib & FLIP_VERT )
  {
	  pCHAR->oamAttrib1State |= oamAttrib;
  }
	  
  return( OK );
}// End Flip_CHAR
  
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
s32 Flip_Reset_CHAR( CHAR_TYP *pCHAR, u16 oamAttrib )
{
  // is this a valid CHAR
  if (!pCHAR)
	  return( ERROR );
	  
  if ( oamAttrib & FLIP_HORZ )
  {
	  pCHAR->oamAttrib1State &= UNFLIP_HORZ;
  }
	  
  if ( oamAttrib & FLIP_VERT )
  {
	  pCHAR->oamAttrib1State &= UNFLIP_VERT;
  }
	  
  return( OK );
}// End Flip_Reset_CHAR
  
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
s32 Move_CHAR( CHAR_TYP *pCHAR )
{
//	s32 tempX, tempY;
	  
  pCHAR->oldX = pCHAR->x;
  pCHAR->oldY = pCHAR->y;

  pCHAR->oldFX = pCHAR->fx;
  pCHAR->oldFY = pCHAR->fy;
  //pCHAR->oldScreenX = pCHAR->screenX;
  //pCHAR->oldScreenY = pCHAR->screenY;

  // translate the CHAR
  if ( pCHAR->direction & CHAR_DIR_RIGHT )
	  pCHAR->fx += pCHAR->xv;
  else if ( pCHAR->direction & CHAR_DIR_LEFT )
	  pCHAR->fx -= pCHAR->xv;
	  
  if ( pCHAR->direction & CHAR_DIR_DOWN )
	  pCHAR->fy += pCHAR->yv;   
  else if ( pCHAR->direction & CHAR_DIR_UP )
	  pCHAR->fy -= pCHAR->yv;   
  
  //pCHAR->screenX = pCHAR->x;
  //pCHAR->screenY = pCHAR->y;

  pCHAR->x = FXTOI(pCHAR->fx);
  pCHAR->y = FXTOI(pCHAR->fy);
  // return success	
  return( OK );
} // end Move_CHAR
  


  
/******************************************************************************************
Function:		s32 Set_Pos_CHAR( CHAR_PTR pCHAR, s32 x, s32 y, s32 *levelPosY )				   *
	                                                                                       *
Description:	This functions sets the postion of a CHAR in relation to the level				*
	                                                                                       *
Args:			pCHAR:	   	Pointer to CHAR to set posistion for								         *
				x: 			x coordinate												                  *
				y:	   		y coordinate				   							                  *																							
            *levelPosY: current level offset value                                        *
				                                                                              *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR							         *
*******************************************************************************************/
s32 Set_Pos_CHAR( CHAR_TYP *pCHAR, s32 x, s32 y )
{	
  // is this a valid CHAR
  if ( !pCHAR )
	  return( ERROR );
	  
  // set posistion
  pCHAR->x = x;
  pCHAR->y = y;

  // return success	
  return( OK );
} // end Set_Pos_CHAR

  

  
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
s32 Set_Vel_CHAR( CHAR_TYP *pCHAR, FIXED xv, FIXED yv )
{
  // is this a valid CHAR
  if ( !pCHAR )
	  return( ERROR );
	  
  // set velocity
  pCHAR->xv = xv;
  pCHAR->yv = yv;
	  
  // return success	
  return( OK );
} // end Set_Vel_CHAR
  
/******************************************************************************************
Function:		s32 Hide_CHAR( CHAR_PTR pCHAR )									   	               *
	                                                                                       *
Description:	This functions hides a CHAR.					 							            *
	                                                                                       *
Args:			pCHAR:	Pointer to CHAR to hide.												               *
                                                                                          *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR						            *
*******************************************************************************************/
s32 Hide_CHAR( CHAR_TYP *pCHAR )
{
 
	//u8 numblocks;  
  // is this a valid CHAR
  if ( !pCHAR )
	  return( ERROR );
	  
  pCHAR->oamAttrib0State |= 0x200;
  
  STATE_OFF(pCHAR->attr, CHAR_ATTR_VISIBLE);
  STATE_ON(pCHAR->attr, CHAR_ATTR_INVISIBLE);
	  
  //numblocks = (pCHAR->width >> 3) * (pCHAR->height >> 3);
  //numblocks += numblocks;
  
  
  //oamManager.oam[(pCHAR->ID)*4] = pCHAR->oamAttrib0State | pCHAR->y;
  //oamManager.oam[(pCHAR->ID)*4+1] = pCHAR->oamAttrib1State | pCHAR->x;									//numblocks
  //oamManager.oam[(pCHAR->ID)*4+2] = pCHAR->oamAttrib2State | (pCHAR->oamName + (pCHAR->currFrame  * numblocks));//(CHAR->currFrame*2) ;
 
  // return success
  return( OK );
} // end Hide_CHAR
  
/******************************************************************************************
Function:		s32 Show_CHAR( CHAR_PTR pCHAR )									   	               *
	                                                                                       *
Description:	This functions shows a hidden CHAR.					 					            *
	                                                                                       *
Args:			pCHAR:	Pointer to CHAR to hide.												               *
	                                                                                       *
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR						            *
*******************************************************************************************/
s32 Show_CHAR( CHAR_TYP *pCHAR )
{
  // is this a valid CHAR
  if ( !pCHAR )
	  return( ERROR );
	  
  pCHAR->oamAttrib0State &= 0x7DFF;
  //pCHAR->attr |= CHAR_ATTR_VISIBLE;
  STATE_OFF(pCHAR->attr, CHAR_ATTR_INVISIBLE);
  STATE_ON(pCHAR->attr, CHAR_ATTR_VISIBLE);
	  
  // return success
  return( OK );
} // end Show_CHAR
  
				
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
s32 Clone_CHAR( CHAR_TYP *pSrcCHAR, CHAR_TYP *pDestCHAR )
{
	// is this a valid CHAR
	if (!pSrcCHAR)
		return( ERROR );
					
	// is this a valid CHAR
	if (!pDestCHAR)
		return( ERROR );
					
	// set state and attributes of CHAR
	pDestCHAR->pCreature				= pSrcCHAR->pCreature;

	pDestCHAR->oamAttrib0State		|= pSrcCHAR->oamAttrib0State;
	pDestCHAR->oamAttrib1State		|= pSrcCHAR->oamAttrib1State;
	pDestCHAR->oamAttrib2State		|= pSrcCHAR->oamAttrib2State;

	pDestCHAR->attr						= pSrcCHAR->attr;

	pDestCHAR->BoundingBox.left		= pSrcCHAR->BoundingBox.left;
	pDestCHAR->BoundingBox.top		= pSrcCHAR->BoundingBox.top;
	pDestCHAR->BoundingBox.right		= pSrcCHAR->BoundingBox.right;
	pDestCHAR->BoundingBox.bottom		= pSrcCHAR->BoundingBox.bottom;
	
	pDestCHAR->animState				= pSrcCHAR->animState;
	pDestCHAR->curFrame				= pSrcCHAR->curFrame;	
	pDestCHAR->curAnimationSeq		= pSrcCHAR->curAnimationSeq;
	pDestCHAR->curAnimFrameIndex	= pSrcCHAR->curAnimFrameIndex;
	pDestCHAR->animDelay				= pSrcCHAR->animDelay;
	pDestCHAR->animDelayCount		= pSrcCHAR->animDelayCount;

	pDestCHAR->x							= pSrcCHAR->x;
	pDestCHAR->y							= pSrcCHAR->y;
	pDestCHAR->oldX						= pSrcCHAR->oldX;
	pDestCHAR->oldY						= pSrcCHAR->oldY;
	pDestCHAR->xv						= pSrcCHAR->xv;	
	pDestCHAR->yv						= pSrcCHAR->yv;
   
	pDestCHAR->angle					= pSrcCHAR->angle;
	pDestCHAR->scaleX					= pSrcCHAR->scaleX;
	pDestCHAR->scaleY					= pSrcCHAR->scaleY;

	pDestCHAR->direction				= pSrcCHAR->direction;

	pDestCHAR->state					= pSrcCHAR->state;

	// return success	
	return( OK );
}

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
s32 Set_Bounding_Box_CHAR( CHAR_TYP *pCHAR, const RECT_TYP *pBoundRect )
{
	
	pCHAR->BoundingBox.left = pBoundRect->left;
	pCHAR->BoundingBox.top = pBoundRect->top;
	pCHAR->BoundingBox.right = pBoundRect->right;
	pCHAR->BoundingBox.bottom = pBoundRect->bottom;
	// return success
   return( OK );	
}
