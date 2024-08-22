#include <Agb.h>
#include "Types3d.h"

void Radix( s32 byte, s32 N, polygon_list_type *source, polygon_list_type *dest)
{
	s32 count[256];
	s32 index[256];
	s32 i;

	//memset(count, 0, sizeof(count));
	//CpuFastClear(0, count, sizeof(count)>>2);	
	for (i=0; i<256; i++)
	{
		count[i] = 0;
	}
	
	for(i=N-1; i>=0; i--)
	{ 
		count[((-source->polygon[i]->distance)>>(byte*8))&0xff]++;
	}

	index[0] = 0;

	for(i=1; i<256; i++)
	{
		index[i]=index[i-1]+count[i-1];
	}

	for(i=0; i<N; i++)
	{
		dest->polygon[index[((-source->polygon[i]->distance)>>(byte*8))&0xff]++] =  source->polygon[i];
	}
}

void Radix_Sort(polygon_list_type *source, polygon_list_type *temp, s32 N)
{
	Radix(0, N, source, temp);

	Radix(1, N, temp, source);

	Radix(2, N, source, temp);

	Radix(3, N, temp, source);
	
}
