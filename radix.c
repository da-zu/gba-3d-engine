#include <Agb.h>
#include "Types3d.h"

extern void Radix32(polygon_list_type *source, polygon_list_type *temp, u32 count, u32 N) __attribute__ ((section (".iwram"), long_call));
extern vu16 *screen_buffer2;
extern void Font_itoa ( u32 number, u8 *pBuffer, s32 size );//__attribute__ ((long_call));
extern void Gba_Print( s32 x, s32 y, u8 *text, u8 *screen);//__attribute__ ((long_call));

void Radix( s32 byte, s32 N, polygon_list_type *source, polygon_list_type *dest)
{
	s32 count[256];
	s32 index[256];
	s32 i;
	s32 j,k;
	u8 szdis[10];
	s32 distance[N];

	//memset(count, 0, sizeof(count));
	//CpuFastClear(0, count, sizeof(count)>>2);	
	for (i=0; i<256; i++)
	{
		count[i] = 0;
	}
	
	//for(i=N-1; i>=0; i--)

	j = 0;
	
	k=((N-1)>>2);
	i=0;
	//while(i < k)
	while(k>0)
	{
		distance[j] = ((-source->polygon[j]->distance)>>(byte*8))&0xff;
		count[distance[j]]++;
		j++;

		distance[j] = ((-source->polygon[j]->distance)>>(byte*8))&0xff;
		count[distance[j]]++;
		j++;

		distance[j] = ((-source->polygon[j]->distance)>>(byte*8))&0xff;
		count[distance[j]]++;
		j++;

		distance[j] = ((-source->polygon[j]->distance)>>(byte*8))&0xff;
		count[distance[j]]++;
		j++;
		
		
		k--;

	}
		
	i = (N-1)-(((N-1)>>2)<<2);

	Font_itoa(i, szdis, 10 );
	Gba_Print(130,150,szdis,screen_buffer2);

	
	while(i >= 0)
	{
		distance[j] = ((-source->polygon[j]->distance)>>(byte*8))&0xff;
		count[distance[j]]++;
		
		j++;

		i--;
	}
	/*for(i=N-1; i>=0; i--)
	{ 
		count[distance[i]]++;
	}*/

	// UPTO HERE!
	index[0] = 0;

	for(i=1; i<256; i++)
	{
		index[i]=index[i-1]+count[i-1];
	}

	for(i=0; i<N; i++)
	{
		dest->polygon[index[distance[i]]++] =  source->polygon[i];
	}
}

void Radix_Sort(polygon_list_type *source, polygon_list_type *temp, s32 N)
{
	Radix(0, N, source, temp);

	Radix(1, N, temp, source);

	Radix(2, N, source, temp);

	Radix(3, N, temp, source);
	
}
