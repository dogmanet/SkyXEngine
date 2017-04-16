
#ifndef __sxgcore
#define __sxgcore

//ЗАНИМАЕМЫЕ РЕГИСТРЫ
//{
#define G_RI_INT_COUNT_POLY	0
#define G_RI_INT_COUNT_DIP		1
//}

#include <gdefines.h>

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



//типы материалов
#define MTL_TYPE_GEOM 0
#define MTL_TYPE_GRASS 1
#define MTL_TYPE_TREE 2
#define MTL_TYPE_SKIN 3
//#define MTL_LIGHT 4

//размер отладочного сообщения, выводимого в окно рендера
#define SXGC_STR_SIZE_DBG_MSG 4096

//
#define SXGC_ERR_NON_DETECTED_D3D -1
#define SXGC_ERR_FAILED_INIT_D3D -2

//количество mipmap уровней в загружаемых текстурах
#define SXGC_LOADTEX_COUNT_MIPMAP 5	

//формат в который будут преобразованы загружаемые текстуры
//если установить формат со сжатием то необходимо следовать всем правилам оформления текстур для данного формата
#define SXGC_LOADTEX_FORMAT_TEX D3DFMT_FROM_FILE	

SX_LIB_API long SGCore_0GetVersion();			//версия подсистемы
SX_LIB_API void SGCore_Dbg_Set(report_func rf);	//установка функции вывода сообщений

//инициализация подсистемы
SX_LIB_API void SGCore_0Create(
	const char* name,			//передваваемое имя подсистемы
	HWND hwnd,					//дескриптор окна в которое будет осуществляться рендер
	int width,					//ширина области рендера
	int heigth,					//высота области рендера
	bool windowed,				//оконный режим использовать ли? иначе полноэкранный
	DWORD create_device_flags,	//флаги создания устрайства (стандартные dx)
	bool is_unic = true			//должна ли подсистема быть уникальной на основе имени
	);

SX_LIB_API void SGCore_0Kill();	//уничтожение либы

SX_LIB_API IDirect3DDevice9* SGCore_GetDXDevice();	//возвращает dx устройство

//вывод отладочного сообщения в окно рендера
SX_LIB_API void SGCore_DbgMsg(const char* format, ...);

//ОБРАБОТКА ПОТЕРИ И ВОССТАНОВЛЕНИЯ УСТРОЙСТВА
//{
SX_LIB_API void SGCore_OnLostDevice();	//вызывать при потере устройства
SX_LIB_API int SGCore_OnDeviceReset(int width,int heigth,bool windewed);	//вызывать при попытке сброса устройства
SX_LIB_API void SGCore_OnResetDevice();	//вызывать при сбросе устроства
//}

//БАЗОВЫЕ ФУНКЦИИ УСТАНОВКИ НАСТРОЕК ВЫБОРКИ ИЗ ТЕКСТУР
//{{
//установка фильтрации
SX_LIB_API void SGCore_SetSamplerFilter(DWORD id, DWORD value);	//для конкретного слота
SX_LIB_API void SGCore_SetSamplerFilter2(DWORD begin_id, DWORD end_id, DWORD value);	//для набора слотов от begin_id до end_id

//установка адресации
SX_LIB_API void SGCore_SetSamplerAddress(DWORD id, DWORD value);	//для конкретного слота
SX_LIB_API void SGCore_SetSamplerAddress2(DWORD begin_id, DWORD end_id, DWORD value);	//для набора слотов от begin_id до end_id
//}}

//отрисовка full screen quad (уже смещенного как надо чтобы не было размытия)
SX_LIB_API void SGCore_ScreenQuadDraw();


//ПРОТОТИПЫ ПЕРЕОПРЕДЕЛЯЕМЫХ ФУНКЦИЙ
//{
//draw indexed primitive, команда отрисовки
typedef void(*g_func_dip) (UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);
//установка материала id, world - мировая матрица
typedef void(*g_func_mtl_set) (ID id, float4x4* world);
//загрузка материала, name - имя текстуры с расширением, mtl_type - тип материала на случай провала загрузки, MTL_TYPE_
//в случаях провала загрузки тип стандартного материала будет определен на основании mtl_type
typedef ID(*g_func_mtl_load) (const char* name, int mtl_type);
//получить сорт материала, по дефолту 0
typedef int(*g_func_mtl_get_sort) (ID id);
//рисовать ли подгруппы моделей данного материала раздельно?
typedef bool(*g_func_mtl_group_render_is_singly) (ID id);
//}

