#include "Agb.h"
#include "gbaIO.h"
#include "Types3d.h"
#include "Trig.h"
#include "textpoly.h"

#define XORIGIN 120
#define YORIGIN 80

// the viewport to clip to, can be smaller than the screen
#define poly_clip_min_x 0
#define poly_clip_max_x 240
#define poly_clip_min_y 0
#define poly_clip_max_y 160

u8 szbuffer33[10];



extern void Fill2Array32 (u32 colr, u32 *screen,  u32 length, u32 start) __attribute__ ((section (".iwram"), long_call));

extern void Put1PixelArray32(u32 index, u32 colr, u32 screen) __attribute__ ((section (".iwram"), long_call));

extern void x1GTy1_x2GTy2(DRAW_POLY_TYP *pDrawPolyTyp)  __attribute__ ((section (".iwram"), long_call));
extern void x1GTy1_x2LEy2(DRAW_POLY_TYP *pDrawPolyTyp)  __attribute__ ((section (".iwram"), long_call));
extern void x1LEy1_x2GTy2(DRAW_POLY_TYP *pDrawPolyTyp)  __attribute__ ((section (".iwram"), long_call));
extern void x1LEy1_x2LEy2(DRAW_POLY_TYP *pDrawPolyTyp)  __attribute__ ((section (".iwram"), long_call));

extern void AnotherEdge_Count1(DRAW_POLY_TYP *pDrawPolyTyp)  __attribute__ ((section (".iwram"), long_call));
extern void AnotherEdge_Count2(DRAW_POLY_TYP *pDrawPolyTyp)  __attribute__ ((section (".iwram"), long_call));

