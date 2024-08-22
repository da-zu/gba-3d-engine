#include "fix.h"
#include "time.h"
#include "shape.text.iwram.h"
#include "Trig.h"
#include "fix_div_lut.all.rodata.h"

#define SCREEN_WIDTH    239
#define SCREEN_HEIGHT   159
#define XORIGIN         120
#define YORIGIN         80
#define DISTANCE        256

#define XMIN            -50  
#define XMAX            289
#define YMIN            -50
#define YMAX            209
#define ZMIN            400
#define ZMAX            16384

TIMER realfpsTimer; 
u32 avgTime, numSamples, avgTicks;

extern u32 Start_Timer ( TIMER_PTR pTimer );//__attribute__ ((long_call));
extern u32 Stop_Timer ( TIMER_PTR pTimer );//__attribute__ ((long_call));

extern void ProcPolyList(u32 count, clipped_polygon_type *pClipPoly, polygon_list_type *pPolyList, u32 *screen ) __attribute__ ((section (".iwram"), long_call));
extern void ProcPolyList2(u32 count, polygon_list_type *pPolyList, u32 *screen ) __attribute__ ((section (".iwram"), long_call));
		// count
extern void IsView(polygon_type *pPolygons, polygon_type *pPolyList, u32 numPolys, u32 *count) __attribute__ ((section (".iwram"), long_call));
//extern s32 Draw_Poly32(clipped_polygon_type *clip, u32 *screen)  __attribute__ ((section (".iwram"), long_call));

extern void Radix_Sort32( polygon_type *source, polygon_type *dest, u32 N) __attribute__ ((section (".iwram"), long_call));

 
extern world_type world  __attribute__ ((section (".ewram")));
vu16 *screen_buffer2;

clipped_polygon_type clip_array __attribute__ ((section (".ewram")));// __attribute__ ((section (".iwram")));// IN_EWRAM;
polygon_list_type polylist __attribute__ ((section (".ewram")));// IN_EWRAM;

polygon_list_type templist __attribute__ ((section (".ewram")));
// Create a list of all polygons potentially visible in
//  the viewport, removing backfaces and polygons outside
//  of the viewing pyramid in the process

////////////////////////// Recursive QuickSort ///////////////////////
// pointers luts in iwram
//extern	s16 *pCos, *pSin;

/*TIMER funcTimer;

u32 numTicksV;
u8 szticksV[10]; 
u32 cntSampleV;
u32 avgSampleV;
u32 numSamplesV;
*/
extern void inittrans(object_type *pObject);// __attribute__ ((section (".iwram"), long_call));
extern void scale( object_type *pObject);// __attribute__ ((section (".iwram"), long_call));
extern void reflect( object_type *pObject);// __attribute__ ((section (".iwram"), long_call));
extern void rotate( object_type *pObject);// __attribute__ ((section (".iwram"), long_call));
extern void translate( object_type *pObject);// __attribute__ ((section (".iwram"), long_call));
extern void transform( object_type *pObject);// __attribute__ ((section (".iwram"), long_call));

extern void inittrans_view( view_type *view);// __attribute__ ((section (".iwram"), long_call));
extern void rotate_view( view_type *view);// __attribute__ ((section (".iwram"), long_call));
extern void translate_view( view_type *view);// __attribute__ ((section (".iwram"), long_call));

extern void inittrans_boss(object_boss_type *pObject);// __attribute__ ((section (".iwram"), long_call));
extern void translate_boss(object_boss_type *pObject);// __attribute__ ((section (".iwram"), long_call));
extern void scale_boss(object_boss_type *pObject);/// __attribute__ ((section (".iwram"), long_call));
extern void rotate_boss(object_boss_type *pObject);// __attribute__ ((section (".iwram"), long_call));
extern void transform_boss(object_boss_type *pObject);// __attribute__ ((section (".iwram"), long_call));
 
extern void inittrans_char(CHAR_TYP *pChar);// __attribute__ ((section (".iwram"), long_call));
extern void translate_char(CHAR_TYP *pChar);// __attribute__ ((section (".iwram"), long_call));
extern void rotate_char(CHAR_TYP *pChar);// __attribute__ ((section (".iwram"), long_call));
extern void transform_char(CHAR_TYP *pChar);// __attribute__ ((section (".iwram"), long_call));

extern void transform_object( object_type *pObject);// __attribute__ ((section (".iwram"), long_call));

