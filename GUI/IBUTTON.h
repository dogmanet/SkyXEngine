#ifndef _IBUTTON_H_
#define _IBUTTON_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
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

			void SetAttribute(const StringW & name, const StringW & value);
		};
	};
};

#endif
