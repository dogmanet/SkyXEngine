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
/*	m_pTextBox = pXUI->createTextBox(pCalc);

	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::inputNumOne, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::inputNumTwo, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::inputNumThree, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::inputNumFour, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::inputNumFive, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::inputNumSix, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::inputNumSeven, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::inputNumEight, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::inputNumNine, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::inputNumZero, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::operationPlus, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::operationMinus, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::operationMultiply, this));
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::operationDiv, this));	
	mpCommands.push_back(new CUIClassCommand(&UICalcContainer::operationEqually, this));

	
	for(int i = 0; i < ARRAYSIZE(aszButtonNames); ++i)
	{
		mButtons[i] = pXUI->createButton(pCalc);
	//	mButtons[i]->setLabel(StringW(aszButtonNames[i]).c_str());
		mButtons[i]->setPicture(L"/xbg.png", 25, 25);
		mButtons[i]->setClickCommand(mpCommands[i]);
	}

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < ARRAYSIZE(aszButtonNames) / 3; ++j)
		{
			mButtons[j * 3 + i]->setPosition(150 + i * 150, 150 + j * 50);
			mButtons[j * 3 + i]->setSize(140, 40);
		}
	}
	*/
//	m_pTextBox->setPosition(150, 100);
//	m_pTextBox->setSize(440, 25);

//	pc = pXUI->createPicture(pCalc);

//	pc->setPicture(L"/stalin.jpg", 400, 400);
//	pc->setPosition(25, 25);
//	pc->setSize(1000, 1000);
	
	//pc->setLabel(L"box");



	cb = pXUI->createCheckBox(pCalc);


//	cb->setPosition(50, 100);
//	cb->setSize(240, 50);
//	cb->setLabel(L"box");
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

	printDisplayStr();
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
	case '=':
		mCurrOperation = L" = ";
		mbuffRes = mFirstNum * mSecondNum;
		break;
	} 

	mDisplayedStr = (bInputNumTwo ? String(mbuffRes) : String(mFirstNum)) + mCurrOperation;

	if (bInputNumTwo)
	{
		mFirstNum = mbuffRes;
		mSecondNum = 0;
	}

	bInputNumTwo = true;
	printDisplayStr();
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

void UICalcContainer::operationMinus()
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

void UICalcContainer::operationEqually()
{
	if (cb->isChecked())
	{
		m_pTextBox->setLabel(L"True");
	}
	else
	{
		m_pTextBox->setLabel(L"False");
	}
	
	//inputOperation('=');
	//bInputNumTwo = false;
}

void UICalcContainer::printDisplayStr()
{
	m_pTextBox->setLabel(mDisplayedStr.c_str());
}