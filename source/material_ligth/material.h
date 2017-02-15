
#ifndef __sxmaterial
#define __sxmaterial

#define MTL_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < ArrMaterials.size()))\
	{reportf(REPORT_MSG_LEVEL_ERROR, "%s - material: unresolved index of access '%d'", gen_msg_location, id); return stdval; }

class Materials
{
public:
	Materials();
	~Materials();

	SX_ALIGNED_OP_MEM

	long Load(const char* name, int type = 0);
	void Save();
	void SetMainTexture(DWORD slot, long id);
	void Render(long id, float4x4* world);

	void RenderDepthPSSMDirect(long id, float4x4* world);
	void RenderDepthCube(long id, float4x4* world);

	long IsExists(const char* name);
	int GetType(long id);
	long GetID(const char* name);

	inline long GetCount();

	struct Material
	{
		Material();
		~Material();

		SX_ALIGNED_OP_MEM

		//структура из материала указывающая на маску и 4 детальных и 4 микрорельефных карты
		struct MaterialMaskPM
		{
			MaterialMaskPM();
			~MaterialMaskPM();

			DWORD Mask;
			DWORD ArrDeatail[4];
			DWORD ArrMicroDiffuse[4];
		};

		//структура из материала определяющая состояния рендера
		struct MaterialRenderStates
		{
			MaterialRenderStates();
			~MaterialRenderStates();

			bool IsCullBack;
			bool IsAlphaTest;
		};

		//структура из материала определяющая основные характеристики просчета освещения
		struct MaterialLightParam
		{
			MaterialLightParam();
			~MaterialLightParam();

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
			float4 Param;
		};

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

		int Type;

		float4x4 view, proj, worldtrans, viewtrans, projtrans;
	};

protected:

	//структура описывающая папку и все текстуры в ней, у каждой свой id для доступа
	struct TLPath
	{
		TLPath()
		{
			Path[0] = 0;
		}

		char Path[MTL_MAX_SIZE_DIR];//имя папки
		Array<long> ArrID;		//идентификатор
		Array<String> ArrNames;	//массив с именами текстур которые находятся в данной папке
	};
	Array<TLPath*> ArrHMtls;

	void AddName(const char* name,long id);

	Array<Material*> ArrMaterials;
	float4x4 view, proj, worldtrans, viewtrans, projtrans;
};

#endif