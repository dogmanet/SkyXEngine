#include "ParticleEffectEmitter.h"
#include "ParticleSystem.h"

CParticleEffectEmitter::CParticleEffectEmitter(CParticleSystem *pSystem):
	m_pSystem(pSystem)
{
}

void XMETHODCALLTYPE CParticleEffectEmitter::FinalRelease()
{
	m_pSystem->onEmitterReleased(this);
}
