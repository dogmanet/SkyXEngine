
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __GREEN_H
#define __GREEN_H

#include <gdefines.h>
#include <common/string.h>
#include <common/array.h>
#include <common/SXMath.h>
#include <common/string_utils.h>

#include <d3d9.h>
#include <gcore/sxgcore.h>
#include "sxgreen.h"

extern g_phy_intersect g_fnIntersect;

//##########################################################################

#define GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr) \
if(!(id_arr < (ID)m_aArrComFor.size()))\
{\
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - green: unresolved id '%d' for array of compute visible", GEN_MSG_LOCATION, id_arr); \
}

#define GREEN_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model) \
if(!(id_model < (ID)m_aGreens.size() && m_aGreens[id_model]))\
{\
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - green: unresolved id '%d' for array of models", GEN_MSG_LOCATION, id_model); \
}

//##########################################################################

//! тип деления пространства для рендера растительности
#define GREEN_COUNT_TYPE_SEGMENTATION 4

//! количество лодов растительноси
#define GREEN_COUNT_LOD 3

#define GREEN_DEFAULT_RESERVE_GEN 1024
#define GREEN_DEFAULT_RESERVE_COM 1024

//##########################################################################

//! класс растительности
class CGreen
{
public:

	//! плотность растительности при рендере (0,100]
	static int m_iRenderFreqGrass;

	//! дистанция лодов x - где заканчивается первый лод и начинается второй, y - где заканчивается второй и начинается третий лод
	static float2_t m_vDistLods;

	//! дистанция с которой начнет уменьшаться трава, пределы [0, m_vDistLods.x)
	static float m_fDistGrassLessening;

	//! использовать ли сортировку от ближнего к дальнему для сплитов
	static bool m_isUseSortFrontToBackSplits;

	//! direct3d device
	static IDirect3DDevice9* m_pDXDevice;

	//######################################################################

	SX_ALIGNED_OP_MEM

	CGreen();
	~CGreen();

	//**********************************************************************

	ID init(
		const char *szName,
		const char *szPathMask,
		float fCountMax,
		const char *szPath, const char *szLod1, const char *szLod2,
		const char *szPhysMesh
		);

	ID getIDSplit(ID idGreen, float3 *pPos);
	ID addObject(ID idGreen, float3 *pPos, CGreenDataVertex *pData, ID *pIDsplit);
	void deleteObject(ID idGreen, ID idSplit, ID idObj);
	void getPosObject(ID idGreen, ID idSplit, ID idObj, float3_t *pPos);
	void setPosObject(ID idGreen, ID *pIDsplit, ID *pIDobj, const float3_t *pPos);

	void deleteGreen(ID idGreen);
	
	void onLostDevice();
	void onResetDevice();
	
	ID addArrForCom();
	bool existsArrForCom(ID idGreen);
	void deleteArrForCom(ID idArr);
	
	void save(const char *szPath);
	void load(const char *szPath);
	void clear();
	void comArrIndeces(const IFrustum *pFrustum, const float3 *pViewPos, ID idArr = 0);
	void render(DWORD timeDelta, const float3 *pViewPos, GREEN_TYPE type, ID idArr = 0);
	void renderSingly(DWORD timeDelta, const float3 *pViewPos, ID id, ID idTexture);
	void renderObject(DWORD timeDelta, const float3 *pViewPos, ID id, ID idSplit, ID idObj, ID idTexture);

	ID getCountGreen();
	char* getGreenName(ID idGreen);
	int getGreenCountGen(ID idGreen);
	int getGreenCountPoly(ID idGreen);
	int getGreenTypeCountGen(ID idGreen);
	const char* getGreenModel(ID idGreen);
	const char* getGreenLod1(ID idGreen);
	const char* getGreenLod2(ID idGreen);
	const char* getGreenMask(ID idGreen);
	const char* getGreenNav(ID idGreen);
	void setGreenLod(ID idGreen, int iLod, const char *szPathName);
	void setGreenNav(ID idGreen, const char *szPathName);

	void getNavMeshAndTransform(float3_t ***pppArrVertex, int32_t **ppArrCountVertex, uint32_t ***pppArrIndex, ID ***pppArrMtl, int32_t **ppArrCountIndex, CGreenDataVertex ***pppArrTransform, int32_t **ppArrCountTransform, int32_t *pArrCountGreen);
	bool getOccurencessLeafGrass(float3 *pMin, float3 *pMax, int iPhysicMtl);
	
	bool traceBeam(const float3 *pStart, const float3 *pDir, float3 *pResult, ID *pIDgreen, ID *pIDsplits, ID *pIDobj, ID *pIDmtl);

	//######################################################################

	//! сегмент пространства с растительностью
	struct CSegment
	{
		SX_ALIGNED_OP_MEM

		CSegment();
		~CSegment();

		//! массив из 4 частей данного участка
		CSegment *m_aSplits[GREEN_COUNT_TYPE_SEGMENTATION]; 

		//! массив данных трансформаций каждого элемента данного куска
		CGreenDataVertex *m_pObjData;

		ID *m_pArrIDs;

		//! количество элементов
		int m_iCountObj;	

		//! выравненный ограничивающий объем для равномерного деления
		ISXBound *m_pBoundVolumeSys;

		//! облегающий ограничивающий объем
		ISXBound *m_pBoundVolumeP;		

		//! дистация от камеры
		float m_fDistForCamera;

		//! идентификатор куска
		ID m_id;	

		//! имеет ли кусок куски внутри себя?
		bool m_idNonEnd;

		//ID3DXMesh* BoundBox;	//ограничивающий параллелепипед (меш)
	};

	//**********************************************************************

	//! лод растительности
	struct CLod
	{
		CLod();
		~CLod();

		//! путь до лода
		String m_sPath;

		//! массив текстур лода
		Array<ID> m_aIDsTextures;

		//! статическая модель
		ISXDataStaticModel *m_pModel;
	};

	//**********************************************************************

	//! модель растительности, в том числе лоды и навигационные оболочки
	struct CModel
	{
		SX_ALIGNED_OP_MEM

		CModel();
		~CModel();

		//! класс физической модели для растительности, формат модели стандартный
		struct CPhysMesh
		{
			CPhysMesh();
			~CPhysMesh();

			//! массив вершин
			float3_t *m_pArrVertex;

			//! количество вершин в массиве m_pArrVertex
			int32_t m_iCountVertex;

			//! массив индексов
			uint32_t *m_pArrIndex;

			//! количество индексов в массиве m_pArrIndex
			int32_t m_iCountIndex;

			//! массив id материалов последовательно для каждого индекса их m_pArrIndex
			ID *m_pArrMtl;

			//! путь до статической модели
			String m_sPathName;
		};

		//! тип растительности
		GREEN_TYPE m_typeGreen;

		//! имя текстуры маски
		String m_szMaskName;

		//! сколько всего сгенерировано объектов
		uint32_t m_uiCountObj;

		//! имя объекта растительности
		char m_szName[OBJECT_NAME_MAX_LEN];

		//! физическая модель
		CPhysMesh *m_pPhysMesh;

		//! начальный/главный сплит пространства
		CSegment *m_pSplitsTree;

		//! массив всех конечных сплитов пространтва
		Array<CSegment*> m_aSplitsArr;

		//! экстремумы пространства занимаемого этой растительностью
		float3 m_vMax, m_vMin;

		//! массив трансформаций всех объектов растительности
		//CGreenDataVertex *m_pAllTrans;
		Array<CGreenDataVertex> m_pAllTrans;
		Array<CBoundBox> m_aTransW;

		Array<ID> m_aDeleteObj;

		//! массив лодов
		CLod *m_aLods[GREEN_COUNT_LOD];

		//! общее количество сегментов/сплитов
		ID m_idCountSplits;
	};

	//**********************************************************************

	//! структура содержащая минимальную необходимую информацию о сегменте модели для рендера
	struct CInfoRenderSegments
	{
		CInfoRenderSegments();
		~CInfoRenderSegments();

		//!массив хранящий в себе указатели на сегменты REALTIME
		CSegment **m_ppSegments;

		//! размер Arr 
		int m_iCount;

		//! сколько сегментов записано в Arr REALTIME 
		int m_iCountCom;
	};

	//**********************************************************************

	//! данные для просчетов видимости
	struct CIRSData
	{
		CIRSData();
		~CIRSData();

		//! очередь обработки просчетов видимости для каждого сегмента
		Array<CSegment*, GREEN_DEFAULT_RESERVE_COM> m_aQueue;

		//! массив информации для рендера сегментов
		Array<CInfoRenderSegments*> m_aIRS;

		Array<Array<bool>> m_aVisible;
	};
	
protected:
	
	void saveSplit(CSegment *pSplit, FILE *pFile, Array<CSegment*> *pQueue);
	void loadSplit(CSegment **ppSplit, FILE * file, Array<CSegment**> *pQueue);
	void render2(DWORD timeDelta, const float3 *pViewPos, ID nm, int iLod, ID idTexture);
	void alignBound(CModel *pModel, CSegment *pSplit);
	void genByTex(CModel *pModel, ID idMask, float3 *pMin, float3 *pMax, float fCountMax);

	void getPartBeam(const float3 *pPos, const float3 *pDir, CSegment **ppArrSplits, int *pCount, CSegment *pSegment, ID idCurrSplitRender);
	void getPartBB(float3 *pMin, float3 *pMax, CSegment **ppArrSplits, int *pCount, CSegment *pSegment, ID idCurrSplitRender);

	void comRecArrIndeces(ID idGreen, ID idArr, const IFrustum *pFrustum, CSegment **ppArrSplits, int *pCount, CSegment *pSegment, const float3 *pViewPos, Array<CSegment*, GREEN_DEFAULT_RESERVE_COM> *pQueue, ID idCurrSplitRender);

	void addModelInArrCom(ID idModel);
	void deleteModelInArrCom(ID idModel);
	
	void preSegmentation(CModel *pModel, float3 *pMinLevel, float3 *pMaxLevel);
	void cycleSegmentation(CSegment *pSplit, CModel *pMesh);
	void segmentation(CSegment *pSplit, CModel *pMesh);
	void setSplitID(CModel *pModel);
	void setSplitID2(CModel *pModel, CSegment *pSplit, Array<CSegment*, GREEN_DEFAULT_RESERVE_COM> *pQueue);

	void ComputeBBtrans(ID idGreen);
	void ComputeBB(const CGreenDataVertex &oTrans, CBoundBox &oBox);

	//**********************************************************************

	//! информация о сегментах для рендера
	Array<CIRSData*> m_aArrComFor;

	//! количество элементов текущей растительности для отрисовки
	int m_iCurrCountDrawObj;

	//! массив всех green
	Array<CModel*> m_aGreens;

	//! буфер вершин с трансформациями растительности
	IDirect3DVertexBuffer9 *m_pTransVertBuf;

	//! декларация вершин
	IDirect3DVertexDeclaration9 *m_pVertexDeclarationGreen;
};



#endif
