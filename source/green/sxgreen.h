
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sgreen - библиотеки базовой орагнизации растительности
*/

/*! \defgroup sgreen sgreen - библиотека с базовой организацией растительности
 \note Поддерживается quad (4) деление моделей
@{
*/

#ifndef __SXGREEN_H
#define __SXGREEN_H

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

//! магическое слово для записи в бинарный файл растительности, для идентификации
#define SX_GREEN_MAGIC_WORD	"sxgreens"

//! магичесок число для илентификации файла растительности
#define SX_GREEN_MAGIC_NUM	8317697048185370739

//! версия формата файла растительности
#define SX_GREEN_FILE_FORMAT_VERSION 1

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
	bool isUnic = true,		//!< должна ли подсистема быть уникальной по имени
	bool isServerMode = false //!< серверный режим (без графики)
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


//! получить текущую частоту (густоту) травы, (0, 100]
SX_LIB_API int SGreen_0SettGreenGetFreqGrass();				

//! установить текущую частоту (густоту) травы, (0, 100]
SX_LIB_API void SGreen_0SettSetFreqGrass(int iFreq);		


//! получить начало уменьшения травы, пределы от [0, DistLods1]
SX_LIB_API float SGreen_0SettGetBeginEndLessening();		

//! установить начало уменьшения травы, пределы [0, DistLods1]
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
 \note Для каждого элемента из списка растительности создаются ограничивающие объемы, которые строятся иерархично от большего к меньшему по площади всего уровня, эти объемы имеют внутренние ограничения
 \note Рендер растительности основан на hardware instancing
 \note Рендер единицы растительности происходит на основании кусков, поэтому некоторые эффекты присущие единчным экзмеплярам рендера будут ошибочные (к примеру отражения)
 \note В любой момент времени, при наличии в загруженных моделях, можно пройти по списку моделей циклом, начиная с 0 и до #SGreen_GetCount
 \note Пример распаралеливания просчетов видимости и рендера:
 \code
 ID idVisCaclObjCamera = SGreen_AddVisCaclObj();
 ID idVisCaclObjShadow = SGreen_AddVisCaclObj();
 ID idVisCaclObjLight = SGreen_AddVisCaclObj();
 //...
 SGreen_ComVisible(pFrustumCamera, pPosCamera, idVisCaclObjCamera);
 SGreen_ComVisible(pFrustumShadow, pPosShadow, idVisCaclObjShadow);
 SGreen_ComVisible(pFrustumLight, pPosLight, idVisCaclObjLight);
 //...
 SGreen_Render(timeDelta, pViewPos, type, idVisCaclObjCamera);
 SGreen_Render(timeDelta, pViewPos, type, idVisCaclObjShadow);
 SGreen_Render(timeDelta, pViewPos, type, idVisCaclObjLight);
 \endcode
 \note Модели, лоды, лоды навигации, сохраняются в виде относительных путей, маски и текстуры сохраняются в виде имен
 \todo Описать формат файла с сохраненным списком растительности
