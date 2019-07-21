
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

//#include <d3d9.h>
//#include <d3dx9.h>
//#include <dxerr9.h>
#include <graphix/graphix.h>

#define SGCore_ShaderSetVRF(...) assert(!"Deprecated!")

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

class IFrustum;

//! \name Базовые функции библиотеки 
//!@{

//! версия подсистемы
SX_LIB_API long SGCore_0GetVersion();			

//! установка функции вывода сообщений
SX_LIB_API void SGCore_Dbg_Set(report_func fnReport);	

//! инициализация подсистемы
SX_LIB_API void SGCore_0Create(
	const char *szName,			//!< передваваемое имя подсистемы
	SXWINDOW hWnd,					//!< дескриптор окна в которое будет осуществляться рендер
	int iWidth,					//!< ширина области рендера
	int iHeigth,				//!< высота области рендера
	bool isWindowed,			//!< оконный режим использовать ли? иначе полноэкранный
	bool isUnic = true			//!< должна ли подсистема быть уникальной на основе имени
	);

//! возвращает HWND окна рендера
SX_LIB_API SXWINDOW SGCore_GetHWND();

//! уничтожение либы
SX_LIB_API void SGCore_AKill();	

//! возвращает dx устройство
SX_LIB_API IGXContext* SGCore_GetDXDevice();	

//! возвращает массив всех доступных разрешений монитора, в iCount записывает размер массива
SX_LIB_API const DEVMODE* SGCore_GetModes(int *iCount);

//! Проверяет успешность вызова по HRESULT, выводит сообщение об ошибке
SX_LIB_API HRESULT SGCore_DXcallCheck(HRESULT hr, const char *callStr);
#define DX_CALL(call) SGCore_DXcallCheck((call), #call)

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
По умолчанию установка текстуры в нулевой текстурный слот, id – идентификатор материала (по умолчанию - текстуры), pWorld – матрица трансформации модели, pColor - цвет, если материал принимает
*/
typedef void(*g_func_mtl_set) (ID id, const float4x4 *pWorld, const float4 *pColor);

/*! загрузка материала, szName - имя текстуры с расширением, iMtlType - тип материала на случай провала загрузки.
Загрузка материала (по умолчанию – текстуры) с именем name, iMtlType – типа материала, 
будет задействован только в случае провала определения типа материала (по умолчанию не используется) - 
тип стандартного материала будет определен на основании iMtlType, может принимать одно из значений MTL_TYPE_
*/
typedef ID(*g_func_mtl_load) (const char *szName,	int iMtlType);

//! является ли материал полупрозрачным
typedef bool(*g_func_mtl_is_transparency) (ID id);

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
SX_LIB_API void SGCore_MtlSet(ID id, const float4x4 *pWorld=0, const float4 *pColor=0);

//! \copydoc g_func_mtl_load
SX_LIB_API ID SGCore_MtlLoad(const char *szName, int iMtlType);

//! \copydoc g_func_mtl_is_transparency
SX_LIB_API bool SGCore_MtlIsTransparency(ID id);

//! \copydoc g_func_mtl_get_physic_type
SX_LIB_API int SGCore_MtlGetPhysicType(ID id);

//! \copydoc g_func_mtl_group_render_is_singly
SX_LIB_API bool SGCore_MtlGroupRenderIsSingly(ID id);

//!@}

//#############################################################################

/*! \defgroup sxgcore_ds Render targets для deferred shading
\ingroup sxgcore
@{*/

//! типы render targets
enum DS_RT
{
	//! цвет rgba8
	DS_RT_COLOR = 0,

	//! нормали (включая микрорельеф) rfb10a2
	DS_RT_NORMAL,

	//! параметры освещения rgba8
	DS_RT_PARAM,

	//! глубина r16f, непрозрачные материалы
	DS_RT_DEPTH,

	//! глубина r16f, непрозрачные материалы
	DS_RT_DEPTH0,

	//! глубина r16f, непрозрачные и полупрозрачные пиксели, однако прозрачные (если есть) будут перекрывать непрозрачные, и в этом случае их глубина будет 1 то есть максимальной(для идентификации)
	DS_RT_DEPTH1,


