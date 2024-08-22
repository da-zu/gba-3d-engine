/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: oamMan.h                                       *
*        Creation Date: 24/04/01                                  *
*        Description:	OAM Manager. Maintans and copies shadow	  *
*						OAM to internal vram.					  *
*						Draw_OAM is called during/after vysnc.	  *
*						OAMMAN_DATA entries and attrbiutes are	  *
*						cleared every frame	therefore each CHAR must
*						be added each frame they are required to  *
*			 			be drawen to screen                       *
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
#include "oamMan.h"
#include "Agb.h"

/****************************** 
*  Globals                    *
*******************************/
OAMDATA		OAMMAN_DATA IN_EWRAM;
u32 curCharDataPtr;


/******************************
*  Function Definitions       *
*******************************/
u32 oamWait;
/*********************************************************************************************
Function:		void Init_OAMMAN (void)												                  
                                                                                          
Description:	Initialze The OAM Manager.													            
					Clears VRAM, setup OAM structs, initalize variables and setup ISR's.			
                                                                                          
Args:			None.			                                                                  
                                                                                          
Returns:		None																		                     
*********************************************************************************************/
void Init_OAMMAN (void)
{  
   // Clear OAM H/W memory ( all attribs and px )     
   ClearOamRam(); 

   // Clear Character Obj H/W memory for mode 0-3
   DmaClear(3, 0, OBJ_MODE4_VRAM, OBJ_MODE4_VRAM_SIZE, 16);
   DmaWait(3);
	// Clear OAM LEVELMAN structures
   DmaClear(3, 0, &OAMMAN_DATA, sizeof(OAMMAN_DATA), 16);
	DmaWait(3);

   curCharDataPtr = 0;
   
   oamWait = 0;

   OAMMAN_DATA.nextName = 512;
   OAMMAN_DATA.curName = 512;
}
/*********************************************************************************************
Function:		s32 Add_CHAR_OAMMAN( CHAR_PTR pCHAR )					
                                                                                    
Description:	Adds CHARs attribs to internal shadow ram.										
					OAM list management.				                                         
					Generally SPRMAN will fill OAMENTRY with appropiate values.            
                                                                                      
                                                                                      
Args:			pCHAR:			    Pointer to a CHAR_TYP to be added to OAM for drawing and
											applying h/w effects.                                
                                                                                                  
Returns:		s32:   0(OK)            Successfully added OAMENTRY                    
                    or                                                                
                       -1(ERROR)		OAM Manager general error								
                       -2(OUTOFIDS)		OAM Manager out of OAM ID's (entries)            
                       -3(OUTOFNAMES)	OAM Manager out of NAMES                         
                       -4(WRONGARGS)	One of the functions args were incorrect         
                       -5(OUTOFOBJRAM)  OAM Manager has ran out of vailable obj char vram
*********************************************************************************************/
s32 Add_CHAR_OAMMAN ( CHAR_TYP *pCHAR )
{

	u32 curID, curName;
	u32 curFrame;
	u32 angle, scaleX, scaleY;
	s32 x, y;
	u32 flagRotScale;

	flagRotScale = 0;
	// Get ID's and Name's for CHAR_TYP
	if ( FAILURE( Get_ID_OAMMAN() ) )
	{
		// Opps OamMan ran out available oam entries
		
		return(OUTOFIDS);		
	}
	curID = OAMMAN_DATA.curID;
   
	if ( FAILURE( Get_Name_OAMMAN(pCHAR) ) )
	{
		// Opps OamMan ran out of available names
		return(OUTOFNAMES);
	}
	curName = OAMMAN_DATA.curName;

	OAMMAN_DATA.oamEntry[curID].curFrame = pCHAR->curFrame;

   	// Load the CHARs character data into vram
	if ( FAILURE( Load_CHAR_OAMMAN(pCHAR) ) )
	{
		// Opps ran out of Obj Char vram
		return(OUTOFOBJRAM);
	}

	// Check if CHAR needs to be rotated or scaled
	if ( (pCHAR->angle != 0) || (pCHAR->scaleX != 1<<8) || (pCHAR->scaleY != 1<<8) )
	{
		angle = pCHAR->angle;
		scaleX = pCHAR->scaleX;
		scaleY = pCHAR->scaleY;

		if ( FAILURE( Add_RotScale_OAMMAN(angle, scaleX, scaleY) ) )
		{
			return(OUTOFROTSCALEIDS);
		}
	
	}
	curFrame = OAMMAN_DATA.oamEntry[curID].curFrame;
      
	//x = LEVEL_TO_SCREEN_X(gLevelData_LEVELMAN.curLevelX, pCHAR->x);
	//y = LEVEL_TO_SCREEN_Y(gLevelData_LEVELMAN.curLevelY, pCHAR->y);

	// LEVELMAN is currently being updated so for now pCHAR->x/y is also CHAR_TYP levelX/Y
	x = pCHAR->sx;//LEVEL_TO_SCREEN_X(0, pCHAR->lx);
	y = pCHAR->sy;//LEVEL_TO_SCREEN_Y(0, pCHAR->ly);

	if ( x < 0 )
		x += 512;

	if ( y < 0 )
		y += 256;

	OAMMAN_DATA.oamEntry[curID].oamAttribs.attrib0 |= pCHAR->oamAttrib0State | y;
	OAMMAN_DATA.oamEntry[curID].oamAttribs.attrib1 |= pCHAR->oamAttrib1State | x;
	OAMMAN_DATA.oamEntry[curID].oamAttribs.attrib2 |= pCHAR->oamAttrib2State | curName;
	OAMMAN_DATA.oamEntry[curID].oamAttribs.pad = 0;
 
	return(OK);
}

