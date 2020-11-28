#include "UICalcContainer.h"

UICalcContainer::UICalcContainer(IXUI* pXUI)
{
	XWINDOW_DESC wdesc;
	wdesc.iPosX = XCW_CENTER;
	wdesc.iPosY = XCW_CENTER;
	wdesc.iSizeX = 800;
	wdesc.iSizeY = 600;
	wdesc.szTitle = "Calculator";
	wdesc.flags = XWF_BUTTON_CLOSE | XWF_BUTTON_MINIMIZE | XWF_BUTTON_MAXIMIZE | XWF_TRANSPARENT;

	const wchar_t aszButtonNames[] = {L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', L'+', L'-', L'*', L'/', L'='};

	pCalc = pXUI->createWindow(&wdesc);
	m_pTextBox = pXUI->createTextBox(pCalc);

	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::pressButton, this));

	for(int i = 0; i < ARRAYSIZE(aszButtonNames); ++i)
	{
		mButtons[i] = pXUI->createButton(pCalc);
		mButtons[i]->setLabel(StringW(aszButtonNames[i]).c_str());
		mButtons[i]->setClickCommand(mpCommands[0]);
	}

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < ARRAYSIZE(aszButtonNames) / 3; ++j)
		{
			mButtons[j * 3 + i]->setPosition(150 + i * 150, 150 + j * 50);
			mButtons[j * 3 + i]->setSize(140, 40);
		}
	}
	
	m_pTextBox->setPosition(150, 100);
	m_pTextBox->setSize(440, 25);
}

void UICalcContainer::pressButton()
{
	m_pTextBox->setLabel(L"You press button");
}