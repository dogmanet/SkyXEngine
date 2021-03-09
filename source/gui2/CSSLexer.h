#ifndef __CSSLEXER_H
#define __CSSLEXER_H

#include <gdefines.h>
#include <xcommon/gui/IXCSSProperty.h>
#include <common/StreamReader.h>

// https://drafts.csswg.org/css-syntax-3/

/* 
<ident-token>, 
<function-token>, 
<at-keyword-token>, 
<hash-token>, 
<string-token>, 
<bad-string-token>, 
<url-token>, 
<bad-url-token>, 
<delim-token>, 
<number-token>, 
<percentage-token>, 
<dimension-token>, 
<whitespace-token>, 
<CDO-token>, 
<CDC-token>, 
<colon-token>, 
<semicolon-token>, 
<comma-token>, 
<[-token>, 
<]-token>, 
<(-token>, 
<)-token>, 
<{-token>, 
<}-token>
*/

enum CSS_TOKEN_TYPE
{
	CSS_TOKEN_TYPE_UNKNOW_TOKEN,
	CSS_TOKEN_TYPE_EOF_TOKEN,

	CSS_TOKEN_TYPE_IDENT_TOKEN,
	CSS_TOKEN_TYPE_FUNCTION_TOKEN,
	CSS_TOKEN_TYPE_AT_KEYWORD_TOKEN,
	CSS_TOKEN_TYPE_HASH_TOKEN,
	CSS_TOKEN_TYPE_STRING_TOKEN,
	CSS_TOKEN_TYPE_BAD_STRING_TOKEN,
	CSS_TOKEN_TYPE_URL_TOKEN,
	CSS_TOKEN_TYPE_BAD_URL_TOKEN,
	CSS_TOKEN_TYPE_DELIM_TOKEN,
	CSS_TOKEN_TYPE_NUMBER_TOKEN,
	CSS_TOKEN_TYPE_PERCENTAGE_TOKEN,
	CSS_TOKEN_TYPE_DIMENSION_TOKEN,
	CSS_TOKEN_TYPE_WHITESPACE_TOKEN,
	CSS_TOKEN_TYPE_CDO_TOKEN,
	CSS_TOKEN_TYPE_CDC_TOKEN,
	CSS_TOKEN_TYPE_COLON_TOKEN,
	CSS_TOKEN_TYPE_SEMICOLON_TOKEN,
	CSS_TOKEN_TYPE_COMMA_TOKEN,
	CSS_TOKEN_TYPE_OPEN_SQUARE_BRACKET,
	CSS_TOKEN_TYPE_CLOSE_SQUARE_BRACKET,
	CSS_TOKEN_TYPE_OPEN_CURLY_BRACKET,
	CSS_TOKEN_TYPE_CLOSE_CURLY_BRACKET,
	CSS_TOKEN_TYPE_OPEN_BRACE,
	CSS_TOKEN_TYPE_CLOSE_BRACE
};

struct CSSToken
{
	CSS_TOKEN_TYPE type = CSS_TOKEN_TYPE_UNKNOW_TOKEN;
	UINT uStructSize = 0;
};

#define INIT_CSS_TOKEN(cls, typ, ...) cls(){type=typ;uStructSize=sizeof(cls); __VA_ARGS__}

struct CSSTokenEof: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenEof, CSS_TOKEN_TYPE_EOF_TOKEN);
};

struct CSSTokenIdent: public CSSToken
{
	const char *szValue = NULL;

	INIT_CSS_TOKEN(CSSTokenIdent, CSS_TOKEN_TYPE_IDENT_TOKEN);
};

struct CSSTokenFunction: public CSSToken
{
	const char *szValue = NULL;

	INIT_CSS_TOKEN(CSSTokenFunction, CSS_TOKEN_TYPE_FUNCTION_TOKEN);
};

struct CSSTokenAtKeyword: public CSSToken
{
	const char *szValue = NULL;

	INIT_CSS_TOKEN(CSSTokenAtKeyword, CSS_TOKEN_TYPE_AT_KEYWORD_TOKEN);
};

struct CSSTokenHash: public CSSToken
{
	const char *szValue = NULL;
	bool isId = false; // unrestricted otherwise

