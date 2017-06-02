
#ifndef __effect_h
#define __effect_h

#include <particles/emitter.h>
#include <common/array.h>
#include <common/string.cpp>

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
		Effect(Effect& eff);
		~Effect();
		inline void NullingInit();

		SX_ALIGNED_OP_MEM

		char Name[OBJECT_NAME_MAX_LEN];

		ID Id;
		ID Key;

		float3 CurrMin;
		float3 CurrMax;

		float3 Position, Direction, Rotation;
		float4x4 MatTranslation;
		float4x4 MatRotate;

		float ViewDist;
		bool ViewRender;

		bool Enable;
		bool Alife;
		Array<Emitter*> Arr;
	};

	void Load(const char* file);
	void Save(const char* file);
	void Clear();

	inline void OnLostDevice();
	inline void OnResetDevice();

	inline ID EmitterAdd(ID id, ParticlesData* data);
	inline void EmitterReInit(ID id, ID id_part, ParticlesData* data);
	inline int EmitterGetCount(ID id);
	inline void EmitterDelete(ID id, ID id_part);
	inline ParticlesData* EmitterGetData(ID id, ID id_part);

	inline void EmitterCountSet(ID id, ID id_part, int count);
	inline int EmitterCountGet(ID id, ID id_part);
	inline int EmitterCountLifeGet(ID id, ID id_part);

	inline void EmitterEnableSet(ID id, ID id_part, bool enable);
	inline bool EmitterEnableGet(ID id, ID id_part);

	inline void EmitterTextureSet(ID id, ID id_part, const char* tex);
	inline void EmitterTextureSetID(ID id, ID id_part, ID tex);
	inline ID EmitterTextureGetID(ID id, ID id_part);
	inline void EmitterTextureGet(ID id, ID id_part, char* tex);

	inline void EmitterNameSet(ID id, ID id_part, const char* name);
	inline void EmitterNameGet(ID id, ID id_part, char* name);

	inline ID EffectCopyName(const char* name);
	inline ID EffectCopyID(ID id);
	inline ID EffectGetByName(const char* name);
	inline ID EffectAdd(const char* name);
	inline int EffectCountGet();
	inline ID EffectIdOfKey(ID key);
	inline void EffectDelete(ID id);
	inline void EffectNameSet(ID id, const char* name);
	inline void EffectNameGet(ID id, char* name);

	void EffectCompute(ID id);
	void EffectComputeLighting(ID id);
	void EffectRender(ID id, DWORD timeDelta);

	void EffectComputeAll();
	void EffectComputeLightingAll();
	void EffectRenderAll(DWORD timeDelta);

	inline bool EffectEnableGet(ID id);
	inline void EffectEnableSet(ID id, bool isenable);

	inline bool EffectAlifeGet(ID id);
	inline void EffectAlifeSet(ID id, bool alife);

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

protected:

	inline ID AddEffect(Effect* obj);

	Array<Effect*> ArrKey;	//массив всех элементов по порядку
	Array<Effect*> ArrID;	//массив всех элементов, основанный на id
	Array<ID> ArrSort;
	int ArrSortSizeCurr;	//текущий размер массива ArrSort
};

#endif