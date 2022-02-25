#ifndef __PARTICLEPLAYER_H
#define __PARTICLEPLAYER_H

#include <xcommon/particles/IXParticleSystem.h>
#include "ParticleEffect.h"

class CParticlePlayer final: public IXUnknownImplementation<IXParticlePlayer>
{
public:
	CParticlePlayer(CParticleEffect *pEffect);

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

protected:
	CParticleEffect *m_pEffect;

};

#endif
