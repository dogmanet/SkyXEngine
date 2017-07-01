
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxgcore - графического ядра (переопределяемые функции, шейдеры, загружаемые текстуры, render targets, ограничиваюий объем, камера, статическая модель dse формата, skybox, skyclouds)
*/

/*! \defgroup sxgcore sxgcore - графическое ядро, использует технологии DirectX 9
@{
*/

#ifndef __sxgcore
#define __sxgcore

#include <gdefines.h>
#include <GRegisterIndex.h>

#include <d3d9.h>
#include <d3dx9.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif
#include <core\\sxcore.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "DxErr9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <gcore\ModelFile.h>

//
#define SXGC_ERR_NON_DETECTED_D3D -1
#define SXGC_ERR_FAILED_INIT_D3D -2

//! \name Базовые функции библиотеки 
//!@{
SX_LIB_API long SGCore_0GetVersion();			//!< версия подсистемы
SX_LIB_API void SGCore_Dbg_Set(report_func rf);	//!< установка функции вывода сообщений

//! инициализация подсистемы
SX_LIB_API void SGCore_0Create(
	const char* name,			//!< передваваемое имя подсистемы
	HWND hwnd,					//!< дескриптор окна в которое будет осуществляться рендер
	int width,					//!< ширина области рендера
	int heigth,					//!< высота области рендера
	bool windowed,				//!< оконный режим использовать ли? иначе полноэкранный
	DWORD create_device_flags,	//!< флаги создания устрайства (стандартные dx)
	bool is_unic = true			//!< должна ли подсистема быть уникальной на основе имени
	);

SX_LIB_API void SGCore_AKill();	//!< уничтожение либы

SX_LIB_API IDirect3DDevice9* SGCore_GetDXDevice();	//!< возвращает dx устройство

//!@}

//#############################################################################

/*! \name Отладочное сообщение в окне рендера
 \note Графическое ядро предусматривает наличие текстового сообщения в окне рендера, которое формирует непосредственно сам программист и дает команду на его вывод
@{
*/
#define SXGC_STR_SIZE_DBG_MSG 4096 /*!< размер отладочного сообщения, выводимого в окно рендера */
/*! Вывод отладочного сообщения в окно рендера.
Аргументы аналогичны стандартным функциям типа printf.
*/
SX_LIB_API void SGCore_DbgMsg(const char* format, ...);
//!@}

//#############################################################################

/*! \name Обработка потери/восстановления устройства 
 \warning Функции обработки потери/восстановления устройства обрабатывают ресурсы только в пределах данной библиотеки, другие библиотеки должны сами производить данную обработку!
*/
//!@{

SX_LIB_API void SGCore_OnLostDevice();	//!< вызывать при потере устройства

//! вызывать при попытке сброса устройства
SX_LIB_API bool SGCore_OnDeviceReset(
	int width,		//!< новая ширина окна
	int heigth,		//!< новая выоста окна
	bool windewed	//!< true - оконный режим, false - полноэкранный
	);	

SX_LIB_API void SGCore_OnResetDevice();	//!< вызывать при сбросе устроства

//! отрисовка full screen quad (уже смещенного как надо чтобы не было размытия)
SX_LIB_API void SGCore_ScreenQuadDraw();

//!@}

//#############################################################################

/*! \name Базовые функции установки настроек выборки из текстуры 
id - идентификатор текстурного слота
value - для Filter D3DTEXTUREFILTERTYPE, для Address D3DTEXTUREADDRESS
*/
//!@{

SX_LIB_API void SGCore_SetSamplerFilter(DWORD id, DWORD value);	//!< установка фильтрации для конкретного слота
SX_LIB_API void SGCore_SetSamplerFilter2(DWORD begin_id, DWORD end_id, DWORD value);	//!< установка фильтрации для набора слотов от begin_id до end_id

SX_LIB_API void SGCore_SetSamplerAddress(DWORD id, DWORD value);	//!< установка адресации для конкретного слота
SX_LIB_API void SGCore_SetSamplerAddress2(DWORD begin_id, DWORD end_id, DWORD value);	//!< установка адресации для набора слотов от begin_id до end_id
//!@}

//#############################################################################

/*! \defgroup sxgcore_redefinition_func Переопределяемые функции
 \ingroup sxgcore

 \note sxgcore содержит базовые и необходимые функции для переопределения их пользователем, которые будут доступны из графического ядра в других библиотеках зависимых от графического ядра, но в тоже время, эти функции могут быть переопределены функциями из других библиотек. \n
Это обеспечивает централизованную обобщенную зависимость от самого графического ядра и исключает перекрестные зависимости библиотек между собой, позволяя программисту воздействовать на ценрт управления не  из центра. \n
Переопределяемые функции могут быть переопределены во внутреннем состоянии, то есть функция обертка так и останется функцией оберткой только будет вызывать уже переопределенную функцию. \n
Переопределяемые функции изначально выполняют штатные действия исходя из их назначения, и могут вообще не переопределяться если так надо программисту, то есть необходимость в их переопределении исходит только от программиста. \n

@{
*/

//! \name Прототипы переопределяемых функций 
//!@{

