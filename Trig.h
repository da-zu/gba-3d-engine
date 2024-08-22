#include <Agb.h>
#include "HBDefines.h"

//#define COS_DEG(X) cos_table_deg[X]
//#define SIN_DEG(X) sin_table_deg[X]

#define NUM_DEGS 256
#define NUM_SHIFTS 8
#define NUM_SHIFTS_MULT 1<<NUM_SHIFTS
#define ONE	(1<<NUM_SHIFTS)


// fixed point tables
#define ABS(X) (X<0?-X:X)
#define COS(X) cos_table[ABS(X)&255]
#define SIN(X) sin_table[ABS(X)&255]
//#define COS( X ) ( X ) < 0 ? cos_table[ -( X ) & 255 ] : cos_table[ ( X ) & 255 ] 
//#define SIN( X ) ( X ) < 0 ? -sin_table[ -( X ) & 255 ] : sin_table[ ( X ) & 255 ]

extern const FIXED cos_table[];
extern const FIXED sin_table[];
