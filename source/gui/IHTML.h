#ifndef _IHTML_H_
#define _IHTML_H_

#include "DOM.h"

#include <cstdio>

namespace gui
{
	namespace dom
	{
		class IHTML: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IHTML, IDOMnodeTag);
			DECLARE_DOM_NODE(IHTML);


		};
	};
};

#endif
