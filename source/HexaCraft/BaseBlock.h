#ifndef _CBaseBlock_H_
#define _CBaseBlock_H_

#include <gdefines.h>
#include "blockID.h"
#include <xcommon/resource/IXResourceModel.h>
/*
enum HC_BLOCK_TYPE
{
	HC_BLOCK_TYPE_NONE = 0,

	
	HC_BLOCK_TYPE_HALF = 0x20, // 0010 0000

	HC_BLOCK_TYPE_HALF_SOUTH = 0x29, // 0010 1001
	HC_BLOCK_TYPE_HALF_NORTH = 0x2A,

	HC_BLOCK_TYPE_HALF_SOUTH_EAST = 0x31,
	HC_BLOCK_TYPE_HALF_NORTH_WEST = 0x32,

	HC_BLOCK_TYPE_HALF_SOUTH_WEST = 0x39,
	HC_BLOCK_TYPE_HALF_NORTH_EAST = 0x3A,
	

	HC_BLOCK_TYPE_QUART = 0x40,

	HC_BLOCK_TYPE_QUART_NORTH = 0x41,
	HC_BLOCK_TYPE_QUART_SOUTH_EAST = 0x42,
	HC_BLOCK_TYPE_QUART_SOUTH_WEST = 0x44,
	HC_BLOCK_TYPE_QUART_CENTER_SOUTH = 0x48,
	
	HC_BLOCK_TYPE_QUART_SOUTH = 0x51,
	HC_BLOCK_TYPE_QUART_NORTH_EAST = 0x52,
	HC_BLOCK_TYPE_QUART_NORTH_WEST = 0x54,
	HC_BLOCK_TYPE_QUART_CENTER_WEST = 0x58,


	HC_BLOCK_TYPE_FULL = 0x60,
};*/
enum HC_BLOCK_TYPE
{
	HC_BLOCK_TYPE_NONE = 0,

	HC_BLOCK_TYPE_TWO = 0x40,
	HC_BLOCK_TYPE_TWO_1 = 0x41,
	HC_BLOCK_TYPE_TWO_2 = 0x42,
	HC_BLOCK_TYPE_TWO_3 = 0x44,
	HC_BLOCK_TYPE_TWO_4 = 0x48,
	HC_BLOCK_TYPE_TWO_5 = 0x50,
	HC_BLOCK_TYPE_TWO_6 = 0x60,

	HC_BLOCK_TYPE_THREE = 0x80,
	HC_BLOCK_TYPE_THREE_1 = 0x81,
	HC_BLOCK_TYPE_THREE_2 = 0x82,
	HC_BLOCK_TYPE_THREE_3 = 0x84,
	HC_BLOCK_TYPE_THREE_4 = 0x88,
	HC_BLOCK_TYPE_THREE_5 = 0x90,
	HC_BLOCK_TYPE_THREE_6 = 0xA0,

	HC_BLOCK_TYPE_ONE = 0xC0,
	HC_BLOCK_TYPE_ONE_1 = 0xC1,
	HC_BLOCK_TYPE_ONE_2 = 0xC2,
	HC_BLOCK_TYPE_ONE_3 = 0xC4,
	HC_BLOCK_TYPE_ONE_4 = 0xC8,
	HC_BLOCK_TYPE_ONE_5 = 0xD0,
	HC_BLOCK_TYPE_ONE_6 = 0xE0,

	HC_BLOCK_TYPE_FULL = 0xFF,
};
/*
#define HC_GET_TYPE(bt) ((bt) >> 9)
#define HC_GET_ID(bt) ((bt) & 0x1FF)

#define HC_MKTYPE(type, id) ((((type) & 0x7F) << 9) | ((id) & 0x1FF))

#define HC_BIT_SET(a, b) ((a) & (b))
*/

//#define HC_MAX_BLOCKID 256

#define HC_GET_TYPE(bt) ((bt) >> 8)
#define HC_GET_ID(bt) ((bt) & 0xFF)

#define HC_MKTYPE(type, id) ((((type) & 0xFF) << 8) | ((id) & 0xFF))
#define HC_TYPEID(id) ((id) & 0xFF)

#define HC_BIT_SET(a, b) ((((b) >> 6) == ((a) >> 6)) && ((b) & (a) & 0x3F))

#define HC_IS_FULL(a) (((a) & 0x3F) == 0x3F)

#define HC_IS_TYPE(block, type) (((block) >> 6) == ((type) >> 6))


class CBaseBlock
{
protected:
	WORD m_BlockType = 0;

public:

	CBaseBlock();

	WORD getType();
	WORD getTypeID();
	WORD getTypeFull();

	void setType(WORD type);
	void setTypeFull(WORD type);

	virtual void advance(float fTime) = 0;

	virtual void createMesh(UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT * pIndices, UINT * pCurVertex, UINT * pCurIndex, bool * b, WORD * pBlocks) = 0;
	static void CreateMesh(UINT blockID, UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT * pIndices, UINT * pCurVertex, UINT * pCurIndex, bool * b, WORD * pBlocks);

