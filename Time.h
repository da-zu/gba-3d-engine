/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: Time.h                                         *
*        Creation Date: 27/07/01                                  *
*        Description: Time functions based on GBA internal timers.*
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
#ifndef TIMEINC
#define TIMEINC

#include <Agb.h>
#include "HBDefines.h"
#include "share.h"

typedef struct
{
    u32 timerNum;       // Timer to use
    u32 startTicks;     // Start of number of ticks before overflow is generated
    u32 preScaler;      // Prescaler value TMR_PRESCALER_1CK, TMR_PRESCALER_64CK, TMR_PRESCALER_256CK, TMR_PRESCALER_1024CK
    u32 interuptEn;     // Enable interupt generation upon overflow
    u32 overflowCount;  // Overflow counter, user not to set. Contains number of overflows that occured if interupt is enabled

}TIMER, *TIMER_PTR;

/******************************************************************************************
Function:		u32 Start_Timer( TIMER_PTR pTimer )                                        *
                                                                                          *
Description:	Start timer. Be warned if timer generates an interupt on overflow,         *
               preformance loss will be encounted.                                        *
                                                                                          *
Args:			pTimer:  Pointer to TIME structure                                            *
                                                                                          *
Returns:		u32   :  Returns OK on success or ERROR on failure                            *
*******************************************************************************************/
//u32 Start_Timer ( TIMER_PTR pTimer );

/******************************************************************************************
Function:		u32 Stop_Timer( TIMER_PTR pTimer )                                         *
                                                                                          *
Description:	Stop timer and check number of ticks.                                      *
                                                                                          *
Args:			pTimer:  Pointer to TIME structure                                            *
                                                                                          *
Returns:		u32   :  Returns Number of ticks timer generated or ERROR on failure          *
*******************************************************************************************/
//u32 Stop_Timer( TIMER_PTR pTimer );

/******************************************************************************************
Function:		void TimerXIntr( void )                                                    *
                                                                                          *
Description:	Timer interupt service routine, called upon overflow.                      *
               Increments overflow counter                                                *
                                                                                          *
Args:			none:                                                                         *
                                                                                          *
Returns:		none:                                                                         *
*******************************************************************************************/
void Timer0Intr( void );
void Timer1Intr( void );
void Timer2Intr( void );
void Timer3Intr( void );
#endif
