
#ifndef __light_h
#define __light_h

#define LIGHTS_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < ArrIDLights.size()))\
	{reportf(REPORT_MSG_LEVEL_ERROR, "%s - light: unresolved index of access '%d'", gen_msg_location, id); return stdval; }\
	else if (!ArrIDLights[id]) \
	{reportf(REPORT_MSG_LEVEL_ERROR, "%s - light: light is deleted '%d'", gen_msg_location, id); return stdval; }

#define LIGHTS_PRE_COND_KEY(key,stdval) \
if (!(key >= 0 && key < ArrKeyLights.size()))\
	{reportf(REPORT_MSG_LEVEL_ERROR, "%s - light: unresolved key of access '%d'", gen_msg_location, key); return stdval; }

#define LIGHTS_PRE_COND_KEY_DEL(key,stdval) \
if (!(key >= 0 && key < ArrKeyDelLights.size()))\
	{reportf(REPORT_MSG_LEVEL_ERROR, "%s - light: unresolved key of access '%d'", gen_msg_location, key); return stdval; }

#include <mtllight\\shadow.cpp>
#include <common\\string.cpp>

class Lights
{
public:

	Lights();
	~Lights();

	ID CreateCopy(ID id);

	void OnLostDevice();
	void OnResetDevice();

	void Save(const char* path);
	void Load(const char* path);

	inline long GetCountLights();
	void Clear();

	ID CreatePoint(ID id, float3* center, float power, float dist, float3* color, bool isglobal, bool is_shadowed, const char* bound_volume);
	ID CreateDirection(ID id, float3* pos, float power, float dist, float3* color, float3* dir, float top_radius, float angle, bool is_shadow, const char* bound_volume);

	void DeleteLight(ID id);

	void Render(ID id, DWORD timeDelta);

	ID GetLightGlobal();

	char* GetLightName(ID id);
	void SetLightName(ID id, const char* name);

	void GetLightColor(ID id, float3* vec);
	void SetLightColor(ID id, float3* vec);

	void GetLightPos(ID id, float3* vec, bool greal = false);
	void SetLightPos(ID id, float3* vec, bool greal = false);

	void SetLightPower(ID id, float power);
	float GetLightPower(ID id);
	float GetLightPowerDiv(ID id);

	float GetLightDist(ID id);

	void GetLightRot(ID id, float3* vec);
	void SetLightRot(ID id, float3* vec);
	void GetLightDir(ID id, float3* vec);
	void SetLightDir(ID id, float3* vec);

	void SetLightBias(ID id, float val);
	float GetLightBias(ID id);

	float GetLightTopRadius(ID id);
	float GetLightAngle(ID id);

	void ChangeAngle(ID id, float angle, bool is_create);
	void ChangeTopRadius(ID id, float top_radius);
	void ChangeRadiusHeight(ID id, float radius_height, bool is_create);
	//void ChangeShadow(ID id, bool is_shadow);
	void SetBlurPixel(ID id, float blur_pixel);
	float GetBlurPixel(ID id);
	void SetShadowLocalNear(ID id, float slnear);
	float GetShadowLocalNear(ID id);
	void SetShadowLocalFar(ID id, float slfar);
	float GetShadowLocalFar(ID id);
	void SetEnableCubeEdge(ID id, int edge, bool enable);
	bool GetEnableCubeEdge(ID id, int edge);

	bool ComVisibleForFrustum(ID id, ISXFrustum* frustum);
	bool GetVisibleForFrustum(ID id);
	float ComDistFor(ID id, float3* vec);
	float GetDistFor(ID id);
	void ComVisibleFrustumDistFor(ISXFrustum* frustum, float3* vec);

	bool IsEnable(ID id);
	void SetEnable(ID id, bool val);
	bool IsShadow(ID id);
	LightsTypeLight GetType(ID id);

	IDirect3DTexture9* GetShadow2();

	void InRenderBegin(ID id);
	void InRenderEnd(ID id);
	void InRenderPre(ID id, int cube);
	void InRenderPost(ID id, int cube);

	void InitShaderOfTypeMaterial(ID id, int typemat, float4x4* wmat);

	ISXFrustum* GetFrustum(ID id, int how);
	ISXFrustum* GetFrustumG(ID id, int split);

	void UpdateFrustumsG(ID id, int split, float3* pos, float3* dir);

	void GenShadow(ID id);
	inline void NullingShadow();
	void GenShadow2(ID id);
	void SoftShadow(bool randomsam, float size, bool isfirst = false);

	ID GetLightIDArr(ID id, ID inid, int how);
	void SetLightIDArr(ID id, ID inid, int how, ID id_arr);

	void SetLightTypeShadowed(ID id, LightsTypeShadow type);
	inline LightsTypeShadow GetLightTypeShadowed(ID id);

	bool UpdateLightCountUpdate(ID id, float3* viewpos, int ghow = -1);
	bool AllowedRenderLight(ID id, int ghow = -1);
	inline void SetNullLightCountUpdate(ID id);

	void ComToneMapping(DWORD timeDelta, float factor_adapted = 10);

	inline void Set4Or3Splits(ID id, bool is4);
	inline bool Get4Or3Splits(ID id);

	struct Light
	{
		Light();
		~Light();

		SX_ALIGNED_OP_MEM;

		ID Id;
		long Key;

		LightsTypeLight TypeLight;
		char Name[OBJECT_NAME_MAX_LEN];

		bool IsVisible;
		bool IsEnable;
		//bool IsShadow;
		bool IsGlobal;

		LightsTypeShadow TypeShadowed;
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
	};

	ID GetIdOfKey(long key);

	long DelGetCount();
	LightsTypeLight DelGetType(ID key);
	void DelDel(ID key);
	ID DelGetIDArr(ID key, ID inid, int how);

protected:
	inline ID AddLight(Light* obj);

	Array<Light*> ArrKeyLights;	//массив всех элементов по порядку
	Array<Light*> ArrIDLights;	//массив всех элементов, основанный на id
	Array<Light*> ArrKeyDelLights;	//массив света который надо удалить

	ID ShadowMap;
	ID ShadowMap2;
	int HowShadow;

	ID GlobalLight;
};

#endif