extern void inittrans32(object_type *pObject)  __attribute__ ((section (".iwram"), long_call));
extern void scale32(object_type *pObject)  __attribute__ ((section (".iwram"), long_call));
extern void rotate32(object_type *pObject)  __attribute__ ((section (".iwram"), long_call));
extern void translate32(object_type *pObject)  __attribute__ ((section (".iwram"), long_call));
extern void transform32(object_type *pObject)  __attribute__ ((section (".iwram"), long_call));


extern void inittrans_view32(view_type *pView)  __attribute__ ((section (".iwram"), long_call));
extern void rotate_view32(view_type *pView)  __attribute__ ((section (".iwram"), long_call));
extern void rotate_view32(view_type *pView)  __attribute__ ((section (".iwram"), long_call));

extern void Draw_Poly_Span_Textured( clipped_polygon_type *clip, u8 *screen )  __attribute__ ((section (".text"), long_call));
extern signed long int gba_div(signed long int numerator, signed long int denominator)   __attribute__ ((section (".iwram"), long_call));

extern void Font_itoa ( u32 number, u8 *pBuffer, s32 size );//__attribute__ ((long_call));
extern void Gba_Print( s32 x, s32 y, u8 *text, u8 *screen);//__attribute__ ((long_call));
//extern u32 Start_Timer ( TIMER_PTR pTimer )__attribute__ ((long_call));
//extern u32 Stop_Timer ( TIMER_PTR pTimer )__attribute__ ((long_call));

DRAW_POLY_TYP vars;
/*
void StopTimer(void)
{
	
	numTicksV = Stop_Timer(&funcTimer);
	
	Font_itoa(numTicksV, szticksV, 10 );
	Gba_Print(60,130,szticksV,screen_buffer2);
	
	funcTimer.preScaler = TMR_PRESCALER_64CK;
	funcTimer.interuptEn = 0;
	funcTimer.startTicks = 0;
	funcTimer.timerNum = 0;
	Start_Timer(&funcTimer);
	
	cntSampleV += numTicksV;
	
	numSamplesV++;
	if ( numSamplesV >= 60 )
	{
		numSamplesV = 0;
		avgSampleV = cntSampleV/60;
		cntSampleV = 0;
	}
	Font_itoa(avgSampleV, szticksV, 10 );
	Gba_Print(60,140,szticksV,screen_buffer2);
	
}
*/

// Clip polygon against edges of window with coordinates
//  xmin,ymin,xmax,ymax
 
void xyclip()
{
/*	// Clip against sides of viewport		
	u32 cp, numverts;
	u32 v1, v2;
		
	clip_type *pcv;
	
	pcv=clip_array.vertex;
	// Clip against left edge of viewport:	
	cp = 0;
	
	// Initialize pointer to last vertex:	
	v1=2;//clip_array.number_of_vertices-1;
	numverts = 3;//v1 + 1;//clip_array.number_of_vertices;
	
	for ( v2=0; v2<numverts; v2++) 
	{
		// Categorize edges by type:
		if ((pcv[v1].x >= XMIN) && (pcv[v2].x >= XMIN)) 
		{
			// Edge isn't off left side of viewport
			pcv[cp].x1   = pcv[v2].x;
			pcv[cp++].y1 = pcv[v2].y;
		}
		
		v1=v2;
	}
	clip_array.number_of_vertices = cp;
	numverts = cp;
	v1 = cp - 1;
	// Clip against right edge of viewport:	
	cp = 0;
	
	for (v2=0; v2<numverts; v2++) 
	{
		// Categorize edges by type:
		if ((pcv[v1].x1 <= XMAX) && (pcv[v2].x1 <= XMAX)) 
		{
			// Edge isn't off right side of viewport
			pcv[cp].x = pcv[v2].x1;
			pcv[cp++].y = pcv[v2].y1;
		}
		v1=v2;
	}
	clip_array.number_of_vertices = cp;
	numverts = cp;
	v1 = cp - 1;
	// Clip against upper edge of viewport:
	
	cp = 0;
	
	for (v2=0; v2<numverts; v2++) 
	{
		// Categorize edges by type:
		if ((pcv[v1].y >= YMIN) && (pcv[v2].y >= YMIN)) 
		{
			// Edge is not off top off viewport
			pcv[cp].x1 = pcv[v2].x;
			pcv[cp++].y1 = pcv[v2].y;
		}
		v1=v2;
	}
	clip_array.number_of_vertices = cp;
	numverts = cp;
	v1 = cp - 1;
	// Clip against lower edge of viewport:
	cp = 0;
		
	for (v2=0; v2<numverts; v2++) 
	{
		// Categorize edges by type:
		if ((pcv[v1].y1 <= YMAX) && (pcv[v2].y1 <= YMAX)) 
		{
			// Edge is not off bottom of viewport
			pcv[cp].x = pcv[v2].x1;
			pcv[cp++].y = pcv[v2].y1;
		}
		v1=v2;
	}
	clip_array.number_of_vertices = cp;
	*/
}


