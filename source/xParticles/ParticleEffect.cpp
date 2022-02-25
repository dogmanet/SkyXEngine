#include "ParticleEffect.h"

CParticleEffect::CParticleEffect()
{

}

UINT XMETHODCALLTYPE CParticleEffect::getEmitterCount()
{
	return(m_aEmitters.size());
}
void XMETHODCALLTYPE CParticleEffect::setEmitterCount(UINT uCount)
{
	m_aEmitters.resizeFast(uCount);
}
IXParticleEffectEmitter* XMETHODCALLTYPE CParticleEffect::getEmitterAt(UINT uIdx)
{
	assert(m_aEmitters.size() > uIdx);

	if(m_aEmitters.size() > uIdx)
	{
		return(&m_aEmitters[uIdx]);
	}

	return(NULL);
}

bool XMETHODCALLTYPE CParticleEffect::save()
{
}
