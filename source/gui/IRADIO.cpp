#include "GUIbase.h"
#include "IRADIO.h"


namespace gui
{
	namespace dom
	{
		DEFINE_DOM_NODE(IRADIO);

		IRADIO::IRADIO()
		{
			m_bToggleable = true;
			m_bToggleOnly = true;
			m_bFocusable = true;
		}

		void IRADIO::setAttribute(const StringW & name, const StringW & value)
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