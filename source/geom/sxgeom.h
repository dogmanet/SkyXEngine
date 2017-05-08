
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxgeom - библиотеки базовой орагнизации статической геометрии и растительности
*/

/*! \defgroup sxgeom sxgeom - библиотека с базовой организацией статической геометрии и растительности

\note В основе просчетов видимости для фрустумов лежит идея о том что желаетельно распаралеллить просчет видимости и рендер.\n
Поэтому есть возможность создать дополнительные массивы для записи просчетов видимости для конкретного фрустума.\n
Эти просчеты можно вынести в отдельный поток, и после того как просчет будет сделан можно рисовать на основании информации из этого массива.\n
Примеры в подмодулях.
@{
*/

#ifndef __sxgeom
#define __sxgeom

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>

/*! \name Стандартные функции библиотеки
@{*/
SX_LIB_API long SGeom_0GetVersion();			//!< возвращает версию подсистемы геометрии
SX_LIB_API void SGeom_Dbg_Set(report_func rf);	//!< установить новую функцию вывода сообщений
SX_LIB_API void SGeom_0Create(	//!< инициализация подсистемы
	const char* name,			//!< имя
	IDirect3DDevice9* device,	//!< устроство dx
	const char* std_path_mesh,	//!< путь до папки с моделями (статическая геометрия и растительность)
	bool is_unic = true			//!< должна ли подсистема быть уникальной по имени
	);

SX_LIB_API void SGeom_0CreateKill();	//!< уничтожение подсистемы
//!@}

/*! \name Обработка потери/восстановления устройства
@{*/

SX_LIB_API void SGeom_OnLostDevice();	//!< обработка потери устройства
SX_LIB_API void SGeom_OnResetDevice();	//!< обработка восстановления устройства

//!@}

/*! \defgroup sxgeom_set Функции настроек, предварительная организация подсистемы необязательна
 \ingroup sxgeom
@{*/

/*! \name Модели
@{*/

//SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackModels(bool val);	//unsupported
//SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackModels();			//unsupported

SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackSplits(bool val);	//!< установка сортировки в моделях front to back по кускам деления
SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackSplits();			//!< установлена ли сортировка в моделях front to back по кускам деления

//!@}

/*! \name Растительность
@{*/

SX_LIB_API float SGeom_0SettGreenGetDistLods1();			//!< получить дистанцию окончания нулевого лода (лода травы) и начало первого лода
SX_LIB_API void SGeom_0SettGreenSetDistLods1(float val);	//!< установить дистанцию окончания нулевого лода (лода травы) и начало первого лода

SX_LIB_API float SGeom_0SettGreenGetDistLods2();			//!< получить дистанцию окончания первого лода и начало второго лода
SX_LIB_API void SGeom_SettGreenSetDistLods2(float val);		//!< установить дистанцию окончания первого лода и начало второго лода

SX_LIB_API int SGeom_0SettGreenGetFreqGrass();				//!< получить текущую частоту (густоту) травы, от 0 до 100
SX_LIB_API void SGeom_0SettGreenSetFreqGrass(int val);		//!< установить текущую частоту (густоту) травы, от 0 до 100

SX_LIB_API float SGeom_0SettGreenGetBeginEndLessening();		//!< получить начало уменьешния травы,пределы от 0 до DistLods1
SX_LIB_API void SGeom_0SettGreenSetBeginEndLessening(float val);//!< установить начало уменьешния травы,пределы от 0 до DistLods1

SX_LIB_API bool SGeom_0SettGreenGetSortFrontToBackSplits();			//!< установлена ли сортировка front to back по кускам деления
SX_LIB_API void SGeom_0SettGreenSetSortFrontToBackSplits(bool val);	//!< установить сортировку front to back по кускам деления

//!@}
//!@} sxgeom_set

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
 \todo Описать формат файла с сохраненным списком моделей
