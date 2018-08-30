
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sgreen - библиотеки базовой орагнизации растительности
*/

/*! \defgroup sgreen sgreen - библиотека с базовой организацией растительности
 \note Поддерживается quad (4) деление моделей, определние необходимости в делении осуществлятся внутренним механизмом
@{
*/

#ifndef __SXGREEN_H
#define __SXGREEN_H

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#pragma comment(lib, "sxgeom_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#pragma comment(lib, "sxgeom.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>
#include <geom/sxgeom.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

//##########################################################################

/*! \name Стандартные функции библиотеки
@{*/

//! возвращает версию подсистемы геометрии
SX_LIB_API long SGreen_0GetVersion();			

//! установить новую функцию вывода сообщений
SX_LIB_API void SGreen_Dbg_Set(report_func fnFunc);	

//! инициализация подсистемы
SX_LIB_API void SGreen_0Create(	
	const char *szName,		//!< имя
	bool isUnic = true		//!< должна ли подсистема быть уникальной по имени
	);

//! уничтожение подсистемы
SX_LIB_API void SGreen_AKill();	

//!@}

//**************************************************************************

/*! \name Обработка потери/восстановления устройства
@{*/

//! обработка потери устройства
SX_LIB_API void SGreen_OnLostDevice();	

//! обработка восстановления устройства
SX_LIB_API void SGreen_OnResetDevice();	

//!@}

//##########################################################################

/*! \defgroup sgreen_set Функции настроек, предварительная организация подсистемы необязательна
 \ingroup sgreen
@{*/

/*! \name Растительность
@{*/

//! получить дистанцию окончания нулевого лода (лода травы) и начало первого лода
SX_LIB_API float SGreen_0SettGetDistLods1();			

//! установить дистанцию окончания нулевого лода (лода травы) и начало первого лода
SX_LIB_API void SGreen_0SettSetDistLods1(float fDist);	


//! получить дистанцию окончания первого лода и начало второго лода
SX_LIB_API float SGreen_0SettGetDistLods2();			

//!< установить дистанцию окончания первого лода и начало второго лода
SX_LIB_API void SGreen_0SettSetDistLods2(float fDist);		


//! получить текущую частоту (густоту) травы, от 0 до 100
SX_LIB_API int SGreen_0SettGreenGetFreqGrass();				

//! установить текущую частоту (густоту) травы, от 0 до 100
SX_LIB_API void SGreen_0SettSetFreqGrass(int iFreq);		


//! получить начало уменьшения травы, пределы от 0 до DistLods1
SX_LIB_API float SGreen_0SettGetBeginEndLessening();		

//! установить начало уменьшения травы, пределы от 0 до DistLods1
SX_LIB_API void SGreen_0SettSetBeginEndLessening(float fBless);


//! установлена ли сортировка front to back по кускам деления
SX_LIB_API bool SGreen_0SettGetSortFrontToBackSplits();			

//! установить сортировку front to back по кускам деления
SX_LIB_API void SGreen_0SettSetSortFrontToBackSplits(bool canSort);	

//!@}
//!@} sgreen_set

//#############################################################################