	virtual UINT checkVecCollision(const float3 &pos, const float3 &vec, const float3 &mypos) = 0;
	static UINT CheckVecCollision(UINT blockID, const float3 &pos, const float3 &vec, const float3 &mypos);

	static CBaseBlock* GetInstance(UINT id);

	virtual UINT getMaterialCount() = 0;
	//virtual const char* getMaterialById(UINT id) = 0;
};

#include "BaseBlockTool.h"

#if 0
class DSHCblock_MALLOC
{
public:
	virtual CBaseBlock * Alloc() = 0;
	virtual void Free(CBaseBlock * ptr) = 0;
	virtual ~DSHCblock_MALLOC()
	{
	};
};

class DSHCblockPool
{
	DSHCblock_MALLOC * m_pMem[HC_MAX_BLOCKID];
public:
	DSHCblockPool();
	~DSHCblockPool();
	CBaseBlock * Alloc(UINT id);
	void Free(CBaseBlock * ptr);
	void Release();
};
#endif


class CBlockManager
{
public:
	struct vTable
	{
		CBaseBlock*(*create)(UINT/*, DSHCblockPool**/);
#if 0
		DSbaseTool*(*createtool)(UINT);
#endif
		void(*remove)(CBaseBlock* /*, DSHCblockPool**/);
		//void(*memdump)(void);
	//	DSHCblock_MALLOC*(*requestmp)(void);
		UINT(*getmaterialscount)(void);
		const char*(*getmaterialbyid)(UINT);
		UINT(*getmaterialidbyid)(UINT);
		void(*setmaterialidbyid)(UINT,UINT);
		void(*createmesh)(UINT blockID, UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT * pIndices, UINT * pCurVertex, UINT * pCurIndex, bool * b, WORD * pBlocks);
		UINT(*checkveccollision)(UINT blockID, const float3 &pos, const float3 &vec, const float3 &mypos);
	};
	//static std::map<UINT, vTable> mBlocks;
	static vTable mBlocks[HC_MAX_BLOCKID];
	static CBaseBlock *GetInstance(const UINT id/*, DSHCblockPool * pool*/);
	static Array<const char*> ms_aszTextures;
	static void Init();
#if 0
	static DSbaseTool * CreateTool(const UINT id);
#endif
	//static void DumpMemStats(UINT id);
	static void DumpMemStatsCMD(std::string cmd);
	static void FreeInstance(CBaseBlock * block/*, DSHCblockPool * pool*/);
	//static void AddBlock(UINT id, CBaseBlock*(*fn)());
//	static DSHCblock_MALLOC * RequestMemPool(const UINT id);
	static UINT GetMaterialById(const UINT bid, const UINT mid);
	static const char* GetMaterialById(const UINT mid);
	static UINT GetMaterialCount(UINT bid);
	static UINT CountMaterials();

	static void CreateMesh(UINT blockID, UINT iTexture, float fXpos, float fYpos, float fZpos, float R, float H, XResourceModelStaticVertex *pVertices, UINT * pIndices, UINT * pCurVertex, UINT * pCurIndex, bool * b, WORD * pBlocks);
	static UINT CheckVecCollision(UINT blockID, const float3 &pos, const float3 &vec, const float3 &mypos);
};

template <class T, class C>
class DSHCblockFactory
{
public:
	DSHCblockFactory(UINT id)
	{
		CBlockManager::vTable vt;
		vt.create = &(T::GetInstance);
		//vt.createtool = &(C::GetInstance);
		vt.remove = &(T::FreeInstance);
		//vt.requestmp = &(T::RequestMP);
		vt.getmaterialscount = &(T::GetMaterialCount);
		vt.getmaterialbyid = &(T::GetMaterialById);
		vt.createmesh = &(T::CreateMesh);
		vt.checkveccollision = &(T::CheckVecCollision);
		vt.getmaterialidbyid = &(T::GetMaterialIdById);
		vt.setmaterialidbyid = &(T::SetMaterialIdById);
		//vt.memdump = &(T::DumpMemStats);
		CBlockManager::mBlocks[HC_GET_ID(id)] = vt;
	}
};

//#define HC_LINK_BLOCK_TO_CLASS(id, cls) CBlockManager::mBlocks[id] = &(cls::GetInstance)


#define HC_LINK_BLOCK_TO_CLASS(id, cls) static DSHCblockFactory<cls, cls##Tool> C##cls(id)

#define HC_NEW_BLOCK(id, pool) CBlockManager::GetInstance(id, pool)

#define _CREATE_BLOCK(cls, id) CBaseBlock * p = cls##_MALLOC::Alloc();				\
	p->SetTypeFull(id);																	\
	return(p);

#define HC_FREE_BLOCK(block, pool) CBlockManager::FreeInstance(block, pool)

