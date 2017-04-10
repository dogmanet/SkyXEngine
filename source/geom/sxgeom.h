
#ifndef __sxgeom
#define __sxgeom

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>

SX_LIB_API long SGeom_0GetVersion();			//возвращает версию подсистемы геометрии
SX_LIB_API void SGeom_Dbg_Set(report_func rf);	//установить новую функцию вывода сообщений
SX_LIB_API void SGeom_0Create(	//инициализация подсистемы
	const char* name,			//имя
	IDirect3DDevice9* device,	//устроство дх
	const char* std_path_mesh,	//путь до папки с моделями (статическая геометрия и растительность, не анимация)
	bool is_unic = true			//должна ли подсистема быть уникальной по имени
	);

SX_LIB_API void SGeom_0CreateKill();	//уничтожение подсистемы

SX_LIB_API void SGeom_OnLostDevice();	//обработка потери устройства
SX_LIB_API void SGeom_OnResetDevice();	//обработка восстановления устройства

//ФУНКЦИИ НАСТРОЕК, ПРЕДВАРИТЕЛЬНАЯ ИНИЦИАЛИЗАЦИЯ ПОДСИСТЕМЫ НЕОБЯЗАТЕЛЬНА
//{{
SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackModels(bool val);	//unsupported
SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackModels();			//unsupported

//сортировка в моделях front to back по кускам деления
SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackSplits(bool val);
SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackSplits();

//растительность
//{
//дистанци окончания нулевого лода (лода травы) и начало первого лода
SX_LIB_API float SGeom_0SettGreenGetDistLods1();
SX_LIB_API void SGeom_0SettGreenSetDistLods1(float val);

//дистанция окончания первого лода и начало второго лода
SX_LIB_API float SGeom_0SettGreenGetDistLods2();
SX_LIB_API void SGeom_SettGreenSetDistLods2(float val);

//текущая частота (густота) травы, от 0 до 100
SX_LIB_API int SGeom_0SettGreenGetFreqGrass();	
SX_LIB_API void SGeom_0SettGreenSetFreqGrass(int val);

//начало уменьешния травы,пределы от 0 до DistLods1
SX_LIB_API float SGeom_0SettGreenGetBeginEndLessening();
SX_LIB_API void SGeom_0SettGreenSetBeginEndLessening(float val);

//сортировка front to back по кускам деления
SX_LIB_API bool SGeom_0SettGreenGetSortFrontToBackSplits();
SX_LIB_API void SGeom_0SettGreenSetSortFrontToBackSplits(bool val);
//}
//}}

/*
В основе просчетов видимости для фрустумов лежит идея о том что желаетельно распаралеллить просчет видимости и рендер.
Поэтому есть возможность создать дополнительные массивы для записи просчетов видимости для конкретного фрустума.
Эти просчеты можно вынести в отдельный поток, и после того как просчет будет сделан можно рисовать на основании информации из этого массива.
Пример:
long id_arr_cam = SGeom_ModelsAddArrForCom();
long id_arr_shadow = SGeom_ModelsAddArrForCom();
long id_arr_light = SGeom_ModelsAddArrForCom();
...
SGeom_ModelsComVisible(frustum_cam,pos_cam,id_arr_cam);
SGeom_ModelsComVisible(frustum_shadow,pos_shadow,id_arr_shadow);
SGeom_ModelsComVisible(frustum_light,pos_light,id_arr_light);
...
SGeom_ModelsRender(timeDelta,id_arr_cam);
SGeom_ModelsRender(timeDelta,id_arr_shadow);
SGeom_ModelsRender(timeDelta,id_arr_light);
*/

//ФУНКЦИИ СТАТИЧЕСКОЙ ГЕОМЕТРИИ, ПРЕДВАРИТЕЛЬНАЯ ИНИЦИАЛИЗАЦИЯ ПОДСИСТЕМЫ ОБЯЗАТЕЛЬНА!!!
//{{
SX_LIB_API void SGeom_ModelsClear();	//очистить список всех моделей

//!лоды сохраняются в виде относительных путей, текстуры в виде имен
SX_LIB_API void SGeom_ModelsSave(const char* path);	//сохранить все модели в бинарный файл
SX_LIB_API void SGeom_ModelsLoad(const char* path);	//загрузить бинарный файл с моделями

SX_LIB_API long SGeom_ModelsGetCount();	//количество моделей в текущем списке

