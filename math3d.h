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
#ifndef __MATH3DINC__
#define __MATH3DINC__

#include <Agb.h>
#include "HBDefines.h"
#include "Types3d.h"

/****************************
*  Function Prototypes      *
*****************************/
s32 point_in_poly(polygon_type *poly, s32 x, s32 y);

s32 Fast_Distance_3D(s32 x, s32 y, s32 z);

s64 bSphereTest(object_type *obj1, object_type *obj2, s32 rad1, s32 rad2);
 
/* returns squared length of input vector */	
s64 V3SquaredLength(Vector3 *a);

/* returns length of input vector */
s32 V3Length(Vector3 *a);

/* negates the input vector and returns it */
Vector3 *V3Negate(Vector3 *v);

/* normalizes the input vector and returns it */
Vector3 *V3Normalize(Vector3 *v);

/* scales the input vector to the new length and returns it */
Vector3 *V3Scale(Vector3 *v, s32 newlen);

/* return vector sum c = a+b */
Vector3 *V3Add(Vector3 *a, Vector3 *b, Vector3 *c);

/* return vector difference c = a-b */
Vector3 *V3Sub(Vector3 *a, Vector3 *b, Vector3 *c);

/* return the dot product of vectors a and b */
s32 V3Dot(Vector3 *a, Vector3 *b);

/* linearly interpolate between vectors by an amount alpha */
/* and return the resulting vector. */
/* When alpha=0, result=lo.  When alpha=1, result=hi. */
Vector3 *V3Lerp(Vector3 *lo, Vector3 *hi, s32 alpha, Vector3 *result);

/* make a linear combination of two vectors and return the result. */
/* result = (a * ascl) + (b * bscl) */
Vector3 *V3Combine (Vector3 *a, Vector3 *b, Vector3 *result, s32 ascl, s32 bscl);

/* multiply two vectors together component-wise and return the result */
Vector3 *V3Mul (Vector3 *a, Vector3 *b, Vector3 *result);

/* return the distance between two points */
s32 V3DistanceBetween2Points(Point3 *a, Point3 *b);
s32 V3DistanceBetween2PointsSqr(Point3 *a, Point3 *b);

/* return the cross product c = a cross b */
Vector3 *V3Cross(Vector3 *a, Vector3 *b, Vector3 *c);

/* create, initialize, and return a new vector */
Vector3 *V3New(s32 x, s32 y, s32 z);

/* create, initialize a new vector */
void V3Init(Vector3 *newVec, s32 x, s32 y, s32 z);

/* create, initialize, and return a duplicate vector */
Vector3 *V3Duplicate(Vector3 *a);

/* create, initialize, and copy a duplicate vector */
void V3Copy(Vector3 *src, Vector3 *dest);

/* multiply a point by a projective matrix and return the transformed point */
Point3 *V3MulPointByProjMatrix(Point3 *pin, Matrix4 *m, Point3 *pout);

s32 DistancePointToPlane(Plane *pPlane, Point3 *pPoint);

Plane *NewPlane(polygon_type *pPoly, Plane *newPlane);
#endif
