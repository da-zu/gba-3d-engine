#include <Agb.h>
#include "fix.h"
#include "Types3D.h"
#include "HBDefines.h"
#include "Trig.h"

extern u32 VideoBuffer;

// Global transformation arrays:
s32 matrix[4][4];          // Master transformation matrix

// Function prototype:

// Multiply two 4x4 matrices, result in RESULT:
void matmult(s32 result[4][4],s32 mat1[4][4],s32 mat2[4][4]);

// Copy one 4x4 matrix s32o another 4x4 matrix:
void matcopy(s32 dest[4][4],s32 source[4][4]);
/*
| Ux Vx Nx 0 |

| Uy Vy Ny 0 |

| Uz Vz Nz 0 | 
 
|  0  0  0 1 |
*/ 
void InitCamRotMatV3(Vector3 *v1, Vector3 *v2, Vector3 *v3, s32 result[4][4])
{
	result[0][0]=v1->x; result[0][1]=v2->x;   result[0][2]=v3->x;   result[0][3]=0;
	result[1][0]=v1->y;   result[1][1]=v2->y; result[1][2]=v3->y;   result[1][3]=0;
	result[2][0]=v1->z;   result[2][1]=v2->z;   result[2][2]=v3->z; result[2][3]=0;
	result[3][0]=0;   result[3][1]=0;   result[3][2]=0;   result[3][3]=ONE;
}
// Transformation functions:
void inittrans( void)
{ 

// Initialize master transformation matrix to the identity matrix

	matrix[0][0]=ONE; matrix[0][1]=0;   matrix[0][2]=0;   matrix[0][3]=0;
	matrix[1][0]=0;   matrix[1][1]=ONE; matrix[1][2]=0;   matrix[1][3]=0;
	matrix[2][0]=0;   matrix[2][1]=0;   matrix[2][2]=ONE; matrix[2][3]=0;
	matrix[3][0]=0;   matrix[3][1]=0;   matrix[3][2]=0;   matrix[3][3]=ONE;
}
void inittrans_boss( object_boss_type *object)
{

// Initialize master transformation matrix to the identity matrix

	object->matrix[0][0]=ONE; object->matrix[0][1]=0;   object->matrix[0][2]=0;   object->matrix[0][3]=0;
	object->matrix[1][0]=0;   object->matrix[1][1]=ONE; object->matrix[1][2]=0;   object->matrix[1][3]=0;
	object->matrix[2][0]=0;   object->matrix[2][1]=0;   object->matrix[2][2]=ONE; object->matrix[2][3]=0;
	object->matrix[3][0]=0;   object->matrix[3][1]=0;   object->matrix[3][2]=0;   object->matrix[3][3]=ONE;
}
void inittrans_view( view_type *view)
{

// Initialize master transformation matrix to the identity matrix

	view->matrix[0][0]=ONE; view->matrix[0][1]=0;   view->matrix[0][2]=0;   view->matrix[0][3]=0;
	view->matrix[1][0]=0;   view->matrix[1][1]=ONE; view->matrix[1][2]=0;   view->matrix[1][3]=0;
	view->matrix[2][0]=0;   view->matrix[2][1]=0;   view->matrix[2][2]=ONE; view->matrix[2][3]=0;
	view->matrix[3][0]=0;   view->matrix[3][1]=0;   view->matrix[3][2]=0;   view->matrix[3][3]=ONE;
}
void inittrans_char( CHAR_TYP *pChar)
{

// Initialize master transformation matrix to the identity matrix

	pChar->matrix[0][0]=ONE; pChar->matrix[0][1]=0;   pChar->matrix[0][2]=0;   pChar->matrix[0][3]=0;
	pChar->matrix[1][0]=0;   pChar->matrix[1][1]=ONE; pChar->matrix[1][2]=0;   pChar->matrix[1][3]=0;
	pChar->matrix[2][0]=0;   pChar->matrix[2][1]=0;   pChar->matrix[2][2]=ONE; pChar->matrix[2][3]=0;
	pChar->matrix[3][0]=0;   pChar->matrix[3][1]=0;   pChar->matrix[3][2]=0;   pChar->matrix[3][3]=ONE;
}
void scale( s32 xs, s32 ys, s32 zs)
{	
	s32 mat[4][4];
	s32 smat[4][4];				    // Scaling matrix
	
	// Initialize scaling matrix:
	smat[0][0] = (s32)xs; smat[0][1] = 0; smat[0][2] = 0; smat[0][3] = 0;
	smat[1][0] = 0; smat[1][1] = (s32)ys; smat[1][2] = 0; smat[1][3] = 0;
	smat[2][0] = 0; smat[2][1] = 0; smat[2][2] = (s32)zs; smat[2][3] = 0;
	smat[3][0] = 0; smat[3][1] = 0; smat[3][2] = 0;  smat[3][3] = ONE;

	// Concatenate with master matrix:
	matmult(matrix,smat,matrix);
	

}
void scale_boss( object_boss_type *object)
{
	s32 mat[4][4];
	s32 smat[4][4];				    // Scaling matrix
	s32 xs, ys, zs;

	xs = object->xscale;
	ys = object->yscale;
	zs = object->zscale;
	
	// Initialize scaling matrix:

	smat[0][0] = (s32)xs>>NUM_SHIFTS; smat[0][1] = 0; smat[0][2] = 0; smat[0][3] = 0;
	smat[1][0] = 0; smat[1][1] = (s32)ys>>NUM_SHIFTS; smat[1][2] = 0; smat[1][3] = 0;
	smat[2][0] = 0; smat[2][1] = 0; smat[2][2] = (s32)zs>>NUM_SHIFTS; smat[2][3] = 0;
	smat[3][0] = 0; smat[3][1] = 0; smat[3][2] = 0;  smat[3][3] = ONE;

  // Concatenate with master matrix:

	// Concatenate with master matrix:
	matmult(mat,smat,object->matrix);
	matcopy(object->matrix,mat);

}

