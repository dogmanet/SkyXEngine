#include "IScrollBar.h"
#include "IRenderFrame.h"


namespace GUI
{
	namespace DOM
	{
		namespace Render
		{
			IScrollBar::IScrollBar(IRenderFrame * _parent, SCROLLBAR_DIR _dir):
				m_pParent(_parent),
				m_eDir(_dir),
				m_iScrollMax(0),
				m_iScrollCur(0),
				m_iLength(0)
			{
			}

			void IScrollBar::SetLength(int len)
			{
				m_iLength = len;
			}

			void IScrollBar::UpdateData()
			{
				if(!m_pParent)
				{
					return;
				}
				if(m_eDir == SCROLLBAR_DIR_HORIZONTAL)
				{
					m_iScrollCur = m_pParent->GetScrollLeft();
					m_iScrollMax = m_pParent->GetScrollLeftMax();
				}
				else if(m_eDir == SCROLLBAR_DIR_VERTICAL)
				{
					m_iScrollCur = m_pParent->GetScrollTop();
					m_iScrollMax = m_pParent->GetScrollTopMax();
				}
			}
		};
	};
};