#define HC_DECLARE_BLOCK_MEMALLOC(cls) class cls##_MALLOC: public DSHCblock_MALLOC {	\
	public:																				\
		DSmemAlloc<cls, 2048> mData;													\
		CBaseBlock * Alloc();														\
		void Free(CBaseBlock * ptr);													\
	}

#define HC_DEFINE_BLOCK_MEMALLOC(cls)				\
	CBaseBlock * cls##_MALLOC::Alloc()			\
	{												\
		return(mData.Alloc());						\
	}												\
	void cls##_MALLOC::Free(CBaseBlock * ptr)	\
	{												\
		mData.Delete((cls*)ptr);					\
	}												

#define HC_DECLARE_BLOCK_IFACE(cls)													\
	static CBaseBlock * GetInstance(UINT id/*, DSHCblockPool * pool*/);				\
	static void FreeInstance(CBaseBlock * block/*, DSHCblockPool * pool*/);			\
	/*static DSHCblock_MALLOC * RequestMP();*/											\
	static const char * m_szTextures[];												\
	static UINT m_puMaterials[];												\
	/*static DSMATERIAL m_pMaterials[];*/												\
	static UINT GetMaterialCount();												\
	/*static DSMATERIAL GetMaterialById(UINT id);*/									\
	static const char* GetMaterialById(UINT id);									\
	static void SetMaterialIdById(UINT id, UINT uMat);									\
	static UINT GetMaterialIdById(UINT id);									\
	UINT getMaterialCount()															\
	{																				\
		return(GetMaterialCount());												\
	}																				\
	/*DSMATERIAL getMaterialById(UINT id)	*/											\
	/*{*/																				\
		/*return(GetMaterialById(id));*/												\
	/*}*/																				

#define HC_DEFINE_BLOCK_IFACE(cls)													\
	CBaseBlock * cls::GetInstance(UINT id/*, DSHCblockPool * pool*/)					\
	{	cls *nb = new cls(); nb->setTypeFull(id);return(nb);																			\
		/*return(pool->Alloc(id));*/													\
	}																				\
	void cls::FreeInstance(CBaseBlock * block/*, DSHCblockPool * pool*/)				\
	{	delete block;																			\
		/*pool->Free(block);*/															\
	}																				\
	/*DSHCblock_MALLOC * cls::RequestMP()												\
	{																				\
		return(new cls##_MALLOC());													\
	}																				\
	DSbaseTool * cls##Tool::GetInstance(UINT id)									\
	{																				\
		CBaseBlockTool * pTool = new cls##Tool();								\
		pTool->SetTypeFull(id);														\
		return(pTool);																\
	}*/

#define HC_BEGIN_MATERIAL_TABLE(cls) const char * cls::m_szTextures[] = {
#define HC_END_MATERIAL_TABLE(cls)													\
	};																				\
	/*DSMATERIAL cls::m_pMaterials[sizeof(cls::m_szTextures) /						\
		sizeof(cls::m_szTextures[0])] = {NULL};*/										\
	UINT cls::m_puMaterials[sizeof(cls::m_szTextures) /						\
		sizeof(cls::m_szTextures[0])] = {0};										\
	UINT cls::GetMaterialCount()													\
	{																				\
		return(sizeof(m_szTextures) / sizeof(m_szTextures[0]));						\
	}																				\
	UINT cls::GetMaterialIdById(UINT id)													\
	{																				\
		return(m_puMaterials[id]);						\
	}																				\
	void cls::SetMaterialIdById(UINT id, UINT uMat)													\
	{																				\
		assert(ARRAYSIZE(m_szTextures) > id);\
		m_puMaterials[id] = uMat;						\
	}																				\
	const char* cls::GetMaterialById(UINT id)										\
	{																				\
		/*if(!m_pMaterials[id])														\
		{																			\
			DSmaterialSystem::loadMaterial(m_szTextures[id]);						\
			m_pMaterials[id] = DSmaterialSystem::GetMaterial(m_szTextures[id]);		\
		}*/																			\
		return(m_szTextures[id]);													\
	}

#define HC_DEFINE_BLOCK(typeID, cls)												\
	/*HC_DEFINE_BLOCK_MEMALLOC(cls);*/													\
	HC_LINK_BLOCK_TO_CLASS(typeID, cls);											\
	HC_DEFINE_BLOCK_IFACE(cls);

#define HC_BEGIN_BLOCK_DECL(cls, basecls)											\
	class cls: public basecls														\
	{																				\
	public:																			\
		HC_DECLARE_BLOCK_IFACE(DSHCblockGrass);										\
	private:

#define HC_END_BLOCK_DECL(cls)														\
	};																				\
	/*HC_DECLARE_BLOCK_MEMALLOC(cls);*/													\
	class cls##Tool: public CBaseBlockTool										\
	{																				\
		DECLARE_CLASS(cls##Tool, CBaseBlockTool);								\
	public:																			\
		static CBaseBlockTool* GetInstance(UINT id);									\
	}
#endif
