/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDEMITTER_H
#define __SOUNDEMITTER_H

#ifdef _MSC_VER
#pragma warning(disable : 4250)
#endif

#include <xcommon/IXSoundSystem.h>
#include "SoundBase.h"
#include <mital.h>

#include "SoundTypes.h"
#include "SoundSystem.h"

//##########################################################################

class CSoundEmitter : public CSoundBase, public virtual IXSoundEmitter
{
public:
	SX_ALIGNED_OP_MEM

	~CSoundEmitter();

	//########################################################################

	bool canInstance() const override;

	CSoundBase* newInstance() override;

	void update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp) override;

	//########################################################################

	void XMETHODCALLTYPE play() override;

	//########################################################################

	//! продолжить проигрывание инстансов, если они проигрывались
	void resume();

	//! остановка проигрывания
	void pause();

private:
	virtual void XMETHODCALLTYPE FinalRelease() override;

protected:

	friend CSoundLayer;
	friend CSoundSystem;

	bool create(const char* szName, CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget, SOUND_SPACE space);

	//**********************************************************************
	// функции сообщений

	void _play();
	void _resume();
	void _pause();
	void _setSpace(SOUND_SPACE space);
	void _setVolume(float fVolume);
	void _setPan(float fPan);
	void _setWorldPos(const float3 &vPos);
	void _setDistance(float fDist);

	//#########################################################################

	//! инстанс звукового обьекта
	struct Instance
	{
		Instance() = default;
		Instance(IAudioBuffer *pBuffer) { pAB = pBuffer; }
		~Instance() { mem_release(pAB); }
		IAudioBuffer *pAB = NULL;
		bool isPlaying = false;
	};

	//#########################################################################

	//! массив инстансов
	Array<Instance*> m_aInstances;

	//! параметры слушателя
	float3 m_vListenerPos, m_vListenerDir, m_vListenerUp;
};

#endif
