#ifndef __PARTICLEEFFECTEMITTER_H
#define __PARTICLEEFFECTEMITTER_H

#include <xcommon/particles/IXParticleSystem.h>
#include "ParticleEffectEmitterGenericData.h"
#include "ParticleEffectEmitterEmissionData.h"
#include "ParticleEffectEmitterShapeData.h"
#include "ParticleEffectEmitterVelocityLifetimeData.h"
#include "ParticleEffectEmitterLimitVelocityLifetimeData.h"

class CParticleSystem;
class CParticleEffectEmitter final: public IXUnknownImplementation<IXParticleEffectEmitter>
{
	friend class CParticlePlayerEmitter;
public:
	CParticleEffectEmitter(CParticleSystem *pSystem);

	XMETHOD_2CONST_IMPL(IXParticleEffectEmitterGenericData*, getGenericData, &m_dataGeneric);
	XMETHOD_2CONST_IMPL(IXParticleEffectEmitterEmissionData*, getEmissionData, &m_dataEmission);
	XMETHOD_2CONST_IMPL(IXParticleEffectEmitterShapeData*, getShapeData, &m_dataShape);
	XMETHOD_2CONST_IMPL(IXParticleEffectEmitterVelocityLifetimeData*, getVelocityLifetimeData, &m_dataVelocityLifetime);
	XMETHOD_2CONST_IMPL(IXParticleEffectEmitterLimitVelocityLifetimeData*, getLimitVelocityLifetimeData, &m_dataLimitVelocityLifetime);

private:
	void XMETHODCALLTYPE FinalRelease() override;

private:
	CParticleSystem *m_pSystem;

	CParticleEffectEmitterGenericData m_dataGeneric;
	CParticleEffectEmitterEmissionData m_dataEmission;
	CParticleEffectEmitterShapeData m_dataShape;
	CParticleEffectEmitterVelocityLifetimeData m_dataVelocityLifetime;
	CParticleEffectEmitterLimitVelocityLifetimeData m_dataLimitVelocityLifetime;
};

#endif
