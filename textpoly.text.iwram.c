#include <Agb.h>
#include "Types3d.h"
#include "textpoly.h" // include all the header info and classes
#include "time.h"

#define SIGN(x) ((x)>0 ? 1: -1)

u8 szp[10];

extern TIMER realfpsTimer; 
extern u32 avgTime, numSamples, avgTicks;
extern u32 Start_Timer ( TIMER_PTR pTimer ) __attribute__ ((long_call));
extern u32 Stop_Timer ( TIMER_PTR pTimer ) __attribute__ ((long_call));
u32 NumzzTicks;

extern u32 calltimes;
extern long fixedpointasm_div(long r0,long r1) __attribute__ ((section (".iwram")));
extern void gba_setpixel(u32 index, u32 c) __attribute__ ((section (".iwram")));

extern void inner(u32 dest,u32 *levelTmap, u32 uv,u32 dudv,u32 width) __attribute__ ((section (".iwram")));
extern void inner2(u32 dest,u32 *levelTmap, u32 uv,u32 dudv,u32 width) __attribute__ ((section (".iwram")));
extern void inner3(u32 dest,u32 *levelTmap, u32 uv,u32 dudv,u32 width) __attribute__ ((section (".iwram")));

//extern void inner(u8 *bitmap, s32 u, s32 v, s32 du, s32 dv, s32 width, s32 x1, u16 *destptr) __attribute__ ((section (".iwram"), long_call));
extern void tritmap( vertexTMap *v1,vertexTMap *v2,vertexTMap *v3, u16 *screen ) __attribute__ ((section (".iwram")));
extern int rightsec(void) __attribute__ ((section (".iwram")));
extern int leftsec(void) __attribute__ ((section (".iwram")));
extern void Font_itoa ( u32 number, u8 *pBuffer, s32 size ) __attribute__ ((long_call));
extern void Gba_Print( s32 x, s32 y, u8 *text, u8 *screen) __attribute__ ((long_call));

extern u32 PrjTable[];

extern u32 VideoBuffer; 
extern const u8 texture_IndexBitmap[256*256 + 0];

inline long ceil(long x)
{
	x += 0xffff;
	return x >> 16;
}


extern u8 levelTmap[256*256] IN_EWRAM; 

//u32 lookupin[512]; 

