#include "CSSLeherAlgoritm.h"

bool CSSLeherAlgoritm::isNewLine(char ch, char chNext)
{
	return ch == '\n' || (ch == '\r' && chNext == '\n') || ch == '\r' || ch == '\f';
}

bool CSSLeherAlgoritm::isSpaceSymbol(char ch)
{
	// 0x0009
	// 0x0020 
	return ch == ' ' || ch == '\t';
}

bool CSSLeherAlgoritm::isWhitespaceToken(char ch, char chNext)
{
	return isNewLine(ch, chNext) || isSpaceSymbol(ch);
}

bool CSSLeherAlgoritm::isColonToken(char ch)
{
	return ch == ':'; // 0x003A
}

bool CSSLeherAlgoritm::isSemicolonToken(char ch)
{
	return ch == ';'; // 0x003B
}

bool CSSLeherAlgoritm::isOpenBraceToken(char ch)
{
	return ch == '{'; // 0x007B
}

bool CSSLeherAlgoritm::isCloseBraceToken(char ch)
{
	return ch == '}'; // 0x007D
}