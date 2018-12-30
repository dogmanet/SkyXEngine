
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __MODELS_H
#define __MODELS_H

#include <new>
#include <common/string.h>
#include <common/string_utils.h>
#include <common/array.h>
#include <gcore/sxgcore.h>
#include "sxgeom.h"

/*! \name Типы деления
@{*/

//! квадро деление
#define GEOM_COUNT_TYPE_SEGMENTATION_QUAD 4

//! октоделение
#define GEOM_COUNT_TYPE_SEGMENTATION_OCTO 8

//! резервное количество элементов для массива просчетов 
#define GEOM_DEFAULT_RESERVE_COM 512	

//!@}

#define GEOM_COPY_POSTFIX "-copy"

extern IDirect3DDevice9 *g_pDXDevice;
extern D3DCAPS9 g_dxCaps;

//##########################################################################

#define STATIC_PRECOND_VISCALCOBJ_ID(idVisCalcObj, retVal) \
if (!(idVisCalcObj >= 0 && m_aVisInfo.size() > idVisCalcObj && m_aVisInfo[idVisCalcObj]))\
{\
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - static: unresolved id '%d' for array of compute visible", GEN_MSG_LOCATION, idVisCalcObj); \
	return retVal; \
}

#define STATIC_PRECOND_MODEL_ID(idModel, retVal) \
if(!(idModel >= 0 && idModel < (int)m_aModels.size() && m_aModels[idModel]))\
{\
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - static: unresolved id '%d' for array of models", GEN_MSG_LOCATION, idModel); \
	return retVal; \
}

#define STATIC_PRECOND_TRANSPARENCY_MODEL_ID(idModel, retVal) \
if(!(idModel >= 0 &&idModel < (int)m_aTransparency.size() && m_aTransparency[idModel]))\
{\
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - static: unresolved id '%d' for array of transparency models", GEN_MSG_LOCATION, idModel); \
	return retVal; \
}

#define STATIC_ERROR_MODEL_GROUP_ID(idModel, idGroup, retVal) \
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - static: unresolved id '%d' for group models %d", GEN_MSG_LOCATION, idGroup, idModel); \
	return retVal;

//##########################################################################

/*
идентификаторы моделей не константны, при удалении однйо из моделей, все верхние модели изменяют свой id, ппп меняют при этом id родителя, объекты просчетов видимости удаляют из себя инфу о удаленной модели
лучшего варианта установки пп моделям id родителя не нашел, поэтмоу сначала разбирается основная модель - создаются пп модели, затем остальные операции с основной моделью + добавление ее в массив моделей, а затем всем пп моделям присваиваем id родителя
*/

//! класс моделей статической геометрии
class CModels
{
public:
	CModels();
	~CModels();

	//! класс физической модели, формат модели стандартный
	struct CPhysMesh
	{
		CPhysMesh();
		~CPhysMesh();

		//! массив вершин
		Array<float3_t> m_aVertex;

		//! массив индексов
		Array<uint32_t> m_aIndex;

		//! массив id материалов для каждого индекса
		Array<ID> m_aMtrl;

		//! номера имен текстур для каждого индекса
		Array<ID> m_aNumMtrl;

		//! массив имен материалов для каждой подгруппы
		Array<String> m_aStringMtrl;

		//! путь до статической модели
		String m_sPath;
	};

	/*! Лод - низкополигональная модель, которая рисуется вместо оригинальной модели когда наблюдатель на достаточном удалении от модели
	*/
	struct CLod
	{
		CLod();
		~CLod();

		//! путь до модели лода в папке статической геометрии
		String m_sPath;

		//! указатель на объект ISXDataStaticModel
		ISXDataStaticModel *m_pModel;

		//! массив идентификаторов текстур, все текстуры должны быть одного сорта
		Array<ID> m_aIDsTextures;
	};

	//! сегмент, кусок модели
	struct CSegment
	{
		CSegment();
		~CSegment();

		//! массив из 4/8 частей данного участка
		CSegment *m_aSplits[GEOM_COUNT_TYPE_SEGMENTATION_OCTO];

		//! указатель на родительский кусок
		CSegment *m_pParent;

		//! двумерный массив по количеству подгрупп, вложенный массив - все полигоны для данной подгруппы
		uint32_t **m_ppArrPoly;

		//! массив с количеством полигонов на каждую подгруппу
		uint32_t *m_pCountPoly;

		//! массив с номерами подгрупп
		uint32_t *m_pNumberGroup;

		//! количество подгрупп
		uint32_t m_uiCountSubSet;

		//! общее количество полигонов
		uint32_t m_uiCountAllPoly;


		//! выравненный ограничивающий объем для равномерного деления
		ISXBound *m_pBoundVolumeSys;

		//! облегающий ограничивающий объем
		ISXBound *m_pBoundVolumeP;

		//! текущее расстояние до камеры (для определения когда рисовать лод)
		float m_fDistForCamera;

		//! идентификатор куска
		ID m_ID;

		//! порядковый номер куска из массива рисующихся кусков
		ID m_SID;

		//! глубина вложенности текущего куска
		ID m_idDepth;

		//! имеет ли кусок куски внутри себя?
		bool m_isFinite;

		//ID3DXMesh *m_pBoundBox;	
	};

	//! информация о подгруппе
	struct CGroupInfo
	{
		CGroupInfo();
		~CGroupInfo();

		SX_ALIGNED_OP_MEM2();

		//! минимум
		float3 m_vMin;

		//! максимум
		float3 m_vMax;

		//! центр
		float3 m_vCenter;

		//! плоскость
		D3DXPLANE m_oPlane;
	};

	//! полупрозрачная модель
	struct CTransparencyModel
	{
		CTransparencyModel();
		~CTransparencyModel();

		SX_ALIGNED_OP_MEM2();

		//! id модели к которой принадлжеит данная ппп
		ID m_idModel;

		//! видна ли камере
		bool m_isVisible4Observer;

		//! дистанция до наблюдателя
		float m_fDist4Observer;

		//! информация о подгруппе :)
		CGroupInfo m_oGroupInfo;

		//! имя текстуры
		String m_sTex;

		//! id текстуры
		ID m_idTex;

		//! количество вершин
		int32_t m_iCountVertex;

		//! количество индексов
		int32_t m_iCountIndex;

		//! вершинный буфер
		IDirect3DVertexBuffer9 *m_pVertexBuffer;

		//! индексный буфер
		IDirect3DIndexBuffer9 *m_pIndexBuffer;

		//! ограничивающий объем
		ISXBound *m_pBoundVolume;
	};

	//! структура модели
	struct CModel
	{
		CModel();
		~CModel();

		SX_ALIGNED_OP_MEM

		//! имя модели
		String m_sName;

		//! путь до модели в папке статической геометрии
		String m_sPath;

		//! указатель на объект ISXDataStaticModel
		ISXDataStaticModel *m_pModel;

		//! указатель на лод
		CLod *m_pLod;

		//! физическая модель
		CPhysMesh *m_pPhysics;

		//! массив идентификаторов текстур
		Array<ID> m_aIDsTextures;

		//! массив с информацией о подгруппах
		Array<CGroupInfo*> m_aGroupInfo;

		//! ограничивающий объем оригинальной модели
		ISXBound *m_pBoundVolume;

		//! рисовать ли лод?
		bool m_isRenderLod;

		//! дистанция до наблюдателя
		float m_fDist4Observer;

		//! массив полупрозрачных моделей принадлежащих данной модели
		Array<ID> m_aTransparency;

		//! массив с сегментами
		CSegment *m_pArrSplits;

		//! общее количество сегментов/спилтов
		ID m_idCountSplits;

		//! количество рисующихся сегментов
		ID m_idCountSplitsRender;

		//! количество полигонво которые надо отрисовать для каждой подгруппы
		UINT *m_pCountDrawPoly;

		//! массив массивов индексов для каждой подгруппы, которые надо отрисовать
		UINT **m_pVisibleIndeces;

		//! буфер индексов, заполняется в real-time
		IDirect3DIndexBuffer9 *m_pVisibleIndexBuffer;
	};

	//######################################################################

	//! структура содержащая минимальную необходимую информацию о сегменте модели для рендера
	struct CVisInfoModel
	{
		CVisInfoModel();
		~CVisInfoModel();

		//! видна ли модель
		bool m_isVisible;

		//! массив хранящий в себе указатели на сегменты
		CSegment **m_ppSegments;

		//! размер m_ppSegments 
		int m_iCount;

		//! сколько сегментов записано в m_ppSegments
		int m_iCountCom;
	};

	//! объект расчетов видимости
	struct CVisInfo
	{
		CVisInfo();
		~CVisInfo();

		//! информация о видимости основной модели
		Array<CVisInfoModel*> m_aVisible4Model;

		//! информация о видимости пп моделей
		Array<bool> m_aVisible4Transparency;
	};

	//######################################################################

	//! сохранение данных о всех моделях в бинарный файл
	void save(const char *szPath);

	//! загрузка бинарного файла с данными моделей
	void load(const char *szPath);

	//**********************************************************************

	//! добавить модель
	ID addModel(const char *szPath, const char *szName, const char *szLod, const char *szPhys, bool needSegmentation);

	//! удалить модель (из объектов расчетов видимости тоже удалится)
	void deleteModel(ID idModel);

	//! очистка списка моделей (удаляет все модели)
	void clear();

	//! полное копирование модели по ее id, возвращает id новой (копии) модели
	ID copy(ID idModel);

	//! возвращает количество моделей
	int getCountModel();

	//! записывает минимум и максимум всех моделей
	void getMinMax(float3 *pMin, float3 *pMax);

	//**********************************************************************

	//! добавить новый объект расчетов видимости, возвращает id
	ID addVisCaclObj();

	//! существует ли объект расчетов видимости с id
	bool existsVisCaclObj(ID idVisCaclObj);

	//! удалить объект расчетов видимости по его id
	void deleteVisCaclObj(ID idVisCaclObj);

	//**********************************************************************

	//! сортировка пп моделей для наблюдателя
	void sortTransparency(const float3 *pViewPos);

	//! есть ли пп модели для рендра в idVisCalcObj объекте просчетов видимости
	bool existsTransparency4Render(ID idVisCalcObj);

	//**********************************************************************

	//! установка подгруппы модели
	void modelSetPosition(ID idModel, const float3 *pPos);

	//! возвращает (и записывает в аргумент если не NULL) позицию модели
	const float3* modelGetPosition(ID idModel, float3 *pPos=0);

	//! установка поворотов модели (в радианах)
	void modelSetRotation(ID idModel, const float3 *pRot);

	//! возвращает (и записывает в аргумент если не NULL) повороты модели
	const float3* modelGetRotation(ID idModel, float3 *pRot=0);

	//! установка масштабирования модели
	void modelSetScale(ID idModel, const float3 *pScale);

	//! возвращает (и записывает в аргумент если не NULL) масштаб модели
	const float3* modelGetScale(ID idModel, float3 *pScale=0);

	//! расчет ограничивающих объемов, при изменении мировой матрицы объекта, объемы могут трансформироваться не совсем корректно, эта функция должна исправить ситуацию
	void modelComBound(ID idModel);

	//**********************************************************************

	//! установка лода модели
	void modelSetLod(ID idModel, const char *szPath);

	//! установка физической оболочки модели
	void modelSetPhysics(ID idModel, const char *szPath);

	//! установка физической оболочки модели
	void modelSetPhysics2(CModel *pModel, const char *szPath);

	//! возвращает путь до файла модели
	const char* modelGetPath4Model(ID idModel);

	//! возвращает путь до файла лода
	const char* modelGetPath4Lod(ID idModel);

	//! возвращает путь до файла физической модели
	const char* modelGetPath4Physics(ID idModel);
	
	//! записывает минимум и максимум модели
	void modelGetMinMax(ID idModel, float3 *pMin, float3 *pMax);

	//! возвращает имя модели и записывает ее в szName если этот аргумент не NULL
	const char* modelGetName(ID idModel, char *szName=0);

	//! установка имени для модели
	void modelSetName(ID idModel, const char *szName);

	//! возвращает количество полигонов модели
	int modelGetCountPoly(ID idModel);
	
	//! возвращает количество подгрупп модели
	int modelGetCountGroups(ID idModel);

	//! возвращает id материала подгруппы модели
	ID modelGetGroupMtrlID(ID idModel, ID idGroup);
	
	//! записывает минимум/максимум подгруппы модели
	void modelGetGroupMinMax(ID idModel, ID idGroup, float3_t *pMin, float3_t *pMax);

	//! записывает плоскость подгруппы модели
	void modelGetGroupPlane(ID idModel, ID idGroup, D3DXPLANE *pPlane);

	//! записывает точку центра подгруппы модели
	void modelGetGroupCenter(ID idModel, ID idGroup, float3_t *pCenter);

	//! сегментирована ли модель
	bool modelGerSegmentation(ID idModel);

	//**********************************************************************

	//! расчет видимости
	void comVisible(const IFrustum *pFrustum, const float3 *pViewPos, ID idVisCalcObj = SX_GEOM_DEFAULT_VISCALCOBJ);

	//**********************************************************************

	//! рендер одной модели
	void renderSingly(DWORD timeDelta, ID idModel, ID idMtrl = -1, const float3 *pPos = 0);

	//! рендер моделей
	void render(DWORD timeDelta, GEOM_RENDER_TYPE type, ID idVisCalcObj = SX_GEOM_DEFAULT_VISCALCOBJ);

	//**********************************************************************

	//! записыает физические данные всех моделей
	void getArrBuffsGeom(
		float3_t ***pppArrVertex,	//!< (*arr_vertex)[num_model][num_vertex] - вершины модели
		int32_t	**ppArrCountVertex, //!< (*arr_count_vertex)[num_model] - количество вершин для модели
		uint32_t ***pppArrIndex,	//!< (*arr_index)[num_model][num_vertex] - индексы модели
		ID ***pppArrMtl,			//!< (*arr_mtl)[num_model][num_vertex] - материал для индекса
		int32_t **ppArrCountIndex,	//!< (*arr_count_index)[num_model] - количество индексов для модели
		int32_t *pCountModels		//!< (*count_model); количество моделей
		);

	//трассировка луча
	bool traceBeam(const float3 *pStart, const float3 *pDir, float3 *pResult, ID * idModel, ID * idMtrl);

protected:

	//! массив моделей, нулевых указателей нет, все очищается
	Array<CModel*> m_aModels;

	//! массив объектов расчетов видимости
	Array<CVisInfo*> m_aVisInfo;

	//**********************************************************************

	//! создание пп модели
	ID createTransparencyModel(ID idTex, const char *szTex, const vertex_static_ex *pArrVertex, int iCountVertex, const UINT *pArrIndex, int iCountIndex);

	//! массив пп моделей
	Array<CTransparencyModel*> m_aTransparency;

	//! массив пп моделей, сортированный по дистанции до пп модели от наблюдателя
	Array<CTransparencyModel*> m_aSortTransparency;

	//**********************************************************************

	/*! Данные текущей загружаемой модели
	@{*/

	//! массив вершин текущей модели
	float3 *m_pCurrArrMeshVertex;

	//! массив индексов текущей модели
	UINT *m_pCurrArrMeshIndex;

	//!@}

	//**********************************************************************

	//! расчет доступных для луча сегментов, для #traceBeam
	void getPartBeam(ID idModel, ID idVisCalcObj, const float3 *pStart, const float3 *pDir);

	//**********************************************************************

	//! добавить модель в объекты расчетов видимости
	void addModelInVisCalcObj(ID idModel);

	//! удалить модель из объектов расчетов видимости
	void deleteModelOutVisCalcObj(ID idModel);

	//**********************************************************************

	//! сегментация модели
	void segmentation(CModel *pModel);

	//! сегментации сегмента модели
	void segmentation2(CSegment *pSplit, CModel *pMesh, int iCountSplitsSys, int iCountPolyInSegment, Array<CSegment*> *pQueue);

	//! рекусривный вызов сегментации
	void segmentationCycle(CSegment *pSplit, CModel *pMesh, int iCountSplitsSys, int iCountPolyInSegment);

	//! устанавливает m_pBoundVolumeP сплита так чтобы он полностью облегал сплит
	void editVolume(CModel *pMesh, CSegment *pSplit);

	//! установка каждому куску идентификатора, удаление пустых кусков, установка каждому куску родителя
	void setSplitID(CSegment *pSplit, ID *pSplitsIDs, ID *pSplitsIDsRender);

	//! создание внешний данных для расчетов видимости сегментов модели
	void createExternalData4SegmentModel(CModel *pModel);

	//**********************************************************************

	//! пересчет идентификаторов пп моделей для родительских моделей (на случай перекомпоновки массива, когда удаляется одна из основных моделей)
	void reCalcIdTransparency();

	//! переустановка id родителя для пп моделей
	void reSetIdModel4Transparency(ID idModel);

	//**********************************************************************

	//! генерация информации о подгруппах модели
	void modelGenGroupInfo(CModel *pModel);

	//! генерация информации подгруппе пп модели
	void modelGenGroupInfo4Transparency(CTransparencyModel *pModel);

	//**********************************************************************

	//! расчет баундов для пп моделей по id основной модели, баунд основной модели при этом может увеличиться, так как пп дочерние объекты
	void modelComBoundArrTransparency(ID idModel);

	//! расчет баундов сегментов
	void modelComBound4Segments(ID idModel);

	//**********************************************************************

	//! расчет видимости пп моделей
	void comVisibleTransparency(const IFrustum *pFrustum, const float3 *pViewPos, ID idVisCalcObj = 0);

	//! определение видимости сегмента
	void comVisibleSegment(ID idVisCalcObj, const IFrustum *pFrustum, CSegment **ppSplits, int *iCount, CSegment *pSegment, const float3 *pViewPos, Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> *pQueue, int iCountSplitsRender);

	//**********************************************************************

	//! рендер лода модели
	void renderLod(DWORD timeDelta, ID idModel, ID idTex = -1, const float3 *pPos = 0);

	//! рендер модели
	void renderObject(DWORD timeDelta, ID idModel, ID idTex = -1, const float3 *pPos = 0);

	//! рендер сегментов модели
	void renderSegmets(DWORD timeDelta, ID idModel, ID idTex = -1, ID idVisCalcObj = SX_GEOM_DEFAULT_VISCALCOBJ);

	//! рендер пп модели
	void renderTransparency(DWORD timeDelta, CTransparencyModel *pTransparencyModel);
};

#endif