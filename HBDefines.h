 /******************************************************************
*        Author: David Zurillo                                    *
*        Filename: HBDefines.h                                    *
*        Creation Date: 09/08/01                                  *
*        Description: Defines for common symbols and macros       *
*                                                                 *                                                                 
*                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/ 
#ifndef HBDEFINESINC
#define HBDEFINESINC

#include "Agb.h"

extern u8 sixtythsec;				// used for random numbers.

/***************************
*  Defines                 *
****************************/
#define IN					0
#define OUT 1

#define FIXED				s32
#define PI					3.141592
#define PITIMES2			6.283185	/* 2 * pi */
#define PIOVER2				1.570796	/* pi / 2 */

#define FALSE				0
#define TRUE				1

#define OFF					0
#define ON					1

#define NO					0
#define YES					1

#define ERROR				-1		
#define OK					0



/***************************
*  Return status defines   *
***************************/
// OAM Manager		-2    to    -25
#define OUTOFIDS			-2		// OAM Manager out of OAM ID's (entries)
#define OUTOFNAMES			-3		// OAM Manager out of NAMES
#define WRONGARGS			-4		// One of the functions args were incorrect 
#define OUTOFOBJRAM			-5		// OAM Manager has ran out of available Obj Char data vram
#define OUTOFROTSCALEIDS	-6		// OAM Manager has ran out of available PX regs for different rotations/scaling

// Background Manager   -26   to    -50


// Sprite Manager       -51   to    -75
#define OUTOFSPRMANENTRIES   -51

// Level Manager        -76   to    -100


// AI System            -101  to    -125


// ISR Manager          -126  to    -150


// Messaging System     -151  to    -175


// Sound System         -176  to    -200


// Timer                -201  to    -225


// HUD                  -226  to    -250


// State System         -251  to    -275


// Physics              -276  to    -300


// Animation            -301  to    -325
#define INVALIDSEQNAME		-301
#define OUTOFANIMMANENTRIES	-302

// Fonts                -326  to    -350


// Input System         -351  to    -375


// Collision System     -376  to    -400
#define INVALIDCHAR			-376
#define INVALIDCOLMAP		-377

// Bit flips for registers
#define BIT00				1
#define BIT01				2
#define BIT02				4
#define BIT03				8
#define BIT04				16
#define BIT05				32
#define BIT06				64
#define BIT07				128
#define BIT08				256
#define BIT09				512
#define BIT10				1024
#define BIT11				2048
#define BIT12				4096
#define BIT13				8192
#define BIT14				16384
#define BIT15				32768

// Screen dimensions
#define SCREEN_WIDTH		240
#define SCREEN_HEIGHT		160

//OAM Attrib0 defines
#define SHAPE_SQUARE		0     
#define SHAPE_HRECT			BIT14 
#define SHAPE_VRECT			BIT15 

#define COLOR_16			0     
#define COLOR_256			BIT13 

#define MOSAIC_ON			BIT12 
#define MOSAIC_OFF			0

#define MODE_NORMAL			0     
#define MODE_SEMITRANS		BIT10 
#define MODE_OBJWINDOW		BIT11 

#define SINGLEFOLD			0
#define DOUBLEFOLD			BIT09

#define ROTSCALE_OFF		0
#define ROTSCALE_ON			BIT08       

//Shape square
#define SIZE_8x8			0
#define SIZE_16x16			1
#define SIZE_32x32			2
#define SIZE_64x64			3
//Shape VRect
#define SIZE_16x8			4
#define SIZE_32x8			5
#define SIZE_32x16			6
#define SIZE_64x32			7
//Shape HRect
#define SIZE_8x16			8
#define SIZE_8x32			9
#define SIZE_16x32			10
#define SIZE_32x64			11

//OAM Attrib1 defines
//Shape square
#define OAMMAN_SIZE_8x8     0     //00
#define OAMMAN_SIZE_16x16   BIT14 //01
#define OAMMAN_SIZE_32x32   BIT15 //10
#define OAMMAN_SIZE_64x64   BIT15 | BIT14 //11
//Shape VRect
#define OAMMAN_SIZE_16x8    0     //00
#define OAMMAN_SIZE_32x8    BIT14 //01
#define OAMMAN_SIZE_32x16   BIT15 //10
#define OAMMAN_SIZE_64x32   BIT15 | BIT14  //11
//Shape HRect
#define OAMMAN_SIZE_8x16    0     //00
#define OAMMAN_SIZE_8x32    BIT14 //01
#define OAMMAN_SIZE_16x32   BIT15 //10
#define OAMMAN_SIZE_32x64   BIT15 | BIT14  //11

