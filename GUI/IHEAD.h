#ifndef _IHEAD_H_
#define _IHEAD_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
	{
		class IHEAD: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IHEAD, IDOMnodeTag);
			DECLARE_DOM_NODE(IHEAD);


		};
	};
};

#endif