// draw polygon in structure polygon
DRAW_POLY_TYP vars;
void Draw_Poly ( clipped_polygon_type *clip, u32 *screen )
{
	s32 counter;//, start, length;
	
	vars.clip = clip;
	vars.screen = screen;

	vars.edge_count = 2;
	
	// determine which vertex is at top of polygon
	// start by assuming vertex 0 is at top
	vars.first_vert = 0;
	vars.min_amt = (clip->vertex[0].y);
	
	
	for ( counter = 1; counter < 3; counter++ )
	{
		s32 tempy;
		tempy = clip->vertex[counter].y; 

			// is next vertex even higher?
		if (  tempy < vars.min_amt )
		{			
		
			// if so make a note of it
			vars.first_vert = counter;
			vars.min_amt = tempy;
		}
	}

	
	// find starting and ending of first 2 edges
	vars.start_vert1 = vars.first_vert;		// edge 1 start
	vars.start_vert2 = vars.first_vert;		// edge 2 start
	vars.xstart1 = clip->vertex[vars.start_vert1].x;	
	vars.ystart1 = clip->vertex[vars.start_vert1].y;
	vars.xstart2 = clip->vertex[vars.start_vert2].x;
	vars.ystart2 = clip->vertex[vars.start_vert2].y;
	
	// get end of edge 1 and check for wrap at vertext
	vars.end_vert1 = vars.start_vert1 - 1;
	

	if ( vars.end_vert1 < 0 )
	{
		vars.end_vert1 = 2;
	}

	vars.xend1 = clip->vertex[vars.end_vert1].x;
	
	vars.yend1 = clip->vertex[vars.end_vert1].y;

	// get end of edge 2 and check for wrap at vertext
	vars.end_vert2 = vars.start_vert2 + 1;
	if ( vars.end_vert2 == 3)
	{
		vars.end_vert2 =0;
	}
	vars.xend2 = clip->vertex[vars.end_vert2].x;
	vars.yend2 = clip->vertex[vars.end_vert2].y;

	// Draw the polygon:
	vars.color = clip->color;
	
	while (vars.edge_count>0) 
	{   
		
		// Continue drawing until all edges drawn
		//1000 start
	 
		//1000 end		
		vars.offset1= ( (vars.ystart1<<7) + (vars.ystart1<<6) + (vars.ystart1<<5) + (vars.ystart1<<4) )+ vars.xstart1;//240*ystart1+xstart1;//+FP_OFF(screen);  // Offset of edge 1
		vars.offset2= ( (vars.ystart2<<7) + (vars.ystart2<<6) + (vars.ystart2<<5) + (vars.ystart2<<4) )+ vars.xstart2;//240*ystart2+xstart2;//+FP_OFF(screen);  // Offset of edge 2
	
		
		vars.error_term1=0;           // Initialize error terms
		vars.error_term2=0;           // for edges 1 & 2
		
		vars.ydiff1=vars.yend1-vars.ystart1;		
		if (vars.ydiff1<0) 
		{
			vars.ydiff1=-vars.ydiff1; // Get absolute value of
		}
		
		vars.ydiff2=vars.yend2-vars.ystart2;
		if (vars.ydiff2<0) 
		{
			vars.ydiff2=-vars.ydiff2; // x & y lengths of edges
		}
		
		vars.xdiff1=vars.xend1-vars.xstart1;
		if (vars.xdiff1<0) 
		{               // Get value of length
			vars.xunit1=-1;                                  // Calculate X increment
			vars.xdiff1=-vars.xdiff1;
		}
		else {
			vars.xunit1=1;
		}
		
		
		vars.xdiff2=vars.xend2-vars.xstart2;
		if (vars.xdiff2<0) {               // Get value of length
			vars.xunit2=-1;                                  // Calculate X increment
			vars.xdiff2=-vars.xdiff2;
		}
		else {
			vars.xunit2=1;
		}
		
		//Font_itoa( xunit2, szbuffer3, 10);
		//Gba_Print( 0, 40, szbuffer3, screen);
		//return;
		
		// Choose which of four routines to use:
		
		if (vars.xdiff1>vars.ydiff1) 
		{    // If X length of edge 1 is greater than y length
			if (vars.xdiff2>vars.ydiff2) 
			{  // If X length of edge 2 is greater than y length
			
				x1GTy1_x2GTy2(&vars);
				// Increment edge 1 on X and edge 2 on X:
				/*
				vars.count1=vars.xdiff1;    // Count for x increment on edge 1
				vars.count2=vars.xdiff2;    // Count for x increment on edge 2
				
				
				while (vars.count1 && vars.count2) 
				{  // Continue drawing until one edge is done
					
					// Calculate edge 1:					
					while ((vars.error_term1<vars.xdiff1)&&(vars.count1)) 
					{ // Finished w/edge 1?
						if (vars.count1) 
						{     // Count down on edge 1
							vars.offset1+=vars.xunit1;  // Increment pixel offset
							vars.xstart1+=vars.xunit1;
						}
						vars.error_term1+=vars.ydiff1; // Increment error term
						vars.count1--;
					}
					vars.error_term1-=vars.xdiff1; // If time to increment X, restore error term
					
				
					// Calculate edge 2:
					
					while ((vars.error_term2<vars.xdiff2)&&(vars.count2)) 
					{  // Finished w/edge 2?
						if (vars.count2) {     // Count down on edge 2
							vars.offset2+=vars.xunit2;  // Increment pixel offset
							vars.xstart2+=vars.xunit2;
						}
						vars.error_term2+=vars.ydiff2; // Increment error term
						vars.count2--;
					}
					vars.error_term2-=vars.xdiff2; // If time to increment X, restore error term
				
						
					

					// Draw line from edge 1 to edge 2:
					
					vars.length=vars.offset2-vars.offset1; // Determine length of horizontal line
					if (vars.length<0) 
					{         // If negative...
						vars.length=-vars.length;       // Make it positive
						vars.start=vars.offset2;        // And set START to edge 2
					}
					else vars.start = vars.offset1;
					
					
					Fill2Array32 (vars.color, vars.screen, vars.length+1, vars.start);
					
					vars.offset1+=240;           // Advance edge 1 offset to next line
					vars.ystart1++;
					vars.offset2+=240;           // Advance edge 2 offset to next line
					vars.ystart2++;

				}
				*/
			}
			else 
			{
				
				//testy(&vars);
				
				// Increment edge 1 on X and edge 2 on Y:
				x1GTy1_x2LEy2(&vars);
			/*	
				vars.count1=vars.xdiff1;    // Count for X increment on edge 1
				vars.count2=vars.ydiff2;    // Count for Y increment on edge 2

				

				while (vars.count1 && vars.count2) 
				{  // Continue drawing until one edge is done

					
					
					// Calculate edge 1:
					
					while ((vars.error_term1<vars.xdiff1)&&(vars.count1)) 
					{ // Finished w/edge 1?
						if (vars.count1) {     // Count down on edge 1
							vars.offset1+=vars.xunit1;  // Increment pixel offset
							vars.xstart1+=vars.xunit1;
						}
						vars.error_term1+=vars.ydiff1; // Increment error term
						vars.count1--;
						
					}
					vars.error_term1-=vars.xdiff1; // If time to increment X, restore error term
					
					
					// Calculate edge 2:
					
					vars.error_term2+=vars.xdiff2; // Increment error term
					
					if (vars.error_term2 >= vars.ydiff2)  
					{ // If time to increment Y...
						vars.error_term2-=vars.ydiff2;        // ...restore error term
						vars.offset2+=vars.xunit2;           // ...and advance offset to next pixel
						vars.xstart2+=vars.xunit2;
					}
					--vars.count2;
					
					
					// Draw line from edge 1 to edge 2:
					
					vars.length=vars.offset2-vars.offset1; // Determine length of horizontal line
					if (vars.length<0) 
					{         // If negative...
						vars.length=-vars.length;       // ...make it positive
						vars.start=vars.offset2;        // And set START to edge 2
					}
					else vars.start=vars.offset1;     // Else set START to edge 1
					
					
					Fill2Array32 (vars.color, vars.screen, vars.length+1, vars.start);
					
					vars.offset1+=240;           // Advance edge 1 offset to next line
					vars.ystart1++;
					vars.offset2+=240;           // Advance edge 2 offset to next line
					vars.ystart2++;
					
						
				}
			*/
			}
		}
		else 
		{
			if (vars.xdiff2>vars.ydiff2) 
			{
				x1LEy1_x2GTy2(&vars);
				/*
				// Increment edge 1 on Y and edge 2 on X:
				
				vars.count1=vars.ydiff1;  // Count for Y increment on edge 1
				vars.count2=vars.xdiff2;  // Count for X increment on edge 2

				

				while(vars.count1 && vars.count2) 
				{  // Continue drawing until one edge is done
					
					
					// Calculate edge 1:
					
					vars.error_term1+=vars.xdiff1; // Increment error term
					if (vars.error_term1 >= vars.ydiff1)  
					{  // If time to increment Y...
						vars.error_term1-=vars.ydiff1;         // ...restore error term
						vars.offset1+=vars.xunit1;            // ...and advance offset to next pixel
						vars.xstart1+=vars.xunit1;
					}
					--vars.count1;
				
					
					// Calculate edge 2:					
					while ((vars.error_term2<vars.xdiff2)&&(vars.count2)) 
					{ // Finished w/edge 1?
						if (vars.count2) 
						{     // Count down on edge 2
							vars.offset2+=vars.xunit2;  // Increment pixel offset
							vars.xstart2+=vars.xunit2;
						}
						vars.error_term2+=vars.ydiff2; // Increment error term
						vars.count2--;
					
					}
					vars.error_term2-=vars.xdiff2;  // If time to increment X, restore error term
					
					// Draw line from edge 1 to edge 2:					
					vars.length=vars.offset2-vars.offset1; // Determine length of horizontal line
					if (vars.length<0) 
					{    // If negative...
						vars.length=-vars.length;  // ...make it positive
						vars.start=vars.offset2;   // And set START to edge 2
					}
					else vars.start=vars.offset1;  // Else set START to edge 1
										

			//		Fill2Array32 (vars.color, vars.screen, vars.length+1, vars.start);
					
					vars.offset1+=240;         // Advance edge 1 offset to next line
					vars.ystart1++;
					vars.offset2+=240;         // Advance edge 2 offset to next line
					vars.ystart2++;
					
				}
				*/
			}
			else 
			{
				x1LEy1_x2LEy2(&vars);
				// Increment edge 1 on Y and edge 2 on Y:
				
				/*
				vars.count1=vars.ydiff1;  // Count for Y increment on edge 1
				vars.count2=vars.ydiff2;  // Count for Y increment on edge 2

				
				while(vars.count1 && vars.count2) 
				{  // Continue drawing until one edge is done
					
					
					// Calculate edge 1:
					vars.error_term1+=vars.xdiff1;  // Increment error term
					if (vars.error_term1 >= vars.ydiff1)  
					{  // If time to increment Y
						vars.error_term1-=vars.ydiff1;         // ...restore error term
						vars.offset1+=vars.xunit1;            // ...and advance offset to next pixel
						vars.xstart1+=vars.xunit1;
					}
					vars.count1--;
					
					
					// Calculate edge 2:
					
					vars.error_term2+=vars.xdiff2; // Increment error term
					
					if (vars.error_term2 >= vars.ydiff2)  
					{  // If time to increment Y
						vars.error_term2-=vars.ydiff2;         // ...restore error term
						vars.offset2+=vars.xunit2;            // ...and advance offset to next pixel
						vars.xstart2+=vars.xunit2;
					}
					vars.count2--;
					
				
					// Draw line from edge 1 to edge 2:
					
					//if (ischange)
					//{
					vars.length=vars.offset2-vars.offset1;  // Determine length of horizontal line
					if (vars.length<0) 
					{          // If negative...
						vars.length=-vars.length;        // ...make it positive
						vars.start=vars.offset2;         // And set START to edge 2
					}
					else 
					{
						vars.start=vars.offset1;      // Else set START to edge 1
					}
					
					
					
					Fill2Array32 (vars.color, vars.screen, vars.length+1, vars.start);
					
					
					vars.offset1+=240;            // Advance edge 1 offset to next line
					vars.ystart1++;
					vars.offset2+=240;            // Advance edge 2 offset to next line
					vars.ystart2++;
					//}
				}

				*/
			}
			
		}
		// Another edge (at least) is complete. Start next edge, if any.
		/*
		if (!vars.count1) {           // If edge 1 is complete...
			--vars.edge_count;           // Decrement the edge count
			vars.start_vert1=vars.end_vert1;   // Make ending vertex into start vertex
			--vars.end_vert1;            // And get new ending vertex
			if (vars.end_vert1<0) vars.end_vert1=clip->number_of_vertices-1; // Check for wrap
			vars.xend1=clip->vertex[vars.end_vert1].x;  // Get x & y of new end vertex
			vars.yend1=clip->vertex[vars.end_vert1].y;
		}*/
		AnotherEdge_Count1(&vars);
		/*if (!vars.count2) {          // If edge 2 is complete...
			--vars.edge_count;          // Decrement the edge count
			vars.start_vert2=vars.end_vert2;  // Make ending vertex into start vertex
			vars.end_vert2++;           // And get new ending vertex
			if (vars.end_vert2==(clip->number_of_vertices)) vars.end_vert2=0; // Check for wrap
			vars.xend2=clip->vertex[vars.end_vert2].x;  // Get x & y of new end vertex
			vars.yend2=clip->vertex[vars.end_vert2].y;

			
		}*/
		AnotherEdge_Count2(&vars);

	}
	
}

