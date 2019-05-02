
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxgeom - библиотеки базовой орагнизации статической геометрии
*/

/*! \defgroup sxgeom sxgeom - библиотека с базовой организацией статической геометрии
 \note Поддерживается quad (4) и octo (8) деление моделей, определение необходимости в делении осуществлятся внутренним механизмом
@{
*/

#ifndef __SXGEOM_H
#define __SXGEOM_H

#include <gdefines.h>

#include <xcommon/render/IFrustum.h>

//#if defined(_DEBUG)
//#pragma comment(lib, "sxgcore_d.lib")
//#else
//#pragma comment(lib, "sxgcore.lib")
//#endif
//
//#undef SX_LIB_API
//#define SX_LIB_API extern "C" __declspec (dllimport)
//#include <gcore/sxgcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

//##########################################################################

/*! \defgroup sxgeom_models Статическая геометрия
 \ingroup sxgeom
 \note Вся геометрия состояит из набора подгрупп. То есть в данной реализации не существует отдельного понятия модель, но есть некоторая его интерпретация.\n\n
По умолчанию рендер моделей осуществляется целой подгруппой со всеми входящими в нее моделями за один вызов DIP, но есть возможность раздельного рендера подгрупп каждой модели в единой подгруппе.
Это возможно за счет функции #SGCore_MtlGroupRenderIsSingly которая вызывается для каждой загружаемой подгруппы модели.
 \note В любой момент времени, при наличии в загруженных моделей, можно пройти по списку моделей циклом, начиная с 0 и до #SGeom_ModelsGetCount
 \note Пример распаралеливания просчетов видимости и рендера:
\code
ID idVisCalcObj4Observer = SGeom_ModelsAddVisCaclObj();
ID idVisCalcObj4Shadow = SGeom_ModelsAddVisCaclObj();
ID idVisCalcObj4Light = SGeom_ModelsAddVisCaclObj();
//...
SGeom_ModelsComVisible(pFrustumObserver, &vPosObserver, idVisCalcObj4Observer);
SGeom_ModelsComVisible(pFrustumShadow, &vPosShadow, idVisCalcObj4Shadow);
SGeom_ModelsComVisible(pFrustumLight, &vPosLight, idVisCalcObj4Light);
//...
SGeom_ModelsRender(timeDelta, type, idVisCalcObj4Observer);
SGeom_ModelsRender(timeDelta, type, idVisCalcObj4Shadow);
SGeom_ModelsRender(timeDelta, type, idVisCalcObj4Light);
\endcode
@{
*/

//! минимальная разница между сторонами для окто деления
#define SX_GEOM_DIFFERENCE_SIDES_FOR_OCTO	0.3		

//! минимальный общий объем модели для деления */
#define SX_GEOM_MIN_ALLVOLUME_FOR_SEGMENTATION	20		

//! минимальный длина по горизонтальной оси модели для деления
#define SX_GEOM_MIN_LENGTH_FOR_SEGMENTATION	10		

//! минимальный длина по горизонтальной оси модели для деления
#define SX_GEOM_FORCE_ALLVOLUME_FOR_SEGMENTATION	150		

//! минимальный высота модели для деления окто
#define SX_GEOM_MIN_HEIGHT_FOR_SEGMENTATION	14		

//! минимальнео количество полигонов в модели для деления
#define SX_GEOM_MIN_POLYGONS_FOR_SEGMENTATION	5000	

//! минимальное количество полигонов в сплите
#define SX_GEOM_MIN_COUNT_POLY	500		

//! максимальное количество полигонов в сплите
#define SX_GEOM_MAX_COUNT_POLY	1000	

//! магическое слово для записи в бинарный файл статики, для идентификации
#define SX_GEOM_MAGIC_WORD	"sxgeom  "

//! магическое число для идентификации файла статики
#define SX_GEOM_MAGIC_NUM	2314970533678512243

