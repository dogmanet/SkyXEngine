#include "GUIbase.h"
#include "IBUTTON.h"

namespace GUI
{
	namespace DOM
	{
		DEFINE_DOM_NODE(IBUTTON);

		void IBUTTON::SetAttribute(const StringW & name, const StringW & value)
		{
			if(name == L"active")
			{
				if(value.ToBool())
				{
					AddPseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
					m_bToggleState = true;
				}
				else
				{
					RemovePseudoclass(CSS::ICSSrule::PSEUDOCLASS_ACTIVE);
					m_bToggleState = false;
				}
			}
			if(name == L"toggleable")
			{
				m_bToggleable = value.ToBool();
			}
			BaseClass::SetAttribute(name, value);
		}
	};
};