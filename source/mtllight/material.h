
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __MATERIAL_H
#define __MATERIAL_H

#include <common/String.h>
#include <common/Array.h>
#include <common/file_utils.h>
#include "reflection.h"
#include <direct.h>

#define MTL_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < ArrMaterials.size()))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - material: unresolved index of access '%d'", GEN_MSG_LOCATION, id); return stdval; }\
else if (!ArrMaterials[id])\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - material: material '%d' is not init", GEN_MSG_LOCATION, id); return stdval; }

#define MTL_REF_PRE_COND_ID(id,stdval) \
MTL_PRE_COND_ID(id, stdval)\
if (!(ArrMaterials[id]->Reflect))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - material: material id = '%d', name = '%s' unsupported reflection", GEN_MSG_LOCATION, id, ArrMaterials[id]->mtl->Name); return stdval; }

class Materials
{
public:
	Materials();
	~Materials();

	void OnLostDevice();
	void OnResetDevice();

	SX_ALIGNED_OP_MEM

	void Clear(bool clear_del);
	void Update(DWORD timeDelta);
	void SetMainTexture(ID slot, ID id);
	void Render(ID id, float4x4* world);
	void RenderStd(MTLTYPE_MODEL type, float4x4* world, ID slot, ID id_mtl);
	void RenderLight(float4_t* color, float4x4* world);
	long GetCount();

	void SetForceblyAlphaTest(bool isat);
	bool GetForceblyAlphaTest();

	void SetIsIncrCountSurf(bool bf);
	bool GetIsIncrCountSurf();
	void NullingCurrCountSurf();
	int GetCurrCountSurf();
	void SetCurrCountSurf(int count);


	ID MtlLoad(const char* name, MTLTYPE_MODEL type = MTLTYPE_MODEL_STATIC);
	void MtlReLoad(ID id, const char* name = 0);
	void MtlSave(ID id);
	
	bool MtlGetUsingAlphaTest(ID id);
	void MtlSetUsingAlphaTest(ID id, bool is_using);
	MTLTYPE_REFLECT MtlTypeReflection(ID id);

	ID GetStdMtl(MTLTYPE_MODEL type_model);
	ID IsExists(const char* name);
	MTLTYPE_MODEL GetTypeModel(ID id);
	void SetTypeModel(ID id, MTLTYPE_MODEL type_model);
	ID GetID(const char* name);

	

	///////

	void MtlRefSetIDArr(ID id, ID inid, int cube, ID idarr);
	ID MtlRefGetIDArr(ID id, ID inid, int cube);

	//void MtlRefSetPlane(ID id, D3DXPLANE* plane);
	//void MtlRefSetCenter(ID id, float3_t* center);
	void MtlRefPreRenderPlane(ID id, D3DXPLANE* plane);
	ISXFrustum* MtlRefGetfrustum(ID id, int cube);
	void MtlRefPostRenderPlane(ID id);
	IDirect3DTexture9* MtlRefPlaneGetTex(ID id);

	void MtlRefSetMinMax(ID id, float3_t* min, float3_t* max);
	bool MtlRefIsAllowedRender(ID id);

	void MtlRefCubeBeginRender(ID id, float3_t* center);
	void MtlRefCubePreRender(ID id, int cube, float4x4* world);
	void MtlRefCubePostRender(ID id, int cube);
	void MtlRefCubeEndRender(ID id, float3_t* viewpos);
	bool MtlRefUpdateCountUpdate(ID id, float3_t* viewpos);
	void MtlRefNullingCountUpdate(ID id);
	IDirect3DCubeTexture9* RefCubeGetTex(ID id);

	//////

	void MtlSetPhysicMaterial(ID id, MTLTYPE_PHYSIC type);
	MTLTYPE_PHYSIC MtlGetPhysicMaterial(ID id);

	void MtlSetTexture(ID id, const char* path_tex);
	void MtlGetTexture(ID id, char* name);
	ID MtlGetTextureID(ID id);
	void MtlSetVS(ID id, const char* path_vs);
	void MtlGetVS(ID id, char* name);
	void MtlSetPS(ID id, const char* path_vs);
	void MtlGetPS(ID id, char* name);


	void MtlSetLighting(ID id, bool is_lighting);
	bool MtlGetLighting(ID id);

	void MtlSetIsTextureLighting(ID id, bool is_tex_lighting);
	bool MtlGetIsTextureLighting(ID id);

	void MtlSetTextureLighting(ID id, const char* path_tex);
	void MtlGetTextureLighting(ID id, char* path_tex);

	void MtlSetRoughness(ID id, float roughness);
	float MtlGetRoughness(ID id);

	void MtlSetF0(ID id, float f0);
	float MtlGetF0(ID id);

	void MtlSetThickness(ID id, float thickness);
	float MtlGetThickness(ID id);

	void MtlSetPenetration(ID id, float penetration);
	float MtlGetPenetration(ID id);

	void MtlSetHitChance(ID id, float fHitChance);
	float MtlGetHitChance(ID id);

	void MtlSetDensity(ID id, float fDensity);
	float MtlGetDensity(ID id);

	void MtlSetTypeTransparency(ID id, MTLTYPE_TRANSPARENCY type);
	MTLTYPE_TRANSPARENCY MtlGetTypeTransparency(ID id);

