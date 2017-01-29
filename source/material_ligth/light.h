
#ifndef __light_h
#define __light_h

#define LIGHTS_POS_G_MAX 1000

#include <material_ligth\\shadow.cpp>

class Lights
{
public:

	Lights();
	~Lights();

	long CreateCopy(long id);

	void OnLostDevice();
	void OnResetDevice();

	inline long GetCountLights();

	long CreatePoint(float4* center, float3* color, bool isglobal, bool is_shadowed, const char* bound_volume);
	long CreateDirection(float4* pos, float3* color, float3* dir, float top_radius, float angle, bool is_shadow, const char* bound_volume);
	void DeleteLight(long id);

	void Render(long id,DWORD timeDelta);

	char* GetLightName(long id);
	void SetLightName(long id, const char* name);

	void GetLightColor(long id, float3* vec);
	void SetLightColor(long id, float3* vec);

	void GetLightPosW(long id, float4* vec);
	void GetLightPos(long id, float3* vec);
	void SetLightPos(long id, float3* vec);

	void GetLightRot(long id, float3* vec);
	void SetLightRot(long id, float3* vec);
	void GetLightDir(long id, float3* vec);
	void SetLightDir(long id, float3* vec);

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
	bool GetVisibleForFrustum(long id, ISXFrustum* frustum);
	float ComDistFor(long id, float3* vec);
	float GetDistFor(long id);
	void ComVisibleFrustumDistFor(ISXFrustum* frustum, float3* vec);

	bool IsEnable(long id);
	void SetEnable(long id,bool val);
	bool IsShadow(long id);
	int GetType(long id);
	IDirect3DTexture9* GetShadow(long id);

	void InRenderBegin(long id);
	void InRenderEnd(long id);
	void InRenderPre(long id, int cube);
	void InRenderPost(long id, int cube);
	ISXFrustum* GetFrustum(long id);
	ISXFrustum* GetFrustumG(long id,int split);
	void UpdateG(long id, int split,float3* pos, float3* dir);

	void GenShadow(long id);
	void SoftShadow(long id, bool randomsam, float size);

	struct Light
	{
		Light();
		~Light();

		SX_ALIGNED_OP_MEM

		int TypeLight;	
		char Name[LIGHTS_MAX_LEN_NAME];

		bool IsVisible;
		bool IsEnable;
		bool IsShadow;
		bool IsGlobal;

		ID3DXMesh* Mesh;

		float4 Position; //xyz, w - power
		float GAngleX, GAngleY; //sun pos
		float3 Direction;
		float3 Rotation;
		float3 Color;

		float2_t TopBottomRadius;
		float Angle;

		ISXBound* BoundVolume;
		float4x4 WorldMat;
		float4x4 MatRot;

		bool IsVisibleFor;
		float DistFor;

		PSSM* ShadowPSSM;
		ShadowMapTech* ShadowSM;
		ShadowMapCubeTech* ShadowCube;

		//char PathNameBoundVol[1024];
	};

protected:
	inline long AddLight(Light* obj);
	Array<Light*> ArrLights;
};

#endif