//! версия формата файла статики
#define SX_GEOM_FILE_FORMAT_VERSION 1

//! дистанция до модели в метрах, при достижении которой будет рисоваться лод
#define SX_GEOM_DIST4LOD 100.0


/*! \name Предустановленные (константные) идентификаторы объектов расчетов видимости
 \warning Менять значения категорически не рекомендуется, только если Вы знаете что делаете
@{*/

//! идентификатор дефолтного (для наблюдателя) объекта расчетов видимости
#define SX_GEOM_DEFAULT_VISCALCOBJ	0

//! идентификатор объекта расчетов видимости для трассировки лучей
#define SX_GEOM_TRACEBEAM_VISCALCOBJ	1

//!@}

//! типы рендера моделей
enum GEOM_RENDER_TYPE
{
	//! рисовать все модели
	GEOM_RENDER_TYPE_ALL = -1,

	//! рисовать только непрозрачные модели
	GEOM_RENDER_TYPE_OPAQUE = 0,

	//! рисовать только прозрачные материалы
	GEOM_RENDER_TYPE_TRANSPARENCY = 1,
};

//**************************************************************************
	
//! очистить список всех моделей
SX_LIB_API void SGeom_Clear();	

//! сохранить все модели в бинарный файл
SX_LIB_API void SGeom_Save(const char *szPath);	

//! загрузить бинарный файл с моделями
SX_LIB_API void SGeom_Load(const char *szPath);

//! количество моделей в текущем списке
SX_LIB_API int SGeom_GetCountModels();

//! просчитать видимость всех моделей для фрустума 
SX_LIB_API void SGeom_ComVisible(	
	const IFrustum *pFrustum,	//!< фрустум, для которого считаем видимость моделей
	const float3 *pViewPos,		//!< позиция источника фрустума чтобы просчитать дистанцию
	ID idVisCalcObj = SX_GEOM_DEFAULT_VISCALCOBJ //!< идентификатор массива информации о видимости для фрустума, создается через #SGeom_ModelsAddVisCaclObj, если 0 то считаем в дефолтный
	);	

//! есть ли полупрозрачные поверхности к отрисовке в данном объекте просчетов видимости
SX_LIB_API bool SGeom_TransparencyExistsForRender(ID idVisCalcObj = SX_GEOM_DEFAULT_VISCALCOBJ);

//! рендер одной модели
SX_LIB_API void SGeom_RenderSingly(DWORD timeDelta, ID idModel, ID idMtrl = -1, const float3 *pPos = 0);

//! отрисовка всех моделей, на основе информации о видимости из массива видимости idVisCalcObj
SX_LIB_API void SGeom_Render(
	DWORD timeDelta,		//!< время рендера кадра в млсек
	GEOM_RENDER_TYPE type,	//!< рисовать ли полупрозрачные модели?
	ID idVisCalcObj = SX_GEOM_DEFAULT_VISCALCOBJ, //!< идентификатор массива с информацией о видимости
	ID idExcludeModel = -1,	//!< идентификатор модели которую необходимо исключить из рендера (на случай отрисовки отражений)
	ID idExcludeGroup = -1	//!< идентификатор подгруппы которую надо исключить из рендера, либо указать значение меньше 0, тогда отсекется целая модель с id idExcludeGroup	
	);

/*! добавить модель
 \warning возвращаемый идентификатор не является константным. При удалении другой модели, которая в списке находится выше, идентификатор текущей модели будет смещен!
Иными словами: id модели не постоянен, если удалить модель из списка то все элементы после id станут на единицу меньше
*/
SX_LIB_API ID SGeom_ModelAdd(
	const char *szPath,		//!< путь до модели, относительно стандартного пути
	const char *szName,		//!< имя модели
	const char *szLod,		//!< путь до лода, относительно стандартного пути
	const char *szPhys,		//!< путь до физической модели, относительно стандартного пути
	bool needSegmentation	//!< нужно ли делить модель?
	);

