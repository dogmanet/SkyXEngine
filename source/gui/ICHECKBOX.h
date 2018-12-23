#ifndef _ICHECKBOX_H_
#define _ICHECKBOX_H_

#include "DOM.h"


namespace gui
{
	namespace dom
	{
		class ICHECKBOX: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(ICHECKBOX, IDOMnodeTag);
			DECLARE_DOM_NODE(ICHECKBOX);

			ICHECKBOX();

			void setAttribute(const StringW & name, const StringW & value);
		};
	};
};

#endif
