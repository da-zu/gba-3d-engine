 /******************************************************************
*        Author: David Zurillo                                    *
*        Filename: math3D.c                                       *
*        Creation Date: 04/10/02                                  *
*        Description:	Utility set for 3D related Math.          *
*                                                                 *
*                                                                 *                                                                 
*                                                                 *
*        Modification:                                            *
*        -----------------------                                  *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
*                                                                 *
******************************************************************/ 
/* NOTES:
*			
*
* TODO:
*			
********************************************************************/
#include "math3d.h"
#include "math2d.h"
#include "Types3d.h"

/******************************
*  Globals                    *
*******************************/

extern u32 VideoBuffer;
extern u32 *PrjTable;
extern long fixedpointasm_div_t(long r0,long r1) __attribute__ ((section (".iwram"), long_call));
/****************************** 
*  Function Definitions       *
*******************************/
/* quadrant id's, incremental angles, accumulated angle values */
typedef short quadrant_type;


	/* determine the quadrant of a polygon point
	   relative to the test point */
#define quadrant(vertex, x, y) ( (vertex->wx > x) ? ((vertex->wy > y) ? 0 : 3) : ( (vertex->wy > y) ? 1 : 2) )

	/* determine x intercept of a polygon edge
	   with a horizontal line at the y value of the test point */
#define x_intercept(pt1, pt2,  yy) (pt2->wx - ( (pt2->wy - yy) * ((pt1->wx - pt2->wx) / (pt1->wy - pt2->wy)) ) )

	/* adjust delta */
#define adjust_delta(delta, vertex, next_vertex, xx, yy)		\
  switch (delta) {							\
	    /* make quadrant deltas wrap around */			\
    case  3:	delta = -1; break;					\
    case -3:	delta =	 1; break;					\
	    /* check if went around point cw or ccw */			\
    case  2: case -2: if (x_intercept(vertex, next_vertex, yy) > xx)	\
		    delta =  - (delta);					\
		break;							\
    }

/* determine if a test point is inside of or outside of a polygon */
/* polygon is "poly", test point is at "x","y" */
s32 point_in_poly(polygon_type *poly, s32 x, s32 y)
{
	u32 index;

	index = 0;
  vertex_type  *vertex, *first_vertex, *next_vertex;
  quadrant_type quad, next_quad, delta, angle;

	    /* initialize */
  vertex = 0;    /* because polygon_get_vertex is a macro */
  vertex = first_vertex = poly->vertex[index++];//polygon_get_vertex(poly,vertex);
  quad = quadrant(vertex, x, y);
  angle = 0;
	    /* loop on all vertices of polygon */
  do {
    next_vertex = poly->vertex[index++];//polygon_get_vertex(poly,vertex);

	/* calculate quadrant and delta from last quadrant */
    next_quad = quadrant(next_vertex, x, y);
    delta = next_quad - quad;
    adjust_delta(delta,vertex,next_vertex,x,y);
	
	/* add delta to total angle sum */
    angle = angle + delta;
	
	/* increment for next step */
    quad = next_quad;
    vertex = next_vertex;
    } while (index <3 );//(vertex != first_vertex);


  /* complete 360 degrees (angle of + 4 or -4 ) means inside */
  /*if ((angle == +4) || (angle == -4)) return 1; else return 0;*/

    /* odd number of windings rule */
   if (angle & 4) return 1; else return 0; 

   /* non-zero winding rule */
   /*if (angle != 0) return 1; else return 0; */
}

s32 Fast_Distance_3D(s32 x, s32 y, s32 z)
{
	// this function computes the distance from 0,0 to x,y with 3.5% error
	
	// first compute the absolute value of x,y
	x = ABS(x);
	y = ABS(y);
	z = ABS(z);
	
	// compute the minimum of x,y
	s32 mn = MIN(x,y);
	mn = MIN(mn,z);
	
	// return the distance
	return(x+y+z-(mn>>1)-(mn>>2)+(mn>>3));
	
} // end Fast_Distance_2D