void reflect( object_type *object)
{
/*	s32 mat[4][4];
	s32 rmat[4][4];					  // Perspective matrix


	rmat[0][0]=xr>>NUM_SHIFTS; rmat[0][1]=0; rmat[0][2]=0; rmat[0][3]=0;
	rmat[1][0]=0; rmat[1][1]=yr>>NUM_SHIFTS; rmat[1][2]=0; rmat[1][3]=0;
	rmat[2][0]=0; rmat[2][1]=0; rmat[2][2]=zr>>NUM_SHIFTS; rmat[2][3]=0;
	rmat[3][0]=0; rmat[3][1]=0; rmat[3][2]=0; 				rmat[3][3]=ONE;

	matmult(mat,object->matrix,rmat);
	matcopy(object->matrix,mat);
	*/
}

void rotate( s32 ax, s32 ay, s32 az)
{  
	//s32 zmat[4][4];				    // Z rotation matrix
	//s32 xmat[4][4];				    // X rotation matrix
	s32 rotmat[4][4];				    // Y rotation matrix
		// Create three rotation matrices that will rotate an object
	// AX radians on the X axis, AY radians on the Y axis and
	// AZ radians on the Z axis

	s32 mat1[4][4];
	s32 mat2[4][4];

	// Initialize Y rotation matrix:

	rotmat[0][0]=COS(ay); rotmat[0][1]=0;   rotmat[0][2]=-SIN(ay); rotmat[0][3]=0;
	rotmat[1][0]=0;       rotmat[1][1]=ONE; rotmat[1][2]=0;        rotmat[1][3]=0;
	rotmat[2][0]=SIN(ay); rotmat[2][1]=0;   rotmat[2][2]=COS(ay);  rotmat[2][3]=0;
	rotmat[3][0]=0;       rotmat[3][1]=0;   rotmat[3][2]=0;        rotmat[3][3]=ONE;

	// Concatenate this matrix with master matrix:
	matmult(mat1,matrix,rotmat);

	// Initialize X rotation matrix: 

	rotmat[0][0]=ONE;  rotmat[0][1]=0;        rotmat[0][2]=0;       rotmat[0][3]=0;
	rotmat[1][0]=0;    rotmat[1][1]=COS(ax);  rotmat[1][2]=SIN(ax); rotmat[1][3]=0;
	rotmat[2][0]=0;    rotmat[2][1]=-SIN(ax); rotmat[2][2]=COS(ax); rotmat[2][3]=0;
	rotmat[3][0]=0;    rotmat[3][1]=0;        rotmat[3][2]=0;       rotmat[3][3]=ONE;

	// Concatenate this matrix with master matrix:
	matmult(mat2,mat1,rotmat);

	// Initialize Z rotation matrix:

	rotmat[0][0]=COS(az);  rotmat[0][1]=SIN(az);  rotmat[0][2]=0;    rotmat[0][3]=0;
	rotmat[1][0]=-SIN(az); rotmat[1][1]=COS(az);  rotmat[1][2]=0;    rotmat[1][3]=0;
	rotmat[2][0]=0;        rotmat[2][1]=0;        rotmat[2][2]=ONE;  rotmat[2][3]=0;
	rotmat[3][0]=0;        rotmat[3][1]=0;        rotmat[3][2]=0;    rotmat[3][3]=ONE;

	// Concatenate this matrix with master matrix:	
	matmult(matrix,mat2,rotmat);
}
void rotate_boss( object_boss_type *object)
{

}
void rotate_char( CHAR_TYP *pChar)
{
}
void rotate_view( view_type *view)
{
	s32 xmat[4][4];
	s32 ymat[4][4];
	s32 zmat[4][4];
	// Create three rotation matrices that will rotate an object
	// AX radians on the X axis, AY radians on the Y axis and
	// AZ radians on the Z axis
	s32 mat1[4][4];
	s32 mat2[4][4];

	s32 ax, ay, az;

	ax = -view->xangle;
	ay = -view->yangle;
	az = -view->zangle;

	// Initialize Y rotation matrix:

	ymat[0][0]=COS(ay); ymat[0][1]=0;   ymat[0][2]=-SIN(ay); ymat[0][3]=0;
	ymat[1][0]=0;       ymat[1][1]=ONE; ymat[1][2]=0;        ymat[1][3]=0;
	ymat[2][0]=SIN(ay); ymat[2][1]=0;   ymat[2][2]=COS(ay);  ymat[2][3]=0;
	ymat[3][0]=0;       ymat[3][1]=0;   ymat[3][2]=0;        ymat[3][3]=ONE;

	// Concatenate this matrix with master matrix:


	// Initialize X rotation matrix:

	xmat[0][0]=ONE;  xmat[0][1]=0;        xmat[0][2]=0;       xmat[0][3]=0;
	xmat[1][0]=0;    xmat[1][1]=COS(ax);  xmat[1][2]=SIN(ax); xmat[1][3]=0;
	xmat[2][0]=0;    xmat[2][1]=-SIN(ax); xmat[2][2]=COS(ax); xmat[2][3]=0;
	xmat[3][0]=0;    xmat[3][1]=0;        xmat[3][2]=0;       xmat[3][3]=ONE;

	// Concatenate this matrix with master matrix:


	// Initialize Z rotation matrix:

	zmat[0][0]=COS(az);  zmat[0][1]=SIN(az);  zmat[0][2]=0;    zmat[0][3]=0;
	zmat[1][0]=-SIN(az); zmat[1][1]=COS(az);  zmat[1][2]=0;    zmat[1][3]=0;
	zmat[2][0]=0;        zmat[2][1]=0;        zmat[2][2]=ONE;  zmat[2][3]=0;
	zmat[3][0]=0;        zmat[3][1]=0;        zmat[3][2]=0;    zmat[3][3]=ONE;

	// Concatenate this matrix with master matrix:

	matmult(mat1,view->matrix,ymat);
	matmult(mat2,mat1,xmat);
	matmult(view->matrix,mat2,zmat);
}

