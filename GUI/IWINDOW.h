#ifndef _IWINDOW_H_
#define _IWINDOW_H_

#include "CDOM.h"


namespace GUI
{
	namespace DOM
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
