#include "UICalcContainer.h"

UICalcContainer::UICalcContainer(IXUI* pXUI)
{
	XWINDOW_DESC wdesc;
	wdesc.iPosX = XCW_CENTER;
	wdesc.iPosY = XCW_CENTER;
	wdesc.iSizeX = 1280;
	wdesc.iSizeY = 1024;
	wdesc.szTitle = "Calculator";
	wdesc.flags = XWF_BUTTON_CLOSE | XWF_BUTTON_MINIMIZE | XWF_BUTTON_MAXIMIZE | XWF_TRANSPARENT;

	pCalc = pXUI->createWindow(&wdesc);
	m_pTextBox = pXUI->createTextBox(pCalc);

	mDisplayedStr.appendReserve(30);
	mCurrOperation.appendReserve(3);

	for (int i = 0; i < 14; ++i)
	{
		mButtons[i] = pXUI->createButton(pCalc);
	}
}

void UICalcContainer::inputNumber(int iNum)
{
	if (bInputNumTwo)
	{
		mFirstNum = (mFirstNum * 10) + iNum;
		mDisplayedStr += iNum;
	}
	else
	{
		mSecondNum = (mSecondNum * 10) + iNum;
		mDisplayedStr += iNum;
	}

	m_pTextBox->setLabel(mDisplayedStr.c_str());
}

void UICalcContainer::inputOperation(char op)
{
	switch (op)
	{
	case '+':
		mCurrOperation = ' + ';
		mbuffRes = mFirstNum + mSecondNum;
		break;
	case '-':
		mCurrOperation = ' - ';
		mbuffRes = mFirstNum - mSecondNum;
		break;
	case '/':
		mCurrOperation = ' / ';
		mbuffRes = mFirstNum / mSecondNum;
		break;
	case '*':
		mCurrOperation = ' * ';
		mbuffRes = mFirstNum * mSecondNum;
		break;
	}

	if (bInputNumTwo)
	{
		mDisplayedStr = String(mbuffRes) + mCurrOperation;
	}

	bInputNumTwo = true;
}

void UICalcContainer::inputNumOne()
{
	inputNumber(1);
}

void UICalcContainer::inputNumTwo()
{
	inputNumber(2);
}

void UICalcContainer::inputNumThree()
{
	inputNumber(3);
}

void UICalcContainer::inputNumFour()
{
	inputNumber(4);
}

void UICalcContainer::inputNumFive()
{
	inputNumber(5);
}

void UICalcContainer::inputNumSix()
{
	inputNumber(6);
}

void UICalcContainer::inputNumSeven()
{
	inputNumber(7);
}

void UICalcContainer::inputNumEight()
{
	inputNumber(8);
}

void UICalcContainer::inputNumNine()
{
	inputNumber(9);
}

void UICalcContainer::inputNumZero()
{
	inputNumber(0);
}

void UICalcContainer::operationPlus()
{
	inputOperation('+');
}

void UICalcContainer::operationMenis()
{
	inputOperation('-');
}

void UICalcContainer::operationDiv()
{
	inputOperation('/');
}

void UICalcContainer::operationMultiply()
{
	inputOperation('*');
}