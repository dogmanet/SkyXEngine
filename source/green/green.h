
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __GREEN_H
#define __GREEN_H

#include <gdefines.h>
#include <common/string.h>
#include <common/array.h>
#include <common/Math.h>
#include <common/string_utils.h>

#include <graphix/graphix.h>
#include <gcore/sxgcore.h>
#include "sxgreen.h"

extern g_phy_intersect g_fnIntersect;

//##########################################################################

#define GREEN_PRECOND_IDVISCALCOBJ_ERR(idVisCalcObj) \
if (!(idVisCalcObj < (ID)m_aVisCaclObj.size()))\
{\
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - green: unresolved id '%d' for VisCalcObj", GEN_MSG_LOCATION, idVisCalcObj); \
}

#define GREEN_PRECOND_IDGREEN_ERR(idGreen) \
if (!(idGreen < (ID)m_aGreens.size() && m_aGreens[idGreen]))\
{\
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - green: unresolved id '%d' for array of models", GEN_MSG_LOCATION, idGreen); \
}

//##########################################################################

//! тип деления пространства для рендера растительности
#define GREEN_COUNT_TYPE_SEGMENTATION 4

//! количество лодов растительноси
#define GREEN_COUNT_LOD 3

//! резервация количества элементов при расчетах генерации
#define GREEN_DEFAULT_RESERVE_GEN 512

//! резервация количества элементов при расчетах видимости
#define GREEN_DEFAULT_RESERVE_COM 128

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
	static IGXContext* m_pDXDevice;

	//######################################################################

	SX_ALIGNED_OP_MEM

	CGreen();
	~CGreen();

	//**********************************************************************

	ID generate(
		const char *szName,
		const char *szPathMask,
		bool shouldAveragedRGB,
		float fCountMax,
		const char *szPath, const char *szLod1, const char *szLod2,
		const char *szPhysMesh
		);

	ID getIDsplit(ID idGreen, const float3 *pPos);

	/*! добавить объект в текущий вид растительности, если добавляет то возвращает номер последнего записанного элемента в массив сплита
	 \note pData может быть NULL, тогда будет создан новый объект, иначе этот объект перемещается между сплитами
	*/
	ID addObject(ID idGreen, float3 *pPos, CGreenDataVertex *pData, ID *pIDsplit);
	void deleteObject(ID idGreen, ID idSplit, ID idObj);
	void getPosObject(ID idGreen, ID idSplit, ID idObj, float3_t *pPos);
	void setPosObject(ID idGreen, ID *pIDsplit, ID *pIDobj, const float3_t *pPos);

	void deleteGreen(ID idGreen);
	
	void onLostDevice();
	void onResetDevice();
	
	ID addVisCaclObj();
	bool existsVisCaclObj(ID idGreen);
	void deleteVisCaclObj(ID idVisCaclObj);
	
	void save(const char *szPath);
	bool load(const char *szPath);
	void clear();
	void comArrIndeces(const IFrustum *pFrustum, const float3 *pViewPos, ID idVisCaclObj = 0);
	void render(DWORD timeDelta, const float3 *pViewPos, GREEN_TYPE type, ID idVisCaclObj = 0);
	void renderSingly(DWORD timeDelta, const float3 *pViewPos, ID id, ID idTexture, ID idVisCaclObj = 0);
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
	bool getOccurencessLeafGrass(const float3 *pMin, const float3 *pMax, int iPhysicMtl);
	
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

		//! является ли сегмент конечным (то есть не содержит внутри себя других сегментов)?
		bool m_isFinite;

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
			Array<float3_t> m_aVertex;

			//! массив индексов
			Array<uint32_t> m_aIndex;

			/*! массив id материалов, 
			 \note Если используется как физическая модель то последовательно для каждого индекса #m_aIndex
			 \note Если используется как данные нулевого лода то последовательно для каждой подгруппы
			*/
			Array<ID> m_aMtrl;

			//! количество индексов для каждой подгруппы, для физической модели не используется
			Array<int> m_aIndexCount;

			//! путь до статической модели, только для физической модели
			String m_sPathName;
		};

		//! тип растительности
		GREEN_TYPE m_typeGreen;

		//! имя текстуры маски
		String m_szMaskName;

		/*! сколько всего объектов в данной растительности
		 \note Массивы #m_pAllTrans и #m_aTransW содержат информацию обо всех объектах, в том числе и для тех, которые подготовлены к удалению
		*/
		uint32_t m_uiCountObj;

		//! имя объекта растительности
		char m_szName[OBJECT_NAME_MAX_LEN];

		//! физическая модель
		CPhysMesh *m_pPhysMesh;

		//! данные нулевого лода для определения столкновений на стороне либы для воспроизвденеие звуков листвы и определения пересечений
		CPhysMesh *m_pDataLod0;

		//! начальный/главный сплит пространства
		CSegment *m_pSplitsTree;

		//! массив всех конечных сплитов пространтва
		Array<CSegment*> m_aSplitsArr;

		//! экстремумы пространства занимаемого этой растительностью
		float3 m_vMax, m_vMin;

		//! массив трансформаций всех объектов растительности
		Array<CGreenDataVertex> m_pAllTrans;

		//! ограничивающие объемы всех объектов
		Array<CBoundBox> m_aTransW;

		//! массив удаляемых объектов, удаление происходит при специальном вызове функции удаления удаленных объектов
		Array<ID> m_aDeleteObj;

		//! массив лодов
		CLod *m_aLods[GREEN_COUNT_LOD];

		//! общее количество сегментов/сплитов
		ID m_idCountSplits;
	};

	//**********************************************************************

	//! структура содержащая информацию о видимости отдельного вида растительности
	struct CVisInfo
	{
		CVisInfo();
		~CVisInfo();

		//! массив хранящий в себе указатели на сегменты
		CSegment **m_ppSegments;

		//! размер #m_ppSegments 
		int m_iCount;

		//! сколько сегментов записано в #m_ppSegments (записывается каждый раз при расчетах)
		int m_iCountCom;

		//! виден ли объект (дерево/куст)
		Array<bool> m_aVisible;
	};

	//**********************************************************************

	//! данные для просчетов видимости
	struct CVisCaclObj
	{
		CVisCaclObj();
		~CVisCaclObj();

		//! очередь обработки просчетов видимости для каждого сегмента
		Array<CSegment*, GREEN_DEFAULT_RESERVE_COM> m_aQueue;

		//! массив информации видимости для каждого вида растительности
		Array<CVisInfo*> m_aVisibleInfo;
	};
	