//ПЕРЕОПРЕДЕЛЯЕМЫЕ ФУНКЦИИ (точнее переопредяется их внутрення реализация)
//{
SX_LIB_API void SGCore_DIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);
SX_LIB_API void SGCore_MtlSet(ID id, float4x4* world);
SX_LIB_API ID SGCore_MtlLoad(const char* name, int mtl_type);
SX_LIB_API int SGCore_MtlGetSort(ID id);
SX_LIB_API bool SGCore_MtlGroupRenderIsSingly(ID id);
//}

//ПЕРЕОПРЕДЕЛЕНИЕ ФУНКЦИЙ
//{
SX_LIB_API void SGCore_SetFunc_DIP(g_func_dip func);
SX_LIB_API void SGCore_SetFunc_MtlSet(g_func_mtl_set func);
SX_LIB_API void SGCore_SetFunc_MtlLoad(g_func_mtl_load func);
SX_LIB_API void SGCore_SetFunc_MtlGetSort(g_func_mtl_get_sort func);
SX_LIB_API void SGCore_SetFunc_MtlGroupRenderIsSingly(g_func_mtl_group_render_is_singly func);
//}

//ШЕЙДЕРЫ
//{{
//распознавание  шейдеров (обращение через функции) происходит на основе идентификаторов представленных типом DWORD
//если речь дет об идентификации на основе имени то имя в данном случае эта та строка name которая была передана SGCore_ShaderLoad
//все шейдеры загружаются с версией 3.0
//если имя шейдера не содержит нижний пробел (_) то значит шейдера находится в корне директории
//если шейдер содержит нижний пробел (_) то строка до первого нижнего проблема это имя папки в котором находится шейдер с целым именем
//pp_shader.vs - лежит по загружаемому пути: /pp/pp_shader.vs

//максимальный размер имени директории (до _)
#define SXGC_SHADER_MAX_SIZE_DIR 64
//максимальный размер имени с расширением (после _)
#define SXGC_SHADER_MAX_SIZE_NAME 64
//общий максимальный  размер имени текстуры с расширением
#define SXGC_SHADER_MAX_SIZE_DIRNAME SXGC_SHADER_MAX_SIZE_DIR + SXGC_SHADER_MAX_SIZE_NAME
//максимальный размер пути до файла шейдера (без имени файла)
#define SXGC_SHADER_MAX_SIZE_STDPATH 256
//максимальный размер полного пути до шейдера (включая имя шейдера)
#define SXGC_SHADER_MAX_SIZE_FULLPATH SXGC_SHADER_MAX_SIZE_STDPATH + SXGC_SHADER_MAX_SIZE_DIRNAME

//максимальная длина имени переменной в шейдере
#define SXGC_SHADER_VAR_MAX_SIZE 64
//максимальное количество переменных в шейдере
#define SXGC_SHADER_VAR_MAX_COUNT 64

//количество макросов в массиве макросов
#define SXGC_SHADER_COUNT_MACRO 12

//типы шейдеров (int type_shader)
enum ShaderType
{
	st_vertex,	//вершинный
	st_pixel	//пиксельный
};

//типы проверок дубликатов шейдеров
enum ShaderCheckDouble
{
	scd_none,	//нет проверки
	scd_path,	//проверка по пути (имени шейдера с расширением)
	scd_name	//проверка по пользовательскому имени
};

//загрузка шейдера
SX_LIB_API ID SGCore_ShaderLoad(
	ShaderType type_shader,	//тип шейдера
	const char* path,		//имя файла шейдера с расширением
	const char* name,		//имя шейдера которое присвоится при загрузке
	ShaderCheckDouble is_check_double,	//проверять ли на уникальность (на основании имени файла с расширением)
	D3DXMACRO* macro = 0	//макросы
	);

