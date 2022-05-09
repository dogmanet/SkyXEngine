#include "ParticlePlayer.h"

CParticlePlayer::CParticlePlayer(CParticleEffect *pEffect):
	m_pEffect(pEffect)
{
	add_ref(pEffect);
}
CParticlePlayer::~CParticlePlayer()
{
	mem_release(m_pEffect);
}

void XMETHODCALLTYPE CParticlePlayer::play()
{
}
void XMETHODCALLTYPE CParticlePlayer::pause()
{
}
void XMETHODCALLTYPE CParticlePlayer::stop(bool bClear)
{
}
void XMETHODCALLTYPE CParticlePlayer::clear()
{
}

void XMETHODCALLTYPE CParticlePlayer::simulate(float fTime, bool bRestart)
{
}

bool XMETHODCALLTYPE CParticlePlayer::isEmitting()
{
	assert(!"Not implemented");
	return(false);
}
bool XMETHODCALLTYPE CParticlePlayer::isPaused()
{
	assert(!"Not implemented");
	return(false);
}
bool XMETHODCALLTYPE CParticlePlayer::isPlaying()
{
	assert(!"Not implemented");
	return(false);
}
bool XMETHODCALLTYPE CParticlePlayer::isStopped()
{
	assert(!"Not implemented");
	return(false);
}
bool XMETHODCALLTYPE CParticlePlayer::isAlive()
{
	assert(!"Not implemented");
	return(false);
}

UINT XMETHODCALLTYPE CParticlePlayer::getParticleCount()
{
	assert(!"Not implemented");
	return(0);
}
float XMETHODCALLTYPE CParticlePlayer::getTime()
{
	assert(!"Not implemented");
	return(0.0f);
}

float3_t XMETHODCALLTYPE CParticlePlayer::getPos()
{
	assert(!"Not implemented");
	return(0.0f);
}
void XMETHODCALLTYPE CParticlePlayer::setPos(const float3_t &vPos)
{
}

SMQuaternion XMETHODCALLTYPE CParticlePlayer::getOrient()
{
	assert(!"Not implemented");
	return(SMQuaternion());
}
void XMETHODCALLTYPE CParticlePlayer::setOrient(const SMQuaternion &qRot)
{
}

void CParticlePlayer::update(float fDelta)
{
	UINT uEmCount = m_pEffect->getEmitterCount();
	if(uEmCount != m_aEmitters.size())
	{
		m_aEmitters.resize(uEmCount);
	}

	for(UINT i = 0; i < uEmCount; ++i)
	{
		m_aEmitters[i].setData(m_pEffect->getEmitterAtInternal(i));
	}

	for(UINT i = 0; i < uEmCount; ++i)
	{
		m_aEmitters[i].update(fDelta);
	}
}

float CParticlePlayer::getDeltaPos()
{
	assert(!"Not implemented");
	return(0.0f);
}

//#############################################################################

CParticlePlayerEmitter::~CParticlePlayerEmitter()
{
	mem_release(m_pData);
}