/*! draw indexed primitive, команда отрисовки.
Аналогична DrawIndexedPrimitive, в дополнение к DIP инкрементирует счетчик DIPов в int регистрах по индексу #G_RI_INT_COUNT_DIP, обнуление данных только на стороне приложения
*/
typedef void(*g_func_dip) (UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);

/*! установка материала id, world - мировая матрица.
По умолчанию установка текстуры в нулевой текстурный слот, id – идентификатор материала (по умолчанию - текстуры), world – матрица трансформации модели
*/
typedef void(*g_func_mtl_set) (ID id, float4x4* world);

/*! загрузка материала, name - имя текстуры с расширением, mtl_type - тип материала на случай провала загрузки.
Загрузка материала (по умолчанию – текстуры) с именем name, mtl_type – типа материала, 
будет задействован только в случае провала определения типа материала (по умолчанию не используется) - 
тип стандартного материала будет определен на основании mtl_type, может принимать одно из значений MTL_TYPE_
*/
typedef ID(*g_func_mtl_load) (const char* name,	int mtl_type);

//! получить сорт материала, по дефолту 0
typedef int(*g_func_mtl_get_sort) (ID id);
//! получить физический тип материала
typedef int(*g_func_mtl_get_physic_type)(ID id);
//! рисовать ли подгруппы моделей данного материала раздельно?
typedef bool(*g_func_mtl_group_render_is_singly) (ID id);
//!@}

//! \name Переопределяемые функции(переопределение реализации внутри)
//!@{

//! \copydoc g_func_dip
SX_LIB_API void SGCore_DIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);
//! \copydoc g_func_mtl_set
SX_LIB_API void SGCore_MtlSet(ID id, float4x4* world);
//! \copydoc g_func_mtl_load
SX_LIB_API ID SGCore_MtlLoad(const char* name, int mtl_type);
//! \copydoc g_func_mtl_get_sort
SX_LIB_API int SGCore_MtlGetSort(ID id);
//! \copydoc g_func_mtl_get_physic_type
SX_LIB_API int SGCore_MtlGetPhysicType(ID id);
//! \copydoc g_func_mtl_group_render_is_singly
SX_LIB_API bool SGCore_MtlGroupRenderIsSingly(ID id);

//!@}

//! \name Переопределение функций
//!@{

//! переназначение g_func_dip
SX_LIB_API void SGCore_SetFunc_DIP(g_func_dip func);
//! переназначение g_func_mtl_set
SX_LIB_API void SGCore_SetFunc_MtlSet(g_func_mtl_set func);
//! переназначение g_func_mtl_load
SX_LIB_API void SGCore_SetFunc_MtlLoad(g_func_mtl_load func);
//! переназначение g_func_mtl_get_sort
SX_LIB_API void SGCore_SetFunc_MtlGetSort(g_func_mtl_get_sort func);
//! переназначение g_func_mtl_get_physic_type
SX_LIB_API void SGCore_SetFunc_MtlGetPhysicType(g_func_mtl_get_physic_type func);
//! переназначение g_func_mtl_group_render_is_singly
SX_LIB_API void SGCore_SetFunc_MtlGroupRenderIsSingly(g_func_mtl_group_render_is_singly func);
//!@}

//!@} group sxgcore_redefinition_func

//#############################################################################

/*! \defgroup sxgcore_shaders Шейдеры
 \ingroup sxgcore

 \note Распознавание  шейдеров (обращение через функции) происходит на основе идентификаторов представленных типом ID \n
Если речь идет об идентификации на основе имени то имя в данном случае эта та строка name которая была передана SGCore_ShaderLoad \n
Все шейдеры загружаются с версией 3.0 \n
Если имя шейдер не содержит нижний пробел (_) то значит шейдера находится в корне директории \n
Если шейдер содержит нижний пробел (_) то строка до первого нижнего проблема это имя папки в котором находится шейдер с целым именем \n
Пример: pp_shader.vs - лежит по загружаемому пути: /pp/pp_shader.vs
@{
*/

/*! \name Ограничения на размеры
@{
*/

//! максимальный размер имени директории (до _)
#define SXGC_SHADER_MAX_SIZE_DIR 64
//! максимальный размер имени с расширением (после _)
#define SXGC_SHADER_MAX_SIZE_NAME 64
//! общий максимальный  размер имени текстуры с расширением
#define SXGC_SHADER_MAX_SIZE_DIRNAME SXGC_SHADER_MAX_SIZE_DIR + SXGC_SHADER_MAX_SIZE_NAME
//! максимальный размер пути до файла шейдера (без имени файла)
#define SXGC_SHADER_MAX_SIZE_STDPATH 256
//! максимальный размер полного пути до шейдера (включая имя шейдера)
#define SXGC_SHADER_MAX_SIZE_FULLPATH SXGC_SHADER_MAX_SIZE_STDPATH + SXGC_SHADER_MAX_SIZE_DIRNAME

//! максимальная длина имени переменной в шейдере
#define SXGC_SHADER_VAR_MAX_SIZE 64
//! максимальное количество переменных в шейдере
#define SXGC_SHADER_VAR_MAX_COUNT 64

//! количество макросов в массиве макросов
#define SXGC_SHADER_COUNT_MACRO 12

