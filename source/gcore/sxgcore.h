
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxgcore - графического ядра (переопределяемые функции, шейдеры, загружаемые текстуры, render targets, ограничиваюий объем, камера, статическая модель dse формата, skybox, skyclouds)
*/

/*! \defgroup sxgcore sxgcore - графическое ядро, использует технологии DirectX 9
@{
*/

#ifndef __SXGCORE_H
#define __SXGCORE_H

#include <GRegisterIndex.h>

#include <d3d9.h>
#include <d3dx9.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <core/sxcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

#include <gdefines.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "DxErr9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <gcore/ModelFile.h>

//флаги компиляции шейдеров
#define SHADER_DEBUG D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3DXSHADER_AVOID_FLOW_CONTROL | D3DXSHADER_SKIPOPTIMIZATION
#define SHADER_RELEASE D3DXSHADER_OPTIMIZATION_LEVEL3 | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3DXSHADER_PARTIALPRECISION | D3DXSHADER_PREFER_FLOW_CONTROL

//определяем флаг компилции шейдеров
#if defined(DEBUG) || defined(_DEBUG) 
#define SHADER_FLAGS SHADER_DEBUG 
#else 
#define SHADER_FLAGS SHADER_RELEASE 
#endif

//
#define SXGC_ERR_NON_DETECTED_D3D -1
#define SXGC_ERR_FAILED_INIT_D3D -2

//! \name Базовые функции библиотеки 
//!@{

//! версия подсистемы
SX_LIB_API long SGCore_0GetVersion();			

//! установка функции вывода сообщений
SX_LIB_API void SGCore_Dbg_Set(report_func fnReport);	

//! инициализация подсистемы
SX_LIB_API void SGCore_0Create(
	const char *szName,			//!< передваваемое имя подсистемы
	HWND hWnd,					//!< дескриптор окна в которое будет осуществляться рендер
	int iWidth,					//!< ширина области рендера
	int iHeigth,				//!< высота области рендера
	bool isWindowed,			//!< оконный режим использовать ли? иначе полноэкранный
	DWORD dwFlags,				//!< флаги создания устрайства (стандартные dx)
	bool isUnic = true			//!< должна ли подсистема быть уникальной на основе имени
	);

//! уничтожение либы
SX_LIB_API void SGCore_AKill();	

//! возвращает dx устройство
SX_LIB_API IDirect3DDevice9* SGCore_GetDXDevice();	

//! возвращает массив всех доступных разрешений монитора, в iCount записывает размер массива
SX_LIB_API const DEVMODE* SGCore_GetModes(int *iCount);

//!@}

//#############################################################################

/*! \name Отладочное сообщение в окне рендера
 \note Графическое ядро предусматривает наличие текстового сообщения в окне рендера, которое формирует непосредственно сам программист и дает команду на его вывод
@{*/

/*!< размер отладочного сообщения, выводимого в окно рендера */
#define SXGC_STR_SIZE_DBG_MSG 4096 

/*! Вывод отладочного сообщения в окно рендера.
Аргументы аналогичны стандартным функциям типа printf.
*/
SX_LIB_API void SGCore_DbgMsg(const char *szFormat, ...);

//!@}

//#############################################################################

/*! \name Обработка потери/восстановления устройства 
 \warning Функции обработки потери/восстановления устройства обрабатывают ресурсы только в пределах данной библиотеки, другие библиотеки должны сами производить данную обработку!
!@{*/

//! вызывать при потере устройства
SX_LIB_API void SGCore_OnLostDevice();	

//! вызывать при попытке сброса устройства
SX_LIB_API bool SGCore_OnDeviceReset(
	int iWwidth,		//!< новая ширина окна
	int iHeigth,		//!< новая выоста окна
	bool isWindewed		//!< true - оконный режим, false - полноэкранный
	);	

//! вызывать при сбросе устроства
SX_LIB_API void SGCore_OnResetDevice();	

//! отрисовка full screen quad (уже смещенного как надо чтобы не было размытия)
SX_LIB_API void SGCore_ScreenQuadDraw();

//!@}

//#############################################################################

/*! \name Базовые функции установки настроек выборки из текстуры 
id - идентификатор текстурного слота
value - для Filter D3DTEXTUREFILTERTYPE, для Address D3DTEXTUREADDRESS
@{*/

//! установка фильтрации для конкретного слота
SX_LIB_API void SGCore_SetSamplerFilter(DWORD dwNum, DWORD dwValue);	

//! установка фильтрации для набора слотов от begin_id до end_id
SX_LIB_API void SGCore_SetSamplerFilter2(DWORD dwNumStart, DWORD dwCount, DWORD dwValue);


//! установка адресации для конкретного слота
SX_LIB_API void SGCore_SetSamplerAddress(DWORD dwNum, DWORD dwValue);

//! установка адресации для набора слотов от begin_id до end_id
SX_LIB_API void SGCore_SetSamplerAddress2(DWORD dwNumStart, DWORD dwCount, DWORD dwValue);

