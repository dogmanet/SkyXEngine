#ifndef _IINPUT_H_
#define _IINPUT_H_

#include "DOM.h"

namespace gui
{
	namespace dom
	{
		class IINPUT: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IINPUT, IDOMnodeTag);
			DECLARE_DOM_NODE(IINPUT);

			IINPUT()
			{
				m_bEditable = true;
				m_bFocusable = true;
				m_bIgnHotkeys = true;
			}
		};
	};
};

#endif
