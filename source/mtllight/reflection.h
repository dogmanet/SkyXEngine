
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

	void Init(MtlTypeReflect howref);
	inline MtlTypeReflect GetTypeReflect(){ return TypeRef; };

	void PreRenderRefPlane(float4x4* world);
	void PostRenderRefPlane();
	inline IDirect3DTexture9* GetRefPlaneTex();

	void BeginRenderRefCube();
	void PreRenderRefCube(int cube, float4x4* world);
	void PostRenderRefCube(int cube);
	void EndRenderRefCube(float3_t* viewpos);
	bool UpdateCountUpdate(float3_t* viewpos);
	bool AllowedRender();
	void NullingCountUpdate();
	inline IDirect3DCubeTexture9* GetRefCubeTex();

	void SetMinMax(float3_t* min, float3_t* max);

	void SetIDArr(ID id, int cube, ID idarr);
	inline int GetCountIDArrs();
	inline ID GetIDArr(ID id, int cube);

	bool IsComNow;	//обрабатывать ли сейчас, на случай еси видно или нет
	float3 PositionReflect;	//позиция откуда идут отражения, обновляется каждый раз
	float3 Position;
	float3 PosMin, PosMax;
	float4x4 MatrixView;
	ISXFrustum* ReflectFrustum[6];
	Array<ID*> IDArr;

	long CountUpdate;

	//protected:
	MtlTypeReflect TypeRef;
	D3DXPLANE Plane, Plane2;
	float ZNear;
	float4x4 OldMatProj,OldMatView,OldMatViewProj;
	IDirect3DSurface9* BackBuffer;

	IDirect3DSurface9* SurfaceZBuffer;
	IDirect3DSurface9* LastSurfaceZBuffer;

	IDirect3DTexture9* TextureReflect;
	IDirect3DSurface9* SurfaceReflect;

	IDirect3DSurface9* CubeReflectSurface[6];
	IDirect3DCubeTexture9* TextureCubeReflect;
};

#endif