#ifndef __PARTICLE_EFFECT_EMITTER_EMISSION_DATA_H
#define __PARTICLE_EFFECT_EMITTER_EMISSION_DATA_H

#include <xcommon/particles/IXParticleSystem.h>
#include <common/MinMaxCurve.h>
#include "ParticleBurst.h"


class CParticleEffectEmitterEmissionData final: public IXParticleEffectEmitterEmissionData
{
	friend class CParticlePlayerEmitter;

public:
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getRatePerSecondCurve, &m_curveRatePerSecond);
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getRatePerMeterCurve, &m_curveRatePerMeter);

	void XMETHODCALLTYPE setBurstsCount(UINT uCount) override
	{
		m_aBursts.resize(uCount);
	}
	UINT XMETHODCALLTYPE getBurstsCount() const
	{
		return(m_aBursts.size());
	}

	IXParticleBurst* XMETHODCALLTYPE getBurstAt(UINT uIndex) override
	{
		assert(uIndex < m_aBursts.size());

		return(&m_aBursts[uIndex]);
	}
	
	const IXParticleBurst* XMETHODCALLTYPE getBurstAt(UINT uIndex) const override
	{
		assert(uIndex < m_aBursts.size());

		return(&m_aBursts[uIndex]);
	}

private:
	CMinMaxCurve m_curveRatePerSecond;
	CMinMaxCurve m_curveRatePerMeter;

	Array<CParticleBurst> m_aBursts;
};

#endif
