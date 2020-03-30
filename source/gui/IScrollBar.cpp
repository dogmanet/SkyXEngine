#include "IScrollBar.h"
#include "IRenderFrame.h"


namespace gui
{
	namespace dom
	{
		namespace render
		{
			IScrollBar::IScrollBar(IRenderFrame *_parent, SCROLLBAR_DIR _dir):
				m_pParent(_parent),
				m_eDir(_dir)
			{
			}

			void IScrollBar::setLength(int len)
			{
				m_iLength = len;
			}

			void IScrollBar::updateData()
			{
				if(!m_pParent)
				{
					return;
				}
				if(m_eDir == SCROLLBAR_DIR_HORIZONTAL)
				{
					m_iScrollCur = m_pParent->getScrollLeft();
					m_iScrollMax = m_pParent->getScrollLeftMax();
				}
				else if(m_eDir == SCROLLBAR_DIR_VERTICAL)
				{
					m_iScrollCur = m_pParent->getScrollTop();
					m_iScrollMax = m_pParent->getScrollTopMax();
				}
			}
		};
	};
};