/******************************************************************
*        Author: David Zurillo                                    *
*        Filename: Time.c                                         *
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
#include "Time.h"

/************************
*  Globals              *
************************/
TIMER_PTR pDefaultTimer __attribute__ ((section (".ewram")));

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
u32 Start_Timer( TIMER_PTR pTimer )
{  
     //pTimer= pTimer;

   pTimer->overflowCount = 0;

   switch ( pTimer->timerNum )
   {
   case 0:
       {
         // Start timer 0
         *( u16 *)REG_TM0CNT_L = (u16)pTimer->startTicks;              // number of ticks before overflow ( interupt )
         *( u16 *)REG_TM0CNT_H = 0x80 | (pTimer->interuptEn | pTimer->preScaler)>>16;
         
//         SetTimer0Func ( Timer0Intr );                         // set interupt function
         
//         *(vu16 *)REG_IE |= TIMER0_INTR_FLAG;                  // Enable interupt
         
         break; 
      }
   case 1:
      {
         // Start timer 1
         *( u16 *)REG_TM1CNT_L = (u16)pTimer->startTicks;              // number of ticks before overflow ( interupt )
         *( u16 *)REG_TM1CNT_H = 0x80 | (pTimer->interuptEn | pTimer->preScaler)>>16; 
         
//         SetTimer1Func ( Timer1Intr );                         // set interupt function
         
//         *(vu16 *)REG_IE |= TIMER1_INTR_FLAG;                  // Enable interupt
         
         break;
      }
   case 2:
      {
         // Start timer 2
         *( u16 *)REG_TM2CNT_L = (u16)pTimer->startTicks;              // number of ticks before overflow ( interupt )
         *( u16 *)REG_TM2CNT_H = 0x80 | (pTimer->interuptEn | pTimer->preScaler)>>16; 
         
//         SetTimer2Func ( Timer2Intr );                         // set interupt function
         
//         *(vu16 *)REG_IE |= TIMER2_INTR_FLAG;                  // Enable interupt
         
         break;
      }
   case 3:
       {
         // Start timer 3
         *( u16 *)REG_TM3CNT_L = (u16)pTimer->startTicks;              // number of ticks before overflow ( interupt )
         *( u16 *)REG_TM3CNT_H = 0x80 | (pTimer->interuptEn | pTimer->preScaler)>>16;
         
//         SetTimer3Func ( Timer3Intr );                         // set interupt function
         
//         *(vu16 *)REG_IE |= TIMER3_INTR_FLAG;                  // Enable interupt
         
         break;
      }
   default:
      {
         return ( ERROR ); // Invalid timer number
      }
   }
   
   return ( OK );
}

/******************************************************************************************
Function:		u32 Stop_Timer( TIMER_PTR pTimer )                                         *
                                                                                          *
Description:	Stop timer and check number of ticks.                                      *
                                                                                          *
Args:			pTimer:  Pointer to TIME structure                                            *
                                                                                          *
Returns:		u32   :  Returns Number of ticks timer generated or ERROR on failure          *
*******************************************************************************************/
u32 Stop_Timer( TIMER_PTR pTimer )
{
   u32 numCounts;
   
   switch ( pTimer->timerNum )
   {
   case 0:
      {
         // Stop Timer 0
         *( u16 *)REG_TM0CNT_H = 0; 
         
         numCounts = *( u16 *)REG_TM0CNT_L;
         
//         *(vu16 *)REG_IE &= ~TIMER0_INTR_FLAG;   // Disable interupt
         
         break;
      }
   case 1:
      {
         // Stop Timer 1
         *( u16 *)REG_TM1CNT_H = 0; 
         
         numCounts = *( u16 *)REG_TM1CNT_L;
         
//         *(vu16 *)REG_IE &= ~TIMER1_INTR_FLAG;   // Disable interupt
         
         break;
      }
   case 2:
      {
         // Stop Timer 2
         *( u16 *)REG_TM2CNT_H = 0; 
         
         numCounts = *( u16 *)REG_TM2CNT_L;
         
//         *(vu16 *)REG_IE &= ~TIMER2_INTR_FLAG;   // Disable interupt
         
         break;
      }
   case 3:
      {
         // Stop Timer 3
         *( u16 *)REG_TM3CNT_H = 0; 
         
         numCounts = *( u16 *)REG_TM3CNT_L;
         
//         *(vu16 *)REG_IE &= ~TIMER3_INTR_FLAG;   // Disable interupt
         
         break;
      }
   default:
      {
         return ( ERROR ); // Error out of range timer
      }
      
   }
   return ( numCounts );
}

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
/////////////////////////////////////////////////////////////////////
//Timer0 Interrupt Routine
void Timer0Intr(void)
{	
 //  pDefaultTimer->overflowCount += 1;

	*(vu16 *)REG_IF	= TIMER0_INTR_FLAG;		// clear Timer0 interrupt request flag
	
}
/////////////////////////////////////////////////////////////////////
//Timer1 Interrupt Routine
void Timer1Intr(void)
{	
 //  pDefaultTimer->overflowCount += 1;

	*(vu16 *)REG_IF	= TIMER1_INTR_FLAG;		// clear V-Timer1 interrupt request flag
	
}
/////////////////////////////////////////////////////////////////////
//Timer2 Interrupt Routine
void Timer2Intr(void)
{	
//   pDefaultTimer->overflowCount += 1;

	*(vu16 *)REG_IF	= TIMER2_INTR_FLAG;		// clear V-Timer2 interrupt request flag
	
}
/////////////////////////////////////////////////////////////////////
//Timer3 Interrupt Routine
void Timer3Intr(void)
{	
//   pDefaultTimer->overflowCount += 1;

	*(vu16 *)REG_IF	= TIMER3_INTR_FLAG;		// clear V-Timer3 interrupt request flag
	
}
