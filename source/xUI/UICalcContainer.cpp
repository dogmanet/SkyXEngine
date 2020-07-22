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

	mDisplayedStr.appendReserve(30);
	mCurrOperation.appendReserve(3);

	for(int i = 0; i < ARRAYSIZE(aszButtonNames); ++i)
	{
		mButtons[i] = pXUI->createButton(pCalc);
		mButtons[i]->setLabel(StringW(aszButtonNames[i]).c_str());
	}
	//IUICommand *pCommand = new CUICommand<decltype(&UICalcContainer::inputNumOne)>(/*&UICalcContainer::inputNumOne*/this->inputNumber);

	//IUICommand *pCommand = new CUICommand(F);
	IUICommand *pCommand = new CUIClassCommand(&UICalcContainer::inputNumOne, this);

	// mButtons[0]->setSize(150, 150);
	mButtons[0]->setClickCommand(pCommand);
	//mButtons[0]->setPosition(100, 100);
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < ARRAYSIZE(aszButtonNames) / 3; ++j)
		{
			mButtons[j * 3 + i]->setPosition(25 + i * 150, 25 + j * 50);
			mButtons[j * 3 + i]->setSize(140, 40);
		}
	}
}

void UICalcContainer::inputNumber(int iNum)
{
	if (bInputNumTwo)
	{
		mSecondNum = (mSecondNum * 10) + iNum;
		mDisplayedStr += iNum;
	} 
	else
	{
		mFirstNum = (mFirstNum * 10) + iNum;
		mDisplayedStr += iNum;
	}

	printf("String: %s \n", mDisplayedStr);
	printf("Num1: %d \nNum2: %d", mFirstNum, mSecondNum);
	m_pTextBox->setLabel(mDisplayedStr.c_str());
}

void UICalcContainer::inputOperation(char op)
{
	switch (op)
	{
	case '+':
		mCurrOperation = L" + ";
		mbuffRes = mFirstNum + mSecondNum;
		break;
	case '-':
		mCurrOperation = L" - ";
		mbuffRes = mFirstNum - mSecondNum;
		break;
	case '/':
		mCurrOperation = L" / ";
		mbuffRes = mFirstNum / mSecondNum;
		break;
	case '*':
		mCurrOperation = L" * ";
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