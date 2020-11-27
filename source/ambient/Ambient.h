#ifndef __AMBIENT_H
#define __AMBIENT_H

#include <xcommon/IXAmbient.h>

#include "SkyBox.h"

class CAmbient: public IXUnknownImplementation<IXAmbient>
{
public:
	CAmbient(IXCore *pCore);

	void XMETHODCALLTYPE setSkybox(const char *szTexture) override;

	void XMETHODCALLTYPE enableSkybox(bool bEnable) override;

	void XMETHODCALLTYPE getSkyboxTexture(IXTexture **ppTexture) override;

	void setDevice(IGXDevice *pDevice);
	void setMaterialSystem(IXMaterialSystem *pMaterialSystem);

	//! рендер
	void render();

	void update(float fDT);

protected:
	IXCore *m_pCore = NULL;

	CSkyBox *m_pSkyBox = NULL;
};

#endif
