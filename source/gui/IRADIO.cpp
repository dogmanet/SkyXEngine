#include "GUIbase.h"
#include "IRADIO.h"


namespace GUI
{
	namespace DOM
	{
		DEFINE_DOM_NODE(IRADIO);

		IRADIO::IRADIO()
		{
			m_bToggleable = true;
			m_bToggleOnly = true;
			m_bFocusable = true;
		}

		void IRADIO::SetAttribute(const StringW & name, const StringW & value)
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