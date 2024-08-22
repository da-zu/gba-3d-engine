
#ifndef _3DTYPESINC_
#define _3DTYPESINC_

#include <Agb.h>
#include "HBDefines.h"
#include "HBTypes.h"

#define MAX_NUM_SCRIPT			20

typedef struct planestruct
{
	s32 A;
	s32 B;
	s32 C;
	s32 D;
}Plane;

typedef struct vertex2struct
{
    s32 x,y;    // screen coordinates (integers)

    s32 u,v;    // vertex u,v (26:6 bit fixed point)

}vertexTMap;

typedef struct dirLightStruct
{
	s32 light_ax,light_ay,light_az;
	s32 light_nx,light_ny,light_nz;
	s32 light_x,light_y,light_z;

} DirectionalLight;

typedef struct Point3Struct 
{	
	/* 3d point */
	s32 x, y, z;

} Point3;

typedef Point3 Vector3;

typedef struct Matrix4Struct 
{	
	/* 4-by-4 matrix */
	s32 element[4][4];

} Matrix4;


// structure for vertices
typedef struct 			
{
	// aligned with view coordinates of vertex
	s16 ax,ay,az;

	// local coordinates of vertex
	s16 lx, ly, lz;		

	// world coordinates of vertex
	s16 wx, wy, wz;
		
	s16 sx, sy;
	// texture coordinates
	//s16 u;				
	//s16 v;
}vertex_type;  
 
typedef struct
{
		vertex_type *vertex[3];		// pointer list of vertices

	//12
	u16 color;					// color of polygon
	
	//14
	s16 distance;				// distance from viewer
	
	//16
	s16 istextured;				// polygon textured flag
	//u16 textureNum;				// the index into texture array that this polygon uses

	//s16 testBackface;			// do we need to test backface culling for this polygon
	
	//18,20
	s16 u0,v0;

	//22,24
	s16 u1,v1;

	//26, 28
	s16 u2,v2;

	//30, 32, 34
	s16 nx, ny, nz;

	//36, 38, 40
	s32 anx,any,anz;
	
	//42
	s16 frameNum;
	
	//44
	u8 *spriteMap;
/*	vertex_type *vertex[3];		// pointer list of vertices

	u32 color;					// color of polygon
	s16 distance;				// distance from viewer
	
	s16 istextured;				// polygon textured flag
	//u16 textureNum;				// the index into texture array that this polygon uses

	//s16 testBackface;			// do we need to test backface culling for this polygon
	
	s32 u0,v0;
	s32 u1,v1;
	s32 u2,v2;

	s16 nx, ny, nz;
	s32 anx,any,anz;
	//s16 intensity; 
	//s32 xmin, xmax;
	//s32 ymin, ymax;
	//s32 zmin, zmax;
	u8 *spriteMap;
	s8 frameNum;
*/
}polygon_type;

 
typedef struct 		
{
//	s32 matrix[4][4];
	u16 number_of_vertices;		// number of vertices in shape
	u16 number_of_polygons;		// number of polygons in shape
	s16 x,y,z;					// world coordinates of object's local origin
	s16 xangle,yangle,zangle;	// Orientation of object in space
	s32 xscale,yscale,zscale;

	vertex_type *vertex;//[300];	// array of vertices in object
	polygon_type *polygon;//[450];	// list of polygons in object
	
	
	s16 update;					// Has position been updated?
	s16 isView;
	s16 isAlive;				// is the object alive,
	s16 isHide;					// do we draw the object
	s16 isCheckCollision;		// do we need to check for collision detection?
	s16 isCheckPolygonCollision;
	s16	isLit;
	s16 wx,wy,wz;
	s32 ax,ay,az;
	s32 oldax,olday,oldaz;
	
	// bounding box around object 
	//s32 minX,maxX;
	//s32 minY,maxY;
	//s32 minZ,maxZ;
	
}object_type;

typedef struct 		
{
	s32 matrix[4][4];
	u16 number_of_vertices;		// number of vertices in shape
	u16 number_of_polygons;		// number of polygons in shape
	s32 x,y,z;					// world coordinates of object's local origin
	s32 xangle,yangle,zangle;	// Orientation of object in space
	s32 xscale,yscale,zscale;

	polygon_type polygon[1];	// list of polygons in object
	vertex_type vertex[1];		// array of vertices in object
	
	s16 update;					// Has position been updated?
	s16 isView;
	s16 isAlive;				// is the object alive,
	s16 isHide;					// do we draw the object
	s16 isCheckCollision;		// do we need to check for collision detection?
	s16 isCheckPolygonCollision;
	s32 wx,wy,wz;
	s32 ax,ay,az;
   
}object_boss_type;

