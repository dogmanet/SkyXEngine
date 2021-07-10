#ifndef _CChunk_H_
#define _CChunk_H_

#include "defines.h"
#include "SolidBlock.h"
#include <common/String.h>
//#include "DSHCLOD.h"
//#include "DSphysics.h"

#include <game/sxgame.h>
#include <xcommon/resource/IXResourceManager.h>
#include <xcommon/resource/IXModelProvider.h>

#define HCC_INDEX(y, x, z) ((y) * HC_CHUNK_SIZE_X * HC_CHUNK_SIZE_Z + (x) * HC_CHUNK_SIZE_X + (z))


class CChunkManager;
class CChunk: public IXUnknownImplementation<IXUnknown>
{
public:
	struct CChunkHeader
	{
		CChunkHeader():iVersion(DSHC_CHUNK_VERSION)
		{
			strcpy(szMagick, "DSHCchunk");
		}
		char szMagick[10]; // "DSHCchunk\0"
		uint32_t iVersion;
		uint32_t iDataLen;
	};

	CChunk(int64_t x, int64_t y, CChunkManager *pChunkManager);
	~CChunk();


	void buildBuffer2();

	void makeRandom();

	bool inMyInterval(const float3 &pos);
	bool inMyInterval2(const float3 &pos);
#if 0
	float isOnGround(XMFLOAT3 pos);
	float isOnGround(DSHCplayer * pl);
#endif

#if 0
	XMFLOAT3 fixPlayerPos(XMFLOAT3 pos);
	void fixPlayerPos(DSHCplayer * pl);
#endif
	WORD getBlockOffset(int iXpos, int iZpos, int iH, int iXoffset, int iZoffset);

	void writeFile();
	bool readFile();

	String getFileName();
	
	static UINT getSize()
	{
		return(sizeof(CChunk) + HC_CHUNK_SIZE_X * HC_CHUNK_SIZE_Y * HC_CHUNK_SIZE_Z * sizeof(CBaseBlock));
	}

	void getPos(int64_t * pX, int64_t * pZ)
	{
		if(pX)
		{
			*pX = m_iXpos;
		}
		if(pZ)
		{
			*pZ = m_iYpos;
		}
	}

	void findUserInChunk(float fX, float fZ, UINT * pX, UINT * pZ, float * fXblock = NULL, float * fZblock = NULL);
#if 0
	void findUserInChunk(DSHCplayer * pl, UINT * pX, UINT * pZ, UINT * pY, float * fXblock = NULL, float * fYblock = NULL, float * fZblock = NULL);
#endif

	void findAimingBlock(const float3 &pos, const float3 &vec, float len, DSHCaimPos *pAP);

	float3 getBlockPos(int x, int z, int y);

	void removeBlock(int x, int z, int y);
	void setBlock(int x, int z, int y, UINT type, bool reb=true);

	void reBuild();

	void updatePos();

	void releasePhysics();

	void save();

	CChunk* getHeighbour(UINT which);

	void dematerialize();

private:
	//IXResourceModelStatic *m_pResourceModel = NULL;
	//IXStaticModel *m_pModel = NULL;

	CChunkManager *m_pChunkManager = NULL;

	CBaseEntity *m_pEntity = NULL;

	//CSolidBlock ***m_pppMatrix;
	WORD m_pMatrix[HC_CHUNK_SIZE_X * HC_CHUNK_SIZE_Z * HC_CHUNK_SIZE_Y];
	int64_t m_iXpos;
	int64_t m_iYpos;
	//bool m_bReady = false;

	bool m_bChanged = false;
	bool m_bBorder = false;

	void XMETHODCALLTYPE FinalRelease() override;

#if 0
	btTriangleMesh *m_pCollideMesh = NULL;
	btCollisionShape *m_pCollideShape = NULL;
	btRigidBody *m_pRigidBody = NULL;
#endif


};



#endif
