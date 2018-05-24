
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __STATIC_GEOM_H
#define __STATIC_GEOM_H

#include <common/string.h>
#include <common/array.h>
#include <gcore/sxgcore.h>
#include "sxgeom.h"

//##########################################################################

#define STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr) \
if (!(id_arr < m_aArrComFor.size()))\
{\
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - static: unresolved id '%d' for array of compute visible", GEN_MSG_LOCATION, id_arr); \
}

#define STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model, retval) \
if (!(id_model < m_aAllModels.size() && m_aAllModels[id_model]))\
{\
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - static: unresolved id '%d' for array of models", GEN_MSG_LOCATION, id_model); \
	return retval;\
}

#define STATIC_PRECOND_ERR_ID_GROUP(id_model,id_group, ret_val) \
if (!(id_model < m_aAllModels.size() && m_aAllModels[id_model] && id_group < m_aAllModels[id_model]->m_aSubSets.size()))\
{\
	LibReport(REPORT_MSG_LEVEL_ERROR, "%s - static: unresolved id '%d' for array of group in model '%d'", GEN_MSG_LOCATION, id_group, id_model); \
	return ret_val; \
}

//##########################################################################

/*! \name Типы деления
@{*/

//! квадро деление
#define GEOM_COUNT_TYPE_SEGMENTATION_QUAD 4

//! октоделение
#define GEOM_COUNT_TYPE_SEGMENTATION_OCTO 8

//! резервное количество элементов для массива просчетов 
#define GEOM_DEFAULT_RESERVE_COM 512	

//!@}

//**************************************************************************

/*! Экстремумы Y координаты уровня
@{*/

//! минимальное значение по оси Y
const D3DXVECTOR3 gс_vYmin = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);

//! максимальное значение по оси Y
const D3DXVECTOR3 gс_vYmax = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);

//!@}

//##########################################################################

//! вся геометрия уровня
class CStaticGeom
{
public:

	//! сортировать ли сплиты от ближнего к дальнему
	static bool m_isUseSortFrontToBackSplits;


	//static bool m_isUseSortFrontToBackModels;

	//! direct3d device
	static IDirect3DDevice9 *m_pDXDevice;

	//! дистанция после которой будет рисоваться лод
	static float m_fDistForLod;

	//**********************************************************************

	CStaticGeom();
	~CStaticGeom();

	SX_ALIGNED_OP_MEM

	//**********************************************************************
	
	//! сегмент, кусок модели
	struct CSegment
	{
		CSegment();
		~CSegment();

		//! массив из 4/8 частей данного участка
		CSegment* m_aSplits[GEOM_COUNT_TYPE_SEGMENTATION_OCTO]; 

		//! двумерный массив по количеству подгрупп, вложенный массив - все полигоны для данной подгруппы
		uint32_t **m_ppArrPoly;	

		//! массив с количеством полигонов на каждую подгруппу
		uint32_t *m_pCountPoly;	

		//! массив с номерами подгрупп в контексте уровня
		uint32_t *m_pNumberGroup;	

		//! массив с номерами подгрупп в контексте модели
		uint32_t *m_pNumberGroupModel;

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

		//! имеет ли кусок куски внутри себя?
		bool m_isNonEnd;

		ID3DXMesh *m_pBoundBox;	//ограничивающий параллелепипед (меш)
	};

	//**********************************************************************

	//! модель, главный юнит уровня
	struct CModel
	{
		CModel();
		~CModel();

		SX_ALIGNED_OP_MEM

		/*! Лод - низкополигональная модель, которая рисуется вместо оригинальной модели когда игрок на достаточном удалении от модели
		 \warning Все материалы должны быть одного сорта!!!
		*/
		struct CLod
		{
			CLod();
			~CLod();

			//! сорт материала, принимается только один сорт, последней текстуры
			int m_iSortGroup;

			//! путь до модели лода в папке статиечской геометрии
			char m_szPathName[1024];

			//! указатель на объект ISXDataStaticModel
			ISXDataStaticModel *m_pModel;