s64 bSphereTest(object_type *obj1, object_type *obj2, s32 rad1, s32 rad2)
{
	s32 xdif,ydif,zdif;
	s32 xvdif,yvdif,zvdif;


  //D3DVECTOR relPos = obj1->prPosition - obj2->prPosition;

	xdif = (obj1->ax - obj2->ax);
	ydif = (obj1->ay - obj2->ay);
	zdif = (obj1->az - obj2->az);

  s64 dist = xdif * xdif + ydif * ydif + zdif * zdif;
  s64 minDist = rad1 + rad2;

  return dist <= minDist * minDist;
  

	// Relative velocity
  // Relative position
  /*
	xdif = (obj2->ax - obj1->ax);
	ydif = (obj2->ay - obj1->ay);
	zdif = (obj2->az - obj1->az);

	xvdif = vel2x - vel1x;
	yvdif = vel2y - vel1y;
	zvdif = vel2z - vel1z;

  //Minimal distance squared
  s32 r = rad1 + rad2;
  //dP^2-r^2
  s64 pp = xdif * xdif + ydif * ydif + zdif * zdif - r*r;
  //(1)Check if the spheres are already intersecting
  if ( pp < 0 ) return 1;

  //dP*dV
  s64 pv    = xdif * xvdif + ydif * yvdif + ydif * yvdif;
  //(2)Check if the spheres are moving away from each other
  if ( pv >= 0 ) return 0;
  
  //dV^2
  s64 vv = xvdif * xvdif + yvdif * yvdif + zvdif * zvdif;
  //(3)Check if the spheres can intersect within 1 frame
  if ( (pv + vv) <= 0 && (vv + 2 * pv + pp) >= 0 ) return 0;

  //Discriminant/4
  s64 D    = pv * pv - pp * vv; 
  return ( D > 0 );
*/
}

/* returns squared length of input vector */	
s64 V3SquaredLength(Vector3 *a) 
{
	return(((s64)a->x * (s64)a->x)+((s64)a->y * (s64)a->y)+((s64)a->z * (s64)a->z));
}

/* returns length of input vector */
s32 V3Length(Vector3 *a) 
{ 
	return(fsqrt(V3SquaredLength(a))); 
	 
}
 
/* negates the input vector and returns it */ 
Vector3 *V3Negate(Vector3 *v)  
{
	v->x = -v->x;  v->y = -v->y;  v->z = -v->z;
	return(v);
} 

/* normalizes the input vector and returns it */
Vector3 *V3Normalize(Vector3 *v) 
{
	s32 len = V3Length(v)>>8;
		
	if (len != 0) 
	{ 
		{
			v->x = fixedpointasm_div_t(v->x , len)>>16;
			v->y = fixedpointasm_div_t(v->y , len)>>16; 
			v->z = fixedpointasm_div_t(v->z , len)>>16;
		}
		
	}

	return(v);
}

/* scales the input vector to the new length and returns it */
Vector3 *V3Scale(Vector3 *v, s32 newlen) 
{
	s32 len = V3Length(v);
	if (len != 0.0) {
		v->x *= newlen/len;   v->y *= newlen/len;  v->z *= newlen/len;
	}
	return(v);
}


/* return vector sum c = a+b */
Vector3 *V3Add(Vector3 *a, Vector3 *b, Vector3 *c)
{
	c->x = a->x+b->x;  c->y = a->y+b->y;  c->z = a->z+b->z;
	return(c);
}

/* return vector difference c = a-b */
Vector3 *V3Sub(Vector3 *a, Vector3 *b, Vector3 *c)   
{ 
	c->x = a->x-b->x;  c->y = a->y-b->y;  c->z = a->z-b->z;
	return(c);
}

/* return the dot product of vectors a and b */
s32 V3Dot(Vector3 *a, Vector3 *b) 
{
	return((a->x*b->x)+(a->y*b->y)+(a->z*b->z));
}
 
/* linearly interpolate between vectors by an amount alpha */
/* and return the resulting vector. */
/* When alpha=0, result=lo.  When alpha=1, result=hi. */
Vector3 *V3Lerp(Vector3 *lo, Vector3 *hi, s32 alpha, Vector3 *result) 
{
	result->x = LERP(alpha, lo->x, hi->x);
	result->y = LERP(alpha, lo->y, hi->y);
	result->z = LERP(alpha, lo->z, hi->z);
	return(result);
}

/* make a linear combination of two vectors and return the result. */
/* result = (a * ascl) + (b * bscl) */
Vector3 *V3Combine (Vector3 *a, Vector3 *b, Vector3 *result, s32 ascl, s32 bscl) 
{
	result->x = (ascl * a->x) + (bscl * b->x);
	result->y = (ascl * a->y) + (bscl * b->y);
	result->z = (ascl * a->z) + (bscl * b->z);
	return(result);
}


/* multiply two vectors together component-wise and return the result */
Vector3 *V3Mul (Vector3 *a, Vector3 *b, Vector3 *result) 
{
	result->x = a->x * b->x;
	result->y = a->y * b->y;
	result->z = a->z * b->z;
	return(result);
}

/* return the distance between two points */
s32 V3DistanceBetween2Points(Point3 *a, Point3 *b)
{
	s32 dx = a->x - b->x;
	s32 dy = a->y - b->y;
	s32 dz = a->z - b->z;
	return(fsqrt((dx*dx)+(dy*dy)+(dz*dz)));
}

