#ifndef __PARTICLE_EFFECT_EMITTER_LIMIT_VELOCITY_LIFETIME_DATA_H
#define __PARTICLE_EFFECT_EMITTER_LIMIT_VELOCITY_LIFETIME_DATA_H

#include <xcommon/particles/IXParticleSystem.h>
#include <common/MinMaxCurve.h>


class CParticleEffectEmitterLimitVelocityLifetimeData final: public IXParticleEffectEmitterLimitVelocityLifetimeData
{
public:
	bool XMETHODCALLTYPE isEnabled() const override
	{
		return(m_isEnabled);
	}
	void XMETHODCALLTYPE enable(bool yesNo) override
	{
		m_isEnabled = yesNo;
	}


	XMETHOD_GETSET_IMPL(SeparateAxes, bool, yesNo, m_bSeparateAxes);
	XMETHOD_GETSET_IMPL(SimulationSpace, XPARTICLE_SIMULATION_SPACE, simulationSpace, m_simulationSpace);
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getSpeedCurve, &m_curveSpeed);
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getDampenCurve, &m_curveDampen);
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getDragCurve, &m_curveDrag);
	XMETHOD_GETSET_IMPL(MultiplyBySize, bool, yesNo, m_bMultiplyBySize);
	XMETHOD_GETSET_IMPL(MultiplyByVelocity, bool, yesNo, m_bMultiplyByVelocity);

private:
	bool m_isEnabled = false;

	bool m_bSeparateAxes = false;
	XPARTICLE_SIMULATION_SPACE m_simulationSpace = XPSS_LOCAL;

	CMinMaxCurve m_curveSpeed;
	CMinMaxCurve m_curveDampen;
	CMinMaxCurve m_curveDrag;

	bool m_bMultiplyBySize = false;
	bool m_bMultiplyByVelocity = false;
};

#endif
