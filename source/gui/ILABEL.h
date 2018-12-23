#ifndef _ILABEL_H_
#define _ILABEL_H_

#include "DOM.h"

namespace gui
{
	namespace dom
	{
		class ILABEL: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(ILABEL, IDOMnodeTag);
			DECLARE_DOM_NODE(ILABEL);

			ILABEL()
			{
				m_bDelegateEvents = true;
			}
		};
	};
};

#endif
