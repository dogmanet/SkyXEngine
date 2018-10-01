
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sxparticles.h"
#include "emitter.h"
#include "effect.h"
#include "pe_data.h"

#define SXPARTICLES_VERSION 1

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

bool ParticlesPhyCollision(const float3 * lastpos, const float3* nextpos, float3* coll_pos, float3* coll_nrm)
{
	return false;
}

g_particles_phy_collision g_fnParticlesPhyCollision = ParticlesPhyCollision;

//##########################################################################

CEffects *g_pEffects = 0;

#define PE_PRECOND(retval) if(!g_pEffects){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API void SPE_SetFunc_ParticlesPhyCollision(g_particles_phy_collision fnFunc)
{
	g_fnParticlesPhyCollision = fnFunc;
}

SX_LIB_API long SPE_0GetVersion()
{
	return SXPARTICLES_VERSION;
}

SX_LIB_API void SPE_Dbg_Set(report_func fnFunc)
{
	g_fnReportf = fnFunc;
}

SX_LIB_API void SPE_0Create(const char *szName, bool isUnic)
{
	if (szName && strlen(szName) > 1)
	{
		if (isUnic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, szName);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name", GEN_MSG_LOCATION);
			}
			else
			{
				pe_data::Init();
				g_pEffects = new CEffects();
			}
		}
		else
		{
			pe_data::Init();
			g_pEffects = new CEffects();
		}
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SPE_RTSetDepth(ID id)
{
	pe_data::rt_id::idDepthScene = id;
}

SX_LIB_API void SPE_AKill()
{
	PE_PRECOND(_VOID);

	mem_delete(g_pEffects);
	mem_release(pe_data::pVertexDeclarationParticles);
}

SX_LIB_API void SPE_OnLostDevice()
{
	PE_PRECOND(_VOID);

	g_pEffects->onLostDevice();
}

SX_LIB_API void SPE_OnResetDevice()
{
	PE_PRECOND(_VOID);

	g_pEffects->onResetDevice();
}

//**************************************************************************

SX_LIB_API void SPE_EffectLoad(const char *szPath)
{
	PE_PRECOND(_VOID);

	g_pEffects->load(szPath);
}

SX_LIB_API void SPE_EffectSave(const char *szPath)
{
	PE_PRECOND(_VOID);

	g_pEffects->save(szPath);
}

SX_LIB_API void SPE_EffectsClear()
{
	PE_PRECOND(_VOID);

	g_pEffects->clear();
}

SX_LIB_API ID SPE_EffectInstanceByName(const char *szName)
{
	PE_PRECOND(-1);

	return g_pEffects->effectInstanceByName(szName);
}

SX_LIB_API ID SPE_EffectInstanceByID(ID id)
{
	PE_PRECOND(-1);

	return g_pEffects->effectInstanceByID(id);
}

SX_LIB_API ID SPE_EffectGetByName(const char *szName)
{
	PE_PRECOND(-1);

	return g_pEffects->effectGetByName(szName);
}

SX_LIB_API ID SPE_EffectAdd(const char *szName)
{
	PE_PRECOND(-1);

	return g_pEffects->effectAdd(szName);
}

SX_LIB_API int SPE_EffectGetCount()
{
	PE_PRECOND(0);

	return g_pEffects->effectGetCount();
}

SX_LIB_API ID SPE_EffectGetIdOfKey(int iKey)
{
	PE_PRECOND(-1);

	return g_pEffects->effectGetIdOfKey(iKey);
}

SX_LIB_API void SPE_EffectDelete(ID id)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectDelete(id);
}

SX_LIB_API void SPE_EffectSetName(ID id, const char *szName)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectSetName(id, szName);
}

SX_LIB_API void SPE_EffectGetName(ID id, char *szName)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectGetName(id, szName);
}


SX_LIB_API void SPE_EffectCompute(ID id)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectCompute(id);
}

SX_LIB_API void SPE_EffectComputeLighting(ID id)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectComputeLighting(id);
}

