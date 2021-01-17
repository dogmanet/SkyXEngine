#include "CSSLexer.h"

CCSSLexer::CCSSLexer()
{
}

void CCSSLexer::read(const char *szCode)
{
	m_aLexemes.clearFast();

	StreamReader reader(szCode);

	UINT ch = 0;
	UINT chNext = 1;
	UINT chPrev = 0;
	
	bool isComment = false;
	
	while(chNext)
	{
		if(!ch)
		{
			ch = readChar(reader);
		}
		else
		{
			chPrev = ch;
			ch = chNext;
		}
 		chNext = readChar(reader);

		if(isComment)
		{
			if(chPrev == '*' && ch == '/')
			{
				isComment = false;
			}
			continue;
		}
		else
		{
			if(ch == '/' && chNext == '*')
			{
				isComment = true;
				continue;
			}
		}

		/*
		CSS_LEXEME_TYPE type;
		const char *szStart;
		UINT uLen;
		*/
		
		switch(ch)
		{
		case ':':
			if(chNext == ':')
			{
				m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_PAAMAYIM_NEKUDOTAYIM, "::", 2});
			}
			else
			{
				m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_COLON, ":", 1});
			}
			break;
		case '$':
			if(chNext == '=')
			{
				m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_DOLLAR_EQUAL, "$=", 2});
			}
			break;
		case '*':
			if(chNext == '=')
			{
				m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_STAR_EQUAL, "*=", 2});
			}
			else
			{
				m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_STAR, "*", 1});
			}
			break;
		case '^':
			if(chNext == '=')
			{
				m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_XOR_EQUAL, "^=", 2});
			}
			break;
		case '|':
			if(chNext == '=')
			{
				m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_OR_EQUAL, "|=", 2});
			}
			else
			{
				m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_OR, "|", 1});
			}
			break;
		case '~':
			if(chNext == '=')
			{
				m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_TILDA_EQUAL, "~=", 2});
			}
			else
			{
				m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_TILDA, "~", 1});
			}
			break;

		case '=':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_EQUAL, "=", 1});
			break;
		case '(':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_OPEN_CURLY_BRACKET, "(", 1});
			break;
		case ')':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_CLOSE_CURLY_BRACKET, ")", 1});
			break;
		case '{':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_OPEN_BRACE, "{", 1});
			break;
		case '}':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_CLOSE_BRACE, "}", 1});
			break;
		case '[':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_OPEN_SQUARE_BRACKET, "[", 1});
			break;
		case ']':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_CLOSE_SQUARE_BRACKET, "]", 1});
			break;
		case ',':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_COMMA, ",", 1});
			break;
		case '.':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_DOT, ".", 1});
			break;
		case ';':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_SEMICOLON, ";", 1});
			break;
		case '#':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_HASH, "#", 1});
			break;
		case '>':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_NEXT, ">", 1});
			break;
		case '+':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_PLUS, "+", 1});
			break;
		case '-':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_MINUS, "-", 1});
			break;
		case '/':
			m_aLexemes.push_back({CSS_LEXEME_TYPE_SYM_SLASH, "/", 1});
			break;
		case '@':
			m_aLexemes.push_back({ CSS_LEXEME_TYPE_SYM_AT, "@", 1});
			break;
		case '!':
			// !important
			m_aLexemes.push_back({ CSS_LEXEME_TYPE_EXCLAMATION_MARK, "@", 1 });
			break;

		case '\'':
			m_aLexemes.push_back({ CSS_LEXEME_TYPE_QUOTE_MARK, "\'", 1 });
			break;
		case '"':
			m_aLexemes.push_back({ CSS_LEXEME_TYPE_QUOTATION_MARKS, "\"", 1 });
			break;

		default:
			;
		}
	}
}

UINT CCSSLexer::getCount()
{
	return(m_aLexemes.size());
}

UINT CCSSLexer::getCurrPos()
{
	return(m_uCurrLexeme);
}
void CCSSLexer::setCurrPos(UINT uPos)
{
	m_uCurrLexeme = uPos;
}

const CSSLexeme* CCSSLexer::getLexeme(UINT id)
{
	if(id < m_aLexemes.size())
	{
		return(&m_aLexemes[id]);
	}

	return(NULL);
}
const CSSLexeme* CCSSLexer::getLexemeCurr()
{
	return(getLexeme(m_uCurrLexeme));
}
const CSSLexeme* CCSSLexer::getLexemeNext()
{
	++m_uCurrLexeme;
	return(getLexemeCurr());
}
const CSSLexeme* CCSSLexer::getLexemePrev()
{
	if(m_uCurrLexeme > 0)
	{
		--m_uCurrLexeme;
		return(getLexemeCurr());
	}
	
	return(NULL);
}

UINT CCSSLexer::readChar(StreamReader &reader)
{
	UINT ch = reader.readChar();

	if(ch == '\r')
	{
		ch = '\n';

		size_t pos = reader.getCursor();
		UINT ch2 = reader.readChar();
		if(ch2 != '\n')
		{
			reader.setCursor(pos);
		}
	}
	else if(ch == 0x0c)
	{
		ch = '\n';
	}
	else if(ch >= 0xD800 && ch <= 0xDFFF)
	{
		ch = 0xFFFD;
	}
	// A surrogate is a code point that is in the range U+D800 to U+DFFF, inclusive. 
	// replace with U+FFFD REPLACEMENT CHARACTER. 

	return(ch);
}
