
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "ColorPrint.h"


ColorPrint::ColorPrint()
{
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if(hStdout == INVALID_HANDLE_VALUE)
	{
		return;
	}

	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(hStdout, &csbiInfo);

	wOldColorAttrs = csbiInfo.wAttributes;

	curBG = (AnsiColor)((wOldColorAttrs >> 4) & 0xF);
	curFG = (AnsiColor)(wOldColorAttrs & 0xF);
}

ColorPrint::~ColorPrint()
{
	setDefault();
}

void ColorPrint::textAttr(WORD attr)
{
	SetConsoleTextAttribute(hStdout, attr);
}

void ColorPrint::textAttr(AnsiColor text, AnsiColor background)
{
	if(text > 0x0F || background > 0x0F)
	{
		return;
	}
	curBG = background;
	curFG = text;
	textAttr(background << 4 | text);
}

void ColorPrint::setColor(AnsiColor text)
{
	textAttr(text, curBG);
}
void ColorPrint::setBG(AnsiColor background)
{
	textAttr(curFG, background);
}
void ColorPrint::setDefault()
{
	textAttr(wOldColorAttrs);
}