	//! эмбиент цвет (цвет света rgb) и диффузное освещение (a) rgba16f
	DS_RT_AMBIENTDIFF,

	//! блики r16f
	DS_RT_SPECULAR,

	//! освещенная сцена rgba16
	DS_RT_SCENELIGHT,

	//! освещенная сцена rgba16
	DS_RT_SCENELIGHT2,

	//! текущий rt адаптации глаза к освещению r16f
	DS_RT_ADAPTEDLUM
};

SX_LIB_API void SGCore_ToneMappingCom(DWORD timeDelta, float factor_adapted);

//!@} sxgcore_ds

//##########################################################################

/*! \name Переопределение функций
!@{*/

//! переназначение #g_func_dip
SX_LIB_API void SGCore_SetFunc_DIP(g_func_dip fnFunc);

//! переназначение #g_func_mtl_set
SX_LIB_API void SGCore_SetFunc_MtlSet(g_func_mtl_set fnFunc);

//! переназначение #g_func_mtl_load
SX_LIB_API void SGCore_SetFunc_MtlLoad(g_func_mtl_load fnFunc);

//! переназначение #g_func_mtl_is_transparency
SX_LIB_API void SGCore_SetFunc_MtlIsTransparency(g_func_mtl_is_transparency fnFunc);

//! переназначение #g_func_mtl_get_physic_type
SX_LIB_API void SGCore_SetFunc_MtlGetPhysicType(g_func_mtl_get_physic_type fnFunc);

//! переназначение g_func_mtl_group_render_is_singly
SX_LIB_API void SGCore_SetFunc_MtlGroupRenderIsSingly(g_func_mtl_group_render_is_singly fnFunc);

//!@}

//!@} group sxgcore_redefinition_func

//#############################################################################

/*! \name Occlusion culling - отбсрос загороженного
 \note Глубина должна быть нелинейная, то есть z/w, чтобы преобразовать в линейную надо:
 near / (far + near - depth * (far - near))
 \note Процесс вкратце:
  - в конце этого кадра берем глубину, уменьшаем в несколько раз (в 4) с выборкой наиболее дальних пикселей, перегоняем данные из текстуры в gpu в массив на cpu
  - в начале следующего кадра, после обновления матриц, делаем репроекцию глубины, закрашивая пустые места максимальной глубиной
  - в следующем кадре тестируем боксы на незагороженность, путем растеризации треугольников и сравнения глубины пикселей с пикселями глубины
@{*/

//! будет ли проводится тест на загороженность
SX_LIB_API void SGCore_OC_SetEnable(bool isEnable);

//! обновление буфера глубины для теста, должна вызываться в старом кадре, к примеру после всего рендера (глубина и матрицы для текущего)
SX_LIB_API void SGCore_OC_Update(ID idDepthMap, const IFrustum *pFrustum);

//! Ожидает завершение обновления (для многопоточного режима)
SX_LIB_API void SGCore_OC_UpdateEnsureDone();

//! репроекция глубины, должна вызываться в новом кадре до основного рендера всего того, что подвергается тесту на загороженность, матрицы должны быть от нового кадра
SX_LIB_API void SGCore_OC_Reprojection();

/*! не загорожен ли бокс? вызывается для каждого тестируемого объекта после #SGCore_OC_Reprojection и до #SGCore_OC_Update
 \note Координаты бокса должны быть в пространстве мира
*/
SX_LIB_API bool SGCore_OC_IsVisible(const float3 *pMax, const float3 *pMin);

//!@}

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
#define SXGC_SHADER_MAX_SIZE_DIRNAME (SXGC_SHADER_MAX_SIZE_DIR + SXGC_SHADER_MAX_SIZE_NAME)

//! максимальный размер пути до файла шейдера (без имени файла)
#define SXGC_SHADER_MAX_SIZE_STDPATH 256

