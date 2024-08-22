/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: animMan.c                                      *
*        Creation Date: 19/10/01                                  *
*        Description:	Animation Manager.       				  *
*                       CHAR animation manager.                    *
*                       Provides loading of animation sequences.  *
*                       Support for ani-motion systems.           *
*                       Utility functions for Playing,Stopping etc*
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
#include "animMan.h"

/******************************
*  Globals                    *
*******************************/
ANIMATIONMAN_TYP animationManager IN_EWRAM;

/******************************
*  Function Definitions       *
*******************************/
/*****************************************************************************************
Function:		s32 Init_ANIMMAN( void )
																														
Description:	Initializes the ANIMATION MANAGER																						
																														
Args:			None:
																																		
Returns:		None:	

******************************************************************************************/
void Init_ANIMMAN( void )
{	
	animationManager.nextEntry = 0;
}


/*****************************************************************************************
Function:		s32 Update_CHAR_ANIMMAN( void )
																														
Description:	Updates the managed CHARs.
																														
Args:			None:
																																		
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							
******************************************************************************************/
s32 Update_ANIMMAN( void )
{	
	u32 i;
	
	for( i = 0; i < animationManager.nextEntry; i++ )
	{
		if ( (animationManager.pCHARList[i]->animState & ANIM_STATE_PLAY ) &&
			!(animationManager.pCHARList[i]->animState & ANIM_STATE_STOP ) && 
			!(animationManager.pCHARList[i]->animState & ANIM_STATE_PAUSE ) )
		{

			Animate_CHAR_ANIMMAN( animationManager.pCHARList[i] );
		}
	}
	
	return OK;
}

/*****************************************************************************************
Function:		s32 Play_CHAR_ANIMMAN( CHAR_PTR pCHAR, u32 seqName )
																														
Description:	Sets the CHAR animation state to PLAY.
               Also addes the CHAR from ANIM MANAGER list
																														
Args:			pCHAR:    CHAR to start playing animation for.
            seqName: The sequence name to play
																																		
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							
******************************************************************************************/
s32 Play_CHAR_ANIMMAN( CHAR_TYP *pCHAR, u32 seqName )
{	
	
	STATE_OFF(pCHAR->animState, ANIM_STATE_DONE);
	STATE_OFF(pCHAR->animState, ANIM_STATE_STOP);
	STATE_OFF(pCHAR->animState, ANIM_STATE_PAUSE);
	STATE_ON(pCHAR->animState, ANIM_STATE_PLAY);
	
	pCHAR->curFrame    = 0;	
	pCHAR->animDelayCount = 0;
	pCHAR->curAnimFrameIndex = 0;
	
	Add_CHAR_ANIMMAN( pCHAR, seqName);

	return OK;
}

/*****************************************************************************************
Function:		s32 Stop_CHAR_ANIMMAN( CHAR_PTR pCHAR )
																														
Description:	Sets the CHAR animation state to STOP and set curframe to begining of animation sequence.
               Also removes the CHAR from ANIM MANAGER list
																														
Args:			pCHAR:    CHAR to Stop playing animation for.
																																		
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							
******************************************************************************************/
s32 Stop_CHAR_ANIMMAN( CHAR_TYP *pCHAR )
{	
	
	Remove_CHAR_ANIMMAN( pCHAR );
	
	return OK;
}

/*****************************************************************************************
Function:		s32 Play_CHAR_ANIMMAN( CHAR_PTR pCHAR )
																														
Description:	Sets the CHAR animation state to STOP.
																														
Args:			pCHAR:    CHAR to pause the playing animation for.
																																		
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							
******************************************************************************************/
s32 Pause_CHAR_ANIMMAN(CHAR_TYP *pCHAR )
{	
	
	//pCHAR->animState = ANIM_STATE_PAUSE;
	STATE_OFF(pCHAR->animState, ANIM_STATE_PLAY);
	STATE_OFF(pCHAR->animState, ANIM_STATE_STOP);
	STATE_ON(pCHAR->animState, ANIM_STATE_PAUSE);

	return OK;
}

