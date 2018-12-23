#ifndef _IRADIO_H_
#define _IRADIO_H_

#include "DOM.h"

namespace gui
{
	namespace dom
	{
		class IRADIO: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IRADIO, IDOMnodeTag);
			DECLARE_DOM_NODE(IRADIO);

			IRADIO();

			void setAttribute(const StringW & name, const StringW & value);
		};
	};
};

#endif