//! максимальный размер полного пути до шейдера (включая имя шейдера)
#define SXGC_SHADER_MAX_SIZE_FULLPATH (SXGC_SHADER_MAX_SIZE_STDPATH + SXGC_SHADER_MAX_SIZE_DIRNAME)


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
	//! вершинный
	SHADER_TYPE_VERTEX,

	//! пиксельный
	SHADER_TYPE_PIXEL,

	//! геометрический
	SHADER_TYPE_GEOMETRY,

	//! вычислительный
	SHADER_TYPE_COMPUTE
};

//**************************************************************************

//! загрузить все шейдеры поставленные в очередь
SX_LIB_API void SGCore_ShaderAllLoad();

//! поставить шейдер в очередь загрузки
SX_LIB_API ID SGCore_ShaderLoad(
	SHADER_TYPE type_shader,		//!< тип шейдера
	const char *szPath,				//!< имя файла шейдера с расширением
	const char *szName = NULL,		//!< имя шейдера которое присвоится при загрузке
	GXMacro *pMacro = 0			//!< макросы
	);

//! существует ли файл name в папке с шейдерами
SX_LIB_API bool SGCore_ShaderFileExists(const char *szName);

//! записывает имя шейдер с расширением в path
SX_LIB_API void SGCore_ShaderGetPath(SHADER_TYPE type_shader, ID idShader, char *szPath);

//! существует ли шейдер с именем файла и расширением name, если да то возвращает id
SX_LIB_API ID SGCore_ShaderExists(SHADER_TYPE type_shader, const char *szPath, GXMacro *pMacro = 0);

//! загружен ли шейдер с данным id
SX_LIB_API bool SGCore_ShaderIsLoaded(SHADER_TYPE type_shader, ID idShader);


//! перезагрузить все шейдеры, с учетом макросов
SX_LIB_API void SGCore_ShaderReloadAll();	


//! создание набора шейдеров (вершиный и пиксельный)
SX_LIB_API ID SGCore_ShaderCreateKit(ID idVertexShader, ID idPixelShader, ID idGeometryShader = -1, ID idComputeShader = -1);

//! бинд шейдера по id
SX_LIB_API void SGCore_ShaderBind(ID idShaderKit);


//! обнуление биндов шейдеров
SX_LIB_API void SGCore_ShaderUnBind();	

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
#define SXGC_LOADTEX_MAX_SIZE_DIRNAME (SXGC_LOADTEX_MAX_SIZE_DIR + SXGC_LOADTEX_MAX_SIZE_NAME)

//! максимальный размер пути до файла текстуры (без имени файла)
#define SXGC_LOADTEX_MAX_SIZE_STDPATH 256

//! максимальный размер полного пути до текстуры (включая имя текстуры)
#define SXGC_LOADTEX_MAX_SIZE_FULLPATH (SXGC_LOADTEX_MAX_SIZE_STDPATH + SXGC_LOADTEX_MAX_SIZE_DIRNAME)


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

//! статическая геометрия
#define MTL_TYPE_GEOM 0		

//! растительность - трава
#define MTL_TYPE_GRASS 1	

//! растительность - деревья 
#define MTL_TYPE_TREE 2		

//! скелетная модель
#define MTL_TYPE_SKIN 3	

//#define MTL_LIGHT 4

//!@}

//! типы текстур
enum LOAD_TEXTURE_TYPE
{
	//! загружаемая
	LOAD_TEXTURE_TYPE_LOAD,		

	//! неудаляемая загружаемая
	LOAD_TEXTURE_TYPE_CONST,	

	//! созданная пользователем
	LOAD_TEXTURE_TYPE_CUSTOM,	

	/*! самоопределение типа, на тот случай когда обновляем текстуру которая точно есть.
	Если определить этот тип, а внутри у текстуры на самом деле нет типа (скорее всего нет текстуры)
	то будет определен LOAD_TEXTURE_TYPE_LOAD
	*/
	LOAD_TEXTURE_TYPE_SELF,
};

