
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

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

//##########################################################################

/*! \name Стандартные функции библиотеки
@{*/

//! возвращает версию подсистемы геометрии
SX_LIB_API long SGeom_0GetVersion();			

//! установить новую функцию вывода сообщений
SX_LIB_API void SGeom_Dbg_Set(report_func fnFunc);	

//! инициализация подсистемы
SX_LIB_API void SGeom_0Create(	
	const char *szName,		//!< имя
	bool isUnic = true		//!< должна ли подсистема быть уникальной по имени
	);

//! уничтожение подсистемы
SX_LIB_API void SGeom_AKill();	

//!@}

//**************************************************************************

/*! \name Обработка потери/восстановления устройства
@{*/

//! обработка потери устройства
SX_LIB_API void SGeom_OnLostDevice();	

//! обработка восстановления устройства
SX_LIB_API void SGeom_OnResetDevice();	

//!@}

//##########################################################################

/*! \defgroup sxgeom_set Функции настроек, предварительная организация подсистемы необязательна
 \ingroup sxgeom
@{*/

/*! \name Модели
@{*/

//SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackModels(bool val);	//unsupported
//SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackModels();			//unsupported

//! установка сортировки в моделях front to back по кускам деления
SX_LIB_API void SGeom_0SettSetSortFrontToBackSplits(bool canSort);	

//! установлена ли сортировка в моделях front to back по кускам деления
SX_LIB_API bool SGeom_0SettGetSortFrontToBackSplits();			

//!@}
//!@} sxgeom_set

//##########################################################################

/*! \defgroup sxgeom_models Статическая геометрия
 \ingroup sxgeom
 \note Вся геометрия состояит из набора подгрупп. То есть в данной реализации не существует отдельного понятия модель, но есть некоторая его интерпретация.\n\n
По умолчанию рендер моделей осуществляется целой подгруппой со всеми входящими в нее моделями за один вызов DIP, но есть возможность раздельного рендера подгрупп каждой модели в единой подгруппе.
Это возможно за счет функции #SGCore_MtlGroupRenderIsSingly которая вызывается для каждой загружаемой подгруппы модели.
 \note В любой момент времени, при наличии в загруженных моделей, можно пройти по списку моделей циклом, начиная с 0 и до #SGeom_ModelsGetCount
 \note Пример распаралеливания просчетов видимости и рендера:
\code
ID id_arr_cam = SGeom_ModelsAddArrForCom();
ID id_arr_shadow = SGeom_ModelsAddArrForCom();
ID id_arr_light = SGeom_ModelsAddArrForCom();
//...
SGeom_ModelsComVisible(frustum_cam,pos_cam,id_arr_cam);
SGeom_ModelsComVisible(frustum_shadow,pos_shadow,id_arr_shadow);
SGeom_ModelsComVisible(frustum_light,pos_light,id_arr_light);
//...
SGeom_ModelsRender(timeDelta, type, id_arr_cam);
SGeom_ModelsRender(timeDelta, type, id_arr_shadow);
SGeom_ModelsRender(timeDelta, type, id_arr_light);
\endcode
 \note Лоды сохраняются в виде относительных путей, текстуры в виде имен
 \todo Описать формат файла с сохраненным списком моделей
@{
*/

#define GEOM_MAX_POLY_IN_GROUP					400000	/*!< максимальное количество полигонов на одну подгруппу */
#define GEOM_DIFFERENCE_SIDES_FOR_OCTO			0.3		/*!< минимальная разница между сторонами для окто деления */
#define GEOM_MIN_ALLVOLUME_FOR_SEGMENTATION		20		/*!< минимальный общий объем модели для деления */
#define GEOM_MIN_LENGTH_FOR_SEGMENTATION		10		/*!< минимальный длина по горизонтальной оси модели для деления */
#define GEOM_FORCE_ALLVOLUME_FOR_SEGMENTATION	150		/*!< минимальный длина по горизонтальной оси модели для деления */
#define GEOM_MIN_HEIGHT_FOR_SEGMENTATION		14		/*!< минимальный высота модели для деления окто */
#define GEOM_MIN_POLYGONS_FOR_SEGMENTATION		5000	/*!< минимальнео количество полигонов в модели для деления */
#define GEOM_MIN_COUNT_POLY						500		/*!< минимальное количество полигонов в сплите */
#define GEOM_MAX_COUNT_POLY						1000	/*!< максимальное количество полигонов в сплите */

//**************************************************************************
	
//! очистить список всех моделей
SX_LIB_API void SGeom_ModelsClear();	

//! сохранить все модели в бинарный файл
SX_LIB_API void SGeom_ModelsSave(const char *szPath);	

//! загрузить бинарный файл с моделями
SX_LIB_API void SGeom_ModelsLoad(const char *szPath);	

//! количество моделей в текущем списке
SX_LIB_API int SGeom_ModelsGetCount();	

