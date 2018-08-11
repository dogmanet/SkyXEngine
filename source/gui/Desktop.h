#ifndef _CDesktop_H_
#define _CDesktop_H_

#include "GUIbase.h"
#include "IDesktop.h"
#include "DOMdocument.h"

namespace gui
{
	class CDesktop: public IDesktop
	{
	public:
		CDesktop(const StringW & sName);
		~CDesktop();

		void loadFromFile(const WCHAR * str);

		void setWidth(UINT w);
		void setHeight(UINT h);
		void updateSize();

		void render(float fTimeDelta, bool bPresent = true);

		CPITexture getTexture();

		void dispatchEvent(IEvent ev);

		dom::IDOMdocument * getDocument();

		void requestFocus(dom::IDOMnode * pNode);

		dom::IDOMnode * getFocus();

		const dom::IDOMnodeCollection & createFromText(const StringW & html);

		void release();

		void createRenderTarget();
		void releaseRenderTarget();
		void setDirty();

		float getParallaxFactor();

	protected:

		UINT m_iWidth;
		UINT m_iHeight;

		CPITexture m_txFinal;

		bool m_bShowSimulatedCursor;
		bool m_bShowSystemCursor;

		dom::CDOMdocument * m_pDoc;

		dom::IDOMnode * m_pFocusedNode;
		dom::IDOMnode * m_pHoveredNode;

		//dom::IDOMnodeCollection m_cTmpNodes;

		IDirect3DSurface9 * m_pRenderSurface;
		IDirect3DSurface9 * m_pDepthStencilSurface;

		StringW m_sName;

		float m_fParallaxFactor;
	};
};

#endif
