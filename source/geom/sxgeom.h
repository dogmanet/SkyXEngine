
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxgeom - библиотеки базовой орагнизации статической геометрии и растительности
*/

/*! \defgroup sxgeom sxgeom - библиотека с базовой организацией статической геометрии и растительности
 \note Поддерживается quad (4) и octo (8) деление моделей, определние необходимости в делении осуществлятся внутренним механизмом
 \note В основе просчетов видимости для фрустумов лежит идея о том что желаетельно распаралеллить просчет видимости и рендер.\n
Поэтому есть возможность создать дополнительные массивы для записи просчетов видимости для конкретного фрустума.\n
Эти просчеты можно вынести в отдельный поток, и после того как просчет будет сделан можно рисовать на основании информации из этого массива.\n
Примеры в подмодулях.
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
SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackSplits(bool canSort);	

//! установлена ли сортировка в моделях front to back по кускам деления
SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackSplits();			

//!@}

/*! \name Растительность
@{*/

//! получить дистанцию окончания нулевого лода (лода травы) и начало первого лода
SX_LIB_API float SGeom_0SettGreenGetDistLods1();			

//! установить дистанцию окончания нулевого лода (лода травы) и начало первого лода
SX_LIB_API void SGeom_0SettGreenSetDistLods1(float fDist);	


//! получить дистанцию окончания первого лода и начало второго лода
SX_LIB_API float SGeom_0SettGreenGetDistLods2();			

//!< установить дистанцию окончания первого лода и начало второго лода
SX_LIB_API void SGeom_0SettGreenSetDistLods2(float fDist);		


//! получить текущую частоту (густоту) травы, от 0 до 100
SX_LIB_API int SGeom_0SettGreenGetFreqGrass();				

//! установить текущую частоту (густоту) травы, от 0 до 100
SX_LIB_API void SGeom_0SettGreenSetFreqGrass(int iFreq);		


//! получить начало уменьшения травы, пределы от 0 до DistLods1
SX_LIB_API float SGeom_0SettGreenGetBeginEndLessening();		

//! установить начало уменьшения травы, пределы от 0 до DistLods1
SX_LIB_API void SGeom_0SettGreenSetBeginEndLessening(float fBless);


//! установлена ли сортировка front to back по кускам деления
SX_LIB_API bool SGeom_0SettGreenGetSortFrontToBackSplits();			

//! установить сортировку front to back по кускам деления
SX_LIB_API void SGeom_0SettGreenSetSortFrontToBackSplits(bool canSort);	

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

//!@} sxgeom

//#############################################################################

/*! \defgroup sxgeom_green Растительность
 \ingroup sxgeom
 \note Перед размещением растительности на уровне необходимо создать сам уровнень состоящий из статических моделей, к которым и будет физически привязыватсья растительность
 \note Для каждого элемента из списка растительности создаются ограничивающие объемы которые строятся иерархично от большего к меньшему по площади всего уровня, эти объемы имеют внутренние ограничения
 \note Рендер растительности основан на hardware instancing
 \note Рендер единицы растительности происходит на основании кусков, поэтому некоторые эффекты присущие идинчным экзмеплярам рендера будут ошибочные (к примеру отражения)
 \note В любой момент времени, при наличии в загруженных моделей, можно пройти по списку моделей циклом, начиная с 0 и до #SGeom_GreenGetCount
 \note Пример распаралеливания просчетов видимости и рендера:
 \code
 ID id_arr_cam = SGeom_GreenAddArrForCom();
 ID id_arr_shadow = SGeom_GreenAddArrForCom();
 ID id_arr_light = SGeom_GreenAddArrForCom();
 //...
 SGeom_GreenComVisible(frustum_cam,pos_cam,id_arr_cam);
 SGeom_GreenComVisible(frustum_shadow,pos_shadow,id_arr_shadow);
 SGeom_GreenComVisible(frustum_light,pos_light,id_arr_light);
 //...
 SGeom_GreenRender(timeDelta, viewpos, type, id_arr_cam);
 SGeom_GreenRender(timeDelta, viewpos, type, id_arr_shadow);
 SGeom_GreenRender(timeDelta, viewpos, type, id_arr_light);
 \endcode
 \note Модели, лоды, лоды навигации, сохраняются в виде относительных путей, маски и текстуры сохраняются в виде имен
 \todo Описать формат файла с сохраненным списком растительности
 \todo Добавить возможность единичного добавления в массив растительности (мышкой тыкаем в окне где должно быть дерево например и оно там появляется)
 \todo Добавить возможность единичного удаления из массива растительности
@{
*/

