 #include <Agb.h>
#include "Types3d.h"
#include "textpoly.h" // include all the header info and classes
#include "gbaIO.h"

// DEFINES //////////////////////////////////////////////////////////////////

// defines for fixed point math
#define FIXP16_SHIFT     16
#define FIXP16_MAG       65536
#define FIXP16_DP_MASK   0x0000ffff
#define FIXP16_WP_MASK   0xffff0000
#define FIXP16_ROUND_UP  0x00008000



// defines for texture mapper triangular analysis
#define TRI_TYPE_NONE           0
#define TRI_TYPE_FLAT_TOP       1 
#define TRI_TYPE_FLAT_BOTTOM	2
#define TRI_TYPE_FLAT_MASK      3
#define TRI_TYPE_GENERAL        4
#define INTERP_LHS              0
#define INTERP_RHS              1
#define MAX_VERTICES_PER_POLY   6

// MACROS ///////////////////////////////////////////////////////////////////

#define SIGN(x) ((x) > 0 ? (1) : (-1))
#define SWAP(a,b,temp) {temp = a; a=b; b=temp;}

// TYPES ///////////////////////////////////////////////////////////////////

// basic types
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

extern long fixedpointasm_div_t(long r0,long r1) __attribute__ ((section (".iwram"), long_call));
extern void gba_setpixel(u32 index, u32 c)  __attribute__ ((section (".iwram"), long_call));
extern signed long int gba_divT(signed long int numerator, signed long int denominator)  __attribute__ ((section (".iwram"), long_call));
extern u32 PrjTable[];


extern u8 levelTmap[160*240] IN_EWRAM; 
extern const u8 texture_IndexBitmap2[64*64 + 0];

// TMAPPER.CPP - The texture mapping function
// compile this and link it to the TMAPDEMO.CPP to
// make a complete executable
// Affine Texture Mapper, for 256 color linear
// memory 64x64 pixel texture mapping of 3 vertex polys.
// there's an example at the bottom!

int poly_clip_min_x = 0;
int poly_clip_max_x = 239;
int poly_clip_min_y = 0;
int poly_clip_max_y = 159;

// GLOBALS /////////////////////////////////////////////////////////////////

typedef struct
{
	s32 u, v;
}UVs;
// FUNCTIONS ////////////////////////////////////////////////////////////////