//Flip
#define FLIP_VERT			BIT13
#define FLIP_HORZ			BIT12
#define FLIP_NONE			0
#define UNFLIP_HORZ			0xEFFF
#define UNFLIP_VERT			0xDFFF
//Rotation Scaling parameter
#define ROTSCAL_ID_1		0     
#define ROTSCAL_ID_2		BIT09 
#define ROTSCAL_ID_3		BIT10 
#define ROTSCAL_ID_4		BIT09 | BIT10 
#define ROTSCAL_ID_5		BIT11 
#define ROTSCAL_ID_6		BIT09 | BIT11 
#define ROTSCAL_ID_7		BIT10 | BIT11 
#define ROTSCAL_ID_8		BIT09 | BIT10 | BIT11 

//OAM Attrib2 defines
//Palette number 16 color only
#define PALETTE_ID_1		0     
#define PALETTE_ID_2		BIT15 
#define PALETTE_ID_3		BIT14 
#define PALETTE_ID_4		BIT14 | BIT15  
#define PALETTE_ID_5		BIT13 
#define PALETTE_ID_6		BIT13 | BIT15
#define PALETTE_ID_7		BIT13 | BIT14
#define PALETTE_ID_8		BIT13 | BIT14 | BIT15
#define PALETTE_ID_9		BIT12
#define PALETTE_ID_10		BIT12 | BIT15
#define PALETTE_ID_11		BIT12 | BIT14
#define PALETTE_ID_12		BIT12 | BIT14 | BIT15
#define PALETTE_ID_13		BIT12 | BIT13
#define PALETTE_ID_14		BIT12 | BIT13 | BIT15
#define PALETTE_ID_15		BIT12 | BIT13 | BIT14
#define PALETTE_ID_16		BIT12 | BIT13 | BIT14 | BIT15

//Priority of OBJ in relation to Background
#define PRIORITY_1			0     
#define PRIORITY_2			BIT10 
#define PRIORITY_3			BIT11 
#define PRIORITY_4			BIT10 | BIT11  

// CHAR states
#define CHAR_STATE_DEAD		1        // this is a dead CHAR

/*#define CHAR_STATE_ALIVE       1        // this is a live CHAR
#define CHAR_STATE_DYING       2        // this CHAR is dying
#define CHAR_STATE_WALK		   8	      // this CHAR is walking
#define CHAR_STATE_JUMP        16	      // this CHAR is jumping
#define CHAR_STATE_IDLE        32	      // this CHAR is idle
#define CHAR_STATE_FALL        64	      // this CHAR is falling
#define CHAR_STATE_STAND       128	   // this CHAR is standing still
#define CHAR_STATE_RESERVED9   256	   // Reserved for common states
#define CHAR_STATE_RESERVED10  512	   // Reserved for common states
#define CHAR_STATE_RESERVED11  1024	   // Reserved for common states
*/
// CHAR attributes
#define CHAR_ATTR_DUMBCHAR		1      // CHAR is a dumb character, not player controlled or ai conrtolled, but merely exists
#define CHAR_ATTR_AICHAR		2      // CHAR is not a player controlled chararcter and it is a AI controlled char
#define CHAR_ATTR_MAINCHAR		4      // CHAR is the player controlled character
#define CHAR_ATTR_VISIBLE       32     // CHAR is visible
#define CHAR_ATTR_INVISIBLE     0xFFEF // CHAR is invisible
#define CHAR_ATTR_WRAPAROUNDX   256    // CHAR wraps around edges
#define CHAR_ATTR_WRAPAROUNDY   512    // CHAR wraps around edges
#define CHAR_ATTR_LOADED        1024   // the CHAR has been loaded
#define CHAR_ATTR_FLIPPED_HORZ  2048   // CHAR is flipped horz
#define CHAR_ATTR_FLIPPED_VERT  4096   // CHAR is flipped vert
#define CHAR_ATTR_COLLIDEWALLX  8192   // CHAR can collide with wall and velocity will be zero
#define CHAR_ATTR_COLLIDEWALLY  16384  // CHAR can collide with wall and velocity will be zero
#define CHAR_ATTR_RESPAWN       32768  // CHAR will respawn 

// CHAR directions
#define CHAR_DIR_STOP			1
#define CHAR_DIR_LEFT			2
#define CHAR_DIR_RIGHT			4
#define CHAR_DIR_UP				8
#define CHAR_DIR_DOWN			16

// ANIMATION states
#define ANIM_STATE_NONE			0
#define ANIM_STATE_DONE			1        // done animation state
#define ANIM_STATE_STOP			2
#define ANIM_STATE_PLAY			4
#define ANIM_STATE_PAUSE		8
#define ANIM_STATE_ADDED		16
#define ANIM_STATE_FRAME_DONE	32			// Current frame has finished

// ANIMATION SEQUENCE attributes
#define ANIMMAN_SEQ_ATTR_REPEAT	0
#define ANIMMAN_SEQ_ATTR_ONESHOT	1

