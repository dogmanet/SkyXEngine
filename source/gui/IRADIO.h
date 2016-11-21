#ifndef _IRADIO_H_
#define _IRADIO_H_

#include "CDOM.h"

namespace GUI
{
	namespace DOM
	{
		class IRADIO: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IRADIO, IDOMnodeTag);
			DECLARE_DOM_NODE(IRADIO);

			IRADIO();

			void SetAttribute(const StringW & name, const StringW & value);
		};
	};
};

#endif
