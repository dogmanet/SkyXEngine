#ifndef _IRANGE_H_
#define _IRANGE_H_

#include "DOM.h"


namespace gui
{
	namespace dom
	{
		class IRANGE: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IRANGE, IDOMnodeTag);
			DECLARE_DOM_NODE(IRANGE);

			IRANGE();

			void dispatchClientEvent(IEvent ev, bool * preventDefault);
			void dispatchEvent(IEvent & ev);
			void setAttribute(const StringW & name, const StringW & value);

		protected:
			bool m_bDragging;

			float m_fMin;
			float m_fMax;
			float m_fStep;
			float m_fValue;

			StringW m_wsOutFormat;

			IDOMnode *m_pBarNode;
			IDOMnode *m_pDragNode;
			IDOMnode *m_pMeterNode;
			IDOMnode *m_pTextNode;
			IDOMnode *m_pText;
		};
	};
};

#endif