	INIT_CSS_TOKEN(CSSTokenHash, CSS_TOKEN_TYPE_HASH_TOKEN);
};

struct CSSTokenString: public CSSToken
{
	const char *szValue = NULL;

	INIT_CSS_TOKEN(CSSTokenString, CSS_TOKEN_TYPE_STRING_TOKEN);
};

struct CSSTokenBadString: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenBadString, CSS_TOKEN_TYPE_BAD_STRING_TOKEN);
};

struct CSSTokenUrl: public CSSToken
{
	const char *szValue = NULL;

	INIT_CSS_TOKEN(CSSTokenUrl, CSS_TOKEN_TYPE_URL_TOKEN);
};

struct CSSTokenBadUrl: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenBadUrl, CSS_TOKEN_TYPE_BAD_URL_TOKEN);
};

struct CSSTokenDelim: public CSSToken
{
	UINT uCodePoint = 0;

	INIT_CSS_TOKEN(CSSTokenDelim, CSS_TOKEN_TYPE_DELIM_TOKEN);
};

struct CSSTokenNumber: public CSSToken
{
	bool isNumber = false; // otherwise integer
	union
	{
		int iValue; // integer
		float fValue; // number
	};

	INIT_CSS_TOKEN(CSSTokenNumber, CSS_TOKEN_TYPE_NUMBER_TOKEN);
};

struct CSSTokenPercentage: public CSSToken
{
	float fValue;

	INIT_CSS_TOKEN(CSSTokenPercentage, CSS_TOKEN_TYPE_PERCENTAGE_TOKEN);
};

struct CSSTokenDimension: public CSSTokenNumber
{
	XCSS_VAR_UNIT unit = XCSS_VAR_UNIT_NONE;

	INIT_CSS_TOKEN(CSSTokenDimension, CSS_TOKEN_TYPE_DIMENSION_TOKEN);
};

struct CSSTokenWhitespace: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenWhitespace, CSS_TOKEN_TYPE_WHITESPACE_TOKEN);
};

struct CSSTokenCDO: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenCDO, CSS_TOKEN_TYPE_CDO_TOKEN);
};

struct CSSTokenCDC: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenCDC, CSS_TOKEN_TYPE_CDC_TOKEN);
};

struct CSSTokenColon: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenColon, CSS_TOKEN_TYPE_COLON_TOKEN);
};

struct CSSTokenSemicolon: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenSemicolon, CSS_TOKEN_TYPE_SEMICOLON_TOKEN);
};

struct CSSTokenComma: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenComma, CSS_TOKEN_TYPE_COMMA_TOKEN);
};

struct CSSTokenOpenSquareBracket: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenOpenSquareBracket, CSS_TOKEN_TYPE_OPEN_SQUARE_BRACKET);
};

struct CSSTokenCloseSquareBracket: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenCloseSquareBracket, CSS_TOKEN_TYPE_CLOSE_SQUARE_BRACKET);
};

struct CSSTokenOpenCurlyBracket: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenOpenCurlyBracket, CSS_TOKEN_TYPE_OPEN_CURLY_BRACKET);
};

struct CSSTokenCloseCurlyBracket: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenCloseCurlyBracket, CSS_TOKEN_TYPE_CLOSE_CURLY_BRACKET);
};

struct CSSTokenOpenBrace: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenOpenBrace, CSS_TOKEN_TYPE_OPEN_BRACE);
};

struct CSSTokenCloseBrace: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenCloseBrace, CSS_TOKEN_TYPE_CLOSE_BRACE);
};

#undef INIT_CSS_TOKEN

struct CSSLexeme
{
	CSS_TOKEN_TYPE type;
	const char *szStart;
	UINT uLen;
};

class CCSSLexer
{
public:
	CCSSLexer();

	void read(const char *szCode);

	UINT getCount();

	UINT getCurrPos();
	void setCurrPos(UINT uPos);

	const CSSLexeme* getLexeme(UINT id);
	const CSSLexeme* getLexemeCurr();
	const CSSLexeme* getLexemeNext();
	const CSSLexeme* getLexemePrev();

private:
	UINT readChar(StreamReader &reader);
private:
	Array<CSSLexeme> m_aLexemes;
	UINT m_uCurrLexeme = 0;

};

#endif
