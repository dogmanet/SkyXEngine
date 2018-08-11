#ifndef _IBUTTON_H_
#define _IBUTTON_H_

#include "DOM.h"

namespace gui
{
	namespace dom
	{
		class IBUTTON: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IBUTTON, IDOMnodeTag);
			DECLARE_DOM_NODE(IBUTTON);

			IBUTTON()
			{
				m_bFocusable = true;
			}

			void setAttribute(const StringW & name, const StringW & value);
		};
	};
};

#endif
