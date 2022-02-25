#ifndef __PARTICLEEFFECT_H
#define __PARTICLEEFFECT_H

#include <xcommon/particles/IXParticleSystem.h>
#include "ParticleEffectEmitter.h"

class CParticleEffect final: public IXUnknownImplementation<IXParticleEffect>
{
public:
	CParticleEffect();

	UINT XMETHODCALLTYPE getEmitterCount() override;
	void XMETHODCALLTYPE setEmitterCount(UINT uCount) override;
	IXParticleEffectEmitter* XMETHODCALLTYPE getEmitterAt(UINT uIdx) override;

	bool XMETHODCALLTYPE save() override;

private:
	Array<CParticleEffectEmitter> m_aEmitters;
};

#endif
