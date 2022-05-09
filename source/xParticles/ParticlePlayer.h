#ifndef __PARTICLEPLAYER_H
#define __PARTICLEPLAYER_H

#include <xcommon/particles/IXParticleSystem.h>
#include "ParticleEffect.h"

class CParticlePlayer;
class CParticlePlayerEmitter final
{
public:
	~CParticlePlayerEmitter();

	void setPlayer(CParticlePlayer *pPlayer);
	void setData(CParticleEffectEmitter *pData);
	//bool isEmitting();

	//void reset();

	void update(float fDt);

	float getTime();

	void restart();
	void clear();

private:
	void emit(float fCount);
	void emitOne(UINT uCountInGen = 1, UINT uIdInGen = 0);

	float evalCurve(const CMinMaxCurve &curve);

private:
	CParticleEffectEmitter *m_pData = NULL;
	CParticlePlayer *m_pPlayer = NULL;

	float m_fEmitFrac = 0.0f;

	float m_fTime = 0.0f;

	struct Particle
	{
		float fRemainingLifetime;
		float3_t vPos;
		float3_t vSpeed;
		float4_t vColor;
	};
	Array<Particle> m_aParticles;

	Array<CParticleBurst> m_aBursts;
};

//#############################################################################

class CParticlePlayer final: public IXUnknownImplementation<IXParticlePlayer>
{
public:
	CParticlePlayer(CParticleEffect *pEffect);
	~CParticlePlayer();

	void XMETHODCALLTYPE play() override;
	void XMETHODCALLTYPE pause() override;
	void XMETHODCALLTYPE stop(bool bClear = false) override;
	void XMETHODCALLTYPE clear() override;

	void XMETHODCALLTYPE simulate(float fTime, bool bRestart = false) override;

	bool XMETHODCALLTYPE isEmitting() override;
	bool XMETHODCALLTYPE isPaused() override;
	bool XMETHODCALLTYPE isPlaying() override;
	bool XMETHODCALLTYPE isStopped() override;
	bool XMETHODCALLTYPE isAlive() override;

	UINT XMETHODCALLTYPE getParticleCount() override;
	float XMETHODCALLTYPE getTime() override;

	float3_t XMETHODCALLTYPE getPos() override;
	void XMETHODCALLTYPE setPos(const float3_t &vPos) override;

	SMQuaternion XMETHODCALLTYPE getOrient() override;
	void XMETHODCALLTYPE setOrient(const SMQuaternion &qRot) override;

	void update(float fDelta);

	float getDeltaPos();

private:
	//void simulateEmitter(UINT i, float fTime)

private:
	CParticleEffect *m_pEffect;

	Array<CParticlePlayerEmitter> m_aEmitters;
};

#endif
