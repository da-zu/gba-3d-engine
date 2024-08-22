#ifndef _VIEW32INC_
#define _VIEW32INC_

#include <Agb.h>
#include "Types3D.h"
#include "matrix.h"
#include "HBDefines.h"
#include "math2D.h"

void setbuf(u32 *screen_buf) __attribute__ ((section (".iwram"), long_call));
void setworld(void) __attribute__ ((section (".iwram"), long_call));
void display(view_type *curview) __attribute__ ((section (".iwram"), long_call));

#endif