@{
*/

#define GREEN_MAX_ELEM_IN_DIP	65536	/*!< максимальное количество объектов для единовременной отрисовки для одного вида растительности для одного сплита */
#define GREEN_BB_MIN_X			10.f	/*!< минимальный размер сплита по ширине */
#define GREEN_BB_MIN_Z			10.f	/*!< минимальный размер сплита по длине */
#define GREEN_GEN_RAND_SCALE	1.f		/*!< рандомный масштаб модели при генерации (0, 1 + GREEN_GEN_RAND_SCALE) */
#define GREEN_GEN_RAND_ROTATE_Y	SM_2PI	/*!< рандомный поворот (в радианах) модели при генерации (0, 1 + GREEN_GEN_RAND_ROTATE_Y) */

//! вид функции для обнаружения столкновения для генерации растительности
typedef bool(*g_phy_intersect)(const float3 *pStart, const float3 *pFinish, float3 *pResult);

//! типы растительности
enum GREEN_TYPE
{
	//! используется только для указания отрисовки всех типов и только в рендере
	GREEN_TYPE_ALL = -1,	
	
	//! трава
	GREEN_TYPE_GRASS = 0,	

	//! деревья/кусты
	GREEN_TYPE_TREE,		
};

//! структура трансформаций растительности
struct CGreenDataVertex
{
	//! позиция
	float3_t m_vPosition;	

	//! x - общий масштаб, y - поворот по оси y, z -
	float3_t m_vTexCoord;	

	//! синус и косинус (угол TexCoord.y) для поворота
	float2_t m_vSinCosRot;	
};

//**************************************************************************

//! установить функцию определения столкновения с миром (при генерации)
SX_LIB_API void SGreen_SetFuncIntersect(g_phy_intersect fnFunc);

/*! добавить единицу растительности
 \note Добавить растительность можно по пустой маске (то есть без маски вообще), указав в szPathMask NULL, для добавления объектов растительности в элемент списка растительности можно воспользоваться #SGreen_AddObject
 \note szPathLod0, szPathLod1, szPathLod2 - пути относительно стандартного пути, до модели и лодов, 
если szPathLod1 и szPathLod2 равны 0, тогда будет добавлен элемент трава, иначе элемент дерево.
Лоды рисуются на определенных расстояниях
 \note Информация о месте генерации растительности берется из маски текстуры szPathMask (которая должна быть сохранена в dds формат A8R8G8B8) на основании значения альфа канала.\n
В целом текстура представляет из себя весь уровень по ширине и длине, таким образом каждый пиксель текстуры это определенное место на уровне.
Если размер текстуры в пикселях равен размерам уровня в метрах, тогда один пиксель текстуры будет описывать один метр уровня\n
Количество объект генерируемых на квадратный метр уровня фиксированное в контексте одной генерации ~ 100 объектов на 1 кв. метр * fCountMax? который в пределах [0, 1]. 
Но это условно, чем больше размер текстуры тем больше объектов может быть сгенерировано за один проход.
 \note При генерации деревьев идет проверка на отсутсвие других деревьев в радиусе вписанной окружности bound box'а дерева по оси xz
 \warning Возвращаемый идентификатор не является константным. При удалении другой единицы, которая в списке находится выше, идентификатор текущей будет смещен!
Иными словами: id растительности не постоянен, если удалить растительность из списка то все элементы после id станут на единицу меньше
*/
SX_LIB_API ID SGreen_AddGreen(
	const char *szName,			//!< имя
	const char *szPathMask,		//!< путь до маски? по которой будет генерироваться растительность
	bool shouldAveragedRGB,		//!< true - использовать ли усредненное значение RGB, false - использовать альфа канал
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
 \note Так как новая позиция объекта растительности может выходит за пределы текущего сплита и может входить в другой сплит, то указатели pIdSplit и pIdObj могут быть изменены после вызова данной функции
*/
SX_LIB_API void SGreen_SetPosObject(
	ID idGreen,		//!< идентификатор растительности
	ID *pIdSplit,	//!< идентификатор сплита
	ID *pIdObj,		//!< идентификатор объекта
	float3_t *pPos	//!< новая позиция объекта растительности
	);

//! удалить единицу растительности
SX_LIB_API void SGreen_DelGreen(ID idGreen);

//**************************************************************************

/*! \name Объекты расчетов видимости
 \warning Каждый вызов #SGreen_AddVisCaclObj создает новый объект расчетов, который занимает определенное количества памяти, которое зависит от объема текущего списка растительности.\n
Поэтому без причинные вызовы данной функции не допустимы.\n
Если идентификатор созданный #SGreen_AddVisCaclObj больше не актуален (это определяет только программист) то его необходимо освободить через функцию #SGreen_DelVisCaclObj\n
Идентификаторы возвращаемые #SGreen_AddAVisCaclObj константны, и не будут удалены или сдвинуты пока сам программист этого не сделает, либо в случае перезагрузки
@{*/

//! добавить объект расчетов видимости, возвращает его идентификатор
SX_LIB_API ID SGreen_AddVisCaclObj();

//! существует ли объект расчетов видимости с номером id
SX_LIB_API bool SGreen_ExistsVisCaclObj(ID idVisCaclObj);

//! удалить объект расчетов
SX_LIB_API void SGreen_DelVisCaclObj(ID idVisCaclObj);

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
	const float3 *pViewPos,		//!< позиция источника фрустума чтобы просчитать дистанцию
	ID idVisCaclObj = 0			//!< идентификатор объекта расчетов видимости для фрустума, создается через #SGreen_AddVisCaclObj, если 0 то считаем в дефолтный
	);

//! отрисовка растительности
SX_LIB_API void SGreen_Render(
	DWORD timeDelta,		//!< время рендера кадра
	const float3 *pViewPos,	//!< позиция наблюдателя (для которого будет рендер)
	GREEN_TYPE typeGreen,	//!< тип рисуемой растительности из перечисления GREEN_TYPE, указав GREEN_TYPE::GREEN_TYPE_ALL будут отрисованы все типы растительности
	ID idVisCaclObj = 0		//!< идентификатор объекта расчетов видимости (создается #SGreen_AddVisCaclObj)
	);

//! индивидуальная отрисовка растительности по идентификатору, массив просчетов по умолчанию (0)
SX_LIB_API void SGreen_RenderSingly(
	DWORD timeDelta,		//!< время рендера кадра
	const float3 *pViewPos,	//!< позиция наблюдателя (для которого будет рендер)
	ID idGreen,				//!< идентификатор растительности
	ID idTex = -1			//!< идентификатор материала, который будет применен ко всем объектам и подгруппам растительности, -1 если надо использовать назначенные материалы
	);

//! индивидуальная отрисовка растительности по идентификатору, массив просчетов по умолчанию (0)
SX_LIB_API void SGreen_RenderObject(
	DWORD timeDelta,		//!< время рендера кадра
	const float3 *pViewPos,	//!< позиция наблюдателя (для которого будет рендер)
	ID idGreen,				//!< идентификатор растительности
	ID idSplit,				//!< идентификатор сплита (в котором находится объект)
	ID idObj,				//!< идентификатор объекта (в сплите)
	ID idTex = -1			//!< идентификатор материала, который будет применен ко всем объектам и подгруппам растительности, -1 если надо использовать назначенные материалы
	);

//!< получить количество единиц растительности (по видам есесно)
SX_LIB_API int SGreen_GetCount();	

//**************************************************************************

/*! \name Управление единицами растительности
@{*/

//! получить имя
SX_LIB_API char* SGreen_MGetName(ID idGreen);

//! получить количество сгенерированных элементов
SX_LIB_API int SGreen_MGetCountGen(ID idGreen);

//! получить количество полигонов в модели нулевого лода
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
	int iLod,				//!< номер лода [0, 2]
	const char *szPathName	//!< путь до модели относительно стандартного пути
	);	

