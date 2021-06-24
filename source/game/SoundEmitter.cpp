
#include "SoundEmitter.h"

//##########################################################################

/*! \skydocent sound_emitter
	
*/

//##########################################################################

BEGIN_PROPTABLE(CSoundEmitter)

	//! Звуковой файл
	DEFINE_FIELD_STRINGFN(m_szPathSound, 0, "file", "Sound file", setSound, EDITOR_SOUND)

	//! Громкость
	DEFINE_FIELD_FLOATFN(m_fVolume, 0, "volume", "Volume", setVolume, EDITOR_TEXTFIELD)

	//! Дистанция слышимости
	DEFINE_FIELD_FLOATFN(m_fDist, 0, "distance", "Hearing distance", setDistance, EDITOR_TEXTFIELD)
	
	//! Включить
	DEFINE_INPUT(play, "play", "Play", PDF_NONE)

	//! При включении
	DEFINE_OUTPUT(m_onTurnOn, "OnTurnOn", "On Turn On")
//	//! При выключении
//	DEFINE_OUTPUT(m_onTurnOff, "OnTurnOff", "On Turn Off")

	//! Тип фонового звука
	DEFINE_FLAG(SND_EMITTER_TYPE_AMBIENT, "Ambient")

END_PROPTABLE()

//**************************************************************************

REGISTER_ENTITY(CSoundEmitter, sound_emitter, REC_ICON("dev_sound_emitter"));

//##########################################################################

CSoundEmitter::~CSoundEmitter()
{
	mem_release(m_pEmitter);
}

//##########################################################################

void CSoundEmitter::setSound(const char *szSound)
{
	if(!m_pEmitter || fstrcmp(m_szPathSound, szSound) != 0)
	{
		_setStrVal(&m_szPathSound, szSound);

		mem_release(m_pEmitter);
		IXSoundSystem *pSound = (IXSoundSystem*)(Core_GetIXCore()->getPluginManager()->getInterface(IXSOUNDSYSTEM_GUID));
		IXSoundLayer *pGameLayer = pSound->findLayer("xGame");

		SOUND_SPACE space = (getFlags() & SND_EMITTER_TYPE_AMBIENT ? SOUND_SPACE_2D : SOUND_SPACE_3D);
		m_pEmitter = pGameLayer->newSoundEmitter(szSound, space);

		if(m_pEmitter)
		{
			m_pEmitter->setVolume(m_fVolume);
			m_pEmitter->setWorldPos(getPos());
			m_pEmitter->setDistance(m_fDist);
		}
	}
}

//##########################################################################

void CSoundEmitter::setPos(const float3 &pos)
{
	BaseClass::setPos(pos);

	SAFE_CALL(m_pEmitter, setWorldPos, pos);
}

void CSoundEmitter::setVolume(float fVolume)
{
	m_fVolume = fVolume;

	SAFE_CALL(m_pEmitter, setVolume, fVolume);
}
float CSoundEmitter::getVolume()
{
	return(m_fVolume);
}

void CSoundEmitter::setDistance(float fDistance)
{
	m_fDist = fDistance;

	SAFE_CALL(m_pEmitter, setDistance, fDistance);
}
float CSoundEmitter::getDistance()
{
	return(m_fDist);
}

//##########################################################################

void CSoundEmitter::play(inputdata_t * pInputdata)
{
	SAFE_CALL(m_pEmitter, play);

	FIRE_OUTPUT(m_onTurnOn, pInputdata->pInflictor);
}

//##########################################################################

void CSoundEmitter::updateFlags()
{
	BaseClass::updateFlags();

	if(!m_pEmitter)
		return;

	if(getFlags() & SND_EMITTER_TYPE_AMBIENT)
	{
		m_pEmitter->setSpace(SOUND_SPACE_2D);
	}
	else
	{
		m_pEmitter->setSpace(SOUND_SPACE_3D);
	}
}