//!@}

//#############################################################################

/*! \defgroup sxgcore_redefinition_func Переопределяемые функции
 \ingroup sxgcore
 \note sxgcore содержит базовые и необходимые функции для переопределения их пользователем, которые будут доступны из графического ядра в других библиотеках зависимых от графического ядра, но в тоже время, эти функции могут быть переопределены функциями из других библиотек. \n
Это обеспечивает централизованную обобщенную зависимость от самого графического ядра и исключает перекрестные зависимости библиотек между собой, позволяя программисту воздействовать на ценрт управления не  из центра. \n
Переопределяемые функции могут быть переопределены во внутреннем состоянии, то есть функция обертка так и останется функцией оберткой только будет вызывать уже переопределенную функцию. \n
Переопределяемые функции изначально выполняют штатные действия исходя из их назначения, и могут вообще не переопределяться если так надо программисту, то есть необходимость в их переопределении исходит только от программиста. \n
@{*/

/*! \name Прототипы переопределяемых функций 
@{*/

/*! draw indexed primitive, команда отрисовки.
Аналогична DrawIndexedPrimitive, в дополнение к DIP инкрементирует счетчик DIPов в int регистрах по индексу #G_RI_INT_COUNT_DIP, обнуление данных только на стороне приложения
*/
typedef void(*g_func_dip) (UINT uiTypePrimitive, long lBaseVertexIndex, UINT uiMinVertexIndex, UINT uiNumVertices, UINT uiStartIndex, UINT uiPrimitiveCount);

/*! установка материала id, world - мировая матрица.
По умолчанию установка текстуры в нулевой текстурный слот, id – идентификатор материала (по умолчанию - текстуры), world – матрица трансформации модели
*/
typedef void(*g_func_mtl_set) (ID id, float4x4 *pWorld);

/*! загрузка материала, szName - имя текстуры с расширением, iMtlType - тип материала на случай провала загрузки.
Загрузка материала (по умолчанию – текстуры) с именем name, iMtlType – типа материала, 
будет задействован только в случае провала определения типа материала (по умолчанию не используется) - 
тип стандартного материала будет определен на основании iMtlType, может принимать одно из значений MTL_TYPE_
*/
typedef ID(*g_func_mtl_load) (const char *szName,	int iMtlType);

//! получить сорт материала, по дефолту 0
typedef int(*g_func_mtl_get_sort) (ID id);

//! получить физический тип материала
typedef int(*g_func_mtl_get_physic_type)(ID id);

//! рисовать ли подгруппы моделей данного материала раздельно?
typedef bool(*g_func_mtl_group_render_is_singly) (ID id);

//!@}

/*! \name Переопределяемые функции(переопределение реализации внутри)
!@{*/

//! \copydoc g_func_dip
SX_LIB_API void SGCore_DIP(UINT uiTypePrimitive, long lBaseVertexIndex, UINT uiMinVertexIndex, UINT uiNumVertices, UINT uiStartIndex, UINT uiPrimitiveCount);

//! \copydoc g_func_mtl_set
SX_LIB_API void SGCore_MtlSet(ID id, float4x4 *pWorld);

//! \copydoc g_func_mtl_load
SX_LIB_API ID SGCore_MtlLoad(const char *szName, int iMtlType);

//! \copydoc g_func_mtl_get_sort
SX_LIB_API int SGCore_MtlGetSort(ID id);

//! \copydoc g_func_mtl_get_physic_type
SX_LIB_API int SGCore_MtlGetPhysicType(ID id);

//! \copydoc g_func_mtl_group_render_is_singly
SX_LIB_API bool SGCore_MtlGroupRenderIsSingly(ID id);

//!@}

//##########################################################################

/*! \name Переопределение функций
!@{*/

//! переназначение g_func_dip
SX_LIB_API void SGCore_SetFunc_DIP(g_func_dip fnFunc);

//! переназначение g_func_mtl_set
SX_LIB_API void SGCore_SetFunc_MtlSet(g_func_mtl_set fnFunc);

//! переназначение g_func_mtl_load
SX_LIB_API void SGCore_SetFunc_MtlLoad(g_func_mtl_load fnFunc);

//! переназначение g_func_mtl_get_sort
SX_LIB_API void SGCore_SetFunc_MtlGetSort(g_func_mtl_get_sort fnFunc);

//! переназначение g_func_mtl_get_physic_type
SX_LIB_API void SGCore_SetFunc_MtlGetPhysicType(g_func_mtl_get_physic_type fnFunc);