			//! массив идентификаторов текстур, все текстуры должны быть одного сорта
			Array<ID> m_aIDsTextures;

			//! ограничивающий объем лода
			ISXBound *m_pBoundVolume;
		};

		//******************************************************************

		/*! подгруппа модели */
		struct CSubSet
		{
			//! id подгруппы в контексте уровня
			int32_t m_idGroup;

			//! id буфера (в подгруппе) в который записаны данные геометрии модели
			int32_t m_idBuff;

			//! id стартового индекса подгруппы
			int32_t m_idIndexStart;

			//! количество индексов в подгруппе
			int32_t m_uiIndexCount;

			//! id стартовой вершины подгруппы
			int32_t m_idVertexStart;

			//! количество вершин в подгруппе
			int32_t m_uiVertexCount;


			//! плоскость подгруппы
			D3DXPLANE m_oPlane;

			//! максимум подгруппы
			float3_t m_vMax;

			//! минимум подгруппы
			float3_t m_vMin;

			//! центр подгруппы
			float3_t m_vCenter;
		};

		//******************************************************************
		
		//! позиция модели
		float3 m_vPosition;

		//! повороты модели в радианах
		float3 m_vRotation;

		//! масштаб модели
		float3 m_vScale;


		//! предыдущая позиция модели
		float3 m_vOldPosition;

		//! предыдущие повороты модели в радианах
		float3 m_vOldRotation;

		//! предыдущий масштаб модели
		float3 m_vOldScale;


		//! имя модели
		char m_szName[64];

		//! путь до модели
		char m_szPathName[1024];

		//! количество полигонов
		int32_t m_uiCountPoly;

		//! лод модели
		CLod m_oLod0;

		//! рисовать ил в данный момент лод или целую модель?
		bool m_isRenderLod;


		//! описание каждой подгруппы модели
		Array<CSubSet> m_aSubSets;

		Array<float> m_aGroupDist;

		//! массив идентификаторов текстур для каждой подгруппы
		Array<ID> m_aIDsTexures;

		//! массив с сегментами
		CSegment *m_pArrSplits;	

		//! общее количество сегментов/спилтов
		ID m_idCountSplits;	

		//! количество рисующихся сегментов
		ID m_idCountSplitsRender;	
	};

	//**********************************************************************

	/*! подгруппа статической геометрии уровня
	 \note Подгруппа внутри имеет массив, в каждом из которых не более GEOM_MAX_POLY_IN_GROUP полигонов, поэтому можно шибко не беспокоится о количестве полигонов для конкретной подгруппы
	*/
	struct CGroup
	{
		CGroup();
		~CGroup();

		//! рисовать ли полигоны этой подгруппы отдельно для каждой модели? это может понадобится к примеру для полупрозрачных материалов
		bool m_isRenderSingly;

		//! тип/вид/сорт подгруппы, для ранжирования рендера
		int m_iSortGroup;

		//! имя текстуры
		String m_sName;

		//! идентификатор текстуры
		ID m_idTexture;	

		//! общее количество вершин
		int m_iCountVertex;	

		//! общее количество индексов
		int m_iCountIndex;		

		//! количество вершин в буферах
		Array<int32_t, 4> m_aCountVertex;	

		//! количество индексов в буферах
		Array<int32_t, 4> m_aCountIndex;	

		//! массив массивов моделей в текущей подгруппе, в каждом элементе массива не более GEOM_MAX_POLY_IN_GROUP полигонов
		Array<Array<CModel*>> m_aModels;

		//! массив буферов вершин
		Array<IDirect3DVertexBuffer9*, 4> m_aVertexBuffers;
	};

	//**********************************************************************

	//! структура содержащая минимальную необходимую информацию о сегменте модели для рендера
	struct CInfoRenderSegments
	{
		CInfoRenderSegments();
		~CInfoRenderSegments();

		//! массив хранящий в себе указатели на сегменты REALTIME
		CSegment **m_ppSegments;

		//! размер m_ppSegments 
		int m_iCount;

