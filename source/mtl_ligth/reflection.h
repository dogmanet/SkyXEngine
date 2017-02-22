
#ifndef __reflection
#define __reflection

class Reflection
{
public:
	Reflection();
	~Reflection();

	void OnLostDevice();
	void OnResetDevice();

	SX_ALIGNED_OP_MEM

	void Init(int howref); //howref == 0 - плоское отражение, 1 - кубическое
	inline int GetTypeReflect(){ return TypeRef; };

	void PreRenderRefPlane(float4x4* world);
	void PostRenderRefPlane();
	inline IDirect3DTexture9* GetRefPlaneTex();

	void BeginRenderRefCube();
	void PreRenderRefCube(int cube, float4x4* world);
	void PostRenderRefCube(int cube);
	void EndRenderRefCube();
	inline IDirect3DCubeTexture9* GetRefCubeTex();

	bool IsComNow;	//обрабатывать ли сейчас, на случай еси видно или нет
	float3 PositionReflect;	//позиция откуда идут отражения, обновляется каждый раз
	float3 Position;
	float3 PosMin, PosMax;
	float4x4 MatrixView;
	ISXFrustum* ReflectFrustum[6];
	ID IDArr[6];

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

#endif