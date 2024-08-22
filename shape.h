#ifndef _SHAPEINC_
#define _SHAPEINC_

#include <Agb.h>
#include "Types3D.h"
#include "HBDefines.h"


#define XORIGIN 120
#define YORIGIN 80
typedef struct
{
	clipped_polygon_type *clip;
	u32 *screen;
	u32 color;
	s32 count1;
	s32 count2;		// increment count for edges 1 and 2
	
	u32 counter;			// general counter
	s32 ydiff1;
	s32 ydiff2;		// difference between starting x and ending x
	s32 xdiff1;
	s32 xdiff2;		// difference between starting y and ending y
	
	u32 start;				// starting offset of line between edges
	s32 length;				//	distance from edge1 to edge2
	
	s32 error_term1;
	s32 error_term2;	// error terms for edges 1 and 2
	u32 offset1;
	u32 offset2;	//	offset of current pixel in edges 1 and 2
	
	s32 xunit1;
	s32 xunit2;		//	unit to advance x offset in edges 1 and 2
	
	s32 edge_count;			// number of edges to be drawn
	
	s32 first_vert;
	s32 min_amt;
	
	s32 start_vert1;
	s32 start_vert2;
	s32 xstart1;
	s32 ystart1;
	s32 xstart2;
	s32 ystart2;
	s32 end_vert1;
	s32 end_vert2;
	s32 xend1;
	s32 xend2;
	s32 yend1;
	s32 yend2;
	
	u32 numverts;
}DRAW_POLY_TYP;

void Draw_Poly ( clipped_polygon_type *clip, u32 *screen ) __attribute__ ((section (".iwram")));
//void Draw_Poly ( clipped_polygon_type *clip, u32 *screen ) __attribute__ ((section (".iwram"), long_call));

#endif