		//! сколько сегментов записано в m_ppSegments REALTIME
		int m_iCountCom;	
	};

	//**********************************************************************

	//! данные для просчетов видимости
	struct CIRSData
	{
		CIRSData();
		~CIRSData();

		//! очередь обработки просчетов видимости для каждого сегмента
		Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> m_aQueue;

		//! массив информации для рендера сегментов
		Array<CInfoRenderSegments*> m_aIRS;
	};

	//######################################################################

	void onLostDevice();
	void onResetDevice();

	void clear();
	void save(const char *szPath);
	void load(const char *szPath);

	void sortGroup(const float3 *pViewPos, int iSortMtl);
	
	int getCountModel();

	void comArrIndeces(const ISXFrustum *pFrustum, const float3 *pViewPos, ID idArr = 0);
	bool getIntersectedRayY(float3 *pPos);

	bool sortExistsForRender(int iSortMtl, ID idArr = 0);
	
	void render(DWORD timeDelta, int iSortMtl, ID idArr = 0, ID idExcludeModel = -1, ID isExcludeGroup = -1, bool isSorted = false);
	void renderSingly(DWORD timeDelta, ID idModel, ID idTexture);
	ID addModel(const char *szPath, const char *szLod1, const char *szName);
	void deleteModel(ID idModel);

	void getMinMax(float3 *pMin, float3 *pMax);

	ID addArrForCom();
	bool existsArrForCom(ID idArr);
	void deleteArrForCom(ID idArr);

	char* getModelName(ID idModel);
	const char* getModelPathName(ID idModel);
	int getModelCountPoly(ID idModel);
	void getModelMinMax(ID idModel, float3 *pMin, float3 *pMax);

	float3* getModelPosition(ID idModel);
	float3* getModelRotation(ID idModel);
	float3* getModelScale(ID idModel);

	const char* getModelLodPath(ID idModel);
	void setModelLodPath(ID idModel, const char *szPath);

	void applyTransform(ID idModel);

	ID getModelCountGroups(ID idModel);
	ID getModelGroupIDMat(ID idModel, ID idGroup);
	void getModelGroupCenter(ID idModel, ID idGroup, float3_t *pCenter);
	void getModelGroupMin(ID idModel, ID idGroup, float3_t *pMin);
	void getModelGroupMax(ID idModel, ID idGroup, float3_t *pMax);
	void getModelGroupPlane(ID idModel, ID idGroup, D3DXPLANE *pPlane);

	void getArrBuffsGeom(
		float3_t ***pppArrVertex,	//!< (*arr_vertex)[num_model][num_vertex] - вершины модели
		int32_t	**ppArrCountVertex, //!< (*arr_count_vertex)[num_model] - количество вершин для модели
		uint32_t ***pppArrIndex,	//!< (*arr_index)[num_model][num_vertex] - индексы модели
		ID ***pppArrMtl,			//!< (*arr_mtl)[num_model][num_vertex] - материал для индекса
		int32_t **ppArrCountIndex,	//!< (*arr_count_index)[num_model] - количество индексов для модели
		int32_t *pCountModels		//!< (*count_model); количество моделей
		);

	bool traceBeam(const float3 *pStart, const float3 *Dir, float3 *pResult, ID *pIDmodel, ID *pIDmtl);


protected:

	//! структура для сортировки моделей и их подгрупп по дальности от камеры
	struct CInfoGroup
	{
		//! id модели
		ID m_idModel;

		//! id подгруппы модели
		ID m_idGroup;

		//! id подгруппы модели в контексте уровня
		ID m_idGlobalGroup;

		//! дистанция до наблюдателя
		float m_fDist;

		//int m_iCount;
	};

	//! массив для сортировки подгрупп по дальности (для рендера полупрозрачных поверхностей)
	Array<CInfoGroup*> m_aDistGroup;

	//*********************************************************************

	//! информация о сегментах для рендера
	Array<CIRSData*> m_aArrComFor; 