enum SHADER_CONST_REGISTER
{
	SCR_SCENE = 3,
	SCR_CAMERA = 2,
	SCR_OBJECT = 1,
	SCR_SUBSET = 0
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
SX_LIB_API ID SGCore_LoadTexCreate(const char *szName, IGXTexture2D *pTexture);

/*! обновить/перезагрузить текстуру name, если текстуры не было в списке то добавляет.
Если текстуру надо обновить, но тип у нее заранее не известен, но она точно уже загружена  
то можно использовать тип самоопределения LOAD_TEXTURE_TYPE_SELF, тогда тип текстуры не изменится
*/
SX_LIB_API ID SGCore_LoadTexUpdateN(const char *szName, LOAD_TEXTURE_TYPE type);

//! обновить/перезагрузить текстуру
SX_LIB_API void SGCore_LoadTexUpdate(ID idTexture);

//! возвращает текстуру по id
SX_LIB_API IGXTexture2D* SGCore_LoadTexGetTex(ID idTexture);

SX_LIB_API void SGCore_LoadTexSetTex(ID idTexture, IGXTexture2D *pTexture);

//! возвращает текстуру по id
SX_LIB_API IGXTextureCube* SGCore_LoadTexGetTexCube(ID idTexture);

//! загрузка всех текстур поставленных в очередь, если есть очередь
SX_LIB_API void SGCore_LoadTexAllLoad();	

//!@} sxgcore_loadtex

//#############################################################################

/*! \defgroup sxgcore_bb Ограничивающий объем
 \ingroup sxgcore
@{*/

/*! Простой объект трансформаций с минимальным описанием.
 \note Для корректного использования необходимо сначала установить позицию/поворот/масштаб после чего calcWorld
*/
struct ITransObject : public IBaseObject
{
	virtual ~ITransObject(){};

	SX_ALIGNED_OP_MEM

	//! просчет мировой матрицы трансформации на основе поворотов масштабирования и позиции
	virtual const float4x4* calcWorld() = 0;	

	virtual void setPosition(const float3 *pPos) = 0;
	virtual void setRotation(const float3 *pRot) = 0;
	virtual void setScale(const float3 *pScale) = 0;

	virtual const float3* getPosition(float3 *pPos=0) = 0;
	virtual const float3* getRotation(float3 *pRot = 0) = 0;
	virtual const float3* getScale(float3 *pScale = 0) = 0;
};

//! создать ITransObject
SX_LIB_API ITransObject* SGCore_CrTransObject();	

//! структура описывающая ограничивающий квадрат (а точнее параллелепипед) в пространстве экрана
/*struct SXPosBBScreen
{
	float x;		//!< позиция по оси x в пространстве экрана
	float y;		//!< позиция по оси y в пространстве экрана
	float width;	//!< ширина в пикселях
	float height;	//!< высота в пикселях
	float maxdepth;	//!< конец объекта глубина 0-1
	float mindepth;	//!< начало объект глубина 0-1
	bool IsVisible;	//!< виден ли квадрат наблюдателю
};*/

struct CBoundBox
{
	float3_t m_vMin;
	float3_t m_vMax;
};


/*! класс ограничивающего объема
 \warning #setMinMax, #setSphere до вызова #calcWorldAndTrans возвращают нетрансформирвоанные данные
*/
class ISXBound : public virtual ITransObject
{
public:
	virtual ~ISXBound(){};

	SX_ALIGNED_OP_MEM

	/*! Просчет ограничивающего объема по вершинному буферу*/
	virtual void calcBound(
		float3_t *pVertex, //!< вершинный буфер (незаблокированный), в вершинах которого первым элементом идет позиция float3_t вектор  
		int iCountVertex,		//!< количество вершин
		int iBytePerVertex		//!< количество байт в вершине
		) = 0;

	virtual void calcBoundIndex(
		float3_t *pVertexBuffer,
		uint32_t **ppArrIndex, 
		uint32_t *pCountIndex,
		int iCountSubset,
		int iBytePerVertex
		) = 0;

	//! функция просчета мировой матрицы и трансформации минимума и максимума
	//virtual float4x4*  calcWorldAndTrans() = 0;

	virtual void resetTransform() = 0;

