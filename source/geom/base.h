

#define TRANSFORM_COORD_SCREEN2(point,sizemapdepth)\
point.x /= abs(point.w);\
point.y /= abs(point.w);\
point.z /= sizemapdepth->z;\
point.x = point.x * 0.5 + 0.5;\
point.y = point.y * (-0.5) + 0.5;\
point.x *= sizemapdepth->x;\
point.y *= sizemapdepth->y;

//простой объект с минимальным описанием
//дл€ корректного использовани€ необходимо сначала установить позицию/поворот/масштаб после чего CalculateWorld
class Object
{
public:
	Object();
	~Object();

	SX_ALIGNED_OP_MEM
	
	inline void GetPosition(float3* pos){if(pos)*pos = Position;};
	inline void SetPosition(float3* pos){if(pos)Position = *pos;};
	//SMMatrixRotationX(Rotation.x) * SMMatrixRotationY(Rotation.y) * SMMatrixRotationZ(Rotation.z)
	inline void GetRotation(float3* rot){if(rot)*rot = float3(Rotation.x,Rotation.y,Rotation.z);};
	//inline void SetRotation(float3* rot){Rotation = SMQuaternion(rot->x, 'x') * SMQuaternion(rot->y, 'y') * SMQuaternion(rot->z, 'z');};
	inline void SetRotation(float3* rot){if(rot)Rotation = *rot;};
	
	inline void GetScale(float3* scale){*scale = Scale;};
	inline void SetScale(float3* scale){Scale = *scale;};

	void CalculateWorld(float4x4* world=0);
	
//protected:
	float3 Position;
	float3 Rotation;
	//SMQuaternion Rotation;
	float3 Scale;
	float4x4 World;
};

/////////////////////

//структура описывающа€ ограничивающий квадрат в пространстве экрана
struct PosBBScreen
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
class Bound : public Object
{
public:
	Bound();
	~Bound();

	SX_ALIGNED_OP_MEM

	void CalculateBound(IDirect3DVertexBuffer9* vertex_buffer,DWORD count_vert,DWORD bytepervert);

	//функци€ просчета мировой матрицы и трансформации минимума и максимума
	void CalculateWorldAndTrans(float4x4* world);

	void GetPosBBScreen(PosBBScreen *res,float3* campos,float3* sizemapdepth,float4x4* mat);
	
	inline void SetMinMax(float3* min,float3* max);
	inline void GetMinMax(float3* min,float3* max);

	inline void SetSphere(float3* center,float* radius);
	inline void GetSphere(float3* center,float* radius);

	inline bool IsPointInSphere(float3* point);
	inline bool IsPointInBox(float3* point);

protected:
	float3 Min;
	float3 Max;

	float3 Center;
	float Radius;
};

/////////////////////////////////////

struct SkyBoxVertex
{
	SkyBoxVertex(float x,float y,float z,float u,float v,float w)
	{
		_x  = x;  _y  = y;  _z  = z; _u = u; _v = v; _w = w;
	}
	float _x, _y, _z;
	float _u, _v, _w;
};

class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	void LoadTextures(const char *texture);
	void ChangeTexture(const char *texture);
	inline void SetRotation(float angle);
	inline float GetRotation();
	inline void SetColor(float4_t* color);
	inline void GetColor(float4_t* color);
	void Render(float timeDelta,float3* pos,bool is_shadow);

protected:

	float RotaionY;
	float4x4 MatRotation;
	float4_t Color;
	float FactorBlend;

	bool BFChange;
	bool BFChangeMainTex;

	IDirect3DCubeTexture9*	Tex;
	IDirect3DCubeTexture9*	Tex2;
	IDirect3DVertexBuffer9*	Vertices;
	IDirect3DIndexBuffer9*  Indeces;
};


struct SkyCloudsVertex
{
	SkyCloudsVertex(float x,float y,float z,float u,float v)
	{
	_x  = x;  _y  = y;  _z  = z; _u = u; _v = v;
	}
	float _x, _y, _z;
	float _u, _v;
};


class SkyClouds
{
public:
	SkyClouds();
	~SkyClouds();

	//установить параметры облаков
	//если облака отбрасывают тень, то надо шобы облака покрывали почти весь уровень
	void SetWidthHeightPos(float width,float height,float3* pos);
	void LoadTextures(const char *texture);
	void ChangeTexture(const char *texture);

	inline void SetRotation(float angle);
	inline float GetRotation();
	inline void SetAlpha(float alpha);
	inline float GetAlpha();
	inline void SetColor(float4_t* color);
	inline void GetColor(float4_t* color);

	void Render(DWORD timeDetlta,float3* pos,bool is_shadow);

private:

