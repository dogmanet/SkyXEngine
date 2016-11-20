#ifndef _IHTML_H_
#define _IHTML_H_

#include "CDOM.h"

#include <cstdio>

namespace GUI
{
	namespace DOM
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
