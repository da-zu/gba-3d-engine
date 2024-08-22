#include "fix.h"
#include "time.h"
#include "Trig.h"
#include "types3d.h"

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

u8 szp[10];

TIMER realfpsTimer; 
u32 avgTime, numSamples, avgTicks;

extern object_type *objlist[10];

extern u32 VideoBuffer;
extern u32 Start_Timer ( TIMER_PTR pTimer ) __attribute__ ((long_call));
extern u32 Stop_Timer ( TIMER_PTR pTimer ) __attribute__ ((long_call));

extern void z_sort_objs( void ) __attribute__ ((long_call));

extern void setpixel32(u32 index, u32 c)  __attribute__ ((section (".iwram"), long_call));

extern void DrawPolyList32(polygon_list_type *pPolyList, u32 count, u32 video) __attribute__ ((section (".iwram")));		
extern u32 IsView(polygon_type *pPolygons, polygon_list_type *pPolyList, u32 numPolys) __attribute__ ((section (".iwram")));
extern void ViewAlign(object_type *pObj, view_type *pView) __attribute__ ((section (".iwram")));
extern void Radix_Sort32( polygon_type *source, polygon_type *dest, u32 N) __attribute__ ((section (".iwram")));

 
extern world_type world  __attribute__ ((section (".ewram")));

volatile polygon_list_type polylist __attribute__ ((section (".ewram")));// IN_EWRAM;
volatile polygon_list_type templist __attribute__ ((section (".ewram")));
volatile polygon_list_type gndpolylist __attribute__ ((section (".ewram")));// IN_EWRAM;

extern void setcam32(Vector3 *pLookAt, view_type *pView, s32 roll) __attribute__ ((section (".iwram")));


extern void transformNormals( object_type *pObject) __attribute__ ((section (".iwram"), long_call));
extern void transformVec3(s32 x, s32 y, s32 z, s32 *nx, s32 *ny, s32 *nz)  __attribute__ ((section (".iwram"), long_call));

extern void inittrans32(void)  __attribute__ ((section (".iwram")));
extern void scale32(s32 sx, s32 sy, s32 sz)  __attribute__ ((section (".iwram")));
extern void rotate32(s32 ax, s32 ay, s32 az)  __attribute__ ((section (".iwram")));
extern void translate32(s32 xt, s32 yt, s32 zt)  __attribute__ ((section (".iwram")));
extern void transform32(object_type *pObject)  __attribute__ ((section (".iwram")));

extern void inittrans(void)  __attribute__ ((section (".iwram"), long_call));
extern void scale(s32 sx, s32 sy, s32 sz)  __attribute__ ((section (".iwram"), long_call));
extern void rotate(s32 ax, s32 ay, s32 az)  __attribute__ ((section (".iwram"), long_call));
extern void translate(s32 xt, s32 yt, s32 zt)  __attribute__ ((section (".iwram"), long_call));
extern void transform(object_type *pObject)  __attribute__ ((section (".iwram"), long_call));

extern void inittrans_view32(view_type *pView)  __attribute__ ((section (".iwram")));
extern void rotate_view( view_type *view) __attribute__ ((section (".iwram"), long_call));
extern void translate_view( view_type *view) __attribute__ ((section (".iwram"), long_call));



extern void matmult32(s32 result[4][4],s32 mat1[4][4],s32 mat2[4][4]) __attribute__ ((section (".iwram")));


extern s32 V3Length(Vector3 *a) __attribute__ ((long_call));
extern s32 V3DistanceBetween2Points(Point3 *a, Point3 *b) __attribute__ ((long_call));
extern void Font_itoa ( u32 number, u8 *pBuffer, s32 size ) __attribute__ ((long_call));
extern void Gba_Print( s32 x, s32 y, u8 *text, u8 *screen) __attribute__ ((long_call));
extern long fixedpointasm_div2(long r0,long r1) __attribute__ ((section (".iwram")));

extern u32 *PrjTable;

extern int should_be_swapped(polygon_type *poly1,polygon_type *poly2) __attribute__ ((long_call));

void V3Initpoo(Vector3 *newVec, s32 x, s32 y, s32 z)
{
	newVec->x = x;
	newVec->y = y;
	newVec->z = z;	 
}

Vector3 *V3Normalizepoo(Vector3 *v) 
{
	s32 len = V3Length(v)>>8;
		
	if (len != 0) 
	{ 
		{
			v->x = fixedpointasm_div2(v->x , len)>>16;
			v->y = fixedpointasm_div2(v->y , len)>>16; 
			v->z = fixedpointasm_div2(v->z , len)>>16;
		}
		
	}

	return(v);
}

