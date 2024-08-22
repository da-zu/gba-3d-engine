

#ifndef _FIXINC_
#define _FIXINC_

#include <Agb.h>
#include "HBDefines.h"


void init_tables(void) IN_EWRAM;
void FastIntDivide (s32 Numer, s32 Denom, s32 *Result, s32 *Remainder);
//--------------------------------------------------------------
//                     Calculate a*b 
//--------------------------------------------------------------
s16 fix_mul( s16 a , s16 b ) IN_IWRAM;
//--------------------------------------------------------------
//                     Calculate a/b 
//--------------------------------------------------------------
s16 fix_div( s16 a , s16 b ) IN_IWRAM;
//--------------------------------------------------------------
//                     Calculate 1/b 
//--------------------------------------------------------------
s16 fix_inverse( s16 b ) IN_IWRAM;

#endif

