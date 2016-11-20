#ifndef _ITD_H_
#define _ITD_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
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
