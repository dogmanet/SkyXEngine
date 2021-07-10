#ifndef _DSHCdefines_H_
#define _DSHCdefines_H_

//#define HC_LOD_SIZE 64

#include <gdefines.h>

#define DSHC_CHUNK_VERSION 1

#define HC_CHUNK_SIZE_X 16
#define HC_CHUNK_SIZE_Y 128
#define HC_CHUNK_SIZE_Z 16

#define HC_CHUNK_CACHE_SIZE 128 /* about 128mb in total */
//#define HC_CHUNK_CACHE_SIZE 64 /* about 32mb in total */

//#define HC_SIZE 1
//#define HC_SIZE 12
#define HC_SIZE 8
//#define HC_SIZE 24
//#define HC_SIZE 32
//#define HC_SIZE 2
//#define HC_SIZE 1
//#define HC_SIZE 48

#define HC_HYSTER_SIZE 4

//#define HC_CHUNK_SIZE_X 2
//#define HC_CHUNK_SIZE_Y 1
//#define HC_CHUNK_SIZE_Z 2

//X --
#define HC_CHUNK_NEIGHBOR_NORTH 0
//X ++
#define HC_CHUNK_NEIGHBOR_SOUTH 2
//Z ++
#define HC_CHUNK_NEIGHBOR_EAST 1
//Z --
#define HC_CHUNK_NEIGHBOR_WEST 3


#define HC_GEN_SIZE (HC_SIZE * 2)
#define HC_LOD_SIZE HC_GEN_SIZE

#define cos30 0.86602540378443864676372317075294f
#define cos30x2 1.7320508075688772935274463415059f
#define cos30div2 0.43301270189221932338186158537647f

#define HC_R 0.7f
#define HC_H 1.0f

//#define HC_R 0.5f
//#define HC_H 0.7f

#define HC_PR 2.0f
//#define HC_PR 0.2f
#define HC_PH (HC_H * 1.8f)

#define Rcos30x2 (HC_R * cos30x2)
#define Rcos30 (HC_R * cos30)
#define Rdiv2 (HC_R * 0.5f)

//#define HC_GEN_THREADS 3
#define HC_GEN_THREADS 1 // At current time there can be only 1 cause of static array in CChunk::BuildBuffer2()

#define HC_X_SIZE 2097152 // Length from north to south ~ 20 000 km
#define HC_Z_SIZE 4194304 // Cycle length ~ 40 000 km

class CChunk;

struct DSHCaimPos
{
	UINT iAimType;
	UINT iAimPosX;
	UINT iAimPosY;
	UINT iAimPosZ;
	UINT iNewPosX;
	UINT iNewPosY;
	UINT iNewPosZ;
	CChunk *pChunk;
	float3_t vBPos;
};

#endif
