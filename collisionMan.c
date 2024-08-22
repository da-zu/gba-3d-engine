 /******************************************************************
*        Author: David Zurillo                                    *
*        Filename: collisionMan.c                                 *
*        Creation Date: 18/10/01                                  *
*        Description:	Collision Manager.       				      *
*                       A collection of utility functions for     *
*                       collision detection and response calling  *
*                                                                 *                                                                 
*                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*        SD    27/10/01    Added Rect_Intersect function.         *
*        DZ		9/11/01		Changed line interesect function to		*
*									find middle of current collision rect	*
*									and create the CHAR lien from that added* 
*									to its velocity                        *
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
#include "collisionMan.h"
#include "Types3d.h"
#include "Actors.h"

/******************************
*  Globals                    *
*******************************/
extern u32 VideoBuffer;
//extern LEVELMAN_TYP levelManager;

//extern s32 LineIntersect( LINEPAIR_PTR pLines, s32 *pX, s32 *pY );// __attribute((section(".iwram"), long_call));
/******************************
*  Function Definitions       *
*******************************/

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
s32 Rect_Offset( RECT_TYP *pRect, s32 x, s32 y )
{
	pRect->left += x;
	pRect->right += x;
	
	pRect->top += y;
	pRect->bottom += y;
	
	return 0;
}


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
s32 Rect_Intersect( RECT_TYP *pRect1, RECT_TYP *pRect2 ) 
{
	u16 width1, width2, height1, height2;
	s16 cx1, cy1, cx2, cy2, dx, dy;
	
	// get the radi of each rect
	width1  = ((pRect1->right - pRect1->left) >>1);
	height1 = ((pRect1->bottom - pRect1->top) >>1);
	
	width2  = ((pRect2->right - pRect2->left) >>1);
	height2 = ((pRect2->bottom - pRect2->top) >>1);
	
	// compute center of each rect
	cx1 = pRect1->left + width1;
	cy1 = pRect1->top + height1;
	
	cx2 = pRect2->left + width2;
	cy2 = pRect2->top + height2;
	
	// compute deltas
	dx = cx2 - cx1;
	if ( dx < 0 )
	{
		dx = -dx;
	}
	
	dy = cy2 - cy1;
	if ( dy < 0 )
	{
		dy = -dy;
	}
	
	// test if rects overlap
	if (dx < (width1+width2) && dy < (height1+height2))
	{
		return 1;
	}
	
	return 0;
}

