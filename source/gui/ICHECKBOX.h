#ifndef _ICHECKBOX_H_
#define _ICHECKBOX_H_

#include "CDOM.h"


namespace GUI
{
	namespace DOM
	{
		class ICHECKBOX: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(ICHECKBOX, IDOMnodeTag);
			DECLARE_DOM_NODE(ICHECKBOX);

			ICHECKBOX();

			void SetAttribute(const StringW & name, const StringW & value);
		};
	};
};

#endif