#define GREEN_MAX_ELEM_IN_DIP	512000	/*!< максимальнео количество объектов для единовременной отрисовки для одного вида растительности */
#define GREEN_BB_MIN_X			10.f	/*!< минимальный размер сплита по ширине */
#define GREEN_BB_MIN_Z			10.f	/*!< минимальный размер сплита по длине */
#define GREEN_GEN_RAND_SCALE	1.f		/*!< рандомный масштаб модели при генерации (0, 1 + GREEN_GEN_RAND_SCALE) */
#define GREEN_GEN_RAND_ROTATE_Y	SM_2PI	/*!< рандомный поворот (в радианах) модели при генерации (0, 1 + GREEN_GEN_RAND_ROTATE_Y) */

typedef bool(*g_phy_intersect)(const float3 *pStart, const float3 *pFinish, float3 *pResult);

SX_LIB_API void SGeom_GreenSetFncIntersect(g_phy_intersect fnFunc);

//! типы растительности
enum GREEN_TYPE
{
	GREEN_TYPE_ALL = -1,	//!< используется только для указания отрисовки всех типов и только в рендере
	
	GREEN_TYPE_GRASS = 0,	//!< трава
	GREEN_TYPE_TREE,		//!< деревья
};

//! структура трансформаций растительности
struct CGreenDataVertex
{
	float3_t m_vPosition;	//!< позиция
	float3_t m_vTexCoord;	//!< x - общий масштаб, y - поворот по оси y, z -
	float2_t m_vSinCosRot;	//!< синус и косинус (угол TexCoord.y) для поворота
};

//**************************************************************************

/*! добавить единицу растительности
 \note Теперь добавить растительность можно по пустой маске (то есть без маски вообще), указав в path_mask NULL, для добавления объектов растительности в элемент списка растительность можно воспользоваться #SGeom_GreenAddObject
 \note path, lod1, lod2 - пути относительно стандартного пути, до модели и лодов, 
если lod1 и lod2 равны 0, тогда будет добавлен элемент трава, иначе элемент дерево.
Лоды рисуются на определенных расстояниях
 \note При генерации происходит проверка на столкновение растительности со всей геометрией (lock/unlock буферов ... другого способа не нашли)
 \note Информация о месте генерации растительности берется из маски текстуры path_mask (которая должна быть сохранена в dds формат A8R8G8B8) на основании значения альфа канала. Количество генерируемых объектов в текущем пикселе поределяется по формуле:\n
\code
//alpha от 0 - 255
//count_max - аргумент функции #SGeom_GreenAddGreen
int count = int(floor((alpha*count_max * 100) / 2550.f));
\endcode
В целом текстура представляет из себя весь уровень по ширине и длине, таким образом каждый пиксель текстуры это определенное место на уровне:\n
\code
//bbmin и bbmax - точки экстремума, ограничивающий параллелепипед всей геометрии
//x_tex и y_tex - координаты текущего пикселя в текстуре
//width_tex и height_tex - ширина и высота текстуры
//просчет позиции только по оси xz
float2 pixel_position = lerpf(bbmin.xz, bbmax.xz, float2(x_tex + 1, y_tex + 1) / float2(width_tex,height_tex));
\endcode
и описывает определенный периметр:\n
\code
//width_level и depth_level - ширина и длина всей геометрии
//x_tex и y_tex шинира и высота текстуры в пикселях
float2 pixel_perimeter = float2(width_level,length_level) / float2(x_tex,y_tex);
\endcode
Таким образом чем больше текстура в пикселях размера уровня в метрах тем меньший периметр описывает один пиксель и тем большую плотность можно сделать и наоборот.
 \note При генерации деревьев идет проверка на отсутсвие других деревьев в радиусе вписанной окружности bound box'а дерева по оси xz
 \warning Возвращаемый идентификатор не является константным. При удалении другой единицы, которая в списке находится выше, идентификатор текущей будет смещен!
Иными словами: id растительности не постоянен, если удалить растительность из списка то все элементы после id станут на единицу меньше
*/
SX_LIB_API ID SGeom_GreenAddGreen(
	const char *szName,			//!< имя
	const char *szPathMask,		//!< путь до маски по которой будет генерироваться растительность
	float fCountMax,			//!< коэфициент на который будет умножена плотность из маски
	const char *szPathLod0,		//!< путь до файла с моделью для 0 лода
	const char *szPathLod1,		//!< путь до файла с моделью для 1 лода
	const char *szPathLod2,		//!< путь до файла с моделью для 2 лода
	const char *szNavMesh		//!< навигационный меш, который будет передан в функции #SGeom_GreenGetMNavMeshAndTransform как основной по которому нужно считать перемещение
	);