//! копировать модель по ее id, вернет id копии
SX_LIB_API ID SGeom_ModelCopy(ID idModel);

//! удалить модель под номером id
SX_LIB_API void SGeom_ModelDelete(ID idModel);	

//! получить общий ограничивающий объем всего списка моделей
SX_LIB_API void SGeom_GetMinMax(float3 *pMin, float3 *pMax);	

//**************************************************************************

/*! \name Объекты расчетов видимости
 \warning Каждый вызов #SGeom_ModelsAddVisCaclObj создает новый объект расчетов видимости, который занимает определенное количества памяти, которое зависит от объема текущего списка моделей и их делений.\n
Поэтому безпричинные вызовы данной функции не допустимы.\n
Если идентификатор созданный #SGeom_ModelsAddVisCaclObj больше не актуален (это определяет только программист), то необходимо освободить память которую он занимает через функцию #SGeom_ModelsDelVisCaclObj\n
Идентификаторы возвращаемые #SGeom_ModelsAddVisCaclObj константны, и не будут удалены или сдвинуты пока сам программист этого не сделает, либо в случае перезагрузки
@{*/

//! добавить массив просчетов, возвращает его идентификатор
SX_LIB_API ID SGeom_VisCaclObjAdd();

//! существует ли массив просчетов с номером id
SX_LIB_API bool SGeom_VisCaclObjExists(ID idVisCaclObj);

//! удалить массив просчетов с номером id_arr
SX_LIB_API void SGeom_VisCaclObjDelete(ID idVisCaclObj);

//!@}

//**************************************************************************

/*! \name Управление моделями
@{*/

//! получить минимум и максимум модели
SX_LIB_API void SGeom_ModelGetMinMax(ID idModel, float3 *pMin, float3 *pMax);	

//! получить имя модели
SX_LIB_API const char* SGeom_ModelGetName(ID idModel, char *szName=0);		

SX_LIB_API void SGeom_ModelSetName(ID idModel, const char *szName);

//! получить путь до модели относительно стандартного пути
SX_LIB_API const char* SGeom_ModelGetPath4Model(ID idModel);	

//! получить количество полигонов модели
SX_LIB_API int SGeom_ModelGetCountPoly(ID idModel);		


//! получить позицию модели
SX_LIB_API const float3* SGeom_ModelGetPosition(ID idModel, float3 *pPos=0);

//! установить позицию модели
SX_LIB_API void SGeom_ModelSetPosition(ID idModel, const float3 *pPos);

//! получить повороты модели
SX_LIB_API const float3* SGeom_ModelGetRotation(ID idModel, float3 *pRot=0);

//! установить повороты модели
SX_LIB_API void SGeom_ModelSetRotation(ID idModel, const float3 *pRot);

//! получить масштабирование модели
SX_LIB_API const float3* SGeom_ModelGetScale(ID idModel, float3 *pScale=0);

//! установить масштабирование модели
SX_LIB_API void SGeom_ModelSetScale(ID idModel, const float3 *pScale);


//! получить путь до лода
SX_LIB_API const char* SGeom_ModelGetPath4Lod(ID idModel);

//! установить лод
SX_LIB_API void SGeom_ModelSetPath4Lod(ID idModel, const char *szPath);

//! получить путь до физической модели
SX_LIB_API const char* SGeom_ModelGetPath4Physics(ID idModel);

//! установить физическую модель
SX_LIB_API void SGeom_ModelSetPath4Physics(ID idModel, const char *szPath);

//!@}

//**************************************************************************

//! сортировка полупрозрачных моделей front to back относительно позиции наблюдателя pViewPos
SX_LIB_API void SGeom_SortTransparent(const float3 *pViewPos);

//**************************************************************************

/*! \name Данные моделей
Каждая подгруппа есть определенный материал и имеет:
 - центр
 - точки экстремума (минимум, максимум)
 - плоскость, которая строится по первому треугольнику в подгруппе
@{*/

