
#ifndef __sxgcore
#define __sxgcore

#include <gdefines.h>

#include <d3d9.h>
#include <d3dx9.h>
#define SM_D3D_CONVERSIONS
#include <sxmath.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "DxErr9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <gcore\ModelFile.h>

#define SXGC_STR_SIZE_DBG_MSG 4096

#define SXGC_ERR_NON_DETECTED_D3D -1
#define SXGC_ERR_FAILED_INIT_D3D -2

SX_LIB_API long SGCore_0GetVersion();
SX_LIB_API void SGCore_Dbg_Set(report_func rf);
SX_LIB_API void SGCore_0Create(const char* name, HWND hwnd, int width,int heigth,bool windowed, DWORD create_device_flags, bool is_unic = true);
SX_LIB_API IDirect3DDevice9* SGCore_GetDXDevice();

//вывод отладочного сообщени€ в окно рендера
SX_LIB_API void SGCore_DbgMsg(const char* format, ...);

SX_LIB_API void SGCore_OnLostDevice();	//вызывать при потере устройства
SX_LIB_API int SGCore_OnDeviceReset(int width,int heigth,bool windewed);	//вызывать при попытке сброса устройства
SX_LIB_API void SGCore_OnResetDevice();	//вызывать при сбросе устроства

//отрисовка full screen quad
SX_LIB_API void SGCore_ScreenQuadDraw();

//шейдеры
SX_LIB_API DWORD SGCore_ShaderLoad(int type_shader, const char* path, const char* name, int is_check_double, D3DXMACRO* macro = 0);
SX_LIB_API void	SGCore_ShaderUpdateN(int type_shader, const char* name, D3DXMACRO macro[] = 0);
SX_LIB_API void SGCore_ShaderUpdate(int type_shader, DWORD id, D3DXMACRO macro[] = 0);

SX_LIB_API void SGCore_ShaderSetStdPath(const char* path);
SX_LIB_API void SGCore_ShaderGetStdPath(char* path);

SX_LIB_API void SGCore_ShaderReloadAll();

SX_LIB_API DWORD SGCore_ShaderGetID(int type_shader, const char* shader);

SX_LIB_API void SGCore_ShaderBindN(int type_shader, const char* shader);
SX_LIB_API void SGCore_ShaderBind(int type_shader, DWORD shader);

SX_LIB_API void SGCore_ShaderUnBind();

SX_LIB_API void SGCore_ShaderSetVRFN(int type_shader, const char* name_shader, const char* name_var, void* data);
SX_LIB_API void SGCore_ShaderSetVRF(int type_shader, DWORD num_shader, const char* name_var, void* data);

SX_LIB_API void SGCore_ShaderSetVRIN(int type_shader, const char* name_shader, const char* name_var, void* data);
SX_LIB_API void SGCore_ShaderSetVRI(int type_shader, DWORD num_shader, const char* name_var, void* data);

SX_LIB_API DWORD SGCore_ShaderIsExist(int type_shader, const char* name);
SX_LIB_API bool SGCore_ShaderIsValidate(int type_shader, DWORD id);

SX_LIB_API void SGCore_ShaderGetPath(int type_shader, DWORD id, char* path);
SX_LIB_API void SGCore_ShaderGetName(int type_shader, DWORD id, char* name);

//////////////

//текстуры
SX_LIB_API void SGCore_LoadTexStdPath(const char* path);
SX_LIB_API DWORD SGCore_LoadTexAddName(const char* name);	//добавл€ем им€ текстуры, взамен получаем на нее ID (поставить в очередь)
SX_LIB_API DWORD SGCore_LoadTexGetID(const char* name);		//получить id по имени
SX_LIB_API void SGCore_LoadTexGetName(DWORD id, char* name);//получить им€ по id

SX_LIB_API DWORD SGCore_LoadTexCreate(const char* name, IDirect3DTexture9* tex);	//создать место дл€ текстуры tex
SX_LIB_API DWORD SGCore_LoadTexUpdateN(const char* name);		//перезагрузить текстуру name (поставить в очередь)
SX_LIB_API void SGCore_LoadTexUpdate(DWORD id);

