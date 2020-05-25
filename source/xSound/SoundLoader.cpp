
#include "SoundLoader.h"

#include "SoundSystem.h"

//##########################################################################

bool CSoundLoader::can() const
{
	return (bool)m_pCodecTarget;
}

void CSoundLoader::init(CSoundSystem *pSoundSystem)
{
	m_pSoundSystem = pSoundSystem;
}

bool CSoundLoader::load(const char *szPath, AudioRawDesc *pDesc)
{
	if (!m_pSoundSystem)
		return false;

	m_pCodecTarget = m_pSoundSystem->getCodecTarget(szPath);
	if (!m_pCodecTarget)
		return false;

	m_pCodecTarget->getDesc(pDesc);

	return true;
}

size_t CSoundLoader::getPCM(void **ppData, uint32_t uLen, int32_t iPos)
{
	if (!can())
		return 0;

	if (iPos < 0)
		iPos = getPos();

	return m_pCodecTarget->decode(iPos, uLen, ppData);
}

void CSoundLoader::setPos(uint32_t uPos)
{
	if (!can())
		return;

	m_pCodecTarget->setPos(uPos);
}

int32_t CSoundLoader::getPos() const
{
	if (!can())
		return 0;

	return m_pCodecTarget->getPos();
}
