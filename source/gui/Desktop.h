#ifndef _CDesktop_H_
#define _CDesktop_H_

#include "GUIbase.h"
#include "IDesktop.h"
#include "DOMdocument.h"

namespace gui
{
	class CDesktop: public IXUnknownImplementation<IDesktop>
	{
	public:
		CDesktop(CDesktopStack *pDecktopStack, const StringW &sName);
		~CDesktop();

		void loadFromFile(const WCHAR *str);

		void setWidth(UINT w);
		void setHeight(UINT h);
		void updateSize();

		void render(float fTimeDelta, bool bPresent = true);

		CPITexture getTexture();

		void dispatchEvent(IEvent ev);

		dom::IDOMdocument* getDocument();

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
	};
};

#endif