s32 Collision_ActToPoly(ACTOR_TYP *pAct1, polygon_type *pPoly, s32 distance, s32 x, s32 y)
{
	u32 isCollide, i, j;
	
	isCollide = NO;
	
	s32 vx1,vx2,vx3,vy1,vy2,vy3,vz1,vz2,vz3;
	
	vx1 = pPoly->vertex[0]->wx;
	vx2 = pPoly->vertex[1]->wx;
	vx3 = pPoly->vertex[2]->wx;
	
	vy1 = pPoly->vertex[0]->wy;
	vy2 = pPoly->vertex[1]->wy;
	vy3 = pPoly->vertex[2]->wy;
	
	vz1 = pPoly->vertex[0]->wz;
	vz2 = pPoly->vertex[1]->wz;
	vz3 = pPoly->vertex[2]->wz;	
	
	Point3 a,b;
								
	a.x = pAct1->pObj->x;
	a.y = pAct1->pObj->y;
	a.z = pAct1->pObj->z;
				
	b.x = (vx1+vx2+vx3)/3;
	b.y = (vy1+vy2+vy3)/3;
	b.z = (vz1+vz2+vz3)/3;
				
	//if ( dist <= minDist * minDist)
	if (ABS(V3DistanceBetween2PointsSqr(&a,&b)) < (distance*distance))
	{
		Plane polyPlane;
					Point3 tp;
					s32 oldDist,curDist;
					
					NewPlane(pPoly, &polyPlane);
					
					tp.x = pAct1->pObj->x; 
					tp.y = pAct1->pObj->y;
					tp.z = pAct1->pObj->z; 
					curDist = DistancePointToPlane(&polyPlane, &tp);
					
					tp.x = pAct1->oldx;
					tp.y = pAct1->oldy;
					tp.z = pAct1->oldz;
					oldDist = DistancePointToPlane(&polyPlane, &tp);
					
					  
					//u8 szpoo[10];
					//Font_itoa((s32)curDist,szpoo,10);
					//Gba_Print(50,50,szpoo,VideoBuffer);
					
					if ( ((oldDist >= 0) && (curDist <= 0)) ||
						((oldDist <= 0) && (curDist >= 0)) )
					{
						if (point_in_poly(pPoly, x,y))//pAct1->pObj->x, pAct1->pObj->y))
						{
							return YES;
						}
					}
		/*
		Plane polyPlane;
		Point3 tp;
		s32 oldDist,curDist;
		
		NewPlane(pPoly, &polyPlane);
		
		curDist = DistancePointToPlane(&polyPlane, pCurPoint);
		
		oldDist = DistancePointToPlane(&polyPlane, pOldPoint);
		
		
		//u8 szpoo[10];
		//Font_itoa((s32)curDist,szpoo,10);
		//Gba_Print(50,50,szpoo,VideoBuffer);
		
		if ( ((oldDist >= 0) && (curDist <= 0)) ||
			((oldDist <= 0) && (curDist >= 0)) )
		{
			if (point_in_poly(pPoly, pCurPoint->x, pCurPoint->y))
			{
				return YES;
			}
		}*/
	}
	
	return(NO);	
}
/******************************************************************************************
Function:		s32 Collision_CHARS( CHAR_PTR pCHAR1, CHAR_PTR pCHAR2, pResponceFunc_CHAR_TO_CHAR pResponce);									

  Description:	This tests if pCHAR1 has collided with pCHAR2 and if so call their 
  respective collision responce functions											
  
    Args:			pCHAR1:	      Pointer to test for CHAR.
				pCHAR2:	      Pointer to test against CHAR.																		
				
				  Returns:		s32:		Returns YES on collision or NO if no collision	or invalid CHARs			
*******************************************************************************************/
s32 Collision_ACTORS( ACTOR_TYP *pAct1, ACTOR_TYP *pAct2, s32 distance ) 
{
	u32 isCollide, i, j;
	u32 width1, width2, height1, height2;
	u32 dwidth1, dwidth2, dheight1, dheight2;
	s32 cx1, cy1, cx2, cy2, czx1, czx2, czy1, czy2, dx, dy, dzx, dzy;
	
	
	RECT_TYP CHAR1BoundBox, CHAR2BoundBox;
	RECT_TYP CHAR1BoundBoxZ, CHAR2BoundBoxZ;
	
	s32 minX, minY, maxX, maxY;
	s32 minZ, maxZ;
	
	
	minX = minY = minZ = 2147483647;
	maxX = maxY = maxZ = -2147483647;
	
	// find smallest and largest boundbox dimensions to encomaps laser object
	for (i=0; i<pAct1->pObj->number_of_vertices; i++)
	{
		if(pAct1->pObj->vertex[i].ax < minX)
		{
			minX = pAct1->pObj->vertex[i].ax;
		}
		if(pAct1->pObj->vertex[i].ax > maxX)
		{
			maxX = pAct1->pObj->vertex[i].ax;
		}
		
		if(pAct1->pObj->vertex[i].ay < minY)
		{
			minY = pAct1->pObj->vertex[i].ay;
		}
		if(pAct1->pObj->vertex[i].ay > maxY)
		{
			maxY = pAct1->pObj->vertex[i].ay;
		}
	}
	
	//set object bounding box to values found above
	CHAR1BoundBox.left = minX;
	CHAR1BoundBox.top = minY;
	CHAR1BoundBox.right = maxX; 
	CHAR1BoundBox.bottom = maxY;
	
	
	minX = minY = minZ = 2147483647;
	maxX = maxY = maxZ = -2147483647;
	
	// find smallest and largest boundbox dimensions to encomaps object
	for (i=0; i<pAct2->pObj->number_of_vertices; i++)
	{
		if(pAct2->pObj->vertex[i].ax < minX)
		{
			minX = pAct2->pObj->vertex[i].ax;
		}
		if(pAct2->pObj->vertex[i].ax > maxX)
		{
			maxX = pAct2->pObj->vertex[i].ax;
		}
		
		if(pAct2->pObj->vertex[i].ay < minY)
		{
			minY = pAct2->pObj->vertex[i].ay;
		}
		if(pAct2->pObj->vertex[i].ay > maxY)
		{
			maxY = pAct2->pObj->vertex[i].ay;
		}
		
		
	}
	
	//set object bounding box to values found above
	CHAR2BoundBox.left = minX;
	CHAR2BoundBox.top = minY;
	CHAR2BoundBox.right = maxX; 
	CHAR2BoundBox.bottom = maxY;
	
	// get the radi of each rect		
	width1  = ((CHAR1BoundBox.right - CHAR1BoundBox.left));// - (CHAR1->width>>3);
	height1 = ((CHAR1BoundBox.bottom - CHAR1BoundBox.top));// - (CHAR1->height>>3);
	
	width2  = ((CHAR2BoundBox.right - CHAR2BoundBox.left));// - (CHAR2->width>>3);
	height2 = ((CHAR2BoundBox.bottom - CHAR2BoundBox.top));// - (CHAR2->height>>3);
	
	// compute center of each rect
	cx1 = CHAR1BoundBox.left + width1>>1;
	cy1 = CHAR1BoundBox.top + height1>>1;
	
	cx2 = CHAR2BoundBox.left + width2>>1;
	cy2 = CHAR2BoundBox.top + height2>>1;
	
	// compute deltas
	dx = cx2 - cx1;
	if ( dx < 0 )
		dx = -dx;
	
	dy = cy2 - cy1;
	if ( dy < 0 )
		dy = -dy;
	
	
	isCollide = NO;
	
	if (pAct2->pObj->isCheckPolygonCollision == 0)
	{
		if (dx < (width1+width2) && dy < (height1+height2))//&& dzy < (dheight1+dheight2) && dzx < (dwidth1+dwidth2))
		{		
			Point3 a,b;
			
			a.x = pAct1->pObj->x;
			a.y = pAct1->pObj->y;
			a.z = pAct1->pObj->z;
			
			b.x = pAct2->pObj->x;
			b.y = pAct2->pObj->y;
			b.z = pAct2->pObj->z;
			
			if (ABS(V3DistanceBetween2PointsSqr(&a,&b)) < (distance*distance))
				//if (ABS(pAct1->pObj->z - pAct2->pObj->z) > 50)
			{
				
				isCollide = YES;
				return YES;
			}
			
		}		
	}
	else if (pAct2->pObj->isCheckPolygonCollision == 1)
	{				
		
		isCollide = NO;
		for (j=0; j<pAct2->pObj->number_of_polygons; j++)
		{		
			s32 vx1,vx2,vx3,vy1,vy2,vy3,vz1,vz2,vz3;


			vx1 = pAct2->pObj->polygon[j].vertex[0]->ax;
			vx2 = pAct2->pObj->polygon[j].vertex[1]->ax;
			vx3 = pAct2->pObj->polygon[j].vertex[2]->ax;

			vy1 = pAct2->pObj->polygon[j].vertex[0]->ay;
			vy2 = pAct2->pObj->polygon[j].vertex[1]->ay;
			vy3 = pAct2->pObj->polygon[j].vertex[2]->ay;

			vz1 = pAct2->pObj->polygon[j].vertex[0]->az;
			vz2 = pAct2->pObj->polygon[j].vertex[1]->az;
			vz3 = pAct2->pObj->polygon[j].vertex[2]->az;
		

			minX = minY = minZ = 2147483647;
			maxX = maxY = maxZ = -2147483647;

			if (vx1 < minX)
				minX = vx1;
			if (vx1 > maxX)
				maxX = vx1;
			if (vx2 < minX)
				minX = vx2;
			if (vx2 > maxX)
				maxX = vx2;
			if (vx3 < minX)
				minX = vx3;
			if (vx3 > maxX)
				maxX = vx3;

			if (vy1 < minY)
				minY = vy1;
			if (vy1 > maxY)
				maxY = vy1;
			if (vy2 < minY)
				minY = vy2;
			if (vy2 > maxY)
				maxY = vy2;
			if (vy3 < minY)
				minY = vy3;
			if (vy3 > maxY)
				maxY = vy3;

			
			CHAR2BoundBox.left = minX;
			CHAR2BoundBox.top = minY;
			CHAR2BoundBox.right = maxX; 
			CHAR2BoundBox.bottom = maxY;

			width2  = ((CHAR2BoundBox.right - CHAR2BoundBox.left));// - (CHAR2->width>>3);
			height2 = ((CHAR2BoundBox.bottom - CHAR2BoundBox.top));// - (CHAR2->height>>3);
	
			// compute center of each rect
			cx2 = CHAR2BoundBox.left + width2>>1;
			cy2 = CHAR2BoundBox.top + height2>>1;

			// compute deltas
			dx = cx2 - cx1;
			if ( dx < 0 )
				dx = -dx;
	
			dy = cy2 - cy1;
			if ( dy < 0 )
				dy = -dy;

			if (dx < (width1+width2) && dy < (height1+height2))
			{
				Point3 a,b;
								
				a.x = pAct1->pObj->ax;
				a.y = pAct1->pObj->ay;
				a.z = pAct1->pObj->az;
				
				b.x = (vx1+vx2+vx3)/3;
				b.y = (vy1+vy2+vy3)/3;
				b.z = (vz1+vz2+vz3)/3;
				
				//if ( dist <= minDist * minDist)
				if (ABS(V3DistanceBetween2PointsSqr(&a,&b)) < (200*200))
				{
					Plane polyPlane;
					Point3 tp;
					s32 oldDist,curDist;
					
					NewPlane(&pAct2->pObj->polygon[j], &polyPlane);
					
					tp.x = pAct1->pObj->x; 
					tp.y = pAct1->pObj->y;
					tp.z = pAct1->pObj->z; 
					curDist = DistancePointToPlane(&polyPlane, &tp);
					
					tp.x = pAct1->oldx;
					tp.y = pAct1->oldy;
					tp.z = pAct1->oldz;
					oldDist = DistancePointToPlane(&polyPlane, &tp);
					
					
					//u8 szpoo[10];
					//Font_itoa((s32)curDist,szpoo,10);
					//Gba_Print(50,50,szpoo,VideoBuffer);
					
					if ( ((oldDist >= 0) && (curDist <= 0)) ||
						((oldDist <= 0) && (curDist >= 0)) )
					{
						if (point_in_poly(&pAct2->pObj->polygon[j], pAct1->pObj->x, pAct1->pObj->y+50))
						{
							return YES;
						}
					}
				}
				/*
				Point3 a,b;
								
				a.x = pAct1->pObj->ax;
				a.y = pAct1->pObj->ay;
				a.z = pAct1->pObj->az;
				
				b.x = (vx1+vx2+vx3)/3;
				b.y = (vy1+vy2+vy3)/3;
				b.z = (vz1+vz2+vz3)/3;
				
				
				//if ( dist <= minDist * minDist)
				if (ABS(V3DistanceBetween2PointsSqr(&a,&b)) < (200*200))
				{
					Gba_Print(50,50,"1234",VideoBuffer);
					//if (point_in_poly(&pAct2->pObj->polygon[j],pAct1->pObj->ax, pAct1->pObj->ay))
					{
						return YES; 						
					}
				}
				*/
			}
		}
	}
	
	return(isCollide);	
} // end Collision_CHARS