SX_LIB_API void SGCore_ShaderGetName(ShaderType type_shader, ID id, char* name);	//записывает пользовательское имя шейдера в name
SX_LIB_API void SGCore_ShaderGetPath(ShaderType type_shader, ID id, char* path);	//записывает имя шейдер с расширением в path
SX_LIB_API ID SGCore_ShaderIsExistName(ShaderType type_shader, const char* name);	//существует ли шейдер с пользовательским именем name, если да то возвращает id
SX_LIB_API ID SGCore_ShaderIsExistPath(ShaderType type_shader, const char* path);	//существует ли шейдер с именем файла и расширением name, если да то возвращает id
SX_LIB_API bool SGCore_ShaderIsValidate(ShaderType type_shader, ID id);				//загружен ли шейдер с данным id

//обновление шейдера
SX_LIB_API void	SGCore_ShaderUpdateN(ShaderType type_shader, const char* name, D3DXMACRO macro[] = 0);
SX_LIB_API void SGCore_ShaderUpdate(ShaderType type_shader, ID id, D3DXMACRO macro[] = 0);

SX_LIB_API void SGCore_ShaderSetStdPath(const char* path);	//установить абсолютный путь откуда брать шейдеры
SX_LIB_API void SGCore_ShaderGetStdPath(char* path);		//возвращает абсолютный путь откуда берутся шейдеры

//перезагрузить все шейдеры
//с учетом макросов
SX_LIB_API void SGCore_ShaderReloadAll();	

SX_LIB_API ID SGCore_ShaderGetID(ShaderType type_shader, const char* shader);	//получить идентификатор шейдера по имени

//бинд шейдера
SX_LIB_API void SGCore_ShaderBindN(ShaderType type_shader, const char* shader);
SX_LIB_API void SGCore_ShaderBind(ShaderType type_shader, ID id);

SX_LIB_API void SGCore_ShaderUnBind();	//обнуление биндов шейдеров

//передача данных в шейдер
//{
//name_var - имя переменной которой присваивается значение
//data указатель значение

//передача float значений
SX_LIB_API void SGCore_ShaderSetVRFN(ShaderType type_shader, const char* name_shader, const char* name_var, void* data, int count_float4 = 0);
SX_LIB_API void SGCore_ShaderSetVRF(ShaderType type_shader, ID id, const char* name_var, void* data, int count_float4 = 0);

//передача int значений
SX_LIB_API void SGCore_ShaderSetVRIN(ShaderType type_shader, const char* name_shader, const char* name_var, void* data, int count_int4 = 0);
SX_LIB_API void SGCore_ShaderSetVRI(ShaderType type_shader, ID id, const char* name_var, void* data, int count_int4 = 0);
//}
//}}


//ТЕКСТУРЫ
//{{
//распознавание текстур (обращение через функции) происходит на основе идентификаторов представленных типом DWORD
//если речь идет об идентификации на основе имени то имя в данном случае это имя текстуры с расширением
//имя текстуры обязательно долно содержать нижний пробел (_), строка до первого нижнего проблема это имя папки в котором находится шейдер с целым именем
//mtl_tex.dds - лежит по загружаемому пути: /mtl/mtl_tex.dds

//максимальный размер имени директории (до _)
#define SXGC_LOADTEX_MAX_SIZE_DIR 64
//максимальный размер имени с расширением (после _)
#define SXGC_LOADTEX_MAX_SIZE_NAME 64
//общий максимальный  размер имени текстуры с расширением
#define SXGC_LOADTEX_MAX_SIZE_DIRNAME SXGC_LOADTEX_MAX_SIZE_DIR + SXGC_LOADTEX_MAX_SIZE_NAME
//максимальный размер пути до файла текстуры (без имени файла)
#define SXGC_LOADTEX_MAX_SIZE_STDPATH 256
//максимальный размер полного пути до текстуры (включая имя текстуры)
#define SXGC_LOADTEX_MAX_SIZE_FULLPATH SXGC_LOADTEX_MAX_SIZE_STDPATH + SXGC_LOADTEX_MAX_SIZE_DIRNAME

//типы текстур
enum LoadTexType
{
	ltt_load,	//загружаемая
	ltt_const,	//неудаляемая загружаемая
	ltt_custom,	//созданная пользователем

	//самоопределение типа, на тот случай когда мы обновляем текстуру которая точно есть
	//если определить этот тип, а внутри у текстуры на самом деле нет типа (скорее всего нет текстуры)
	//то будет определен ltt_load
	ltt_self,	
};

