#ifndef __UICALCCONTAINER_H
#define __UICALCCONTAINER_H

#include <xUI/UIWindow.h>
#include <xUI/IXUI.h>
#include <xUI/UICommand.h>
#include <xUI/UIClassCommand.h>

class UICalcContainer : public IXUnknownImplementation<IXUnknown>
{
public:
	UICalcContainer(IXUI* pXUI);

private:
	void inputNumber(int iNum);
	void inputOperation(char op);

	void pressButton();

	IXUI* pXUI = NULL;
	IUIWindow *pCalc = NULL;

	Array<IUIButton*> mButtons;
	Array<IUICommand*> mpCommands;
	IUITextBox *m_pTextBox = NULL;

	StringW mDisplayedStr;
	StringW mCurrOperation;

	bool bInputNumTwo = false; //Если первая введена то ставим переключатель в true
};

#endif