/*****************************************************************************************
Function:		s32 unPause_CHAR_ANIMMAN( CHAR_PTR pCHAR )
																														
Description:	Sets the CHAR animation state to STOP.
																														
Args:			pCHAR:    CHAR to pause the playing animation for.
																																		
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							
******************************************************************************************/
s32 unPause_CHAR_ANIMMAN( CHAR_TYP *pCHAR )
{	
	//pCHAR->animState = ANIM_STATE_PAUSE;
	STATE_ON(pCHAR->animState, ANIM_STATE_PLAY);   
	STATE_OFF(pCHAR->animState, ANIM_STATE_PAUSE);

	return OK;
}

/******************************************************************************************
Function:		static s32 Animate_CHAR_ANIMMAN(CHAR_PTR CHAR)																	
																														
Description:	This function animates a CHAR, basically it takes a look at						
					the attributes of the CHAR and determines if the CHAR is							
					a single frame, multiframe, or multi animation, updates							
					the counters and frames appropriately.													
																														
Args:			pCHAR1:	Pointer to CHAR.																		
																														
Returns:		s32:		OK on success 
						or 
							ERROR if invalid CHAR or CHAR is invisible				
*******************************************************************************************/
s32 Animate_CHAR_ANIMMAN(CHAR_TYP *pCHAR)
{	
	u32 curSeqName;

	  
	if ( !(pCHAR->attr & CHAR_ATTR_VISIBLE) )
		return ( ERROR );
	  
	curSeqName = pCHAR->curAnimationSeq; 
		
	STATE_OFF(pCHAR->animState, ANIM_STATE_FRAME_DONE);

	// first test if its time to animate
	if (++pCHAR->animDelayCount >= pCHAR->animDelay)
	{
		STATE_ON(pCHAR->animState, ANIM_STATE_FRAME_DONE);

		// reset counter
		pCHAR->animDelayCount = 0;
		
		// increment the animation frame index
		pCHAR->curAnimFrameIndex++;
				
		// is this and end sequence 
		if (pCHAR->curAnimFrameIndex > pCHAR->pCreature->animation->animSeq[curSeqName].numFrames - 1)
		{			 
			// test if this is a single shot animation
			if (pCHAR->pCreature->animation->animSeq[curSeqName].seqAttrib == ANIMMAN_SEQ_ATTR_ONESHOT)
			{
				// set animation state message to stop
				STATE_OFF(pCHAR->animState, ANIM_STATE_PLAY);
				STATE_ON(pCHAR->animState, ANIM_STATE_STOP);
				
				// reset frame back one
				pCHAR->curAnimFrameIndex--;
				
				// extract animation frame and animotion timing
				pCHAR->curFrame = pCHAR->pCreature->animation->animSeq[curSeqName].animFrame[pCHAR->curAnimFrameIndex].frameNum;
				pCHAR->animDelay = pCHAR->pCreature->animation->animSeq[curSeqName].animFrame[pCHAR->curAnimFrameIndex].frameTime;
				
				// Set animation state for CHAR to done so user can test when animtion sequence is finished
				STATE_ON(pCHAR->animState, ANIM_STATE_DONE); 
				
			} // end if
			else 
			{
				// reset animation index
				pCHAR->curAnimFrameIndex = 0;
				
				// extract first animation frame and animotion timing
				pCHAR->curFrame = pCHAR->pCreature->animation->animSeq[curSeqName].animFrame[pCHAR->curAnimFrameIndex].frameNum;
				pCHAR->animDelay = pCHAR->pCreature->animation->animSeq[curSeqName].animFrame[pCHAR->curAnimFrameIndex].frameTime;
				// set animation state message to done
				STATE_ON(pCHAR->animState, ANIM_STATE_DONE); 				
			} 
			
		}
		else
		{
		
		}
		// extract the next frame from animation list and animotion timing
		pCHAR->curFrame = pCHAR->pCreature->animation->animSeq[curSeqName].animFrame[pCHAR->curAnimFrameIndex].frameNum; 
		pCHAR->animDelay = pCHAR->pCreature->animation->animSeq[curSeqName].animFrame[pCHAR->curAnimFrameIndex].frameTime; 
		
	} 
	  
	// return success	
	return( OK );
	  
} // end Animate_CHAR

