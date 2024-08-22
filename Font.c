/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: Font.c                                         *
*        Creation Date: 17/08/01                                  *
*        Description: Fontloading and management.                 *
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
#include "Font.h"

/****************************
*		  Globals			   *
*****************************/
CHAR_TYP CHAR_Font[36] IN_EWRAM;		// 10 numbers a 26 letters
//extern LEVELMAN_DATA gLevelData_LEVELMAN;

/***********************************************
         Creature Data Characteristics
***********************************************/
extern u32 Font_raw;

const CREATURE_TYP Font_CREATURE=
{
   0, // Creature Name
	CHAR_ATTR_DUMBCHAR | CHAR_ATTR_VISIBLE | CHAR_ATTR_COLLIDEWALLX | CHAR_ATTR_WRAPAROUNDY,	// The creatures default CHAR Attributes
	COLOR_256 | SHAPE_SQUARE,		// OAM attribute 0
	OAMMAN_SIZE_8x8,					// OAM attribute 1
	PRIORITY_1,		// OAM attribute 2
	8,				// Width
	8,				// Height
	0,					// Number of Bounding Boxes 
	&Font_raw,	// Data source
	// Creatures Bounding Boxes MAX_NUM_BOUNDBOXES
	0,
   0,
   0,
   0,
   0,
   0,
   0
};
/****************************
*  Function Definitions     *
*****************************/
void ColResponce_Font_PLAYER_TO_NPC( CHAR_TYP *pCHAR, u32 isCollide )
{
  
}

void ColResponce_Font_NPC_TO_NPC( CHAR_TYP *pCHAR, u32 isCollide )
{
  
}

void ColResponce_Font_LEVEL( CHAR_TYP *pCHAR, u32 isCollide, s32 impactX, s32 impactY )
{
}
/******************************************************************************************
Function:		void Font_Init(void)                                                       *
                                                                                          *
Description:	Initializes CHAR's used to represent chracters                              *
                                                                                          *
Args:			none                                                                          *
                                                                                          *
Returns:		none                                                                          *
*******************************************************************************************/
void Init_Font(void)
{ 
	
	u32 i;
	
	
	for ( i = 0; i < 36; i++ )
	{
	 Create_CHAR(&CHAR_Font[i], &Font_CREATURE);   	
	
	Set_Vel_CHAR(&CHAR_Font[i], FONT_SPEEDX, FONT_SPEEDY);
	
//	STATE_ON(CHAR_Font[i].state, CHAR_STATE_IDLE);
	
	CHAR_Font[i].direction = CHAR_DIR_STOP;
	}
		
}

/******************************************************************************************
Function:		void Font_itoa ( u32 number, u8 *pBuffer, s32 size )                       *
                                                                                          *
Description:	Converts a u32 number into a ascii string.                                 *
                                                                                          *
Args:			number:  The number to be converted.                                          *
            pBuffer: Pointer to the character buffer to store the ascii string.           *
            size:    Size of string. If total size is not used padding will occur.        *
Returns:		none                                                                          *
*******************************************************************************************/
void Font_itoa ( u32 number, u8 *pBuffer, s32 size )
{
   s32 i;
   
   for( i=size; i>0; i-- )
   {
      pBuffer[i] = (number%10) + 0x30;
      number /= 10;
   }

}

/******************************************************************************************
Function:		void Font_Print( s32 x, s32 y, u8 *text, s32 *levelPosY )                 *
                                                                                          *
Description:	Prints a string of text to the gba screen. Terminated by '\0'              *
               ATM only prints numeric ascii characters.                                  *
                                                                                          *
Args:			x:          X coordinate to start output.                                     *
            y:          Y coordinate to start output.                                     *
            *text:      Pointer to a string of text to display.                           *
            *levelPosY: Pointer to current level posistion of main character              *
                                                                                          *
Returns:		none                                                                          *
*******************************************************************************************/
void Font_Print( s32 x, s32 y, u8 *text )
{   	
	
   s32 i;
   u16 curText;
	s32 levelX, levelY;

	levelX = 0;//gLevelData_LEVELMAN.curLevelX;
	levelY = 0;//gLevelData_LEVELMAN.curLevelY;

   for (i = 0; i < 36; i++ )
   {
      Hide_CHAR( &CHAR_Font[i] );
    
   }

   i = 0;
   
   do
	{
		curText = text[i];
      
      switch ( curText )
      {
      case '0':   
		  CHAR_Font[i].curFrame = 0;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
      break;
      case '1':
		  CHAR_Font[i].curFrame = 1;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break;
      case '2':
		  CHAR_Font[i].curFrame = 2;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break;
      case '3':
		  CHAR_Font[i].curFrame = 3;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break;
      case '4':
		  CHAR_Font[i].curFrame = 4;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break;
      case '5':
		  CHAR_Font[i].curFrame = 5;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break;
      case '6':
		  CHAR_Font[i].curFrame = 6;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break;
      case '7':
		  CHAR_Font[i].curFrame = 7;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break;
      case '8':
		  CHAR_Font[i].curFrame = 8;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break;
      case '9':
		  CHAR_Font[i].curFrame = 9;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break;      
		case 'A':
		case 'a':
		  CHAR_Font[i].curFrame = 10;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'B':
		case 'b':
		  CHAR_Font[i].curFrame = 11;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'C':
		case 'c':
		  CHAR_Font[i].curFrame = 12;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'D':
		case 'd':
		  CHAR_Font[i].curFrame = 13;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'E':
		case 'e':
		  CHAR_Font[i].curFrame = 14;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'F':
		case 'f':
		  CHAR_Font[i].curFrame = 15;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'G':
		case 'g':
		  CHAR_Font[i].curFrame = 16;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'H':
		case 'h':
		  CHAR_Font[i].curFrame = 17;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'I':
		case 'i':
		  CHAR_Font[i].curFrame = 18;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'J':
		case 'j':
		  CHAR_Font[i].curFrame = 19;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'K':
		case 'k':
		  CHAR_Font[i].curFrame = 20;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'L':
		case 'l':
		  CHAR_Font[i].curFrame = 21;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'M':
		case 'm':
		  CHAR_Font[i].curFrame = 22;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'N':
		case 'n':
		  CHAR_Font[i].curFrame = 23;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'O':
		case 'o':
		  CHAR_Font[i].curFrame = 24;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'P':
		case 'p':
		  CHAR_Font[i].curFrame = 25;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'Q':
		case 'q':
		  CHAR_Font[i].curFrame = 26;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'R':
		case 'r':
		  CHAR_Font[i].curFrame = 27;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'S':
		case 's':
		  CHAR_Font[i].curFrame = 28;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'T':
		case 't':
		  CHAR_Font[i].curFrame = 29;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'U':
		case 'u':
		  CHAR_Font[i].curFrame = 30;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'V':
		case 'v':
		  CHAR_Font[i].curFrame = 31;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'W':
		case 'w':
		  CHAR_Font[i].curFrame = 32;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'X':
		case 'x':
		  CHAR_Font[i].curFrame = 33;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'Y':
		case 'y':
		  CHAR_Font[i].curFrame = 34;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
		case 'Z':
		case 'z':
		  CHAR_Font[i].curFrame = 35;
         Show_CHAR( &CHAR_Font[i] );
         CHAR_Font[i].x = SCREEN_TO_LEVEL_X(levelX, x + ( i * 8));
			CHAR_Font[i].y = SCREEN_TO_LEVEL_Y(levelY, y);
			Add_CHAR_OAMMAN(&CHAR_Font[i]);
         break; 
      }
		
		i++;

	} while( text[i] != '\0' );
	
}