//! переназначение g_func_mtl_group_render_is_singly
SX_LIB_API void SGCore_SetFunc_MtlGroupRenderIsSingly(g_func_mtl_group_render_is_singly fnFunc);

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
@{*/

/*! \name Ограничения на размеры
@{*/

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

//**************************************************************************

//! типы шейдеров (int type_shader)
enum SHADER_TYPE
{
	SHADER_TYPE_VERTEX,	//!< вершинный
	SHADER_TYPE_PIXEL	//!< пиксельный
};

//! типы проверок дубликатов шейдеров
enum SHADER_CHECKDOUBLE
{
	SHADER_CHECKDOUBLE_NONE,	//!< нет проверки
	SHADER_CHECKDOUBLE_PATH,	//!< проверка по пути (имени шейдера с расширением)
	SHADER_CHECKDOUBLE_NAME		//!< проверка по пользовательскому имени
};

//**************************************************************************

//! загрузить все шейдеры поставленные в очередь
SX_LIB_API void SGCore_ShaderAllLoad();

//! поставить шейдер в очередь загрузки
SX_LIB_API ID SGCore_ShaderLoad(
	SHADER_TYPE type_shader,		//!< тип шейдера
	const char *szPath,				//!< имя файла шейдера с расширением
	const char *szName,				//!< имя шейдера которое присвоится при загрузке
	SHADER_CHECKDOUBLE check_double,//!< проверять ли на уникальность
	D3DXMACRO *pMacro = 0			//!< макросы
	);

//! существует ли файл name в папке с шейдерами
SX_LIB_API bool SGCore_ShaderFileExists(const char *szName);

//! записывает пользовательское имя шейдера в name
SX_LIB_API void SGCore_ShaderGetName(SHADER_TYPE type_shader, ID idShader, char *szName);

//! записывает имя шейдер с расширением в path
SX_LIB_API void SGCore_ShaderGetPath(SHADER_TYPE type_shader, ID idShader, char *szPath);

//! существует ли шейдер с пользовательским именем name, если да то возвращает id
SX_LIB_API ID SGCore_ShaderExistsName(SHADER_TYPE type_shader, const char *szName);

//! существует ли шейдер с именем файла и расширением name, если да то возвращает id
SX_LIB_API ID SGCore_ShaderExistsPath(SHADER_TYPE type_shader, const char *szPath);

//! загружен ли шейдер с данным id
SX_LIB_API bool SGCore_ShaderIsValidated(SHADER_TYPE type_shader, ID idShader);


//! обновление шейдера по имени
SX_LIB_API void SGCore_ShaderUpdateN(SHADER_TYPE type_shader, const char *szName);

//! обновление шейдера по id
SX_LIB_API void SGCore_ShaderUpdate(SHADER_TYPE type_shader, ID idShader);

//! перезагрузить все шейдеры, с учетом макросов
SX_LIB_API void SGCore_ShaderReloadAll();	


//! получить идентификатор шейдера по имени
SX_LIB_API ID SGCore_ShaderGetID(SHADER_TYPE type_shader, const char *szName);

//! бинд шейдера по имени
SX_LIB_API void SGCore_ShaderBindN(SHADER_TYPE type_shader, const char *szName);

//! бинд шейдера по id
SX_LIB_API void SGCore_ShaderBind(SHADER_TYPE type_shader, ID idShader);


//! обнуление биндов шейдеров
SX_LIB_API void SGCore_ShaderUnBind();	

//**************************************************************************

/*! \name Передача данных в шейдер
@{*/

//! передача float значений в шейдер по имени
SX_LIB_API void SGCore_ShaderSetVRFN(
	SHADER_TYPE type_shader,	//!< тип шейдера из #SHADER_TYPE
	const char *szNameShader,	//!< пользовательское имя шейдера
	const char *szNameVar,		//!< имя переменной которой присваивается значение
	void *pData,				//!< указатель на массив данных
	int iCountFloat4 = 0		//!< количество float4 значений в передаваемом массиве, если меньше одного значит количество будет взято из шейдера
	);

//! передача float значений в шейдер по ID
SX_LIB_API void SGCore_ShaderSetVRF(
	SHADER_TYPE type_shader,	//!< тип шейдера из #SHADER_TYPE
	ID idShader,				//!< идентификатор шейдера
	const char *szNameVar,		//!< имя переменной которой присваивается значение
	void *pData,				//!< указатель на массив данных
	int iCountFloat4 = 0		//!< количество float4 значений в передаваемом массиве, если меньше одного значит количество будет взято из шейдера
	);

//! передача int значений в шейдер по имени
SX_LIB_API void SGCore_ShaderSetVRIN(
	SHADER_TYPE type_shader,	//!< тип шейдера из #SHADER_TYPE
	const char *szNameShader,	//!< пользовательское имя шейдера
	const char *szNameVar,		//!< имя переменной которой присваивается значение
	void *pData,				//!< указатель на массив данных
	int iCountInt4 = 0			//!< количество int4 значений в передаваемом массиве, если меньше одного значит количество будет взято из шейдера
	);

//! передача int значений в шейдер по ID
SX_LIB_API void SGCore_ShaderSetVRI(
	SHADER_TYPE type_shader,	//!< тип шейдера из #SHADER_TYPE
	ID idShader,				//!< идентификатор шейдера
	const char *szNameVar,		//!< имя переменной которой присваивается значение
	void *pData,				//!< указатель на массив данных
	int iCountInt4 = 0			//!< количество int4 значений в передаваемом массиве, если меньше одного значит количество будет взято из шейдера
	);

//!@}
//!@} sxgcore_shaders

//#############################################################################

/*! \defgroup sxgcore_loadtex Загружаемые текстуры
 \ingroup sxgcore
 \note Распознавание текстур (обращение через функции) происходит на основе идентификаторов представленных типом ID. \n
Имя текстуры обязательно долно содержать нижний пробел (_), строка до первого нижнего проблема это имя папки в котором находится текстура с целым именем. \n
Например: mtl_tex.dds - лежит по загружаемому пути: /mtl/mtl_tex.dds
@{*/

/*! \name Ограничения на размеры
@{*/

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

//**************************************************************************

/*! \name Типы материалов
!@{*/

#define MTL_TYPE_GEOM 0		/*!< статическая геометрия */
#define MTL_TYPE_GRASS 1	/*!< растительность - трава */
#define MTL_TYPE_TREE 2		/*!< растительность - деревья */
#define MTL_TYPE_SKIN 3		/*!< скелетная модель */
//#define MTL_LIGHT 4

//!@}

//! типы текстур
enum LOAD_TEXTURE_TYPE
{
	LOAD_TEXTURE_TYPE_LOAD,		//!< загружаемая
	LOAD_TEXTURE_TYPE_CONST,	//!< неудаляемая загружаемая
	LOAD_TEXTURE_TYPE_CUSTOM,	//!< созданная пользователем

	/*! самоопределение типа, на тот случай когда обновляем текстуру которая точно есть.
	Если определить этот тип, а внутри у текстуры на самом деле нет типа (скорее всего нет текстуры)
	то будет определен LOAD_TEXTURE_TYPE_LOAD
	*/
	LOAD_TEXTURE_TYPE_SELF,
};

//**************************************************************************

//! существует ил файл name в папке с текстурами
SX_LIB_API bool SGCore_LoadTexFileExists(const char *szName);	

//! очистить список загружаемых текстур
SX_LIB_API void SGCore_LoadTexClearLoaded();				

//! удалить тектуру по id (независимо от типа)
SX_LIB_API void SGCore_LoadTexDelete(ID idTexture);				

//! добавляем имя текстуры, взамен получаем на нее ID (поставить в очередь)
SX_LIB_API ID SGCore_LoadTexAddName(const char *szName, LOAD_TEXTURE_TYPE type);

//! получить id по имени
SX_LIB_API ID SGCore_LoadTexGetID(const char *szName);

//! получить имя по id
SX_LIB_API void SGCore_LoadTexGetName(ID idTexture, char *szName);


/*! создать место для текстуры tex и присвоить ей имя name, возвращает id
 \warning создавать текстур необходимо в managed pool (D3DPOOL_MANAGED) ибо обработка потери и восстановления устройства сюда не приходит
*/
SX_LIB_API ID SGCore_LoadTexCreate(const char *szName, IDirect3DTexture9 *pTexture);

/*! обновить/перезагрузить текстуру name, если текстуры не было в списке то добавляет.
Если текстуру надо обновить, но тип у нее заранее не известен, но она точно уже загружена  
то можно использовать тип самоопределения LOAD_TEXTURE_TYPE_SELF, тогда тип текстуры не изменится
*/
SX_LIB_API ID SGCore_LoadTexUpdateN(const char *szName, LOAD_TEXTURE_TYPE type);

//! обновить/перезагрузить текстуру
SX_LIB_API void SGCore_LoadTexUpdate(ID idTexture);

//! возвращает текстуру по id
SX_LIB_API IDirect3DTexture9* SGCore_LoadTexGetTex(ID idTexture);

//! загрузка всех текстур поставленных в очередь, если есть очередь
SX_LIB_API void SGCore_LoadTexAllLoad();	

//!@} sxgcore_loadtex

//#############################################################################

/*! \defgroup sxgcore_rt Render targets
 \ingroup sxgcore
 \note Cброс и восстановление устройства сюда приходят
@{*/

//! добавить новый render target
SX_LIB_API ID SGCore_RTAdd(
	UINT iWidth,			//!< ширина
	UINT iHeight,			//!< высота
	UINT iLevels,			//!< количество mip-map уровней
	DWORD dwUsage,			//!< признак применения, возможные значения из D3DUSAGE_
	D3DFORMAT format,		//!< формат из D3DFORMAT
	D3DPOOL pool,			//!< где будет размещена текстура, из D3DPOOL
	const char *szName,		//!< имя rt
	/*! коэфициент размеров rt относительно области рендера 
	(если rt и размер области одинаковы то 1, если rt меньша на 0.5 то 0.5) 
	если указаны фиксированные значения то ставить 0, это нужно для устновления размеров при восстановлении устройства
	*/
	float fCoefFullScreen
	);

//! удалить rt по имени
SX_LIB_API void SGCore_RTDeleteN(const char *szName);	

//! удалить rt по id
SX_LIB_API void SGCore_RTDelete(ID id);				

//! возвращает id по имени
SX_LIB_API ID SGCore_RTGetNum(const char *szName);

//! возвращает текстуру по имени
SX_LIB_API IDirect3DTexture9* SGCore_RTGetTextureN(const char *szName);

//! возвращает текстуру по id
SX_LIB_API IDirect3DTexture9* SGCore_RTGetTexture(ID id);				

//!@} sxgcore_rt

//#############################################################################

/*! \defgroup sxgcore_dse_static Статическая модель dse формата
 \ingroup sxgcore
 \note sxgcore предоставляет возможность загрузки статических моделей, формат вершин которых представлен структурой #vertex_static, которая объявлена в файле ModelFile.h
 \todo Описать формат файла статической модели dse
@{*/

//! структура статической модели dse
struct ISXDataStaticModel : public IBaseObject
{
	virtual ~ISXDataStaticModel(){};

	virtual ISXDataStaticModel* getCopy()=0;//!< получить абсолютную копию модели
	
	IDirect3DVertexBuffer9 *m_pVertexBuffer;//!< вершиный буфер
	IDirect3DIndexBuffer9 *m_pIndexBuffer;	//!< индексный буфер

	UINT m_uiSubsetCount;		//!< количество подгрупп
	char **m_ppTextures;		//!< массив имен текстур без расширения
	UINT *m_pStartIndex;		//!< массив стартовых позиций индексов для каждой подгруппы
	UINT *m_pIndexCount;		//!< массив количества индексов для каждой подгруппы
	UINT *m_pStartVertex;		//!< массив стартовых позиций вершин для каждой подгруппы
	UINT *m_pVertexCount;		//!< массив количества вершин для каждой подгруппы
	UINT m_uiAllIndexCount;		//!< общее количество индексов
	UINT m_uiAllVertexCount;	//!< общее количество вершин

	float4_t m_vBSphere;
	float3_t m_vBBMax, m_vBBMin;
};

//! создать статическую модель
SX_LIB_API ISXDataStaticModel* SGCore_StaticModelCr();	

//! загрузить статическую модель, data инициализируется внутри
SX_LIB_API void SGCore_StaticModelLoad(const char *szFile, ISXDataStaticModel **ppData);	

//! сохранить статическую модель
SX_LIB_API void SGCore_StaticModelSave(const char *szFile, ISXDataStaticModel **pData);

//! возвращает декларацию вершин статической модели
SX_LIB_API IDirect3DVertexDeclaration9* SGCore_StaticModelGetDecl();	

//!@} sxgcore_dse_static

//#############################################################################

/*! \defgroup sxgcore_bb Ограничивающий объем
 \ingroup sxgcore
@{*/

/*! Простой объект трансформаций с минимальным описанием.
 \note Для корректного использования необходимо сначала установить позицию/поворот/масштаб после чего CalculateWorld
*/
struct ISXTransObject : public IBaseObject
{
	virtual ~ISXTransObject(){};

	SX_ALIGNED_OP_MEM

	//! просчет мировой матрицы трансформации на основе поворотов масштабирования и позиции
	virtual inline float4x4* calcWorld() = 0;	

	float3 m_vPosition;	//!< позиция
	float3 m_vRotation;	//!< повороты
	float3 m_vScale;	//!< масштабирование
	float4x4 m_mWorld;	//!< мировая матрица на основе поворотов масштабирования и позиции
};

//! создать ISXTransObject
SX_LIB_API ISXTransObject* SGCore_CrTransObject();	

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
	virtual void calcBound(
		IDirect3DVertexBuffer9 *pVertexBuffer, //!< вершинный буфер (незаблокированный), в вершинах которого первым элементом идет позиция float3_t вектор  
		DWORD dwCountVertex,	//!< количество вершин
		DWORD dwBytePerVertex	//!< количество байт в вершине
		) = 0;

	//! функция просчета мировой матрицы и трансформации минимума и максимума
	virtual float4x4*  calcWorldAndTrans() = 0;

	//! просчет структуры SXPosBBScreen 
	virtual void getPosBBScreen(
		SXPosBBScreen *res,		//!< инициализированная стркутура #SXPosBBScreen для записи
		float3* campos,			//!< позиция наблюдателя
		float3* sizemapdepth,	//!< размер карты глубины глябины для просчета float3(ширина, высота, максильная глубина)
		float4x4* mat			//!< произведение видовой и проекционной матриц
		) = 0;

	//! установить экстремум, также просчитает и сферу
	virtual void setMinMax(const float3 *pMin, const float3 *pMax) = 0;	

	//! запишет в min и max точки экстремума
	virtual void getMinMax(float3 *pMin, float3 *pMax) const = 0;			


	//! установить сферу, просчитает также и параллелепипед
	virtual void setSphere(const float3 *pCenter, float fRadius) = 0;	

	//! запишет в center центр сферы, в radius радиус сферы
	virtual void getSphere(float3 *pCenter, float *pRadius) const = 0;	


	//! находится ли точка point в пределах сферы
	virtual bool isPointInSphere(const float3 *pPoint) const = 0;	

	//! находится ли точка point в пределах параллелепипеда
	virtual bool isPointInBox(const float3 *pPoint) const = 0;		
};

