#include "GUIbase.h"
#include "IRANGE.h"

#include "IRenderFrame.h"

namespace gui
{
	namespace dom
	{
		DEFINE_DOM_NODE(IRANGE);
		LINK_DOM_NODE_TO_TAG(IRANGE, L"range");

		IRANGE::IRANGE():
			m_bDragging(false),
			m_fMin(0.0f),
			m_fMax(100.0f),
			m_fStep(1.0f),
			m_fValue(0.0f),
			m_wsOutFormat(L"%.0f")
		{
			m_pBarNode = createNode(L"range-bar");
			m_pDragNode = createNode(L"range-drag");
			m_pMeterNode = createNode(L"range-meter");
			m_pTextNode = createNode(L"range-text");
			m_pText = createNode(L"text");
			m_pText->setText(L"0");
			m_pTextNode->appendChild(m_pText, false);
			m_pBarNode->appendChild(m_pMeterNode, false);
			m_pBarNode->appendChild(m_pDragNode, false);

			appendChild(m_pBarNode, false);
			appendChild(m_pTextNode);
		}

		void IRANGE::setAttribute(const StringW &name, const StringW &value)
		{
			BaseClass::setAttribute(name, value);

			if(name == L"min")
			{
				swscanf(value.c_str(), L"%f", &m_fMin);
			}
			else if(name == L"max")
			{
				swscanf(value.c_str(), L"%f", &m_fMax);
			}
			else if(name == L"step")
			{
				swscanf(value.c_str(), L"%f", &m_fStep);
			}
			else if(name == L"format")
			{
				m_wsOutFormat = value;
				//swscanf(value.c_str(), L"%f", &m_fStep);
			}
			else if(name == L"value")
			{
				//m_pTextNode->setText(value, TRUE);
				swscanf(value.c_str(), L"%f", &m_fValue);

				wchar_t tmp[128];
				swprintf_s(tmp, m_wsOutFormat.c_str(), m_fValue);
				m_pTextNode->setText(tmp, TRUE);

				float fFrac = (m_fValue - m_fMin) / (m_fMax - m_fMin);

				gui::css::ICSSproperty *pProp = m_pMeterNode->getStyleSelf()->width;
				pProp->set(fFrac * 100.0f);
				pProp->setDim(gui::css::ICSSproperty::DIM_PC);
				m_pMeterNode->updateStyles();
			}
		}

		void IRANGE::dispatchEvent(IEvent & ev)
		{
			BaseClass::dispatchEvent(ev);

			switch(ev.type)
			{

			case GUI_EVENT_TYPE_MOUSEUP:
				m_bDragging = false;
				break;

			}
		}

		void IRANGE::dispatchClientEvent(IEvent ev, bool *preventDefault)
		{
			//ev.offsetX
			switch(ev.type)
			{
			case GUI_EVENT_TYPE_MOUSEDOWN:
				if(ev.target == m_pBarNode || ev.target == m_pMeterNode)
				{
					m_bDragging = true;
				}
				break;

			case GUI_EVENT_TYPE_MOUSEMOVE:
				if(m_bDragging && (ev.target == m_pBarNode || ev.target == m_pMeterNode))
				{
					UINT width = ((CDOMnode*)m_pBarNode)->getRenderFrame()->getWidth();
					float fFrac = (float)ev.offsetX / (float)width;

					if(fFrac < 0.0f)
					{
						fFrac = 0.0f;
					}
					else if(fFrac > 1.0f)
					{
						fFrac = 1.0f;
					}

					
					m_fValue = lerpf(m_fMin, m_fMax, fFrac);
					m_fValue = m_fStep * roundf(m_fValue / m_fStep);

					setAttribute(L"value", StringW(m_fValue));
				}
				break;
			}
			BaseClass::dispatchClientEvent(ev, preventDefault);
		}
	};
};