//! количество подгрупп в модели
SX_LIB_API ID SGeom_ModelGetCountGroups(ID idModel);

//! идентификатор материала подгруппы с номером idGroup в модели		
SX_LIB_API ID SGeom_ModelGetGroupMtrlID(ID idModel, ID idGroup);

//! записывает в pCenter центр подгруппы idGroup
SX_LIB_API void SGeom_ModelGetGroupCenter(ID idModel, ID idGroup, float3_t *pCenter);

//! записывает в pPlane плоскость подгруппы idGroup
SX_LIB_API void SGeom_ModelGetGroupPlane(ID idModel, ID idGroup, SMPLANE *pPlane);

//! записывает максимум и минимум подгруппы idGroup
SX_LIB_API void SGeom_ModelGetGroupMinMax(ID idModel, ID idGroup, float3_t *pMin, float3_t *pMax);

//! сегментирована ли модель
SX_LIB_API bool SGeom_ModelGetSegmentation(ID idModel);

//!@}

//**************************************************************************

/*! получить все данные о физических характеристиках для навигации из всего того, что на данный момент содержится в списке моделей
 \note Функция сама инициализирует все данные, кроме pCountModels, в pCountModels функция только запишет число. Для очистки памяти - SGeom_ModelsClearArrBuffsGeom
*/
SX_LIB_API void SGeom_GetArrBuffsGeom(
	float3_t ***pppVertices,	//!< (*arr_vertex)[num_model][num_vertex] - вершины модели
	int32_t **ppCountVertices,	//!< (*arr_count_vertex)[num_model] - количество вершин для модели
	uint32_t ***pppIndeces,		//!< (*arr_index)[num_model][num_vertex] - индексы модели
	ID ***pppMtl,				//!< (*arr_mtl)[num_model][num_vertex] - материал для индекса
	int32_t **ppCountIndeces,	//!< (*arr_count_index)[num_model] - количество индексов для модели
	int32_t *pCountModels		//!< (*count_model) - количество моделей
	);

/*! Очищает папять, выделенную в SGeom_ModelsGetArrBuffsGeom
*/
SX_LIB_API void SGeom_ClearArrBuffsGeom(
	float3_t **ppVertices,		//!< arr_vertex[num_model][num_vertex] - вершины модели
	int32_t *pCountVertices,	//!< arr_count_vertex[num_model] - количество вершин для модели
	uint32_t **ppIndeces,		//!< arr_index[num_model][num_vertex] - индексы модели
	ID **ppMtl,					//!< (*arr_mtl)[num_model][num_vertex] - материал для индекса
	int32_t *pCountIndeces,		//!< arr_count_index[num_model] - количество индексов для модели
	int32_t CountModels			//!< count_model - количество моделей
	);

//! трассировка луча, из pStart в направлении pDir, в случае пересечения возвращает true, иначе false, в pRes записывает координаты пересечения если есть
SX_LIB_API bool SGeom_TraceBeam(
	const float3 *pStart,	//!< стартовая точка луча
	const float3 *pDir,		//!< направление луча
	float3 *pResult,		//!< если есть пересечение запишет координаты
	ID *pIDmodel,			//!< если есть пересечение запишет id модели
	ID *pIDmtrl				//!< если есть пересечение запишет id материала
	);

//! трассировка луча, из pStart в pEnd, в случае пересечения возвращает true, иначе false, в pRes записывает координаты пересечения если есть
SX_LIB_API bool SGeom_TraceBeamId(
	ID idModel,             //!< id модели для проверки
	const float3 *pStart,	//!< стартовая точка луча
	const float3 *pEnd,		//!< направление луча
	float3 *pResult,		//!< если есть пересечение запишет координаты
	ID *pIDmtrl				//!< если есть пересечение запишет id материала
	);

//!@} sxgeom_models

#endif

//!@} sxgeom