//!@}

//! типы шейдеров (int type_shader)
enum ShaderType
{
	st_vertex,	//!< вершинный
	st_pixel	//!< пиксельный
};

//! типы проверок дубликатов шейдеров
enum ShaderCheckDouble
{
	scd_none,	//!< нет проверки
	scd_path,	//!< проверка по пути (имени шейдера с расширением)
	scd_name	//!< проверка по пользовательскому имени
};

//! загрузка шейдера
SX_LIB_API ID SGCore_ShaderLoad(
	ShaderType type_shader,	//!< тип шейдера
	const char* path,		//!< имя файла шейдера с расширением
	const char* name,		//!< имя шейдера которое присвоится при загрузке
	ShaderCheckDouble is_check_double,	//!< проверять ли на уникальность
	D3DXMACRO* macro = 0	//!< макросы
	);

SX_LIB_API void SGCore_ShaderGetName(ShaderType type_shader, ID id, char* name);	//!< записывает пользовательское имя шейдера в name
SX_LIB_API void SGCore_ShaderGetPath(ShaderType type_shader, ID id, char* path);	//!< записывает имя шейдер с расширением в path
SX_LIB_API ID SGCore_ShaderIsExistName(ShaderType type_shader, const char* name);	//!< существует ли шейдер с пользовательским именем name, если да то возвращает id
SX_LIB_API ID SGCore_ShaderIsExistPath(ShaderType type_shader, const char* path);	//!< существует ли шейдер с именем файла и расширением name, если да то возвращает id
SX_LIB_API bool SGCore_ShaderIsValidate(ShaderType type_shader, ID id);				//!< загружен ли шейдер с данным id

SX_LIB_API void SGCore_ShaderUpdateN(ShaderType type_shader, const char* name, D3DXMACRO macro[] = 0);	//!< бинд шейдера по имени
SX_LIB_API void SGCore_ShaderUpdate(ShaderType type_shader, ID id, D3DXMACRO macro[] = 0);	//!< бинд шейдера по id

SX_LIB_API void SGCore_ShaderSetStdPath(const char* path);	//!< установить абсолютный путь откуда брать шейдеры
SX_LIB_API void SGCore_ShaderGetStdPath(char* path);		//!< возвращает абсолютный путь откуда берутся шейдеры

//! перезагрузить все шейдеры, с учетом макросов
SX_LIB_API void SGCore_ShaderReloadAll();	

SX_LIB_API ID SGCore_ShaderGetID(ShaderType type_shader, const char* name);	//!< получить идентификатор шейдера по имени

SX_LIB_API void SGCore_ShaderBindN(ShaderType type_shader, const char* name);	//!< бинд шейдера по имени
SX_LIB_API void SGCore_ShaderBind(ShaderType type_shader, ID id);	//!< бинд шейдера по id

SX_LIB_API void SGCore_ShaderUnBind();	//!< обнуление биндов шейдеров

/*! \name Передача данных в шейдер
@{
*/

//! передача float значений в шейдер по имени
SX_LIB_API void SGCore_ShaderSetVRFN(
	ShaderType type_shader,	//!< тип шейдера из #ShaderType
	const char* name_shader,//!< пользовательское имя шейдера
	const char* name_var,	//!< имя переменной которой присваивается значение
	void* data,				//!< указатель на массив данных
	int count_float4 = 0	//!< количество float4 значений в передаваемом массиве, если меньше одного значит количество будет взято из шейдера
	);

//! передача float значений в шейдер по ID
SX_LIB_API void SGCore_ShaderSetVRF(
	ShaderType type_shader, //!< тип шейдера из #ShaderType
	ID id,					//!< идентификатор шейдера
	const char* name_var,	//!< имя переменной которой присваивается значение
	void* data,				//!< указатель на массив данных
	int count_float4 = 0	//!< количество float4 значений в передаваемом массиве, если меньше одного значит количество будет взято из шейдера
	);

//! передача int значений в шейдер по имени
SX_LIB_API void SGCore_ShaderSetVRIN(
	ShaderType type_shader,	//!< тип шейдера из #ShaderType
	const char* name_shader,//!< пользовательское имя шейдера
	const char* name_var,	//!< имя переменной которой присваивается значение
	void* data,				//!< указатель на массив данных
	int count_int4 = 0		//!< количество int4 значений в передаваемом массиве, если меньше одного значит количество будет взято из шейдера
	);

//! передача int значений в шейдер по ID
SX_LIB_API void SGCore_ShaderSetVRI(
	ShaderType type_shader, //!< тип шейдера из #ShaderType
	ID id,					//!< идентификатор шейдера
	const char* name_var,	//!< имя переменной которой присваивается значение
	void* data,				//!< указатель на массив данных
	int count_int4 = 0		//!< количество int4 значений в передаваемом массиве, если меньше одного значит количество будет взято из шейдера
	);

//!@}
//!@} sxgcore_shaders

//#############################################################################

