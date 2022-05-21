#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM_H

#include <xcommon/particles/IXParticleSystem.h>
#include <xcommon/IXCore.h>
#include "ParticleEffect.h"
#include "ParticlePlayer.h"
#include <common/queue.h>
#include <mtrl/IXMaterialSystem.h>

class CParticleSystem final: public IXUnknownImplementation<IXParticleSystem>
{
public:
	CParticleSystem(IXCore *pCore);

	XIMPLEMENT_VERSION(IXPARTICLESYSTEM_VERSION);

	bool XMETHODCALLTYPE newEffect(const char *szName, IXParticleEffect **ppOut) override;
	bool newEffect(const char *szName, CParticleEffect **ppOut);
	bool XMETHODCALLTYPE getEffect(const char *szName, IXParticleEffect **ppOut) override;

	void XMETHODCALLTYPE newEffectPlayer(IXParticleEffect *pEffect, IXParticlePlayer **ppOut) override;

	void onEffectReleased(const char *szName);
	void onEffectPlayerReleased(CParticlePlayer *pPlayer);
	void onEmitterReleased(CParticleEffectEmitter *pEmitter);

	bool saveEffect(CParticleEffect *pEffect);

	void update(float fDelta);
	void sync();

	CParticleEffectEmitter* allocEmitter();


	void setDevice(IGXDevice *pDevice);
	void setMaterialSystem(IXMaterialSystem *pMaterialSystem);

	void newVisData(IXRenderableVisibility **ppVisibility);
	void render(IXRenderableVisibility *pVisibility);

protected:
	IXCore *m_pCore;

	Map<String, CParticleEffect> m_mapEffects;

	struct PlayerQueueItem
	{
		enum
		{
			PQI_ADD,
			PQI_REMOVE
		} cmd;
		CParticlePlayer *pPlayer;
	};
	SpinLock m_slPlayerPool;
	MemAlloc<CParticlePlayer> m_poolPlayers;
	Queue<PlayerQueueItem> m_queuePlayers;
	Array<CParticlePlayer*> m_aPlayers;

	SpinLock m_slEmittersPool;
	MemAlloc<CParticleEffectEmitter> m_poolEmitters;

	XVertexShaderHandler *m_pVertexShaderHandler = NULL;
	IGXDevice *m_pDevice = NULL;
	IXMaterialSystem *m_pMaterialSystem = NULL;
};

#endif