void translate( s32 xt, s32 yt, s32 zt)
{

// Create a translation matrix that will translate an object an
// X distance of XT, a Y distance of YT, and a Z distance of ZT
// from the screen origin
  s32 mat[4][4];
  s32 tmat[4][4];            // Translation matrix


	tmat[0][0]=ONE;      tmat[0][1]=0;      tmat[0][2]=0;      tmat[0][3]=0;
	tmat[1][0]=0;        tmat[1][1]=ONE;    tmat[1][2]=0;      tmat[1][3]=0;
  tmat[2][0]=0;        tmat[2][1]=0;      tmat[2][2]=ONE;    tmat[2][3]=0;
	tmat[3][0]=(s32)xt<<NUM_SHIFTS; tmat[3][1]=(s32)yt<<NUM_SHIFTS; tmat[3][2]=(s32)zt<<NUM_SHIFTS; tmat[3][3]=ONE;

	// Concatenate with master matrix:

	matmult(matrix,matrix,tmat);
	//matcopy(matrix,mat);
   
}
void translate_boss( object_boss_type *object)
{

// Create a translation matrix that will translate an object an
// X distance of XT, a Y distance of YT, and a Z distance of ZT
// from the screen origin
  s32 mat[4][4];
  s32 tmat[4][4];            // Translation matrix
  s32 xt, yt, zt;

  xt = object->x;
  yt = object->y;
  zt = object->z;


	tmat[0][0]=ONE;      tmat[0][1]=0;      tmat[0][2]=0;      tmat[0][3]=0;
	tmat[1][0]=0;        tmat[1][1]=ONE;    tmat[1][2]=0;      tmat[1][3]=0;
  tmat[2][0]=0;        tmat[2][1]=0;      tmat[2][2]=ONE;    tmat[2][3]=0;
	tmat[3][0]=(s32)xt<<NUM_SHIFTS; tmat[3][1]=(s32)yt<<NUM_SHIFTS; tmat[3][2]=(s32)zt<<NUM_SHIFTS; tmat[3][3]=ONE;

	// Concatenate with master matrix:

	matmult(mat,object->matrix,tmat);
	matcopy(object->matrix,mat);
   
}
void translate_view( view_type *view)
{


// Create a translation matrix that will translate an object an
// X distance of XT, a Y distance of YT, and a Z distance of ZT
// from the screen origin
  s32 mat[4][4];
  s32 view_tmat[4][4];
  s32 xt, yt, zt;

  xt = -view->copx;
  yt = -view->copy;
  zt = -view->copz;
	
	view_tmat[0][0]=ONE;      view_tmat[0][1]=0;      view_tmat[0][2]=0;      view_tmat[0][3]=0;
	view_tmat[1][0]=0;        view_tmat[1][1]=ONE;    view_tmat[1][2]=0;      view_tmat[1][3]=0;
  view_tmat[2][0]=0;        view_tmat[2][1]=0;      view_tmat[2][2]=ONE;    view_tmat[2][3]=0;
	view_tmat[3][0]=(s32)xt<<NUM_SHIFTS; view_tmat[3][1]=(s32)yt<<NUM_SHIFTS; view_tmat[3][2]=(s32)zt<<NUM_SHIFTS; view_tmat[3][3]=ONE;

	// Concatenate with master matrix:

	matmult(mat,view->matrix,view_tmat);
	matcopy(view->matrix,mat);  
}
void translate_view_zero( view_type *view)
{


// Create a translation matrix that will translate an object an
// X distance of XT, a Y distance of YT, and a Z distance of ZT
// from the screen origin
  s32 mat[4][4];
  s32 view_tmat[4][4];
  s32 xt, yt, zt;

  xt = 0;
  yt = 0;
  zt = 0;
	
	view_tmat[0][0]=ONE;      view_tmat[0][1]=0;      view_tmat[0][2]=0;      view_tmat[0][3]=0;
	view_tmat[1][0]=0;        view_tmat[1][1]=ONE;    view_tmat[1][2]=0;      view_tmat[1][3]=0;
  view_tmat[2][0]=0;        view_tmat[2][1]=0;      view_tmat[2][2]=ONE;    view_tmat[2][3]=0;
	view_tmat[3][0]=(s32)xt<<NUM_SHIFTS; view_tmat[3][1]=(s32)yt<<NUM_SHIFTS; view_tmat[3][2]=(s32)zt<<NUM_SHIFTS; view_tmat[3][3]=ONE;

	// Concatenate with master matrix:

	matmult(mat,view->matrix,view_tmat);
	matcopy(view->matrix,mat);
}
void translate_char( CHAR_TYP *pChar)
{

}
void transformNormals( object_type *object)
{ 
	u32 v, numpolys;
	s64 temp;
 
   numpolys = (*object).number_of_polygons;  
  
   for ( v=0; v<numpolys; v++) 
   { 
	   temp = (object->polygon[v].nx*matrix[0][0]+object->polygon[v].ny*matrix[1][0]
			+object->polygon[v].nz*matrix[2][0]+matrix[3][0]);
   
	   object->polygon[v].anx = temp>>8 ;
  
	   temp = (object->polygon[v].nx*matrix[0][1]+object->polygon[v].ny*matrix[1][1]
			+object->polygon[v].nz*matrix[2][1]+matrix[3][1]);

	   object->polygon[v].any = temp>>8;
 
	   temp = (object->polygon[v].nx*matrix[0][2]+object->polygon[v].ny*matrix[1][2]
			+object->polygon[v].nz*matrix[2][2]+matrix[3][2]);
 
	   object->polygon[v].anz = temp>>8;  
   } 
}   
 