//! создать ISXBound
SX_LIB_API ISXBound* SGCore_CrBound(); 

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

//##########################################################################

/*! \defgroup sxgcore_bb_intersect Функции просчета попаданий точек в объемы и деление объемов
 \ingroup sxgcore
@{*/

/*! \name Просчеты попадания точки/точек в объем 
 \note 2d - на основании x и z координат \n
3d - на основании всех трех координат \n
Abs - абсолютное нахождение внутри, не на границах, иное допускает нахождение на границах 
@{*/

//! находится ли точка pos в пределах [min,max] по осям x z
SX_LIB_API bool SGCore_0InPos2D(float3* min, float3* max, float3* pos);		

//! находится ли точка pos в пределах (min,max) по осям x z
SX_LIB_API bool SGCore_0InPosAbs2D(float3* min, float3* max, float3* pos);	


//! возвращает количество точек (p1,p2,p3) лежащих в пределах [min,max]  по осям x z
SX_LIB_API int SGCore_0CountPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);

//! возвращает количество точек (p1,p2,p3) лежащих в пределах (min,max)  по осям x z
SX_LIB_API int SGCore_0CountPosPointsAbs2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);

//! лежит ли хотя бы одна точка абсолютно в (min,max) или хотя бы 2 точки в пределах [min,max], из числа трех точек p1,p2,p3, по осям x z
SX_LIB_API bool SGCore_0InPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);