//! установить лод навигации, szPathName путь до модели относительно стандартного пути
SX_LIB_API void SGreen_MSetNav(ID id, const char *szPathName);	

//!@}

//**************************************************************************

/*! получить все данные о физических характеристиках для навигации из всего того что на данный момент содержит растительность и только того что иметт навигационные модели
 \note Все данные кроме arr_count_green функция инициализирует сама, в arr_count_green запишет число
*/
SX_LIB_API void SGreen_GetNavMeshAndTransform(
	float3_t ***pppVertices,			//!< (*pppVertices)[num_green_mesh_nav][num_vertex] - вершины модели
	int32_t **ppCountVertices,			//!< (*ppCountVertices)[num_green_mesh_nav] - количество вершин для модели
	uint32_t ***pppIndeces,				//!< (*pppIndeces)[num_green_mesh_nav][num_vertex] - индексы модели
	ID ***pppMtl,						//!< (*pppMtl)[num_green_mesh_nav][num_vertex] - материал для индекса
	int32_t **ppCountIndeces,			//!< (*ppCountIndeces)[num_green_mesh_nav] - количество индексов для модели
	CGreenDataVertex ***pppTransforms,	//!< (*pppTransforms)[num_green_mesh_nav][num_obj] - трансформации модели навигации
	int32_t **ppCountTransforms,		//!< (*ppCountTransforms)[num_green_mesh_nav] - количество матриц для трансформаций
	int32_t *pCountGreens				//!< (*pCountGreens) - количество единиц растительности по видам
	);

/*! Очищает память, выделенную в #SGreen_GetNavMeshAndTransform, аргументы аналогичны
*/
SX_LIB_API void SGreen_ClearNavMeshAndTransform(
	float3_t **ppVertices,			
	int32_t *pCountVertices,		
	uint32_t **ppIndeces,			
	ID **ppMtl,						
	int32_t *pCountIndeces,			
	CGreenDataVertex **ppTransforms,
	int32_t *pCountTransforms,		
	int32_t iCountGreens			
	);

//! трасировка луча, возвращает "было ли пересечение луча и растительности (по объектно)"
SX_LIB_API bool SGreen_TraceBeam(
	const float3 *pStart,	//!< позиция испускания луча
	const float3 *pDir,		//!< направление взгляда луча
	float3 *pRes,			//!< запишется в какой позиции было персечение (если было)
	ID *pIdGreen,			//!< запишется идентификатор растительности
	ID *pIdSplits,			//!< запишется идентификатор сплита
	ID *pIdObj,				//!< запишется идентификатор объекта
	ID *pIdMtl				//!< запишется идентификатор материала
	);

/*! есть ли входящая в объем растительность (ее объекты), при первом попавшемся полигоне который входит в объем будет возвращено true,
иными словами есть ли соприкосновение бокса с растительность указанного физического материала?
*/
SX_LIB_API bool SGreen_GetOccurencessLeafGrass(
	const float3 *pMin,	//!< минимум
	const float3 *pMax,	//!< максимум
	int iPhysicMtl		//!< физический тип материала (#SGCore_MtlGetPhysicType) который будет проверятся на вхождение
	);

//!@} sgreen_green

#endif

//!@} sgreen
