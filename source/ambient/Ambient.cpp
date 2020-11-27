#include "Ambient.h"

CAmbient::CAmbient(IXCore *pCore):
	m_pCore(pCore),
	m_pSkyBox(new CSkyBox(pCore))
{
}

void XMETHODCALLTYPE CAmbient::setSkybox(const char *szTexture)
{
	m_pSkyBox->setTexture(szTexture);
}

void XMETHODCALLTYPE CAmbient::enableSkybox(bool bEnable)
{
	m_pSkyBox->enable(bEnable);
}

void XMETHODCALLTYPE CAmbient::getSkyboxTexture(IXTexture **ppTexture)
{
	m_pSkyBox->getTexture(ppTexture);
}

void CAmbient::setDevice(IGXDevice *pDevice)
{
	m_pSkyBox->setDevice(pDevice);
}
void CAmbient::setMaterialSystem(IXMaterialSystem *pMaterialSystem)
{
	m_pSkyBox->setMaterialSystem(pMaterialSystem);
}

void CAmbient::render()
{
	m_pSkyBox->render();
}

void CAmbient::update(float fDT)
{
	m_pSkyBox->update(fDT);
}