//! находится ли точка pos в пределах [min,max]
SX_LIB_API bool SGCore_0InPos3D(float3* min, float3* max, float3* pos);		

//! находится ли точка pos в пределах (min,max)
SX_LIB_API bool SGCore_0InPosAbs3D(float3* min, float3* max, float3* pos);	


//! возвращает количество точек (p1,p2,p3) лежащих в пределах [min,max]
SX_LIB_API int SGCore_0CountPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);

//! возвращает количество точек (p1,p2,p3) лежащих в пределах (min,max)
SX_LIB_API int SGCore_0CountPosPointsAbs3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);

//! лежит ли хотя бы одна точка абсолютно в (min,max) или хотя бы 2 точки в пределах [min,max], из числа трех точек p1,p2,p3
SX_LIB_API bool SGCore_0InPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);

//!@}

//**************************************************************************

/*! кубическое (octo) деление объема (по всем трем осям)
 \warning массивы и объекты должны быть заранее инициализированны
 */
SX_LIB_API void SGCore_0ComBoundBoxArr8(ISXBound* bound, ISXBound** bound_arr);

/*! квадратичное (quad) деление объема (по двум осям x и z)
 \warning массивы и объекты должны быть заранее инициализированны
*/
SX_LIB_API void SGCore_0ComBoundBoxArr4(ISXBound* bound, ISXBound** bound_arr);