/*****************************************************************************************
Function:	s32 Add_CHAR_ANIMMAN( CHAR_PTR pCHAR, u32 seqName)
																														
Description:	Adds a CHAR to be managed by the ANIMATION MANAGER																						
																																			
Args:			pCHAR:		Pointer to CHAR.																												*
				seqName:	The animtaion sequence to play.
				
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							OUTOFANIMMANENTRIES if Aniamation manager has ran out of availiable CHAR slots to be managed
******************************************************************************************/
s32 Add_CHAR_ANIMMAN( CHAR_TYP *pCHAR, u32 seqName)
{	
	u32 curEntry;
	
	curEntry = animationManager.nextEntry;
	
	if ( curEntry >= MAX_ANIMMAN_CHAR_ENTRIES )
	{
		return( OUTOFANIMMANENTRIES );
	}
	
	animationManager.pCHARList[curEntry] = pCHAR;
	
	// set the animation index
	pCHAR->curAnimationSeq = seqName;
	
	// reset animation 
	pCHAR->curFrame    = 0;	
	pCHAR->animDelayCount = 0;
	pCHAR->curAnimFrameIndex = 0;
	
	STATE_ON(pCHAR->animState, ANIM_STATE_ADDED);
	
	pCHAR->curFrame    = pCHAR->pCreature->animation->animSeq[seqName].animFrame[0].frameNum;
	pCHAR->animDelay   = pCHAR->pCreature->animation->animSeq[seqName].animFrame[0].frameTime;
	
	animationManager.nextEntry++;
	
	return(OK);
}

/*****************************************************************************************
Function:	s32  Remove_CHAR_ANIMMAN( CHAR_PTR pCHAR )
																														
Description:	Removes a CHAR that is managed by the ANIMATION MANAGER																						
																																			
Args:			pCHAR:		Pointer to CHAR.																												*
				
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							OUTOFANIMMANENTRIES if Aniamation manager has ran out of availiable CHAR slots to be managed
******************************************************************************************/
s32 Remove_CHAR_ANIMMAN( CHAR_TYP *pCHAR )
{
	u32 i, y;
	
	for( i = 0; i <= animationManager.nextEntry; i++ )
	{
		if( animationManager.pCHARList[i] == pCHAR )
		{
			STATE_OFF(pCHAR->animState , ANIM_STATE_ADDED);
			
			// Found a match now delete and move all folowing entries up in list
			for ( y = i+1; y <= animationManager.nextEntry; y++ )
			{
				STATE_OFF(pCHAR->animState, ANIM_STATE_PLAY);
				STATE_OFF(pCHAR->animState, ANIM_STATE_PAUSE);
				STATE_ON(pCHAR->animState, ANIM_STATE_STOP);
				STATE_ON(pCHAR->animState, ANIM_STATE_DONE);
				
				pCHAR->curFrame    = pCHAR->pCreature->animation->animSeq[pCHAR->curAnimationSeq].animFrame[0].frameNum;
				pCHAR->animDelay   = pCHAR->pCreature->animation->animSeq[pCHAR->curAnimationSeq].animFrame[0].frameTime;
				pCHAR->animDelayCount = 0;
				pCHAR->curAnimFrameIndex = 0;
				pCHAR->animDelayCount = 0;
				animationManager.pCHARList[i++] = animationManager.pCHARList[y];
				
			}
			
			animationManager.nextEntry--;
			// Found a match so return ok
			return (OK);
		}
	}
	// Didnt find a mathing CHAR alreading edded to ANimatin Manager control
	return (ERROR);
}
