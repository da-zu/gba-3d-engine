/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: HBTypes.h	                                    *
*        Creation Date: 21/10/01                                  *
*        Description: Global typedefs 										*
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
#ifndef HBTYPESINC
#define HBTYPESINC

#include "HBDefines.h"
#include "Agb.h"

/****************************  
*  Types                    *
****************************/
typedef void StateAction(void);
typedef StateAction* pStateAction;


/********************************************************************************* 
Structure:		
Decription:    

  Modification:
  
*********************************************************************************/
typedef struct tag_TILESET
{
	u16 tiles;
	u16 size;
	u32 data;
}TILESET_TYP;//, *TILESET_PTR;

/********************************************************************************* 
Structure:		
Decription:    

  Modification:
  
*********************************************************************************/
typedef struct tag_TILEMAP
{
	u16 columns;
	u16 rows;
	u16 data;
}TILEMAP_TYP;

/********************************************************************************* 
Structure:		Rectangle Type
Decription:    Structure to define a rectangle shape. x and y are the topleft of RECT

  Modification:
  
*********************************************************************************/
typedef struct tag_RECT
{
	s32 left;
	s32 top;
	s32 right;
	s32 bottom;
	
}RECT_TYP;//, *RECT_PTR;

/********************************************************************************* 
Structure:		ANIMATION Sequence type

  Decription:    Descibes a frame of information.
  ANIMATION->ANIMSETSEQ->ANIMSEQENTRY			
  
	Modification:
	
*********************************************************************************/
typedef struct tag_ANIMFRAME
{	
	s32 frameNum;					// Frame number to play
	s32 frameTime;					// How long to play frame for ( 0 = fastest )
	
}ANIMFRAME_TYP;//, *ANIMFRAME_PTR;


/********************************************************************************* 
Structure:		ANIMATION Sequence type

  Decription:    Descibes a frame of information.
  ANIMATION->ANIMSETSEQ->ANIMSEQENTRY			
  
	Modification:
	
*********************************************************************************/
typedef struct tag_ANIMSEQ
{
	u32 numFrames;					// The number of different frames 
	u32 seqAttrib;
	const ANIMFRAME_TYP *animFrame;
	
}ANIMSEQ_TYP;//, *ANIMSEQ_PTR;


/********************************************************************************* 
Structure:		ANIMATION type

  Decription:    Descibes a animation set of information.			
  ANIMATION->ANIMSETSEQ->ANIMSEQENTRY
  Modification:
  
*********************************************************************************/
typedef struct tag_ANIMATION
{
	u32 numAnimSeqs;						   // The number of aniamtion sequences
	//ANIMSEQ animSeq[MAX_ANIM_SEQS];	// Decribes a sequence of animation
	const ANIMSEQ_TYP *animSeq;
	
}ANIMATION_TYP;//, *ANIMATION_PTR;


/********************************************************************************* 
Structure:		STATEMAN 

  Decription:    State Manager maintains state related information relative to execute
  and update a creatures state execution	
  
	Modification:
	
	  NOTES:		
*********************************************************************************/
typedef struct tag_STATEMAN
{
	vu32 curState;
	vu32 nextState;
	
			
	vs32 idleCounter;              // Used for idling
	vs32 counter;                  // general counter1
	vs32 counter2;                  // general counter2
	vs32 param1;                   // Used to pass state parameters
	vs32 param2;                   // Used to pass state parameters
	vs32 param3;                   // Used to pass state parameters
	vs32 param4;

	vs32 jumpCounter;

}STATEMAN_TYP;//, *STATEMAN_PTR;

typedef struct tag_RECTLIST
{
	u32 ID;

	s32 left;
	s32 top;
	s32 right;
	s32 bottom;
	
	s32 attributes;
	
}RECTLIST_TYP;

typedef struct tag_LINELIST
{
	u32 ID;

	s32 startX;
	s32 startY;
	s32 endX;
	s32 endY;
	
	s32 attributes;		// Attributes of line
	
}LINELIST_TYP;


typedef struct tag_PATHPOINT
{
	u32 ID;
	u32 attributes;
	s32 x;
	s32 y;

}PATHPOINT_TYP;

typedef struct tag_PATHPOINTLIST
{
	u32 ID;
	u32 numPoints;

	const PATHPOINT_TYP pPointList[];

}PATHPOINTLIST_TYP;

typedef struct tag_PATHLIST
{	
	u32 numPaths;
	const PATHPOINTLIST_TYP *pPathList;

}PATHLIST_TYP;

struct tag_CREATURE;

