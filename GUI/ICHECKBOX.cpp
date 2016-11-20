#include "GUIbase.h"
#include "ICHECKBOX.h"

namespace GUI
{
	namespace DOM
	{
		DEFINE_DOM_NODE(ICHECKBOX);

		ICHECKBOX::ICHECKBOX()
		{
			m_bToggleable = true;
			m_bFocusable = true;
		}

		void ICHECKBOX::SetAttribute(const StringW & name, const StringW & value)
		{
			if(name == L"checked")
			{
				if(value.ToBool())
				{
					AddPseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
					m_bToggleState = true;
				}
				else
				{
					RemovePseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
					m_bToggleState = true;
				}
			}
			BaseClass::SetAttribute(name, value);
		}
	};
};