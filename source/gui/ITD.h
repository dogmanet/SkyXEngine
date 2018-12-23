#ifndef _ITD_H_
#define _ITD_H_

#include "DOM.h"

namespace gui
{
	namespace dom
	{
		class ITD: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(ITD, IDOMnodeTag);
			DECLARE_DOM_NODE(ITD);


		};
	};
};

#endif
