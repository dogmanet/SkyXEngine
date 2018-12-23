#include "GUIbase.h"
#include "ISELECT.h"


namespace gui
{
	namespace dom
	{
		DEFINE_DOM_NODE(ISELECT);

		void ISELECT::dispatchClientEvent(IEvent ev, bool * preventDefault)
		{
			if(ev.target == this)
			{
				if(ev.type == GUI_EVENT_TYPE_CLICK)
				{
					addPseudoclass(css::ICSSrule::PSEUDOCLASS_CHECKED);
				}

				if(ev.type == GUI_EVENT_TYPE_BLUR)
				{
					static UINT nOPTION = CDOMnode::getNodeIdByName(L"option");
					if(((CDOMnode*)ev.relatedTarget)->getNodeId() != nOPTION)
					{
						removePseudoclass(css::ICSSrule::PSEUDOCLASS_CHECKED);
					}
				}
			}

			BaseClass::dispatchClientEvent(ev, preventDefault);
		}

		void ISELECT::setAttribute(const StringW & name, const StringW & value)
		{
			BaseClass::setAttribute(name, value);

			static UINT nOPTION = CDOMnode::getNodeIdByName(L"option");

			for(int i = 0, l = m_vChilds.size(); i < l; ++i)
			{
				if(((CDOMnode*)m_vChilds[i])->getNodeId() == nOPTION && m_vChilds[i]->getAttribute(L"value") == value)
				{
					setText(m_vChilds[i]->getText(), TRUE);
					break;
				}
			}
		}
	};
};