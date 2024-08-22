#ifndef _SHAPEINC_
#define _SHAPEINC_

#include <Agb.h>
#include "Types3D.h"
#include "HBDefines.h"



void Draw_Poly ( clipped_polygon_type *clip, u32 *screen ) __attribute__ ((section (".iwram"), long_call));
//void Draw_Poly ( clipped_polygon_type *clip, u32 *screen ) __attribute__ ((section (".iwram"), long_call));

#endif