/********************************************************************************* 
Structure:		CHAR Type

  Decription:    Holds the information to define a CHAR
  
	Modification:
	
*********************************************************************************/
typedef struct tag_CHAR
{   
	s32 name;
	
	u16 oamAttrib0State;       // OAM attribute 0
	u16 oamAttrib1State;       // OAM attribute 1
	u16 oamAttrib2State;       // OAM attribute 2
	
	// States and attributes
	u32 attr;						// attributes pertaining to the object (general)
	vu32 state;						// the state of the object (general)
	u8 animState;					// an animation state variable, up to you
	
	// Characteristics
	FIXED fx, fy, fz;						// running fixed point position
	FIXED oldFX, oldFY, oldFZ;
	s32 x,y,z;						   // Position of CHAR in level coordinates
	s32 ax, ay, az;
	s32 wx, wy, wz;
	s32 lx, ly, lz;
	s32 sx, sy, sz;
	FIXED offX, offY;		// running offsets to keep perspective

	s32 oldX, oldY, oldZ;
	FIXED xv,yv;						// velocity of object
	s16 angle;
	FIXED scaleX, scaleY;
	u8 direction;					// direction of sprite
	
	// Collision
	//u32 curBoundBox;				// The CHARs current bounding box from the bounding boxes declared in the CHARs creature type
	RECT_TYP BoundingBox;
	
	// Animation
	u32 animDelay;					// The overall speed of animation playback for the CHAR ( 1 = fast..10 = slow )
	u32 animDelayCount;			// Counter for delay of update frame in animation
	u32 curAnimFrameIndex;		// The current index into the animation->sequence->frame
	s32 curFrame;					// current animation frame	of current animation sequence
	u32 curAnimationSeq;			// Current animation sequence	
	const struct tag_CREATURE *pCreature;		// Pointer to the creature type the CHAR is
	
	STATEMAN_TYP stateMan;

	u32 curPathID;
	u32 curPathPointID;
	const PATHLIST_TYP *pLevelPathLists;

	const PATHPOINTLIST_TYP *pCurPathList;

	const PATHPOINT_TYP *pCurPathPoint;
	
	s32 curPlatformID;

	s32 matrix[4][4];
	u32 update;
	s32 distance;
	u32 isView;
	s32 added;
	
}CHAR_TYP;//, *CHAR_PTR;

typedef void StateActionChar(CHAR_TYP *pCHAR);
typedef StateActionChar* pStateActionChar;

// Collision Detection Responce function types
typedef void CollisionResponceFunc_CHARS( CHAR_TYP *pCHAR1, CHAR_TYP *pCHAR2, u32 isCollide );
typedef CollisionResponceFunc_CHARS* pColResponceFunc_CHARS;

typedef void CollisionResponceFunc_LEVEL_LINE( CHAR_TYP *pCHAR, u32 isCollide, s32 impactX, s32 impactY );
typedef CollisionResponceFunc_LEVEL_LINE* pCollisionResponceFunc_LEVEL_LINE;

typedef void CollisionResponceFunc_LEVEL_RECT( CHAR_TYP *pCHAR, u32 isCollide, const RECTLIST_TYP *pRect );
typedef CollisionResponceFunc_LEVEL_RECT* pCollisionResponceFunc_LEVEL_RECT;

// AI update function types
typedef void Update_AI_Func_CREATURE( CHAR_TYP *pCHAR );
typedef Update_AI_Func_CREATURE* pUpdate_AI_Func_CREATURE;
/********************************************************************************* 
Structure:		CREATURE Type

  Decription:    Holds the information to define a creature in ROM
  
	Modification:
	
*********************************************************************************/
typedef struct tag_CREATURE
{
	u16 creatureName;          // Creature name. see "creatures.h" defines.
	
	u16 attr;
	
	u16 oamAttrib0State;       // OAM attribute 0
	u16 oamAttrib1State;       // OAM attribute 1
	u16 oamAttrib2State;       // OAM attribute 2
	
	u16 width, height;			// The width and height of the CREATURE
	//u32 numFrames;					// The total number of frames for the CREATURE
	u32 numBoundBoxes;			// The number of bounding boxes the creature has 
	u32 *pDataSource;				// Pointer to the creatures data source
	
	//RECT boundingBox[MAX_NUM_BOUNDBOXES];// The CREATUREs predefined bounding boxes
	const RECT_TYP *pBoundingBoxes;
	
	//u32 numAnimSeqs;					// The number of different animation sequences
	const ANIMATION_TYP *animation;
	
	pColResponceFunc_CHARS pResponce_PLAYER_TO_NPC;    // Player character to non player character collision responce function
	pColResponceFunc_CHARS pResponce_NPC_TO_NPC;       // Non player character to non player character collision responce function
	pCollisionResponceFunc_LEVEL_LINE pResponce_LEVEL_LINE;
	pCollisionResponceFunc_LEVEL_RECT pResponce_LEVEL_RECT;
	
	// AI update fucntion
	pUpdate_AI_Func_CREATURE pAI_Update;
	
	// State Table pointer
	//STATETABLE_PTR pStateTable;	// Pointer to the state table
	
}CREATURE_TYP;//, *CREATURE_PTR;

