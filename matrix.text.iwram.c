#include <Agb.h>
#include "fix.h"
#include "Types3D.h"
#include "HBDefines.h"
#include "Trig.h"


// Global transformation arrays:
//s32 matrix[4][4];          // Master transformation matrix

// Function prototype:

// Multiply two 4x4 matrices, result in RESULT:
void matmult(s32 result[4][4],s32 mat1[4][4],s32 mat2[4][4]);

// Copy one 4x4 matrix s32o another 4x4 matrix:
void matcopy(s32 dest[4][4],s32 source[4][4]);

// Transformation functions:
void inittrans( object_type *object)
{

// Initialize master transformation matrix to the identity matrix

	object->matrix[0][0]=ONE; object->matrix[0][1]=0;   object->matrix[0][2]=0;   object->matrix[0][3]=0;
	object->matrix[1][0]=0;   object->matrix[1][1]=ONE; object->matrix[1][2]=0;   object->matrix[1][3]=0;
	object->matrix[2][0]=0;   object->matrix[2][1]=0;   object->matrix[2][2]=ONE; object->matrix[2][3]=0;
	object->matrix[3][0]=0;   object->matrix[3][1]=0;   object->matrix[3][2]=0;   object->matrix[3][3]=ONE;
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
void scale( object_type *object)
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

void rotate( object_type *object)
{
	s32 zmat[4][4];				    // Z rotation matrix
	s32 xmat[4][4];				    // X rotation matrix
	s32 ymat[4][4];				    // Y rotation matrix
	// Create three rotation matrices that will rotate an object
	// AX radians on the X axis, AY radians on the Y axis and
	// AZ radians on the Z axis
	s32 mat1[4][4];
	s32 mat2[4][4];

	s32 ax, ay, az;

	ax = object->xangle;
	ay = object->yangle;
	az = object->zangle;

	// Initialize X rotation matrix: 
	xmat[0][0]=ONE;  xmat[0][1]=0;        xmat[0][2]=0;       xmat[0][3]=0;
	xmat[1][0]=0;    xmat[1][1]=COS(ax);  xmat[1][2]=SIN(ax); xmat[1][3]=0;
	xmat[2][0]=0;    xmat[2][1]=-SIN(ax); xmat[2][2]=COS(ax); xmat[2][3]=0;
	xmat[3][0]=0;    xmat[3][1]=0;        xmat[3][2]=0;       xmat[3][3]=ONE;

	matmult(mat1,xmat,object->matrix);

	// Initialize Y rotation matrix:
	ymat[0][0]=COS(ay); ymat[0][1]=0;   ymat[0][2]=-SIN(ay); ymat[0][3]=0;
	ymat[1][0]=0;       ymat[1][1]=ONE; ymat[1][2]=0;        ymat[1][3]=0;
	ymat[2][0]=SIN(ay); ymat[2][1]=0;   ymat[2][2]=COS(ay);  ymat[2][3]=0;
	ymat[3][0]=0;       ymat[3][1]=0;   ymat[3][2]=0;        ymat[3][3]=ONE;

	matmult(mat2,ymat,mat1);
		
	// Concatenate this matrix with master matrix:
	// Initialize Z rotation matrix:
	zmat[0][0]=COS(az);  zmat[0][1]=SIN(az);  zmat[0][2]=0;    zmat[0][3]=0;
	zmat[1][0]=-SIN(az); zmat[1][1]=COS(az);  zmat[1][2]=0;    zmat[1][3]=0;
	zmat[2][0]=0;        zmat[2][1]=0;        zmat[2][2]=ONE;  zmat[2][3]=0;
	zmat[3][0]=0;        zmat[3][1]=0;        zmat[3][2]=0;    zmat[3][3]=ONE;

	// Concatenate this matrix with master matrix:
	matmult(object->matrix,zmat,mat2);
}
void rotate_boss( object_boss_type *object)
{
	s32 zmat[4][4];				    // Z rotation matrix
	s32 xmat[4][4];				    // X rotation matrix
	s32 ymat[4][4];				    // Y rotation matrix
	// Create three rotation matrices that will rotate an object
	// AX radians on the X axis, AY radians on the Y axis and
	// AZ radians on the Z axis
	s32 mat1[4][4];
	s32 mat2[4][4];

	s32 ax, ay, az;

	ax = object->xangle;
	ay = object->yangle;
	az = object->zangle;

	// Initialize X rotation matrix: 
	xmat[0][0]=ONE;  xmat[0][1]=0;        xmat[0][2]=0;       xmat[0][3]=0;
	xmat[1][0]=0;    xmat[1][1]=COS(ax);  xmat[1][2]=SIN(ax); xmat[1][3]=0;
	xmat[2][0]=0;    xmat[2][1]=-SIN(ax); xmat[2][2]=COS(ax); xmat[2][3]=0;
	xmat[3][0]=0;    xmat[3][1]=0;        xmat[3][2]=0;       xmat[3][3]=ONE;

	matmult(mat1,xmat,object->matrix);

	// Initialize Y rotation matrix:
	ymat[0][0]=COS(ay); ymat[0][1]=0;   ymat[0][2]=-SIN(ay); ymat[0][3]=0;
	ymat[1][0]=0;       ymat[1][1]=ONE; ymat[1][2]=0;        ymat[1][3]=0;
	ymat[2][0]=SIN(ay); ymat[2][1]=0;   ymat[2][2]=COS(ay);  ymat[2][3]=0;
	ymat[3][0]=0;       ymat[3][1]=0;   ymat[3][2]=0;        ymat[3][3]=ONE;

	matmult(mat2,ymat,mat1);
		
	// Concatenate this matrix with master matrix:
	// Initialize Z rotation matrix:
	zmat[0][0]=COS(az);  zmat[0][1]=SIN(az);  zmat[0][2]=0;    zmat[0][3]=0;
	zmat[1][0]=-SIN(az); zmat[1][1]=COS(az);  zmat[1][2]=0;    zmat[1][3]=0;
	zmat[2][0]=0;        zmat[2][1]=0;        zmat[2][2]=ONE;  zmat[2][3]=0;
	zmat[3][0]=0;        zmat[3][1]=0;        zmat[3][2]=0;    zmat[3][3]=ONE;

	// Concatenate this matrix with master matrix:
	matmult(object->matrix,zmat,mat2);
}
void rotate_char( CHAR_TYP *pChar)
{
	/*
	s32 zmat[4][4];				    // Z rotation matrix
	s32 xmat[4][4];				    // X rotation matrix
	s32 ymat[4][4];				    // Y rotation matrix
	// Create three rotation matrices that will rotate an object
	// AX radians on the X axis, AY radians on the Y axis and
	// AZ radians on the Z axis
	s32 mat1[4][4];
	s32 mat2[4][4];

	s32 ax, ay, az;

	ax = pChar->anglexangle;
	ay = object->yangle;
	az = object->zangle;

	// Initialize X rotation matrix: 
	xmat[0][0]=ONE;  xmat[0][1]=0;        xmat[0][2]=0;       xmat[0][3]=0;
	xmat[1][0]=0;    xmat[1][1]=COS(ax);  xmat[1][2]=SIN(ax); xmat[1][3]=0;
	xmat[2][0]=0;    xmat[2][1]=-SIN(ax); xmat[2][2]=COS(ax); xmat[2][3]=0;
	xmat[3][0]=0;    xmat[3][1]=0;        xmat[3][2]=0;       xmat[3][3]=ONE;

	matmult(mat1,xmat,object->matrix);

	// Initialize Y rotation matrix:
	ymat[0][0]=COS(ay); ymat[0][1]=0;   ymat[0][2]=-SIN(ay); ymat[0][3]=0;
	ymat[1][0]=0;       ymat[1][1]=ONE; ymat[1][2]=0;        ymat[1][3]=0;
	ymat[2][0]=SIN(ay); ymat[2][1]=0;   ymat[2][2]=COS(ay);  ymat[2][3]=0;
	ymat[3][0]=0;       ymat[3][1]=0;   ymat[3][2]=0;        ymat[3][3]=ONE;

	matmult(mat2,ymat,mat1);
		
	// Concatenate this matrix with master matrix:
	// Initialize Z rotation matrix:
	zmat[0][0]=COS(az);  zmat[0][1]=SIN(az);  zmat[0][2]=0;    zmat[0][3]=0;
	zmat[1][0]=-SIN(az); zmat[1][1]=COS(az);  zmat[1][2]=0;    zmat[1][3]=0;
	zmat[2][0]=0;        zmat[2][1]=0;        zmat[2][2]=ONE;  zmat[2][3]=0;
	zmat[3][0]=0;        zmat[3][1]=0;        zmat[3][2]=0;    zmat[3][3]=ONE;

	// Concatenate this matrix with master matrix:
	matmult(object->matrix,zmat,mat2);
	*/
}
void rotate_view( view_type *view)
{
	s32 view_xmat[4][4];
	s32 view_ymat[4][4];
	s32 view_zmat[4][4];
	// Create three rotation matrices that will rotate an object
	// AX radians on the X axis, AY radians on the Y axis and
	// AZ radians on the Z axis
	s32 mat1[4][4];
	s32 mat2[4][4];

	s32 ax, ay, az;

	ax = -view->xangle;
	ay = -view->yangle;
	az = -view->zangle;

	ax = ABS(ax);// & 0xFFFE;
	ay = ABS(ay);// & 0xFFFE;
	az = ABS(az);// & 0xFFFE;

	// Initialize Y rotation matrix:

	view_ymat[0][0]=COS(ay); view_ymat[0][1]=0;   view_ymat[0][2]=-SIN(ay); view_ymat[0][3]=0;
	view_ymat[1][0]=0;       view_ymat[1][1]=ONE; view_ymat[1][2]=0;        view_ymat[1][3]=0;
	view_ymat[2][0]=SIN(ay); view_ymat[2][1]=0;   view_ymat[2][2]=COS(ay);  view_ymat[2][3]=0;
	view_ymat[3][0]=0;       view_ymat[3][1]=0;   view_ymat[3][2]=0;        view_ymat[3][3]=ONE;

	// Concatenate this matrix with master matrix:


	// Initialize X rotation matrix:

	view_xmat[0][0]=ONE;  view_xmat[0][1]=0;        view_xmat[0][2]=0;       view_xmat[0][3]=0;
	view_xmat[1][0]=0;    view_xmat[1][1]=COS(ax);  view_xmat[1][2]=SIN(ax); view_xmat[1][3]=0;
	view_xmat[2][0]=0;    view_xmat[2][1]=-SIN(ax); view_xmat[2][2]=COS(ax); view_xmat[2][3]=0;
	view_xmat[3][0]=0;    view_xmat[3][1]=0;        view_xmat[3][2]=0;       view_xmat[3][3]=ONE;

	// Concatenate this matrix with master matrix:


	// Initialize Z rotation matrix:

	view_zmat[0][0]=COS(az);  view_zmat[0][1]=SIN(az);  view_zmat[0][2]=0;    view_zmat[0][3]=0;
	view_zmat[1][0]=-SIN(az); view_zmat[1][1]=COS(az);  view_zmat[1][2]=0;    view_zmat[1][3]=0;
	view_zmat[2][0]=0;        view_zmat[2][1]=0;        view_zmat[2][2]=ONE;  view_zmat[2][3]=0;
	view_zmat[3][0]=0;        view_zmat[3][1]=0;        view_zmat[3][2]=0;    view_zmat[3][3]=ONE;

	// Concatenate this matrix with master matrix:

	matmult(mat1,view->matrix,view_ymat);
	matmult(mat2,mat1,view_xmat);
	matmult(view->matrix,mat2,view_zmat);
}

void translate( object_type *object)
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
	tmat[3][0]=(s32)xt<<NUM_SHIFTS; tmat[3][1]=(s32)yt<<NUM_SHIFTS; tmat[3][2]=(s32)zt*NUM_SHIFTS_MULT; tmat[3][3]=ONE;

	// Concatenate with master matrix:

	matmult(mat,object->matrix,tmat);
	matcopy(object->matrix,mat);
   
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
	tmat[3][0]=(s32)xt<<NUM_SHIFTS; tmat[3][1]=(s32)yt<<NUM_SHIFTS; tmat[3][2]=(s32)zt*NUM_SHIFTS_MULT; tmat[3][3]=ONE;

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
	view_tmat[3][0]=(s32)xt<<NUM_SHIFTS; view_tmat[3][1]=(s32)yt<<NUM_SHIFTS; view_tmat[3][2]=(s32)zt*NUM_SHIFTS_MULT; view_tmat[3][3]=ONE;

	// Concatenate with master matrix:

	matmult(mat,view->matrix,view_tmat);
	matcopy(view->matrix,mat);
}
void translate_char( CHAR_TYP *pChar)
{

// Create a translation matrix that will translate an object an
// X distance of XT, a Y distance of YT, and a Z distance of ZT
// from the screen origin
  s32 mat[4][4];
  s32 tmat[4][4];            // Translation matrix
  s32 xt, yt, zt;

  xt = pChar->x;
  yt = pChar->y;
  zt = pChar->z;


	tmat[0][0]=ONE;      tmat[0][1]=0;      tmat[0][2]=0;      tmat[0][3]=0;
	tmat[1][0]=0;        tmat[1][1]=ONE;    tmat[1][2]=0;      tmat[1][3]=0;
  tmat[2][0]=0;        tmat[2][1]=0;      tmat[2][2]=ONE;    tmat[2][3]=0;
	tmat[3][0]=(s32)xt<<NUM_SHIFTS; tmat[3][1]=(s32)yt<<NUM_SHIFTS; tmat[3][2]=(s32)zt*NUM_SHIFTS_MULT; tmat[3][3]=ONE;

	// Concatenate with master matrix:

	matmult(mat,pChar->matrix,tmat);
	matcopy(pChar->matrix,mat);
   
}
void transform( object_type *object)
{
  // Multiply all vertices in OBJECT with master transformation matrix:
	
	u32 v, numverts;

   numverts = (*object).number_of_vertices;

  for ( v=0; v<numverts; v++) {
    //vertex_type *vptr=&object->vertex[v].;

    object->vertex[v].wx=((s32)object->vertex[v].lx*(s32)object->matrix[0][0]+(s32)object->vertex[v].ly*(s32)object->matrix[1][0]
	 +(s32)object->vertex[v].lz*(s32)object->matrix[2][0]+(s32)object->matrix[3][0])>>NUM_SHIFTS;
    
	object->vertex[v].wy=((s32)object->vertex[v].lx*(s32)object->matrix[0][1]+(s32)object->vertex[v].ly*(s32)object->matrix[1][1]
	 +(s32)object->vertex[v].lz*(s32)object->matrix[2][1]+(s32)object->matrix[3][1])>>NUM_SHIFTS;
    
	object->vertex[v].wz=((s32)object->vertex[v].lx*(s32)object->matrix[0][2]+(s32)object->vertex[v].ly*(s32)object->matrix[1][2]
	 +(s32)object->vertex[v].lz*(s32)object->matrix[2][2]+(s32)object->matrix[3][2])>>NUM_SHIFTS;
  }
}
void transform_boss( object_boss_type *object)
{
  // Multiply all vertices in OBJECT with master transformation matrix:
	
	u32 v, numverts;

   numverts = (*object).number_of_vertices;

  for ( v=0; v<numverts; v++) {
    //vertex_type *vptr=&object->vertex[v].;

    object->vertex[v].wx=((s32)object->vertex[v].lx*(s32)object->matrix[0][0]+(s32)object->vertex[v].ly*(s32)object->matrix[1][0]
	 +(s32)object->vertex[v].lz*(s32)object->matrix[2][0]+(s32)object->matrix[3][0])>>NUM_SHIFTS;
    
	object->vertex[v].wy=((s32)object->vertex[v].lx*(s32)object->matrix[0][1]+(s32)object->vertex[v].ly*(s32)object->matrix[1][1]
	 +(s32)object->vertex[v].lz*(s32)object->matrix[2][1]+(s32)object->matrix[3][1])>>NUM_SHIFTS;
    
	object->vertex[v].wz=((s32)object->vertex[v].lx*(s32)object->matrix[0][2]+(s32)object->vertex[v].ly*(s32)object->matrix[1][2]
	 +(s32)object->vertex[v].lz*(s32)object->matrix[2][2]+(s32)object->matrix[3][2])>>NUM_SHIFTS;
  }
}
void transform_object( object_type *object)
{
  // Multiply all vertices in OBJECT with master transformation matrix:
	
    object->wx = ((s32)object->x*(s32)object->matrix[0][0]+(s32)object->y*(s32)object->matrix[1][0]
	 +(s32)object->z*(s32)object->matrix[2][0]+(s32)object->matrix[3][0])>>NUM_SHIFTS;
    
	object->wy = ((s32)object->x*(s32)object->matrix[0][1]+(s32)object->y*(s32)object->matrix[1][1]
	 +(s32)object->z*(s32)object->matrix[2][1]+(s32)object->matrix[3][1])>>NUM_SHIFTS;
    
	object->wz = ((s32)object->x*(s32)object->matrix[0][2]+(s32)object->y*(s32)object->matrix[1][2]
	 +(s32)object->z*(s32)object->matrix[2][2]+(s32)object->matrix[3][2])>>NUM_SHIFTS;
 
}
void transform_char( CHAR_TYP *pChar)
{
  // Multiply all vertices in OBJECT with master transformation matrix:
	  
    //vertex_type *vptr=&object->vertex[v].;

    pChar->wx=((s32)pChar->lx*(s32)pChar->matrix[0][0]+(s32)pChar->ly*(s32)pChar->matrix[1][0]
	 +(s32)pChar->lz*(s32)pChar->matrix[2][0]+(s32)pChar->matrix[3][0])>>NUM_SHIFTS;
    
	pChar->wy=((s32)pChar->lx*(s32)pChar->matrix[0][1]+(s32)pChar->ly*(s32)pChar->matrix[1][1]
	 +(s32)pChar->lz*(s32)pChar->matrix[2][1]+(s32)pChar->matrix[3][1])>>NUM_SHIFTS;
    
	pChar->wz=((s32)pChar->lx*(s32)pChar->matrix[0][2]+(s32)pChar->ly*(s32)pChar->matrix[1][2]
	 +(s32)pChar->lz*(s32)pChar->matrix[2][2]+(s32)pChar->matrix[3][2])>>NUM_SHIFTS;
  
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
      result[i][j]=((mat1[i][0]*mat2[0][j])
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

