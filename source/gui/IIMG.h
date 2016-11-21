#ifndef _IIMG_H_
#define _IIMG_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
	{
		class IIMG: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IIMG, IDOMnodeTag);
			DECLARE_DOM_NODE(IIMG);


		};
	};
};

#endif
