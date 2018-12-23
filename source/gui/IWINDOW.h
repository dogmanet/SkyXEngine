#ifndef _IWINDOW_H_
#define _IWINDOW_H_

#include "DOM.h"


namespace gui
{
	namespace dom
	{
		class IWINDOW: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IWINDOW, IDOMnodeTag);
			DECLARE_DOM_NODE(IWINDOW);


		};
	};
};

#endif