Vector3 *V3Crosspoo(Vector3 *a, Vector3 *b, Vector3 *c)
{
	c->x = (a->y*b->z) - (a->z*b->y);
	c->y = (a->z*b->x) - (a->x*b->z);
	c->z = (a->x*b->y) - (a->y*b->x);
	return(c); 
}
void cammatrixpoo(Vector3 *up, Vector3 *right, Vector3 *forward, view_type *pView)
{
	s32 rot[4][4];
	s32 trans[4][4];
	Vector3 campos;

	V3Initpoo(&campos, pView->copx, pView->copy, pView->copz);


	rot[0][0]=-(right->x);		rot[0][1]=(up->x);		rot[0][2]=(forward->x);	rot[0][3]=0;
	rot[1][0]=-(right->y);		rot[1][1]=(up->y);		rot[1][2]=(forward->y);	rot[1][3]=0;
	rot[2][0]=-(right->z);		rot[2][1]=(up->z);		rot[2][2]=(forward->z);	rot[2][3]=0;
	rot[3][0]=0;			rot[3][1]=0;			rot[3][2]=0;			rot[3][3]=ONE;

	
	trans[0][0]=ONE;		trans[0][1]=0;		trans[0][2]=0;		trans[0][3]=0;
	trans[1][0]=0;		trans[1][1]=ONE;		trans[1][2]=0;		trans[1][3]=0;
	trans[2][0]=0;		trans[2][1]=0;		trans[2][2]=ONE;		trans[2][3]=0;
	trans[3][0]=-pView->copx<<NUM_SHIFTS;		trans[3][1]=-pView->copy<<NUM_SHIFTS;		trans[3][2]=-pView->copz<<NUM_SHIFTS;		trans[3][3]=ONE;

	matmult32(pView->matrix, trans, rot); 
}
void camshitpoo(Vector3 *pLookAt, view_type *pView, s32 roll)
{ 
    
	Vector3 forward, up, right; 
   
	up.x = SIN(roll);  
	up.y = -COS(roll); 
	up.z = 0;
     
   
	forward.x = (pLookAt->x - pView->copx)<<8;
	forward.y = (pLookAt->y - pView->copy)<<8;   
	forward.z = (pLookAt->z - pView->copz)<<8;
	 
	 

	V3Normalizepoo(&forward); 

		
	V3Crosspoo(&up, &forward,&right);
	
	
	V3Normalizepoo(&right);

	V3Crosspoo(&right, &forward, &up); 
 	V3Normalizepoo(&up);

	cammatrixpoo(&up,&right,&forward, pView);


}
/* return the dot product of vectors a and b */
s32 DotProduct(polygon_type *a, s32 bnx,s32 bny,s32 bnz)
{

    return(((a->anx*bnx)+(a->any*bny)+(a->anz*bnz)));
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
	u32 count,realcount;  // Determine number of polygons in list
	
	
	s32 isbackface;
	
	s32 isview;
	//zlip
	
	s32 t;	   
	
	//cproject
	s32 z, num, res1;
	/************
	//lopping vars
	************/
	s32 iobj, iobjvert, ipoly, ichar;
	u32 ilistpoly;
	/************
	//lopping restrictors
	************/	
	u32 numobjs;
	u32 numobjverts;

	u8 szticks[10];
	u32 numTicks;

	s32 i;
Start_Timer(&realfpsTimer);
	
	
	realfpsTimer.preScaler = TMR_PRESCALER_64CK;
	realfpsTimer.interuptEn = 0;
	realfpsTimer.startTicks = 0;
	realfpsTimer.timerNum = 2;
	numTicks = 0;
	
											
	count = realcount = 0;
	
	numobjs = world.number_of_objects;


	/*********************
	* Setup directional lights
	**********************/

	for (i=0; i<world.number_of_dirlights; i++)
	{
		inittrans32();
		//rotate(world.dirlight[i].light_ax, world.dirlight[i].light_ay, world.dirlight[i].light_az);
		// Create rotation matrix			
		/*rotate(world.dirlight[i].light_ax, world.dirlight[i].light_ay, world.dirlight[i].light_az);
		
		world.dirlight[i].light_ax++;
		if (world.dirlight[i].light_ax > 255)
		world.dirlight[i].light_ax = 0;
		
		world.dirlight[i].light_ay++;
		if (world.dirlight[i].light_ay > 255)
		 world.dirlight[i].light_ay = 0;
		  
			world.dirlight[i].light_az++;
			if (world.dirlight[i].light_az > 255) 
		world.dirlight[i].light_az = 0;
		*/
		// Create translation matrix:			
		//translate(world.dirlight[i].light_x,world.dirlight[i].light_y,world.dirlight[i].light_z);
		
		
		transformVec3(0,0,-1<<8,&world.dirlight[i].light_nx,&world.dirlight[i].light_ny,&world.dirlight[i].light_nz);
	
		/*
		// Plot a pixel to represent light
		world.dirlight[i].light_ny >>= 2;
		world.dirlight[i].light_nx >>= 2;
		
		world.dirlight[i].light_ny += 80;
		
		world.dirlight[i].light_nx += 120;
		
		if (world.dirlight[i].light_ny > 159)	
			world.dirlight[i].light_ny = 159;
		if (world.dirlight[i].light_ny < 0)
			world.dirlight[i].light_ny = 0;
		
		if (world.dirlight[i].light_nx > 239)
			world.dirlight[i].light_nx = 239;
		if (world.dirlight[i].light_nx < 0)
			world.dirlight[i].light_nx = 0;*/
		
	//		setpixel32(VideoBuffer+(world.dirlight[i].light_nx)+(world.dirlight[i].light_ny)*240,255);
	//	setpixel32(VideoBuffer+(world.dirlight[i].light_nx+1)+(world.dirlight[i].light_ny)*240,255|255<<8|255<<16|255<<24);
	//	setpixel32(VideoBuffer+(world.dirlight[i].light_nx)+(world.dirlight[i].light_ny+1)*240,255|255<<8|255<<16|255<<24);
	//	setpixel32(VideoBuffer+(world.dirlight[i].light_nx+1)+(world.dirlight[i].light_ny+1)*240,255|255<<8|255<<16|255<<24);

	}

//----------1
	/*********************
	* Setup view matrix's
	**********************/
	inittrans_view32(curView);
	
	if (curView->isTargetCam == 1)
	{
		Vector3 target;
		
		V3Initpoo(&target,curView->targetx,curView->targety,curView->targetz);
	
		camshitpoo(&target, curView, 0);
	}
	else
	{		
		translate_view(curView);
		rotate_view(curView);
	}
//----------1 = 468


	//for ( iobj=numobjs-1; iobj>=0; iobj--) 
	for ( iobj=0; iobj<numobjs; iobj++) 
	{
		u32 i; 
		world.object[iobj].isView = 1;// set obj to be in z plane until tested
		
		if ( world.object[iobj].isAlive == 0)
		{
			continue;
		}

//--------------2

			if (world.object[iobj].update == 1) 
			{
				
				// Initialize transformations:			
				inittrans32();
				
				// Create rotation matrix			
				rotate32(world.object[iobj].xangle, world.object[iobj].yangle, world.object[iobj].zangle);
				
				// Create translation matrix:			
				translate32(world.object[iobj].x,world.object[iobj].y,world.object[iobj].z);
				
				// Create scaling matrix:			
				scale32(world.object[iobj].xscale,world.object[iobj].yscale,world.object[iobj].zscale);
				

				// Transform OBJECT verts with master transformation matrix:			
				transform32(&world.object[iobj]);
				
				if (world.object[iobj].isCheckPolygonCollision)
				{
					inittrans32();

					rotate32(world.object[iobj].xangle, world.object[iobj].yangle, world.object[iobj].zangle);

					transformNormals(&world.object[iobj	]);
				}
				/*if (world.object[iobj].isLit == 1)
				{
					u32 j;
					
					inittrans();
					// Create rotation matrix			
					rotate(world.object[iobj].xangle, world.object[iobj].yangle, world.object[iobj].zangle);
					
					// Create translation matrix:			
					translate(world.object[iobj].x,world.object[iobj].y,world.object[iobj].z);
					
					transformNormals(&world.object[iobj]);
					
					
					for (i=0; i < world.object[iobj].number_of_polygons; i++)
					{
						world.object[iobj].polygon[i].color = 0;

						for (j=0; j<world.number_of_dirlights; j++)
						{
							s32 intensity;
							
							intensity = DotProduct(&world.object[iobj].polygon[i],world.dirlight[j].light_nz,world.dirlight[j].light_ny,world.dirlight[j].light_nx>>1)>>8;
							
							
							//intensity >>= 2;
							//intensity += 31;
							
							if (intensity > 254)
							{
								intensity = 254;
								//while(1);
							}
							if (intensity < 16)
								intensity = 16;
							
							world.object[iobj].polygon[i].color += intensity;
							
							if (world.object[iobj].polygon[i].color > 254)
							{
								world.object[iobj].polygon[i].color = 254;
							}
							
						}
					}
				}*/
				// Indicate update complete:						
				world.object[iobj].update = 0;
			}////////////////////
//-----------2 = SEC01 2340, SEC02 2500, SEC03 2140.
			// 1500 ticks lost because of transformNormals

//-----------3

//			if (world.object[iobj].isAlive == 1)//isCheckCollision)
			{
				s32 wx, wy, wz;
				
				// Transform objects vertices with view matrix
				//////////////////////////////////
				// START Transform objects vertices with view matrix
				///////////////////////////////////
				world.object[iobj].oldax = world.object[iobj].ax;
				world.object[iobj].olday = world.object[iobj].ay;
				world.object[iobj].oldaz = world.object[iobj].az;

				// Create pointer to current object vertex:			
				//vptr=&world.object[iobj].vertex[iobjvert];
				wx = (s32)world.object[iobj].x;
				wy = (s32)world.object[iobj].y;
				wz = (s32)world.object[iobj].z;
				
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

				/*if (iobj >= 2)
				{
				if ((world.object[iobj].az < 100) || (world.object[iobj].az > 2000))
				{
					world.object[iobj].isView = 0;
				}
				}*/
			}
//-----------3 = SEC01 60, SEC02 75, SEC03 31

			//}

//-----------4		

		//	z_sort_objs();
			
//-----------4 = SEC01 329, SEC02 498, SEC03 44

			//for ( iobj=0; iobj<numobjs; iobj++) 
			//{
				if ( world.object[iobj].isAlive == 0)
					continue;
				
								
				if ( world.object[iobj].isView == 1 )
				{		
//------------5
					ViewAlign(&world.object[iobj], curView);
//------------5 = SEC01 1060, SEC02 860, SEC03 1150

					if ( world.object[iobj].isHide == 1)
					continue;

					if (world.object[iobj].number_of_polygons > 0)
					{			
						if (iobj != 0)
						{
//------------6
							count += IsView(world.object[iobj].polygon, &polylist.polygon[count], world.object[iobj].number_of_polygons);					
//------------6 = SEC01 1120, SEC02 830, SEC03 880
						}
						else
						{				
//------------7
							realcount = IsView(world.object[iobj].polygon, &polylist.polygon[realcount], world.object[iobj].number_of_polygons);					
							DrawPolyList32(&polylist, realcount, VideoBuffer);
//------------7 = SEC01 5980, SEC02 5015, SEC03 5215
						}
					}
					
				}// end if any verts are within z far plane
				//-----------5 = 1770 ticks per obj = 3*1770 = 5310,optimise stages = 3*1490 = 4470, 3*1445 = 4335, HW = 3*2374 = 7122, 3*1025 = 3075 (not prjecting), HW = 3*1492 = 4476
				
				
				//}// end test if in viewing distance
		
	}//////////////////////
	// end loop numobjs
	
	
	
	if (count < 1)
		return;


//Start_Timer(&realfpsTimer);	 
//-----------8
	// BEWARE SP OVERFLOW!!!!!!!! CRASHES WITH large count POLYGONS!!
 	Radix_Sort32(&polylist, &templist, count);
//-----------8 = SEC01 780, SEC02 590, SEC03 615
//numTicks += Stop_Timer(&realfpsTimer);
//-----------9
				
	DrawPolyList32(&polylist, count, VideoBuffer);
//-----------9 = SEC01 7800, SEC02 6680, SEC03 6920


	numTicks += Stop_Timer(&realfpsTimer);

	
/*
	avgTime += numTicks;
	numSamples++;

	if (numSamples == 60)
	{
		numSamples = 0;
		avgTicks = avgTime / 60;
		avgTime = 0;
	}
	Font_itoa(avgTicks, szticks, 10 );
	Gba_Print(130,150,szticks,VideoBuffer);
	*/
//	Font_itoa(numTicks, szticks, 10 );
//	Gba_Print(130,150,szticks,VideoBuffer);
	 
}

/******************************************************************
----------------------------Timings--------------------------------

							Emulator

  SEC01: Current: 19937(13.15fps)		Predicted: 17980(14.58fps)



							Hardware

******************************************************************/
