#ifndef __PARTICLE_EFFECT_EMITTER_VELOCITY_LIFETIME_DATA_H
#define __PARTICLE_EFFECT_EMITTER_VELOCITY_LIFETIME_DATA_H

#include <xcommon/particles/IXParticleSystem.h>
#include <common/MinMaxCurve.h>


class CParticleEffectEmitterVelocityLifetimeData final: public IXParticleEffectEmitterVelocityLifetimeData
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

	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getLinearXCurve, &m_curveLinearX);
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getLinearYCurve, &m_curveLinearY);
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getLinearZCurve, &m_curveLinearZ);

	XMETHOD_GETSET_IMPL(SimulationSpace, XPARTICLE_SIMULATION_SPACE, simulationSpace, m_simulationSpace);

	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getOrbitalXCurve, &m_curveOrbitalX);
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getOrbitalYCurve, &m_curveOrbitalY);
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getOrbitalZCurve, &m_curveOrbitalZ);
	
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getOffsetXCurve, &m_curveOffsetX);
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getOffsetYCurve, &m_curveOffsetY);
	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getOffsetZCurve, &m_curveOffsetZ);

	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getRadialCurve, &m_curveRadial);

	XMETHOD_2CONST_IMPL(IXMinMaxCurve*, getSpeedModifierCurve, &m_curveSpeedModifier);

private:
	bool m_isEnabled = false;

	CMinMaxCurve m_curveLinearX;
	CMinMaxCurve m_curveLinearY;
	CMinMaxCurve m_curveLinearZ;

	XPARTICLE_SIMULATION_SPACE m_simulationSpace = XPSS_LOCAL;

	CMinMaxCurve m_curveOrbitalX;
	CMinMaxCurve m_curveOrbitalY;
	CMinMaxCurve m_curveOrbitalZ;

	CMinMaxCurve m_curveOffsetX;
	CMinMaxCurve m_curveOffsetY;
	CMinMaxCurve m_curveOffsetZ;

	CMinMaxCurve m_curveRadial;

	CMinMaxCurve m_curveSpeedModifier;

};

#endif