/*! \defgroup sxgcore_loadtex Загружаемые текстуры
 \ingroup sxgcore
 \note Распознавание текстур (обращение через функции) происходит на основе идентификаторов представленных типом ID. \n
Имя текстуры обязательно долно содержать нижний пробел (_), строка до первого нижнего проблема это имя папки в котором находится текстура с целым именем. \n
Например: mtl_tex.dds - лежит по загружаемому пути: /mtl/mtl_tex.dds
@{
*/

//! \name Ограничения на размеры
//!@{

//! максимальный размер имени директории (до _)
#define SXGC_LOADTEX_MAX_SIZE_DIR 64
//! максимальный размер имени с расширением (после _)
#define SXGC_LOADTEX_MAX_SIZE_NAME 64
//! общий максимальный  размер имени текстуры с расширением
#define SXGC_LOADTEX_MAX_SIZE_DIRNAME SXGC_LOADTEX_MAX_SIZE_DIR + SXGC_LOADTEX_MAX_SIZE_NAME
//! максимальный размер пути до файла текстуры (без имени файла)
#define SXGC_LOADTEX_MAX_SIZE_STDPATH 256
//! максимальный размер полного пути до текстуры (включая имя текстуры)
#define SXGC_LOADTEX_MAX_SIZE_FULLPATH SXGC_LOADTEX_MAX_SIZE_STDPATH + SXGC_LOADTEX_MAX_SIZE_DIRNAME

//! количество mipmap уровней в загружаемых текстурах
#define SXGC_LOADTEX_COUNT_MIPMAP 5	

/*! формат в который будут преобразованы загружаемые текстуры
 \warning если установить формат со сжатием то необходимо следовать всем правилам оформления текстур для данного формата
*/
#define SXGC_LOADTEX_FORMAT_TEX D3DFMT_FROM_FILE

//!@}

//! \name Типы материалов
//!@{

#define MTL_TYPE_GEOM 0		/*!< статическая геометрия */
#define MTL_TYPE_GRASS 1	/*!< растительность - трава */
#define MTL_TYPE_TREE 2		/*!< растительность - деревья */
#define MTL_TYPE_SKIN 3		/*!< скелетная модель */
//#define MTL_LIGHT 4

//!@}

//! типы текстур
enum LoadTexType
{
	ltt_load,	//!< загружаемая
	ltt_const,	//!< неудаляемая загружаемая
	ltt_custom,	//!< созданная пользователем

	/*! самоопределение типа, на тот случай когда обновляем текстуру которая точно есть.
	Если определить этот тип, а внутри у текстуры на самом деле нет типа (скорее всего нет текстуры)
	то будет определен ltt_load
	*/
	ltt_self,	
};

SX_LIB_API void SGCore_LoadTexStdPath(const char* path);	//!< установить стандартный путь откуда брать текстуры
SX_LIB_API void SGCore_LoadTexClearLoaded();				//!< очистить список загружаемых текстур
SX_LIB_API void SGCore_LoadTexDelete(ID id);				//!< удалить тектуру по id (независимо от типа)
SX_LIB_API ID SGCore_LoadTexAddName(const char* name, LoadTexType type);//!< добавляем имя текстуры, взамен получаем на нее ID (поставить в очередь)
SX_LIB_API ID SGCore_LoadTexGetID(const char* name);	//!< получить id по имени
SX_LIB_API void SGCore_LoadTexGetName(ID id, char* name);//!< получить имя по id

/*! создать место для текстуры tex и присвоить ей имя name, возвращает id
 \warning создавать текстур необходимо в managed pool (D3DPOOL_MANAGED) ибо обработка потери и восстановления устройства сюда не приходит
*/
SX_LIB_API ID SGCore_LoadTexCreate(const char* name, IDirect3DTexture9* tex);

/*! обновить/перезагрузить текстуру name, если текстуры не было в списке то добавляет.
Если текстуру надо обновить, но тип у нее заранее не известен, но она точно уже загружена  
то можно использовать тип самоопределения ltt_self, тогда тип текстуры не изменится
*/
SX_LIB_API ID SGCore_LoadTexUpdateN(const char* name, LoadTexType type);
SX_LIB_API void SGCore_LoadTexUpdate(ID id);//!< обновить/перезагрузить текстуру

SX_LIB_API IDirect3DTexture9* SGCore_LoadTexGetTex(ID id);	//в!< озвращает текстуру по id

//! загрузка всех текстур поставленных в очередь, если есть очередь
SX_LIB_API void SGCore_LoadTexLoadTextures();	

//!@} sxgcore_loadtex

//#############################################################################

/*! \defgroup sxgcore_rt Render targets
 \ingroup sxgcore
 \note Cброс и восстановление устройства сюда приходят
@{
*/

//! добавить новый render target
SX_LIB_API ID SGCore_RTAdd(
	UINT width,				//!< ширина
	UINT height,			//!< высота
	UINT levels,			//!< количество mip-map уровней
	DWORD usage,			//!< признак применения, возможные значения из D3DUSAGE_
	D3DFORMAT format,		//!< формат из D3DFORMAT
	D3DPOOL pool,			//!< где будет размещена текстура, из D3DPOOL
	const char* name,		//!< имя rt
	/*! коэфициент размеров rt относительно области рендера 
	(если rt и размер области одинаковы то 1, если rt меньша на 0.5 то 0.5) 
	если указаны фиксированные значения то ставить 0, это нужно для устновления размеров при восстановлении устройства
	*/
	float coeffullscreen
	);