//! находит квадрат расстояния между лучем и точкой
SX_LIB_API float SGCore_0DistancePointBeam2(const float3 & p, const float3 & start, const float3 & dir);

//! возвращает пересекаются ли боксы или нет
SX_LIB_API bool SGCore_0InretsectBox(const float3 * min1, const float3 * max1, const float3 * min2, const float3 * max2);

struct SXTriangle
{
	float3_t a;
	float3_t b;
	float3_t c;
	SXTriangle()
	{
	};
	SXTriangle(float3_t _a, float3_t _b, float3_t _c) :a(_a), b(_b), c(_c)
	{
	};

	//Проверкка пересечения треугольника и отрезка
	bool IntersectLine(const float3 & l1, const float3 &l2, float3 * p)
	{
		float3 n = SMVector3Normalize(SMVector3Cross((b - a), (c - b)));
		float d1 = SMVector3Dot((l1 - a), n) / SMVector3Length(n);
		float d2 = SMVector3Dot((l2 - a), n) / SMVector3Length(n);
		if ((d1 > 0 && d2 > 0) || (d1 < 0 && d2 < 0))
			return(false);
		*p = l1 + (l2 - l1) * (-d1 / (d2 - d1));
		if (SMVector3Dot(SMVector3Cross((b - a), (*p - a)), n) <= 0) return(false);
		if (SMVector3Dot(SMVector3Cross((c - b), (*p - b)), n) <= 0) return(false);
		if (SMVector3Dot(SMVector3Cross((a - c), (*p - c)), n) <= 0) return(false);
		return(true);
	}
};


//!@} sxgcore_bb_intersect

//#############################################################################

/*! \defgroup sxgcore_camera Камера
 \ingroup sxgcore
 \todo Добавить в библиотеку математики плоскости и операции с ними, заменить dx плоскости на свои
 \todo Возможно надо расширить возможности и абстракцию класса камеры, к примеру матрицу проекции хранить тоже в камере, и сделать отдельные функции для ее модификации
@{
*/

