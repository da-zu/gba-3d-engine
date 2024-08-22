#include <Agb.h>
#include "Types3d.h"

#define SIGN(x) ((x)>0 ? 1: -1)
extern u32 PrjTable[];

void Stretch(u8 *spriteMap, u8 *screen, s32 x1, s32 x2, s32 y1, s32 y2, s32 yr, s32 yw)
{
	s32 dx, dy, e, d, dx2;
	s32 sx, sy;
	u8 color;

	dx = ABS(x2-x1);

	dy = ABS(y2-y1);

	sx = SIGN(x2-x1);
	sy = SIGN(y2-y1);

	e = (dy)-dx;
	dx2 = dx;
	//dy <<= 1;

	
	
	for(d=0; d<=dx; d++)
	{
		// extract texture map		
		color = spriteMap[y1+yr*32];
		
		if (color != 0)
		{
			//gba_setpixel(screen+x1+yw*240,color);
			//screen[x1+yw*240] = (color|color<<8|color<<16|color<<24);
		}

		while(e >= 0)
		{
			y1 += sy;
			e -= dx2;
		}

		x1 += sx;
		e += dy;

	}
}

void RectStretch( polygon_type *poly, u16 *screen )
{
	s32 xs1, ys1, xs2, ys2;	// source rect dimensions
	s32 xd1, yd1, xd2, yd2;	// dest rect dimensions
	s32 dx, dy, e, d, dx2;
	s32 sx, sy;
	u8 *tmap;


	xs1 = 0;
	ys1 = 0;
	xs2 = 31;
	ys2 = 31;

	xd1 = poly->vertex[0]->sx;
	yd1 = poly->vertex[2]->sy;
	xd2 = poly->vertex[1]->sx;
	yd2 = poly->vertex[1]->sy;

	if(xd2 <= 1)
		return;

	if(xd1 >= 238)
		return;

	if(yd2 <= 1)
		return;
	if(yd1 >= 158)
		return;
/*
	if(xd1 < 0)
		return;
	if(yd1 < 0)
		return;
	if(xd2 < 0)
		return;
	if(yd2 < 0)
		return;

	
	if(xd1 > 239)
		return;
	if(yd1 > 159)
		return;
	if(xd2 > 239)
		return;
	if(yd2 > 159)
		return;
*/
	tmap = (poly->frameNum * (32*32))+poly->spriteMap;

	dx = ABS(yd2-yd1);
	
	dy = ABS(ys2-ys1);
	
	sx = SIGN(yd2-yd1);	
	
	sy = SIGN(ys2-ys1);

	e = (dy)-dx;

	dx2 =dx;
	//dy <<= 1;

	// Clip Y Max
	if ((yd2)>159)
	{
		int left = yd2 - 159;
		dx -= left;
	}

	// Clip Y Min
	if (yd1<-1)
	{
		int left = -1 - yd1;
		dx -= left;
		yd1 += left*sx;
		//ys1 += left*sy;
		e += dy*left;
	}

	// Clip X Max
	if ((xd2)>239)
	{
		int left = xd2 - 239;
		xd2 -= left;
		xs2 -= left;

		if (xs2 < xs1)
			xs2 = xs1;
	}

	// Clip X Min
	if (xd1<0)
	{
		int left = 0 - xd1;
		xd1 += left;
		xs1 += left;

		if (xs1 > xs2)
			xs1 = xs2;
	}

		
	for (d=0; d<=dx; d++)
	{
		//if (yd1>150)
		//	return;
		Stretch(tmap, screen, xd1, xd2, xs1, xs2, ys1, yd1);

		while(e>=0)
		{
			ys1 += sy;
			e -= dx2;
		}

		yd1 += sx;
		e += dy;
	}
		
}