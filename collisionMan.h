/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: collisionMan.h                                 *
*        Creation Date: 18/10/01                                  *
*        Description:	Collision Manager.       				      *
*                       A collection of utility functions for     *
*                       collision detection and responce calling  *
*                                                                 *                                                                 
*                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*        SD    27/10/01    Added Rect_Intersect function.         *
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
#ifndef COLMANINC
#define COLMANINC

#include <Agb.h>
#include "Char.h"
#include "HBDefines.h"
#include "Types3d.h"
/***************************
*  Defines	               *
****************************/

/***************************
*  Types		               *
***************************/

/***************************
*  Function Prototypes     *
****************************/
s32 Collision_ActToPoly(ACTOR_TYP *pAct1, polygon_type *pPoly, s32 distance, s32 x, s32 y);

/******************************************************************************************
* Function:		s32 Rect_Offset( RECT_PTR pRect, s32 x, s32 y )                            * 
*                                                                                         *
* Description:	Offsets a RECT by the specified x and y value.                             *
*                                                                                         *
* Args:		   pRect:   A pointer to the RECT to offset.                                  *
*              x:       x offset.               											         *
*              y:       y offset.               											         *
*                                                                                         *
* Returns:	   s32:		 Returns 0 if successful else an error code. see HBDefines.h      *
******************************************************************************************/
s32 Rect_Offset( RECT_TYP *pRect, s32 x, s32 y );


/******************************************************************************************
* Function:		s32 Rect_Intersect( RECT_PTR pRect1, RECT_PTR pRect2 )                     * 
*                                                                                         *
* Description:	This tests if pRect1 and pRect2 intersect.                                 *
*                                                                                         *
* Args:		   pRect1:   Pointer to source RECT.                                          *
*              pRect2:   Pointer to source RECT.											         *
*                                                                                         *
* Returns:	   s32:		Returns 1 if intersection or 0 if no intersection                 *
******************************************************************************************/
//s32 Rect_Intersect( RECT_TYP *pRect1, RECT_TYP *pRect2 );


/******************************************************************************************
Function:		s32 Collision_CHARS( CHAR_PTR pCHAR1, CHAR_PTR pCHAR2, pResponceFunc_CHAR_TO_CHAR pResponce);									
																														
Description:	This tests if pCHAR1 has collided with pCHAR2 and if so call their 
               respective collision responce functions											
																														
Args:			pCHAR1:	      Pointer to test for CHAR.
				pCHAR2:	      Pointer to test against CHAR.																		
                        																														
Returns:		s32:		Returns 1 on collision or 0 if no collision	or invalid CHARs			
*******************************************************************************************/
s32 Collision_ACTORS( ACTOR_TYP *pAct1, ACTOR_TYP *pAct2,s32 distance);
s32 Collision_ACTOR_TO_WORLDDB( ACTOR_TYP *pAct1 );

//s32 Collision_LEVEL_CHARS( CHAR_TYP *pCHAR1, u32 whichLayers );
/******************************************************************************************
Function:		s32 Collision_CHAR_LEVEL( CHAR_PTR pCHAR, s32 *pCollisionMap, u32 numLines, pResponceFunc pResponce)									
																														
Description:	This tests if pCHAR has collided with a collision map, defined by lines with attribs
               that specify wether a collision has occured if point has crossed line from one side to the other.
               if Collision has occured then call the CHARs respective collision responce function
																														
Args:			pCHAR:	         Pointer to test for CHAR.
				pCollisionMap: Pointer to the collision map.
            numLines:      The number of lines to test against
                        																														
Returns:		s32:		Returns 1 on collision or 0 if no collision	or invalid CHARs			
*******************************************************************************************/
//s32 Collision_CHAR_LEVEL_LINE( CHAR_TYP *pCHAR, const RECTLIST_TYP *pRectList, const GBA_BKG_LAYER_STATS_TYP *pLayerStats );

//s32 Collision_CHARS_LEVEL_RECT( CHAR_TYP *pCHAR1, const RECTLIST_TYP *pRectList, const GBA_BKG_LAYER_STATS_TYP *pLayerStats  );
//s32 Collision_CHARS_LEVEL_RECT_ALL( CHAR_TYP *pCHAR1, u32 whichLayers );

//s32 Collision_CHAR_PATH( CHAR_TYP *pCHAR1, const PATHPOINT_TYP *pPathPoint);

#endif
