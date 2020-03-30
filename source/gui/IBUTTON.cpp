#include "GUIbase.h"
#include "IBUTTON.h"

namespace gui
{
	namespace dom
	{
		DEFINE_DOM_NODE(IBUTTON);
		LINK_DOM_NODE_TO_TAG(IBUTTON, L"button");

		void IBUTTON::setAttribute(const StringW & name, const StringW & value)
		{
			if(name == L"active")
			{
				if(value.toBool())
				{
					addPseudoclass(css::ICSSrule::PSEUDOCLASS_ACTIVE);
					m_bToggleState = true;
				}
				else
				{
					removePseudoclass(css::ICSSrule::PSEUDOCLASS_ACTIVE);
					m_bToggleState = false;
				}
			}
			if(name == L"toggleable")
			{
				m_bToggleable = value.toBool();
			}
			BaseClass::setAttribute(name, value);
		}
	};
};
