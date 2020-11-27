
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __EFFECT_H
#define __EFFECT_H

#include <gdefines.h>

//#define SM_D3D_CONVERSIONS
#include <common/Math.h>

#include "pe_data.h"

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
if (id_part < 0 || id_part >= ArrID[id]->m_aEmitters.size() || !(ArrID[id]->m_aEmitters[id_part]))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles - unresolved address to particles %d in effect %d", GEN_MSG_LOCATION, id_part, id); return retval; }

#define EFFECTS_PRECOND(id, id_part, retval) \
	EFFECTS_EFFECT_PRECOND(id, retval); \
	EFFECTS_PARTICLES_PRECOND(id, id_part, retval);



class CEffects
{
public:
	CEffects();
	~CEffects();

	SX_ALIGNED_OP_MEM

	struct CEffect
	{
		CEffect();
		CEffect(CEffect& eff);
		~CEffect();

		void nullingInit();

		SX_ALIGNED_OP_MEM

		char m_szName[OBJECT_NAME_MAX_LEN];

		ID m_id;
		ID m_idKey;

		float3 m_vCurrMin;
		float3 m_vCurrMax;

		float3 m_vCurrMin2;
		float3 m_vCurrMax2;

		float3 m_vPosition, m_vDirection, m_vRotation;
		float4x4 m_mTranslation;
		float4x4 m_mRotate;

		float m_fViewDist;
		bool m_isViewRender;

		bool m_isEnable;
		bool m_isAlife;
		Array<CEmitter*> m_aEmitters;

		ID m_idPool;
		bool m_isBusy;
		bool m_isOriginal;
	};

	void load(const char* file);
	void save(const char* file);
	void clear();

	void onLostDevice();
	void onResetDevice();

	ID emitterAdd(ID id, CParticlesData* data);
	void emitterReInit(ID id, ID id_part, CParticlesData* data);
	int emitterGetCount(ID id);
	void emitterDelete(ID id, ID id_part);
	CParticlesData* emitterGetData(ID id, ID id_part);

	void emitterSetCount(ID id, ID id_part, int count);
	int emitterGetCount(ID id, ID id_part);
	int emitterGetCountLife(ID id, ID id_part);

	void emitterSetEnable(ID id, ID id_part, bool enable);
	bool emitterGetEnable(ID id, ID id_part);

	void emitterSetTexture(ID id, ID id_part, const char* tex);
	void emitterSetTextureID(ID id, ID id_part, ID tex);
	ID emitterGetTextureID(ID id, ID id_part);
	void emitterGetTexture(ID id, ID id_part, char* tex);

	void emitterSetTextureTrack(ID id, ID id_part, const char* tex);
	void emitterSetTextureTrackID(ID id, ID id_part, ID tex);
	ID emitterGetTextureTrackID(ID id, ID id_part);
	void emitterGetTextureTrack(ID id, ID id_part, char* tex);

	void emitterSetName(ID id, ID id_part, const char* name);
	void emitterGetName(ID id, ID id_part, char* name);

	int emitterGetTrackCount(ID id, ID id_part);
	int emitterGetTrackPos(ID id, ID id_part, float3** arr, int count);

	ID effectInstanceByID(ID id);
	ID effectInstanceByName(const char* name);
	
	ID effectGetByName(const char* name);
	ID effectAdd(const char* name);
	int effectGetCount();
	ID effectGetIdOfKey(ID key);

	void effectDelete(ID id);
	void effectSetName(ID id, const char* name);
	void effectGetName(ID id, char* name);

	void effectCompute(ID id);
	void effectComputeLighting(ID id);
	void effectRender(ID id, DWORD timeDelta);

	void effectComputeAll();
	void effectComputeLightingAll();
	void effectRenderAll(DWORD timeDelta);

	bool effectGetEnable(ID id);
	void effectSetEnable(ID id, bool isenable);

	void effectPlayByID(ID id, const float3* pos, const float3* dir);
	void effectPlayByName(const char* name, const float3* pos, const float3* dir);

	bool effectGetAlife(ID id);
	void effectSetAlife(ID id, bool alife);

	void effectSetPos(ID id, const float3* pos);
	void effectSetDir(ID id, const float3* dir);
	void effectSetRot(ID id, const float3* rot);
	void effectSetRot(ID id, const SMQuaternion & rot);

	void effectGetPos(ID id, float3* pos);
	void effectGetDir(ID id, float3* dir);
	void effectGetRot(ID id, float3* rot);

	bool effectVisibleCom(ID id, const IXFrustum* frustum, const float3* view);
	void effectVisibleComAll(const IXFrustum* frustum, const float3* view);
	bool effectGetVisible(ID id);
	float effectGetDistToView(ID id);

protected:

	struct CPool
	{
		CPool();

		Array<ID> arr;
		ID ideff;
	};

	ID effectCopyName(const char* name);
	ID effectCopyID(ID id);
	void effectDel(ID id);

	ID addEffect(CEffect* obj);
	ID poolAdd(ID ideff);
	void poolDelete(ID id);
	void poolExtend(ID id);
	ID poolGet(ID id);

	Array<CEffect*> ArrKey;	//массив всех элементов по порядку
	Array<CEffect*> ArrID;	//массив всех элементов, основанный на id
	Array<CPool*> Pools;
	Array<ID> ArrSort;
	int ArrSortSizeCurr;	//текущий размер массива ArrSort
};

#endif