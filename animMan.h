/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: animMan.c                                      *
*        Creation Date: 19/10/01                                  *
*        Description:	Animation Manager.       				      *
*                       CHAR animation manager.                    *
*                       Provides loading of animation sequences.  *
*                       Support for animotion systems.            *
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
*		Add support to start and stop and animation at anytime
********************************************************************/
#ifndef ANIMMANINC
#define ANIMMANINC

#include <Agb.h>
#include "HBDefines.h"
#include "HBTypes.h"

/***************************
*  Defines                 *
****************************/
#define MAX_ANIMMAN_CHAR_ENTRIES	12

/***************************
*  Types							*
****************************/

typedef struct
{
	CHAR_TYP *pCHAR;
	u32 numFrames;
	u32 frameCount;
		
}ANIMMANLISTENTRY_TYP;//, *ANIMMANLISTENTRY_PTR;

/********************************************************************************* 
Structure:		ANIMATION Manager type

Decription:    Controls animation playback and automatic updating
Modification:

*********************************************************************************/
typedef struct 
{
	CHAR_TYP *pCHARList[MAX_ANIMMAN_CHAR_ENTRIES];	// List of CHAR pointers that ANIMATION Manager manages
	u32 nextEntry;
	
}ANIMATIONMAN_TYP;//, *ANIMATIONMAN_PTR;


/****************************
*  Function Prototypes      *
*****************************/
/*****************************************************************************************
Function:		s32 Init_ANIMMAN( void )
																														
Description:	Initializes the ANIMATION MANAGER																						
																														
Args:			None:
																																		
Returns:		None:	

******************************************************************************************/
void Init_ANIMMAN( void );

/*****************************************************************************************
Function:	s32 Add_CHAR_ANIMMAN( CHAR_PTR pCHAR, u32 seqName)
																														
Description:	Adds a CHAR to be managed by the ANIMATION MANAGER																						
					Must be called each frame update to add CHARs to the manager
																														
Args:			pCHAR:		Pointer to CHAR.																												*
				seqName:	The animtaion sequence to play.
				
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							OUTOFANIMMANENTRIES if Aniamation manager has ran out of availiable CHAR slots to be managed
******************************************************************************************/
//static s32 Add_CHAR_ANIMMAN( CHAR_PTR pCHAR, u32 seqName);

/*****************************************************************************************
Function:	s32  Remove_CHAR_ANIMMAN( CHAR_PTR pCHAR )
																														
Description:	Removes a CHAR that is managed by the ANIMATION MANAGER																						
																																			
Args:			pCHAR:		Pointer to CHAR.																												*
				
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							OUTOFANIMMANENTRIES if Aniamation manager has ran out of availiable CHAR slots to be managed
******************************************************************************************/
//static s32 Remove_CHAR_ANIMMAN( CHAR_PTR pCHAR );

/*****************************************************************************************
Function:		s32 Update_ANIMMAN( void )
																														
Description:	Updates the managed CHARs.
																														
Args:			None:
																																		
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							
******************************************************************************************/
s32 Update_ANIMMAN( void );

/*****************************************************************************************
Function:		s32 Play_CHAR_ANIMMAN( CHAR_PTR pCHAR )
																														
Description:	Sets the CHAR animation state to PLAY.
																														
Args:			None:
																																		
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							
******************************************************************************************/
s32 Play_CHAR_ANIMMAN( CHAR_TYP *pCHAR, u32 seqName );

/*****************************************************************************************
Function:		s32 Play_CHAR_ANIMMAN( CHAR_PTR pCHAR )
																														
Description:	Sets the CHAR animation state to STOP and set curframe to begining of animation sequence.
																														
Args:			None:
																																		
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							
******************************************************************************************/
s32 Stop_CHAR_ANIMMAN( CHAR_TYP *pCHAR );

/*****************************************************************************************
Function:		s32 Play_CHAR_ANIMMAN( CHAR_PTR pCHAR )
																														
Description:	Sets the CHAR animation state to STOP.
																														
Args:			None:
																																		
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							
******************************************************************************************/
s32 Pause_CHAR_ANIMMAN( CHAR_TYP *pCHAR );

/*****************************************************************************************
Function:		s32 unPause_CHAR_ANIMMAN( CHAR_PTR pCHAR )
																														
Description:	Sets the CHAR animation state to STOP.
																														
Args:			pCHAR:    CHAR to pause the playing animation for.
																																		
Returns:		s32:		Returns OK on success 
						or 
							ERROR if invalid CHAR	
							
******************************************************************************************/
s32 unPause_CHAR_ANIMMAN( CHAR_TYP *pCHAR );

/******************************************************************************************
Function:		s32 Animate_CHAR_ANIMMAN(CHAR_PTR CHAR)																	
																														
Description:	This function animates a CHAR, basically it takes a look at						
					the attributes of the CHAR and determines if the CHAR is							
					a single frame, multiframe, or multi animation, updates							
					the counters and frames appropriately.													
																														
Args:			pCHAR1:	Pointer to CHAR.																		
																														
Returns:		s32:		Returns 1 on success or 0 if invalid CHAR or CHAR is invisible				
*******************************************************************************************/
//static s32 Animate_CHAR_ANIMMAN(CHAR_PTR pCHAR);

s32 Animate_CHAR_ANIMMAN(CHAR_TYP *pCHAR);
s32 Add_CHAR_ANIMMAN( CHAR_TYP *pCHAR, u32 seqName);
s32 Remove_CHAR_ANIMMAN( CHAR_TYP *pCHAR );

#endif