/*! \defgroup sgreen_green Растительность
 \ingroup sgreen
 \note Перед размещением растительности на уровне необходимо создать сам уровнень состоящий из статических моделей, к которым и будет физически привязыватсья растительность
 \note Для каждого элемента из списка растительности создаются ограничивающие объемы которые строятся иерархично от большего к меньшему по площади всего уровня, эти объемы имеют внутренние ограничения
 \note Рендер растительности основан на hardware instancing
 \note Рендер единицы растительности происходит на основании кусков, поэтому некоторые эффекты присущие идинчным экзмеплярам рендера будут ошибочные (к примеру отражения)
 \note В любой момент времени, при наличии в загруженных моделей, можно пройти по списку моделей циклом, начиная с 0 и до #SGreen_GetCount
 \note Пример распаралеливания просчетов видимости и рендера:
 \code
 ID id_arr_cam = SGreen_AddArrForCom();
 ID id_arr_shadow = SGreen_AddArrForCom();
 ID id_arr_light = SGreen_AddArrForCom();
 //...
 SGreen_ComVisible(frustum_cam,pos_cam,id_arr_cam);
 SGreen_ComVisible(frustum_shadow,pos_shadow,id_arr_shadow);
 SGreen_ComVisible(frustum_light,pos_light,id_arr_light);
 //...
 SGreen_Render(timeDelta, viewpos, type, id_arr_cam);
 SGreen_Render(timeDelta, viewpos, type, id_arr_shadow);
 SGreen_Render(timeDelta, viewpos, type, id_arr_light);
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

SX_LIB_API void SGreen_SetFuncIntersect(g_phy_intersect fnFunc);

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
 \note Теперь добавить растительность можно по пустой маске (то есть без маски вообще), указав в path_mask NULL, для добавления объектов растительности в элемент списка растительность можно воспользоваться #SGreen_AddObject
 \note path, lod1, lod2 - пути относительно стандартного пути, до модели и лодов, 
если lod1 и lod2 равны 0, тогда будет добавлен элемент трава, иначе элемент дерево.
Лоды рисуются на определенных расстояниях
 \note При генерации происходит проверка на столкновение растительности со всей геометрией (lock/unlock буферов ... другого способа не нашли)
 \note Информация о месте генерации растительности берется из маски текстуры path_mask (которая должна быть сохранена в dds формат A8R8G8B8) на основании значения альфа канала. Количество генерируемых объектов в текущем пикселе поределяется по формуле:\n
\code
//alpha от 0 - 255
//count_max - аргумент функции #SGreen_AddGreen
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
SX_LIB_API ID SGreen_AddGreen(
	const char *szName,			//!< имя
	const char *szPathMask,		//!< путь до маски по которой будет генерироваться растительность
	float fCountMax,			//!< коэфициент на который будет умножена плотность из маски
	const char *szPathLod0,		//!< путь до файла с моделью для 0 лода
	const char *szPathLod1,		//!< путь до файла с моделью для 1 лода
	const char *szPathLod2,		//!< путь до файла с моделью для 2 лода
	const char *szNavMesh		//!< навигационный меш, который будет передан в функции #SGreen_GetMNavMeshAndTransform как основной по которому нужно считать перемещение
	);

//! добавить объект растительности
SX_LIB_API ID SGreen_AddObject(
	ID idGreen,		//!< идентификатор растительности к которой добавляем
	float3 *vPos,	//!< позиция в которой будет находится объект
	ID *pIDsplit	//!< запишет идентификатор сплита, в котором был добавлен объект
	);

//! удалить объект растительности
SX_LIB_API void SGreen_DelObject(
	ID idGreen,	//!< идентификатор растительности
	ID idSplit,	//!< идентификатор слпита из которого надо удалить
	ID idObj	//!< идентификатор объекта который надо удалить
	);

//! в pos записывает позицию объекта растительности
SX_LIB_API void SGreen_GetPosObject(
	ID idGreen,		//!< идентификатор растительности
	ID idSplit,		//!< идентификатор слпита
	ID idObj,		//!< идентификатор объекта
	float3_t *pPos	//!< вектор в который будет записана позиция
	);

/*! устанавливает позицию объекта растительности
 \note Так как новая позиция объекта растительности может выходит за пределы текущего сплита и может входить в другой сплит, то указатели idsplit и idobj могут быть изменены после вызова данной функции
*/
SX_LIB_API void SGreen_SetPosObject(
	ID idGreen,		//!< идентификатор растительности
	ID* idSplit,	//!< идентификатор слпита
	ID* idObj,		//!< идентификатор объекта
	float3_t *pPos	//!< пновая позиция объекта растительности
	);

//! удалить единицу растительности
SX_LIB_API void SGreen_DelGreen(ID idGreen);

