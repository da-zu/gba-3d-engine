/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: oamMan.h                                       *
*        Creation Date: 24/04/01                                  *
*        Description:	OAM Manager. Maintans and copies shadow	*
*						OAM to internal vram.									*
*                                                                 *                                                                 
*                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*        10/10/01	-	Implemented dynamic support						*
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/ 
/* NOTES:
*			-Every CHAR must have a seperate oamID.
*			-A cloned CHAR or creature have the same char name
*
* TODO:
*			-Add support for different color mapping modes for a obj
********************************************************************/
#ifndef OAMMANINC
#define OAMMANINC

#include <Agb.h>
#include "HBDefines.h"
#include "HBTypes.h"
#include "share.h"
#include "trig.h"
/***************************
*  Defines	               *
****************************/
#define MAX_OAMOBJS			   20
#define MAX_OAMNAMES_256COL		1024
#define MAX_OAMNAMES_16COL	   1024
#define MAX_PXREGS			   8
#define FREE_ENTRY			   -1

#define MIN_SCALE          10
#define MAX_SCALE          512

#define OAMMEM     ((OAMATTRIBS*)0x07000000)   // Pointer to OAM VRAM
#define OBJDATA    ( u16 * )0x06014000
/***************************
*  Types		               *
****************************/

/********************************************************************************* 
Structure:		OAM Manager Type
Decription:    Backwards compatability, with 2d_engine v1.0

Modification:

*********************************************************************************/
typedef struct tag_OAMMANAGER
{
   u8 nextID;					//ID of Obj 
   u16 nextObjName;
   u16 *oam;               //= &REG_OAM;
}OAMMANAGER;

/*********************************************************************************
Structure:		OAMATTRIBS_TYP
Decription:    Holds OAM attibutes which bind the relationship from CHARs current
                 attribute elements to OAM specific meaningful values.

Modification:                

*********************************************************************************/
typedef struct tag_OAMATTRIBS
{
	////////////////////////
   // Attrib 0
   ////////////////////////////////////////////////////////////////////////
   //	[15][14][13][12][11][10][09][08][07][06][05][04][03][02][01][00]
   //	  \  /    |   |   \  /    |   |   \                           /
   //     ||     |   |    ||     |   |     |                       |
   //     ||     |   |    ||     |   |       ||| Y Coordinate |||
   //     ||     |   |    ||     |   |
   //     ||     |   |    ||     |   Rot|Scale ON/OFF ((ON 1, OFF 0))
   //     ||     |   |    ||     |
   //     ||     |   |    ||     Rot|Scale Dble Size ((Single Fold 0, Double Fold 1))
   //     ||     |   |    ||
   //     ||     |   |    Obj Mode ((Normal 00, Semi-Trans 01, Obj Window 10, 11 PROHIBITED))
   //     ||     |   |
   //     ||     |   Obj Mosaic ON/OFF ((ON 1, OFF 0))
   //     ||     |
   //     ||     Color Mode ((16 Color 0, 256 Color 1))
   //     ||
   //    Obj Shape  ((Square 00, Horz Rect 01, Vert Rect 10, 11 PROHIBITED))
   u16 attrib0;			// Oam attribute 0
   ////////////////////////
   // Attrib 1
   ////////////////////////////////////////////////////////////////////////
   //	[15][14][13][12][11][10][09][08][07][06][05][04][03][02][01][00]
   //	  \  /   \                /   \                              /
   //     ||     |   |    |             |                        |
   //     ||     |   |    |                 ||| X Coordinate |||
   //     ||     |   |    |         
   //     ||     |   |    |         
   //     ||     |   |    |     
   //     ||     |   |    |     
   //     ||     |   |    |
   //     ||     |   |    Rot|Scale Parameter Selection ((0-31))
   //     ||     |   |    <Bit 13-9 dependent on attrib0 rot/scale ON.. No flip and Rot/Scale at same time>
   //     ||     |   |
   //     ||     |   Horz Flip ((ON 1, OFF 0))
   //     ||     |
   //     ||     Vert Flip ((ON 1, OFF 0))
   //     ||
   //    Obj Size(( Square
   //                      00 - 8x8, 01 - 16x16, 10 - 32x32, 11 - 64x64
   //               Horz Rect
   //                      00 - 16x8, 01 - 32x8, 10 - 32x16, 11 - 64x32
   //               Vert Rect
   //                      00 - 8x16, 01 - 8x32, 10 - 16x32, 11 - 32x64
   //               <Dependent on attrib0 shape setting>
   //
   u16 attrib1;			// Oam attribute 1
   ////////////////////////
   // Attrib 2
   ////////////////////////////////////////////////////////////////////////
   //	[15][14][13][12][11][10][09][08][07][06][05][04][03][02][01][00]
   //	  \           /   \  /    \                                  /
   //          |          ||         |                          |
   //          |     |    ||            ||| Character Name |||
   //          |     |    ||     
   //          |     |    ||     
   //          |     |    ||     
   //          |     |    ||     
   //          |     |    ||
   //          |     |    Prioity Relative to BG (( 1st 00, 01 2nd, 3rd 10, 4th 11))
   //          |     |
   //          |     Obj Mosaic ON/OFF ((ON 1, OFF 0))
   //          |
   //          Color Palette Number ((16x16 0-15, 256 disabled))
   //     	
   u16 attrib2;			// Oam attribute 2
   
   u16 pad;				// Px reg
}OAMATTRIBS;