//! добавить объект растительности
SX_LIB_API ID SGeom_GreenAddObject(
	ID idGreen,		//!< идентификатор растительности к которой добавляем
	float3 *vPos,	//!< позиция в которой будет находится объект
	ID *pIDsplit	//!< запишет идентификатор сплита, в котором был добавлен объект
	);

//! удалить объект растительности
SX_LIB_API void SGeom_GreenDelObject(
	ID idGreen,	//!< идентификатор растительности
	ID idSplit,	//!< идентификатор слпита из которого надо удалить
	ID idObj	//!< идентификатор объекта который надо удалить
	);

//! в pos записывает позицию объекта растительности
SX_LIB_API void SGeom_GreenGetPosObject(
	ID idGreen,		//!< идентификатор растительности
	ID idSplit,		//!< идентификатор слпита
	ID idObj,		//!< идентификатор объекта
	float3_t *pPos	//!< вектор в который будет записана позиция
	);

/*! устанавливает позицию объекта растительности
 \note Так как новая позиция объекта растительности может выходит за пределы текущего сплита и может входить в другой сплит, то указатели idsplit и idobj могут быть изменены после вызова данной функции
*/
SX_LIB_API void SGeom_GreenSetPosObject(
	ID idGreen,		//!< идентификатор растительности
	ID* idSplit,	//!< идентификатор слпита
	ID* idObj,		//!< идентификатор объекта
	float3_t *pPos	//!< пновая позиция объекта растительности
	);

//! удалить единицу растительности
SX_LIB_API void SGeom_GreenDelGreen(ID idGreen);

//**************************************************************************

/*! \name Массивы просчетов видимости
 \warning Каждый вызов #SGeom_GreenAddArrForCom создает новый элемент массива просчета, который занимает определенное количества памяти, которое зависит от объема текущего списка растительности и их делений.\n
Поэтому без причинные вызовы данной функции не допустимы.\n
Если идентификатор созданный #SGeom_GreenAddArrForCom больше не актуален (это определяет только программист) то его необходимо освободить память которую он занимает через функцию #SGeom_GreenDelArrForCom\n
Идентификаторы возвращаемые #SGeom_GreenAddArrForCom константны, и не будут удалены или сдвинуты пока сам программист этого не сделает, либо в случае перезагрузки
@{*/

//! добавить массив просчетов, возвращает его идентификатор
SX_LIB_API ID SGeom_GreenAddArrForCom();			

