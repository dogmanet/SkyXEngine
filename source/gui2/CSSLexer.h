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
	CSS_TOKEN_TYPE_UNKNOWN,
	CSS_TOKEN_TYPE_EOF,
	CSS_TOKEN_TYPE_IDENT,
	CSS_TOKEN_TYPE_FUNCTION,
	CSS_TOKEN_TYPE_AT_KEYWORD,
	CSS_TOKEN_TYPE_HASH,
	CSS_TOKEN_TYPE_STRING,
	CSS_TOKEN_TYPE_BAD_STRING,
	CSS_TOKEN_TYPE_URL,
	CSS_TOKEN_TYPE_BAD_URL,
	CSS_TOKEN_TYPE_DELIM,
	CSS_TOKEN_TYPE_NUMBER,
	CSS_TOKEN_TYPE_PERCENTAGE,
	CSS_TOKEN_TYPE_DIMENSION,
	CSS_TOKEN_TYPE_WHITESPACE,
	CSS_TOKEN_TYPE_CDO,
	CSS_TOKEN_TYPE_CDC,
	CSS_TOKEN_TYPE_COLON,
	CSS_TOKEN_TYPE_SEMICOLON,
	CSS_TOKEN_TYPE_COMMA,
	CSS_TOKEN_TYPE_OPEN_SQUARE_BRACKET, // [
	CSS_TOKEN_TYPE_CLOSE_SQUARE_BRACKET, // ]
	CSS_TOKEN_TYPE_OPEN_CURLY_BRACKET, // (
	CSS_TOKEN_TYPE_CLOSE_CURLY_BRACKET, // )
	CSS_TOKEN_TYPE_OPEN_BRACE, // {
	CSS_TOKEN_TYPE_CLOSE_BRACE // }
};

struct CSSToken
{
	CSS_TOKEN_TYPE type = CSS_TOKEN_TYPE_UNKNOWN;
	UINT uStructSize = 0;
};

#define INIT_CSS_TOKEN(cls, typ, ...) cls(){type=typ;uStructSize=sizeof(cls); __VA_ARGS__}

struct CSSTokenEof: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenEof, CSS_TOKEN_TYPE_EOF);
};

struct CSSTokenIdent: public CSSToken
{
	const char *szValue = NULL;

	INIT_CSS_TOKEN(CSSTokenIdent, CSS_TOKEN_TYPE_IDENT);
};

struct CSSTokenFunction: public CSSToken
{
	const char *szValue = NULL;

	INIT_CSS_TOKEN(CSSTokenFunction, CSS_TOKEN_TYPE_FUNCTION);
};

struct CSSTokenAtKeyword: public CSSToken
{
	const char *szValue = NULL;

	INIT_CSS_TOKEN(CSSTokenAtKeyword, CSS_TOKEN_TYPE_AT_KEYWORD);
};

struct CSSTokenHash: public CSSToken
{
	const char *szValue = NULL;
	bool isId = false; // unrestricted otherwise

	INIT_CSS_TOKEN(CSSTokenHash, CSS_TOKEN_TYPE_HASH);
};

struct CSSTokenString: public CSSToken
{
	const char *szValue = NULL;

	INIT_CSS_TOKEN(CSSTokenString, CSS_TOKEN_TYPE_STRING);
};

struct CSSTokenBadString: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenBadString, CSS_TOKEN_TYPE_BAD_STRING);
};

struct CSSTokenUrl: public CSSToken
{
	const char *szValue = NULL;

	INIT_CSS_TOKEN(CSSTokenUrl, CSS_TOKEN_TYPE_URL);
};

struct CSSTokenBadUrl: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenBadUrl, CSS_TOKEN_TYPE_BAD_URL);
};

struct CSSTokenDelim: public CSSToken
{
	UINT uCodePoint = 0;

	INIT_CSS_TOKEN(CSSTokenDelim, CSS_TOKEN_TYPE_DELIM);
};

struct CSSTokenNumber: public CSSToken
{
	bool isNumber = false; // otherwise integer
	union
	{
		int iValue; // integer
		float fValue; // number
	};

	INIT_CSS_TOKEN(CSSTokenNumber, CSS_TOKEN_TYPE_NUMBER);
};