//проситать видимость всех моделей для фрустума 
SX_LIB_API void SGeom_ModelsComVisible(	
	ISXFrustum* frustum,	//фрустум для которого считаем видимость моделей
	float3* viewpos,		//позиция источника фрустума чтобы просчитать дистанцию
	ID id_arr = 0			//идентификатор массива информации о видимости для фрустума, создается через SGeom_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);	

//отрисовка всех моделей, на основе информации о видимости из массива видимости id_arr
SX_LIB_API void SGeom_ModelsRender(
	DWORD timeDelta,			
	int sort_mtl,				//сорт материала, для ранжирования геометрии по "сортам", по умолчанию 0
	ID id_arr = 0,				//идентификатор массива с информацией о видимости
	bool is_sorted = false,		//использовать ли информацию о сортировке подгрупп front to back
	ID exclude_model_id = -1,	//идентификатор модели которую необходимо исключить из рендера (на случай отрисовки отражений0
	ID exclude_group_id = -1	//идентификатор подгруппы которую надо исключить из рендера, либо указать значение меньше 0, тогда отсекется целая модель с id exclude_model_id	
	);	

//добавить модель
SX_LIB_API ID SGeom_ModelsAddModel(
	const char* path,	//путь до модели, относительно стандартного пути
	const char* lod1,	//путь до лода, относительно стандартного пути
	const char* name	//имя модели
	);

SX_LIB_API void SGeom_ModelsDelModel(long id);	//удалить модель под номером id

SX_LIB_API void SGeom_ModelsGetMinMax(float3* min, float3* max);	//получить общий ограничивающий объем всего списка моделей
SX_LIB_API ID SGeom_ModelsAddArrForCom();				//добавить массив просчетов, возвращает его идентификатор
SX_LIB_API void SGeom_ModelsDelArrForCom(ID id_arr);	//удалить массив просчетов с номером id_arr

//управление моделями по id - порядковым номерам моделей
//id модели не поятоянен, если удалить модель из середины списка то все элементы после id станут на единицу меньше
//{
SX_LIB_API char* SGeom_ModelsMGetName(ID id);			//получить имя модели
SX_LIB_API const char* SGeom_ModelsMGetPathName(ID id);	//получить путь до модели относительно стандартного пути
SX_LIB_API long SGeom_ModelsMGetCountPoly(ID id);		//поулчить количество полигонов модели

//после получения вектора его уже можно менять, для того чтобы изменения вступили в силу надо вызывать SGeom_ModelsMApplyTransform(long id)
SX_LIB_API float3* SGeom_ModelsMGetPosition(ID id);	//получить позицию модели
SX_LIB_API float3* SGeom_ModelsMGetRotation(ID id);	//получить повороты модели
SX_LIB_API float3* SGeom_ModelsMGetScale(ID id);	//получить масштабирование модели

SX_LIB_API const char* SGeom_ModelsMGetLodPath(ID id);				//получить путь до лода относительно стандартного пути
SX_LIB_API void SGeom_ModelsMSetLodPath(ID id, const char* path);	//остановить путь до лода относительно стандартного пути и загрузить

SX_LIB_API void SGeom_ModelsMApplyTransform(ID id);	//применить трансформации для модели
//}

SX_LIB_API void SGeom_ModelsMSortGroups(float3* viewpos,int sort_mtl);

SX_LIB_API ID SGeom_ModelsMGetCountGroups(ID id);
SX_LIB_API ID SGeom_ModelsMGetGroupIDMat(ID id, ID group);
SX_LIB_API void SGeom_ModelsMGetGroupCenter(ID id, ID group, float3_t* center);
SX_LIB_API void SGeom_ModelsMGetGroupMin(ID id, ID group, float3_t* min);
SX_LIB_API void SGeom_ModelsMGetGroupMax(ID id, ID group, float3_t* max);
SX_LIB_API void SGeom_ModelsMGetGroupPlane(ID id, ID group, D3DXPLANE* plane);

SX_LIB_API void SGeom_ModelsMGetArrBuffsGeom(
	float3_t*** arr_vertex,		//(*arr_vertex)[num_model][num_vertex] - вершины модели
	int32_t** arr_count_vertex, //(*arr_count_vertex)[num_model] - количество вершин для модели
	uint32_t*** arr_index,		//(*arr_index)[num_model][num_vertex] - индексы модели
	int32_t** arr_count_index,	//(*arr_count_index)[num_model] - количество индексов для модели
	int32_t* count_models		//(*count_model) - количество моделей
	);
//}}

//*************************************************************************************************************************

//ФУНКЦИИ РАСТИТЕЛЬНОСТИ, ПРЕДВАРИТЕЛЬНАЯ ИНИЦИАЛИЗАЦИЯ ПОДСИСТЕМЫ ОБЯЗАТЕЛЬНА!!!
//{{

//типы растительности
enum GeomGreenType
{
	ggtr_all = -1,	//используется только для указания отрисовки всех типов и только в рендере
	
	ggt_grass = 0,	//трава
	ggt_tree,		//деревья
};

//добавить единицу растительности
SX_LIB_API ID SGeom_GreenAddGreen(
	const char* name,		//имя
	const char* path_mask,	//путь до маски по которой будет генерироваться растительность
	float count_max,		//коэфициент на который будет умножена плотность из маски
	const char* path, const char* lod1, const char* lod2,	//пути относительно стандартного пути, до модели и лодов,
															//если lod1 и lod2 равны 0, тогда будет добавлен элемент трава, иначе элемент дерево
															//лоды рисуются на определенных расстояниях
	const char* navmesh		//навигационный меш, который будет передан в функции SGeom_GreenGetMNavMeshAndTransform как основной по которому нужно считать перемещение
	);

SX_LIB_API void SGeom_GreenDelGreen(ID id);	//удалить единицу растительности

SX_LIB_API ID SGeom_GreenAddArrForCom();			//добавить массив просчетов, возвращает его идентификатор
SX_LIB_API void SGeom_GreenDelArrForCom(ID id_arr);	//удалить массив просчетов

//!модели, лоды, лоды навигации, сохраняются в виде относительных путей, маски и текстуры сохраняются в виде имен
SX_LIB_API void SGeom_GreenSave(const char* path);	//сохранить информацию о растительности в бинарный файл
SX_LIB_API void SGeom_GreenLoad(const char* path);	//загрузить информацию о растительности из бинарного файла

SX_LIB_API void SGeom_GreenClear();	//очистить список растительности

//просчитать видимость всей растительности для фрустума
SX_LIB_API void SGeom_GreenComVisible(
	ISXFrustum* frustum,	//фрустум для которого считаем видимость моделей
	float3* viewpos,		//позиция источника фрустума чтобы просчитать дистанцию
	ID id_arr = 0			//идентификатор массива информации о видимости для фрустума, создается через SGeom_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);

//отрисовка всех моделей, на основе информации о видимости из массива видимости id_arr
//type - тип рисуемой растительности из перечисления GeomGreenType
//указав GeomGreenType::ggtr_all будут отрисованы все типы растительности
SX_LIB_API void SGeom_GreenRender(DWORD timeDelta, float3* viewpos, GeomGreenType type, ID id_arr = 0);	

SX_LIB_API long SGeom_GreenGetCount();	//поулчить количество единиц растительности (по видам есесно)

//управление единицами растительности
//{
SX_LIB_API char* SGeom_GreenMGetName(ID id);		//получить имя
SX_LIB_API long SGeom_GreenMGetCountGen(ID id);		//получить количество сгенерированных элементов
SX_LIB_API int SGeom_GreenMGetTypeCountGen(ID id);	//получить тип растительности
SX_LIB_API const char* SGeom_GreenMGetModel(ID id);	//получить путь до модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetLod1(ID id);	//получить путь до первого лода модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetLod2(ID id);	//получить путь до второго лода модели относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetMask(ID id);	//получить путь до маски по которой сгенерирована растительность, относительно стандартного пути
SX_LIB_API const char* SGeom_GreenMGetNav(ID id);		//получить путь до лода навигации относительно стандартного пути

SX_LIB_API void SGeom_GreenMSetLod(ID id, int lod, const char* pathname);	//установить лод (для рендера) lod от 0 до 2 включительно, pathname путь до модели относительно стандартного пути
SX_LIB_API void SGeom_GreenMSetNav(ID id, const char* pathname);	//установить лод навигации, pathname путь до модели относительно стандартного пути
//}

SX_LIB_API void SGeom_GreenGetMNavMeshAndTransform(
	float3_t*** arr_vertex,			//(*arr_vertex)[num_green_mesh_nav][num_vertex] - вершины модели
	int32_t** arr_count_vertex,		//(*arr_count_vertex)[num_green_mesh_nav] - количество вершин для модели
	uint32_t*** arr_index,			//(*arr_index)[num_green_mesh_nav][num_vertex] - индексы модели
	int32_t** arr_count_index,		//(*arr_count_index)[num_green_mesh_nav] - количество индексов для модели
	float4x4*** arr_transform,		//(*arr_transform)[num_type_green][num_elem] - матрица для трансформации модели навигации
	int32_t** arr_count_transform,	//(*arr_count_transform)[num_type_green] - количество матриц для трансформаций
	int32_t* arr_count_green		//(*arr_count_green) - количество единиц растительности по видам
	);

//}}

#endif