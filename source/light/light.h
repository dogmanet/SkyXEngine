
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __LIGHT_H
#define __LIGHT_H

#include <gdefines.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <common/Array.h>
#include <common/String.h>
#include <d3d9.h>

#include "sxlight.h"
#include "ml_data.h"

#define LIGHTS_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < ArrLights.size()))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - light: unresolved index of access '%d'", GEN_MSG_LOCATION, id); return stdval; }\
	else if (!ArrLights[id]) \
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - light: light is deleted '%d'", GEN_MSG_LOCATION, id); return stdval; }

#define LIGHTS_PRE_COND_KEY_DEL(key,stdval) \
if (!(key >= 0 && key < ArrDelLights.size()))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - light: unresolved key of access '%d'", GEN_MSG_LOCATION, key); return stdval; }

#include "shadow.h"


class CLights
{
public:

	CLights();
	~CLights();

	ID createCopy(ID id);

	void onLostDevice();
	void onResetDevice();

	int getCountLights() const;
	void clear();

	//очистка массивов идентификацторов (просчетов видимости)
	void clearIDArr();

	bool getExists(ID id) const;

	ID createPoint(ID id, const  float3* center, float dist, const  float3* color, bool isglobal, bool is_shadow/*, const char* bound_volume*/);
	ID createDirection(ID id, const  float3* pos, float dist, const  float3* color, const SMQuaternion* orient, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/);

	void deleteLight(ID id);

	void render(ID id, DWORD timeDelta);

	ID getLightGlobal() const;

	bool getCastGlobalShadow() const;
	void setCastGlobalShadow(bool isShadowed);

	//-----------------------------------------------------------------------------

	char* getLightName(ID id);
	void setLightName(ID id, const char* name);

	void getLightColor(ID id, float3* vec) const;
	void setLightColor(ID id, const float3* vec);

	void getLightPos(ID id, float3* vec, bool greal = false) const;
	void setLightPos(ID id, const float3* vec, bool greal = false);

	float getLightPower(ID id) const;

	float getLightDist(ID id) const;
	void setLightDist(ID id, float radius_height, bool is_create);

	//-----------------------------------------------------------------------------
	
	void getLightOrient(ID id, SMQuaternion* q) const;
	void setLightOrient(ID id, const  SMQuaternion* q);

	float getLightTopRadius(ID id) const;
	void setLightTopRadius(ID id, float top_radius);

	float getLightAngle(ID id) const;
	void setLightAngle(ID id, float angle, bool is_create);

	void setLightCubeEdgeEnable(ID id, int edge, bool enable);
	bool getLightCubeEdgeEnable(ID id, int edge) const;
	
	//-----------------------------------------------------------------------------
	
	void setShadowBias(ID id, float val);
	float getShadowBias(ID id) const;
	
	void setShadowBlurPixel(ID id, float blur_pixel);
	float getShadowBlurPixel(ID id) const;

	void setShadowLocalNear(ID id, float slnear);
	float getShadowLocalNear(ID id) const;

	void setShadowLocalFar(ID id, float slfar);
	float getShadowLocalFar(ID id) const;

	//-----------------------------------------------------------------------------
	
	bool comVisibleForFrustum(ID id, const IFrustum* frustum);
	bool getVisibleForFrustum(ID id);
	float comDistFor(ID id, const float3* vec);
	float getDistFor(ID id);
	void comVisibleFrustumDistFor(const IFrustum* frustum, const float3* vec);

	//-----------------------------------------------------------------------------

	bool getLightEnable(ID id) const;
	void setLightEnable(ID id, bool val);
	bool getLightShadowed(ID id) const;
	LTYPE_LIGHT getLightType(ID id) const;
	void setLightTypeShadowed(ID id, LTYPE_SHADOW type);
	LTYPE_SHADOW getLightTypeShadowed(ID id) const;

	//-----------------------------------------------------------------------------

	IDirect3DTexture9* getShadow2();

	void shadowRenderBegin(ID id);
	void shadowRenderEnd(ID id);
	void shadowRenderPre(ID id, int cube);
	void shadowRenderPost(ID id, int cube);

	//-----------------------------------------------------------------------------

	void initShaderOfTypeMaterial(ID id, int typemat, const float4x4* wmat);

	//-----------------------------------------------------------------------------

	IFrustum* getLightFrustum(ID id, int how) const;
	IFrustum* getLightFrustumG(ID id, int split) const;

	void updateLightGFrustums(ID id, int split, const float3* pos, const float3* dir);

	//-----------------------------------------------------------------------------

	ID getLightIDArr(ID id, ID inid, int how);
	void setLightIDArr(ID id, ID inid, int how, ID id_arr);

	//-----------------------------------------------------------------------------

	bool lightCountUpdateUpdate(ID id, const float3* viewpos, int ghow = -1);
	bool lightCountUpdateAllowed(ID id, int ghow = -1) const;
	void lightCountUpdateNull(ID id);

	//-----------------------------------------------------------------------------

	void toneMappingCom(DWORD timeDelta, float factor_adapted = 10);

	//-----------------------------------------------------------------------------

	void set4Or3Splits(ID id, bool is4);
	bool get4Or3Splits(ID id);

	//-----------------------------------------------------------------------------

	void shadowGen(ID id);
	void shadowNull();
	void shadowGen2(ID id);
	void shadowSoft(bool randomsam, float size, bool isfirst = false);

	//-----------------------------------------------------------------------------

	int delGetCount();
	LTYPE_LIGHT delGetType(ID key);
	void delDel(ID key);
	ID delGetIDArr(ID key, ID inid, int how);

	//-----------------------------------------------------------------------------

	struct Light
	{
		Light();
		~Light();

		SX_ALIGNED_OP_MEM;

		ID Id;
		
		LTYPE_LIGHT TypeLight;
		char Name[OBJECT_NAME_MAX_LEN];

		bool IsVisible;
		bool IsEnable;
		bool IsGlobal;

		LTYPE_SHADOW TypeShadowed;
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
	ID addLight(Light* obj);

	Array<Light*> ArrLights;	//массив всех элементов, основанный на id
	Array<Light*> ArrDelLights;	//массив света который надо удалить
	Array<ID> ArrFreeIDs;		//массив света который надо удалить

	ID ShadowMap;
	ID ShadowMap2;
	int HowShadow;

	ID GlobalLight;
	bool m_isCastGlobalShadow;
};

#endif