#ifndef _CDesktop_H_
#define _CDesktop_H_

#include "GUIbase.h"
#include "IDesktop.h"
#include "CDOMdocument.h"

namespace GUI
{
	class CDesktop: public IDesktop
	{
	public:
		CDesktop();
		~CDesktop();

		void LoadFromFile(const WCHAR * str);

		void SetWidth(UINT w);
		void SetHeight(UINT h);
		void UpdateSize();

		void Render();

		CPITexture GetTexture();

		void DispatchEvent(IEvent ev);

		DOM::IDOMdocument * GetDocument();

		void RequestFocus(DOM::IDOMnode * pNode);

		DOM::IDOMnode * GetFocus();

		const DOM::IDOMnodeCollection & CreateFromText(const StringW & html);

		void Release();

	protected:

		UINT m_iWidth;
		UINT m_iHeight;

		ITexture m_txFinal;

		bool m_bShowSimulatedCursor;
		bool m_bShowSystemCursor;

		DOM::CDOMdocument * m_pDoc;

		DOM::IDOMnode * m_pFocusedNode;
		DOM::IDOMnode * m_pHoveredNode;

		DOM::IDOMnodeCollection m_cTmpNodes;
	};
};

#endif