SX_LIB_API void SGCore_LoadTexStdPath(const char* path);	//установить стандартный путь откуда брать текстуры
SX_LIB_API void SGCore_LoadTexClearLoaded();				//очистить список загружаемых текстур
SX_LIB_API void SGCore_LoadTexDelete(ID id);				//удалить тектуру по id (независимо от типа)
SX_LIB_API ID SGCore_LoadTexAddName(const char* name, LoadTexType type);//добавляем имя текстуры, взамен получаем на нее ID (поставить в очередь)
SX_LIB_API ID SGCore_LoadTexGetID(const char* name);	//получить id по имени
SX_LIB_API void SGCore_LoadTexGetName(ID id, char* name);//получить имя по id

//создать место для текстуры tex и присвоить ей имя name, возвращает id
//прежде вызова этой функции, все добавленные текстуры должны быть загружены
//создавать текстур необходимо в managed pool (D3DPOOL_MANAGED) ибо обработка потери и восстановления устройства сюда не приходит
SX_LIB_API ID SGCore_LoadTexCreate(const char* name, IDirect3DTexture9* tex);

//обновить/перезагрузить текстуру name, если текстуры не было в списке то добавляет
//если текстуру надо обновить, но тип у нее заранее не известен, но она точно уже загружена 
//то можно использовать тип самоопределения ltt_self, тогда тип текстуры не изменится
SX_LIB_API ID SGCore_LoadTexUpdateN(const char* name, LoadTexType type);
SX_LIB_API void SGCore_LoadTexUpdate(ID id);//обновить/перезагрузить текстуру

SX_LIB_API IDirect3DTexture9* SGCore_LoadTexGetTex(ID id);	//возвращает текстуру по id

//загрузка всех текстур поставленных в очередь
//можно вызывать каждый кадр рендера
//функция сначала проверяет есть ли не загруженные текстуры на основе счетчиков и только потом грузит
SX_LIB_API void SGCore_LoadTexLoadTextures();	
//}


//RENDER TARGETS
//{{
//сброс и восстановление устройства сюда приходят

//добавить новый render target
SX_LIB_API ID SGCore_RTAdd(
	UINT width,				//ширина
	UINT height,			//высота
	UINT levels,			//количество mip-map уровней
	DWORD usage,			//
	D3DFORMAT format,		//формат из D3DFORMAT
	D3DPOOL pool,			//где будет размещена текстура, из D3DPOOL
	const char* name,		//имя rt
	//коэфициент размеров rt относительно области рендера
	//(если rt и размер области одинаковы то 1, если rt меньша на 0.5 то 0.5)
	//если указаны фиксированные значения то ставить 0, это нужно для устновления размеров при восстановлении устройства
	float coeffullscreen
	);

SX_LIB_API void SGCore_RTDeleteN(const char* text);	//удалить rt по имени
SX_LIB_API void SGCore_RTDelete(ID num);			//удалить rt по id

SX_LIB_API ID SGCore_RTGetNum(const char* text); //возвращает id по имени

SX_LIB_API IDirect3DTexture9* SGCore_RTGetTextureN(const char* text);	//возвращает текстуру по имени
SX_LIB_API IDirect3DTexture9* SGCore_RTGetTexture(ID num);				//возвращает текстуру по id
//}


//структура статической модели dse
struct ISXDataStaticModel : public IBaseObject
{
	virtual ~ISXDataStaticModel(){};

	virtual ISXDataStaticModel* GetCopy()=0;	//получить абсолютную копию модели
	
	IDirect3DVertexBuffer9* VertexBuffer;	//вершиный буфер
	IDirect3DIndexBuffer9* IndexBuffer;		//индексный буфер
	vertex_static* ArrVertBuf;				//массив вершин
	UINT* ArrIndBuf;						//массив индексов

	UINT SubsetCount;		//количество подгрупп
	char** ArrTextures;		//массив имен текстур без расширения
	UINT* StartIndex;		//массив стартовых позиций индексов для каждой подгруппы
	UINT* IndexCount;		//массив количества индексов для каждой подгруппы
	UINT* StartVertex;		//массив стартовых позиций вершин для каждой подгруппы
	UINT* VertexCount;		//массив количества вершин для каждой подгруппы
	UINT AllIndexCount;		//общее количество индексов
	UINT AllVertexCount;	//общее количество вершин
};

