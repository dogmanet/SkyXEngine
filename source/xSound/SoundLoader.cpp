
#include "SoundLoader.h"

#include "SoundSystem.h"

//##########################################################################

bool CSoundLoader::can() const
{
	return(m_pCodecTarget != NULL);
}

void CSoundLoader::init(CSoundSystem *pSoundSystem)
{
	m_pSoundSystem = pSoundSystem;
}

bool CSoundLoader::load(const char *szPath, AudioRawDesc *pDesc)
{
	if(!m_pSoundSystem)
		return false;

	m_pCodecTarget = m_pSoundSystem->getCodecTarget(szPath);
	if(!m_pCodecTarget)
		return false;

	m_pCodecTarget->getDesc(pDesc);

	return true;
}

size_t CSoundLoader::getPCM(void **ppData, size_t uLen, int64_t iPos)
{
	if(!can())
		return 0;

	if(iPos < 0)
		iPos = (int64_t)getPos();

	return m_pCodecTarget->decode(iPos, uLen, ppData);
}

void CSoundLoader::setPos(size_t uPos)
{
	if(!can())
		return;

	m_pCodecTarget->setPos(uPos);
}

size_t CSoundLoader::getPos() const
{
	if(!can())
		return 0;

	return m_pCodecTarget->getPos();
}
