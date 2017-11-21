#ifndef _ColorPrint_H_
#define _ColorPrint_H_

#include <windows.h> 

enum AnsiColor
{
	ANSI_BLACK = 0x00,
	ANSI_BLUE = 0x01,
	ANSI_GREEN = 0x02,
	ANSI_RED = 0x04,
	ANSI_INTENSITY = 0x08,

	ANSI_CYAN = ANSI_BLUE | ANSI_GREEN,
	ANSI_MAGENTA = ANSI_RED | ANSI_BLUE,
	ANSI_OLIVE = ANSI_RED | ANSI_GREEN,
	ANSI_GRAY = ANSI_BLUE | ANSI_GREEN | ANSI_RED,

	ANSI_DARKGRAY = ANSI_INTENSITY | ANSI_BLACK,
	ANSI_LBLUE = ANSI_INTENSITY | ANSI_BLUE,
	ANSI_LGREEN = ANSI_INTENSITY | ANSI_GREEN,
	ANSI_LRED = ANSI_INTENSITY | ANSI_RED,
	ANSI_LCYAN = ANSI_INTENSITY | ANSI_BLUE | ANSI_GREEN,
	ANSI_LMAGENTA = ANSI_INTENSITY | ANSI_RED | ANSI_BLUE,
	ANSI_YELLOW = ANSI_INTENSITY | ANSI_RED | ANSI_GREEN,
	ANSI_WHITE = ANSI_INTENSITY | ANSI_BLUE | ANSI_GREEN | ANSI_RED
};

class ColorPrint
{
public:
	ColorPrint();
	~ColorPrint();

	void textAttr(AnsiColor text, AnsiColor background);
	void setColor(AnsiColor text);
	void setBG(AnsiColor background);
	void setDefault();
	AnsiColor getDefaultFG()
	{
		return((AnsiColor)(wOldColorAttrs & 0xF));
	}
	AnsiColor getDefaultBG()
	{
		return((AnsiColor)((wOldColorAttrs >> 4) & 0xF));
	}
private:
	HANDLE hStdout;
	WORD wOldColorAttrs;
	AnsiColor curBG;
	AnsiColor curFG;
	void textAttr(WORD attr);
};

#endif