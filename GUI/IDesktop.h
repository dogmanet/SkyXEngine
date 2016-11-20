#ifndef _IDesktop_H_
#define _IDesktop_H_

//#include "guimain.h"
#include "IDOMdocument.h"
//#include "ITexture.h"
//#include "IEvent.h"


namespace GUI
{
	namespace DOM
	{
		class IDOMnode;
	};
	class ITexture;
	class IEvent;

	class IDesktop
	{
	public:
		virtual ~IDesktop()
		{
		}

		virtual void LoadFromFile(const WCHAR * str) = 0;

		virtual void SetWidth(UINT w) = 0;
		virtual void SetHeight(UINT h) = 0;
		virtual void UpdateSize() = 0;

		virtual void Render() = 0;

		virtual const ITexture * GetTexture() = 0;

		virtual void DispatchEvent(IEvent ev) = 0;

		virtual DOM::IDOMdocument * GetDocument() = 0;

		virtual void RequestFocus(DOM::IDOMnode * pNode) = 0;

		virtual DOM::IDOMnode * GetFocus() = 0;

		virtual DOM::IDOMnodeCollection CreateFromText(const StringW & html) = 0;

		virtual void Release() = 0;
	};
};

#endif