/*********************************************************************************************
Function:		s32 Get_ID_OAMMAN ( void )												            
                                                                                            
Description:	Returns next available OAM ID number for CHAR_TYP and increments to next OAM ID.
                                                                                          
Args:			None:																		
                                                                     
Returns:		s32:	OK if there is an available OAM ID or ERROR if no more available											            *
*********************************************************************************************/
s32 Get_ID_OAMMAN ( void )
{	
   s32 nextID, curID;
	   
   nextID = OAMMAN_DATA.nextID;
   OAMMAN_DATA.curID = nextID;

   curID = OAMMAN_DATA.curID;

   if ( ++nextID >= MAX_OAMOBJS )
	{
		// No more ID's available
		return (ERROR);
	}
	else
	{		
	   OAMMAN_DATA.nextID = nextID;
	}	
	
	return (OK);
}

/*********************************************************************************************
Function:		s32 Get_Name_OAMMAN ( CHAR_PTR pCHAR )										   
                                                                                            
Description:	Returns next available OAM NAME for CHAR_TYP and increments to next OAM NAME.	   
                                                                                            
Args:				pCHAR:	Pointer to CHAR_TYP.										
                                                                                            
Returns:			s32:	Next available OAM NAME	or ERROR if no more available					
*********************************************************************************************/
s32 Get_Name_OAMMAN ( CHAR_TYP *pCHAR )
{
	u32 CHARWidth, CHARHeight;	// Width and height of CHAR_TYP
	s32 nextName;			    // Next available name
    s32 curName;				// Current name
	u32 numBlocks;		        // Number of 8x8 tiles that CHAR_TYP uses
    u32 numNames;				// Number of names CHAR_TYP requests
		
	CHARWidth = pCHAR->pCreature->width;
	CHARHeight = pCHAR->pCreature->height;
		
	nextName = OAMMAN_DATA.nextName;
	OAMMAN_DATA.curName = nextName;

	curName = OAMMAN_DATA.curName;
	   
	if ( (CHARWidth > 8) && (CHARHeight > 8) )
	{
		// Figure out char name based on 8x8 tiles that make up CHAR_TYP
		numBlocks = (CHARWidth >> 3) * (CHARHeight >> 3);	
		numBlocks += numBlocks;
	}
	else
	{
		numBlocks = 2;     
	}
		
	numNames = curName + numBlocks;

	// Opps not enough available names for CHAR_TYP assume 256 color CHARs until 16x16 color support added
	if ( numNames >= MAX_OAMNAMES_256COL + 512)
	{
		return(ERROR);
	}
	else
	{
		OAMMAN_DATA.nextName = numNames;
	}
	
	OAMMAN_DATA.oamEntry[OAMMAN_DATA.curID].numBlocks = numBlocks;

	return(OK);   
}