void CParticlePlayerEmitter::setPlayer(CParticlePlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

void CParticlePlayerEmitter::setData(CParticleEffectEmitter *pData)
{
	if(m_pData != pData)
	{
		mem_release(m_pData);
		m_pData = pData;
		add_ref(m_pData);
	}
}

void CParticlePlayerEmitter::update(float fDt)
{
	m_fTime += fDt;
	if(m_fTime < m_pData->m_dataGeneric.m_fStartDelay)
	{
		return;
	}

	m_aParticles.reserve(m_pData->m_dataGeneric.getMaxParticles());

	// collect dead particles
	fora(i, m_aParticles)
	{
		Particle &p = m_aParticles[i];
		p.fRemainingLifetime -= fDt;

		if(p.fRemainingLifetime <= 0.0f)
		{
			p = m_aParticles[il - 1];
			m_aParticles.erase(il - 1);
			--i; --il;
		}
	}

	// emission controlled by emission module
	CParticleEffectEmitterEmissionData &dataEmission = m_pData->m_dataEmission;
	float fEmitCount = evalCurve(dataEmission.m_curveRatePerSecond) * fDt;
	float fDeltaPos = m_pPlayer->getDeltaPos();
	if(fDeltaPos > SM_EPSILON)
	{
		fEmitCount += evalCurve(dataEmission.m_curveRatePerMeter) * fDeltaPos;
	}

	fora(i, m_aBursts)
	{
		CParticleBurst &burst = m_aBursts[i];
		burst.m_fTime -= fDt;
		if(burst.m_fTime < 0)
		{
			if(randf(0.0f, 1.0f) <= burst.m_fProbability)
			{
				fEmitCount += evalCurve(burst.m_curveCount);
			}

			if(burst.m_uCycles > 1)
			{
				--burst.m_uCycles;
				burst.m_fTime += evalCurve(burst.m_curveInterval);
			}
			else
			{
				burst = m_aBursts[il - 1];
				m_aBursts.erase(il - 1);
				--i; --il;
			}
		}
	}
		
	emit(fEmitCount);
}

float CParticlePlayerEmitter::getTime()
{
	return(m_fTime - m_pData->m_dataGeneric.m_fStartDelay);
}

void CParticlePlayerEmitter::restart()
{
	m_fTime = 0.0f;
	m_fEmitFrac = 0.0f;

	m_aBursts = m_pData->m_dataEmission.m_aBursts;
}

void CParticlePlayerEmitter::clear()
{
	m_aParticles.clearFast();
}

void CParticlePlayerEmitter::emit(float fCount)
{
	m_fEmitFrac += fCount;
	if(m_fEmitFrac >= 1.0f)
	{
		UINT uCount = (UINT)m_fEmitFrac;
		m_fEmitFrac -= (float)uCount;

		for(UINT i = 0; i < uCount; ++i)
		{
			emitOne(uCount, i);
		}
	}
}

void CParticlePlayerEmitter::emitOne(UINT uCountInGen, UINT uIdInGen)
{
	if(m_aParticles.size() == m_aParticles.GetAllocSize())
	{
		// limit is reached
		return;
	}

	CParticleEffectEmitterGenericData &dataGeneric = m_pData->m_dataGeneric;

	Particle &newParticle = m_aParticles[m_aParticles.size()];
	newParticle.fRemainingLifetime = evalCurve(dataGeneric.m_curveStartLifetime);
	newParticle.vColor = dataGeneric.m_vStartColor;

	float3 vBasePos, vBaseDir;
	m_pData->m_dataShape.evaluate(getTime() / m_pData->m_dataGeneric.m_fDuration, &vBasePos, &vBaseDir, uCountInGen, uIdInGen);
	if(dataGeneric.m_simulationSpace == XPSS_WORLD)
	{
		// add player pos and orient
		vBasePos = m_pPlayer->getOrient() * vBasePos + m_pPlayer->getPos();
		vBaseDir = m_pPlayer->getOrient() * vBaseDir;
	}
	newParticle.vPos = vBasePos;
	newParticle.vSpeed = (float3)(vBaseDir * evalCurve(dataGeneric.m_curveStartSpeed));

	// m_fFlipRotation          //! Makes some particles spin in the opposite direction
	// m_curveStartRotationX    //! The initial rotation of particles around the x-axis when emitted.
	// m_bStartRotationSeparate //! A flag to enable 3D particle rotation.

	// set other properties
	//+ speed
	// size
	// rotation
	// spin
	// space
	// pos

	// shape mod:
	// pos
	// velocity dir
	// rotation
}

float CParticlePlayerEmitter::evalCurve(const CMinMaxCurve &curve)
{
	float fLerpFactor = 1.0f;
	if(curve.getMode() == XMCM_TWO_CONSTANTS || curve.getMode() == XMCM_TWO_CURVES)
	{
		fLerpFactor = randf(0.0f, 1.0f);
	}
	return(curve.evaluate(getTime() / m_pData->m_dataGeneric.m_fDuration, fLerpFactor));
}
