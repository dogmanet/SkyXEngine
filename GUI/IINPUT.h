#ifndef _IINPUT_H_
#define _IINPUT_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
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
			}
		};
	};
};

#endif
