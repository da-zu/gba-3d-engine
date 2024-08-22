/******************************************************************
*        Author:		David Zurillo                              *
*        Filename:		input.c                                   *
*        Creation Date:	20/07/01                                  *
*        Description:	User input functions                      *
*                                                                 *
*                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*        23/07/01	SD		Added KeyHit function.                *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/

#include "input.h"

/***************************
*  Defines                 *
****************************/
#define LASTINPUT		0
#define CURRENTINPUT	1

/***************************
*  Variables               *
****************************/
vu16 buffer[2] __attribute__ ((section (".ewram")));		// Current and Last key buffers.
vu32 downtime[10] __attribute__ ((section (".ewram")));	// Length of time keys were held down.
   
/***************************
*  Functions               *
****************************/

/*****************************************************************
Function:		void InitInput( void )                            *
Description:	Initialise the input system.                      *
Args:			None                                                 *
Returns:		None                                                 *
*****************************************************************/
void InitInput( void )
{
	u32 i;

	for (i = 0; i < 10; i++)
	{
		downtime[i] = 0;
	}

	buffer[LASTINPUT] = 0;
	buffer[CURRENTINPUT] = 0;

}

/*****************************************************************
Function:		void UpdateInput( void )                          *
Description:	Updates the input system. This function should    *
				be called once every game loop.                      *
Args:			None                                                 *
Returns:		None                                                 *
*****************************************************************/
void UpdateInput( void )
{
	u32 i;
	u16 key;

	buffer[LASTINPUT] = buffer[CURRENTINPUT];
	buffer[CURRENTINPUT] = (*(vu16 *)REG_KEYINPUT ^ 0x03ff);
//	buffer[CURRENTINPUT] = (*(vu16 *)REG_KEYINPUT ^ 0xffff);

	// Check key down time
	for (i = 0; i < 10; i++)
	{
		key = (u16) (1 << i);
		if ( buffer[LASTINPUT] & key )
		{
			downtime[i]++;
		}
		else
		{
			downtime[i] = 0;
		}
	}
}

/*****************************************************************
Function:		u32 KeyPressed( u16 key )                         *
Description:	Check to see if the given key has been pressed.   *
Args:			key: The key to check. Use the standard key          *
					 defines in AgbDefine.h.                          *
Returns:		u32:  1 if the key has been pressed. 0 if the key    *
					 hasn't been pressed.                             *
*****************************************************************/
u32 KeyPressed( u16 key )
{
	if (!(buffer[LASTINPUT] & key) && (buffer[CURRENTINPUT] & key))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

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
u32 KeyReleased( u16 key, u32* time )
{
	if ((buffer[LASTINPUT] & key) && !(buffer[CURRENTINPUT] & key))
	{
		switch ( key )
		{
		case A_BUTTON:
			*time = downtime[0];
			break;
		case B_BUTTON:
			*time = downtime[1];
			break;
		case SELECT_BUTTON:
			*time = downtime[2];
			break;
		case START_BUTTON:
			*time = downtime[3];
			break;
		case R_KEY:       
			*time = downtime[4];
			break;
		case L_KEY:
			*time = downtime[5];
			break;
		case U_KEY:
			*time = downtime[6];
			break;
		case D_KEY:
			*time = downtime[7];
			break;
		case R_BUTTON:
			*time = downtime[8];
			break;
		case L_BUTTON:
			*time = downtime[9];
			break;
		}

		return 1;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************
Function:		u32 KeyUp( u16 key )                              *
Description:	Check to see if the given key is up.              *
Args:			key: The key to check. Use the standard key          *
					 defines in AgbDefine.h.                          *
Returns:		u32:  1 if the key is up. 0 if the key is down       *
*****************************************************************/
u32 KeyUp( u16 key )
{
	if (!(buffer[CURRENTINPUT] & key))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

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
u32 KeyDownTime( u16 key, u32* time )
{
	u16 keyStat = buffer[CURRENTINPUT] & buffer[LASTINPUT];

	if (keyStat & key)
	{
		switch ( key )
		{
		case A_BUTTON:
			*time = downtime[0];
			break;
		case B_BUTTON:
			*time = downtime[1];
			break;
		case SELECT_BUTTON:
			*time = downtime[2];
			break;
		case START_BUTTON:
			*time = downtime[3];
			break;
		case R_KEY:       
			*time = downtime[4];
			break;
		case L_KEY:
			*time = downtime[5];
			break;
		case U_KEY:
			*time = downtime[6];
			break;
		case D_KEY:
			*time = downtime[7];
			break;
		case R_BUTTON:
			*time = downtime[8];
			break;
		case L_BUTTON:
			*time = downtime[9];
			break;
		}

		return 1;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************
Function:		u32 KeyDown( u16 key, u32* time )                 *
Description:	Check to see if the given key is held down.       *
Args:			key: The key to check. Use the standard key          *
					 defines in AgbDefine.h.                          *
Returns:		u32:	 1 if the key is been released. 0 if it has    *
					 not been released.                               *
*****************************************************************/
u32 KeyDown( u16 key )
{
	u16 keyStat = buffer[CURRENTINPUT] & buffer[LASTINPUT];

	if (keyStat & key)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*****************************************************************
Function:		u16 KeyHit( void )                                *
Description:	Check to see if any keys have been pressed.       *
Args:			None                                                 *
Returns:		u16: 0 if no keys have been pressed. Positive        *
					 value if keys have been pressed. Use the         *
					 standard key defines in AgbDefine.h to test      *
					 which keys have been pressed.                    *
*****************************************************************/
u16 KeyHit( void )
{
	return buffer[CURRENTINPUT];
}

void GetKeyBuffer( u32 *last, u32 *current)
{
	*last = buffer[LASTINPUT];
	*current = buffer[CURRENTINPUT];
}
