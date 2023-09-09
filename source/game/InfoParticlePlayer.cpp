#include "InfoParticlePlayer.h"

/*! \skydocent info_particle_player
Камера
*/

BEGIN_PROPTABLE(CInfoParticlePlayer)
	//! Файл эффекта
	DEFINE_FIELD_STRINGFN(m_szEffect, 0, "file", "Effect file", setEffect, EDITOR_EFFECT)
END_PROPTABLE()

REGISTER_ENTITY(CInfoParticlePlayer, info_particle_player);

void CInfoParticlePlayer::setEffect(const char *szEffectFile)
{
	_setStrVal(&m_szEffect, szEffectFile);
}