void transform( object_type *object)      
{
  // Multiply all vertices in OBJECT with master transformation matrix :
	
	u32 v, numverts;
	s64 temp; 
  
   numverts = (*object).number_of_vertices; 
  
   for ( v=0; v<numverts; v++) {
    //vertex_type *vptr=&object->vertex[v].; 
	
	  temp = (s64)(object->vertex[v].lx*matrix[0][0]+object->vertex[v].ly*matrix[1][0]
	 +object->vertex[v].lz*matrix[2][0]+matrix[3][0]);

    object->vertex[v].wx=temp>>NUM_SHIFTS; 
    
	temp = (s64)(object->vertex[v].lx*matrix[0][1]+object->vertex[v].ly*matrix[1][1]
	 +object->vertex[v].lz*matrix[2][1]+matrix[3][1]);
 
	object->vertex[v].wy=temp>>NUM_SHIFTS;
    
	temp = (s64)(object->vertex[v].lx*matrix[0][2]+object->vertex[v].ly*matrix[1][2]
	 +object->vertex[v].lz*matrix[2][2]+matrix[3][2]);

	object->vertex[v].wz=temp >>NUM_SHIFTS;
   } 
}
void transform_view(view_type *pv)  
{
  // Multiply all vertices in OBJECT with master transformation matrix :
	 
	matmult(pv->matrix,pv->matrix,matrix);
  
}
void transformVec3(s32 x, s32 y, s32 z, s32 *nx, s32 *ny, s32 *nz)  
{
  // Multiply all vertices in OBJECT with master transformation matrix :
	 
	u32 v, numverts;  
	s64 temp;  
   
  	        
	  temp = (s64)(x*matrix[0][0]+y*matrix[1][0]
	 +z*matrix[2][0]+matrix[3][0]);  
    *nx=temp>>NUM_SHIFTS; 
     
	temp = (s64)(x*matrix[0][1]+y*matrix[1][1]   
	 +z*matrix[2][1]+matrix[3][1]);   
   
	*ny=temp>>NUM_SHIFTS;  
    
	temp = (s64)(x*matrix[0][2]+y*matrix[1][2] 
	 +z*matrix[2][2]+matrix[3][2]);

	*nz=temp >>NUM_SHIFTS;
  
}
void transform_boss( object_boss_type *object) 
{ 

}
void transform_object( object_type *object)
{
  // Multiply all vertices in OBJECT with master transformation matrix:
	
    object->wx = ((s32)object->x*(s32)matrix[0][0]+(s32)object->y*(s32)matrix[1][0]
	 +(s32)object->z*(s32)matrix[2][0]+(s32)matrix[3][0])>>NUM_SHIFTS;
    
	object->wy = ((s32)object->x*(s32)matrix[0][1]+(s32)object->y*(s32)matrix[1][1]
	 +(s32)object->z*(s32)matrix[2][1]+(s32)matrix[3][1])>>NUM_SHIFTS;
    
	object->wz = ((s32)object->x*(s32)matrix[0][2]+(s32)object->y*(s32)matrix[1][2]
	 +(s32)object->z*(s32)matrix[2][2]+(s32)matrix[3][2])>>NUM_SHIFTS;
 
}
void transform_char( CHAR_TYP *pChar)
{
  
} 


