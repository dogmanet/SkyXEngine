#ifndef _ITEXTAREA_H_
#define _ITEXTAREA_H_

#include "DOM.h"

namespace gui
{
	namespace dom
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