//! существует ли массив просчетов с номером id
SX_LIB_API bool SGeom_GreenExistsArrForCom(ID idGreen);

//! удалить массив просчетов
SX_LIB_API void SGeom_GreenDelArrForCom(ID idArr);	

//!@}

//**************************************************************************

//! сохранить информацию о растительности в бинарный файл
SX_LIB_API void SGeom_GreenSave(const char *szPath);	

//! загрузить информацию о растительности из бинарного файла
SX_LIB_API void SGeom_GreenLoad(const char *szPath);	

//! очистить список растительности
SX_LIB_API void SGeom_GreenClear();	

//! просчитать видимость всей растительности для фрустума
SX_LIB_API void SGeom_GreenComVisible(
	const IFrustum *pFrustum,	//!< фрустум для которого считаем видимость моделей
	float3 *pViewPos,			//!< позиция источника фрустума чтобы просчитать дистанцию
	ID idArr = 0				//!< идентификатор массива информации о видимости для фрустума, создается через #SGeom_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);

//! отрисовка растительности
SX_LIB_API void SGeom_GreenRender(
	DWORD timeDelta,		//!< время рендера кадра
	float3 *pViewPos,		//!< позиция наблюдателя (для которого будет рендер)
	GREEN_TYPE typeGreen,	//!< тип рисуемой растительности из перечисления GeomGreenType, указав GeomGreenType::ggtr_all будут отрисованы все типы растительности
	ID idArr = 0			//!< идентификатор массива просчетов видимости (создается #SGeom_GreenAddArrForCom)
	);

//! индивидуальная отрисовка растительности по идентификатору, массив просчетов по умолчанию (0)
SX_LIB_API void SGeom_GreenRenderSingly(
	DWORD timeDelta,	//!< время рендера кадра
	float3 *pViewPos,	//!< позиция наблюдателя (для которого будет рендер)
	ID idGreen,			//!< идентификатор растительности
	ID idTex = -1		//!< идентификатор материала, который будет применен ко всем объектам и подгруппам растительности, -1 если надо использовать назначенные материалы
	);

//! индивидуальная отрисовка растительности по идентификатору, массив просчетов по умолчанию (0)
SX_LIB_API void SGeom_GreenRenderObject(
	DWORD timeDelta,	//!< время рендера кадра
	float3 *pViewPos,	//!< позиция наблюдателя (для которого будет рендер)
	ID idGreen,			//!< идентификатор растительности
	ID idSplit,			//!< идентификатор сплита (в котором находится объект)
	ID idObj,			//!< идентификатор объекта (в сплите)
	ID idTex = -1		//!< идентификатор материала, который будет применен ко всем объектам и подгруппам растительности, -1 если надо использовать назначенные материалы
	);

//!< получить количество единиц растительности (по видам есесно)
SX_LIB_API int SGeom_GreenGetCount();	

//**************************************************************************

/*! \name Управление единицами растительности
 \note id - идентификатор (порядковый номер в массиве растительности) единицы растительности
@{*/

//! получить имя
SX_LIB_API char* SGeom_GreenMGetName(ID idGreen);

//! получить количество сгенерированных элементов
SX_LIB_API int SGeom_GreenMGetCountGen(ID idGreen);

//! получить количество в модели (0 лод)
SX_LIB_API int SGeom_GreenMGetCountPoly(ID idGreen);

//! получить тип растительности
SX_LIB_API int SGeom_GreenMGetTypeCountGen(ID idGreen);

//! получить путь до модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetModel(ID idGreen);

//! получить путь до первого лода модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetLod1(ID idGreen);

//! получить путь до второго лода модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetLod2(ID idGreen);

//! получить путь до маски по которой сгенерирована растительность, относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetMask(ID idGreen);

//! получить путь до лода навигации относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetNav(ID idGreen);

//! установить лод (для рендера)
SX_LIB_API void SGeom_GreenMSetLod(
	ID idGreen,				//!< идентификатор единицы растительности
	int iLod,				//!< номер лода от 0 до 2 включительно
	const char *szPathName	//!< путь до модели относительно стандартного пути
	);	

SX_LIB_API void SGeom_GreenMSetNav(ID id, const char *szPathName);	//!< установить лод навигации, pathname путь до модели относительно стандартного пути

//!@}

//**************************************************************************

/*! получить все данные о физических характеристиках для навигации из всего того что на данный момент содержит растительность и только того что иметт навигационные модели
 \note Все данные кроме arr_count_green функция инициализирует сама, в arr_count_green запишет число
*/
SX_LIB_API void SGeom_GreenGetNavMeshAndTransform(
	float3_t ***pppVertices,			//!< (*arr_vertex)[num_green_mesh_nav][num_vertex] - вершины модели
	int32_t **ppCountVertices,			//!< (*arr_count_vertex)[num_green_mesh_nav] - количество вершин для модели
	uint32_t ***pppIndeces,				//!< (*arr_index)[num_green_mesh_nav][num_vertex] - индексы модели
	ID ***pppMtl,						//!< (*arr_mtl)[num_green_mesh_nav][num_vertex] - материал для индекса
	int32_t **ppCountIndeces,			//!< (*arr_count_index)[num_green_mesh_nav] - количество индексов для модели
	CGreenDataVertex ***pppTransforms,	//!< (*arr_transform)[num_green_mesh_nav][num_obj] - трансформации модели навигации
	int32_t **ppCountTransforms,		//!< (*arr_count_transform)[num_green_mesh_nav] - количество матриц для трансформаций
	int32_t *pCountGreens				//!< (*arr_count_green) - количество единиц растительности по видам
	);

/*! Очищает папять, выделенную в SGeom_GreenGetNavMeshAndTransform
*/
SX_LIB_API void SGeom_GreenClearNavMeshAndTransform(
	float3_t **ppVertices,			//!< (*arr_vertex)[num_green_mesh_nav][num_vertex] - вершины модели
	int32_t *pCountVertices,		//!< (*arr_count_vertex)[num_green_mesh_nav] - количество вершин для модели
	uint32_t **ppIndeces,			//!< (*arr_index)[num_green_mesh_nav][num_vertex] - индексы модели
	ID **ppMtl,						//!< (*arr_mtl)[num_green_mesh_nav][num_vertex] - материал для индекса
	int32_t *pCountIndeces,			//!< (*arr_count_index)[num_green_mesh_nav] - количество индексов для модели
	CGreenDataVertex **ppTransforms,//!< (*arr_transform)[num_type_green][num_elem] - трансформации модели навигации
	int32_t *pCountTransforms,		//!< (*arr_count_transform)[num_type_green] - количество матриц для трансформаций
	int32_t iCountGreens			//!< (*arr_count_green) - количество единиц растительности по видам
	);

//! трасировка луча, возвращает "было ли пересечение луча и растительности (по объектно)"
SX_LIB_API bool SGeom_GreenTraceBeam(
	float3 *pStart,	//!< позиция испускания луча
	float3 *pDir,	//!< направление взгляда луча
	float3 *pRes,	//!< запишется в какой позиции было персечение (если было)
	ID *pIdGreen,	//!< запишется идентификатор растительности
	ID *pIdSplits,	//!< запишется идентификатор сплита
	ID *pIdObj,		//!< запишется идентификатор объекта
	ID *pIdMtl		//!< запишется идентификатор материала
	);

//! есть ли входящая в объем растительность (ее объекты), при первом попавшемся полигоне который входит в объем будет возвращено true
SX_LIB_API bool SGeom_GreenGetOccurencessLeafGrass(
	float3 *pMin,	//!< минимум
	float3 *pMax,	//!< максимум
	int iPhysicMtl	//!< физический тип материала (#SGCore_MtlGetPhysicType) который будет проверятся на вхождение
	);

//!@} sxgeom_green

#endif

//!@} sxgeom