void Draw_Poly_Span_Textured ( polygon_type *pPolygon, u16 *screen )
{
		u32 numTicks;
	
	//vertexTMap * left_array[3], * right_array[3];
	s32 left_section, right_section;
	s32 left_section_height, right_section_height;
	s32 dudx, dvdx;
	s32 left_u, delta_left_u, left_v, delta_left_v;
	s32 left_x, delta_left_x, right_x, delta_right_x;


	u32 i;
	vertexTMap *v1;
    vertexTMap *v2;
    vertexTMap *v3;
	//vertexTMap *vtemp1, *vtemp2;			

	s32 height, temp, longest;
	
	u16 *destptr;
	s32 op1, op2;
	
	vertexTMap temp_vert[3];

	//u8 * bitmap = levelTmap;
	
	temp_vert[0].u = pPolygon->u0;
	temp_vert[0].v = pPolygon->v0;
	temp_vert[0].x = pPolygon->vertex[0]->sx;
	temp_vert[0].y = pPolygon->vertex[0]->sy;
	
	temp_vert[1].u = pPolygon->u1; 
	temp_vert[1].v = pPolygon->v1;
	temp_vert[1].x = pPolygon->vertex[1]->sx;
	temp_vert[1].y = pPolygon->vertex[1]->sy;
	
	temp_vert[2].u = pPolygon->u2;
	temp_vert[2].v = pPolygon->v2;
	temp_vert[2].x = pPolygon->vertex[2]->sx; 
	temp_vert[2].y = pPolygon->vertex[2]->sy;
	

	/*for (i=0; i<3; i++)
	{
		if (temp_vert[i].x < 0)
			temp_vert[i].x = 0; 
		
		if (temp_vert[i].x > 239)
			temp_vert[i].x = 239;
		
		if (temp_vert[i].y < 0)
			temp_vert[i].y = 0;
		
		if (temp_vert[i].y > 159)
			temp_vert[i].y = 159;
			
	}*/
	
//	DrawTextureTriangle(temp_vert,screen,level01_sec01_litpal_IndexBitmap);
	
//	tritmap( &temp_vert[0],&temp_vert[1],&temp_vert[2],screen );

    v1 = &temp_vert[0]; 
    v2 = &temp_vert[1];
    v3 = &temp_vert[2];
	

	realfpsTimer.preScaler = TMR_PRESCALER_64CK;
	realfpsTimer.interuptEn = 0;
	realfpsTimer.startTicks = 0;
	realfpsTimer.timerNum = 2;

//	Start_Timer(&realfpsTimer);
	tritmap( v1,v2,v3, screen );		      
//	numTicks = Stop_Timer(&realfpsTimer);
//	Font_itoa(numTicks,szp,10);
//	Gba_Print(50,2,szp,screen);

	return;	
	

    // Sort the triangle so that v1 points to the topmost, v2 to the
    // middle and v3 to the bottom vertexTMap.
	
    if(v1->y > v2->y) 
	{ 
		vertexTMap *v;
		
		v = v1; 
		v1 = v2; 
		v2 = v; 
	}
    if(v1->y > v3->y)
	{ 
		vertexTMap *v;
		v = v1; 
		v1 = v3; 
		v3 = v; 
	}
    if(v2->y > v3->y)  
	{ 
		vertexTMap *v;
		v = v2; 
		v2 = v3; 
		v3 = v; 
	}
	
    // We start out by calculating the length of the longest scanline.
	
    height = v3->y - v1->y;
    if(height == 0)
        return;
	
    //temp = ((v2->y - v1->y) << 16) / height;
	//temp = Div(((v2->y - v1->y) << 16) , height);
    op1 = ((v2->y - v1->y));//<<16);
	op2 = PrjTable[height];
	temp = (op1 * op2);//>>16;
	
	/*
	Font_itoa(temp, szp, 10 );
	Gba_Print(0,150,szp,screen);*/
	
	longest = (temp * (v3->x - v1->x)) + ((v1->x - v2->x) << 16);

	
	if(longest == 0)
        return;
	
    // Now that we have the length of the longest scanline we can use that 
    // to tell us which is left and which is the right side of the triangle.
	
    if(longest < 0)
    {     	

	// If longest is neg. we have the middle vertex on the right side.
        // Store the pointers for the right and left edge of the triangle.        
        right_section  = 2;        
        left_section   = 1;
		
		
        // Calculate initial left and right parameters
		{// LEF_SEC START			
			//vtemp1 = v1;//left_array[ left_section ];
			//vtemp2 = v3;//left_array[ left_section-1 ];
			
			//height = vtemp2->y - vtemp1->y;
			height = v3->y - v1->y;
			if(height <= 0)
				return;
	
				
			// Calculate the deltas along this section	
			//op1 = ((vtemp2->x - vtemp1->x));
			op1 = ((v3->x - v1->x));
			op2 = PrjTable[height];
			delta_left_x = (op1 * op2);
			

			//left_x = vtemp1->x << 16;
			left_x = v1->x << 16;
			
			//op1 = ((vtemp2->u - vtemp1->u));
			op1 = ((v3->u - v1->u));
			delta_left_u = (op1 * op2)>>6;
			
						
			//left_u = vtemp1->u << 10;
			left_u = v1->u << 10;
			
			
			//op1 = ((vtemp2->v - vtemp1->v));
			op1 = ((v3->v - v1->v));
			delta_left_v = (op1 * op2)>>6;
			
			//left_v = vtemp1->v << 10;
			left_v = v1->v << 10;
				

			left_section_height = height;			
		}// LEFT_SEC END
		
		{// RIGHT_SEC START					
			//vtemp1 = v1;//right_array[ right_section ];
			//vtemp2 = v2;//right_array[ right_section-1 ];
			
			//height = vtemp2->y - vtemp1->y;
			height = v2->y - v1->y;

			

			if(height <= 0)
			{
				// The first right section had zero height. Use the next section. 
				right_section--;
			
								//vtemp1 = v2;//right_array[ right_section ];
				//vtemp2 = v3;//right_array[ right_section-1 ];
				
				//height = vtemp2->y - vtemp1->y;
				height = v3->y - v2->y;
				
				
				if(height <= 0)	return;
				

				// Calculate the deltas along this section
				//op1 = ((vtemp2->x - vtemp1->x));
				op1 = ((v3->x - v2->x));
				op2 = PrjTable[height];
				delta_right_x = (op1 * op2);
				
				//right_x = vtemp1->x << 16;			
				right_x = v2->x << 16;			

				
				right_section_height = height;		
			}
			else
			{			
				// Calculate the deltas along this section
				//op1 = ((vtemp2->x - vtemp1->x));
				op1 = ((v2->x - v1->x));
				op2 = PrjTable[height];
				delta_right_x = (op1 * op2);
								
				//right_x = vtemp1->x << 16;			
				right_x = v1->x << 16;			
				

				right_section_height = height;		
			}
		}// RIGHT_SEC END
		
		 
        // Ugly compensation so that the dudx,dvdx divides won't overflow
        // if the longest scanline is very short.
        if(longest > -0x1000)
            longest = -0x1000;     


		// Now we calculate the constant deltas for u and v (dudx, dvdx)
		dudx = fixedpointasm_div(temp*(v3->u - v1->u)+((v1->u - v2->u)<<16), longest)>>8;
		
		dvdx = fixedpointasm_div(temp*(v3->v - v1->v)+((v1->v - v2->v)<<16), longest)>>8; 
		
		destptr = (u16 *) (v1->y * 120 + screen);

		

		// Here starts the outer loop (for each scanline)		  
		for(;;)         
		{
			s32 x1, x2;
			s32 width;
			
			//int u  = left_u >> 8;
			//int v  = left_v >> 8;
			//int du = dudx;//   >> 8;
			//int dv = dvdx;//  >> 8;
			u32 uv;
			u32 dudv;
					
			uv = (((left_u>>8) << 16)|((left_v>>8) & 0xffff));
			dudv = ((dudx << 16)|(dvdx & 0xffff));
					
			x1 = ceil(left_x);//>> 16;
		
			

			// test for x clipping
			if (x1 < 0)
			{
				int dx; 
				// compute x overlap
				dx = 0- x1;
				
				// slide interpolants over
				//u+=dx*dudx;//   >> 8;
				//v+=dx*dvdx;//   >> 8;
				uv+=dx*dudv;
				
				x1 = 0;
				
			} // end if
			
			x2 = ceil(right_x);//>> 16;

			// test for x clipping RHS
			if (x2 > 239)
				x2 = 239;
			
			

			width = x2 - x1;

			
			if(width > 0)
			{
				
			
				u8 * dest = (u32)destptr + (u32)x1;				
				if ((dest >= screen) && (dest<= screen+0x05000))
				{
					
					
					inner2(dest,&levelTmap,uv,dudv,width);				
							
				}
			}
			
			destptr += 120;

			

			
			// Interpolate along the left edge of the triangle
			if(--left_section_height <= 0)  // At the bottom of this section?
			{
				return;				
			}
			else
			{
				left_x += delta_left_x;
				left_u += delta_left_u;
				left_v += delta_left_v;

				
			}
			
			
			// Interpolate along the right edge of the triangle
			if(--right_section_height <= 0) // At the bottom of this section?
			{
				if (right_section == 2) // section did not initally have 0 height
				{
					//if(--right_section <= 0)    // All sections done?
						//return;

					right_section--;
					
					{// RIGHT_SEC START					
						//vtemp1 = v2;//right_array[ right_section ];
						//vtemp2 = v3;//right_array[ right_section-1 ];
						
						//height = vtemp2->y - vtemp1->y;
						height = v3->y - v2->y;

						
						if(height <= 0)
						{
							return;
						}
						
						// Calculate the deltas along this section
						//op1 = ((vtemp2->x - vtemp1->x));
						op1 = ((v3->x - v2->x));
						op2 = PrjTable[height];
						delta_right_x = (op1 * op2);
						
						//right_x = vtemp1->x << 16;			
						right_x = v2->x << 16;			

						right_section_height = height;		
					}// RIGHT_SEC END
					
				}
				else // section initally had 0 height
				{
					return;				
				}
				
			}
			else
			{
				right_x += delta_right_x;
				
			}
		}
    }
    else
	{
	
		    // If longest is pos. we have the middle vertex on the left side.
        // Store the pointers for the left and right edge of the triangle.
        //left_array[0]  = v3;
        //left_array[1]  = v2;
        //left_array[2]  = v1;
        left_section   = 2;
        //right_array[0] = v3;
        //right_array[1] = v1;
        right_section  = 1;
		
        // Calculate initial right and left parameters
        {// RIGHT_SEC START					
			//vtemp1 = v1;//right_array[ right_section ];
			//vtemp2 = v3;//right_array[ right_section-1 ];
			
			//height = vtemp2->y - vtemp1->y;
			height = v3->y - v1->y;
	
			
			if(height <= 0)
			{
				return;
			}

						
			// Calculate the deltas along this section
			//op1 = ((vtemp2->x - vtemp1->x));
			op1 = ((v3->x - v1->x));
			op2 = PrjTable[height];
			delta_right_x = (op1 * op2);
			
			//right_x = vtemp1->x << 16;			
			right_x = v1->x << 16;			
			right_section_height = height;		

			

		}// RIGHT_SEC END
		
		//if(rightsec() <= 0)
		//  return;
		
		
		{// LEF_SEC START			
			//vtemp1 = v1;//left_array[ left_section ];
			//vtemp2 = v2;//left_array[ left_section-1 ];
			
			//height = vtemp2->y - vtemp1->y;
			height = v2->y - v1->y;
			
			
			if(height <= 0)
			{
				left_section--;
				
				//vtemp1 = v2;//left_array[ left_section ];
				//vtemp2 = v3;//left_array[ left_section-1 ];
				
				//height = vtemp2->y - vtemp1->y;
				height = v3->y - v2->y;
				
				if(height <= 0)
					return;
				
				// Calculate the deltas along this section	
				//op1 = ((vtemp2->x - vtemp1->x));
				op1 = ((v3->x - v2->x));
				op2 = PrjTable[height];
				delta_left_x = (op1 * op2);
				
				//left_x = vtemp1->x << 16;
				left_x = v2->x << 16;
				
				//op1 = ((vtemp2->u - vtemp1->u));
				op1 = ((v3->u - v2->u));
				delta_left_u = (op1 * op2)>>6;
				//left_u = vtemp1->u << 10;
				left_u = v2->u << 10;
				
				//op1 = ((vtemp2->v - vtemp1->v));
				op1 = ((v3->v - v2->v));
				delta_left_v = (op1 * op2)>>6;
				
				//left_v = vtemp1->v << 10;
				left_v = v2->v << 10;
				
				left_section_height = height;	
			}
			else
			{			
				
				// Calculate the deltas along this section	
				//op1 = ((vtemp2->x - vtemp1->x));
				op1 = ((v2->x - v1->x));
				op2 = PrjTable[height];
				delta_left_x = (op1 * op2);
				
				//left_x = vtemp1->x << 16;
				left_x = v1->x << 16;
			
				//op1 = ((vtemp2->u - vtemp1->u));
				op1 = ((v2->u - v1->u));
				delta_left_u = (op1 * op2)>>6;
				//left_u = vtemp1->u << 10;
				left_u = v1->u << 10;
				
					
				//op1 = ((vtemp2->v - vtemp1->v));
				op1 = ((v2->v - v1->v));
				delta_left_v = (op1 * op2)>>6;
				
				
				//left_v = vtemp1->v << 10; 
				left_v = v1->v << 10;
				
								
				left_section_height = height;			
			}
		}// LEFT_SEC END
		
		 /*if(leftsec() <= 0)
		 {
		 // The first left section had zero height. Use the next section.
		 left_section--;
		 if(leftsec() <= 0)
		 return;
	}*/
		
        // Ugly compensation so that the dudx,dvdx divides won't overflow
        // if the longest scanline is very short.
        if(longest < 0x1000)
            longest = 0x1000;     
		
		
		// Now we calculate the constant deltas for u and v (dudx, dvdx)
		dudx = fixedpointasm_div(temp*(v3->u - v1->u)+((v1->u - v2->u)<<16), longest)>>8;
		
		dvdx = fixedpointasm_div(temp*(v3->v - v1->v)+((v1->v - v2->v)<<16), longest)>>8;
		
		destptr = (u16 *) (v1->y * 120 + screen);
		
				
		// Here starts the outer loop (for each scanline)		  
		for(;;)         
		{
			s32 x1, x2;
			s32 width;
			//u32 *poo;
			//poo = lookupin;
			
			//int u  = left_u >> 8;
			//int v  = left_v >> 8;
			//int du = dudx;//   >> 8;
			//int dv = dvdx;//   >> 8;

			u32 uv;
			u32 dudv;
					
			uv = (((left_u>>8) << 16)|((left_v>>8) & 0xffff));
			dudv = ((dudx << 16)|(dvdx & 0xffff));
		
			
			x1 = ceil(left_x);//>> 16;
			
						
			// test for x clipping
			if (x1 < 0)
			{
				int dx;
				// compute x overlap
				dx = 0- x1;
				
				// slide interpolants over
				//u+=dx*dudx;//   >> 8;
				//v+=dx*dvdx;//   >> 8;
				uv+=dx*dudv;
				
				//left_u+=dx*dudx   >> 8;
				//left_v+=dx*dvdx   >> 8;
				
				//	poo += dx;
				// reset vars
				x1 = 0;
				
			} // end if
			
			x2 = ceil(right_x);//>> 16;
			
			
			// test for x clipping RHS
			if (x2 > 239)
				x2 = 239;
			
			width = x2 - x1;
			
						
			if(width > 0)
			{
								
				u8 * dest = (u32)destptr + (u32)x1;	
				// Watcom C/C++ 10.0 can't get this inner loop any tighter 
				// than about 10-12 clock ticks.
				if ((dest >= screen) && (dest<= screen+0x05000))
				{
					//u += du;
					//v += dv;
					
					
					inner2(dest,&levelTmap,uv,dudv,width);				
					
					
				}
				
			}

			destptr += 120;
			
			
				

			// Interpolate along the left edge of the triangle
			if(--left_section_height <= 0)  // At the bottom of this section?
			{
								
				if (left_section == 2)
				{
					//if(--left_section <= 0)     // All sections done?
					//	return;
					
					left_section--;
					

					{// LEF_SEC START			
						//vtemp1 = v2;//left_array[ left_section ];
						//vtemp2 = v3;//left_array[ left_section-1 ];
						
						//height = vtemp2->y - vtemp1->y;
						height = v3->y - v2->y;
												
						if(height <= 0)
							return;
				
												
						// Calculate the deltas along this section	
						//op1 = ((vtemp2->x - vtemp1->x));
						op1 = ((v3->x - v2->x));
						op2 = PrjTable[height];
						delta_left_x = (op1 * op2);
						
						
						//left_x = vtemp1->x << 16;
						left_x = v2->x << 16;
						
						
						//op1 = ((vtemp2->u - vtemp1->u));
						op1 = ((v3->u - v2->u));
						delta_left_u = (op1 * op2)>>6;						
						left_u = v2->u << 10;
						
												
						//op1 = ((vtemp2->v - vtemp1->v));
						op1 = ((v3->v - v2->v));
						delta_left_v = (op1 * op2)>>6;
						
												
						//left_v = vtemp1->v << 10;
						left_v = v2->v << 10;
						
												
						left_section_height = height;			

												
						// Interpolate along the right edge of the triangle
						if(--right_section_height <= 0) // At the bottom of this section?
						{
							return;
						}
						else
						{
						
							right_x += delta_right_x;

						}
					}// LEFT_SEC END
				}
				else
				{
					return;
				}
				//if(leftsec() <= 0)      // Nope, do the last section
				//   return;
			}
			else
			{
				left_x += delta_left_x;
				left_u += delta_left_u;
				left_v += delta_left_v;

				
							
				// Interpolate along the right edge of the triangle
				if(--right_section_height <= 0) // At the bottom of this section?
				{
					return;
				}
				else
				{					
					right_x += delta_right_x;					

					//Font_itoa(right_x,szp,10);
					//Gba_Print(2,2,szp,screen);
					//return;	
				}
			}
		
			
		}
    }
	
    
}
