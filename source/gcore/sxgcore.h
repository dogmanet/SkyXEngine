
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
SX_LIB_API IGXDevice* SGCore_GetDXDevice();

//! возвращает массив всех доступных разрешений монитора, в iCount записывает размер массива
SX_LIB_API const DEVMODE* SGCore_GetModes(int *iCount);

//! Проверяет успешность вызова по HRESULT, выводит сообщение об ошибке
SX_LIB_API HRESULT SGCore_DXcallCheck(HRESULT hr, const char *callStr);
#define DX_CALL(call) SGCore_DXcallCheck((call), #call)

//!@}

//#############################################################################

/*! \name Обработка потери/восстановления устройства 
 \warning Функции обработки потери/восстановления устройства обрабатывают ресурсы только в пределах данной библиотеки, другие библиотеки должны сами производить данную обработку!
!@{*/

//! вызывать при сбросе устроства
SX_LIB_API void SGCore_OnResetDevice();	

//! отрисовка full screen quad (уже смещенного как надо чтобы не было размытия)
SX_LIB_API void SGCore_ScreenQuadDraw();

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

	//! яркость
	DS_RT_LUMINANCE,

	//! освещенная сцена rgba16
	DS_RT_SCENELIGHT,

	//! освещенная сцена rgba16
	DS_RT_SCENELIGHT2,

	//! текущий rt адаптации глаза к освещению r16f
	DS_RT_ADAPTEDLUM
};

//!@} sxgcore_ds


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
#define SXGC_SHADER_COUNT_MACRO 32

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

//!@} sxgcore_loadtex

enum SHADER_CONST_REGISTER
{
	SCR_OBSERVER_CAMERA = 5,
	SCR_FRAME = 4,
	SCR_SCENE = 3,
	SCR_CAMERA = 2,
	SCR_OBJECT = 1,
	SCR_SUBSET = 0,

	SCR_XMATERIAL = 7
};

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
