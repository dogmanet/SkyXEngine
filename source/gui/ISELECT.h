#ifndef _ISELECT_H_
#define _ISELECT_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
	{
		class ISELECT: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(ISELECT, IDOMnodeTag);
			DECLARE_DOM_NODE(ISELECT);


		};
	};
};

#endif
