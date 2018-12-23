#ifndef _IIMG_H_
#define _IIMG_H_

#include "DOM.h"

namespace gui
{
	namespace dom
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