	void MtlSetTypeReflection(ID id, MTLTYPE_REFLECT type);
	MTLTYPE_REFLECT MtlGetTypeReflection(ID id);


	void MtlSetMaskTex(ID id, const char* path_tex);
	void MtlGetMaskTex(ID id, char* path_tex);

	void MtlSetMRTex(ID id, int channel, const char* path_tex);
	void MtlGetMRTex(ID id, int channel, char* path_tex);

	void MtlSetDTex(ID id, int channel, const char* path_tex);
	void MtlGetDTex(ID id, int channel, char* path_tex);


	void MtlSetSTDVS(ID id, MTL_SHADERSTD type, bool is_send);
	bool MtlGetSTDVS(ID id, MTL_SHADERSTD type);
	void MtlSetSTDPS(ID id, MTL_SHADERSTD type, bool is_send);
	bool MtlGetSTDPS(ID id, MTL_SHADERSTD type);


	void MtlSetUDVS(ID id, int component, float val);
	float MtlGetUDVS(ID id, int component);
	void MtlSetUDVS_InPS(ID id, bool is_send_ps);
	bool MtlGetUDVS_InPS(ID id);
	void MtlSetUDPS(ID id, int component, float val);
	float MtlGetUDPS(ID id, int component);
	void MtlSetUDPS_InPS(ID id, bool is_send_vs);
	bool MtlGetUDPS_InPS(ID id);

	//////

	int DelRefCount();
	ID DelRefGetIDArr(ID key, ID inid, int cube);
	void DelRefAllDel();

	//////

	struct Material
	{
		Material();
		~Material();

		SX_ALIGNED_OP_MEM

		void Nulling();

		bool IsDelete;

		//структура из материала указывающая на маску и 4 детальных и 4 микрорельефных карты
		struct MaterialMaskPM
		{
			MaterialMaskPM();
			~MaterialMaskPM();

			ID Mask;
			ID ArrDeatail[4];
			ID ArrMicroDiffuse[4];
		};

		//структура из материала определяющая основные характеристики просчета освещения
		struct MaterialLightParam
		{
			MaterialLightParam();
			~MaterialLightParam();

			ID ParamTex;
			ID ParamTexHand;

			//FOR EDITORS
			bool IsTextureParam;

			float RoughnessValue;
			float F0Value;
			float ThicknessValue;

			float OldRoughnessValue;
			float OldF0Value;
			float OldThicknessValue;

			MTLTYPE_REFLECT TypeReflect;
			MTLTYPE_TRANSPARENCY TypeRefraction;
		};

		struct MaterialDataShader
		{
			MaterialDataShader();
			~MaterialDataShader();

			SX_ALIGNED_OP_MEM

			bool IsTransWorld;
			bool IsTransView;
			bool IsTransProjection;
			bool IsTransWorldView;
			bool IsTransWorldViewProjection;
			bool IsTransPosCam;
			bool IsTransTimeDelta;
			bool IsTransUserData;
			bool IsTransWinSize;
			float4 Param;
		};

		MTLTYPE_PHYSIC PhysicsMaterial;

		char Name[64];
		ID MainTexture;
		ID PreShaderVS;
		ID PreShaderPS;
		
		MaterialMaskPM MicroDetail;
		MaterialLightParam LightParam;

		MaterialDataShader VS;
		MaterialDataShader PS;

		bool TransVSDataInPS;
		bool TransPSDataInVS;

		bool IsUnlit;
		bool IsAlphaTest;

		float Penetration;
		float HitChance;
		float Density;

		MTLTYPE_MODEL Type;
	};

	struct UnitMaterial
	{
		UnitMaterial();
		~UnitMaterial();

		Material* mtl;
		Reflection* Reflect;
	};

protected:

	bool IsForceblyAlphaTest;

	bool LoadMtl(const char* name, Material** mtl);
	void CreateMtl(const char* name, Material** mtl, MTLTYPE_MODEL type);
	ID CreateTexParamLighting(float roughness, float f0, float thickness);

	//структура описывающая папку и все текстуры в ней, у каждой свой id для доступа
	struct TLPath
	{
		TLPath(){}

		String Path;

		struct TLTex
		{
			TLTex(){ id = -1; }
			TLTex(ID _id, const char* _name){ id = _id; name = _name; }
			ID id;
			String name;
		};

		Array<TLTex*> ArrTex;
	};
	Array<TLPath*> ArrHMtls;

	void AddName(const char* name, ID id);
	ID AddUnitMaterial(UnitMaterial* umtl);
	ID AddMaterial(Material* mtl);

	Array<UnitMaterial*> ArrMaterials;
	Array<Material*> ArrMtrls;
	Array<Reflection*> ArrDelRefMtrls;

	bool IsIncrCountSurf;
	int CurrIdSurf;

	int CurrFirstFree;

	DWORD CurrTimeDelta;
	DWORD CountTimeDelta;
	ID MtrlDefLight;
	ID MtrlDefTree;
	ID MtrlDefGrass;
	ID MtrlDefStatic;
	ID MtrlDefSkin;
	ID BeginNonDef;

	float4x4 view, proj, worldtrans, viewtrans, projtrans;
};

#endif