@{
*/

SX_LIB_API void SGeom_ModelsClear();	//!< очистить список всех моделей

//лоды сохраняются в виде относительных путей, текстуры в виде имен
SX_LIB_API void SGeom_ModelsSave(const char* path);	//!< сохранить все модели в бинарный файл
SX_LIB_API void SGeom_ModelsLoad(const char* path);	//!< загрузить бинарный файл с моделями

SX_LIB_API long SGeom_ModelsGetCount();	//!< количество моделей в текущем списке

//! проситать видимость всех моделей для фрустума 
SX_LIB_API void SGeom_ModelsComVisible(	
	ISXFrustum* frustum,	//!< фрустум для которого считаем видимость моделей
	float3* viewpos,		//!< позиция источника фрустума чтобы просчитать дистанцию
	ID id_arr = 0			//!< идентификатор массива информации о видимости для фрустума, создается через #SGeom_ModelsAddArrForCom, если 0 то считаем в дефолтный
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

SX_LIB_API void SGeom_ModelsGetMinMax(float3* min, float3* max);	//!< получить общий ограничивающий объем всего списка моделей

/*! \name Массивы просчетов видимости
 \warning Каждый вызов #SGeom_ModelsAddArrForCom создает новый элемент массива просчета, который занимает определенное количества памяти, которое зависит от объема текущего списка моделей и их делений.\n
Поэтому без причинные вызовы данной функции не допустимы.\n
Если идентификатор созданный #SGeom_ModelsAddArrForCom больше не актуален (это определяет только программист) то его необходимо освободить память которую он занимает через функцию #SGeom_ModelsDelArrForCom\n
Идентификаторы возвращаемые #SGeom_ModelsAddArrForCom константны, и не будут удалены или сдвинуты пока сам программист этого не сделает, либо в случае перезагрузки
@{*/

SX_LIB_API ID SGeom_ModelsAddArrForCom();				//!< добавить массив просчетов, возвращает его идентификатор
SX_LIB_API void SGeom_ModelsDelArrForCom(ID id_arr);	//!< удалить массив просчетов с номером id_arr

//!@}


/*! \name Управление моделями по id - порядковым номерам моделей
 \note Функции #SGeom_ModelsMGetPosition #SGeom_ModelsMGetRotation #SGeom_ModelsMGetScale возвращают указатели, поэтому после получения вектора его уже можно менять, для того чтобы изменения вступили в силу надо вызывать #SGeom_ModelsMApplyTransform
@{*/

SX_LIB_API char* SGeom_ModelsMGetName(ID id);			//!< получить имя модели
SX_LIB_API const char* SGeom_ModelsMGetPathName(ID id);	//!< получить путь до модели относительно стандартного пути
SX_LIB_API long SGeom_ModelsMGetCountPoly(ID id);		//!< поулчить количество полигонов модели

SX_LIB_API float3* SGeom_ModelsMGetPosition(ID id);	//!< получить позицию модели
SX_LIB_API float3* SGeom_ModelsMGetRotation(ID id);	//!< получить повороты модели
SX_LIB_API float3* SGeom_ModelsMGetScale(ID id);	//!< получить масштабирование модели

SX_LIB_API const char* SGeom_ModelsMGetLodPath(ID id);				//!< получить путь до лода относительно стандартного пути
SX_LIB_API void SGeom_ModelsMSetLodPath(ID id, const char* path);	//!< установить путь до лода относительно стандартного пути и загрузить

SX_LIB_API void SGeom_ModelsMApplyTransform(ID id);	//!< применить трансформации для модели

//!@}

/*! сортировка front to back 
 \note Рендер по этой сортировке возможен только индивидуально для каждой  подгруппы модели. В данной версии сделано для поддержки полупрозрачных поверхностей.
*/
SX_LIB_API void SGeom_ModelsMSortGroups(
	float3* viewpos,	//!< позиция наблюдателя относительно которой будет произведена сортировка
	int sort_mtl		//!< сорт материала для которого будет произведена сортировка
	);

/*! \name Данные моделей
 \note id - идентификатор (порядковый номер) модели, который возвращается функцией #SGeom_ModelsAddModel\n
Каждая подгруппа есть определнный материал и имеет:
 - центр
 - точки экстремума (минимум, максимум)
 - плоскость, которая строится по первому треугольнику в подгруппе
@{*/

SX_LIB_API ID SGeom_ModelsMGetCountGroups(ID id);			//!< количество подгрупп в модели
SX_LIB_API ID SGeom_ModelsMGetGroupIDMat(ID id, ID group);	//!< идентификатор материала подгруппы с номером group в модели				
SX_LIB_API void SGeom_ModelsMGetGroupCenter(ID id, ID group, float3_t* center);	//!< записвыает в center центр подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupMin(ID id, ID group, float3_t* min);		//!< записвыает в min минимум (минимальную позицию) подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupMax(ID id, ID group, float3_t* max);		//!< записвыает в max максимум (максимальную позицию) центр подгруппы group
SX_LIB_API void SGeom_ModelsMGetGroupPlane(ID id, ID group, D3DXPLANE* plane);	//!< записвыает в plane плоскость подгруппы group

//!@}

/*! получить все данные о физических характеристиках для навигации из всего того что на данный момент содержится в списке моделей
 \note Функция сама инициализирует все данные, кроме count_models, в count_models функция только запишет число. Для очистки памяти - SGeom_ModelsClearArrBuffsGeom
*/
SX_LIB_API void SGeom_ModelsGetArrBuffsGeom(
	float3_t*** arr_vertex,		//!< (*arr_vertex)[num_model][num_vertex] - вершины модели
	int32_t** arr_count_vertex, //!< (*arr_count_vertex)[num_model] - количество вершин для модели
	uint32_t*** arr_index,		//!< (*arr_index)[num_model][num_vertex] - индексы модели
	int32_t** arr_count_index,	//!< (*arr_count_index)[num_model] - количество индексов для модели
	int32_t* count_models		//!< (*count_model) - количество моделей
	);

/*! Очищает папять, выделенную в SGeom_ModelsGetArrBuffsGeom
\note Функция сама инициализирует все данные, кроме count_models, в count_models функция только запишет число
*/
SX_LIB_API void SGeom_ModelsClearArrBuffsGeom(
	float3_t** arr_vertex,		//!< arr_vertex[num_model][num_vertex] - вершины модели
	int32_t* arr_count_vertex, //!< arr_count_vertex[num_model] - количество вершин для модели
	uint32_t** arr_index,		//!< arr_index[num_model][num_vertex] - индексы модели
	int32_t* arr_count_index,	//!< arr_count_index[num_model] - количество индексов для модели
	int32_t count_models		//!< count_model - количество моделей
	);

//!@} sxgeom

//#############################################################################

/*! \defgroup sxgeom_green Растительность
 \ingroup sxgeom
 \note Рендер растительности основан на hardware instancing
 \note Рендер единицы растительности происходит на основании кусков, рендер единичной модели исключен, поэтому некоторые эффекты присущие идинчным экзмеплярам рендера будут ошибочные (к примеру отражения)
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
 \todo Описать формат файла с сохраненным списком растительности
 \todo Добавить возможность единичного добавления в массив растительности (мышкой тыкаем в окне где должно быть дерево например и оно там появляется)
 \todo Добавить возможность единичного удаления из массива растительности
@{
*/

//! типы растительности
enum GeomGreenType
{
	ggtr_all = -1,	//!< используется только для указания отрисовки всех типов и только в рендере
	
	ggt_grass = 0,	//!< трава
	ggt_tree,		//!< деревья
};

/*! добавить единицу растительности
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

SX_LIB_API void SGeom_GreenDelGreen(ID id);	//!< удалить единицу растительности

/*! \name Массивы просчетов видимости
 \warning Каждый вызов #SGeom_GreenAddArrForCom создает новый элемент массива просчета, который занимает определенное количества памяти, которое зависит от объема текущего списка растительности и их делений.\n
Поэтому без причинные вызовы данной функции не допустимы.\n
Если идентификатор созданный #SGeom_GreenAddArrForCom больше не актуален (это определяет только программист) то его необходимо освободить память которую он занимает через функцию #SGeom_GreenDelArrForCom\n
Идентификаторы возвращаемые #SGeom_GreenAddArrForCom константны, и не будут удалены или сдвинуты пока сам программист этого не сделает, либо в случае перезагрузки
@{*/

SX_LIB_API ID SGeom_GreenAddArrForCom();			//!< добавить массив просчетов, возвращает его идентификатор
SX_LIB_API void SGeom_GreenDelArrForCom(ID id_arr);	//!< удалить массив просчетов

//!@}

//модели, лоды, лоды навигации, сохраняются в виде относительных путей, маски и текстуры сохраняются в виде имен
SX_LIB_API void SGeom_GreenSave(const char* path);	//!< сохранить информацию о растительности в бинарный файл
SX_LIB_API void SGeom_GreenLoad(const char* path);	//!< загрузить информацию о растительности из бинарного файла

SX_LIB_API void SGeom_GreenClear();	//!< очистить список растительности

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

SX_LIB_API long SGeom_GreenGetCount();	//!< получить количество единиц растительности (по видам есесно)

/*! \name Управление единицами растительности
 \note id - идентификатор (порядковый номер в массиве растительности) единицы растительности
@{
*/
SX_LIB_API char* SGeom_GreenMGetName(ID id);		//!< получить имя
SX_LIB_API long SGeom_GreenMGetCountGen(ID id);		//!< получить количество сгенерированных элементов
SX_LIB_API int SGeom_GreenMGetTypeCountGen(ID id);	//!< получить тип растительности
SX_LIB_API const char* SGeom_GreenMGetModel(ID id);	//!< получить путь до модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetLod1(ID id);	//!< получить путь до первого лода модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetLod2(ID id);	//!< получить путь до второго лода модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetMask(ID id);	//!< получить путь до маски по которой сгенерирована растительность, относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetNav(ID id);	//!< получить путь до лода навигации относительно стандартного пути

//! установить лод (для рендера)
SX_LIB_API void SGeom_GreenMSetLod(
	ID id,					//!< идентификатор единицы растительности
	int lod,				//!< номер лода от 0 до 2 включительно
	const char* pathname	//!< путь до модели относительно стандартного пути
	);	
SX_LIB_API void SGeom_GreenMSetNav(ID id, const char* pathname);	//!< установить лод навигации, pathname путь до модели относительно стандартного пути

//!@}

/*! получить все данные о физических характеристиках для навигации из всего того что на данный момент содержит растительность и только того что иметт навигационные модели
 \note Все данные кроме arr_count_green функция инициализирует сама, в arr_count_green запишет число
*/
SX_LIB_API void SGeom_GreenGetMNavMeshAndTransform(
	float3_t*** arr_vertex,			//!< (*arr_vertex)[num_green_mesh_nav][num_vertex] - вершины модели
	int32_t** arr_count_vertex,		//!< (*arr_count_vertex)[num_green_mesh_nav] - количество вершин для модели
	uint32_t*** arr_index,			//!< (*arr_index)[num_green_mesh_nav][num_vertex] - индексы модели
	int32_t** arr_count_index,		//!< (*arr_count_index)[num_green_mesh_nav] - количество индексов для модели
	float4x4*** arr_transform,		//!< (*arr_transform)[num_type_green][num_elem] - матрица для трансформации модели навигации
	int32_t** arr_count_transform,	//!< (*arr_count_transform)[num_type_green] - количество матриц для трансформаций
	int32_t* arr_count_green		//!< (*arr_count_green) - количество единиц растительности по видам
	);

//!@} sxgeom_green

#endif

//!@} sxgeom