	float Alpha;
	float RotaionY;
	float4x4 MatRotation;
	float4_t Color;

	float FactorBlend;
	float2_t WidthHeight;
	bool BFChange;
	bool BFChangeMainTex;

	float Bias;
	SkyCloudsVertex* Vertices;
	IDirect3DVertexBuffer9*	SkyCloudsVertices;
	IDirect3DIndexBuffer9*  SkyCloudsIndeces;

	IDirect3DTexture9*	SkyCloudsTex;
	IDirect3DTexture9*	SkyCloudsTex2;
};

struct SkyBoxCreatorVertex
{
	SkyBoxCreatorVertex(float x,float y,float z,float u0,float v0,float u1,float v1)
	{
		_x  = x;  _y  = y;  _z  = z; 
		_u0 = u0; _v0 = v0;
		_u1 = u1; _v1 = v1;
	}
	float _x, _y, _z;
	float _u0, _v0;
	float _u1, _v1;
	float _u2, _v2;
	float _u3, _v3;
	float _u4, _v4;
	float _u5, _v5;
};

struct SkyBoxCreatorVertex2
{
	SkyBoxCreatorVertex2(){}
	SkyBoxCreatorVertex2(float x,float y,float z,float nx,float ny,float nz,float u0,float v0)
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx  = nx;  _ny  = ny;  _nz  = nz; 
		_u0 = u0; _v0 = v0;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u0, _v0;
};

class SkyBoxCreator
{
public:
	SkyBoxCreator();
	~SkyBoxCreator();
	void Init();
	void LoadOrientTex(int cube,const char *texture);
	void LoadTextures(int cube,const char *texture);
	
	void RenderInRT();
	void RenderInRT2();
	void Render(float3* pos);
	void Render2(float3* pos);
	void CreateCubeMap2();

	void CreateCubeMap();
	void SaveCubeMap(const char* path);

	float3_t IsUse[6][4];
	float4_t Value[6][4];
	float4_t StartLerp[6][4];

	IDirect3DTexture9*	ArrCubeMap[6];
	IDirect3DCubeTexture9*	CubeMap;
	IDirect3DSurface9*	CubeSurface[6];
	float4x4 View[6];
	float4x4 Proj[6];

	bool EnableEdge[6];

	IDirect3DSurface9* OldColorSurface;

protected:

	char ArrPathCube[6][1024];
	IDirect3DTexture9*	TexCube[6];
	IDirect3DTexture9*	TexOrient[6];

	IDirect3DVertexBuffer9*	Vertices[6];
	IDirect3DIndexBuffer9*  Indeces[6];

	IDirect3DVertexBuffer9*	Vertices2;
	IDirect3DIndexBuffer9*  Indeces2;

	DWORD CountVert,CountInd;
};

////////////////////////////////

class Model : public Bound
{
public:
	Model();
	Model(Model& mm);
	~Model();

	void Load(const char* path,bool is_open);
	void LoadLod(const char* path);

	//ismaterial == 2 то использовать базовый материал
	void Render(int lod,DWORD timeDelta,int ismaterial,DWORD tex_selected = -1);

	//инициализаци€ отражений дл€ подгруппы
	void InitReflection(DWORD group,int type_reflect);

	bool GetOpen();
	void SetOpen(bool is_open);
				
	void OnLostDevice();			//вызывать при потере устрйостве
	void OnResetDevice();			//вызывать при восстановлении устройства

	/*inline void GetMatPosition(float3* pos);
	inline void GetMatRotation(float3* rot);
	inline void SetMatPosition(float3* pos);
	inline void SetMatRotation(float3* rot);

	inline void GetMatPosition(float4x4* matpos);
	inline void GetMatRotation(float4x4* matrot);
	inline void SetMatPosition(float4x4* matpos);
	inline void SetMatRotation(float4x4* mattor);*/

	//void ComMatrixTrans();

	inline void SetName(const char* name);
	inline void GetName(char* name);

	bool IsSegmentationModel;
	bool ThisIsLod;
			
//protected:
	/*float3 Position;
	float3 Rotation;

	float4x4 MatRot;
	float4x4 MatPos;
	float4x4 MatWorld;*/

	char Name[256];
	char PathForModel[1024];
	char PathForLod[1024];
	bool IsOpen;
	bool IsLoded;
	Model* Lod;
	Graphics::ThreeD::DataStaticModel* Data;
	DWORD* TextureID;
	DWORD AllCountPoly;
	DWORD AllCountVertex;
	DWORD* ArrMaterials;

	SkyXEngine::Graphics::ThreeD::RenderReflection** ArrReflection;
};