SX_LIB_API void SGCore_RTDeleteN(const char* name);	//!< удалить rt по имени
SX_LIB_API void SGCore_RTDelete(ID id);				//!< удалить rt по id

SX_LIB_API ID SGCore_RTGetNum(const char* name); //!< возвращает id по имени

SX_LIB_API IDirect3DTexture9* SGCore_RTGetTextureN(const char* name);	//!< возвращает текстуру по имени
SX_LIB_API IDirect3DTexture9* SGCore_RTGetTexture(ID id);				//!< возвращает текстуру по id

//!@} sxgcore_rt

//#############################################################################

/*! \defgroup sxgcore_dse_static Статическая модель dse формата
 \ingroup sxgcore
 \note sxgcore предоставляет возможность загрузки статических моделей, формат вершин которых представлен структурой #vertex_static, которая объявлена в файле ModelFile.h
 \todo Описать формат файла статической модели dse
 @{
*/

//! структура статической модели dse
struct ISXDataStaticModel : public IBaseObject
{
	virtual ~ISXDataStaticModel(){};

	virtual ISXDataStaticModel* GetCopy()=0;	//!< получить абсолютную копию модели
	
	IDirect3DVertexBuffer9* VertexBuffer;	//!< вершиный буфер
	IDirect3DIndexBuffer9* IndexBuffer;		//!< индексный буфер

	UINT SubsetCount;	//!< количество подгрупп
	char** ArrTextures;	//!< массив имен текстур без расширения
	UINT* StartIndex;	//!< массив стартовых позиций индексов для каждой подгруппы
	UINT* IndexCount;	//!< массив количества индексов для каждой подгруппы
	UINT* StartVertex;	//!< массив стартовых позиций вершин для каждой подгруппы
	UINT* VertexCount;	//!< массив количества вершин для каждой подгруппы
	UINT AllIndexCount;	//!< общее количество индексов
	UINT AllVertexCount;//!< общее количество вершин

	float4_t BSphere;
	float3_t BBMax, BBMin;
};

SX_LIB_API ISXDataStaticModel* SGCore_StaticModelCr();	//!< создать статическую модель
SX_LIB_API void SGCore_StaticModelLoad(const char* file, ISXDataStaticModel** data);	//!< загрузить статическую модель, data инициализируется внутри
SX_LIB_API void SGCore_StaticModelSave(const char* file, ISXDataStaticModel** data);	//!< сохранить статическую модель
SX_LIB_API IDirect3DVertexDeclaration9* SGCore_StaticModelGetDecl();	//!< возвращает декларацию вершин статической модели

//!@} sxgcore_dse_static

//#############################################################################

/*! \defgroup sxgcore_bb Ограничивающий объем
 \ingroup sxgcore
@{
*/

/*! Простой объект трансформаций с минимальным описанием.
 \note Для корректного использования необходимо сначала установить позицию/поворот/масштаб после чего CalculateWorld
*/
struct ISXTransObject : public IBaseObject
{
	virtual ~ISXTransObject(){};

	SX_ALIGNED_OP_MEM

	virtual inline float4x4* CalcWorld() = 0;	//!< просчет мировой матрицы трансформации на основе поворотов масштабирования и позиции

	float3 Position;	//!< позиция
	float3 Rotation;	//!< повороты
	float3 Scale;		//!< масштабирование
	float4x4 World;		//!< мировая матрица на основе поворотов масштабирования и позиции
};

SX_LIB_API ISXTransObject* SGCore_CrTransObject();	//!< создать ISXTransObject

//! структура описывающая ограничивающий квадрат (а точнее параллелепипед) в пространстве экрана
struct SXPosBBScreen
{
	float x;		//!< позиция по оси x в пространстве экрана
	float y;		//!< позиция по оси y в пространстве экрана
	float width;	//!< ширина в пикселях
	float height;	//!< высота в пикселях
	float maxdepth;	//!< конец объекта глубина 0-1
	float mindepth;	//!< начало объект глубина 0-1
	bool IsVisible;	//!< виден ли квадрат наблюдателю
};


/*! класс ограничивающего объема
 \warning GetMinMax, GetSphere до вызова CalcWorldAndTrans возвращают нетрансформирвоанные данные
*/
class ISXBound : public virtual ISXTransObject
{
public:
	virtual ~ISXBound(){};

	SX_ALIGNED_OP_MEM

	/*! Просчет ограничивающего объема по вершинному буферу*/
	virtual void CalcBound(
		IDirect3DVertexBuffer9* vertex_buffer, //!< вершинный буфер (незаблокированный), в вершинах которого первым элементом идет позиция float3_t вектор  
		DWORD count_vert,	//!< количество вершин
		DWORD bytepervert	//!< количество байт в вершине
		) = 0;

	//! функция просчета мировой матрицы и трансформации минимума и максимума
	virtual float4x4* CalcWorldAndTrans() = 0;

