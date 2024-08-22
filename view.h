#ifndef _VIEWINC_
#define _VIEWINC_

#include <Agb.h>
#include "Types3D.h"
#include "matrix.h"
#include "HBDefines.h"
#include "math2D.h"

void setbuf(u32 *screen_buf);
void setworld(void);
void display(view_type *curview);
void xyclip();

#endif
