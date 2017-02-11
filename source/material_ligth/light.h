
#ifndef __light_h
#define __light_h


#define LIGHTS_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < ArrLights.size()))\
	{reportf(-1, "%s - light: unresolved index of access '%d'", gen_msg_location, id); return stdval; }

#include <material_ligth\\shadow.cpp>
#include <handler_dx_func.cpp>

class Lights
{
public:

	Lights();
	~Lights();

	long CreateCopy(long id);

	void OnLostDevice();
	void OnResetDevice();

	void Save(const char* path);
	void Load(const char* path);

	inline long GetCountLights();
	void Clear();

	long CreatePoint(long id, float3* center, float power, float dist, float3* color, bool isglobal, bool is_shadowed, const char* bound_volume);
	long CreateDirection(long id, float3* pos, float power, float dist, float3* color, float3* dir, float top_radius, float angle, bool is_shadow, const char* bound_volume);

	void LoadLightMeshSource(long id, const char* path);
	void BindLightToGroup(long id, int group);
	long GetLightBindedGroupSource(long id);
	long GetLightCountGroupMesh(long id);
	const char* GetLightNameGroupMesh(long id, int group);
	const char* GetLightPathSource(long id);

	void RenderSource(long id, bool render_bind_group, DWORD timeDelta);
	void RenderSourceAll(bool render_bind_group, DWORD timeDelta);
	void RenderSourceAllExceptGroup(long id, DWORD timeDelta);

	void DeleteLight(long id);

	void Render(long id,DWORD timeDelta);

	char* GetLightName(long id);
	void SetLightName(long id, const char* name);

	void GetLightColor(long id, float3* vec);
	void SetLightColor(long id, float3* vec);

	void GetLightPos(long id, float3* vec, bool for_mesh, bool greal = false);
	void SetLightPos(long id, float3* vec, bool greal = false);

	void SetLightPower(long id,float power);
	float GetLightPower(long id);
	float GetLightPowerDiv(long id);

	float GetLightDist(long id);

	void GetLightRot(long id, float3* vec, bool rot_mesh);
	void SetLightRot(long id, float3* vec, bool rot_mesh);
	void GetLightDir(long id, float3* vec, bool rot_mesh);
	void SetLightDir(long id, float3* vec, bool rot_mesh);

	void SetLightBias(long id, float val);
	float GetLightBias(long id);

	float GetLightTopRadius(long id);
	float GetLightAngle(long id);

	void ChangeAngle(long id, float angle, bool is_create);
	void ChangeTopRadius(long id, float top_radius);
	void ChangeRadiusHeight(long id, float radius_height, bool is_create);
	void ChangeShadow(long id, bool is_shadow);
	void SetBlurPixel(long id, float blur_pixel);
	float GetBlurPixel(long id);
	void SetShadowLocalNear(long id, float slnear);
	float GetShadowLocalNear(long id);
	void SetShadowLocalFar(long id, float slfar);
	float GetShadowLocalFar(long id);
	void SetEnableCubeEdge(long id, int edge, bool enable);
	bool GetEnableCubeEdge(long id, int edge);

	bool ComVisibleForFrustum(long id, ISXFrustum* frustum);
	bool GetVisibleForFrustum(long id);
	float ComDistFor(long id, float3* vec);
	float GetDistFor(long id);
	void ComVisibleFrustumDistFor(ISXFrustum* frustum, float3* vec);

	bool IsEnable(long id);
	void SetEnable(long id,bool val);
	bool IsShadow(long id);
	int GetType(long id);

	IDirect3DTexture9* GetShadow2();

	void InRenderBegin(long id);
	void InRenderEnd(long id);
	void InRenderPre(long id, int cube);
	void InRenderPost(long id, int cube);
	ISXFrustum* GetFrustum(long id, int how);
	ISXFrustum* GetFrustumG(long id,int split);

	void UpdateFrustumsG(long id, int split, float3* pos, float3* dir);

	void GenShadow(long id);
	inline void NullingShadow();
	void GenShadow2(long id);
	void SoftShadow(bool randomsam, float size, bool isfirst = false);

	long GetLightIDArr(long id, long inid, int how);
	void SetLightIDArr(long id, long inid, int how, long id_arr);

	inline void SetLightTypeShadowed(long id, int type);
	inline int GetLightTypeShadowed(long id);

	bool UpdateLightCountUpdate(long id,float3* viewpos, int ghow = -1);
	bool AllowedRenderLight(long id, int ghow = -1);
	inline void SetNullLightCountUpdate(long id);

	void ComHDR(DWORD timeDelta, float factor_adapted = 10);

	inline void Set4Or3Splits(long id, bool is4);
	inline bool Get4Or3Splits(long id);

	struct Light
	{
		Light();
		~Light();

		SX_ALIGNED_OP_MEM

		int TypeLight;	
		char Name[OBJECT_NAME_MAX_LEN];

		bool IsVisible;
		bool IsEnable;
		bool IsShadow;
		bool IsGlobal;

		int TypeShadowed;
		long CountUpdate;
		

		ID3DXMesh* Mesh;

		float3 Position;
		float Power;
		float Dist;
		float GAngleX, GAngleY; //sun pos
		float3 Direction;
		float3 Rotation;
		float3 Color;

		float2_t TopBottomRadius;
		float Angle;

		ISXBound* BoundVolume;
		float4x4 WorldMat;
		float4x4 MatRot;
		float4x4 MatRotL;

		bool IsVisibleFor;
		float DistFor;

		PSSM* ShadowPSSM;
		ShadowMapTech* ShadowSM;
		ShadowMapCubeTech* ShadowCube;

		struct MeshSource
		{
			MeshSource();
			~MeshSource();

			SX_ALIGNED_OP_MEM

			char Path[1024];
			ISXDataStaticModel* Mesh;
			float4x4 WorldMat;
			float4x4 WorldMatLocal;
			float4x4 MatRot;
			float3 Position;
			float3 Rotation;
			float3 Direction;
			float3 LocalPos;
			long IdGroupBind;
			DWORD* ArrTex;
		};
		
		MeshSource* Source;
	};

protected:
	inline long AddLight(Light* obj);
	Array<Light*> ArrLights;
	IDirect3DVertexDeclaration9* VertexDeclarationStatic;

	DWORD ShadowMap;
	DWORD ShadowMap2;
	int HowShadow;
};

#endif