	//! просчет структуры SXPosBBScreen 
	virtual void GetPosBBScreen(
		SXPosBBScreen *res,		//!< инициализированная стркутура #SXPosBBScreen для записи
		float3* campos,			//!< позиция наблюдателя
		float3* sizemapdepth,	//!< размер карты глубины глябины для просчета float3(ширина, высота, максильная глубина)
		float4x4* mat			//!< произведение видовой и проекционной матриц
		) = 0;

	virtual void SetMinMax(float3* min, float3* max) = 0;	//!< установить экстремум, также просчитает и сферу
	virtual void GetMinMax(float3* min, float3* max) const = 0;	//!< запишет в min и max точки экстремума

	virtual void SetSphere(float3* center, float* radius) = 0;	//!< установить сферу, просчитает также и параллелепипед
	virtual void GetSphere(float3* center, float* radius) const = 0;	//!< запишет в center центр сферы, в radius радиус сферы

	virtual bool IsPointInSphere(float3* point) const = 0;	//!< находится ли точка point в пределах сферы
	virtual bool IsPointInBox(float3* point) const = 0;		//!< находится ли точка point в пределах параллелепипеда

protected:
	float3 Min;
	float3 Max;

	float3 Center;
	float Radius;
};

SX_LIB_API ISXBound* SGCore_CrBound(); //!< создать ISXBound

//!@} sxgcore_bb

//#############################################################################

//! создание меша (ID3DXMesh) конуса 
SX_LIB_API void SGCore_FCreateCone(
	float fTopRadius,		//!< верхний радиус
	float fBottomRadius,	//!< нижний радиус
	float fHeight,			//!< длина
	ID3DXMesh ** ppMesh,	//!< выходной меш
	UINT iSideCount			//!< количество частей??
	);

//! просчет ограничивающего объема по вершинному буфер
SX_LIB_API void SGCore_FCompBoundBox(
	IDirect3DVertexBuffer9* vertex_buffer, //!< вершинный буфер (незаблокированный), в вершинах которого первым элементом идет позиция float3_t вектор
	ISXBound** bound,	//!< инициализированный ISXBound
	DWORD count_vert,	//!< количество вершин
	DWORD bytepervert	//!< количество байт в вершине
	);

//! создание меша (ID3DXMesh) ограничивающего объема
SX_LIB_API void SGCore_FCreateBoundingBoxMesh(float3* min, float3* max, ID3DXMesh** bbmesh);

/*! \name Оптимизация индексных буферов
@{*/

//! 16 битные индексные буферы
SX_LIB_API void SGCore_OptimizeIndecesInSubsetUint16(
	uint16_t* ib,		//!< указатель на индексный буфер
	uint16_t numFaces,	//!< количество индексов
	uint16_t numVerts	//!< количество вершин
	);	

//! 16 битные индексные буферы
SX_LIB_API void SGCore_OptimizeIndecesInSubsetUint32(
	uint32_t* ib,		//!< указатель на индексный буфер
	uint32_t numFaces,	//!< количество индексов
	uint32_t numVerts	//!< количество вершин
	);	
//!@}


/*! \defgroup sxgcore_bb_intersect Функции просчета попаданий точек в объемы и деление объемов
 \ingroup sxgcore
@{
*/

/*! \name Просчеты попадания точки/точек в объем 
 \note 2d - на основании x и z координат \n
3d - на основании всех трех координат \n
Abs - абсолютное нахождение внутри, не на границах, иное допускает нахождение на границах 
@{
*/

SX_LIB_API bool SGCore_0InPos2D(float3* min, float3* max, float3* pos);		//!< находится ли точка pos в пределах [min,max] по осям x z
SX_LIB_API bool SGCore_0InPosAbs2D(float3* min, float3* max, float3* pos);	//!< находится ли точка pos в пределах (min,max) по осям x z

//! возвращает количество точек (p1,p2,p3) лежащих в пределах [min,max]  по осям x z
SX_LIB_API int SGCore_0CountPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);
//! возвращает количество точек (p1,p2,p3) лежащих в пределах (min,max)  по осям x z
SX_LIB_API int SGCore_0CountPosPointsAbs2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);
//! лежит ли хотя бы одна точка абсолютно в (min,max) или хотя бы 2 точки в пределах [min,max], из числа трех точек p1,p2,p3, по осям x z
SX_LIB_API bool SGCore_0InPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);


SX_LIB_API bool SGCore_0InPos3D(float3* min, float3* max, float3* pos);		//!< находится ли точка pos в пределах [min,max]
SX_LIB_API bool SGCore_0InPosAbs3D(float3* min, float3* max, float3* pos);	//!< находится ли точка pos в пределах (min,max)

//! возвращает количество точек (p1,p2,p3) лежащих в пределах [min,max]
SX_LIB_API int SGCore_0CountPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);
//! возвращает количество точек (p1,p2,p3) лежащих в пределах (min,max)
SX_LIB_API int SGCore_0CountPosPointsAbs3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);
//! лежит ли хотя бы одна точка абсолютно в (min,max) или хотя бы 2 точки в пределах [min,max], из числа трех точек p1,p2,p3
SX_LIB_API bool SGCore_0InPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);

//!@}

/*! кубическое (octo) деление объема (по всем трем осям)
 \warning массивы и объекты должны быть заранее инициализированны
 */
