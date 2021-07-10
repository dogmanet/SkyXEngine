#ifndef _CChunkManager_H_
#define _CChunkManager_H_

#include "Chunk.h"
#include <xcommon/IXCore.h>

#include <xcommon/IAsyncTaskRunner.h>
//#include "DSHCLOD.h"

//#include <vector>
//#include <map>

class CChunkManager;
class CUpdateTask: public IXUnknownImplementation<IAsyncTask>
{
public:
	CUpdateTask(CChunkManager *pChunkManager):
		m_pChunkManager(pChunkManager)
	{
	}
	~CUpdateTask()
	{
	}
	void XMETHODCALLTYPE exec();
	void XMETHODCALLTYPE abort()
	{
		// do nothing
	}
	void XMETHODCALLTYPE onFinished()
	{
		// do nothing
	}

	const char* XMETHODCALLTYPE getName()
	{
		return("Chunk updating");
	}

	int XMETHODCALLTYPE getProgress()
	{
		return(0);
	}

protected:
	CChunkManager *m_pChunkManager;
	std::atomic_int m_iDone;
};

//##########################################################################

class CChunkManager
{
public:
	CChunkManager(IXCore *pCore);
	~CChunkManager();

	void generate(IEventChannel<XEventLevelProgress> *pEventChannel = NULL, XEventLevelProgress *pEv = NULL);

	UINT getTotalTextureCount()
	{
		return(CBlockManager::CountMaterials());
	}

	const char* getTextureByID(UINT id)
	{
		return(CBlockManager::GetMaterialById(id));
	}

	IXResourceManager* getResourceManager()
	{
		return(m_pResourceManager);
	}

	void enqueueChunkDelete(CChunk *pChunk);

	void setLevelName(const char *szName)
	{
		m_sLevelName = szName;
	}

	const char* getLevelName()
	{
		return(m_sLevelName.c_str());
	}

	IFileSystem* getFileSystem();

#if 0
	float isOnGround(const float3 &pos);
	float isOnGround(DSHCplayer * pl);
#endif

	void release();

	CChunk* getChunk(int64_t x, int64_t z, bool addRef = true);

	void freeChunk(int64_t x, int64_t z, bool bForce=false);

	void shift(int x, int z);
	void shift();

#if 0
	static void ShiftX(std::string prm);
	static void ShiftZ(std::string prm);
#endif

	void preLoad();// try to load as much chunks as possible
	void forceUnload(); // unload all chunks outside the border

	void findChunkByCoord(const float3 &vPos, int64_t *pX, int64_t *pZ);

#if 0
	float3 fixPosition(const float3 &pos);
	void fixPosition(DSHCplayer * pl);
#endif

	void findAimingBlock(const float3 &pos, const float3 &vec, float len, CChunk *chunk, DSHCaimPos *pAP);

	void getDelta(int64_t * x, int64_t * z)
	{
		if(x)
		{
			*x = m_iXdelta;
		}
		if(z)
		{
			*z = m_iZdelta;
		}
	}

	void saveAll();

	static int ClearMem(size_t);

	void unload();
#if 0
	static void DumpRenderChunkCMD(std::string);
#endif


	void setCameraPos(const float3 &vPos);

private:
	IXCore *m_pCore;

	IXResourceManager *m_pResourceManager = NULL;

	CChunk *m_pChunk = NULL;

	MemAlloc<CChunk, 1024> m_dChunks;

	int64_t m_iXdelta = 0;
	int64_t m_iZdelta = 0;

	int64_t m_iXplayer = 0;
	int64_t m_iZplayer = 0;

	//int64_t m_iXshift = 0;
	//int64_t m_iZshift = 0;


	//Map<int64_t, Map<int64_t, DSHCCCelem>> m_mChunks;
	CChunk *m_aapChunks[HC_CHUNK_CACHE_SIZE][HC_CHUNK_CACHE_SIZE];

	UINT m_uLastSize = 0;

	UINT m_iTotalTextureCount;

	String m_sLevelName;

	IAsyncTaskRunner *m_pTaskRunner = NULL;
	CUpdateTask m_updateTask;
	std::atomic_bool m_isDone;
};

#endif
