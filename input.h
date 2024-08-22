/*****************************************************************
*        Author:		David Zurillo                             *
*        Filename:		input.h                                  *
*        Creation Date:	20/07/01                                 *
*        Description:	User input functions                     *
*                                                                *
*                                                                *
*        Modification:                                           *
*        -----------------------                                 *
*                                                                *
*                                                                *
*                                                                *
*                                                                *
*                                                                *
*                                                                *
*****************************************************************/ 
#ifndef INPUTINC
#define INPUTINC

#include <Agb.h>

/*****************************************************************
*  Defines                                                       *
*****************************************************************/


/*****************************************************************
*  Variables                                                     *
*****************************************************************/
   

/*****************************************************************
*  Function Prototypes                                           *
*****************************************************************/

/*****************************************************************
Function:		void InitInput( void )                            *
Description:	Initialise the input system.                      *
Args:			None                                                 *
Returns:		None                                                 *
*****************************************************************/
void InitInput( void );

/*****************************************************************
Function:		void UpdateInput( void )                          *
Description:	Updates the input system. This function should    *
				be called once every game loop.                      *
Args:			None                                                 *
Returns:		None                                                 *
*****************************************************************/
void UpdateInput( void );

/*****************************************************************
Function:		u32 KeyPressed( u16 key )                         *
Description:	Check to see if the given key has been pressed.   *
Args:			key: The key to check. Use the standard key          *
					 defines in AgbDefine.h.                          *
Returns:		u32:  1 if the key has been pressed. 0 if the key    *
					 hasn't been pressed.                             *
*****************************************************************/
u32 KeyPressed( u16 key );

/*****************************************************************
Function:		u32 KeyReleased( u16 key, u32* time )             *
Description:	Check to see if the given key has been released.  *
Args:			key: The key to check. Use the standard key          *
					 defines in AgbDefine.h.                          *
				time: A positive value which specifies the           *
					 number of vsyncs the key had been held down      *
					 for before being released.                       *
Returns:		u32:	1 if the key has been released. 0 if it has    *
					not been released.                                *
*****************************************************************/
u32 KeyReleased( u16 key, u32* time );

/*****************************************************************
Function:		u32 KeyUp( u16 key )                              *
Description:	Check to see if the given key is up.              *
Args:			key: The key to check. Use the standard key          *
					 defines in AgbDefine.h.                          *
Returns:		u32:  1 if the key is up. 0 if the key is down       *
*****************************************************************/
u32 KeyUp( u16 key );

/*****************************************************************
Function:		u32 KeyDownTime( u16 key, u32* time )             *
Description:	Check to see if the given key is held down.       *
Args:			key: The key to check. Use the standard key          *
					 defines in AgbDefine.h.                          *
				time: A positive value which specifies the           *
					 number of vsyncs the key was held down for.      *
Returns:		u32:	 1 if the key is been released. 0 if it has    *
					 not been released.                               *
*****************************************************************/
u32 KeyDownTime( u16 key, u32* time );

/*****************************************************************
Function:		u32 KeyDown( u16 key )                            *
Description:	Check to see if the given key is held down.       *
Args:			key: The key to check. Use the standard key          *
					 defines in AgbDefine.h.                          *
Returns:		u32:	 1 if the key is been released. 0 if it has    *
					 not been released.                               *
*****************************************************************/
u32 KeyDown( u16 key );

/*****************************************************************
Function:		u16 KeyHit( void )                                *
Description:	Check to see if tany keys have been pressed.      *
Args:			None                                                 *
Returns:		u16: 0 if no keys have been pressed. Positive        *
					 value if keys have been pressed. Use the         *
					 standard key defines in AgbDefine.h to test      *
					 which keys have been pressed.                    *
*****************************************************************/
u16 KeyHit( void );

void GetKeyBuffer( u32 *last, u32 *current);

#endif