SX_LIB_API ISXDataStaticModel* SGCore_StaticModelCr();	//создать статическую модель
SX_LIB_API void SGCore_StaticModelLoad(const char* file, ISXDataStaticModel** data);	//загрузить статическую модель, data инициализируется внутри
SX_LIB_API void SGCore_StaticModelSave(const char* file, ISXDataStaticModel** data);	//сохранить статическую модель
SX_LIB_API IDirect3DVertexDeclaration9* SGCore_StaticModelGetDecl();	//возвращает декларацию вершин статической модели
///////////

//простой объект с минимальным описанием
//для корректного использования необходимо сначала установить позицию/поворот/масштаб после чего CalculateWorld
struct ISXTransObject : public IBaseObject
{
	virtual ~ISXTransObject(){};

	SX_ALIGNED_OP_MEM

	virtual inline float4x4* CalcWorld() = 0;

	float3 Position;
	float3 Rotation;
	float3 Scale;
	float4x4 World;
};

SX_LIB_API ISXTransObject* SGCore_CrTransObject();	//создать ISXTransObject

//структура описывающая ограничивающий квадрат в пространстве экрана
struct SXPosBBScreen
{
	float x;		//позиция по оси x в пространстве экрана
	float y;		//позиция по оси y в пространстве экрана
	float width;	//ширина в пикселях
	float height;	//высота в пикселях
	float maxdepth;	//конец объекта глубина 0-1
	float mindepth;	//начало объект глубина 0-1
	bool IsVisible;	//??
	bool IsIn;		//??
};


//класс ограничивающего объема
//для созданяи минимума и максимума необходимо вызвать CalculateBound
//SetMinMax, GetMinMax до вызова CalculateWorldAndTrans возвращают нетрансформирвоанные данные
//конечным этапом построения Bound и Object является CalculateWorldAndTrans
class ISXBound : public virtual ISXTransObject
{
public:
	virtual ~ISXBound(){};

	SX_ALIGNED_OP_MEM

	virtual void CalcBound(IDirect3DVertexBuffer9* vertex_buffer, DWORD count_vert, DWORD bytepervert) = 0;

	//функция просчета мировой матрицы и трансформации минимума и максимума
	virtual float4x4* CalcWorldAndTrans() = 0;

	virtual void GetPosBBScreen(SXPosBBScreen *res, float3* campos, float3* sizemapdepth, float4x4* mat) = 0;

	virtual void SetMinMax(float3* min, float3* max) = 0;
	virtual void GetMinMax(float3* min, float3* max) = 0;

	virtual void SetSphere(float3* center, float* radius) = 0;
	virtual void GetSphere(float3* center, float* radius) = 0;

	virtual bool IsPointInSphere(float3* point) = 0;
	virtual bool IsPointInBox(float3* point) = 0;

protected:
	float3 Min;
	float3 Max;

	float3 Center;
	float Radius;
};

SX_LIB_API ISXBound* SGCore_CrBound(); //создать ISXBound

/////////////

//создание меша (ID3DXMesh) конуса 
SX_LIB_API void SGCore_FCreateCone(
	float fTopRadius,		//верхний радиус
	float fBottomRadius,	//нижний радиус
	float fHeight,			//длина
	ID3DXMesh ** ppMesh,	//выходной меш
	UINT iSideCount			//количество частей??
	);

//просчет ограничивающего объема по вершинному буфер
SX_LIB_API void SGCore_FCompBoundBox(IDirect3DVertexBuffer9* vertex_buffer, ISXBound** bound, DWORD count_vert, DWORD bytepervert); //??
SX_LIB_API void SGCore_FCompBoundBox2(IDirect3DVertexBuffer9* vertex_buffer, ISXBound* bound, DWORD count_vert, DWORD bytepervert);	//??

//создание меша (ID3DXMesh) ограничивающего объема
SX_LIB_API void SGCore_FCreateBoundingBoxMesh(float3* min, float3* max, ID3DXMesh** bbmesh);

