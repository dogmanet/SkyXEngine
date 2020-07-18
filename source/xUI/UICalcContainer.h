#ifndef __UICALCCONTAINER_H
#define __UICALCCONTAINER_H

#include <xUI/UIWindow.h>
#include <xUI/IXUI.h>
#include "UICommand.h"
#include "UIClassCommand.h"

class UICalcContainer : public IXUnknown
{
public:
	UICalcContainer(IXUI* pXUI);

private:
	void inputNumber(int iNum);
	void inputOperation(char op);

	void inputNumOne();
	void inputNumTwo();
	void inputNumThree();
	void inputNumFour();
	void inputNumFive();
	void inputNumSix();
	void inputNumSeven();
	void inputNumEight();
	void inputNumNine();
	void inputNumZero();

	void operationPlus();
	void operationMenis();
	void operationDiv();
	void operationMultiply();

	INT64 mbuffRes = 0;
	INT64 mFirstNum = 0;
	INT64 mSecondNum = 0;

	IXUI* pXUI = NULL;
	IUIWindow *pCalc = NULL;

	Array<IUIButton*> mButtons;
	IUITextBox *m_pTextBox = NULL;

	StringW mDisplayedStr;
	StringW mCurrOperation;
	
	bool bInputNumTwo = false; //Если первая введена то ставим переключатель в true
};

#endif