//**************************************************************************

/*! \name Массивы просчетов видимости
 \warning Каждый вызов #SGreen_AddArrForCom создает новый элемент массива просчета, который занимает определенное количества памяти, которое зависит от объема текущего списка растительности и их делений.\n
Поэтому без причинные вызовы данной функции не допустимы.\n
Если идентификатор созданный #SGreen_AddArrForCom больше не актуален (это определяет только программист) то его необходимо освободить память которую он занимает через функцию #SGreen_DelArrForCom\n
Идентификаторы возвращаемые #SGreen_AddArrForCom константны, и не будут удалены или сдвинуты пока сам программист этого не сделает, либо в случае перезагрузки
@{*/

//! добавить массив просчетов, возвращает его идентификатор
SX_LIB_API ID SGreen_AddArrForCom();			

//! существует ли массив просчетов с номером id
SX_LIB_API bool SGreen_ExistsArrForCom(ID idGreen);

//! удалить массив просчетов
SX_LIB_API void SGreen_DelArrForCom(ID idArr);	

//!@}

//**************************************************************************

//! сохранить информацию о растительности в бинарный файл
SX_LIB_API void SGreen_Save(const char *szPath);	

//! загрузить информацию о растительности из бинарного файла
SX_LIB_API void SGreen_Load(const char *szPath);	

//! очистить список растительности
SX_LIB_API void SGreen_Clear();	