void atransform( object_type *object)

// Multiply all vertices in OBJECT with master
//  transformation matrix:

{
	/*
	u32 v;
	u32 numverts;
	//vertex_type *vptr;
  // Step through all vertices of OBJECT:
	numverts = (*object).number_of_vertices;
  
   for ( v=0; v<numverts; v++) {

  // Create pos32er to vertex:

  vertex_type *vptr=&(*object).vertex[v];

  // Calculate new aligned x coordinate:

  vptr->ax=((s32)vptr->wx*matrix[0][0]
    +(s32)vptr->wy*matrix[1][0]
    +(s32)vptr->wz*matrix[2][0]
    +matrix[3][0])>>NUM_SHIFTS;

  // Calculate new aligned y coordinate:

  vptr->ay=((s32)vptr->wx*matrix[0][1]
    +(s32)vptr->wy*matrix[1][1]
    +(s32)vptr->wz*matrix[2][1]
    +matrix[3][1])>>NUM_SHIFTS;

  // Calculate new aligned z coordinate:

  vptr->az=((s32)vptr->wx*matrix[0][2]
    +(s32)vptr->wy*matrix[1][2]
    +(s32)vptr->wz*matrix[2][2]
    +matrix[3][2])>>NUM_SHIFTS;
  }
   */
}