struct CSSTokenPercentage: public CSSToken
{
	float fValue;

	INIT_CSS_TOKEN(CSSTokenPercentage, CSS_TOKEN_TYPE_PERCENTAGE);
};

struct CSSTokenDimension: public CSSTokenNumber
{
	XCSS_VAR_UNIT unit = XCSS_VAR_UNIT_NONE;

	INIT_CSS_TOKEN(CSSTokenDimension, CSS_TOKEN_TYPE_DIMENSION);
};

struct CSSTokenWhitespace: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenWhitespace, CSS_TOKEN_TYPE_WHITESPACE);
};

struct CSSTokenCDO: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenCDO, CSS_TOKEN_TYPE_CDO);
};

struct CSSTokenCDC: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenCDC, CSS_TOKEN_TYPE_CDC);
};

struct CSSTokenColon: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenColon, CSS_TOKEN_TYPE_COLON);
};

struct CSSTokenSemicolon: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenSemicolon, CSS_TOKEN_TYPE_SEMICOLON);
};

struct CSSTokenComma: public CSSToken
{
	INIT_CSS_TOKEN(CSSTokenComma, CSS_TOKEN_TYPE_COMMA);
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


enum CSS_LEXEME_TYPE
{
	CSS_LEXEME_TYPE_KEYWORD,
	CSS_LEXEME_TYPE_AT_KEYWORD,
	CSS_LEXEME_TYPE_STRING,
	CSS_LEXEME_TYPE_ERROR_MARKER,
	CSS_LEXEME_TYPE_FLOAT,				
	CSS_LEXEME_TYPE_INT,
	CSS_LEXEME_TYPE_IMPORTANT, // !important

	CSS_LEXEME_TYPE_SYM_COMMA, // ,
	CSS_LEXEME_TYPE_SYM_SEMICOLON, // ;		
	CSS_LEXEME_TYPE_SYM_COLON, // :
	CSS_LEXEME_TYPE_SYM_PAAMAYIM_NEKUDOTAYIM, // ::
	CSS_LEXEME_TYPE_SYM_STAR, // *
	CSS_LEXEME_TYPE_SYM_SLASH, // /
	CSS_LEXEME_TYPE_SYM_PLUS, // +
	CSS_LEXEME_TYPE_SYM_MINUS, // -
	CSS_LEXEME_TYPE_SYM_OR, // |
	CSS_LEXEME_TYPE_SYM_OPEN_BRACE, // {
	CSS_LEXEME_TYPE_SYM_CLOSE_BRACE, // }
	CSS_LEXEME_TYPE_SYM_OPEN_CURLY_BRACKET, // (
	CSS_LEXEME_TYPE_SYM_CLOSE_CURLY_BRACKET, // )
	CSS_LEXEME_TYPE_SYM_OPEN_SQUARE_BRACKET, // [
	CSS_LEXEME_TYPE_SYM_CLOSE_SQUARE_BRACKET, // ]
	CSS_LEXEME_TYPE_SYM_DOT, // .
	CSS_LEXEME_TYPE_SYM_HASH, // #
	CSS_LEXEME_TYPE_SYM_NEXT, // >
	CSS_LEXEME_TYPE_SYM_TILDA, // ~
	CSS_LEXEME_TYPE_SYM_EQUAL, // =
	CSS_LEXEME_TYPE_SYM_DOLLAR_EQUAL, // $=
	CSS_LEXEME_TYPE_SYM_STAR_EQUAL, // *=
	CSS_LEXEME_TYPE_SYM_XOR_EQUAL, // ^=
	CSS_LEXEME_TYPE_SYM_OR_EQUAL, // |=
	CSS_LEXEME_TYPE_SYM_TILDA_EQUAL, // ~=		
	CSS_LEXEME_TYPE_SYM_AT, // @		
	CSS_LEXEME_TYPE_EXCLAMATION_MARK, // !
	CSS_LEXEME_TYPE_QUOTE_MARK, // '
	CSS_LEXEME_TYPE_QUOTATION_MARKS // "
};

struct CSSLexeme
{
	CSS_LEXEME_TYPE type;
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
	Array<CSSToken*> m_aLexemes2;
	UINT m_uCurrLexeme = 0;

};

#endif