//! просчитать видимость всех моделей для фрустума 
SX_LIB_API void SGeom_ModelsComVisible(	
	const IFrustum *pFrustum,	//!< фрустум для которого считаем видимость моделей
	const float3 *pViewPos,		//!< позиция источника фрустума чтобы просчитать дистанцию
	ID idArr = 0				//!< идентификатор массива информации о видимости для фрустума, создается через #SGeom_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);	

//! есть ли сорт материала iSort который просчитан к отрисовке в массиве просчетов idArr
SX_LIB_API bool SGeom_ModelsSortExistsForRender(
	int iSort, 		//!< искомый сорт
	ID idArr = 0	//!< идентификатор массива информации о видимости для фрустума, создается через #SGeom_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);

//! отрисовка всех моделей, на основе информации о видимости из массива видимости idArr
SX_LIB_API void SGeom_ModelsRender(
	DWORD timeDelta,		//!< время рендера кадра в млсек
	int iSort,				//!< сорт материала, для ранжирования геометрии по "сортам", по умолчанию 0
	ID idArr = 0,			//!< идентификатор массива с информацией о видимости
	bool canSorted = false,	//!< использовать ли информацию о сортировке подгрупп front to back
	ID idExcludeModel = -1,	//!< идентификатор модели которую необходимо исключить из рендера (на случай отрисовки отражений)
	ID idExcludeGroup = -1	//!< идентификатор подгруппы которую надо исключить из рендера, либо указать значение меньше 0, тогда отсекется целая модель с id exclude_model_id	
	);

//! индивидуальная отрисовка модели, на основе информации о видимости из массива видимости idArr = 0
SX_LIB_API void SGeom_ModelsRenderSingly(
	DWORD timeDelta,//!< время рендера кадра в млсек
	ID idModel,		//!< идентификатор модели
	ID idMtl = -1	//!< идентификатор материала, который необходимо установить модели, либо -1 чтобы установить ее собственные
	);

/*! добавить модель
 \warning возвращаемый идентификатор не является константным. При удалении другой модели, которая в списке находится выше, идентификатор текущей модели будет смещен!
Иными словами: id модели не постоянен, если удалить модель из списка то все элементы после id станут на единицу меньше
*/
SX_LIB_API ID SGeom_ModelsAddModel(
	const char *szPath,	//!< путь до модели, относительно стандартного пути
	const char *szLod1,	//!< путь до лода, относительно стандартного пути
	const char *szName	//!< имя модели
	);

/*! удалить модель под номером id
 \warning Поверхностные тесты не дали ошибок, однако внутренняя организация хранения данных чрезвычайно сложна (по нашему мнению), поэтому могут быть ошибки, в том числе и критические, но пока обнаружить их не удалось
*/
SX_LIB_API void SGeom_ModelsDelModel(ID idModel);	

//! получить общий ограничивающий объем всего списка моделей
SX_LIB_API void SGeom_ModelsGetMinMax(float3 *pMin, float3 *pMax);	

//**************************************************************************

/*! \name Массивы просчетов видимости
 \warning Каждый вызов #SGeom_ModelsAddArrForCom создает новый элемент массива просчета, который занимает определенное количества памяти, которое зависит от объема текущего списка моделей и их делений.\n
Поэтому без причинные вызовы данной функции не допустимы.\n
Если идентификатор созданный #SGeom_ModelsAddArrForCom больше не актуален (это определяет только программист) то его необходимо освободить память которую он занимает через функцию #SGeom_ModelsDelArrForCom\n
Идентификаторы возвращаемые #SGeom_ModelsAddArrForCom константны, и не будут удалены или сдвинуты пока сам программист этого не сделает, либо в случае перезагрузки
@{*/

//! добавить массив просчетов, возвращает его идентификатор
SX_LIB_API ID SGeom_ModelsAddArrForCom();				

//! существует ли массив просчетов с номером id
SX_LIB_API bool SGeom_ModelsExistsArrForCom(ID idArr);

//! удалить массив просчетов с номером id_arr
SX_LIB_API void SGeom_ModelsDelArrForCom(ID idArr);	

//!@}

//**************************************************************************

/*! \name Управление моделями по id - порядковым номерам моделей
 \note Функции #SGeom_ModelsMGetPosition #SGeom_ModelsMGetRotation #SGeom_ModelsMGetScale возвращают указатели, поэтому после получения вектора его уже можно менять, для того чтобы изменения вступили в силу надо вызывать #SGeom_ModelsMApplyTransform
@{*/

//! получить минимум и максимум модели
SX_LIB_API void SGeom_ModelsMGetMinMax(ID idModel, float3 *pMin, float3 *pMax);	

//! получить имя модели
SX_LIB_API char* SGeom_ModelsMGetName(ID idModel);			

//! получить путь до модели относительно стандартного пути
SX_LIB_API const char* SGeom_ModelsMGetPathName(ID idModel);	

//! получить количество полигонов модели
SX_LIB_API int SGeom_ModelsMGetCountPoly(ID idModel);		


//! получить позицию модели
SX_LIB_API float3* SGeom_ModelsMGetPosition(ID idModel);	

//! получить повороты модели
SX_LIB_API float3* SGeom_ModelsMGetRotation(ID idModel);

//! получить масштабирование модели
SX_LIB_API float3* SGeom_ModelsMGetScale(ID idModel);


//! получить путь до лода относительно стандартного пути
SX_LIB_API const char* SGeom_ModelsMGetLodPath(ID idModel);

//! установить путь до лода относительно стандартного пути и загрузить
SX_LIB_API void SGeom_ModelsMSetLodPath(ID idModel, const char *szPath);


//! применить трансформации для модели
SX_LIB_API void SGeom_ModelsMApplyTransform(ID idModel);

//!@}

//**************************************************************************

/*! сортировка front to back 
 \note Рендер по этой сортировке возможен только индивидуально для каждой  подгруппы модели. В данной версии сделано для поддержки полупрозрачных поверхностей.
*/
SX_LIB_API void SGeom_ModelsMSortGroups(
	const float3 *pViewPos,	//!< позиция наблюдателя относительно которой будет произведена сортировка
	int iSortMtl			//!< сорт материала для которого будет произведена сортировка
	);

//**************************************************************************

/*! \name Данные моделей
 \note id - идентификатор (порядковый номер) модели, который возвращается функцией #SGeom_ModelsAddModel\n
Каждая подгруппа есть определнный материал и имеет:
 - центр
 - точки экстремума (минимум, максимум)
 - плоскость, которая строится по первому треугольнику в подгруппе
@{*/

//! количество подгрупп в модели
SX_LIB_API ID SGeom_ModelsMGetCountGroups(ID idModel);

//! идентификатор материала подгруппы с номером group в модели		
SX_LIB_API ID SGeom_ModelsMGetGroupIDMat(ID idModel, ID idGroup);

//! записвыает в center центр подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupCenter(ID idModel, ID idGroup, float3_t *pCenter);

//! записвыает в min минимум (минимальную позицию) подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupMin(ID idModel, ID idGroup, float3_t *pMin);

//! записвыает в max максимум (максимальную позицию) центр подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupMax(ID idModel, ID idGroup, float3_t *pMax);

//! записвыает в plane плоскость подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupPlane(ID idModel, ID idGroup, D3DXPLANE *pPlane);

//!@}

//**************************************************************************

/*! получить все данные о физических характеристиках для навигации из всего того что на данный момент содержится в списке моделей
 \note Функция сама инициализирует все данные, кроме count_models, в count_models функция только запишет число. Для очистки памяти - SGeom_ModelsClearArrBuffsGeom
*/
SX_LIB_API void SGeom_ModelsGetArrBuffsGeom(
	float3_t ***pppVertices,	//!< (*arr_vertex)[num_model][num_vertex] - вершины модели
	int32_t **ppCountVertices,	//!< (*arr_count_vertex)[num_model] - количество вершин для модели
	uint32_t ***pppIndeces,		//!< (*arr_index)[num_model][num_vertex] - индексы модели
	ID ***pppMtl,				//!< (*arr_mtl)[num_model][num_vertex] - материал для индекса
	int32_t **ppCountIndeces,	//!< (*arr_count_index)[num_model] - количество индексов для модели
	int32_t *pCountModels		//!< (*count_model) - количество моделей
	);

/*! Очищает папять, выделенную в SGeom_ModelsGetArrBuffsGeom
*/
SX_LIB_API void SGeom_ModelsClearArrBuffsGeom(
	float3_t **ppVertices,		//!< arr_vertex[num_model][num_vertex] - вершины модели
	int32_t *pCountVertices,	//!< arr_count_vertex[num_model] - количество вершин для модели
	uint32_t **ppIndeces,		//!< arr_index[num_model][num_vertex] - индексы модели
	ID **ppMtl,					//!< (*arr_mtl)[num_model][num_vertex] - материал для индекса
	int32_t *pCountIndeces,		//!< arr_count_index[num_model] - количество индексов для модели
	int32_t CountModels			//!< count_model - количество моделей
	);

//! трассировка луча, из pStart в направлении pDir, в случае пересечения возвращает true, иначе false, в pRes записывает координаты пересечения если есть
SX_LIB_API bool SGeom_ModelsTraceBeam(
	const float3 *pStart,	//!< стартовая точка луча
	const float3 *pDir,		//!< направление луча
	float3 *pRes,			//!< если есть пересечение запишет координаты
	ID *idModel,			//!< если есть пересечение запишет id модели
	ID *idMtl				//!< если есть пересечение запишет id материала
	);

//!@} sxgeom_models

#endif

//!@} sxgeom
