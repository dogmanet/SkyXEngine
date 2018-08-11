#ifndef _ILINK_H_
#define _ILINK_H_

#include "DOM.h"

namespace gui
{
	namespace dom
	{
		class ILINK: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(ILINK, IDOMnodeTag);
			DECLARE_DOM_NODE(ILINK);


		};
	};
};

#endif
