#include "UIButton.h"

void CUIButton::setClickCommand(IUICommand* command)
{
	m_pCommand = command;
}