//ОПТИМИЗАЦИЯ БУФЕРОВ ГЕОМЕТРИИ
//{
//ОПТИМИЗАЦИЯ ИНДЕКСНЫХ БУФЕРОВ
SX_LIB_API void SGCore_OptimizeIndecesInSubsetUint16(uint16_t* ib, uint16_t numFaces, uint16_t numVerts);
SX_LIB_API void SGCore_OptimizeIndecesInSubsetUint32(uint32_t* ib, uint32_t numFaces, uint32_t numVerts);
//}


//ФУНКЦИИ ПРОСЧЕТА ПОПАДАНИЙ ТОЧЕК В ОБЪЕМЫ И ДЕЛЕНИЕ ОБЪЕМОВ
//{
//просчеты попадания точки/точек в объем 
//2d - на основании x и z координат 
//3d - на основании всех трех координат
//Abs - абсолютное нахождение внутри
SX_LIB_API bool SGCore_0InPos2D(float3* min, float3* max, float3* pos);	
SX_LIB_API bool SGCore_0InPosAbs2D(float3* min, float3* max, float3* pos);
SX_LIB_API int SGCore_0CountPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);
SX_LIB_API int SGCore_0CountPosPointsAbs2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);
SX_LIB_API bool SGCore_0InPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);
SX_LIB_API bool SGCore_0InPos3D(float3* min, float3* max, float3* pos);
SX_LIB_API bool SGCore_0InPosAbs3D(float3* min, float3* max, float3* pos);
SX_LIB_API int SGCore_0CountPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);
SX_LIB_API int SGCore_0CountPosPointsAbs3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);
SX_LIB_API bool SGCore_0InPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3);

//массивы и объекты должны быть заранее инициалзированны
//кубическое (octo) деление объема (по всем трем осям)
SX_LIB_API void SGCore_0ComBoundBoxArr8(ISXBound* bound, ISXBound** bound_arr);
//квадратичное (quad) деление объема (по двум осям x и z)
SX_LIB_API void SGCore_0ComBoundBoxArr4(ISXBound* bound, ISXBound** bound_arr);
//}

///////////////
//структура описание плоскости
struct SXFrustumPlane
{
	float3 Normal;
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

//класс описывающий фрустум
class ISXFrustum : public IBaseObject
{
public:
	virtual ~ISXFrustum(){};

	SX_ALIGNED_OP_MEM

	virtual void Update(const float4x4* view, const float4x4* proj) = 0;

	virtual bool PointInFrustum(const float3 *point) = 0;
	virtual bool PolyInFrustum(const float3* p1, const float3* p2, const float3* p3) = 0;
	virtual bool PolyInFrustumAbs(const float3* p1, const float3* p2, const float3* p3) = 0;

	virtual bool SphereInFrustum(const float3 *point, float radius) = 0;

	virtual bool SphereInFrustumAbs(const float3 *point, float radius) = 0;
	virtual bool BoxInFrustum(float3* min, float3* max) = 0;

//protected:
	SXFrustumPlane ArrFrustumPlane[6];

	float3	Point[8];
	float3	Center;
};

SX_LIB_API ISXFrustum* SGCore_CrFrustum(); //создать ISXFrustum


//КЛАСС КАМЕРА
class ISXCamera : public IBaseObject
{
public:
	virtual ~ISXCamera(){};

	SX_ALIGNED_OP_MEM

	//движения
	virtual inline void PosLeftRight(float units) = 0;	//влево/вправо
	virtual inline void PosUpDown(float units) = 0;		//вверх/вниз
	virtual inline void PosFrontBack(float units) = 0;	//вперед/назад

	//вращения
	virtual inline void RotUpDown(float angle) = 0;		//вращение вверх/вниз
	virtual inline void RotRightLeft(float angle) = 0;	//вращение вправо/влево
	virtual inline void Roll(float angle) = 0;			//крен

	virtual inline void GetViewMatrix(float4x4* view_matrix) = 0;//получаем матрицу вида в view_matrix

	//позиция
	virtual inline void GetPosition(float3* pos) = 0;
	virtual inline void SetPosition(float3* pos) = 0;

	//направление взгляда
	virtual inline void GetDirection(float3* dir) = 0;
	virtual inline void SetDirection(float3* dir) = 0;

