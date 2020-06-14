/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDEMITTER_H
#define __SOUNDEMITTER_H

#include <xcommon/IXSoundSystem.h>
#include "SoundBase.h"
#include <mital.h>

#include "SoundTypes.h"
#include "SoundSystem.h"

//##########################################################################

class CSoundEmitter : public CSoundBase, public virtual IXSoundEmitter
{
public:
	~CSoundEmitter();

	//! создание нового инстанса CSoundEmitter
	CSoundEmitter* newInstance();

	virtual void XMETHODCALLTYPE play() override;

	//! продолжить проигрывание инстансов, если они проигрывались
	void resume();

	//! остановка проигрывания
	void pause();

	void update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp);

protected:

	friend CSoundLayer;

	bool create(CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget, SOUND_DTYPE dtype);

	struct Instance
	{
		Instance() = default;
		Instance(IAudioBuffer *pBuffer) { pAB = pBuffer; }
		~Instance() { mem_release(pAB); }
		IAudioBuffer *pAB = NULL;
		bool isPlaying = false;
	};

	Array<Instance> m_aInstances;
};

#endif