void matmult(s32 result[4][4],s32 mat1[4][4],s32 mat2[4][4])
{

	u32 i, j;
  // Multiply matrix MAT1 by matrix MAT2, returning the result in RESULT

  for ( i=0; i<4; i++)
    for ( j=0; j<4; j++) {
      result[i][j]=(
		  (mat1[i][0]*mat2[0][j])
	   +(mat1[i][1]*mat2[1][j])
	   +(mat1[i][2]*mat2[2][j])
	   +(mat1[i][3]*mat2[3][j]))>>NUM_SHIFTS;
    }
}

void matcopy(s32 dest[4][4],s32 source[4][4])
{

// Copy matrix SOURCE to matrix DEST
u32 i, j;
	for ( i=0; i<4; i++)
		for ( j=0; j<4; j++)
			dest[i][j]=source[i][j];
} 

void cammatrix(Vector3 *up, Vector3 *right, Vector3 *forward, view_type *pView)
{
	s32 rot[4][4];
	s32 trans[4][4];
	Vector3 campos; 

	V3Init(&campos, pView->copx, pView->copy, pView->copz);


	rot[0][0]=-(right->x);		rot[0][1]=(up->x);		rot[0][2]=(forward->x);	rot[0][3]=0;
	rot[1][0]=-(right->y);		rot[1][1]=(up->y);		rot[1][2]=(forward->y);	rot[1][3]=0;
	rot[2][0]=-(right->z);		rot[2][1]=(up->z);		rot[2][2]=(forward->z);	rot[2][3]=0;
	rot[3][0]=0;			rot[3][1]=0;			rot[3][2]=0;			rot[3][3]=ONE;

	
	trans[0][0]=ONE;		trans[0][1]=0;		trans[0][2]=0;		trans[0][3]=0;
	trans[1][0]=0;		trans[1][1]=ONE;		trans[1][2]=0;		trans[1][3]=0;
	trans[2][0]=0;		trans[2][1]=0;		trans[2][2]=ONE;		trans[2][3]=0;
	trans[3][0]=-pView->copx<<NUM_SHIFTS;		trans[3][1]=-pView->copy<<NUM_SHIFTS;		trans[3][2]=-pView->copz<<NUM_SHIFTS;		trans[3][3]=ONE;

	matmult(pView->matrix, trans, rot); 
}
void camshit(Vector3 *pLookAt, view_type *pView, s32 roll)
{ 
     
	Vector3 forward, up, right; 
   
	up.x = SIN(roll);  
	up.y = -COS(roll); 
	up.z = 0;
     
   
	forward.x = (pLookAt->x - pView->copx)<<8;
	forward.y = (pLookAt->y - pView->copy)<<8;   
	forward.z = (pLookAt->z - pView->copz)<<8;
	 
	 

	V3Normalize(&forward); 

		
	V3Cross(&up, &forward,&right);
	
	
	V3Normalize(&right);

	V3Cross(&right, &forward, &up); 
 	V3Normalize(&up);

	cammatrix(&up,&right,&forward, pView);


}