/********************************************************************************* 
Structure:		OAMENTRY_TYP

Decription:    OAM Shadow ram entry structure.
						Holds the OAM Shadow entry and additional OAM Manager information 
						pertaining to a CHAR_TYP entry into VRAM

Modification:

**********************************************************************************/
typedef struct tag_OAMENTRY
{
	OAMATTRIBS oamAttribs;

   u32 curFrame;        // CHARs current frame
   u32 *pDataSource;    // CHARs character data source, defining the pixels
   s32 pxEntryID;       // Rotation and scaling ID relative to PXENTRY's(0-7) to apply to CHAR_TYP. -1 if no rot/scale for OAMENTRY
	u32 isRotScale;
   u32 priority;        // CHARs priority amoung BG and other OBJs
	u32 numBlocks;			// Number of 8x8 tiles making up the CHAR_TYP
	
}OAMENTRY;

/********************************************************************************* 
Structure:		PXENTRY_TYP

Decription:    OAM Rotation and Scaling Structure.
					Holds shadow Px rotation and scaling, actual calculation for rot/scale
						performed by SPRMAN

Modification:

**********************************************************************************/
typedef struct tag_PXENTRY
{   
   FIXED pa;              // dx distance moved in X direction along same line
   FIXED pb;              // dmx distance moved in X direction along next line
   FIXED pc;              // dy distance moved in Y direction along same line
   FIXED pd;              // dmy distance moved in Y direction along next line

}PXENTRY;

/********************************************************************************* 
Structure:		OAMDMALIST_TYP

Decription:    Records the Added CHARS character data memory address from ROM->VRAM
					During VSYN this data will be copied into VRAM

Modification:

*********************************************************************************/
typedef struct tag_OAMDMALIST
{
	u32 offset;
	u32 size;
	u32 charDataPtr;

}OAMDMALIST;

/********************************************************************************* 
Structure:		OAMDATA_TYP

Decription:    Holds the current level managers decided list of maintainable CHARs
						OAM shadow entries.

Modification:

*********************************************************************************/
typedef struct tag_OAMDATA
{
   OAMENTRY		oamEntry[MAX_OAMOBJS];
	OAMDMALIST	oamDmaList[MAX_OAMOBJS];

   PXENTRY		pxEntry[MAX_PXREGS];

   s32 curID;           // Next availiable OAM ID
   s32 nextID;
   s32 curName;        // Next availiable OAM NAME. Limited by Mode and color data type
   s32 nextName;
   s32 curPx;
	s32 nextPx;           // Next availiable Px (rot/scale) ID

} OAMDATA;

/****************************
*  Function Prototypes      *
*****************************/
/*********************************************************************************************
Function:		void Init_OAMMAN (void)												                  
                                                                                          
Description:	Initialze The OAM Manager.													            
					Clears VRAM, setup OAM structs, initalize variables and setup ISR's.			
                                                                                          
Args:			None.			                                                                  
                                                                                          
Returns:		None																		                     
*********************************************************************************************/
void Init_OAMMAN (void);
s32 Get_ID_OAMMAN ( void );
s32 Get_Name_OAMMAN ( CHAR_TYP *pCHAR );
s32 Load_CHAR_OAMMAN( CHAR_TYP *pCHAR );
s32 Add_RotScale_OAMMAN ( FIXED angle, FIXED scaleX, FIXED scaleY );

/*********************************************************************************************
Function:		s32 Add_CHAR_OAMMAN( CHAR_PTR pCHAR, u32 *pCHARDataSource)					
                                                                                    
Description:	Adds CHARs attribs to internal shadow ram.										
					OAM list management.				                                         
					Generally SPRMAN will fill OAMENTRY with appropiate values.            
                                                                                      
                                                                                      
Args:			pOamEntry:			Pointer to a OAMENTRY to be added to OAM for drawing and
											applying h/w effects.                                
                                                                                                  
Returns:		s32:        0(OK)          Successfully added OAMENTRY                    
                    or                                                                
                       -1(ERROR)			OAM Manager general error								
                       -2(OUTOFIDS)		OAM Manager out of OAM ID's (entries)            
                       -3(OUTOFNAMES)	OAM Manager out of NAMES                         
                       -4(WRONGARGS)	One of the functions args were incorrect         
                       -5(OUTOFOBJRAM) OAM Manager has ran out of vailable obj char vram
*********************************************************************************************/
s32 Add_CHAR_OAMMAN( CHAR_TYP *pCHAR );


/*********************************************************************************************
Function:		void Draw_OAMMAN (void)												                                
                                                                                            
Description:	Copies shadow OAM and PXREGS into VRAM.				                             
                                                                                            
Args:			None:                                                                         
                                                                                            
Returns:		None:                                                                         
*********************************************************************************************/
void Draw_OAMMAN ( s32 isReadyOam );

#endif
