#ifndef _ISPAN_H_
#define _ISPAN_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
	{
		class ISPAN: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(ISPAN, IDOMnodeTag);
			DECLARE_DOM_NODE(ISPAN);


		};
	};
};

#endif