//! просчитать видимость всей растительности для фрустума
SX_LIB_API void SGreen_ComVisible(
	const IFrustum *pFrustum,	//!< фрустум для которого считаем видимость моделей
	float3 *pViewPos,			//!< позиция источника фрустума чтобы просчитать дистанцию
	ID idArr = 0				//!< идентификатор массива информации о видимости для фрустума, создается через #SGreen_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);

//! отрисовка растительности
SX_LIB_API void SGreen_Render(
	DWORD timeDelta,		//!< время рендера кадра
	float3 *pViewPos,		//!< позиция наблюдателя (для которого будет рендер)
	GREEN_TYPE typeGreen,	//!< тип рисуемой растительности из перечисления GeomGreenType, указав GeomGreenType::ggtr_all будут отрисованы все типы растительности
	ID idArr = 0			//!< идентификатор массива просчетов видимости (создается #SGreen_AddArrForCom)
	);

//! индивидуальная отрисовка растительности по идентификатору, массив просчетов по умолчанию (0)
SX_LIB_API void SGreen_RenderSingly(
	DWORD timeDelta,	//!< время рендера кадра
	float3 *pViewPos,	//!< позиция наблюдателя (для которого будет рендер)
	ID idGreen,			//!< идентификатор растительности
	ID idTex = -1		//!< идентификатор материала, который будет применен ко всем объектам и подгруппам растительности, -1 если надо использовать назначенные материалы
	);

//! индивидуальная отрисовка растительности по идентификатору, массив просчетов по умолчанию (0)
SX_LIB_API void SGreen_RenderObject(
	DWORD timeDelta,	//!< время рендера кадра
	float3 *pViewPos,	//!< позиция наблюдателя (для которого будет рендер)
	ID idGreen,			//!< идентификатор растительности
	ID idSplit,			//!< идентификатор сплита (в котором находится объект)
	ID idObj,			//!< идентификатор объекта (в сплите)
	ID idTex = -1		//!< идентификатор материала, который будет применен ко всем объектам и подгруппам растительности, -1 если надо использовать назначенные материалы
	);

//!< получить количество единиц растительности (по видам есесно)
SX_LIB_API int SGreen_GetCount();	

//**************************************************************************

/*! \name Управление единицами растительности
 \note id - идентификатор (порядковый номер в массиве растительности) единицы растительности
@{*/

//! получить имя
SX_LIB_API char* SGreen_MGetName(ID idGreen);

//! получить количество сгенерированных элементов
SX_LIB_API int SGreen_MGetCountGen(ID idGreen);

//! получить количество в модели (0 лод)
SX_LIB_API int SGreen_MGetCountPoly(ID idGreen);

//! получить тип растительности
SX_LIB_API int SGreen_MGetTypeCountGen(ID idGreen);

//! получить путь до модели относительно стандартного пути
SX_LIB_API const char* SGreen_MGetModel(ID idGreen);

//! получить путь до первого лода модели относительно стандартного пути
SX_LIB_API const char* SGreen_MGetLod1(ID idGreen);

//! получить путь до второго лода модели относительно стандартного пути
SX_LIB_API const char* SGreen_MGetLod2(ID idGreen);

//! получить путь до маски по которой сгенерирована растительность, относительно стандартного пути
SX_LIB_API const char* SGreen_MGetMask(ID idGreen);

//! получить путь до лода навигации относительно стандартного пути
SX_LIB_API const char* SGreen_MGetNav(ID idGreen);

//! установить лод (для рендера)
SX_LIB_API void SGreen_MSetLod(
	ID idGreen,				//!< идентификатор единицы растительности
	int iLod,				//!< номер лода от 0 до 2 включительно
	const char *szPathName	//!< путь до модели относительно стандартного пути
	);	

SX_LIB_API void SGreen_MSetNav(ID id, const char *szPathName);	//!< установить лод навигации, pathname путь до модели относительно стандартного пути

//!@}

//**************************************************************************

/*! получить все данные о физических характеристиках для навигации из всего того что на данный момент содержит растительность и только того что иметт навигационные модели
 \note Все данные кроме arr_count_green функция инициализирует сама, в arr_count_green запишет число
*/
SX_LIB_API void SGreen_GetNavMeshAndTransform(
	float3_t ***pppVertices,			//!< (*arr_vertex)[num_green_mesh_nav][num_vertex] - вершины модели
	int32_t **ppCountVertices,			//!< (*arr_count_vertex)[num_green_mesh_nav] - количество вершин для модели
	uint32_t ***pppIndeces,				//!< (*arr_index)[num_green_mesh_nav][num_vertex] - индексы модели
	ID ***pppMtl,						//!< (*arr_mtl)[num_green_mesh_nav][num_vertex] - материал для индекса
	int32_t **ppCountIndeces,			//!< (*arr_count_index)[num_green_mesh_nav] - количество индексов для модели
	CGreenDataVertex ***pppTransforms,	//!< (*arr_transform)[num_green_mesh_nav][num_obj] - трансформации модели навигации
	int32_t **ppCountTransforms,		//!< (*arr_count_transform)[num_green_mesh_nav] - количество матриц для трансформаций
	int32_t *pCountGreens				//!< (*arr_count_green) - количество единиц растительности по видам
	);

/*! Очищает папять, выделенную в SGreen_GetNavMeshAndTransform
*/
SX_LIB_API void SGreen_ClearNavMeshAndTransform(
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
SX_LIB_API bool SGreen_TraceBeam(
	float3 *pStart,	//!< позиция испускания луча
	float3 *pDir,	//!< направление взгляда луча
	float3 *pRes,	//!< запишется в какой позиции было персечение (если было)
	ID *pIdGreen,	//!< запишется идентификатор растительности
	ID *pIdSplits,	//!< запишется идентификатор сплита
	ID *pIdObj,		//!< запишется идентификатор объекта
	ID *pIdMtl		//!< запишется идентификатор материала
	);

//! есть ли входящая в объем растительность (ее объекты), при первом попавшемся полигоне который входит в объем будет возвращено true
SX_LIB_API bool SGreen_GetOccurencessLeafGrass(
	float3 *pMin,	//!< минимум
	float3 *pMax,	//!< максимум
	int iPhysicMtl	//!< физический тип материала (#SGCore_MtlGetPhysicType) который будет проверятся на вхождение
	);

//!@} sgreen_green

#endif

//!@} sgreen