SX_LIB_API void SPE_EffectRender(ID id, DWORD timeDelta)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectRender(id, timeDelta);
}


SX_LIB_API void SPE_EffectComputeAll()
{
	PE_PRECOND(_VOID);

	g_pEffects->effectComputeAll();
}

SX_LIB_API void SPE_EffectComputeLightingAll()
{
	PE_PRECOND(_VOID);

	g_pEffects->effectComputeLightingAll();
}

SX_LIB_API void SPE_EffectRenderAll(DWORD timeDelta)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectRenderAll(timeDelta);
}


SX_LIB_API bool SPE_EffectGetAlife(ID id)
{
	PE_PRECOND(false);

	return g_pEffects->effectGetAlife(id);
}

SX_LIB_API void SPE_EffectSetAlife(ID id, bool isAlife)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectSetAlife(id, isAlife);
}


SX_LIB_API bool SPE_EffectGetEnable(ID id)
{
	PE_PRECOND(false);

	return g_pEffects->effectGetEnable(id);
}

SX_LIB_API void SPE_EffectSetEnable(ID id, bool isEnable)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectSetEnable(id, isEnable);
}

SX_LIB_API void SPE_EffectPlayByID(ID id, const float3 *pPos, const float3 *pDir)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectPlayByID(id, pPos, pDir);
}

SX_LIB_API void SPE_EffectPlayByName(const char *szName, const float3 *pPos, const float3 *pDir)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectPlayByName(szName, pPos, pDir);
}


SX_LIB_API void SPE_EffectSetPos(ID id, const float3 *pPos)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectSetPos(id, pPos);
}

SX_LIB_API void SPE_EffectSetDir(ID id, const float3 *pDir)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectSetDir(id, pDir);
}

SX_LIB_API void SPE_EffectSetRotf(ID id, const float3 *pRot)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectSetRot(id, pRot);
}

SX_LIB_API void SPE_EffectSetRotQ(ID id, const SMQuaternion &qRot)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectSetRot(id, qRot);
}


SX_LIB_API void SPE_EffectGetPos(ID id, float3 *pPos)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectGetPos(id, pPos);
}

SX_LIB_API void SPE_EffectGetDir(ID id, float3 *pDir)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectGetDir(id, pDir);
}

SX_LIB_API void SPE_EffectGetRot(ID id, float3 *pRot)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectGetRot(id, pRot);
}


SX_LIB_API bool SPE_EffectVisibleCom(ID id, const IFrustum *pFrustum, const float3 *pView)
{
	PE_PRECOND(false);

	return g_pEffects->effectVisibleCom(id, pFrustum, pView);
}

SX_LIB_API void SPE_EffectVisibleComAll(const IFrustum *pFrustum, const float3 *pView)
{
	PE_PRECOND(_VOID);

	g_pEffects->effectVisibleComAll(pFrustum, pView);
}

SX_LIB_API bool SPE_EffectGetVisible(ID id)
{
	PE_PRECOND(false);

	return g_pEffects->effectGetVisible(id);
}

SX_LIB_API float SPE_EffectDistToViewGet(ID id)
{
	PE_PRECOND(0);

	return g_pEffects->effectGetDistToView(id);
}

//##########################################################################

SX_LIB_API ID SPE_EmitterAdd(ID id, CParticlesData *pData)
{
	PE_PRECOND(-1);

	return g_pEffects->emitterAdd(id, pData);
}

SX_LIB_API int SPE_EmitterGetSCount(ID id)
{
	PE_PRECOND(0);

	return g_pEffects->emitterGetCount(id);
}

SX_LIB_API void SPE_EmitterDelete(ID id, ID idPart)
{
	PE_PRECOND(_VOID);

	g_pEffects->emitterDelete(id, idPart);
}

SX_LIB_API CParticlesData* SPE_EmitterGetData(ID id, ID idPart)
{
	PE_PRECOND(0);

	return g_pEffects->emitterGetData(id, idPart);
}

