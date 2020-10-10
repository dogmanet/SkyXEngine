#ifndef __DESKTOP_H
#define __DESKTOP_H

#include <xcommon/gui/IXDesktop.h>

class CDesktopStack;
class CDesktop: public IXUnknownImplementation<IXDesktop>
{
public:
	CDesktop(CDesktopStack *pDecktopStack, const char *szName);
	~CDesktop();

	bool XMETHODCALLTYPE loadFromFile(const char *szFile) override;

	void XMETHODCALLTYPE setWidth(UINT w) override;
	void XMETHODCALLTYPE setHeight(UINT h) override;

	void XMETHODCALLTYPE render(float fTimeDelta, bool bPresent = true) override;

	void XMETHODCALLTYPE getDocument(IXDOMDocument **ppOut) override;

#if 0
	CPITexture getTexture();

	void dispatchEvent(IEvent ev);

	void requestFocus(dom::IDOMnode *pNode);

	dom::IDOMnode* getFocus();

	const dom::IDOMnodeCollection& createFromText(const StringW &html);
		
	void createRenderTarget();
	void releaseRenderTarget();
	void setDirty();

	float getParallaxFactor();

protected:

	CDesktopStack *m_pDesktopStack;

	UINT m_iWidth;
	UINT m_iHeight;

	CPITexture m_txFinal = NULL;

	bool m_bShowSimulatedCursor;
	bool m_bShowSystemCursor;

	dom::CDOMdocument * m_pDoc;

	dom::IDOMnode * m_pFocusedNode = NULL;
	dom::IDOMnode * m_pHoveredNode = NULL;

	//dom::IDOMnodeCollection m_cTmpNodes;

	IGXSurface * m_pRenderSurface = NULL;
	IGXDepthStencilSurface * m_pDepthStencilSurface = NULL;

	StringW m_sName;

	float m_fParallaxFactor = 0.0f;

	IGXVertexBuffer *m_pVertices = NULL;
	IGXRenderBuffer *m_pRenderBuffer = NULL;
	IGXConstantBuffer *m_pColorWhite = NULL;
#endif
};

#endif