	//! добавляет данные о модели к данным для рендера
	void addModelInArrCom(ID idModel);

	//! удаляет данные о модели из данных о рендере
	void deleteModelInArrCom(ID idModel);

	//**********************************************************************

	//! мировая матрица трансформаций
	float4x4 m_mWorld;

	//! матрица трансформации поворотов (для нормалей)
	float4x4 m_mRotation;

	//! применяет трансформации для лода модели
	void applyTransformLod(ID id);

	//**********************************************************************

	//! ограничивающий объем всего уровня
	ISXBound *m_pBoundVolume;

	//**********************************************************************

	//! сохраняет сплит в файл
	void saveSplit(CSegment *pSplit, FILE *pFile, Array<CSegment*> *pQueue);

	//! читает сплит из файла
	void loadSplit(CSegment **ppSplit, FILE *pFile, Array<CSegment**> *pQueue);

	//**********************************************************************

	void getPartBeam(const float3 *pPos, const float3 *pDir, CSegment **ppSplits, int *iCount, CSegment *pSegment, ID idCurrSplitRender);

	void getIntersectedRayY2(float3 *pPos, CSegment **ppSplits, int *iCount, CSegment *pSegment, ID idCurrSplitRender);

	void comRecArrIndeces(ID idArr, const ISXFrustum *pFrustum, CSegment **ppSplits, int *iCount, CSegment *pSegment, const float3 *pViewPos, Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> *pQueue, ID idCurrSplitRender);

	//*************************************************************************

	//! подготовительный этап сегментации
	void preSegmentation(CModel *pMesh, ISXDataStaticModel *pModel);	

	//! сегментации модели
	void segmentation(CSegment *pSplit, CModel *pMesh, ISXDataStaticModel *pModel, int iCountSplitsSys, int iCountPolyInSegment, Array<CSegment*> *pQueue);

	//! рекусривный вызов сегментации
	void cycleSegmentation(CSegment *pSplit, CModel *pMesh, ISXDataStaticModel *pModel, int iCountSplitsSys, int iCountPolyInSegment);

	//! устанавливает m_pBoundVolumeP сплита так чтобы он полностью облегал сплит
	void editVolume(CModel *pMesh, CSegment *pSplit);

	//! установка каждому куску идентификатора, удаление пустых кусков
	void setSplitID(CSegment *pSplit, ID * SplitsIDs, ID * SplitsIDsRender);

	//**********************************************************************

	/*! Данные текущей загружаемой модели
	@{*/

	//! массив вершин текущей модели
	float3 *m_pCurrArrMeshVertex;

	//! массив индексов текущей модели
	UINT *m_pCurrArrMeshIndex;

	//!@}

	//**********************************************************************

	//! массив моделей
	Array<CModel*> m_aAllModels;

	//! массив подгрупп
	Array<CGroup*> m_aAllGroups;

	//**********************************************************************

	/*! real-time данные для рендера моделей
	@{*/

	//! создает основные массивы
	void initArrIndexPtr();

	//! удаляет все основные массивы
	void deleteArrIndexPtr();
	

	//! массив для хранения всех индексов которые будут отправлены на рендер сейчас
	uint32_t ***m_pppRTArrIndices;	

	//! массив для хранения размеров для каждого из массивов RTCPUArrIndicesPtrs
	uint32_t **m_ppRTCountDrawPoly;

	//! массив для хранения количества рисуемых полигонов для каждой подгруппы для каждой модели
	uint32_t ***m_pppRTCountDrawPolyModel;

	//! массив для хранения начала индексов для каждой подгруппы для каждой модели
	uint32_t ***m_pppRTBeginIndexModel;


	//! размер в элементах m_pRenderIndexBuffer
	int m_iSizeRenderIndexBuffer;	

	//! индексный буфер, используется и изменяется в реайлтайме при рендере уровня, имеет размер самого большого индексного буфера из всех подгрупп моделей, создается в момент загрузки моделей
	IDirect3DIndexBuffer9* m_pRenderIndexBuffer;

	//!@}
};

#endif
