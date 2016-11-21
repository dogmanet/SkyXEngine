#ifndef _IBODY_H_
#define _IBODY_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
	{
		class IBODY: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IBODY, IDOMnodeTag);
			DECLARE_DOM_NODE(IBODY);


		};
	};
};
#endif