//! структура описание плоскости
struct CSXFrustumPlane
{
	float3_t m_vNormal;
	float m_fDistance;

	SX_ALIGNED_OP_MEM

	void normalize()
	{
		float fDenom = sqrt((m_vNormal.x*m_vNormal.x) + (m_vNormal.y*m_vNormal.y) + (m_vNormal.z*m_vNormal.z));
		m_vNormal.x = m_vNormal.x / fDenom;
		m_vNormal.y = m_vNormal.y / fDenom;
		m_vNormal.z = m_vNormal.z / fDenom;
		m_fDistance = m_fDistance / fDenom;
	}
};

//! класс описывающий фрустум
class ISXFrustum : public IBaseObject
{
public:
	virtual ~ISXFrustum(){};

	SX_ALIGNED_OP_MEM

	//! обновление фрустума, на вход матрицы по которым необходимо построить фрустум
	virtual void update(
		const float4x4 *pView,	//<! видовая матрица
		const float4x4 *pProj	//<! проекционная матрица
		) = 0;

	//! находится ли точка во фрустуме
	virtual bool pointInFrustum(const float3 *pPoint) const = 0;	

	//! находится ли треугольник во фрутстуме
	virtual bool polyInFrustum(const float3 *pPoint1, const float3 *pPoint2, const float3 *pPoint3) const = 0;		

	//! находится ли полигон во фрустуме полностью
	virtual bool polyInFrustumAbs(const float3 *pPoint1, const float3 *pPoint2, const float3 *pPoint3) const = 0;	


	//! находится ли полигон во фрустуме
	virtual bool sphereInFrustum(const float3 *pPoint, float fRadius) const = 0;	

	//! находится ли сфера во фрустуме полностью
	virtual bool sphereInFrustumAbs(const float3 *pPoint, float fRadius) const = 0;	

	//! находится ли параллелепипед (описанный точками экстремума) во фрустуме
	virtual bool boxInFrustum(float3 *pMin, float3 *pMax) const = 0;				


	//! возвращает координаты точки фрустума, iNumPoint = [0,7]
	virtual float3 getPoint(int iNumPoint) const = 0;

	//! возвращает координаты центра фрустума
	virtual float3 getCenter() const = 0;

	//! устанавливает координаты точки фрустума, iNumPoint = [0,7]
	virtual void setPoint(int iNumPoint, const float3 *pPoint) = 0;

	//! устанавливает координаты центра фрустума
	virtual void setCenter(const float3 *pCenter) = 0;
};

//! создать ISXFrustum
SX_LIB_API ISXFrustum* SGCore_CrFrustum(); 

//**************************************************************************

//! камера
class ISXCamera : public IBaseObject
{
public:
	virtual ~ISXCamera(){};

	SX_ALIGNED_OP_MEM

	/*! \name Движение
	 \note В метрах
	@{*/

	//! влево/вправо
	virtual void posLeftRight(float fUnits) = 0;	

	//! вверх/вниз
	virtual void posUpDown(float fUnits) = 0;		

	//! вперед/назад
	virtual void posFrontBack(float fUnits) = 0;	

	//!@}

	/*! \name Вращение
	 \note В радианах
	@{*/

	//! вращение вверх/вниз
	virtual void rotUpDown(float fAngle) = 0;		

	//! вращение вправо/влево
	virtual void rotRightLeft(float fAngle) = 0;	

	//! крен
	virtual void roll(float fAngle) = 0;			

	//! установить полное вращение
	virtual void setOrientation(const SMQuaternion *pOrientation) = 0; 

	//!@}

	//! получаем матрицу вида в pMatrix
	virtual void getViewMatrix(float4x4 *pMatrix) = 0;

	/*! \name Базис
	@{
	*/

	//! в pos записывает текущую позицию в мире
	virtual void getPosition(float3 *pPos) const = 0;	

	//! устанавливает позицию в мире
	virtual void setPosition(const float3 *pPos) = 0;	


	//! в right записывает парвый вектор
	virtual void getRight(float3 *pRight) const = 0;

	//! в up записывает верхний вектор
	virtual void getUp(float3 *vUp) const = 0;		

	//! в look записывает вектор направление взгляда
	virtual void getLook(float3 *vLook) const = 0;	


	//! в rot записывает углы поворотов по осям, в радианах
	virtual void getRotation(float3 *vRot) const = 0;	

	//повроты по осям

	//! возвращает поворот по оси X, в радианах
	virtual float getRotationX() const = 0;	

	//! возвращает поворот по оси Y, в радианах
	virtual float getRotationY() const = 0;	

	//! возвращает поворот по оси Z, в радианах
	virtual float getRotationZ() const = 0;	

	//!@}

	//! устанавливает FOV камеры
	virtual void setFOV(float fFOV) = 0;

	//! возвращает FOV камеры
	virtual float getFOV() const = 0;	

	//! обновление фрустума камеры
	virtual void updateFrustum(const float4x4 *mProjection) = 0;

