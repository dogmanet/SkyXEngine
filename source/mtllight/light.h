
#ifndef __light_h
#define __light_h

#include <gdefines.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <common/Array.h>
#include <common/String.h>
#include <d3d9.h>

#include "sxmtllight.h"
#include "ml_data.h"
extern report_func g_fnReportf;

#define LIGHTS_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < ArrIDLights.size()))\
	{g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - light: unresolved index of access '%d'", gen_msg_location, id); return stdval; }\
	else if (!ArrIDLights[id]) \
	{g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - light: light is deleted '%d'", gen_msg_location, id); return stdval; }

#define LIGHTS_PRE_COND_KEY(key,stdval) \
if (!(key >= 0 && key < ArrKeyLights.size()))\
	{g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - light: unresolved key of access '%d'", gen_msg_location, key); return stdval; }

#define LIGHTS_PRE_COND_KEY_DEL(key,stdval) \
if (!(key >= 0 && key < ArrKeyDelLights.size()))\
	{g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - light: unresolved key of access '%d'", gen_msg_location, key); return stdval; }

#include "shadow.h"
//#include <common\\string.cpp>

class Lights
{
public:

	Lights();
	~Lights();

	ID CreateCopy(ID id);

	void OnLostDevice();
	void OnResetDevice();

	int GetCountLights() const;
	void Clear();

	ID CreatePoint(ID id, const  float3* center, float dist, const  float3* color, bool isglobal, bool is_shadow, const char* bound_volume);
	ID CreateDirection(ID id, const  float3* pos, float dist, const  float3* color, const SMQuaternion* orient, float top_radius, float angle, bool is_shadow, const char* bound_volume);

	void DeleteLight(ID id);

	ID GetIdByKey(int key);

	void Render(ID id, DWORD timeDelta);

	ID GetLightGlobal() const;

	//-----------------------------------------------------------------------------

	char* GetLightName(ID id);
	void SetLightName(ID id, const char* name);

	void GetLightColor(ID id, float3* vec) const;
	void SetLightColor(ID id, const float3* vec);

	void GetLightPos(ID id, float3* vec, bool greal = false) const;
	void SetLightPos(ID id, const float3* vec, bool greal = false);

	float GetLightPower(ID id) const;

	float GetLightDist(ID id) const;
	void SetLightDist(ID id, float radius_height, bool is_create);

	//-----------------------------------------------------------------------------
	
	void GetLightOrient(ID id, SMQuaternion* q) const;
	void SetLightOrient(ID id, const  SMQuaternion* q);

	float GetLightTopRadius(ID id) const;
	void SetLightTopRadius(ID id, float top_radius);

	float GetLightAngle(ID id) const;
	void SetLightAngle(ID id, float angle, bool is_create);

	void SetLightCubeEdgeEnable(ID id, int edge, bool enable);
	bool GetLightCubeEdgeEnable(ID id, int edge) const;
	
	//-----------------------------------------------------------------------------
	
	void SetShadowBias(ID id, float val);
	float GetShadowBias(ID id) const;
	
	void SetShadowBlurPixel(ID id, float blur_pixel);
	float GetShadowBlurPixel(ID id) const;

	void SetShadowLocalNear(ID id, float slnear);
	float GetShadowLocalNear(ID id) const;

	void SetShadowLocalFar(ID id, float slfar);
	float GetShadowLocalFar(ID id) const;

	//-----------------------------------------------------------------------------
	
	bool ComVisibleForFrustum(ID id, const ISXFrustum* frustum);
	bool GetVisibleForFrustum(ID id);
	float ComDistFor(ID id, const float3* vec);
	float GetDistFor(ID id);
	void ComVisibleFrustumDistFor(const ISXFrustum* frustum, const float3* vec);

	//-----------------------------------------------------------------------------

	bool GetLightEnable(ID id) const;
	void SetLightEnable(ID id, bool val);
	bool GetLightShadowed(ID id) const;
	LightsTypeLight GetLightType(ID id) const;
	void SetLightTypeShadowed(ID id, LightsTypeShadow type);
	LightsTypeShadow GetLightTypeShadowed(ID id) const;

	//-----------------------------------------------------------------------------

	IDirect3DTexture9* GetShadow2();

	void ShadowRenderBegin(ID id);
	void ShadowRenderEnd(ID id);
	void ShadowRenderPre(ID id, int cube);
	void ShadowRenderPost(ID id, int cube);

	//-----------------------------------------------------------------------------

	void InitShaderOfTypeMaterial(ID id, int typemat, float4x4* wmat);

	//-----------------------------------------------------------------------------

	ISXFrustum* GetLightFrustum(ID id, int how) const;
	ISXFrustum* GetLightFrustumG(ID id, int split) const;

	void UpdateLightGFrustums(ID id, int split, const float3* pos, const float3* dir);

	//-----------------------------------------------------------------------------

	ID GetLightIDArr(ID id, ID inid, int how);
	void SetLightIDArr(ID id, ID inid, int how, ID id_arr);

	//-----------------------------------------------------------------------------

	bool LightCountUpdateUpdate(ID id, const float3* viewpos, int ghow = -1);
	bool LightCountUpdateAllowed(ID id, int ghow = -1) const;
	void LightCountUpdateNull(ID id);

	//-----------------------------------------------------------------------------

	void ToneMappingCom(DWORD timeDelta, float factor_adapted = 10);

	//-----------------------------------------------------------------------------

	void Set4Or3Splits(ID id, bool is4);
	bool Get4Or3Splits(ID id);

	//-----------------------------------------------------------------------------

	void ShadowGen(ID id);
	void ShadowNull();
	void ShadowGen2(ID id);
	void ShadowSoft(bool randomsam, float size, bool isfirst = false);

	//-----------------------------------------------------------------------------

	int DelGetCount();
	LightsTypeLight DelGetType(ID key);
	void DelDel(ID key);
	ID DelGetIDArr(ID key, ID inid, int how);

	//-----------------------------------------------------------------------------

	struct Light
	{
		Light();
		~Light();

		SX_ALIGNED_OP_MEM;

		ID Id;
		int Key;

		LightsTypeLight TypeLight;
		char Name[OBJECT_NAME_MAX_LEN];

		bool IsVisible;
		bool IsEnable;
		bool IsGlobal;

		LightsTypeShadow TypeShadowed;
		int CountUpdate;
		
		ID3DXMesh* Mesh;

		float3 Position;
		float Power;
		float Dist;
		float GAngleX, GAngleY; //sun pos
		SMQuaternion Quaternion;
		float3 Color;

		float2_t TopBottomRadius;
		float Angle;

		ISXBound* BoundVolume;
		float4x4 WorldMat;
		//float4x4 MatRot;
		
		bool IsVisibleFor;
		float DistFor;

		PSSM* ShadowPSSM;
		ShadowMapTech* ShadowSM;
		ShadowMapCubeTech* ShadowCube;
	};

protected:
	ID AddLight(Light* obj);

	Array<Light*> ArrKeyLights;	//массив всех элементов по порядку
	Array<Light*> ArrIDLights;	//массив всех элементов, основанный на id
	Array<Light*> ArrKeyDelLights;	//массив света который надо удалить

	ID ShadowMap;
	ID ShadowMap2;
	int HowShadow;

	ID GlobalLight;
};

#endif