void Draw_Textured_Triangle(polygon_type *pPolygon,u16 *dest_buffer)
{
	// this function draws a textured triangle
	int mem_pitch = 120;
	
	//FACE3D_PTR face; // working face
	
	int v0=0,
		v1=1,
		v2=2,
		temp=0,
		tri_type = TRI_TYPE_NONE,
		irestart = INTERP_LHS;
	
	int dx,dy,dyl,dyr,      // general deltas
		u,v,
		du,dv,
		xi,yi,              // the current interpolated x,y
		ui,vi,              // the current interpolated u,v
		index_x,index_y,    // looping vars
		x,y,                // hold general x,y
		xstart,
		xend,
		ystart,
		yrestart,
		yend,
		xl,                 
		dxdyl,              
		xr,
		dxdyr,             
		dudyl,    
		ul,
		dvdyl,   
		vll,
		dudyr,
		ur,
		dvdyr,
		vr;
	
	int x0,y0,tu0,tv0,    // cached vertices
		x1,y1,tu1,tv1,
		x2,y2,tu2,tv2;
	
	u16 *screen_ptr;
	u8  *textmap;
	
	UVs	polyuvs[3];
	
	// convert void ptr to face
	//face = (FACE3D_PTR)vface;
	
	// extract texture map
	textmap = texture_IndexBitmap2;
	
	// first trivial clipping rejection tests 
	if (((pPolygon->vertex[0]->sy < poly_clip_min_y)  && 
		(pPolygon->vertex[1]->sy < poly_clip_min_y)  &&
		(pPolygon->vertex[2]->sy < poly_clip_min_y)) ||
		
		((pPolygon->vertex[0]->sy > poly_clip_max_y)  && 
		(pPolygon->vertex[1]->sy > poly_clip_max_y)  &&
		(pPolygon->vertex[2]->sy > poly_clip_max_y)) ||
		
		((pPolygon->vertex[0]->sx < poly_clip_min_x)  && 
		(pPolygon->vertex[1]->sx < poly_clip_min_x)  &&
		(pPolygon->vertex[2]->sx < poly_clip_min_x)) ||
		
		((pPolygon->vertex[0]->sx > poly_clip_max_x)  && 
		(pPolygon->vertex[1]->sx > poly_clip_max_x)  &&
		(pPolygon->vertex[2]->sx > poly_clip_max_x)))
		return;
	
	// degenerate triangle
	if ( ((pPolygon->vertex[0]->sx==pPolygon->vertex[1]->sx) && (pPolygon->vertex[1]->sx==pPolygon->vertex[2]->sx)) ||
		((pPolygon->vertex[0]->sy==pPolygon->vertex[1]->sy) && (pPolygon->vertex[1]->sy==pPolygon->vertex[2]->sy)))
		return;
	
	polyuvs[0].u = -((pPolygon->u0));
	polyuvs[0].v = -((pPolygon->v0));
	
	polyuvs[1].u = -((pPolygon->u1));
	polyuvs[1].v = -((pPolygon->v1));
	
	polyuvs[2].u = -((pPolygon->u2));
	polyuvs[2].v = -((pPolygon->v2));
	
	// sort vertices
	if (pPolygon->vertex[v1]->sy < pPolygon->vertex[v0]->sy) 
	{SWAP(v0,v1,temp);} 
	
	if (pPolygon->vertex[v2]->sy < pPolygon->vertex[v0]->sy) 
	{SWAP(v0,v2,temp);}
	
	if (pPolygon->vertex[v2]->sy < pPolygon->vertex[v1]->sy) 
	{SWAP(v1,v2,temp);}
	
	// now test for trivial flat sided cases
	if (pPolygon->vertex[v0]->sy==pPolygon->vertex[v1]->sy)
	{ 
		// set triangle type
		tri_type = TRI_TYPE_FLAT_TOP;
		
		// sort vertices left to right
		if (pPolygon->vertex[v1]->sx < pPolygon->vertex[v0]->sx) 
		{SWAP(v0,v1,temp);}
		
	} // end if
	else
		// now test for trivial flat sided cases
		if (pPolygon->vertex[v1]->sy==pPolygon->vertex[v2]->sy)
		{ 
			// set triangle type
			tri_type = TRI_TYPE_FLAT_BOTTOM;
			
			// sort vertices left to right
			if (pPolygon->vertex[v2]->sx < pPolygon->vertex[v1]->sx) 
			{SWAP(v1,v2,temp);}
			
		} // end if
		else
		{
			// must be a general triangle
			tri_type = TRI_TYPE_GENERAL;
			
		} // end else
		
		// extract vertices for processing, now that we have order
		x0  = pPolygon->vertex[v0]->sx;
		y0  = pPolygon->vertex[v0]->sy;
		tu0 = polyuvs[v0].u;//face->tlist[v0].u;//pPolygon->u0;
		tv0 = polyuvs[v0].v;
		
		x1  = pPolygon->vertex[v1]->sx;
		y1  = pPolygon->vertex[v1]->sy;
		tu1 = polyuvs[v1].u;
		tv1 = polyuvs[v1].v;
		
		x2  = pPolygon->vertex[v2]->sx;
		y2  = pPolygon->vertex[v2]->sy;
		tu2 = polyuvs[v2].u;
		tv2 = polyuvs[v2].v;
		
		// set interpolation restart value
		yrestart = y1;
		
		// what kind of triangle
		if (tri_type & TRI_TYPE_FLAT_MASK)
		{
			
			if (tri_type == TRI_TYPE_FLAT_TOP)
			{
				// compute all deltas
				dy = (y2 - y0);
				
				dxdyl = ((x2 - x0)   << FIXP16_SHIFT)/dy;
				dudyl = ((tu2 - tu0) << FIXP16_SHIFT)/dy;  
				dvdyl = ((tv2 - tv0) << FIXP16_SHIFT)/dy;    
				
				dxdyr = ((x2 - x1)   << FIXP16_SHIFT)/dy;
				dudyr = ((tu2 - tu1) << FIXP16_SHIFT)/dy;  
				dvdyr = ((tv2 - tv1) << FIXP16_SHIFT)/dy;   
				
				// test for y clipping
				if (y0 < poly_clip_min_y)
				{
					// compute overclip
					dy = (poly_clip_min_y - y0);
					
					// computer new LHS starting values
					xl = dxdyl*dy + (x0  << FIXP16_SHIFT);
					ul = dudyl*dy + (tu0 << FIXP16_SHIFT);
					vll = dvdyl*dy + (tv0 << FIXP16_SHIFT);
					
					// compute new RHS starting values
					xr = dxdyr*dy + (x1  << FIXP16_SHIFT);
					ur = dudyr*dy + (tu1 << FIXP16_SHIFT);
					vr = dvdyr*dy + (tv1 << FIXP16_SHIFT);
					
					// compute new starting y
					ystart = poly_clip_min_y;
					
				} // end if
				else
				{
					// no clipping
					
					// set starting values
					xl = (x0 << FIXP16_SHIFT);
					xr = (x1 << FIXP16_SHIFT);
					
					ul = (tu0 << FIXP16_SHIFT);
					vll = (tv0 << FIXP16_SHIFT);
					
					ur = (tu1 << FIXP16_SHIFT);
					vr = (tv1 << FIXP16_SHIFT);
					
					// set starting y
					ystart = y0;
					
				} // end else
				
			} // end if flat top
			else
			{
				// must be flat bottom
				
				// compute all deltas
				dy = (y1 - y0);
				
				dxdyl = ((x1 - x0)   << FIXP16_SHIFT)/dy;
				dudyl = ((tu1 - tu0) << FIXP16_SHIFT)/dy;  
				dvdyl = ((tv1 - tv0) << FIXP16_SHIFT)/dy;    
				
				dxdyr = ((x2 - x0)   << FIXP16_SHIFT)/dy;
				dudyr = ((tu2 - tu0) << FIXP16_SHIFT)/dy;  
				dvdyr = ((tv2 - tv0) << FIXP16_SHIFT)/dy;   
				
				// test for y clipping
				if (y0 < poly_clip_min_y)
				{
					// compute overclip
					dy = (poly_clip_min_y - y0);
					
					// computer new LHS starting values
					xl = dxdyl*dy + (x0  << FIXP16_SHIFT);
					ul = dudyl*dy + (tu0 << FIXP16_SHIFT);
					vll = dvdyl*dy + (tv0 << FIXP16_SHIFT);
					
					// compute new RHS starting values
					xr = dxdyr*dy + (x0  << FIXP16_SHIFT);
					ur = dudyr*dy + (tu0 << FIXP16_SHIFT);
					vr = dvdyr*dy + (tv0 << FIXP16_SHIFT);
					
					// compute new starting y
					ystart = poly_clip_min_y;
					
				} // end if
				else
				{
					// no clipping
					
					// set starting values
					xl = (x0 << FIXP16_SHIFT);
					xr = (x0 << FIXP16_SHIFT);
					
					ul = (tu0 << FIXP16_SHIFT);
					vll = (tv0 << FIXP16_SHIFT);
					
					ur = (tu0 << FIXP16_SHIFT);
					vr = (tv0 << FIXP16_SHIFT);
					
					// set starting y
					ystart = y0;
					
				} // end else	
				
			} // end else flat bottom
			
			// test for bottom clip, always
			if ((yend = y2) > poly_clip_max_y)
				yend = poly_clip_max_y;
			
			// test for horizontal clipping
			if ((x0 < poly_clip_min_x) || (x0 > poly_clip_max_x) ||
				(x1 < poly_clip_min_x) || (x1 > poly_clip_max_x) ||
				(x2 < poly_clip_min_x) || (x2 > poly_clip_max_x))
			{
				// clip version
				
				// point screen ptr to starting line
				screen_ptr = dest_buffer + (ystart * mem_pitch);
				
				for (yi = ystart; yi<=yend; yi++)
				{
					// compute span endpoints
					xstart = ((xl + FIXP16_ROUND_UP) >> FIXP16_SHIFT);
					xend   = ((xr + FIXP16_ROUND_UP) >> FIXP16_SHIFT);
					
					// compute starting points for u,v interpolants
					ui = ul + FIXP16_ROUND_UP;
					vi = vll + FIXP16_ROUND_UP;
					
					// compute u,v interpolants
					if ((dx = (xend - xstart))>0)
					{
						du = (ur - ul)/dx;
						dv = (vr - vll)/dx;
					} // end if
					else
					{
						du = (ur - ul);
						dv = (vr - vll);
					} // end else
					
					///////////////////////////////////////////////////////////////////////
					
					// test for x clipping, LHS
					if (xstart < poly_clip_min_x)
					{
						// compute x overlap
						dx = poly_clip_min_x - xstart;
						
						// slide interpolants over
						ui+=dx*du;
						vi+=dx*dv;
						
						// reset vars
						xstart = poly_clip_min_x;
						
					} // end if
					
					// test for x clipping RHS
					if (xend > poly_clip_max_x)
						xend = poly_clip_max_x;
					
					///////////////////////////////////////////////////////////////////////
					
					// draw span
					for (xi=xstart; xi<=xend; xi++)
					{
						screen_ptr[xi] = textmap[(ui >> FIXP16_SHIFT) +
                        ((vi >> FIXP16_SHIFT) << 6)];
						
						// interpolate u,v
						ui+=du;
						vi+=dv;
					} // end for xi
					
					// interpolate u,v,x along right and left edge
					xl+=dxdyl;
					ul+=dudyl;
					vll+=dvdyl;
					
					xr+=dxdyr;
					ur+=dudyr;
					vr+=dvdyr;
					
					// advance screen ptr
					screen_ptr+=mem_pitch;
					
				} // end for y
				
			} // end if clip
			else
			{
				// non-clip version
				
				// point screen ptr to starting line
				screen_ptr = dest_buffer + (ystart * mem_pitch);
				
				for (yi = ystart; yi<=yend; yi++)
				{
					// compute span endpoints
					xstart = ((xl + FIXP16_ROUND_UP) >> FIXP16_SHIFT);
					xend   = ((xr + FIXP16_ROUND_UP) >> FIXP16_SHIFT);
					
					// compute starting points for u,v interpolants
					ui = ul + FIXP16_ROUND_UP;
					vi = vll + FIXP16_ROUND_UP;
					
					// compute u,v interpolants
					if ((dx = (xend - xstart))>0)
					{
						du = (ur - ul)/dx;
						dv = (vr - vll)/dx;
					} // end if
					else
					{
						du = (ur - ul);
						dv = (vr - vll);
					} // end else
					
					// draw span
					for (xi=xstart; xi<=xend; xi++)
					{
                        screen_ptr[xi] = textmap[(ui >> FIXP16_SHIFT) +
                        ((vi >> FIXP16_SHIFT) << 6)];
						
						// interpolate u,v
						ui+=du;
						vi+=dv;
					} // end for xi
					
					// interpolate u,v,x along right and left edge
					xl+=dxdyl;
					ul+=dudyl;
					vll+=dvdyl;
					
					xr+=dxdyr;
					ur+=dudyr;
					vr+=dvdyr;
					
					// advance screen ptr
					screen_ptr+=mem_pitch;
					
				} // end for y
				
			} // end if non-clipped
			
	} // end if
	else
		if (tri_type==TRI_TYPE_GENERAL)
		{
			
			// first test for bottom clip, always
			if ((yend = y2) > poly_clip_max_y)
				yend = poly_clip_max_y;
			
			// pre-test y clipping status
			if (y1 < poly_clip_min_y)
			{
				// compute all deltas
				// LHS
				dyl = (y2 - y1);
				
				dxdyl = ((x2  - x1)  << FIXP16_SHIFT)/dyl;
				dudyl = ((tu2 - tu1) << FIXP16_SHIFT)/dyl;  
				dvdyl = ((tv2 - tv1) << FIXP16_SHIFT)/dyl;    
				
				// RHS
				dyr = (y2 - y0);	
				
				dxdyr = ((x2  - x0)  << FIXP16_SHIFT)/dyr;
				dudyr = ((tu2 - tu0) << FIXP16_SHIFT)/dyr;  
				dvdyr = ((tv2 - tv0) << FIXP16_SHIFT)/dyr;   
				
				// compute overclip
				dyr = (poly_clip_min_y - y0);
				dyl = (poly_clip_min_y - y1);
				
				// computer new LHS starting values
				xl = dxdyl*dyl + (x1  << FIXP16_SHIFT);
				ul = dudyl*dyl + (tu1 << FIXP16_SHIFT);
				vll = dvdyl*dyl + (tv1 << FIXP16_SHIFT);
				
				// compute new RHS starting values
				xr = dxdyr*dyr + (x0  << FIXP16_SHIFT);
				ur = dudyr*dyr + (tu0 << FIXP16_SHIFT);
				vr = dvdyr*dyr + (tv0 << FIXP16_SHIFT);
				
				// compute new starting y
				ystart = poly_clip_min_y;
				
				// test if we need swap to keep rendering left to right
				if (dxdyr > dxdyl)
				{
					SWAP(dxdyl,dxdyr,temp);
					SWAP(dudyl,dudyr,temp);
					SWAP(dvdyl,dvdyr,temp);
					SWAP(xl,xr,temp);
					SWAP(ul,ur,temp);
					SWAP(vll,vr,temp);
					SWAP(x1,x2,temp);
					SWAP(y1,y2,temp);
					SWAP(tu1,tu2,temp);
					SWAP(tv1,tv2,temp);
					
					// set interpolation restart
					irestart = INTERP_RHS;
					
				} // end if
				
			} // end if
			else
				if (y0 < poly_clip_min_y)
				{
					// compute all deltas
					// LHS
					dyl = (y1 - y0);
					
					dxdyl = ((x1  - x0)  << FIXP16_SHIFT)/dyl;
					dudyl = ((tu1 - tu0) << FIXP16_SHIFT)/dyl;  
					dvdyl = ((tv1 - tv0) << FIXP16_SHIFT)/dyl;    
					
					// RHS
					dyr = (y2 - y0);	
					
					dxdyr = ((x2  - x0)  << FIXP16_SHIFT)/dyr;
					dudyr = ((tu2 - tu0) << FIXP16_SHIFT)/dyr;  
					dvdyr = ((tv2 - tv0) << FIXP16_SHIFT)/dyr;   
					
					// compute overclip
					dy = (poly_clip_min_y - y0);
					
					// computer new LHS starting values
					xl = dxdyl*dy + (x0  << FIXP16_SHIFT);
					ul = dudyl*dy + (tu0 << FIXP16_SHIFT);
					vll = dvdyl*dy + (tv0 << FIXP16_SHIFT);
					
					// compute new RHS starting values
					xr = dxdyr*dy + (x0  << FIXP16_SHIFT);
					ur = dudyr*dy + (tu0 << FIXP16_SHIFT);
					vr = dvdyr*dy + (tv0 << FIXP16_SHIFT);
					
					// compute new starting y
					ystart = poly_clip_min_y;
					
					// test if we need swap to keep rendering left to right
					if (dxdyr < dxdyl)
					{
						SWAP(dxdyl,dxdyr,temp);
						SWAP(dudyl,dudyr,temp);
						SWAP(dvdyl,dvdyr,temp);
						SWAP(xl,xr,temp);
						SWAP(ul,ur,temp);
						SWAP(vll,vr,temp);
						SWAP(x1,x2,temp);
						SWAP(y1,y2,temp);
						SWAP(tu1,tu2,temp);
						SWAP(tv1,tv2,temp);
						
						// set interpolation restart
						irestart = INTERP_RHS;
						
					} // end if
					
				} // end if
				else
				{
					// no initial y clipping
					
					// compute all deltas
					// LHS
					dyl = (y1 - y0);
					
					dxdyl = ((x1  - x0)  << FIXP16_SHIFT)/dyl;
					dudyl = ((tu1 - tu0) << FIXP16_SHIFT)/dyl;  
					dvdyl = ((tv1 - tv0) << FIXP16_SHIFT)/dyl;    
					
					// RHS
					dyr = (y2 - y0);	
					
					dxdyr = ((x2 - x0)   << FIXP16_SHIFT)/dyr;
					dudyr = ((tu2 - tu0) << FIXP16_SHIFT)/dyr;  
					dvdyr = ((tv2 - tv0) << FIXP16_SHIFT)/dyr;   		
					
					// no clipping y
					
					// set starting values
					xl = (x0 << FIXP16_SHIFT);
					xr = (x0 << FIXP16_SHIFT);
					
					ul = (tu0 << FIXP16_SHIFT);
					vll = (tv0 << FIXP16_SHIFT);
					
					ur = (tu0 << FIXP16_SHIFT);
					vr = (tv0 << FIXP16_SHIFT);
					
					// set starting y
					ystart = y0;
					
					// test if we need swap to keep rendering left to right
					if (dxdyr < dxdyl)
					{
						SWAP(dxdyl,dxdyr,temp);
						SWAP(dudyl,dudyr,temp);
						SWAP(dvdyl,dvdyr,temp);
						SWAP(xl,xr,temp);
						SWAP(ul,ur,temp);
						SWAP(vll,vr,temp);
						SWAP(x1,x2,temp);
						SWAP(y1,y2,temp);
						SWAP(tu1,tu2,temp);
						SWAP(tv1,tv2,temp);
						
						// set interpolation restart
						irestart = INTERP_RHS;
						
					} // end if
					
				} // end else
				
				
				// test for horizontal clipping
				if ((x0 < poly_clip_min_x) || (x0 > poly_clip_max_x) ||
					(x1 < poly_clip_min_x) || (x1 > poly_clip_max_x) ||
					(x2 < poly_clip_min_x) || (x2 > poly_clip_max_x))
				{
					// clip version
					// x clipping	
					
					// point screen ptr to starting line
					screen_ptr = dest_buffer + (ystart * mem_pitch);
					
					for (yi = ystart; yi<=yend; yi++)
					{
						// compute span endpoints
						xstart = ((xl + FIXP16_ROUND_UP) >> FIXP16_SHIFT);
						xend   = ((xr + FIXP16_ROUND_UP) >> FIXP16_SHIFT);
						
						// compute starting points for u,v interpolants
						ui = ul + FIXP16_ROUND_UP;
						vi = vll + FIXP16_ROUND_UP;
						
						// compute u,v interpolants
						if ((dx = (xend - xstart))>0)
						{
							du = (ur - ul)/dx;
							dv = (vr - vll)/dx;
						} // end if
						else
						{
							du = (ur - ul);
							dv = (vr - vll);
						} // end else
						
						///////////////////////////////////////////////////////////////////////
						
						// test for x clipping, LHS
						if (xstart < poly_clip_min_x)
						{
							// compute x overlap
							dx = poly_clip_min_x - xstart;
							
							// slide interpolants over
							ui+=dx*du;
							vi+=dx*dv;
							
							// set x to left clip edge
							xstart = poly_clip_min_x;
							
						} // end if
						
						// test for x clipping RHS
						if (xend > poly_clip_max_x)
							xend = poly_clip_max_x;
						
						///////////////////////////////////////////////////////////////////////
						
						// draw span
						for (xi=xstart; xi<=xend; xi++)
						{
							screen_ptr[xi] = textmap[(ui >> FIXP16_SHIFT) +
							((vi >> FIXP16_SHIFT) << 6)];
							
							// interpolate u,v
							ui+=du;
							vi+=dv;
						} // end for xi
						
						// interpolate u,v,x along right and left edge
						xl+=dxdyl;
						ul+=dudyl;
						vll+=dvdyl;
						
						xr+=dxdyr;
						ur+=dudyr;
						vr+=dvdyr;
						
						// advance screen ptr
						screen_ptr+=mem_pitch;
						
						// test for yi hitting second region, if so change interpolant
						if (yi==yrestart)
						{
							
							// test interpolation side change flag
							
							if (irestart == INTERP_LHS)
							{
								// LHS
								dyl = (y2 - y1);	
								
								dxdyl = ((x2 - x1)   << FIXP16_SHIFT)/dyl;
								dudyl = ((tu2 - tu1) << FIXP16_SHIFT)/dyl;  
								dvdyl = ((tv2 - tv1) << FIXP16_SHIFT)/dyl;   		
								
								// set starting values
								xl = (x1  << FIXP16_SHIFT);
								ul = (tu1 << FIXP16_SHIFT);
								vll = (tv1 << FIXP16_SHIFT);
								
								// interpolate down on LHS to even up
								xl+=dxdyl;
								ul+=dudyl;
								vll+=dvdyl;
							} // end if
							else
							{
								// RHS
								dyr = (y1 - y2);	
								
								dxdyr = ((x1 - x2)   << FIXP16_SHIFT)/dyr;
								dudyr = ((tu1 - tu2) << FIXP16_SHIFT)/dyr;  
								dvdyr = ((tv1 - tv2) << FIXP16_SHIFT)/dyr;   		
								
								// set starting values
								xr = (x2  << FIXP16_SHIFT);
								ur = (tu2 << FIXP16_SHIFT);
								vr = (tv2 << FIXP16_SHIFT);
								
								// interpolate down on RHS to even up
								xr+=dxdyr;
								ur+=dudyr;
								vr+=dvdyr;
								
							} // end else
							
							
						} // end if
						
		} // end for y
		
	} // end if
	else
	{
		// no x clipping
		// point screen ptr to starting line
		screen_ptr = dest_buffer + (ystart * mem_pitch);
		
		for (yi = ystart; yi<=yend; yi++)
		{
			// compute span endpoints
			xstart = ((xl + FIXP16_ROUND_UP) >> FIXP16_SHIFT);
			xend   = ((xr + FIXP16_ROUND_UP) >> FIXP16_SHIFT);
			
			// compute starting points for u,v interpolants
			ui = ul + FIXP16_ROUND_UP;
			vi = vll + FIXP16_ROUND_UP;
			
			// compute u,v interpolants
			if ((dx = (xend - xstart))>0)
			{
				du = (ur - ul)/dx;
				dv = (vr - vll)/dx;
			} // end if
			else
			{
				du = (ur - ul);
				dv = (vr - vll);
			} // end else
			
			// draw span
			for (xi=xstart; xi<=xend; xi++)
			{
				screen_ptr[xi] = textmap[(ui >> FIXP16_SHIFT) +
                        ((vi >> FIXP16_SHIFT) << 6)];
				
				// interpolate u,v
				ui+=du;
				vi+=dv;
			} // end for xi
			
			// interpolate u,v,x along right and left edge
			xl+=dxdyl;
			ul+=dudyl;
			vll+=dvdyl;
			
			xr+=dxdyr;
			ur+=dudyr;
			vr+=dvdyr;
			
			// advance screen ptr
			screen_ptr+=mem_pitch;
			
			// test for yi hitting second region, if so change interpolant
			if (yi==yrestart)
			{
				// test interpolation side change flag
				
				if (irestart == INTERP_LHS)
				{
					// LHS
					dyl = (y2 - y1);	
					
					dxdyl = ((x2 - x1)   << FIXP16_SHIFT)/dyl;
					dudyl = ((tu2 - tu1) << FIXP16_SHIFT)/dyl;  
					dvdyl = ((tv2 - tv1) << FIXP16_SHIFT)/dyl;   		
					
					// set starting values
					xl = (x1  << FIXP16_SHIFT);
					ul = (tu1 << FIXP16_SHIFT);
					vll = (tv1 << FIXP16_SHIFT);
					
					// interpolate down on LHS to even up
					xl+=dxdyl;
					ul+=dudyl;
					vll+=dvdyl;
				} // end if
				else
				{
					// RHS
					dyr = (y1 - y2);	
					
					dxdyr = ((x1 - x2)   << FIXP16_SHIFT)/dyr;
					dudyr = ((tu1 - tu2) << FIXP16_SHIFT)/dyr;  
					dvdyr = ((tv1 - tv2) << FIXP16_SHIFT)/dyr;   		
					
					// set starting values
					xr = (x2  << FIXP16_SHIFT);
					ur = (tu2 << FIXP16_SHIFT);
					vr = (tv2 << FIXP16_SHIFT);
					
					// interpolate down on RHS to even up
					xr+=dxdyr;
					ur+=dudyr;
					vr+=dvdyr;
					
				} // end else
				
			} // end if
			
		} // end for y
		
	} // end else	
	
	} // end if
	
} // end Draw_Textured_Triangle

  /******************************* EXAMPLE ***********************************
  
	This is the setup:
	
	  To create a single triangle to be rendered consisting of the 3
	  points (in CC order I think):
	  
		(x0,y0,u0,v0)
		(x1,y1,u1,v1)
		(x2,y2,u2,v2)
		
		  64x64 texture at address texture_buffer in row major form
		  
			FACE3D face; // the triangle object
			
			  // set up the vertices
			  face.tlist[0].x = x0;
			  face.tlist[0].y = y0;
			  face.tlist[0].u = u0;
			  face.tlist[0].v = v0;
			  
				face.tlist[1].x = x1;
				face.tlist[1].y = y1;
				face.tlist[1].u = u1;
				face.tlist[1].v = v1;
				
				  face.tlist[2].x = x2;
				  face.tlist[2].y = y2;
				  face.tlist[2].u = u2;
				  face.tlist[2].v = v2;
				  
					// assign the texture to your 64x64 texture map buffer
					face.texture = texture_buffer;
					
					  // set the clipping coords of the desired 2D viewport
					  poly_clip_min_x = 0; 
					  poly_clip_max_x = 0;
					  poly_clip_min_y = SCREEN_WIDTH-1;
					  poly_clip_max_y = SCREEN_HEIGHT-1
					  
						// then draw the triangle to the rendering buffer
						Draw_Textured_Triangle((void *)&face, double_buffer, memory_pitch);
						
						  ***************************************************************************/
						  