void Trans_Object ( object_type *object, s32 matrix[][4] )
{
	//s32 count;
	s32 v;
	// Multiply all vertices in OBJECT with master transformation matrix:
	
	// Multiply all vertices in OBJECT with master transformation matrix:
	
	for ( v=0; v<(*object).number_of_vertices; v++) {
		
		vertex_type *vptr=&(*object).vertex[v];
		
		vptr->wx=((s32)vptr->lx*matrix[0][0]+(s32)vptr->ly*matrix[1][0]
			+(s32)vptr->lz*matrix[2][0]+matrix[3][0])>>NUM_SHIFTS;
		
		vptr->wy=((s32)vptr->lx*matrix[0][1]+(s32)vptr->ly*matrix[1][1]
			+(s32)vptr->lz*matrix[2][1]+matrix[3][1])>>NUM_SHIFTS;
		
		vptr->wz=((s32)vptr->lx*matrix[0][2]+(s32)vptr->ly*matrix[1][2]
			+(s32)vptr->lz*matrix[2][2]+matrix[3][2])>>NUM_SHIFTS;
	}
	
}

// project object onto screen with perspective projection
void Project_Object( object_type *object, s32 distance )
{

	
}

// returns 0 is polygon is visible, -1 if not
// polygono must be part of a convex polyhedron
s32 Test_Backface( polygon_type *polygon )
{/*
 //	vertex_type *v0, *v1, *v2;		// pointer to 3 vertices
	
	  s32 x1, x2, x3;
	  s32 y1, y2, y3;
	  s32 z1, z2, z3;
	  
		s32 c;
		// point to vertices
		//	v0 = polygon.vertex[0];
		//	v1 = polygon.vertex[1];
		//	v2 = polygon.vertex[2];
		
		  x1=polygon->vertex[0]->ax;
		  x2=polygon->vertex[1]->ax;
		  x3=polygon->vertex[2]->ax;
		  y1=polygon->vertex[0]->ay;
		  y2=polygon->vertex[1]->ay;
		  y3=polygon->vertex[2]->ay;
		  z1=polygon->vertex[0]->az;
		  z2=polygon->vertex[1]->az;
		  z3=polygon->vertex[2]->az;
		  
			// Calculate dot product:	
			c=(x3*((z1*y2)-(y1*z2)))+
			(y3*((x1*z2)-(z1*x2)))+
			(z3*((y1*x2)-(x1*y2)));
	*/ 	
	return(0); // return (c<0);
	
}

// draw object in stucture object
void Draw_Object( object_type object, u8 *screen )
{
/*
s32 count;

  //Font_itoa( object.polygon[0].vertex[1]->wx , szbuffer3, 10);
  //Gba_Print( 100, 100, szbuffer3, screen, font8x8_IndexBitmap);
  
	// loop through all polygons in object
	for ( count = 0; count < object.number_of_polygons; count++ )
	{
	// draw current polygon
	if ( object.backface_removal )
	{
	if ( !Test_Backface( object.polygon[count] ) )
	{
	//z_sort(object);
	
	  Draw_Poly( object.polygon[count], screen);
	  
		}
		}
		else
		{
		//z_sort(object);
		Draw_Poly( object.polygon[count], screen);
		
		  }
}*/
}