	/*//! просчет структуры SXPosBBScreen 
	virtual void getPosBBScreen(
		SXPosBBScreen *res,		//!< инициализированная стркутура #SXPosBBScreen для записи
		float3* campos,			//!< позиция наблюдателя
		float3* sizemapdepth,	//!< размер карты глубины глябины для просчета float3(ширина, высота, максильная глубина)
		float4x4* mat			//!< произведение видовой и проекционной матриц
		) = 0;*/

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

	virtual void cloneFrom(ISXBound *pFrom) = 0;
};

//! создать ISXBound
SX_LIB_API ISXBound* SGCore_CrBound(); 

//!@} sxgcore_bb

//#############################################################################

/*! Меш
*/
struct IMesh: public IBaseObject
{
	virtual ~IMesh() {};
public:
	virtual void AddRef() = 0;
	virtual void draw() = 0;
	virtual IGXVertexBuffer *getVertexBuffer() = 0;
	virtual IGXIndexBuffer *getIndexBuffer() = 0;
	virtual void Release() = 0;
	virtual ISXBound *getBound() = 0;
//	virtual UINT getVertexCount() = 0;
//	virtual UINT getVertexSize() = 0;
};

//! создать IMesh
SX_LIB_API IMesh* SGCore_CrMesh(UINT uVertexCount, UINT uIndexCount, float3_t *pVertices, USHORT *pIndices);
//SX_LIB_API IMesh* SGCore_CloneMesh(IMesh *pOther);

//#############################################################################

//! создание меша (ID3DXMesh) конуса 
SX_LIB_API void SGCore_FCreateCone(
	float fTopRadius,       //!< верхний радиус
	float fBottomRadius,    //!< нижний радиус
	float fHeight,          //!< длина
	IMesh ** ppMesh,        //!< выходной меш
	UINT iSideCount         //!< количество боковых сторон
	);

//! создание меша (ID3DXMesh) сферы 
SX_LIB_API void SGCore_FCreateSphere(
	float fRadius,      //!< радиус
	UINT iSideCount,    //!< количество боковых сторон
	UINT iStackCount,   //!< количество горизонтальных слоев
	IMesh ** ppMesh     //!< выходной меш
	);

//! создание меша (ID3DXMesh) ограничивающего объема
SX_LIB_API void SGCore_FCreateBoundingBoxMesh(const float3* min, const float3* max, IMesh** bbmesh);

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
SX_LIB_API bool SGCore_0InPos2D(const float3 *pMin, const float3 *pMax, const float3 *pPos);

//! находится ли точка pos в пределах (min,max) по осям x z
SX_LIB_API bool SGCore_0InPosAbs2D(float3 *pMin, float3 *pMax, float3 *pPos);


//! возвращает количество точек (p1,p2,p3) лежащих в пределах [min,max]  по осям x z
SX_LIB_API int SGCore_0CountPosPoints2D(float3 *pMin, float3 *pMax, float3 *p1, float3 *p2, float3 *p3);

//! возвращает количество точек (p1,p2,p3) лежащих в пределах (min,max)  по осям x z
SX_LIB_API int SGCore_0CountPosPointsAbs2D(float3 *pMin, float3 *pMax, float3 *p1, float3 *p2, float3 *p3);

//! лежит ли хотя бы одна точка абсолютно в (min,max) или хотя бы 2 точки в пределах [min,max], из числа трех точек p1,p2,p3, по осям x z
SX_LIB_API bool SGCore_0InPosPoints2D(float3 *pMin, float3 *pMax, float3 *p1, float3 *p2, float3 *p3);


//! находится ли точка pos в пределах [min,max]
SX_LIB_API bool SGCore_0InPos3D(float3 *pMin, float3 *pMax, float3 *pPos);

//! находится ли точка pos в пределах (min,max)
SX_LIB_API bool SGCore_0InPosAbs3D(float3 *pMin, float3 *pMax, float3 *pPos);


//! возвращает количество точек (p1,p2,p3) лежащих в пределах [min,max]
SX_LIB_API int SGCore_0CountPosPoints3D(float3 *pMin, float3 *pMax, float3 *p1, float3 *p2, float3 *p3);

//! возвращает количество точек (p1,p2,p3) лежащих в пределах (min,max)
SX_LIB_API int SGCore_0CountPosPointsAbs3D(float3 *pMin, float3 *pMax, float3 *p1, float3 *p2, float3 *p3);

//! лежит ли хотя бы одна точка абсолютно в (min,max) или хотя бы 2 точки в пределах [min,max], из числа трех точек p1,p2,p3
SX_LIB_API bool SGCore_0InPosPoints3D(float3 *pMin, float3 *pMax, float3 *p1, float3 *p2, float3 *p3);

//!@}

//**************************************************************************

/*! кубическое (octo) деление объема (по всем трем осям)
 \warning массивы и объекты должны быть заранее инициализированны
 */
SX_LIB_API void SGCore_0ComBoundBoxArr8(ISXBound *pBound, ISXBound **ppBoundArr);

/*! квадратичное (quad) деление объема (по двум осям x и z)
 \warning массивы и объекты должны быть заранее инициализированны
*/
SX_LIB_API void SGCore_0ComBoundBoxArr4(ISXBound *pBound, ISXBound **ppBoundArr);

//! находит квадрат расстояния между лучем и точкой
SX_LIB_API float SGCore_0DistancePointBeam2(const float3 &vPoint, const float3 &vStart, const float3 &vDir);

//! возвращает пересекаются ли боксы или нет
SX_LIB_API bool SGCore_0InretsectBox(const float3 *pMin1, const float3 *pMax1, const float3 *pMin2, const float3 *pMax2);

struct CTriangle
{
	float3_t m_vA;
	float3_t m_vB;
	float3_t m_vC;

