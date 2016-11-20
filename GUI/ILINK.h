#ifndef _ILINK_H_
#define _ILINK_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
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
