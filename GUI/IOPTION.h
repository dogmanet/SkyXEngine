#ifndef _IOPTION_H_
#define _IOPTION_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
	{
		class IOPTION: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IOPTION, IDOMnodeTag);
			DECLARE_DOM_NODE(IOPTION);


		};
	};
};

#endif