SX_LIB_API IDirect3DTexture9* SGCore_LoadTexGetTex(DWORD id);

SX_LIB_API void SGCore_LoadTexLoadTextures();	//загрузка всех текстур поставленных в очередь

//////////////

//рендер таргеты
SX_LIB_API DWORD SGCore_RTAdd(UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, const char* name, float coeffullscreen);

SX_LIB_API void SGCore_RTDeleteN(const char* text);
SX_LIB_API void SGCore_RTDelete(DWORD num);

SX_LIB_API DWORD SGCore_RTGetNum(const char* text);

SX_LIB_API void SGCore_RTOnLostDevice();
SX_LIB_API void SGCore_RTOnResetDevice();

SX_LIB_API IDirect3DTexture9* SGCore_RTGetTextureN(const char* text);
SX_LIB_API IDirect3DTexture9* SGCore_RTGetTexture(DWORD num);

///////////

SX_LIB_API void SGCore_SetSamplerFilter(DWORD id, DWORD value);
SX_LIB_API void SGCore_SetSamplerFilter2(DWORD begin_id, DWORD end_id, DWORD value);
SX_LIB_API void SGCore_SetSamplerAddress(DWORD id, DWORD value);
SX_LIB_API void SGCore_SetSamplerAddress2(DWORD begin_id, DWORD end_id, DWORD value);

///////////

struct ISXDataStaticModel : public IBaseObject
{
	virtual ~ISXDataStaticModel(){};

	virtual ISXDataStaticModel* GetCopy()=0;
	
	IDirect3DVertexBuffer9* VertexBuffer;
	IDirect3DIndexBuffer9* IndexBuffer;
	vertex_static* ArrVertBuf;
	DWORD* ArrIndBuf;
	char** ArrTextures;
	DWORD SubsetCount;
	UINT* StartIndex;
	UINT* IndexCount;
	UINT* StartVertex;
	UINT* VertexCount;
	UINT AllIndexCount;
	UINT AllVertexCount;
};

SX_LIB_API ISXDataStaticModel* SGCore_CrDSModel();
SX_LIB_API void SGCore_LoadStaticModel(const char* file, ISXDataStaticModel** data);
SX_LIB_API void SGCore_SaveStaticModel(const char* file, ISXDataStaticModel** data);

///////////

//простой объект с минимальным описанием
//дл€ корректного использовани€ необходимо сначала установить позицию/поворот/масштаб после чего CalculateWorld
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

SX_LIB_API ISXTransObject* SGCore_CrTransObject();

//структура описывающа€ ограничивающий квадрат в пространстве экрана
struct SXPosBBScreen
{
	float x;
	float y;
	float width;
	float height;
	float maxdepth;
	float mindepth;
	bool IsVisible;
	bool IsIn;
};


//класс ограничивающего объема
//дл€ создан€и минимума и максимума необходимо вызвать CalculateBound
//SetMinMax, GetMinMax до вызова CalculateWorldAndTrans возвращают нетрансформирвоанные данные
//конечным этапом построени€ Bound и Object €вл€етс€ CalculateWorldAndTrans
class ISXBound : public virtual ISXTransObject
{
public:
	virtual ~ISXBound(){};

	SX_ALIGNED_OP_MEM

	virtual void CalcBound(IDirect3DVertexBuffer9* vertex_buffer, DWORD count_vert, DWORD bytepervert) = 0;

