#ifndef __EFFECTLOADER_H
#define __EFFECTLOADER_H

#include "ParticleEffect.h"

class CEffectLoader final
{
public:
	CEffectLoader();

	bool loadFromFile(const char *szFile, CParticleEffect *pEffect);
	bool saveToFile(const char *szFile, CParticleEffect *pEffect);
};

#endif