protected:

	//! полное удаление удаленных объектов
	void deleteDeleted();

	//! создание вершинного буфера инстансов
	void createInstVB();

	//! установка физической оболочки виду растительности
	void setGreenNav2(CModel *pGreen, const char *szPathName);

	//! доабвить новый объект растительности pObject в pGreen
	ID addNewObject2Global(CModel *pGreen, CGreenDataVertex *pObject);

	
	void initGreenDataLod0(CModel *pGreen);

	//! загрузка модели для лода
	void loadModelForLod(CModel *pGreen, int iLod, const char *szPathModel);

	/*! считает данные трансформации для объекта на основании позиции
	 \note Только эта функция должна использоваться для генерации объектов!
	*/
	void genDataObject(CGreenDataVertex *pGreenData, const float3_t *pPos);

	uint32_t getCountBytes4SaveSplit(const CSegment *pSplit, FILE *pFile, Array<CSegment*> *pQueue);
	
	void saveSplit(const CSegment *pSplit, FILE *pFile, Array<CSegment*> *pQueue);
	void loadSplit(CSegment **ppSplit, FILE * file, Array<CSegment**> *pQueue);

	//! отрисовка того что уже посчиталось
	void render2(DWORD timeDelta, const float3 *pViewPos, ID idGreen, int iLod, ID idTexture);

	//! выравнивание объема сплита
	void alignBound(CModel *pGreen, CSegment *pSplit);
	bool genByTex(CModel *pModel, ID idMask, bool shouldAveragedRGB, float3 *pMin, float3 *pMax, float fCountMax);

	void getPartBeam(const float3 *pPos, const float3 *pDir, CSegment **ppArrSplits, int *pCount, CSegment *pSegment, int iCountCom);
	void getPartBB(const float3 *pMin, const float3 *pMax, CSegment **ppArrSplits, int *pCount, CSegment *pSegment, ID idCurrSplitRender);

	void comRecArrIndeces(ID idGreen, ID idVisCaclObj, const IFrustum *pFrustum, CSegment **ppArrSplits, int *pCount, CSegment *pSegment, const float3 *pViewPos, Array<CSegment*, GREEN_DEFAULT_RESERVE_COM> *pQueue, int iAllCount);

	void addModelInVisCaclObj(ID idModel);
	void deleteModelInVisCaclObj(ID idModel);
	
	void preSegmentation(CModel *pGreen, const float3 *pLevelMin, const float3 *pLevelMax);
	void cycleSegmentation(CSegment *pSplit, CModel *pMesh);
	void segmentation(CSegment *pSplit, CModel *pMesh);

	//! установка id сплитам
	void setSplitID(CModel *pModel);
	void setSplitID2(CModel *pModel, CSegment *pSplit, Array<CSegment*, GREEN_DEFAULT_RESERVE_COM> *pQueue);

	//! расчет ограничивающего бокса объекта на основании данных его трансформации
	void computeBB(const CGreenDataVertex &oTrans, CBoundBox &oBox);

	void sort4SplitsFront2Back(const CSegment *pParentSplit, const float3 *pViewPos, ID aSortId[GREEN_COUNT_TYPE_SEGMENTATION]);

	//**********************************************************************

	//! информация о сегментах для рендера
	Array<CVisCaclObj*> m_aVisCaclObj;

	//! количество элементов текущей растительности для отрисовки
	int m_iCurrCountDrawObj;

	//! массив всех green
	Array<CModel*> m_aGreens;

	//! буфер вершин с трансформациями растительности
	IGXVertexBuffer *m_pTransVertBuf;

	//! декларация вершин
	IGXVertexDeclaration *m_pVertexDeclarationGreen;
};



#endif