/////////////////////////////////////////////////////////////////////////
// NEW STUFF FROM NEW TOOL
// GBA Layer type
typedef struct tag_GBA_BKG_LAYER_STATS
{
	// Screen
	s32 screenBaseBlockAddress;	// The starting address of the screen base block for the layer
	s32 screenBaseBlock;	// The starting screen base block for the layer
	s32 screenBlockSize;	// The size of the screen block for the layer
	s32 numScreenBlocks;	// The number of screen 2k blocks used for layer
	s32 numScreenEntries;	// The total number of screen entries used in layer
	
	// Character
	s32 characterBaseBlockAddress;	// The starting address of the character base block for thie layer
	s32 characterBaseBlock;	// The starting address of the character base block for thie layer
	s32 characterBlockSize;	// The siaze of the character block for the layer
	s32 numCharacterBlocks;	// The number of character blocks used for the layer
	s32 numCharacters;		// The total number of characters used in layer
	
	// Collision Rect
	s32 numCollisionRects;
	
	// Collision Line
	s32 numCollisionLines;

	// Paths
	s32 numPaths;

	u32 width;
	u32 height;

	vu16 *controlReg;	// See AgbDefines.h
	u32 priority;		// See AgbDefines.h
	u32 isMosaic;		// See AgbDefines.h
	u32 colMode;		// See AgbDefines.h (16x16 or 1x256)
	u32 screenSize;		// See AgbDefines.h
	u32 areaOverflow;	// See AgbDefines.h
	u8 isRotScale;		// YES or NO
	
}GBA_BKG_LAYER_STATS_TYP;//, *GBA_BKG_LAYER_STATS_PTR;




typedef struct tag_LEVEL_CHAR_DESC
{
	u32 name;
	const CREATURE_TYP *pCreatureType;
	s32 startX;
	s32 startY;
	u32 startState;
	u32 startDirection;
	s32 startPathID;
	s32 startPathPointID;

}LEVEL_CHAR_DESC_TYP;


typedef struct tag_LAYER_HWCONTROL
{
	vs16 angle;
	vs16 x_scroll,y_scroll;
	vs32 DX,DY;
	vs16 PA,PB,PC,PD;
	vs32 intensity;
	FIXED scale;
}LAYER_HWCONTROL_TYP;

typedef struct tag_LAYER
{
	u32 charsListEntry;	// the starting entry into the CHARS_ViewList
	u32 numChars;
	const LEVEL_CHAR_DESC_TYP *pCharDescs;

	u32 numCollisionRects;
	const RECTLIST_TYP *pCollisionRects;
	
	u32 numCollisionLines;	
	const LINELIST_TYP *pCollisionLines;

	u32 numPaths;
	const PATHLIST_TYP *pPaths;

	const u16 *pScreenSet;
	
	const u8 *pCharSet;	

	const GBA_BKG_LAYER_STATS_TYP *pStats;
	
}LAYER_TYP;

typedef struct tag_LEVEL
{
	u32 numLayers;
	const u32 *pLevelCharPalette;	// need to change to u16 *
	const u16 *pLevelBkgPalette;
	LAYER_TYP LAYERS[];

}LEVEL_TYP;

typedef struct tag_LEVELMAN
{
	const LEVEL_TYP *pCurLEVEL;
	LAYER_HWCONTROL_TYP hwControl[4];	// Hw control for each layer that can be manipualted realtime

	CHAR_TYP CHARS_ViewList[20]; // list characters that are onscreen

	u32 isNewLevelPreped;				// Flag to indictate the new level is ready
	u32 isOldLevelPreped;				// Flag to indictate the old level is ready
	s16 mosaicAmount;					// 0  - 10
	u32 transDelay;
	u32 blankDelay;
	
	s16 alpha_EVA;
	s16 alpha_EVB;
	
}LEVELMAN_TYP;


/********************************************************************************* 
Structure:		DMAREQDATA Type

Decription:    Structure for Character DMA tranfers.
               Used for Animations, Dynamic Character Management etc.

               srcAddr:    The address from which it is transfered.
               destAddr:   The address to which it is transfered.
               size:       The number of bytes to tranfer.

Modification:

*********************************************************************************/
typedef struct tag_DMAREQDATA
{
   vu32 srcAddr;
   vu32 destAddr;
   vu16 size;
} DMAREQDATA;

#endif