/*********************************************************************************************
Function:		s32 Load_CHAR_OAMMAN( CHAR_PTR pCHAR )                                 
                                                                                          
Description:	Adds the CHAR_TYP graphics data to OAM obj data DMA list.						            
                                                                                          
Args:			pCHAR:		A pointer to a CHAR_TYP to load
                                                                                          
Returns:		s32:     Return OK on success else returns ERROR if application has ran out   
							VRAM to hold obj data.									                     
**********************************************************************************************/
s32 Load_CHAR_OAMMAN( CHAR_TYP *pCHAR )
{
	u32 curFrame, numBlocks;
	u32 size, offset;
	u32 *pTiles;
	
	TILESET_TYP *pTILESET_Objtiles;
	
	if ( curCharDataPtr >= *( u16*)OBJDATA + 0x8000 )
	{
		return ( ERROR );	// Ran out of vram
	}
		
	pTiles = pCHAR->pCreature->pDataSource;
		
	pTILESET_Objtiles = (TILESET_TYP *) pTiles;

	curFrame = OAMMAN_DATA.oamEntry[OAMMAN_DATA.curID].curFrame;
	numBlocks = OAMMAN_DATA.oamEntry[OAMMAN_DATA.curID].numBlocks;

	numBlocks = numBlocks >> 1;

	size = (numBlocks * 0x40);
	
	offset = (u32)&pTILESET_Objtiles->data;
	offset += (curFrame * size);
	
	OAMMAN_DATA.oamDmaList[OAMMAN_DATA.curID].offset = offset;
	OAMMAN_DATA.oamDmaList[OAMMAN_DATA.curID].size = size;
	OAMMAN_DATA.oamDmaList[OAMMAN_DATA.curID].charDataPtr = OBJDATA + curCharDataPtr;

	curCharDataPtr += (size) >> 1;

		
	return ( OK );
}// End Load_CHAR

/*********************************************************************************************
Function:		u32 Add_RotScale_OAMMAN ( void )												         
                                                                                            
Description:	Adds a rotation and scaling combination to internal PXREGS shadow ram.			
					Generally rotation and scaling math is performed in SPRMAN.				         
                                                                                            
Args:			None:
                                                                                            
Returns:		s32:    0(OK)          Successfully added PXDATA                          
                      or                                                                
                        -1(ERROR)      OAM Manager general error									
                        -2(OUTOFIDS)   OAM Manager out of PXDATA ID's                     
                        -4(WRONGARGS)  One of the functions args were incorrect           
            																		                           
*********************************************************************************************/
s32 Add_RotScale_OAMMAN ( FIXED angle, FIXED scaleX, FIXED scaleY )
{
	s32 nextPxID;
	s32 curID, curPxID;

	curID = OAMMAN_DATA.curID;

	OAMMAN_DATA.oamEntry[curID].isRotScale = ON;

	nextPxID = OAMMAN_DATA.curPx;
	curPxID = nextPxID;		

	if ( ++nextPxID >= MAX_PXREGS )
	{
		OAMMAN_DATA.oamEntry[curID].isRotScale = OFF;
		// No more PxRegs available
		return (ERROR);
	}
	else
	{	      
      switch (curPxID)
      {
      case 0:
         OAMMAN_DATA.oamEntry[curID].pxEntryID = ROTSCAL_ID_1;
         break;
      case 1:
         OAMMAN_DATA.oamEntry[curID].pxEntryID = ROTSCAL_ID_2;
         break;
      case 2:
         OAMMAN_DATA.oamEntry[curID].pxEntryID = ROTSCAL_ID_3;
         break;
      case 3:
         OAMMAN_DATA.oamEntry[curID].pxEntryID = ROTSCAL_ID_4;
         break;
      case 4:
         OAMMAN_DATA.oamEntry[curID].pxEntryID = ROTSCAL_ID_5;
         break;
      case 5:
         OAMMAN_DATA.oamEntry[curID].pxEntryID = ROTSCAL_ID_6;
         break;
      case 6:
         OAMMAN_DATA.oamEntry[curID].pxEntryID = ROTSCAL_ID_7;
         break;
      case 7:
         OAMMAN_DATA.oamEntry[curID].pxEntryID = ROTSCAL_ID_8;
         break;

      default:
         OAMMAN_DATA.oamEntry[curID].pxEntryID = ROTSCAL_ID_1;
         break;
      }
	   OAMMAN_DATA.curPx = nextPxID;		
	}	
		
	OAMMAN_DATA.pxEntry[curPxID].pa = ( scaleX * COS(angle) )>>8;
	OAMMAN_DATA.pxEntry[curPxID].pb = ( scaleY * SIN(angle) )>>8;
	OAMMAN_DATA.pxEntry[curPxID].pc = ( scaleX * -SIN(angle) )>>8;
	OAMMAN_DATA.pxEntry[curPxID].pd = ( scaleY * COS(angle) )>>8;

   return(OK);
}