// Collsision
#define COLLIDE_YES				-1    // Collision occured
#define COLLIDE_NO				0     // No collision occured


#define COL_ACTIVE_BOX			0
#define COL_ACTIVE_TOP			1     // Collision occured when point to test was ABOVE the line previsouly and is now BELOW the line
#define COL_ACTIVE_BOTTOM		2     // Collision occured when point to test was BELOW the line previsouly and is now ABOVE the line
#define COL_ACTIVE_LEFT			4     // Collision occured when point to test was LEFT the line previsouly and is now RIGHT the line     
#define COL_ACTIVE_RIGHT		8     // Collision occured when point to test was RIGHT the line previsouly and is now LEFT the line

#define COL_STATE_KILL_CHAR		16
#define COL_STATE_BOULDER_SIT	32

// Internally set after checked for collision
#define COL_LINE_XDIFF			32
#define COL_LINE_YDIFF			64
#define COL_LINE_HLINE			128
#define COL_LINE_VLINE			256

/***************************
*     General CHAR Defines  *
***************************/

#define COLLISIONBOX_FULL		0
#define COLLISIONBOX_FEET		1
#define COLLISIONBOX_BODY		2


/****************************
*  Level Triggers           *
****************************/


/****************************
*  Macros                   *
****************************/
/***********************/
/* one-argument macros */
/***********************/

/* absolute value of a */
#define ABS(a)		(((a)<0) ? -(a) : (a))

/* round a to nearest int */
#define ROUND(a)	floor((a)+0.5)

/* take sign of a, either -1, 0, or 1 */
#define ZSGN(a)		(((a)<0) ? -1 : (a)>0 ? 1 : 0)	

/* take binary sign of a, either -1, or 1 if >= 0 */
#define SGN(a)		(((a)<0) ? -1 : 1)

/* shout if something that should be true isn't */
#define ASSERT(x) \
if (!(x)) fprintf(stderr," Assert failed: x\n");

/* square a */
#define SQR(a)		((a)*(a))	

/***********************/
/* two-argument macros */
/***********************/

/* find minimum of a and b */
#define MIN(a,b)	(((a)<(b))?(a):(b))	

/* find maximum of a and b */
#define MAX(a,b)	(((a)>(b))?(a):(b))	

/* swap a and b (see Gem by Wyvill) */
#define SWAP(a,b)	{ a^=b; b^=a; a^=b; }

/* linear interpolation from l (when a=0) to h (when a=1)*/
/* (equal to (a*h)+((1-a)*l) */
#define LERP(a,l,h)	((l)+(((h)-(l))*(a)))

/* clamp the input to the specified range */
#define CLAMP(v,l,h)	((v)<(l) ? (l) : (v) > (h) ? (h) : v)

#define IN_IWRAM __attribute__ ((section (".iwram")))
#define IN_EWRAM __attribute__ ((section (".ewram")))

#define IN_IWRAM_LCALL __attribute__ ((section (".iwram"), long_call));
#define IN_EWRAM_LCALL __attribute__ ((section (".ewram"), long_call))

#define RADIAN(n) 		(((float)n)/(float)180*PI)
#define FAILURE(check) (check != OK)

// bit manipulation macros
#define STATE_ON(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define STATE_OFF(word,bit_flag) ((word)=((word) & (~bit_flag)))

#define RAND_MAX(max) ( sixtythsec % (max) )
#define RAND_MIN_MAX(min, max) ( RAND((max) - (min)) + (min) )

// Fixed point math 24:8
#define ITOFX(x) ((x) << 8)				// Integer to fixed point
#define FTOFX(x) ((x) * 256)			// Float to fixed point
#define DTOFX(x) ((x) * 256)			// Double to fixed point
#define FXTOI(x) ((x) >> 8)				// Fixed point to integer
#define FXTOF(x) ((float) (x) / 256)	// Fixed point to float
#define FXTOD(x) ((double)(x) / 256)	// Fixed point to double
#define ADDFX(x,y) ((x) + (y))			// Add a fixed by a fixed
#define SUBFX(x,y) ((x) - (y))			// Subtract a fixed by a fixed
#define MULFX(x,y) (((y) * (x)) >> 8)	// Multiply a fixed by a fixed
#define DIVFX(x,y) ((y << 8) / (x))    	// Divide a fixed by a fixed

#define RGB(R,G,B) ((R)|(G<<5)|(B<<10))

// convert from screen coordinates to level coordinates and back
#define LEVEL_TO_SCREEN_Y(levelPosY, levelY) (levelY - levelPosY)
#define SCREEN_TO_LEVEL_Y(levelPosY, screenY) (levelPosY + screenY)

#define LEVEL_TO_SCREEN_X(levelPosX, levelX) (levelX - levelPosX)
#define SCREEN_TO_LEVEL_X(levelPosX, screenX) (levelPosX + screenX)

#endif
