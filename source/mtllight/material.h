
#ifndef __sxmaterial
#define __sxmaterial

#include <mtllight\\reflection.h>
#include <direct.h>
#define MTL_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < ArrMaterials.size()))\
	{reportf(REPORT_MSG_LEVEL_ERROR, "%s - material: unresolved index of access '%d'", gen_msg_location, id); return stdval; }\
else if (!ArrMaterials[id])\
	{reportf(REPORT_MSG_LEVEL_ERROR, "%s - material: material '%d' is not init", gen_msg_location, id); return stdval; }

#define MTL_REF_PRE_COND_ID(id,stdval) \
MTL_PRE_COND_ID(id, stdval)\
if (!(ArrMaterials[id]->Reflect))\
	{reportf(REPORT_MSG_LEVEL_ERROR, "%s - material: material id = '%d', name = '%s' unsupported reflection", gen_msg_location, id, ArrMaterials[id]->mtl->Name); return stdval; }

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
	void RenderStd(MtlTypeModel type, float4x4* world, ID slot, ID id_mtl, bool is_cp = false, float3* plane_normal = 0, float3* plane_point = 0);
	void RenderLight(float4_t* color, float4x4* world);
	inline long GetCount();

	inline void SetForceblyAlphaTest(bool isat);
	inline bool GetForceblyAlphaTest();

	inline void SetIsIncrCountSurf(bool bf);
	inline bool GetIsIncrCountSurf();
	inline void NullingCurrCountSurf();
	inline int GetCurrCountSurf();
	inline void SetCurrCountSurf(int count);


	ID MtlLoad(const char* name, MtlTypeModel type = MtlTypeModel::tms_static);
	void MtlReLoad(ID id, const char* name = 0);
	void MtlSave(ID id);
	
	bool MtlGetUsingAlphaTest(ID id);
	void MtlSetUsingAlphaTest(ID id, bool is_using);
	MtlTypeReflect MtlTypeReflection(ID id);

	ID GetStdMtl(MtlTypeModel type_model);
	ID IsExists(const char* name);
	MtlTypeModel GetTypeModel(ID id);
	void SetTypeModel(ID id, MtlTypeModel type_model);
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

	void MtlSetPhysicMaterial(ID id, MtlPhysicType type);
	MtlPhysicType MtlGetPhysicMaterial(ID id);

	void MtlSetTexture(ID id, const char* path_tex);
	void MtlGetTexture(ID id, char* name);
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

	void MtlSetTypeTransparency(ID id, MtlTypeTransparency type);
	MtlTypeTransparency MtlGetTypeTransparency(ID id);

	void MtlSetTypeReflection(ID id, MtlTypeReflect type);
	MtlTypeReflect MtlGetTypeReflection(ID id);


	void MtlSetMaskTex(ID id, const char* path_tex);
	void MtlGetMaskTex(ID id, char* path_tex);

	void MtlSetMRTex(ID id, int channel, const char* path_tex);
	void MtlGetMRTex(ID id, int channel, char* path_tex);

	void MtlSetDTex(ID id, int channel, const char* path_tex);
	void MtlGetDTex(ID id, int channel, char* path_tex);


	void MtlSetSTDVS(ID id, MtlTransShaderStd type, bool is_send);
	bool MtlGetSTDVS(ID id, MtlTransShaderStd type);
	void MtlSetSTDPS(ID id, MtlTransShaderStd type, bool is_send);
	bool MtlGetSTDPS(ID id, MtlTransShaderStd type);


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

			float OldRoughnessValue;
			float OldF0Value;

			MtlTypeReflect TypeReflect;
			MtlTypeTransparency TypeRefraction;
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

		MtlPhysicType PhysicsMaterial;

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

		MtlTypeModel Type;
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
	void CreateMtl(const char* name, Material** mtl, MtlTypeModel type);
	ID CreateTexParamLighting(float roughness, float f0);

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