	//функци€ просчета мировой матрицы и трансформации минимума и максимума
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

SX_LIB_API ISXBound* SGCore_CrBound();

/////////////

SX_LIB_API void SGCore_FCreateCone(float fTopRadius, float fBottomRadius, float fHeight, ID3DXMesh ** ppMesh, UINT iSideCount);
SX_LIB_API void SGCore_FCompBoundBox(IDirect3DVertexBuffer9* vertex_buffer, ISXBound** bound, DWORD count_vert, DWORD bytepervert);
SX_LIB_API void SGCore_FCompBoundBox2(IDirect3DVertexBuffer9* vertex_buffer, ISXBound* bound, DWORD count_vert, DWORD bytepervert);
SX_LIB_API void SGCore_FCreateBoundingBoxMesh(float3* min, float3* max, ID3DXMesh** bbmesh);

SX_LIB_API void SGCore_OptimizeIndecesInSubsetUint16(uint16_t* ib, uint16_t numFaces, uint16_t numVerts);
SX_LIB_API void SGCore_OptimizeIndecesInSubsetUint32(uint32_t* ib, uint32_t numFaces, uint32_t numVerts);

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
SX_LIB_API void SGCore_0ComBoundBoxArr8(ISXBound* bound, ISXBound** bound_arr);
SX_LIB_API void SGCore_0ComBoundBoxArr4(ISXBound* bound, ISXBound** bound_arr);

///////////////

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

SX_LIB_API ISXFrustum* SGCore_CrFrustum();

/////////////////////
class ISXCamera : public IBaseObject
{
public:
	virtual ~ISXCamera(){};

	SX_ALIGNED_OP_MEM

	virtual inline void PosLeftRight(float units) = 0;	//влево/вправо
	virtual inline void PosUpDown(float units) = 0;	//вверх/вниз
	virtual inline void PosFrontBack(float units) = 0;	//вперед/назад

	virtual inline void RotUpDown(float angle) = 0;	//вращение вверх/вниз
	virtual inline void RotRightLeft(float angle) = 0;	//вращение вправо/влево
	virtual inline void Roll(float angle) = 0;	//крен

	virtual inline void GetViewMatrix(float4x4* view_matrix) = 0;//получаем матрицу вида

	virtual inline void GetPosition(float3* pos) = 0;
	virtual inline void SetPosition(float3* pos) = 0;

	virtual inline void GetDirection(float3* dir) = 0;
	virtual inline void SetDirection(float3* dir) = 0;

	virtual inline void GetRight(float3* right) = 0;
	virtual inline void GetUp(float3* up) = 0;
	virtual inline void GetLook(float3* look) = 0;
	virtual inline void GetRotation(float3* rot) = 0;

	virtual inline float GetRotationX() = 0;
	virtual inline float GetRotationY() = 0;
	virtual inline float GetRotationZ() = 0;

	//bool Rolling; //флаг операции
	//float AngleRoll; //расчетный угол

	ISXFrustum* ObjFrustum;
	float3 LastVal;
	bool IsAccel;

protected:
	float3 Right;
	float3 Up;
	float3 Look;

	float3 Position;

	float AngleUpDown, AngleRightLeft, AngleRoll;
};

SX_LIB_API ISXCamera* SGCore_CrCamera();

////////
/*
class SXCameraFly
{
public:

	SX_ALIGNED_OP_MEM

		CameraFly();
	~CameraFly();

	void LoadInit(const char* path);
	void LoadSecondSection(bool isstart = false);
	void Activate(const char* section);
	void Update(DWORD timeDelta);
	inline bool IsFly();

	SkyXEngine::Core::Loaders::LoaderConfig* ConfigFly;

	char ActiveSection[32];
	char CurrentSection[32];
	char SecondSection[32];

	//параметры камеры до того момента как полет вз€л управление
	float3 OldPos;
	float OldAngleUpDown;
	float OldAngleRightLeft;
	float OldAngleRoll;


	float3 ActivePos;
	float3 LastPos;

	float ActiveAngleUpDown;
	float ActiveAngleRightLeft;
	float ActiveAngleRoll;

	float LastAngleUpDown;
	float LastAngleRightLeft;
	float LastAngleRoll;

	bool LerpPos;
	bool LerpAngleUpDown;
	bool LerpAngleRightLeft;
	bool LerpAngleRoll;

	DWORD SecTimePos;
	DWORD SecTimeRot;
	DWORD CurrTime;
	bool bfIsFly;
};*/

#endif