	//! возвращает константный указатель фрустума
	virtual const ISXFrustum* getFrustum() = 0;
};

//! создать ISXCamera
SX_LIB_API ISXCamera* SGCore_CrCamera();	

//!@} sxgcore_camera

//#############################################################################

/*! \defgroup sxgcore_sky Небо
 \ingroup sxgcore
@{*/

/*! \name SkyBox
 \note Используются кубические текстуры
@{*/

//! создание
SX_LIB_API void SGCore_SkyBoxCr();	

//! инициализирован ли skybox
SX_LIB_API bool SGCore_SkyBoxIsCr();

//! загружена ли текстура?
SX_LIB_API bool SGCore_SkyBoxIsLoadTex();


//! загрузка текстуры, texture - имя текстуры с расширением
SX_LIB_API void SGCore_SkyBoxLoadTex(const char *szTexture);	

//! смена текстуры, texture - имя текстуры с расширением
SX_LIB_API void SGCore_SkyBoxChangeTex(const char *szTexture);

//! в аргумент записывает путь до текущей активной текстуры
SX_LIB_API void SGCore_SkyBoxGetActiveTex(char *szTexture);

//! в аргумент записывает путь до следующей текстуры 9если включена смена)
SX_LIB_API void SGCore_SkyBoxGetSecondTex(char *szTexture);


//! установка угла поворота angle по оси y, в радианах
SX_LIB_API void SGCore_SkyBoxSetRot(float fAngle);	

//! возвращает угол поворота по оси y, в радианах
SX_LIB_API float SGCore_SkyBoxGetRot();				


//! установка цвета окраски в пределах 0-1, альфа компонента (w) - на сколько будет окрашен
SX_LIB_API void SGCore_SkyBoxSetColor(const float4_t *pColor);	

//! в color записывает текущий цвет окраски
SX_LIB_API void SGCore_SkyBoxGetColor(float4_t *pColor);	

//! рендер скайбокса
SX_LIB_API void SGCore_SkyBoxRender(
	float timeDelta,	//!< время рендера кадра в млсек 
	const float3 *pPos	//!< позиция набладателя, эта же позиция будет центром skybox
	);

//!@}

//**************************************************************************

/*! \name SkyClouds

 \note Простая плоскость параллельная xz на которую зеркально (х2) натягивается текстура, в постоянном движении.
 Положение констатно.
 Используются обычные 2д текстуры.
@{*/

//! создание
SX_LIB_API void SGCore_SkyCloudsCr();		

//! инициализирован ли sky clouds
SX_LIB_API bool SGCore_SkyCloudsIsCr();		

//! загружена ли текстура
SX_LIB_API bool SGCore_SkyCloudsIsLoadTex();


/*! установка размеров и позиции.
 Так как позиция облаков константна то чтобы была илюзия полного покрытия уровня, необходимо облакам указывать размер в несколько раз больше чем весь доступный уровень, к примеру x2
*/
SX_LIB_API void SGCore_SkyCloudsSetWidthHeightPos(
	float fWidth,			//!< ширина в метрах
	float fHeight,			//!< высота в метрах
	const float3 *pCenter	//!< позиция центра
	);

//! загрузка текстуры, texture - имя текстуры с расширением
SX_LIB_API void SGCore_SkyCloudsLoadTex(const char *szNameTexture);	

//! плавная смена текстуры, texture - имя текстуры с расширением
SX_LIB_API void SGCore_SkyCloudsChangeTex(const char *szNameTexture);


//! установка угла поворота angle по оси y, в радианах
SX_LIB_API void SGCore_SkyCloudsSetRot(float fAngle);

//! возвращает текущий угол поворота по оси y, в радианах
SX_LIB_API float SGCore_SkyCloudsGetRot();			


//! устанавливает коэфициент прозрачности, в пределах 0-1
SX_LIB_API void SGCore_SkyCloudsSetAlpha(float fAlpha);	

//! возвращает текущий коэфициент прозрачности
SX_LIB_API float SGCore_SkyCloudsGetAlpha();			


//! устанавливает коэфициент скорости движения
SX_LIB_API void SGCore_SkyCloudsSetSpeed(float fSpeed);	

//! возвращает текущий коэфициент скорости движения
SX_LIB_API float SGCore_SkyCloudsGetSpeed();			


//! установка цвета окраски в пределах 0-1, альфа компонента (w) - на сколько будет окрашен
SX_LIB_API void SGCore_SkyCloudsSetColor(const float4_t *pColor);

//! в color записывает текущий цвет окраски
SX_LIB_API void SGCore_SkyCloudsGetColor(float4_t *pColor);

//! рендер облаков
SX_LIB_API void SGCore_SkyCloudsRender(
	DWORD timeDetlta,		//!< время рендера кадра в млсек
	const float3* pos,		//!< позиция набладателя
	bool isShadow			//!< для теней ли рендерим?
	);

//!@}
//!@} sxgcore_sky

#endif

/*! @} */
