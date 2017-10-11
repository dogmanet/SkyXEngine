
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

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

#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>

#ifdef SX_DLL
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

//##########################################################################

/*! \name Стандартные функции библиотеки
@{*/

//! возвращает версию подсистемы геометрии
SX_LIB_API long SGeom_0GetVersion();			

//! установить новую функцию вывода сообщений
SX_LIB_API void SGeom_Dbg_Set(report_func rf);	

//! инициализация подсистемы
SX_LIB_API void SGeom_0Create(	
	const char* name,			//!< имя
	bool is_unic = true			//!< должна ли подсистема быть уникальной по имени
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
SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackSplits(bool val);	

//! установлена ли сортировка в моделях front to back по кускам деления
SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackSplits();			

//!@}

/*! \name Растительность
@{*/

//! получить дистанцию окончания нулевого лода (лода травы) и начало первого лода
SX_LIB_API float SGeom_0SettGreenGetDistLods1();			

//! установить дистанцию окончания нулевого лода (лода травы) и начало первого лода
SX_LIB_API void SGeom_0SettGreenSetDistLods1(float val);	


//! получить дистанцию окончания первого лода и начало второго лода
SX_LIB_API float SGeom_0SettGreenGetDistLods2();			

//!< установить дистанцию окончания первого лода и начало второго лода
SX_LIB_API void SGeom_0SettGreenSetDistLods2(float val);		


//! получить текущую частоту (густоту) травы, от 0 до 100
SX_LIB_API int SGeom_0SettGreenGetFreqGrass();				

//! установить текущую частоту (густоту) травы, от 0 до 100
SX_LIB_API void SGeom_0SettGreenSetFreqGrass(int val);		


//! получить начало уменьшения травы, пределы от 0 до DistLods1
SX_LIB_API float SGeom_0SettGreenGetBeginEndLessening();		

//! установить начало уменьшения травы, пределы от 0 до DistLods1
SX_LIB_API void SGeom_0SettGreenSetBeginEndLessening(float val);


//! установлена ли сортировка front to back по кускам деления
SX_LIB_API bool SGeom_0SettGreenGetSortFrontToBackSplits();			

//! установить сортировку front to back по кускам деления
SX_LIB_API void SGeom_0SettGreenSetSortFrontToBackSplits(bool val);	

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
SX_LIB_API void SGeom_ModelsSave(const char* path);	

//! загрузить бинарный файл с моделями
SX_LIB_API void SGeom_ModelsLoad(const char* path);	

//! количество моделей в текущем списке
SX_LIB_API long SGeom_ModelsGetCount();	

//! просчитать видимость всех моделей для фрустума 
SX_LIB_API void SGeom_ModelsComVisible(	
	ISXFrustum* frustum,	//!< фрустум для которого считаем видимость моделей
	float3* viewpos,		//!< позиция источника фрустума чтобы просчитать дистанцию
	ID id_arr = 0			//!< идентификатор массива информации о видимости для фрустума, создается через #SGeom_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);	

//! есть ли сорт материала sort который просчитан к отрисовке в массиве просчетов id_arr
SX_LIB_API bool SGeom_ModelsSortExistsForRender(
	int sort, 		//!< искомый сорт
	ID id_arr = 0	//!< идентификатор массива информации о видимости для фрустума, создается через #SGeom_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);

//! отрисовка всех моделей, на основе информации о видимости из массива видимости id_arr
SX_LIB_API void SGeom_ModelsRender(
	DWORD timeDelta,			//!< время рендера кадра в млсек
	int sort_mtl,				//!< сорт материала, для ранжирования геометрии по "сортам", по умолчанию 0
	ID id_arr = 0,				//!< идентификатор массива с информацией о видимости
	bool is_sorted = false,		//!< использовать ли информацию о сортировке подгрупп front to back
	ID exclude_model_id = -1,	//!< идентификатор модели которую необходимо исключить из рендера (на случай отрисовки отражений)
	ID exclude_group_id = -1	//!< идентификатор подгруппы которую надо исключить из рендера, либо указать значение меньше 0, тогда отсекется целая модель с id exclude_model_id	
	);

//! индивидуальная отрисовка модели, на основе информации о видимости из массива видимости id_arr = 0
SX_LIB_API void SGeom_ModelsRenderSingly(
	DWORD timeDelta,//!< время рендера кадра в млсек
	ID id,			//!< идентификатор модели
	ID id_tex = -1	//!< идентификатор материала, который необходимо установить модели, либо -1 чтобы установить ее собственные
	);

/*! добавить модель
 \warning возвращаемый идентификатор не является константным. При удалении другой модели, которая в списке находится выше, идентификатор текущей модели будет смещен!
Иными словами: id модели не постоянен, если удалить модель из списка то все элементы после id станут на единицу меньше
*/
SX_LIB_API ID SGeom_ModelsAddModel(
	const char* path,	//!< путь до модели, относительно стандартного пути
	const char* lod1,	//!< путь до лода, относительно стандартного пути
	const char* name	//!< имя модели
	);

/*! удалить модель под номером id
 \warning Поверхностные тесты не дали ошибок, однако внутренняя организация хранения данных чрезвычайно сложна (по нашему мнению), поэтому могут быть ошибки, в том числе и критические, но пока обнаружить их не удалось
*/
SX_LIB_API void SGeom_ModelsDelModel(ID id);	

//! получить общий ограничивающий объем всего списка моделей
SX_LIB_API void SGeom_ModelsGetMinMax(float3* min, float3* max);	

//**************************************************************************

/*! \name Массивы просчетов видимости
 \warning Каждый вызов #SGeom_ModelsAddArrForCom создает новый элемент массива просчета, который занимает определенное количества памяти, которое зависит от объема текущего списка моделей и их делений.\n
Поэтому без причинные вызовы данной функции не допустимы.\n
Если идентификатор созданный #SGeom_ModelsAddArrForCom больше не актуален (это определяет только программист) то его необходимо освободить память которую он занимает через функцию #SGeom_ModelsDelArrForCom\n
Идентификаторы возвращаемые #SGeom_ModelsAddArrForCom константны, и не будут удалены или сдвинуты пока сам программист этого не сделает, либо в случае перезагрузки
@{*/

//! добавить массив просчетов, возвращает его идентификатор
SX_LIB_API ID SGeom_ModelsAddArrForCom();				

//! удалить массив просчетов с номером id_arr
SX_LIB_API void SGeom_ModelsDelArrForCom(ID id_arr);	

//!@}

//**************************************************************************

/*! \name Управление моделями по id - порядковым номерам моделей
 \note Функции #SGeom_ModelsMGetPosition #SGeom_ModelsMGetRotation #SGeom_ModelsMGetScale возвращают указатели, поэтому после получения вектора его уже можно менять, для того чтобы изменения вступили в силу надо вызывать #SGeom_ModelsMApplyTransform
@{*/

//! получить минимум и максимум модели
SX_LIB_API void SGeom_ModelsMGetMinMax(ID id, float3* min, float3* max);	

//! получить имя модели
SX_LIB_API char* SGeom_ModelsMGetName(ID id);			

//! получить путь до модели относительно стандартного пути
SX_LIB_API const char* SGeom_ModelsMGetPathName(ID id);	

//! получить количество полигонов модели
SX_LIB_API long SGeom_ModelsMGetCountPoly(ID id);		


//! получить позицию модели
SX_LIB_API float3* SGeom_ModelsMGetPosition(ID id);	

//! получить повороты модели
SX_LIB_API float3* SGeom_ModelsMGetRotation(ID id);	

//! получить масштабирование модели
SX_LIB_API float3* SGeom_ModelsMGetScale(ID id);	


//! получить путь до лода относительно стандартного пути
SX_LIB_API const char* SGeom_ModelsMGetLodPath(ID id);				

//! установить путь до лода относительно стандартного пути и загрузить
SX_LIB_API void SGeom_ModelsMSetLodPath(ID id, const char* path);	


//! применить трансформации для модели
SX_LIB_API void SGeom_ModelsMApplyTransform(ID id);	

//!@}

//**************************************************************************

/*! сортировка front to back 
 \note Рендер по этой сортировке возможен только индивидуально для каждой  подгруппы модели. В данной версии сделано для поддержки полупрозрачных поверхностей.
*/
SX_LIB_API void SGeom_ModelsMSortGroups(
	float3* viewpos,	//!< позиция наблюдателя относительно которой будет произведена сортировка
	int sort_mtl		//!< сорт материала для которого будет произведена сортировка
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
SX_LIB_API ID SGeom_ModelsMGetCountGroups(ID id);			

//! идентификатор материала подгруппы с номером group в модели		
SX_LIB_API ID SGeom_ModelsMGetGroupIDMat(ID id, ID group);			

//! записвыает в center центр подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupCenter(ID id, ID group, float3_t* center);	

//! записвыает в min минимум (минимальную позицию) подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupMin(ID id, ID group, float3_t* min);		

//! записвыает в max максимум (максимальную позицию) центр подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupMax(ID id, ID group, float3_t* max);		

//! записвыает в plane плоскость подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupPlane(ID id, ID group, D3DXPLANE* plane);	

//!@}

//**************************************************************************

/*! получить все данные о физических характеристиках для навигации из всего того что на данный момент содержится в списке моделей
 \note Функция сама инициализирует все данные, кроме count_models, в count_models функция только запишет число. Для очистки памяти - SGeom_ModelsClearArrBuffsGeom
*/
SX_LIB_API void SGeom_ModelsGetArrBuffsGeom(
	float3_t*** arr_vertex,		//!< (*arr_vertex)[num_model][num_vertex] - вершины модели
	int32_t** arr_count_vertex, //!< (*arr_count_vertex)[num_model] - количество вершин для модели
	uint32_t*** arr_index,		//!< (*arr_index)[num_model][num_vertex] - индексы модели
	ID*** arr_mtl,				//!< (*arr_mtl)[num_model][num_vertex] - материал для индекса
	int32_t** arr_count_index,	//!< (*arr_count_index)[num_model] - количество индексов для модели
	int32_t* count_models		//!< (*count_model) - количество моделей
	);

/*! Очищает папять, выделенную в SGeom_ModelsGetArrBuffsGeom
*/
SX_LIB_API void SGeom_ModelsClearArrBuffsGeom(
	float3_t** arr_vertex,		//!< arr_vertex[num_model][num_vertex] - вершины модели
	int32_t* arr_count_vertex,	//!< arr_count_vertex[num_model] - количество вершин для модели
	uint32_t** arr_index,		//!< arr_index[num_model][num_vertex] - индексы модели
	ID** arr_mtl,				//!< (*arr_mtl)[num_model][num_vertex] - материал для индекса
	int32_t* arr_count_index,	//!< arr_count_index[num_model] - количество индексов для модели
	int32_t count_models		//!< count_model - количество моделей
	);

SX_LIB_API bool SGeom_ModelsTraceBeam(float3* start, float3* dir, float3* _res, ID* idmodel, ID* idmtl);

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

//! типы растительности
enum GeomGreenType
{
	ggtr_all = -1,	//!< используется только для указания отрисовки всех типов и только в рендере
	
	ggt_grass = 0,	//!< трава
	ggt_tree,		//!< деревья
};

//! структура трансформаций растительности
struct GreenDataVertex
{
	float3_t Position;	//!< позиция
	float3_t TexCoord;	//!< x - общий масштаб,y - поворот по оси y, z -
	float2_t SinCosRot;	//!< синус и косинус (угол TexCoord.y) для поворота
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
	const char* name,		//!< имя
	const char* path_mask,	//!< путь до маски по которой будет генерироваться растительность
	float count_max,		//!< коэфициент на который будет умножена плотность из маски
	const char* path,		//!< путь до файла с моделью для 0 лода
	const char* lod1,		//!< путь до файла с моделью для 1 лода
	const char* lod2,		//!< путь до файла с моделью для 2 лода
	const char* navmesh		//!< навигационный меш, который будет передан в функции #SGeom_GreenGetMNavMeshAndTransform как основной по которому нужно считать перемещение
	);

//! добавить объект растительности
SX_LIB_API ID SGeom_GreenAddObject(
	ID id,			//!< идентификатор растительности к которой добавляем
	float3* pos,	//!< позиция в которой будет находится объект
	ID* idsplit		//!< запишет идентификатор сплита, в котором был добавлен объект
	);

//! удалить объект растительности
SX_LIB_API void SGeom_GreenDelObject(
	ID id,		//!< идентификатор растительности
	ID idsplit,	//!< идентификатор слпита из которого надо удалить
	ID idobj	//!< идентификатор объекта который надо удалить
	);

//! в pos записывает позицию объекта растительности
SX_LIB_API void SGeom_GreenGetPosObject(
	ID id,			//!< идентификатор растительности
	ID idsplit,		//!< идентификатор слпита
	ID idobj,		//!< идентификатор объекта
	float3_t* pos	//!< вектор в который будет записана позиция
	);

/*! устанавливает позицию объекта растительности
 \note Так как новая позиция объекта растительности может выходит за пределы текущего сплита и может входить в другой сплит, то указатели idsplit и idobj могут быть изменены после вызова данной функции
*/
SX_LIB_API void SGeom_GreenSetPosObject(
	ID id,			//!< идентификатор растительности
	ID* idsplit,	//!< идентификатор слпита
	ID* idobj,		//!< идентификатор объекта
	float3_t* pos	//!< пновая позиция объекта растительности
	);

//! удалить единицу растительности
SX_LIB_API void SGeom_GreenDelGreen(ID id);	

//**************************************************************************

/*! \name Массивы просчетов видимости
 \warning Каждый вызов #SGeom_GreenAddArrForCom создает новый элемент массива просчета, который занимает определенное количества памяти, которое зависит от объема текущего списка растительности и их делений.\n
Поэтому без причинные вызовы данной функции не допустимы.\n
Если идентификатор созданный #SGeom_GreenAddArrForCom больше не актуален (это определяет только программист) то его необходимо освободить память которую он занимает через функцию #SGeom_GreenDelArrForCom\n
Идентификаторы возвращаемые #SGeom_GreenAddArrForCom константны, и не будут удалены или сдвинуты пока сам программист этого не сделает, либо в случае перезагрузки
@{*/

//! добавить массив просчетов, возвращает его идентификатор
SX_LIB_API ID SGeom_GreenAddArrForCom();			

//! удалить массив просчетов
SX_LIB_API void SGeom_GreenDelArrForCom(ID id_arr);	

//!@}

//**************************************************************************

//! сохранить информацию о растительности в бинарный файл
SX_LIB_API void SGeom_GreenSave(const char* path);	

//! загрузить информацию о растительности из бинарного файла
SX_LIB_API void SGeom_GreenLoad(const char* path);	

//! очистить список растительности
SX_LIB_API void SGeom_GreenClear();	

//! просчитать видимость всей растительности для фрустума
SX_LIB_API void SGeom_GreenComVisible(
	ISXFrustum* frustum,	//!< фрустум для которого считаем видимость моделей
	float3* viewpos,		//!< позиция источника фрустума чтобы просчитать дистанцию
	ID id_arr = 0			//!< идентификатор массива информации о видимости для фрустума, создается через #SGeom_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);

//! отрисовка растительности
SX_LIB_API void SGeom_GreenRender(
	DWORD timeDelta,	//!< время рендера кадра
	float3* viewpos,	//!< позиция наблюдателя (для которого будет рендер)
	GeomGreenType type, //!< тип рисуемой растительности из перечисления GeomGreenType, указав GeomGreenType::ggtr_all будут отрисованы все типы растительности
	ID id_arr = 0		//!< идентификатор массива просчетов видимости (создается #SGeom_GreenAddArrForCom)
	);

//! индивидуальная отрисовка растительности по идентификатору, массив просчетов по умолчанию (0)
SX_LIB_API void SGeom_GreenRenderSingly(
	DWORD timeDelta,	//!< время рендера кадра
	float3* viewpos,	//!< позиция наблюдателя (для которого будет рендер)
	ID id,				//!< идентификатор растительности
	ID id_tex = -1		//!< идентификатор материала, который будет применен ко всем объектам и подгруппам растительности, -1 если надо использовать назначенные материалы
	);

//! индивидуальная отрисовка растительности по идентификатору, массив просчетов по умолчанию (0)
SX_LIB_API void SGeom_GreenRenderObject(
	DWORD timeDelta,	//!< время рендера кадра
	float3* viewpos,	//!< позиция наблюдателя (для которого будет рендер)
	ID id,				//!< идентификатор растительности
	ID split,			//!< идентификатор сплита (в котором находится объект)
	ID idobj,			//!< идентификатор объекта (в сплите)
	ID id_tex = -1		//!< идентификатор материала, который будет применен ко всем объектам и подгруппам растительности, -1 если надо использовать назначенные материалы
	);

//!< получить количество единиц растительности (по видам есесно)
SX_LIB_API long SGeom_GreenGetCount();	

//**************************************************************************

/*! \name Управление единицами растительности
 \note id - идентификатор (порядковый номер в массиве растительности) единицы растительности
@{*/

//! получить имя
SX_LIB_API char* SGeom_GreenMGetName(ID id);		

//! получить количество сгенерированных элементов
SX_LIB_API long SGeom_GreenMGetCountGen(ID id);		

//! получить количество в модели (0 лод)
SX_LIB_API long SGeom_GreenMGetCountPoly(ID id);	

//! получить тип растительности
SX_LIB_API int SGeom_GreenMGetTypeCountGen(ID id);	

//! получить путь до модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetModel(ID id);	

//! получить путь до первого лода модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetLod1(ID id);	

//! получить путь до второго лода модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetLod2(ID id);	

//! получить путь до маски по которой сгенерирована растительность, относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetMask(ID id);	

//! получить путь до лода навигации относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetNav(ID id);	

//! установить лод (для рендера)
SX_LIB_API void SGeom_GreenMSetLod(
	ID id,					//!< идентификатор единицы растительности
	int lod,				//!< номер лода от 0 до 2 включительно
	const char* pathname	//!< путь до модели относительно стандартного пути
	);	
SX_LIB_API void SGeom_GreenMSetNav(ID id, const char* pathname);	//!< установить лод навигации, pathname путь до модели относительно стандартного пути

//!@}

//**************************************************************************

/*! получить все данные о физических характеристиках для навигации из всего того что на данный момент содержит растительность и только того что иметт навигационные модели
 \note Все данные кроме arr_count_green функция инициализирует сама, в arr_count_green запишет число
*/
SX_LIB_API void SGeom_GreenGetNavMeshAndTransform(
	float3_t*** arr_vertex,			//!< (*arr_vertex)[num_green_mesh_nav][num_vertex] - вершины модели
	int32_t** arr_count_vertex,		//!< (*arr_count_vertex)[num_green_mesh_nav] - количество вершин для модели
	uint32_t*** arr_index,			//!< (*arr_index)[num_green_mesh_nav][num_vertex] - индексы модели
	ID*** arr_mtl,					//!< (*arr_mtl)[num_green_mesh_nav][num_vertex] - материал для индекса
	int32_t** arr_count_index,		//!< (*arr_count_index)[num_green_mesh_nav] - количество индексов для модели
	GreenDataVertex*** arr_transform,//!< (*arr_transform)[num_green_mesh_nav][num_obj] - трансформации модели навигации
	int32_t** arr_count_transform,	//!< (*arr_count_transform)[num_green_mesh_nav] - количество матриц для трансформаций
	int32_t* arr_count_green		//!< (*arr_count_green) - количество единиц растительности по видам
	);

/*! Очищает папять, выделенную в SGeom_GreenGetNavMeshAndTransform
*/
SX_LIB_API void SGeom_GreenClearNavMeshAndTransform(
	float3_t** arr_vertex,			//!< (*arr_vertex)[num_green_mesh_nav][num_vertex] - вершины модели
	int32_t* arr_count_vertex,		//!< (*arr_count_vertex)[num_green_mesh_nav] - количество вершин для модели
	uint32_t** arr_index,			//!< (*arr_index)[num_green_mesh_nav][num_vertex] - индексы модели
	ID** arr_mtl,					//!< (*arr_mtl)[num_green_mesh_nav][num_vertex] - материал для индекса
	int32_t* arr_count_index,		//!< (*arr_count_index)[num_green_mesh_nav] - количество индексов для модели
	GreenDataVertex** arr_transform,//!< (*arr_transform)[num_type_green][num_elem] - трансформации модели навигации
	int32_t* arr_count_transform,	//!< (*arr_count_transform)[num_type_green] - количество матриц для трансформаций
	int32_t arr_count_green			//!< (*arr_count_green) - количество единиц растительности по видам
	);

//! трасировка луча, возвращает "было ли пересечение луча и растительности (по объектно)"
SX_LIB_API bool SGeom_GreenTraceBeam(
	float3* start,	//!< позиция испускания луча
	float3* dir,	//!< направление взгляда луча
	float3* _res,	//!< запишется в какой позиции было персечение (если было)
	ID* idgreen,	//!< запишется идентификатор растительности
	ID* idsplits,	//!< запишется идентификатор сплита
	ID* idobj,		//!< запишется идентификатор объекта
	ID* idmtl		//!< запишется идентификатор материала
	);

//! есть ли входящая в объем растительность (ее объекты), при первом попавшемся полигоне который входит в объем будет возвращено true
SX_LIB_API bool SGeom_GreenGetOccurencessLeafGrass(
	float3* bbmin,	//!< минимум
	float3* bbmax,	//!< максимум
	int physic_mtl	//!< физический тип материала (#SGCore_MtlGetPhysicType) который будет проверятся на вхождение
	);

//!@} sxgeom_green

#endif

//!@} sxgeom