SX_LIB_API void SGCore_0ComBoundBoxArr8(ISXBound* bound, ISXBound** bound_arr);
/*! квадратичное (quad) деление объема (по двум осям x и z)
 \warning массивы и объекты должны быть заранее инициализированны
*/
SX_LIB_API void SGCore_0ComBoundBoxArr4(ISXBound* bound, ISXBound** bound_arr);

//!@} sxgcore_bb_intersect

//#############################################################################

/*! \defgroup sxgcore_camera Камера
 \ingroup sxgcore
 \todo Добавить в библиотеку математики плоскости и операции с ними, заменить dx плоскости на свои
 \todo Возможно надо расширить возможности и абстракцию класса камеры, к примеру матрицу проекции хранить тоже в камере, и сделать отдельные функции для ее модификации
@{
*/

//! структура описание плоскости
struct SXFrustumPlane
{
	float3_t Normal;
	float Distance;

	SX_ALIGNED_OP_MEM

	void Normalize()
	{
			float denom = sqrt((Normal.x*Normal.x) + (Normal.y*Normal.y) + (Normal.z*Normal.z));
			Normal.x = Normal.x / denom;
			Normal.y = Normal.y / denom;
			Normal.z = Normal.z / denom;
			Distance = Distance / denom;
	}
};

//! класс описывающий фрустум
class ISXFrustum : public IBaseObject
{
public:
	virtual ~ISXFrustum(){};

	SX_ALIGNED_OP_MEM

	//! обновление фрустума, на вход матрицы по которым необходимо построить фрустум
	virtual void Update(
		const float4x4* view,	//<! видовая матрица
		const float4x4* proj	//<! проекционная матрица
		) = 0;

	virtual bool PointInFrustum(const float3 *point) = 0;	//!< находится ли точка во фрустуме
	virtual bool PolyInFrustum(const float3* p1, const float3* p2, const float3* p3) = 0;		//!< находится ли треугольник во фрутстуме
	virtual bool PolyInFrustumAbs(const float3* p1, const float3* p2, const float3* p3) = 0;	//!< находится ли полигон во фрустуме полностью

	virtual bool SphereInFrustum(const float3 *point, float radius) const = 0;	//!< находится ли полигон во фрустуме

	virtual bool SphereInFrustumAbs(const float3 *point, float radius) = 0;	//!< находится ли сфера во фрустуме полностью
	virtual bool BoxInFrustum(float3* min, float3* max) = 0;				//!< находится ли параллелепипед (описанный точками экстремума) во фрустуме

//protected:
	SXFrustumPlane ArrFrustumPlane[6];

	float3	Point[8];
	float3	Center;
};

SX_LIB_API ISXFrustum* SGCore_CrFrustum(); //!< создать ISXFrustum


//! камера
class ISXCamera : public IBaseObject
{
public:
	virtual ~ISXCamera(){};

	SX_ALIGNED_OP_MEM

	/*! \name Движение
	 \note В метрах
	@{ 
	*/
	virtual inline void PosLeftRight(float units) = 0;	//!< влево/вправо
	virtual inline void PosUpDown(float units) = 0;		//!< вверх/вниз
	virtual inline void PosFrontBack(float units) = 0;	//!< вперед/назад
	//!@}

	/*! \name Вращение
	 \note В радианах
	@{ 
	*/
	virtual inline void RotUpDown(float angle) = 0;		//!< вращение вверх/вниз
	virtual inline void RotRightLeft(float angle) = 0;	//!< вращение вправо/влево
	virtual inline void Roll(float angle) = 0;			//!< крен
	virtual inline void SetOrientation(const SMQuaternion & q) = 0; //!< установить полное вращение
	//!@}

	virtual inline void GetViewMatrix(float4x4* view_matrix) = 0;//!< получаем матрицу вида в view_matrix

	/*! \name Базис
	@{
	*/

	virtual inline void GetPosition(float3* pos) = 0;	//!< в pos записывает текущую позицию в мире
	virtual inline void SetPosition(float3* pos) = 0;	//!< устанавливает позицию в мире

	virtual inline void GetRight(float3* right) = 0;	//!< в right записывает парвый вектор
	virtual inline void GetUp(float3* up) = 0;			//!< в up записывает верхний вектор
	virtual inline void GetLook(float3* look) = 0;		//!< в look записывает вектор направление взгляда

	virtual inline void GetRotation(float3* rot) = 0;	//!< в rot записывает углы поворотов по осям, в радианах

	//повроты по осям
	virtual inline float GetRotationX() = 0;	//!< возвращает поворот по оси X, в радианах
	virtual inline float GetRotationY() = 0;	//!< возвращает поворот по оси Y, в радианах
	virtual inline float GetRotationZ() = 0;	//!< возвращает поворот по оси Z, в радианах

	//!@}

	virtual inline void SetFOV(float fov) = 0;	//!< Устанавливает FOV камеры
	virtual inline float GetFOV() = 0;	//!< возвращает FOV камеры

	ISXFrustum* ObjFrustum;	//!< фрустум этой камеры