SX_LIB_API void SPE_EmitterReInit(ID id, ID idPart, CParticlesData *pData)
{
	PE_PRECOND(_VOID);

	return g_pEffects->emitterReInit(id, idPart, pData);
}

SX_LIB_API void SPE_EmitterSetCount(ID id, ID idPart, int iCount)
{
	PE_PRECOND(_VOID);

	return g_pEffects->emitterSetCount(id, idPart, iCount);
}

SX_LIB_API int SPE_EmitterGetCount(ID id, ID idPart)
{
	PE_PRECOND(0);

	return g_pEffects->emitterGetCount(id, idPart);
}

SX_LIB_API int SPE_EmitterGetCountLife(ID id, ID idPart)
{
	PE_PRECOND(0);

	return g_pEffects->emitterGetCountLife(id, idPart);
}

SX_LIB_API void SPE_EmitterSetEnable(ID id, ID idPart, bool isEnable)
{
	PE_PRECOND(_VOID);

	g_pEffects->emitterSetEnable(id, idPart, isEnable);
}

SX_LIB_API bool SPE_EmitterGetEnable(ID id, ID idPart)
{
	PE_PRECOND(false);

	return g_pEffects->emitterGetEnable(id, idPart);
}


SX_LIB_API void SPE_EmitterSetTexture(ID id, ID idPart, const char *szTex)
{
	PE_PRECOND(_VOID);

	g_pEffects->emitterSetTexture(id, idPart, szTex);
}

SX_LIB_API void SPE_EmitterSetTextureID(ID id, ID idPart, ID idTex)
{
	PE_PRECOND(_VOID);

	g_pEffects->emitterSetTextureID(id, idPart, idTex);
}

SX_LIB_API ID SPE_EmitterGetTextureID(ID id, ID idPart)
{
	PE_PRECOND(-1);

	return g_pEffects->emitterGetTextureID(id, idPart);
}

SX_LIB_API void SPE_EmitterGetTexture(ID id, ID idPart, char *szTex)
{
	PE_PRECOND(_VOID);

	g_pEffects->emitterGetTexture(id, idPart, szTex);
}


SX_LIB_API void SPE_EmitterSetTextureTrack(ID id, ID idPart, const char *idTex)
{
	PE_PRECOND(_VOID);

	g_pEffects->emitterSetTextureTrack(id, idPart, idTex);
}

SX_LIB_API void SPE_EmitterSetTextureTrackID(ID id, ID idPart, ID idTex)
{
	PE_PRECOND(_VOID);

	g_pEffects->emitterSetTextureTrackID(id, idPart, idTex);
}

SX_LIB_API ID SPE_EmitterGetTextureTrackID(ID id, ID idPart)
{
	PE_PRECOND(-1);

	return g_pEffects->emitterGetTextureTrackID(id, idPart);
}

SX_LIB_API void SPE_EmitterGetTextureTrack(ID id, ID idPart, char *szTex)
{
	PE_PRECOND(_VOID);

	g_pEffects->emitterGetTextureTrack(id, idPart, szTex);
}



SX_LIB_API void SPE_EmitterSetName(ID id, ID idPart, const char *szName)
{
	PE_PRECOND(_VOID);

	g_pEffects->emitterSetName(id, idPart, szName);
}

SX_LIB_API void SPE_EmitterGetName(ID id, ID idPart, char *szName)
{
	PE_PRECOND(_VOID);

	g_pEffects->emitterGetName(id, idPart, szName);
}


SX_LIB_API int SPE_EmitterGetTrackCount(ID id, ID idPart)
{
	PE_PRECOND(0);

	return g_pEffects->emitterGetTrackCount(id, idPart);
}

SX_LIB_API int SPE_EmitterGetTrackPos(ID id, ID idPart, float3** ppArr, int iCount)
{
	PE_PRECOND(0);

	return g_pEffects->emitterGetTrackPos(id, idPart, ppArr, iCount);
}