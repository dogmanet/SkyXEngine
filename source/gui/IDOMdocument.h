#ifndef _IDOMdocument_H_
#define _IDOMdocument_H_

#include "IDOM.h"
//#include "IRenderFrame.h"

namespace GUI
{
	class IDesktop;
	namespace DOM
	{
		class IDOMdocument
		{
		public:
			virtual ~IDOMdocument()
			{
			}

			virtual IDOMnode * CreateNode(const wchar_t * tag) = 0;

			virtual IDOMnode * GetElementById(const StringW & id) = 0;

			virtual const IDOMnodeCollection * GetElementsByClass(const StringW & id) = 0;

			virtual const IDOMnodeCollection * GetElementsByPseudoClass(UINT cid) = 0;

			virtual const IDOMnodeCollection * GetElementsByTag(const StringW & id) = 0;

			virtual IDOMnode * GetElementByXY(int x, int y, bool sendEnterLeave = false) = 0;

			//	IDesktop * GetDesktop();
			virtual void SetDesktop(IDesktop * pdp) = 0;

			virtual void RequestFocus(IDOMnode * pn) = 0;
			virtual IDOMnode * GetFocus() = 0;
		};
	};
};

#endif