	float3 LastVal; //??
	bool IsAccel;	//??

protected:
	float3 Right;
	float3 Up;
	float3 Look;

	float3 Position;

	//float AngleUpDown, AngleRightLeft, AngleRoll;
	
	float3_t m_vPitchYawRoll;

	float m_fFOV;
};

SX_LIB_API ISXCamera* SGCore_CrCamera();	//!< создать ISXCamera

//!@} sxgcore_camera

//#############################################################################

/*! \defgroup sxgcore_sky Небо
 \ingroup sxgcore
@{
*/

/*! \name SkyBox
 \note Используются кубические текстуры
@{
*/

SX_LIB_API void SGCore_SkyBoxCr();	//!< создание
SX_LIB_API bool SGCore_SkyBoxIsCr();//!< инициализирован ли skybox
SX_LIB_API bool SGCore_SkyBoxIsLoadTex();//!< загружена ли текстура?

SX_LIB_API void SGCore_SkyBoxSetStdPathTex(const char* path);	//!< установка пути, относительно которого будут загружаться текстуры
SX_LIB_API void SGCore_SkyBoxGetStdPathTex(char* path);			//!< в path записывает путь относительно которого загружаются текстуры

SX_LIB_API void SGCore_SkyBoxLoadTex(const char *texture);	//!< загрузка текстуры, texture - имя текстуры с расширением
SX_LIB_API void SGCore_SkyBoxChangeTex(const char *texture);//!< смена текстуры, texture - имя текстуры с расширением

SX_LIB_API void SGCore_SkyBoxGetActiveTex(char *texture);
SX_LIB_API void SGCore_SkyBoxGetSecondTex(char *texture);

SX_LIB_API void SGCore_SkyBoxSetRot(float angle);	//!< установка угла поворота angle по оси y, в радианах
SX_LIB_API float SGCore_SkyBoxGetRot();				//!< возвращает угол поворота по оси y, в радианах

SX_LIB_API void SGCore_SkyBoxSetColor(float4_t* color);	//!< установка цвета окраски в пределах 0-1, альфа компонента (w) - на сколько будет окрашен
SX_LIB_API void SGCore_SkyBoxGetColor(float4_t* color);	//!< в color записывает текущий цвет окраски

//! рендер скайбокса
SX_LIB_API void SGCore_SkyBoxRender(
	float timeDelta,	//!< время рендера кадра в млсек 
	float3* pos			//!< позиция набладателя, эта же позиция будет центром skybox
	);

//!@}

/*! \name SkyClouds

 \note Простая плоскость параллельная xz на которую зеркально (х2) натягивается текстура, в постоянном движении.
 Положение констатно.
 Используются обычные 2д текстуры.
@{
*/

SX_LIB_API void SGCore_SkyCloudsCr();		//!< создание
SX_LIB_API bool SGCore_SkyCloudsIsCr();		//!< инициализирован ли sky clouds
SX_LIB_API bool SGCore_SkyCloudsIsLoadTex();//!< загружена ли текстура

SX_LIB_API void SGCore_SkyCloudsSetStdPathTex(const char* path);	//!< установка пути, относительно которого будут загружаться текстуры
SX_LIB_API void SGCore_SkyCloudsGetStdPathTex(char* path);			//!< в path записывает путь относительно которого загружаются текстуры

/*! установка размеров и позиции.
 Так как позиция облаков константна то чтобы была илюзия полного покрытия уровня, необходимо облакам указывать размер в несколько раз больше чем весь доступный уровень, к примеру x2
*/
SX_LIB_API void SGCore_SkyCloudsSetWidthHeightPos(
	float width,	//!< ширина в метрах
	float height,	//!< высота в метрах
	float3* center	//!< позиция центра
	);

SX_LIB_API void SGCore_SkyCloudsLoadTex(const char *texture);	//!< загрузка текстуры, texture - имя текстуры с расширением
SX_LIB_API void SGCore_SkyCloudsChangeTex(const char *texture);	//!< загрузка текстуры, texture - имя текстуры с расширением

SX_LIB_API void SGCore_SkyCloudsSetRot(float angle);//!< установка угла поворота angle по оси y, в радианах
SX_LIB_API float SGCore_SkyCloudsGetRot();			//!< возвращает текущий угол поворота по оси y, в радианах

SX_LIB_API void SGCore_SkyCloudsSetAlpha(float alpha);	//!< устанавливает коэфициент прозрачности, в пределах 0-1
SX_LIB_API float SGCore_SkyCloudsGetAlpha();			//!< возвращает текущий коэфициент прозрачности

SX_LIB_API void SGCore_SkyCloudsSetColor(float4_t* color);//!< установка цвета окраски в пределах 0-1, альфа компонента (w) - на сколько будет окрашен
SX_LIB_API void SGCore_SkyCloudsGetColor(float4_t* color);//!< в color записывает текущий цвет окраски

//! рендер облаков
SX_LIB_API void SGCore_SkyCloudsRender(
	DWORD timeDetlta,	//!< время рендера кадра в млсек
	float3* pos,		//!< позиция набладателя
	bool is_shadow		//!< для теней ли рендерим?
	);

//!@}
//!@} sxgcore_sky

#endif

/*! @} */