	CTriangle()
	{
	};
	CTriangle(float3_t vA, float3_t vB, float3_t vC) :m_vA(vA), m_vB(vB), m_vC(vC)
	{
	};

	//Проверкка пересечения треугольника и отрезка
	bool IntersectLine(const float3 & l1, const float3 &l2, float3 * p)
	{
		float3 n = SMVector3Normalize(SMVector3Cross((m_vB - m_vA), (m_vC - m_vB)));
		float d1 = SMVector3Dot((l1 - m_vA), n) / SMVector3Length(n);
		float d2 = SMVector3Dot((l2 - m_vA), n) / SMVector3Length(n);

		if ((d1 > 0 && d2 > 0) || (d1 < 0 && d2 < 0))
			return(false);

		*p = l1 + (l2 - l1) * (-d1 / (d2 - d1));
		if (SMVector3Dot(SMVector3Cross((m_vB - m_vA), (*p - m_vA)), n) <= 0) return(false);
		if (SMVector3Dot(SMVector3Cross((m_vC - m_vB), (*p - m_vB)), n) <= 0) return(false);
		if (SMVector3Dot(SMVector3Cross((m_vA - m_vC), (*p - m_vC)), n) <= 0) return(false);
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

//@TODO: поменять на SMPLANE
//! структура описание плоскости
struct CFrustumPlane
{
	float3_t m_vNormal;
	float m_fDistance;

	SX_ALIGNED_OP_MEM2();

	CFrustumPlane() = default;
	CFrustumPlane(const SMPLANE &pPlane)
	{
		m_vNormal.x = pPlane.x;
		m_vNormal.y = pPlane.y;
		m_vNormal.z = pPlane.z;
		m_fDistance = pPlane.w;
	}

	void normalize()
	{
		float fDenom = SMVector3Length(m_vNormal);
		m_vNormal = (float3)(m_vNormal / fDenom);
		m_fDistance = m_fDistance / fDenom;
	}
};

#include <xcommon/render/IFrustum.h>

//! создать ISXFrustum
SX_LIB_API IFrustum* SGCore_CrFrustum(); 

//**************************************************************************

//! камера
class ICamera : public IBaseObject
{
public:
	virtual ~ICamera(){};

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
	virtual const IFrustum* getFrustum() = 0;
};

//! создать ISXCamera
SX_LIB_API ICamera* SGCore_CrCamera();	

//!@} sxgcore_camera


#endif

/*! @} */