s32 Collision_ACTOR_TO_WORLDDB( ACTOR_TYP *pAct1)
{
	return(NO);	

} // end Collision_CHARS
s32 Collision_ACTOR_TO_WORLDDB2( ACTOR_TYP *pAct1)
{
	return(NO);	

} // end Collision_CHARS

/*
s32 Collision_LEVEL_CHARS( CHAR_TYP *pCHAR1, u32 whichLayers ) 
{
	u32 j, numColChars;
	u32 isCollide;

	u16 width1, width2, height1, height2;
	s16 cx1, cy1, cx2, cy2, dx, dy;
	
	RECT_TYP CHAR1BoundBox, CHAR2BoundBox;
	CHAR_TYP *pCHAR2;
	
	
	CHAR1BoundBox = pCHAR1->BoundingBox;
	
	// get the radi of each rect		
	width1  = ((CHAR1BoundBox.right - CHAR1BoundBox.left)>>1);// - (CHAR1->width>>3);
	height1 = ((CHAR1BoundBox.bottom - CHAR1BoundBox.top)>>1);// - (CHAR1->height>>3);
	// compute center of each rect
	cx1 = (pCHAR1->x + CHAR1BoundBox.left ) + width1;
	cy1 = (pCHAR1->y + CHAR1BoundBox.top ) + height1;
	
	isCollide = NO;

	for ( j=0; j<levelManager.pCurLEVEL->numLayers; j++ )
	{		
		u32 k,y;
		k=1;
		for ( y=0; y<j+1; y++)
		{
			k = k*2;
		}
		if ( whichLayers & k )
		{
			u32 i;
			
			 ***********************************88
			// numChars for each layer also needs a starting entry into
			// CHARS_ViewList so we now where the layers chars start
			**************************************
			numColChars = levelManager.pCurLEVEL->LAYERS[j].numChars;
			
			
			for ( i = levelManager.pCurLEVEL->LAYERS[j].charsListEntry; i < numColChars; i++)
			{
				
				pCHAR2 = &levelManager.CHARS_ViewList[i];
				
				if (!(pCHAR2->stateMan.curState & CHAR_STATE_DEAD) ) 
				{
					CHAR2BoundBox = pCHAR2->BoundingBox;
					
					// get the radi of each rect
					width2  = ((CHAR2BoundBox.right - CHAR2BoundBox.left)>>1);// - (CHAR2->width>>3);
					height2 = ((CHAR2BoundBox.bottom - CHAR2BoundBox.top)>>1);// - (CHAR2->height>>3);
					// compute center of each rect
					cx2 = (pCHAR2->x + CHAR2BoundBox.left ) + width2;
					cy2 = (pCHAR2->y + CHAR2BoundBox.top ) + height2;
					
					// compute deltas
					dx = cx2 - cx1;
					if ( dx < 0 )
						dx = -dx;
					
					dy = cy2 - cy1;
					if ( dy < 0 )
						dy = -dy;
					
					// test if rects overlap
					if (dx < (width1+width2) && dy < (height1+height2))
					{
						
						// Call the application defined responce function for the CHARs for collision
						//(*pResponce)(pCHAR1, pCHAR2, COLLIDE_YES);
						// Test if one of the characters is the main char and call appropiate PLAYER TO NPC responce function for CHARs
						if ( (pCHAR1->pCreature->attr & CHAR_ATTR_MAINCHAR) || (pCHAR2->pCreature->attr & CHAR_ATTR_MAINCHAR) )
						{			
							(*pCHAR1->pCreature->pResponce_PLAYER_TO_NPC)(pCHAR1, pCHAR2, COLLIDE_YES);
							(*pCHAR2->pCreature->pResponce_PLAYER_TO_NPC)(pCHAR2, pCHAR1, COLLIDE_YES);
							//(*pCHAR2->pResponce_PLAYER_TO_NPC)(pCHAR2, COLLIDE_YES);
							//(*pCHAR2->pResponce_PLAYER_TO_NPC)(COLLIDE_YES);
						}
						else  // Non of the CHARs are a main character so call the appropiate NPC TO NPC response func
						{
							(*pCHAR1->pCreature->pResponce_NPC_TO_NPC)(pCHAR1, pCHAR2, COLLIDE_YES);
							(*pCHAR2->pCreature->pResponce_NPC_TO_NPC)(pCHAR2, pCHAR1, COLLIDE_YES);
							//(*pCHAR2->pResponce_NPC_TO_NPC)(pCHAR2, COLLIDE_YES);
							//(*pCHAR2->pResponce_NPC_TO_NPC)(COLLIDE_YES);
						}
						
						isCollide = YES;
					}
					else
					{
						if ( (pCHAR1->pCreature->attr & CHAR_ATTR_MAINCHAR) || (pCHAR2->pCreature->attr & CHAR_ATTR_MAINCHAR) )
						{			
							(*pCHAR1->pCreature->pResponce_PLAYER_TO_NPC)(pCHAR1, pCHAR2, COLLIDE_NO);
							(*pCHAR2->pCreature->pResponce_PLAYER_TO_NPC)(pCHAR2, pCHAR1, COLLIDE_NO);
							//(*pCHAR2->pResponce_PLAYER_TO_NPC)(pCHAR2, COLLIDE_YES);
							//(*pCHAR2->pResponce_PLAYER_TO_NPC)(COLLIDE_YES);
						}
						else  // Non of the CHARs are a main character so call the appropiate NPC TO NPC response func
						{
							(*pCHAR1->pCreature->pResponce_NPC_TO_NPC)(pCHAR1, pCHAR2, COLLIDE_NO);
							(*pCHAR2->pCreature->pResponce_NPC_TO_NPC)(pCHAR2, pCHAR1, COLLIDE_NO);
							//(*pCHAR2->pResponce_NPC_TO_NPC)(pCHAR2, COLLIDE_YES);
							//(*pCHAR2->pResponce_NPC_TO_NPC)(COLLIDE_YES);
						}
						if ( isCollide != YES )
							isCollide = NO;
					}
				}
			}
		}
	}
	return(isCollide);	
} // end Collision_CHARS



s32 Collision_CHARS_LEVEL_RECT( CHAR_TYP *pCHAR1, const RECTLIST_TYP *pRectList, const GBA_BKG_LAYER_STATS_TYP *pLayerStats ) 
{
	u16 width1, width2, height1, height2;
	s16 cx1, cy1, cx2, cy2, dx, dy;
	u32 numColObjs;
	u32 i;
	u32 isCollide;
	
	RECT_TYP CHAR1BoundBox, levelBoundBox;
	

	numColObjs = pLayerStats->numCollisionRects;
		
	
	CHAR1BoundBox = pCHAR1->BoundingBox;
	
	//Rect_Offset(&CHAR1BoundBox, pCHAR1->x, pCHAR1->y);
	// get the radi of CHAR rect
	width1  = ((CHAR1BoundBox.right - CHAR1BoundBox.left)>>1);//>>1);// - (CHAR1->width>>3);
	height1 = ((CHAR1BoundBox.bottom - CHAR1BoundBox.top)>>1);//>>1);// - (CHAR1->height>>3);
	
	
	// compute center of CHAR rect
	cx1 = (pCHAR1->x + CHAR1BoundBox.left) + width1;
	cy1 = (pCHAR1->y + CHAR1BoundBox.top ) + height1;

	isCollide = NO;

	for ( i = 0; i < numColObjs; i++)
	{
		levelBoundBox.left = pRectList[i].left;
		levelBoundBox.top = pRectList[i].top;
		levelBoundBox.right = pRectList[i].right;
		levelBoundBox.bottom = pRectList[i].bottom;
		
		width2  = ((levelBoundBox.right - levelBoundBox.left)>>1);// - (CHAR2->width>>3);
		height2 = ((levelBoundBox.bottom - levelBoundBox.top)>>1);// - (CHAR2->height>>3);
		
		// compute center of each rect	
		cx2 = levelBoundBox.left + (width2);
		cy2 = levelBoundBox.top + (height2);
	
		// compute deltas
		dx = cx2 - cx1;
		if ( dx < 0 )
			dx = -dx;
		
		dy = cy2 - cy1;
		if ( dy < 0 )
			dy = -dy;
		
		// test if rects overlap
		if (dx < (width1+width2) && dy < (height1+height2))
		{
			// Call the application defined responce function for the CHARs for collision
			(*pCHAR1->pCreature->pResponce_LEVEL_RECT)(pCHAR1, pRectList[i].attributes, &pRectList[i]);		
			isCollide = YES;
		}
		
	}
	return(isCollide);	
} // end Collision_CHARS

// whichLayers is an anded value of the layers the char is to be tested against
s32 Collision_CHARS_LEVEL_RECT_ALL( CHAR_TYP *pCHAR1, u32 whichLayers ) 
{
	u32 isCollide;
	u16 width1, height1;
	s16 cx1, cy1, dx, dy;
	u32 j;
		
	RECT_TYP CHAR1BoundBox;
	

	CHAR1BoundBox = pCHAR1->BoundingBox;
	
	//Rect_Offset(&CHAR1BoundBox, pCHAR1->x, pCHAR1->y);
	// get the radi of CHAR rect
	width1  = ((CHAR1BoundBox.right - CHAR1BoundBox.left)>>1);//>>1);// - (CHAR1->width>>3);
	height1 = ((CHAR1BoundBox.bottom - CHAR1BoundBox.top)>>1);//>>1);// - (CHAR1->height>>3);
	
	
	// compute center of CHAR rect
	cx1 = (pCHAR1->x + CHAR1BoundBox.left) + width1;
	cy1 = (pCHAR1->y + CHAR1BoundBox.top ) + height1;
	
	isCollide = NO;

	for ( j=0; j<levelManager.pCurLEVEL->numLayers; j++ )
	{		
		u32 k,y;
		k=1;
		for ( y=0; y<j+1; y++)
		{
			k = k*2;
		}
		if ( whichLayers & k )
		{
			
			u32 i;
			u32 numColObjs;			
			u16 width2, height2;
			s16 cx2, cy2, dx, dy;
			
			numColObjs = levelManager.pCurLEVEL->LAYERS[j].numCollisionRects;
						
			
			for ( i = 0; i < numColObjs; i++)
			{
				RECT_TYP levelBoundBox;
				
				levelBoundBox.left = levelManager.pCurLEVEL->LAYERS[j].pCollisionRects[i].left;
				levelBoundBox.top = levelManager.pCurLEVEL->LAYERS[j].pCollisionRects[i].top;
				levelBoundBox.right = levelManager.pCurLEVEL->LAYERS[j].pCollisionRects[i].right;
				levelBoundBox.bottom = levelManager.pCurLEVEL->LAYERS[j].pCollisionRects[i].bottom;
				
				width2  = ((levelBoundBox.right - levelBoundBox.left)>>1);// - (CHAR2->width>>3);
				height2 = ((levelBoundBox.bottom - levelBoundBox.top)>>1);// - (CHAR2->height>>3);
				
				// compute center of each rect	
				cx2 = levelBoundBox.left + (width2);
				cy2 = levelBoundBox.top + (height2);
				
				// compute deltas
				dx = cx2 - cx1;
				if ( dx < 0 )
					dx = -dx;
				
				dy = cy2 - cy1;
				if ( dy < 0 )
					dy = -dy;
				
				// test if rects overlap
				if (dx < (width1+width2) && dy < (height1+height2))
				{
					// Call the application defined responce function for the CHARs for collision
					(*pCHAR1->pCreature->pResponce_LEVEL_RECT)(pCHAR1, levelManager.pCurLEVEL->LAYERS[j].pCollisionRects[i].attributes, &levelManager.pCurLEVEL->LAYERS[j].pCollisionRects[i]);
					isCollide = YES;
				}
				
			}
		}
	}
	return(isCollide);	
} // end Collision_CHARS

s32 Collision_CHAR_PATH( CHAR_TYP *pCHAR1, const PATHPOINT_TYP *pPathPoint) 
{
	u16 width1, width2, height1, height2;
	s16 cx1, cy1, cx2, cy2, dx, dy;
	u32 numColObjs;
	u32 i;
	u32 isCollide;
	
	RECT_TYP CHAR1BoundBox, pointBoundBox;
	

	CHAR1BoundBox = pCHAR1->BoundingBox;
	
	//Rect_Offset(&CHAR1BoundBox, pCHAR1->x, pCHAR1->y);
	// get the radi of CHAR rect
	width1  = ((CHAR1BoundBox.right - CHAR1BoundBox.left)>>1);//>>1);// - (CHAR1->width>>3);
	height1 = ((CHAR1BoundBox.bottom - CHAR1BoundBox.top)>>1);//>>1);// - (CHAR1->height>>3);
	
	
	// compute center of CHAR rect
	cx1 = (pCHAR1->x + CHAR1BoundBox.left) + width1;
	cy1 = (pCHAR1->y + CHAR1BoundBox.top ) + height1;

	isCollide = NO;

	pointBoundBox.left = pPathPoint->x - 4;
	pointBoundBox.top = pPathPoint->y - 4;
	pointBoundBox.right = pPathPoint->x + 4;
	pointBoundBox.bottom = pPathPoint->y + 4;
	
	width2  = ((pointBoundBox.right - pointBoundBox.left)>>1);// - (CHAR2->width>>3);
	height2 = ((pointBoundBox.bottom - pointBoundBox.top)>>1);// - (CHAR2->height>>3);
	
	// compute center of each rect	
	cx2 = pointBoundBox.left + (width2);
	cy2 = pointBoundBox.top + (height2);
	
	// compute deltas
	dx = cx2 - cx1;
	if ( dx < 0 )
		dx = -dx;
	
	dy = cy2 - cy1;
	if ( dy < 0 )
		dy = -dy;
	
	// test if rects overlap
	if (dx < (width1+width2) && dy < (height1+height2))
	{
		// Call the application defined responce function for the CHARs for collision
		//(*pCHAR1->pResponce_LEVEL_RECT)(pCHAR1, pRectList[i].attributes, &levelBoundBox);		
		isCollide = YES;
	}
	
	
	return(isCollide);	
} // end Collision_CHARS


******************************************************************************************
  Function:		s32 Collision_CHAR_LEVEL( CHAR_PTR pCHAR, s32 *pCollisionMap, const COLLISIONDATA_PTR colData)
  
    Description:	This tests if pCHAR has collided with a collision map, defined by lines with attribs
    that specify wether a collision has occured if point has crossed line from one side to the other.
    if Collision has occured then call the CHARs respective collision responce function
    
      Args:			pCHAR:	         Pointer to test for CHAR.
      pCollisionMap: Pointer to the collision map.
      numLines:      The number of lines to test against
      
        Returns:		s32:		Returns YES on collision or NO if no collision	or invalid CHARs			
*******************************************************************************************
s32 Collision_CHAR_LEVEL_LINE( CHAR_TYP *pCHAR, const RECTLIST_TYP *pRectList, const GBA_BKG_LAYER_STATS_TYP *pLayerStats)
{   
	u32 isCollide;
	u32 i, u;
	u32 numLineList, numLines;
	s32 CHARX, CHARY;
	u32 attrib;
	s32 CHARColOffsetX, CHARColOffsetY;
	s32 CHARColHeight, CHARColWidth;
	s32 colResult;
	RECT CHARBoundBox,CHARBoundBox2;
	RECT lineBoundBox;
	
	s32 impactX, impactY;
	u32 mapIndex;
	LINEPAIR testLines;

	// is this a valid CHAR
	if ( !pCHAR )
		return(INVALIDCHAR);
	
	isCollide = NO;
	
	CHARX = pCHAR->x;//SCREEN_TO_LEVEL_X(gLevelData_LEVELMAN.curLevelX, pCHAR->x);
	CHARY = pCHAR->y;//SCREEN_TO_LEVEL_Y(gLevelData_LEVELMAN.curLevelY, pCHAR->y);

	//Generate_Collision_Map2_LEVELMAN(CHARX, CHARY);

	CHARBoundBox = pCHAR->pCreature->pBoundingBox[pCHAR->curBoundBox];
	
	CHARColOffsetX = CHARBoundBox.left;
	CHARColOffsetY = CHARBoundBox.top;
	CHARColHeight = CHARBoundBox.bottom;
	CHARColWidth = CHARBoundBox.right;
	
	CHARBoundBox2 = pCHAR->pCreature->pBoundingBox[0];
	

	Rect_Offset(&CHARBoundBox2, CHARX, CHARY);
	// Loop based on number of lineslist entries
	numLineList = pCollisionMap[0];
	
	for ( u=1; u <= numLineList; u++ )
	{
		mapIndex = pCollisionMap[u];
		
		
		numLines = pColData[mapIndex]->numLines;
		// Process collision map
		for (i=0; i < numLines; i++)
		{          
			s32 startX, startY;
			s32 endX, endY;
			s32 tempEndX, tempEndY, tempStartX, tempStartY;

			attrib = 0;
			colResult = DONT_INTERSECT;
			
			// Get lenght of the line to test against
			startX = pColData[mapIndex]->lines[i].startX;
			startY = pColData[mapIndex]->lines[i].startY;
			endX = pColData[mapIndex]->lines[i].endX;
			endY = pColData[mapIndex]->lines[i].endY;
			
			tempStartX = startX;
			tempStartY = startY;
			tempEndX = endX;
			tempEndY = endY;

			if ( endX < startX )
			{
				tempEndX = startX;
				tempStartX = endX;
			}
			else
			{
				tempEndX = endX;
				tempStartX = startX;
			}

			if ( endY < startY )
			{
				tempEndY = startY;
				tempStartY = endY;
			}
			else
			{
				tempEndY = endY;
				tempStartY = startY;
			}

			lineBoundBox.left	= tempStartX;
			lineBoundBox.top	= tempStartY;
			lineBoundBox.right	= tempEndX;// - tempStartX;
			lineBoundBox.bottom =  tempEndY;// - tempStartY;
			
			//lineBoundBox.left	-= 50;
			//lineBoundBox.top	-= 50;
			//lineBoundBox.right	+= 50;
			//lineBoundBox.bottom += 50;

			
			if (YES == Rect_Intersect(&lineBoundBox, &CHARBoundBox2) )
			{	
				
				if (pCHAR->direction & CHAR_DIR_RIGHT)
				{
					if (pColData[mapIndex]->lines[i].attrib & COL_ACTIVE_LEFT) 
					{
						s32 tempX1, tempX2;
						
						
						tempX1 = CHARX + CHARColOffsetX + CHARColWidth + pCHAR->xv;
						tempX2 = CHARX + CHARColOffsetX;
						
						// line 1
						testLines.startX_1 = tempX1;
						testLines.startY_1 = CHARY + (CHARColOffsetY + (CHARColHeight>>1));
						testLines.endX_1 = tempX2;
						testLines.endY_1 = CHARY + (CHARColOffsetY + (CHARColHeight>>1));
						// line 2
						testLines.startX_2 = startX;
						testLines.startY_2 = startY;
						testLines.endX_2 = endX;
						testLines.endY_2 = endY;
						
						colResult = LineIntersect(&testLines, &impactX, &impactY);
						
						if ( colResult == DO_INTERSECT )
						{                           
							attrib = COL_ACTIVE_LEFT;
							(*pCHAR->pResponce_LEVEL)(pCHAR, pColData[mapIndex]->lines[i].attrib, impactX, impactY);
							isCollide = YES;
							
						}
					}
				}
				if (pCHAR->direction & CHAR_DIR_LEFT)
				{
					if (pColData[mapIndex]->lines[i].attrib & COL_ACTIVE_RIGHT) 
					{
						s32 tempX1, tempX2;
						
						tempX1 = CHARX + CHARColOffsetX + CHARColWidth + pCHAR->xv;
						tempX2 = CHARX + CHARColOffsetX; 
						
						// line 1
						testLines.startX_1 = tempX1;
						testLines.startY_1 = CHARY + (CHARColOffsetY + (CHARColHeight>>1));
						testLines.endX_1 = tempX2;
						testLines.endY_1 = CHARY + (CHARColOffsetY + (CHARColHeight>>1));
						// line 2
						testLines.startX_2 = startX;
						testLines.startY_2 = startY;
						testLines.endX_2 = endX;
						testLines.endY_2 = endY;
						
						colResult = LineIntersect(&testLines, &impactX, &impactY);
						
						if ( colResult == DO_INTERSECT )
						{                           
							attrib = COL_ACTIVE_RIGHT;
							(*pCHAR->pResponce_LEVEL)(pCHAR, pColData[mapIndex]->lines[i].attrib, impactX, impactY);
							isCollide = YES;
							
						}
					}
				}
				if (pCHAR->direction & CHAR_DIR_UP)
				{
					if (pColData[mapIndex]->lines[i].attrib & COL_ACTIVE_BOTTOM) 
					{
						s32 tempY1, tempY2;
						
						tempY1 = CHARY + CHARColOffsetY + pCHAR->yv;
						tempY2 = CHARY + CHARColOffsetY + CHARColHeight;
						// line 1
						testLines.startX_1 = CHARX + (CHARColOffsetX + (CHARColWidth>>1));
						testLines.startY_1 = tempY1;
						testLines.endX_1 = CHARX + (CHARColOffsetX + (CHARColWidth>>1));
						testLines.endY_1 = tempY2;
						// line 2
						testLines.startX_2 = startX;
						testLines.startY_2 = startY;
						testLines.endX_2 = endX;
						testLines.endY_2 = endY;

						colResult = LineIntersect(&testLines, &impactX, &impactY);
						
						if ( colResult == DO_INTERSECT )
						{                           
							attrib = COL_ACTIVE_BOTTOM;
							(*pCHAR->pResponce_LEVEL)(pCHAR, pColData[mapIndex]->lines[i].attrib, impactX, impactY);
							isCollide = YES;
							
						}
					}
				}
				if (pCHAR->direction & CHAR_DIR_DOWN)
				{
					if (pColData[mapIndex]->lines[i].attrib & COL_ACTIVE_TOP) 
					{
						s32 tempY1, tempY2;
												
						tempY1 = CHARY + CHARColOffsetY;
						tempY2 = CHARY + CHARColOffsetY + CHARColHeight + pCHAR->yv;
						

						// line 1
						testLines.startX_1 = CHARX + (CHARColOffsetX + (CHARColWidth>>1));
						testLines.startY_1 = tempY1;
						testLines.endX_1 = CHARX + (CHARColOffsetX + (CHARColWidth>>1));
						testLines.endY_1 = tempY2;
						// line 2
						testLines.startX_2 = startX;
						testLines.startY_2 = startY;
						testLines.endX_2 = endX;
						testLines.endY_2 = endY;
						
						colResult = LineIntersect(&testLines, &impactX, &impactY);
						
						if ( colResult == DO_INTERSECT )
						{  
							
							attrib = COL_ACTIVE_TOP;
							
							(*pCHAR->pResponce_LEVEL)(pCHAR, pColData[mapIndex]->lines[i].attrib, impactX, impactY);
							isCollide = YES;
							//pCHAR->y -= pCHAR->yv;
							
						}
					}
				}
			
			}
     }      
   }


   return(isCollide);	


return (NO);   
}
*/