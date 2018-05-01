
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __EFFECT_H
#define __EFFECT_H

#include <gdefines.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>

#include "PESet.h"

#include "emitter.h"
#include <common/array.h>
#include <common/string.h>
#include <common/file_utils.h>

#define EFFECTS_EFFECT_PRECOND_KEY(key,retval) \
if (!(key >= 0 && key < ArrKey.size()))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles: unresolved key of access '%d'", GEN_MSG_LOCATION, key); return retval; }

#define EFFECTS_EFFECT_PRECOND(id, retval) \
	if (id < 0 || id >= ArrID.size() || !(ArrID[id]))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles - unresolved address to effect %d", GEN_MSG_LOCATION, id); return retval; }

#define EFFECTS_POOL_PRECOND(id, retval) \
	if (id < 0 || id >= Pools.size())\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles - unresolved address to pool %d", GEN_MSG_LOCATION, id); return retval; } \
	if (Pools[id]->ideff < 0) \
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles - uninitialized pool %d", GEN_MSG_LOCATION, id); return retval; }

#define EFFECTS_PARTICLES_PRECOND(id, id_part, retval) \
	if (id_part < 0 || id_part >= ArrID[id]->Arr.size() || !(ArrID[id]->Arr[id_part]))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles - unresolved address to particles %d in effect %d", GEN_MSG_LOCATION, id_part, id); return retval; }

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
		void NullingInit();

		SX_ALIGNED_OP_MEM

		char Name[OBJECT_NAME_MAX_LEN];

		ID Id;
		ID Key;

		float3 CurrMin;
		float3 CurrMax;

		float3 CurrMin2;
		float3 CurrMax2;

		float3 Position, Direction, Rotation;
		float4x4 MatTranslation;
		float4x4 MatRotate;

		float ViewDist;
		bool ViewRender;

		bool Enable;
		bool Alife;
		Array<Emitter*> Arr;

		ID IDPool;
		bool Busy;
		bool Original;
	};

	void Load(const char* file);
	void Save(const char* file);
	void Clear();

	void OnLostDevice();
	void OnResetDevice();

	ID EmitterAdd(ID id, ParticlesData* data);
	void EmitterReInit(ID id, ID id_part, ParticlesData* data);
	int EmitterGetCount(ID id);
	void EmitterDelete(ID id, ID id_part);
	ParticlesData* EmitterGetData(ID id, ID id_part);

	void EmitterCountSet(ID id, ID id_part, int count);
	int EmitterCountGet(ID id, ID id_part);
	int EmitterCountLifeGet(ID id, ID id_part);

	void EmitterEnableSet(ID id, ID id_part, bool enable);
	bool EmitterEnableGet(ID id, ID id_part);

	void EmitterTextureSet(ID id, ID id_part, const char* tex);
	void EmitterTextureSetID(ID id, ID id_part, ID tex);
	ID EmitterTextureGetID(ID id, ID id_part);
	void EmitterTextureGet(ID id, ID id_part, char* tex);

	void EmitterTextureTrackSet(ID id, ID id_part, const char* tex);
	void EmitterTextureTrackSetID(ID id, ID id_part, ID tex);
	ID EmitterTextureTrackGetID(ID id, ID id_part);
	void EmitterTextureTrackGet(ID id, ID id_part, char* tex);

	void EmitterNameSet(ID id, ID id_part, const char* name);
	void EmitterNameGet(ID id, ID id_part, char* name);

	int EmitterTrackCountGet(ID id, ID id_part);
	int EmitterTrackPosGet(ID id, ID id_part, float3** arr, int count);

	ID EffectInstanceByID(ID id);
	ID EffectInstanceByName(const char* name);
	ID EffectGetByName(const char* name);
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

	bool EffectEnableGet(ID id);
	void EffectEnableSet(ID id, bool isenable);

	void EffectPlayByID(ID id, float3* pos, float3* dir);
	void EffectPlayByName(const char* name, float3* pos, float3* dir);

	bool EffectAlifeGet(ID id);
	void EffectAlifeSet(ID id, bool alife);

	void EffectPosSet(ID id, float3* pos);
	void EffectDirSet(ID id, float3* dir);
	void EffectRotSet(ID id, float3* rot);
	void EffectRotSet(ID id, const SMQuaternion & rot);

	void EffectPosGet(ID id, float3* pos);
	void EffectDirGet(ID id, float3* dir);
	void EffectRotGet(ID id, float3* rot);

	bool EffectVisibleCom(ID id, const ISXFrustum* frustum, float3* view);
	void EffectVisibleComAll(const ISXFrustum* frustum, float3* view);
	bool EffectVisibleGet(ID id);
	float EffectDistToViewGet(ID id);

protected:

	struct Pool
	{
		Pool();

		Array<ID> arr;
		ID ideff;
	};

	ID EffectCopyName(const char* name);
	ID EffectCopyID(ID id);
	void EffectDel(ID id);

	ID AddEffect(Effect* obj);
	ID PoolAdd(ID ideff);
	void PoolDelete(ID id);
	void PoolExtend(ID id);
	ID PoolGet(ID id);

	Array<Effect*> ArrKey;	//массив всех элементов по порядку
	Array<Effect*> ArrID;	//массив всех элементов, основанный на id
	Array<Pool*> Pools;
	Array<ID> ArrSort;
	int ArrSortSizeCurr;	//текущий размер массива ArrSort
};

#endif