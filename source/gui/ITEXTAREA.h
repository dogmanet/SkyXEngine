#ifndef _ITEXTAREA_H_
#define _ITEXTAREA_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
	{
		class ITEXTAREA: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(ITEXTAREA, IDOMnodeTag);
			DECLARE_DOM_NODE(ITEXTAREA);


		};
	};
};

#endif