/* return the distance between two points squared*/
s32 V3DistanceBetween2PointsSqr(Point3 *a, Point3 *b)
{
	s32 dx = a->x - b->x;
	s32 dy = a->y - b->y;
	s32 dz = a->z - b->z;
	return(((dx*dx)+(dy*dy)+(dz*dz)));
}

/* return the cross  product c = a cross b */
Vector3 *V3Cross(Vector3 *a, Vector3 *b, Vector3 *c)
{
	c->x = (a->y*b->z) - (a->z*b->y);
	c->y = (a->z*b->x) - (a->x*b->z);
	c->z = (a->x*b->y) - (a->y*b->x);
	return(c); 
}


/* create, initialize, and return a new vector */
Vector3 *V3New(s32 x, s32 y, s32 z)
{
	Vector3 *v;// = NEWTYPE(Vector3);
	//v->x = x;  v->y = y;  v->z = z;
	return(v);
}
/* create, initialize a new vector */
void V3Init(Vector3 *newVec, s32 x, s32 y, s32 z)
{
	newVec->x = x;
	newVec->y = y;
	newVec->z = z;	 
}

/* create, initialize, and return a duplicate vector */
Vector3 *V3Duplicate(Vector3 *a)
{
	Vector3 *v;// = NEWTYPE(Vector3);
	//v->x = a->x;  v->y = a->y;  v->z = a->z;
	return(v);
}

/* create, initialize, and copy a duplicate vector */
void V3Copy(Vector3 *src, Vector3 *dest)
{
	dest->x = src->x;  
	dest->y = src->y;  
	dest->z = src->z;	
}



/* multiply a point by a projective matrix and return the transformed point */
Point3 *V3MulPointByProjMatrix(Point3 *pin, Matrix4 *m, Point3 *pout)
{
	s32 w;
	pout->x = (pin->x * m->element[0][0]) + (pin->y * m->element[1][0]) + 
		(pin->z * m->element[2][0]) + m->element[3][0];
	pout->y = (pin->x * m->element[0][1]) + (pin->y * m->element[1][1]) + 
		(pin->z * m->element[2][1]) + m->element[3][1];
	pout->z = (pin->x * m->element[0][2]) + (pin->y * m->element[1][2]) + 
		(pin->z * m->element[2][2]) + m->element[3][2];
	w =    (pin->x * m->element[0][3]) + (pin->y * m->element[1][3]) + 
		(pin->z * m->element[2][3]) + m->element[3][3];
	if (w != 0.0) { pout->x /= w;  pout->y /= w;  pout->z /= w; }
	return(pout);
}

Plane *NewPlane(polygon_type *pPoly, Plane *newPlane)
{
	s32 A,B,C,D;
//	double pX,pY,pZ;
//	double qX,qY,qZ;
//	double nX,nY,nZ;

//	Vector3 polyNorm, polyPoint;
/*
	pX = pPoly->vertex[1]->wx - pPoly->vertex[0]->wx;
	pY = pPoly->vertex[1]->wy - pPoly->vertex[0]->wy;
	pZ = pPoly->vertex[1]->wz - pPoly->vertex[0]->wz;

	qX = pPoly->vertex[2]->wx - pPoly->vertex[0]->wx;
	qY = pPoly->vertex[2]->wy - pPoly->vertex[0]->wy;
	qZ = pPoly->vertex[2]->wz - pPoly->vertex[0]->wz;

	// Cross
	nX = (pY*qZ) - (pZ*qY);
	nY = (pZ*qX) - (pX*qZ);
	nZ = (pX*qY) - (pY*qX);

	// normalize
	s64 length = Sqrt(nX*nX + nY*nY + nZ*nZ);
	nX = nX/length;
	nY = nY/length;
	nZ = nZ/length;


	A = nX;//FXTOF(pPoly->anx);
	B = nY;//FXTOF(pPoly->any);
	C = nZ;//FXTOF(pPoly->anz);
*/
	
	A = (pPoly->anx);
	B = (pPoly->any);
	C = (pPoly->anz);

	//V3Init(&polyNorm, A, B, C);

	//V3Init(&polyPoint, pPoly->vertex[0]->ax, pPoly->vertex[0]->ay, pPoly->vertex[0]->az);
	
	D = -((A*pPoly->vertex[0]->wx)+(B*pPoly->vertex[0]->wy)+(C*pPoly->vertex[0]->wz));

	//D = -V3Dot(&polyNorm, &polyPoint);

	newPlane->A = A;
	newPlane->B = B;
	newPlane->C = C;
	newPlane->D = D;
}

s32 DistancePointToPlane(Plane *pPlane, Point3 *pPoint)
{
	s32 distance;

	distance = ((pPlane->A*pPoint->x)+(pPlane->B*pPoint->y)+
		(pPlane->C*pPoint->z))+pPlane->D;

	return distance>>8;
}