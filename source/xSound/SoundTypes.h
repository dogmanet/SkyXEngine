/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDTYPES_H
#define __SOUNDTYPES_H

#include <xcommon/IXSoundSystem.h>
#include <xcommon/IXAudioCodec.h>
#include <common/math.h>

#include <mital.h>

//##########################################################################

struct CStreamData;

class CSoundBase;
class CSoundLoader;
class CSoundEmitter;
class CSoundPlayer;
class CSoundLayer;
class CSoundSystem;

//##########################################################################

//! состояния воспроизведения звукового объекта
enum SOUND_STATE
{
	SOUND_STATE_STOP,		//!< остановлен
	SOUND_STATE_PAUSE,	//!< приостановлен
	SOUND_STATE_PLAY,		//!< проигрывается
};

//##########################################################################

enum SND_QUEUE_MSG_TYPE
{
	SND_QUEUE_MSG_TYPE_SND_NEW,
	SND_QUEUE_MSG_TYPE_SND_DELETE,
	SND_QUEUE_MSG_TYPE_SND_PLAY,
	SND_QUEUE_MSG_TYPE_SND_RESUME,
	SND_QUEUE_MSG_TYPE_SND_LOOP,
	SND_QUEUE_MSG_TYPE_SND_SPACE,
	SND_QUEUE_MSG_TYPE_SND_PAN,
	SND_QUEUE_MSG_TYPE_SND_VOLUME,
	SND_QUEUE_MSG_TYPE_SND_POS,
	SND_QUEUE_MSG_TYPE_SND_DIST,
	SND_QUEUE_MSG_TYPE_SND_TIME,
	SND_QUEUE_MSG_TYPE_LAYER_NEW,
	SND_QUEUE_MSG_TYPE_LAYER_DELETE,
	SND_QUEUE_MSG_TYPE_LAYER_PLAY
};

struct SndQueueMsg
{
	SND_QUEUE_MSG_TYPE type;
	union
	{
		bool b;
		float f;
		float vector[3];
		SOUND_SPACE space;
		SOUND_LOOP loop;
		SOUND_STATE state;
	} arg;

	CSoundEmitter *pEmitter = NULL;
	CSoundPlayer *pPlayer = NULL;
	CSoundLayer *pLayer = NULL;

	CSoundLayer *pOwner = NULL;
};

#endif
