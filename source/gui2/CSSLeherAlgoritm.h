#ifndef __CSSLEXERALGORITM_H
#define __CSSLEXERALGORITM_H

class CSSLeherAlgoritm
{
private:
	bool isNewLine(char ch, char chNext);
	bool isSpaceSymbol(char ch);

public:
	bool isWhitespaceToken(char ch, char chNext);
	bool isColonToken(char ch);
	bool isSemicolonToken(char ch);
	bool isOpenBraceToken(char ch);
	bool isCloseBraceToken(char ch);
};

#endif

