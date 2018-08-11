#ifndef _ITABLE_H_
#define _ITABLE_H_

#include "DOM.h"

namespace gui
{
	namespace dom
	{
		class ITABLE: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(ITABLE, IDOMnodeTag);
			DECLARE_DOM_NODE(ITABLE);


		};
	};
};

#endif