void generateCamMatrix( Vector3 *pVector, float rollDegrees, s32 result[4][4] )
{
	Vector3 xAxis;
	Vector3 yAxis;
	Vector3 zAxis;
	s32 rot[4][4];  
 
	// Get our direction vector (the Z vector component of the matrix)
	// and make sure it's normalized into a unit vector
	
	//cVector zAxis(vector);
	V3Copy(pVector, &zAxis);
	//zAxis.normalize(); 
	V3Normalize(&zAxis);
	
	// Build the Y vector of the matrix (handle the degenerate case
	// in the way that 3DS does) -- This is not the TRUE vector, only
	// a reference vector.
		
	if (!zAxis.x && !zAxis.z)
	{
		//yAxis = cVector(-zAxis.dy, 0.0f, 0.0f );
		V3Init(&yAxis,-zAxis.y, 0, 0 );
	}
	else
	{
		//yAxis = cVector(0.0f, 1.0f, 0.0f);
		V3Init(&yAxis,0, 1<<8, 0 );
	}
	
	// Build the X axis vector based on the two existing vectors
	
	//cVector xAxis = yAxis.cross( zAxis );
	V3Cross(&yAxis, &zAxis, &xAxis);
	//xAxis.normalize();
	V3Normalize(&xAxis);
	
	// Correct the Y reference vector
	
	//yAxis = xAxis.cross( zAxis );
	V3Cross(&xAxis, &zAxis, &yAxis);
	//yAxis.normalize();
	V3Normalize(&yAxis);
	//yAxis = -yAxis;
	V3Negate(&yAxis);
	
	// Generate rotation matrix without roll included
	
	//cMatrix rot(xAxis, yAxis, zAxis);
	InitCamRotMatV3(&xAxis, &yAxis, &zAxis, rot);
	
	// Generate the Z rotation matrix for roll (bank)
	
	//cMatrix roll(MATRIX_Z, rollDegrees); 
	   
	// Concatinate them for a complete rotation matrix that includes
	// all rotations
	
	//cMatrix result = roll * rot;
	matmult(result,result,rot);
	//matcopy(result, rot);

	// All done
}