void setbuf(u16 *screen_buf)
{	
	screen_buffer2=screen_buf; // Buffer address for screen
}

void setworld(void)
{
	// Registers current world database for use by the
	// view system
	//world=wrld;
	avgTime = 0;
	numSamples = 0;	
	avgTicks = 0;	
}

void display(view_type *curView)
{
/************
// Value vars
	************/	
	u32 count;  // Determine number of polygons in list
	s32 pxmax;
	s32 pxmin;
	s32 pymax;
	s32 pymin;
	s32 pzmax;
	s32 pzmin;
	s32 xcen;
	s32 ycen;
	s32 zcen;
	s32 vx1, vx2, vx3;
	s32 vy1, vy2, vy3;
	s32 vz1, vz2, vz3;
	
	s32 isbackface;
	
	s32 isview;
	//zlip
	
	s32 t;	   
	
	//cproject
	s32 z, num, res1;
	/************
	//lopping vars
	************/
	u32 iobj, iobjvert, ipoly, ichar;
	u32 ilistpoly;
	/************
	//lopping restrictors
	************/	
	u32 numobjs;
	u32 numobjverts;

	u8 szticks[10];
	u32 numTicks;


	
	
	realfpsTimer.preScaler = TMR_PRESCALER_64CK;
	realfpsTimer.interuptEn = 0;
	realfpsTimer.startTicks = 0;
	realfpsTimer.timerNum = 2;
	
											
	count = 0;
	
	numobjs = world.number_of_objects;

	// MATRIX 3x3 Multi
/*
<DMan^rj> 	ldmia		r1,{r3,r4,r5}
<DMan^rj>    	mov		r12,r2
<DMan^rj>    	ldmia		r12,{r6,r7,r8}
<DMan^rj>    	add		r12,r12,#12		@ avoids pipeline stalls
<DMan^rj>    	mul		r9,r6,r3
<DMan^rj>    	mul		r10,r7,r4
<DMan^rj>    	mul		r11,r8,r5
<DMan^rj>    	ldmia		r12,{r6,r7,r8}
<DMan^rj>       	add		r12,r12,#12		@ avoids pipeline stalls
<DMan^rj>    	mla		r9,r6,r3,r9
<DMan^rj>    	mla		r10,r7,r4,r10
<DMan^rj>    	mla		r11,r8,r5,r11
<DMan^rj>    	ldmia		r12,{r6,r7,r8}
<DMan^rj>    	mla		r9,r6,r3,r9
<DMan^rj>    	mla		r10,r7,r4,r10
<DMan^rj>    	mla		r11,r8,r5,r11
<DMan^rj>    	mov		r9,r9,asr#8
<DMan^rj>   	mov		r10,r10,asr#8 
<DMan^rj>   	mov		r11,r11,asr#8    	 
<DMan^rj>    	stmia		r0,{r9,r10,r11}
*/
	//----------1
	/*********************
	* Setup view matrix's
	**********************/
	inittrans_view32(curView);
	
	// Set up translation matrix to shift objects relative
	// to viewer:		
	translate_view(curView);
	
	// Rotate all objects in universe around origin:		
	rotate_view(curView);	
//----------1 = 70 (ARM) 117(THUMB) ticks


	for ( iobj=0; iobj<numobjs; iobj++) 
	{
		u32 i;
		world.object[iobj].isView = 0;// set obj to be in z plane until tested
		
		if ( world.object[iobj].isAlive == 0)
			continue;
		
//		if ( (world.object[iobj].z > curView->copz + ZMIN) &&
//			(world.object[iobj].z < curView->copz + ZMAX) )
		{

//--------------2
			if (world.object[iobj].update == 1) 
			{
				
				// Initialize transformations:			
				inittrans32(&world.object[iobj]);
				
				// Create scaling matrix:			
				scale32(&world.object[iobj]);
				
				// Create rotation matrix			
				rotate(&world.object[iobj]);
				
				// Create translation matrix:			
				translate(&world.object[iobj]);
				
				// Transform OBJECT verts with master transformation matrix:			
				transform(&world.object[iobj]);
				
				//if (world.object[iobj].isCheckCollision)
				{
					// Transform OBJECT with master transformation matrix:			
					transform_object(&world.object[iobj]);
				}
				
				// Indicate update complete:						
				world.object[iobj].update = 0;
			}////////////////////
//-----------2 = 3*970(ARM) = 2910, (THUMB)1970*3 = 5910


//-----------3			
			if (world.object[iobj].isAlive == 1)//isCheckCollision)
			{
				s32 wx, wy, wz;
				
				// Transform objects vertices with view matrix
				//////////////////////////////////
				// START Transform objects vertices with view matrix
				///////////////////////////////////
				
				// Create pointer to current object vertex:			
				//vptr=&world.object[iobj].vertex[iobjvert];
				wx = (s32)world.object[iobj].wx;
				wy = (s32)world.object[iobj].wy;
				wz = (s32)world.object[iobj].wz;
				
				// Calculate new aligned x coordinate:			
				world.object[iobj].ax=(wx*curView->matrix[0][0]
					+wy*curView->matrix[1][0]
					+wz*curView->matrix[2][0]
					+curView->matrix[3][0])>>NUM_SHIFTS;
				
				// Calculate new aligned y coordinate:
				
				world.object[iobj].ay=(wx*curView->matrix[0][1]
					+wy*curView->matrix[1][1]
					+wz*curView->matrix[2][1]
					+curView->matrix[3][1])>>NUM_SHIFTS;
				
				// Calculate new aligned z coordinate:
				
				world.object[iobj].az=(wx*curView->matrix[0][2]
					+wy*curView->matrix[1][2]
					+wz*curView->matrix[2][2]
					+curView->matrix[3][2])>>NUM_SHIFTS;
			}
//-----------3 = 4*3 = 12(ARM), 5*3 = 15(THUMB)
			 
			numobjverts = world.object[iobj].number_of_vertices;

//-----------4			
			for ( iobjvert=0; iobjvert<numobjverts; iobjvert++) 
			{			
				
				s32 wx, wy, wz;
				
				// Transform objects vertices with view matrix
				//////////////////////////////////
				// START Transform objects vertices with view matrix
				///////////////////////////////////
				
				// Create pointer to current object vertex:			
				//vptr=&world.object[iobj].vertex[iobjvert];
				wx = (s32)world.object[iobj].vertex[iobjvert].wx;
				wy = (s32)world.object[iobj].vertex[iobjvert].wy;
				wz = (s32)world.object[iobj].vertex[iobjvert].wz;
				
				// Calculate new aligned x coordinate:			
				world.object[iobj].vertex[iobjvert].ax=(wx*curView->matrix[0][0]
					+wy*curView->matrix[1][0]
					+wz*curView->matrix[2][0]
					+curView->matrix[3][0])>>NUM_SHIFTS;
				
				// Calculate new aligned y coordinate:
				
				world.object[iobj].vertex[iobjvert].ay=(wx*curView->matrix[0][1]
					+wy*curView->matrix[1][1]
					+wz*curView->matrix[2][1]
					+curView->matrix[3][1])>>NUM_SHIFTS;
				
				// Calculate new aligned z coordinate:
				
				world.object[iobj].vertex[iobjvert].az=(wx*curView->matrix[0][2]
					+wy*curView->matrix[1][2]
					+wz*curView->matrix[2][2]
					+curView->matrix[3][2])>>NUM_SHIFTS;
				
				
				//////////////////////////////////
				// END tranform object vertices with view matrix
				///////////////////////////////////
				
				
				// if any of the objects verts are within z far plane
				if ( (world.object[iobj].vertex[iobjvert].az <= ZMAX) &&
					(world.object[iobj].vertex[iobjvert].az > ZMIN) )
				{
					world.object[iobj].isView = 1;
					
				}
				
				
			}//////////////////////
			// end loop numobjverts		 		
//-----------4 = 900 ticks per obj = 3*900 = 2700


			Start_Timer(&realfpsTimer);									
//-----------5						
//			if ( world.object[iobj].isView == 1 )
			{	
				u32 numobjpolys;
				numobjpolys = world.object[iobj].number_of_polygons;				
				
				if (numobjpolys > 0)
				{
					IsView(world.object[iobj].polygon, polylist.polygon, numobjpolys, &count);
				}
				
			}// end if any verts are within z far plane
//-----------5 = 1770 ticks per obj = 3*1770 = 5310,optimise stages = 3*1490 = 4470
			numTicks = Stop_Timer(&realfpsTimer);			
			
		}// end test if in viewing distance
	 
	}//////////////////////
	// end loop numobjs

	

	if (count < 1)
		return;


	
//-----------6
	// BEWARE SP OVERFLOW!!!!!!!! CRASHES WITH large count POLYGONS!!
 	Radix_Sort32(&polylist, &templist, count);
//-----------6 = 9158 ticks, optimise stages = 3360, 3212, 2808, 2470

		
	
//-----------7
	//ProcPolyList(count, &clip_array, &polylist, screen_buffer2);
	ProcPolyList2(count, &polylist, screen_buffer2);
//-----------7 = 24400 ticks, optimise stages = 14090, 9280, 9060,8880,8510,8422
//-----------8
//
// Procpolyist only (no calling drawpoly functions)
//
//-----------8 = 9780 ticks, optimise stages  = 7950, ...., ...., ...., ...., 2283


	Font_itoa(numTicks, szticks, 10 );
	Gba_Print(0,150,szticks,screen_buffer2);

	avgTime += numTicks;
	numSamples++;

	if (numSamples == 60)
	{
		numSamples = 0;
		avgTicks = avgTime / 60;
		avgTime = 0;
	}
	Font_itoa(avgTicks, szticks, 10 );
	Gba_Print(130,150,szticks,screen_buffer2);
	
	//Font_itoa(count, szticks, 10 );
	//Gba_Print(130,150,szticks,screen_buffer2);
}
/*
[10:29] <DmanRj> well... as soon as i saw it i can see an optimization
[10:29] <pharg> that doesnt surprise me
[10:30] <DmanRj> look this:
[10:30] <DmanRj> 		@ isbackface=(vx3*((vz1*vy2)-(vy1*vz2)))+
[10:30] <DmanRj> 		@			(vy3*((vx1*vz2)-(vz1*vx2)))+
[10:30] <DmanRj> 		@			(vz3*((vy1*vx2)-(vx1*vy2)));
[10:30] <DmanRj> 		@ if (isbackface > 0)
[10:31] <pharg> yup
[10:32] <DmanRj> well.. you check in 3d world (CAMERA SPACE) why ???
[10:33] <DmanRj> it can be done FASTER and with the same things in 2d world (SCREEN SPACE)
[10:33] <DmanRj> using just 2 muls and 4 subs and 1 cmp!
[10:34] <pharg> hmm true..but this way i eleminate the backfaces, before I sort the poly's in world space
[10:34] <DmanRj> yeps
[10:34] <pharg> I suppose i could sort in screen space aswell
[10:34] <DmanRj> yes ;>
[10:34] <DmanRj> and also faster ;>
[10:35] <DmanRj> this becomes:
[10:36] <DmanRj>  visible = (vx1-vx0)*(vy2-vy1);
[10:36] <DmanRj>  visible-= (vy1-vy0)*(vx2-vx1);
[10:36] <DmanRj>  if (visible < 0) DRAW IT!
[10:37] <pharg> hmm interesting!
[10:37] <DmanRj> also using asm i can remove that test with 0 and the sub and just compare: IF (vx1-vx0)*(vy2-vy1)<(vy1-vy0)*(vx2-vx1) == VISIBLE
[10:38] <DmanRj> using this you remove LOT of MULS and addisar SURELY slower than a face into a Sorter!!
*/

/* NO SORTING and NO BACKFACE CULLING: 32150 */
/* YES SORTING visible polys and YES BACKFACE CULLING in view space is: 27490 */
/* NO SORTING and YES BACKFACE CULLING in view space is: 25130 */

/* NO SORTING and YES BACKFACE CULLING in screen space: 25600 */
/* YES SORTING all polys and YES BACKFACE CULLING in screen space:  29970 */

/* NOTE: distance calculation is performed on all polys when backfacing in screenspace 
   and so too is sorting... 

   Should do something like... 
   
	 - In the procpolylist functions need to added something like this: loop thru all polygons
   and project and backface cull them into a list of visible polys, then sort the list of visible polys.
   and...
	 - Remove trifillArm from procpolylist function and put into a seperate function that
   loops thru all vertices from the built up projected,sorted,backfaced list of polys.
	- Thus then there is no need to do any calculations or building of new polygon list in IsView.

	once completed these modifcation then the actual ticks gained or loss can be properly 
	calculated.
	 
*/
