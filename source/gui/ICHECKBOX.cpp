#include "GUIbase.h"
#include "ICHECKBOX.h"

namespace gui
{
	namespace dom
	{
		DEFINE_DOM_NODE(ICHECKBOX);

		ICHECKBOX::ICHECKBOX()
		{
			m_bToggleable = true;
			m_bFocusable = true;
		}

		void ICHECKBOX::setAttribute(const StringW & name, const StringW & value)
		{
			if(name == L"checked")
			{
				if(value.toBool())
				{
					addPseudoclass(css::ICSSrule::PSEUDOCLASS_ACTIVE);
					m_bToggleState = true;
				}
				else
				{
					removePseudoclass(css::ICSSrule::PSEUDOCLASS_ACTIVE);
					m_bToggleState = true;
				}
			}
			BaseClass::setAttribute(name, value);
		}
	};
};