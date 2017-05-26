
#ifndef __effect_h
#define __effect_h

#include <particles/particles.h>
#include <common/array.h>

#define EFFECTS_EFFECT_PRECOND_KEY(key,retval) \
if (!(key >= 0 && key < ArrKey.size()))\
	{reportf(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles: unresolved key of access '%d'", gen_msg_location, key); return retval; }

#define EFFECTS_EFFECT_PRECOND(id, retval) \
	if (id >= ArrID.size() || !(ArrID[id]))\
		{reportf(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles - unresolved address to effect %d", gen_msg_location, id); return retval; }

#define EFFECTS_PARTICLES_PRECOND(id, id_part, retval) \
	if (id_part >= ArrID[id]->Arr.size() || !(ArrID[id]->Arr[id_part]))\
		{reportf(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles - unresolved address to particles %d in effect %d", gen_msg_location, id_part, id); return retval; }

#define EFFECTS_PRECOND(id, id_part, retval) \
	EFFECTS_EFFECT_PRECOND(id, retval); \
	EFFECTS_PARTICLES_PRECOND(id, id_part, retval);



class Effects
{
public:
	Effects();
	~Effects();

	SX_ALIGNED_OP_MEM

	struct Effect
	{
		Effect();
		~Effect();

		SX_ALIGNED_OP_MEM

		char Name[OBJECT_NAME_MAX_LEN];

		ID Id;
		ID Key;

		float3_t CurrMin;
		float3_t CurrMax;

		float3 Position, Direction, Rotation;
		float4x4 MatTranslation;
		float4x4 MatRotate;
		float4x4 MatTransform;

		float DistForCamera;
		bool IsRenderForCamera;

		ID IDLight;
		bool IsAlife;

		bool IsEnable;
		Array<Particles*> Arr;
	};

	void OnLostDevice();
	void OnResetDevice();

	inline ID ParticlesAdd(ID id, ParticlesData* data);
	inline void ParticlesReInit(ID id, ID id_part, ParticlesData* data);
	inline int ParticlesGetCount(ID id);
	inline void ParticlesDelete(ID id, ID id_part);
	inline ParticlesData* ParticlesGetData(ID id, ID id_part);

	inline void ParticlesCreate(ID id, ID id_part, int count);
	inline void ParticlesReCreate(ID id, ID id_part, int count = 0);

	inline void ParticlesTextureSet(ID id, ID id_part, ID tex);

	void ParticlesNameSet(ID id, ID id_part, const char* name);
	void ParticlesNameGet(ID id, ID id_part, char* name);


	ID EffectAdd(const char* name);
	int EffectCountGet();
	ID EffectIdOfKey(ID key);
	void EffectDelete(ID id);
	void EffectNameSet(ID id, const char* name);
	void EffectNameGet(ID id, char* name);

	void EffectCompute(ID id);
	void EffectComputeLighting(ID id);
	void EffectRender(ID id, DWORD timeDelta);

	void EffectComputeAll();
	void EffectComputeLightingAll();
	void EffectRenderAll(DWORD timeDelta);

	inline bool EffectAlifeGet(ID id);

	inline bool EffectEnableGet(ID id);
	inline void EffectEnableSet(ID id, bool isenable);

	inline void EffectPosSet(ID id, float3* pos);
	inline void EffectDirSet(ID id, float3* dir);
	inline void EffectRotSet(ID id, float3* rot);

	inline void EffectPosGet(ID id, float3* pos);
	inline void EffectDirGet(ID id, float3* dir);
	inline void EffectRotGet(ID id, float3* rot);

	inline bool EffectVisibleCom(ID id, ISXFrustum* frustum, float3* view);
	inline void EffectVisibleComAll(ISXFrustum* frustum, float3* view);
	inline bool EffectVisibleGet(ID id);
	inline float EffectDistToViewGet(ID id);

	inline void EffectLightSet(ID id, float3* color, float dist, bool is_shadow);
	inline void EffectLightDelete(ID id);
	
protected:

	inline ID AddEffect(Effect* obj);

	inline void EffectLightEnable(ID id, bool is_enable);
	inline void EffectLightRotationSet(ID id);
	inline void EffectLightPositionSet(ID id);

	Array<Effect*> ArrKey;	//массив всех элементов по порядку
	Array<Effect*> ArrID;	//массив всех элементов, основанный на id

	//Array<Effect*> Arr;
};

#endif