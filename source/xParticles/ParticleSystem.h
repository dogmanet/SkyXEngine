#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM_H

#include <xcommon/particles/IXParticleSystem.h>

class CParticleSystem final: public IXUnknownImplementation<IXParticleSystem>
{
public:
	CParticleSystem(IFileSystem *pFileSystem);

	XIMPLEMENT_VERSION(IXPARTICLESYSTEM_VERSION);

	bool XMETHODCALLTYPE newEffect(const char *szName, IXParticleEffect **ppOut) override;
	bool XMETHODCALLTYPE getEffect(const char *szName, IXParticleEffect **ppOut) override;

	void XMETHODCALLTYPE newEffectPlayer(IXParticleEffect *pEffect, IXParticlePlayer **ppOut) override;

protected:
	IFileSystem *m_pFileSystem;
};

#endif
