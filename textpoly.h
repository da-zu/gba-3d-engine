// TMAPPER.H - Header file for TMAPPER.CPP, so external programs can link

// DEFINES //////////////////////////////////////////////////////////////////



// TYPES ///////////////////////////////////////////////////////////////////

// basic types

// CLASSES /////////////////////////////////////////////////////////////////

// general 3D vertex class
//class VERTEX3D
typedef struct 
{
float x,y,z;
float u,v,i;

//float length(void) { return(sqrt(x*x + y*y + z*z)); }

}VERTEX3D, *VERTEX3D_PTR;

///////////////////////////////////////////////////////////////////////////////

// general integer vertex class

typedef struct 
{

int x,y,z;
int u,v,i;

//float length(void) { return(sqrt(x*x + y*y + z*z)); }

}VERTEXI3D, *VERTEXI3D_PTR;

/////////////////////////////////////////////////////////////////////////////////

// general face class, note that 90% of this stuff isn't used, but it
// gives you an idea of what the average 3D engine might keep track of...

