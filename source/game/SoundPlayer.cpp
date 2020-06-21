
#include "SoundPlayer.h"

/*! \skydocent sound_player
	
*/

BEGIN_PROPTABLE(CSoundPlayer)

DEFINE_FIELD_STRINGFN(m_szPathSound, 0, "file", "Sound file", setSound, EDITOR_SOUND)

DEFINE_FIELD_FLOAT(m_fDist, 0, "distance", "Hearing distance", EDITOR_TEXTFIELD)

END_PROPTABLE()

REGISTER_ENTITY(CSoundPlayer, sound_player);

//##########################################################################

CSoundPlayer::CSoundPlayer(CEntityManager *pMgr):
	BaseClass(pMgr),
	m_pPlayer(NULL)
{
	
}

void CSoundPlayer::setSound(const char *szSound)
{
	mem_release(m_pPlayer);
	IXSoundSystem *pSound = (IXSoundSystem*)(Core_GetIXCore()->getPluginManager()->getInterface(IXSOUNDSYSTEM_GUID));
	IXSoundLayer *pMasterLayer = pSound->findLayer("master");
	m_pPlayer = pMasterLayer->newSoundPlayer(szSound, SOUND_DTYPE_3D);
	if (m_pPlayer)
	{
		m_pPlayer->setLoop(SOUND_LOOP_SIMPLE);
		m_pPlayer->play();
	}
}

void CSoundPlayer::onSync()
{
	if (!m_pPlayer)
		return;

	float3 vPos = getPos();
	m_pPlayer->setWorldPos(vPos);
}