	//управляющие векторы
	virtual inline void GetRight(float3* right) = 0;
	virtual inline void GetUp(float3* up) = 0;
	virtual inline void GetLook(float3* look) = 0;
	virtual inline void GetRotation(float3* rot) = 0;

	//повроты по осям
	virtual inline float GetRotationX() = 0;
	virtual inline float GetRotationY() = 0;
	virtual inline float GetRotationZ() = 0;

	ISXFrustum* ObjFrustum;	//фрустум этой камеры

	float3 LastVal; //??
	bool IsAccel;	//??

protected:
	float3 Right;
	float3 Up;
	float3 Look;

	float3 Position;

	float AngleUpDown, AngleRightLeft, AngleRoll;
};

SX_LIB_API ISXCamera* SGCore_CrCamera();	//создать ISXCamera


//SKY BOX
//{
SX_LIB_API void SGCore_SkyBoxCr();	//создание
SX_LIB_API bool SGCore_SkyBoxIsCr();//инициализирован ли skybox
//абсолютный путь загрузки текстур
SX_LIB_API void SGCore_SkyBoxSetStdPathTex(const char* path);
SX_LIB_API void SGCore_SkyBoxGetStdPathTex(char* path);

//!кубические текстуры
SX_LIB_API void SGCore_SkyBoxLoadTex(const char *texture);	//загрузка текстуры, texture - имя текстуры с расширением
SX_LIB_API void SGCore_SkyBoxChangeTex(const char *texture);//смена текстуры, texture - имя текстуры с расширением

//угол на который повернут скайбокс по оси y
SX_LIB_API void SGCore_SkyBoxSetRot(float angle);	
SX_LIB_API float SGCore_SkyBoxGetRot();

//цвет в который будет окрашен скайбокс 0-1, альфа компонента - на сколько будет окрашен
SX_LIB_API void SGCore_SkyBoxSetColor(float4_t* color);
SX_LIB_API void SGCore_SkyBoxGetColor(float4_t* color);

//рендер скайбокса, timeDelta - время рендера кадра, pos - позиция набладателя (y координату необходимо смещать)
SX_LIB_API void SGCore_SkyBoxRender(float timeDelta, float3* pos);
//}

//SKY CLOUDS
//{
//простая плоскость параллельная xz на которую зеркально (х2) натягивается текстура, в постоянном движении
//положение констатно

SX_LIB_API void SGCore_SkyCloudsCr();	//создание
SX_LIB_API bool SGCore_SkyCloudsIsCr();	//инициализирован ли sky clouds
//абсолютный путь загрузки текстур
SX_LIB_API void SGCore_SkyCloudsSetStdPathTex(const char* path);
SX_LIB_API void SGCore_SkyCloudsGetStdPathTex(char* path);

//установка размеров и позиции
//так как позиция облаков константна то чтобы была илюзия полного покрытия уровня, необходимо облакам указывать размер в 2 раза больше чем весь доступный уровень
SX_LIB_API void SGCore_SkyCloudsSetWidthHeightPos(float width, float height, float3* pos);

//!обычные 2д текстуры
SX_LIB_API void SGCore_SkyCloudsLoadTex(const char *texture);	//загрузка текстуры, texture - имя текстуры с расширением
SX_LIB_API void SGCore_SkyCloudsChangeTex(const char *texture);	//загрузка текстуры, texture - имя текстуры с расширением

//угол поворота по оси y
SX_LIB_API void SGCore_SkyCloudsSetRot(float angle);
SX_LIB_API float SGCore_SkyCloudsGetRot();

//коэфициент прозрачности
SX_LIB_API void SGCore_SkyCloudsSetAlpha(float alpha);
SX_LIB_API float SGCore_SkyCloudsGetAlpha();

//цвет в который будут окрашены облака 0-1, альфа компонента - на сколько будет окрашен
SX_LIB_API void SGCore_SkyCloudsSetColor(float4_t* color);
SX_LIB_API void SGCore_SkyCloudsGetColor(float4_t* color);

//рендер облаков, timeDelta - время рендера кадра, pos - позиция набладателя (y координату необходимо смещать), is_shadow - для теней ли рендерим?
SX_LIB_API void SGCore_SkyCloudsRender(DWORD timeDetlta, float3* pos, bool is_shadow);
//}

#endif