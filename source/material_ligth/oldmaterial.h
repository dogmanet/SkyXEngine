

//структура из материала указывающая на маску и 4 детальных и 4 микрорельефных карты
struct MaterialMaskPM
{
	MaterialMaskPM()
	{
		Mask = -1;
		ArrDeatail[0] = -1;
		ArrDeatail[1] = -1;
		ArrDeatail[2] = -1;
		ArrDeatail[3] = -1;

		ArrMicroDiffuse[0] = -1;
		ArrMicroDiffuse[1] = -1;
		ArrMicroDiffuse[2] = -1;
		ArrMicroDiffuse[3] = -1;
	}

	DWORD Mask;
	DWORD ArrDeatail[4];
	DWORD ArrMicroDiffuse[4];
};

//структура из материала определяющая состояния рендера
struct MaterialRenderStates
{
	MaterialRenderStates()
	{
		IsCullBack = false;
		IsAlphaTest = false;
	}
	bool IsCullBack;
	bool IsAlphaTest;
};

//структура из материала определяющая основные характеристики просчета освещения
struct MaterialLightParam
{
	MaterialLightParam()
	{
		ParamTex = -1;
		ParamTex2 = -1;

		RoughnessValue = 0.f;
		F0Value = 0.f;
		SoftSpecularValue = 0.f;
		MapSpecularValue = 0.f;

		TypeReflect = 0;
		IDParamLight = -1;
		IsTextureParam = true;
	}
	
	DWORD ParamTex;

	//FOR EDITORS
	bool IsTextureParam;
	DWORD ParamTex2;
	DWORD IDParamLight;

	float RoughnessValue;
	float F0Value;
	float SoftSpecularValue;
	float MapSpecularValue;
	
	int TypeReflect;

	char Name[64];
};

struct MaterialDataShader
{
	MaterialDataShader()
	{
		IsTransWorld = false;
		IsTransView = false;
		IsTransProjection = false;
		IsTransWorldView = false;
		IsTransWorldViewProjection = false;
		IsTransPosCam = false;
		IsTransTimeDelta = false;
		IsTransUserData = false;
		Param = float4(0,0,0,0);
	};

	SX_ALIGNED_OP_MEM

	bool IsTransWorld;
	bool IsTransView;
	bool IsTransProjection;
	bool IsTransWorldView;
	bool IsTransWorldViewProjection;
	bool IsTransPosCam;
	bool IsTransTimeDelta;
	bool IsTransUserData;
	float4 Param;
};

class Material
{
public:
	Material();
	~Material();

	SX_ALIGNED_OP_MEM

	//устанвока текстуры материала
	void SetMainTexture(DWORD slot);

	void Render(float4x4* world,DWORD timeDelta);

	int PhysicsMaterial;

	char Name[256];
	DWORD MainTexture;
	DWORD PreShaderVS;
	DWORD PreShaderPS;
	DWORD IDSelShaders;
	bool IsRefraction;
	MaterialMaskPM MicroDetail;
	MaterialRenderStates RenderStates;
	MaterialLightParam LightParam;

	MaterialDataShader VS;
	MaterialDataShader PS;

	bool TransVSDataInPS;
	bool TransPSDataInVS;

	bool IsForwardRender;

	float4x4 view,proj,worldtrans,viewtrans,projtrans;
};

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	SX_ALIGNED_OP_MEM

	DWORD Load(const char* path,int type=0);
	bool IsAlpha(DWORD id);
	bool IsForward(DWORD id);
	void Save();
	void SetMainTexture(DWORD slot,DWORD id);
	void Render(DWORD id,float4x4* world,DWORD timeDelta);

	DWORD GetID(const char* name);

	Material* GetMaterial(DWORD id);

	inline DWORD GetCount(){return Arr.size();}

protected:
	Array<Material*> Arr;
};

class RenderReflection
{
public:
	RenderReflection();
	~RenderReflection();

	void OnLostDevice();
	void OnResetDevice();

	SX_ALIGNED_OP_MEM

	void Init(int howref); //howref == 0 - плоское отражение, 1 - кубическое
	inline int GetTypeReflect(){return TypeRef;};

	void PreRenderRefPlane(float4x4* world);
	void PostRenderRefPlane();
	inline IDirect3DTexture9* GetRefPlaneTex();

	void BeginRenderRefCube();
	void PreRenderRefCube(int cube,float4x4* world);
	void PostRenderRefCube(int cube);
	void EndRenderRefCube();
	inline IDirect3DCubeTexture9* GetRefCubeTex();

	bool IsComNow;	//обрабатывать ли сейчас, на случай еси видно или нет
	float3 PositionReflect;	//позиция откуда идут отражения, обновляется каждый раз
	float3 Position;
	float3 PosMin,PosMax;
	float4x4 MatrixView;
	Core::ControllMoving::Frustum* ReflectFrustum;

	int CountRenderInSec;

//protected:
	int TypeRef;
	D3DXPLANE Plane;
	D3DXMATRIX OldMatProj;
	D3DXMATRIX OldMatView;
	D3DXMATRIX OldMatViewProj;
	IDirect3DSurface9* BackBuffer;
	
	IDirect3DSurface9* SurfaceZBuffer;
	IDirect3DSurface9* LastSurfaceZBuffer;

	IDirect3DTexture9* TextureReflect;
	IDirect3DSurface9* SurfaceReflect;

	IDirect3DSurface9* CubeReflectSurface[6];
	IDirect3DCubeTexture9* TextureCubeReflect;
};