typedef struct
{
	u32 number_of_objects;		// number of objects in the world
	u32 number_of_bosses;		// number of objects in the world
	u32 number_of_dirlights;		// number of lights in the world

	object_type *object;		// pointer ot object list
				
//	object_boss_type object_boss;// the special case boss for the stage

	DirectionalLight dirlight[1];

}world_type;


typedef struct 	
{	
	polygon_type *polygon[1000];
}polygon_list_type;

typedef struct {
	s32 matrix[4][4];
	s32 copx,copy,copz;
	s32 xv,yv,zv;
	s32 xangle,yangle,zangle;	
	s32 targetx,targety,targetz;
	s32 isTargetCam;
}view_type;


#define SCRIPT_RESTART		0
#define SCRIPT_STOP			-1


typedef void CollisionResponceFunc_ACTORS( void *pAct1, void *pAct2, u32 isCollide );
typedef CollisionResponceFunc_ACTORS* pColResponceFunc_ACTORS;

typedef void UpdateFunc_ACTORS( void *pAct );
typedef UpdateFunc_ACTORS* pUpdateFunc_ACTORS;


typedef struct tag_ACTOR_TYP
{	
	u16 ID;
	u16 worldSlotID;
	u32 objType;			// Type of actor
	s16 isLoaded;

	object_type *pObj;
	STATEMAN_TYP stateMan;

	s32 x,y,z;					// world coordinates of object's local origin
	s32 oldx, oldy, oldz;
	s32 xv, yv, zv;
	s32 xrotrec, yrotrec, zrotrec;
	s32 collectX, collectY, collectZ;
	s16 xangle,yangle,zangle; // Orientation of object in space
	s32 xscale,yscale,zscale;

	
	u32 curScriptEntry;
	u32 curScriptCommand;
	s32 curScriptTime;	
	
	s32 nextAnimation;
	s32 curAnimation;
	s32 curFrame;
	
	u16 numHits;
	
	pColResponceFunc_ACTORS pResponce_ACTORS_COLLISION;

	pUpdateFunc_ACTORS pUpdateAI;


	//s16 numScriptEntries;			// number of script entries actor is to follow
	const s32 *pScript;		//  "20"script entries. -1 stop, 0 restart


}ACTOR_TYP;

typedef struct tag_ACTOR_BOSS_TYP
{	
	u32 ID;
	u32 worldSlotID;
	u32 objType;			// Type of actor
	s32 isLoaded;

	object_boss_type *pObj;
	STATEMAN_TYP stateMan;

	s32 x,y,z;					// world coordinates of object's local origin	
	s32 xv, yv, zv;
	s32 xrotrec, yrotrec, zrotrec;
	s32 collectX, collectY, collectZ;
	s32 xangle,yangle,zangle; // Orientation of object in space
	s32 xscale,yscale,zscale;

	
	u32 curScriptEntry;
	u32 curScriptCommand;
	s32 curScriptTime;
	
	
	u32 numHits;
	
	pColResponceFunc_ACTORS pResponce_ACTORS_COLLISION;

	pUpdateFunc_ACTORS pUpdateAI;


	s32 numScriptEntries;			// number of script entries actor is to follow
	const s32 *pScript;		//  "20"script entries. -1 stop, 0 restart

}ACTOR_BOSS_TYP;


/*
typedef struct tag_ACTOR_DEF_TYP
{	
	s32 startX, startY, startZ;
	s32 startAngleX, startAngleY, startAngleZ;
	FIXED startScaleX, startScaleY, startScaleZ;	// MUST be FTOFX(1.0f) in struct const def
	s32 startXV, startYV, startZV;

}ACTOR_DEF_TYP;

*/
typedef void StateAction_Actor(ACTOR_TYP *pAct);
typedef StateAction_Actor* pStateAction_Actor;

#define WORLDMAN_MAX_ENTRIES		40
typedef struct
{
	s32 curLevel;		// current level player is plaiying
	
	//s32 curEntry;	
	u32 numEntries;

	s32 isInit;

	s32 worldIDPool[WORLDMAN_MAX_ENTRIES];

	ACTOR_TYP Actors[WORLDMAN_MAX_ENTRIES];

	//ACTOR_BOSS_TYP ActorBoss;

}WORLDMAN_TYP;

typedef void LoadObjFunc(ACTOR_TYP *pAct, world_type *pWorld, u32 id);
typedef LoadObjFunc *pLoadObjFunc;
								
#endif

