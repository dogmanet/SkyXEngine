#include "LogicExpression.h"

CLogicExpression::CLogicExpression()
{}
CLogicExpression::~CLogicExpression()
{
	free(m_szExpression);
}

bool CLogicExpression::setExpression(const char *szExpression)
{
	free(m_szExpression);
	m_szExpression = strdup(szExpression);
	m_aVariables.clearFast();
	m_aTokens.clearFast();
	m_pRootNode = NULL;
	m_szError = false;

	const char *szTokenStart = NULL;
	char ch, cn = 0;
	for(int i = 0, l = strlen(m_szExpression); i <= l; ++i)
	{
		ch = m_szExpression[i];
		if(!ch)
		{
			ch = ' ';
		}
		if(i < l)
		{
			cn = m_szExpression[i + 1];
		}

		if(
			(ch >= 'a' && ch <= 'z')
			|| (ch >= 'A' && ch <= 'Z')
			|| (ch >= '0' && ch <= '9')
			|| ch == '_')
		{
			if(!szTokenStart)
			{
				szTokenStart = m_szExpression + i;
			}
			continue;
		}
		else if(szTokenStart)
		{
			m_szExpression[i] = 0;
			if(!strcmp(szTokenStart, "true") || (szTokenStart[0] >= '1' && szTokenStart[0] <= '9'))
			{
				m_aTokens.push_back({TOKEN_TRUE});
			}
			else if(!strcmp(szTokenStart, "false") || szTokenStart[0] == '0')
			{
				m_aTokens.push_back({TOKEN_FALSE});
			}
			else
			{
				int idx = m_aVariables.indexOf(szTokenStart, [](const Param &a, const char *b){
					return(!fstrcmp(a.szName, b));
				});

				if(idx < 0)
				{
					idx = m_aVariables.size();
					m_aVariables.push_back({szTokenStart});
				}
				m_aTokens.push_back({TOKEN_IDENT, idx});
			}
			szTokenStart = NULL;
		}
		if(isspace((unsigned char)ch))
		{
			continue;
		}

		switch(ch)
		{
		case '(':
			m_aTokens.push_back({TOKEN_LBRACE});
			break;
		case ')':
			m_aTokens.push_back({TOKEN_RBRACE});
			break;
		case '!':
			m_aTokens.push_back({TOKEN_NOT});
			break;
		case '&':
			if(cn == '&')
			{
				m_aTokens.push_back({TOKEN_AND});
				++i;
			}
			else
			{
				m_szError = "Unexpected token near '&'";
				return(false);
			}
			break;
		case '|':
			if(cn == '|')
			{
				m_aTokens.push_back({TOKEN_OR});
				++i;
			}
			else
			{
				m_szError = "Unexpected token near '|'";
				return(false);
			}
			break;
		default:
			m_szError = "Unexpected token";
			return(false);
		}
	}

	Array<Token*> aStack;
	Array<Token*> aOut;
	for(UINT i = 0, l = m_aTokens.size(); i < l; ++i)
	{
		Token *pCur = &m_aTokens[i];

		if(pCur->type == TOKEN_FALSE || pCur->type == TOKEN_TRUE || pCur->type == TOKEN_IDENT)
		{
			aOut.push_back(pCur);
		}
		else
		{
			if(aStack.size())
			{
				if(getPriority(aStack[aStack.size() - 1]->type) >= getPriority(pCur->type))
				{
					while(aStack.size())
					{
						if(aStack[aStack.size() - 1]->type == TOKEN_LBRACE)
						{
							if(pCur->type == TOKEN_RBRACE)
							{
								aStack.erase(aStack.size() - 1);
							}
							break;
						}
						else
						{
							aOut.push_back(aStack[aStack.size() - 1]);
							aStack.erase(aStack.size() - 1);
						}
					}
				}
			}

			if(pCur->type != TOKEN_RBRACE)
			{
				aStack.push_back(pCur);
			}
		}
	}

	while(aStack.size())
	{
		aOut.push_back(aStack[aStack.size() - 1]);

		if(aStack[aStack.size() - 1]->type == TOKEN_LBRACE)
		{
			m_szError = "Mismatched braces";
			return(false);
		}
		aStack.erase(aStack.size() - 1);
	}
	
	for(UINT i = 0, l = aOut.size(); i < l; ++i)
	{
		Token *pCur = aOut[i];
		if(pCur->type == TOKEN_FALSE || pCur->type == TOKEN_TRUE || pCur->type == TOKEN_IDENT)
		{
			aStack.push_back(pCur);
		}
		else if(pCur->type == TOKEN_NOT)
		{
			if(aStack.size() < 1)
			{
				m_szError = "Invalid expression";
				return(false);
			}

			pCur->pLeft = aStack[aStack.size() - 1];
			aStack[aStack.size() - 1] = pCur;
		}
		else
		{
			if(aStack.size() < 2)
			{
				m_szError = "Invalid expression";
				return(false);
			}

			pCur->pLeft = aStack[aStack.size() - 2];
			pCur->pRight = aStack[aStack.size() - 1];
			aStack[aStack.size() - 2] = pCur;
			aStack.erase(aStack.size() - 1);
		}
	}

	if(aStack.size() != 1)
	{
		m_szError = "Invalid expression";
		return(false);
	}

	m_pRootNode = aStack[0];

	return(true);
}

const char* CLogicExpression::getError()
{
	return(m_szError);
}

bool CLogicExpression::evaluate()
{
	if(!m_pRootNode)
	{
		return(false);
	}
	return(evalNode(m_pRootNode));
}
void CLogicExpression::resetParams()
{
	for(UINT i = 0, l = m_aVariables.size(); i < l; ++i)
	{
		m_aVariables[i].bValue = false;
	}
}
void CLogicExpression::setParam(const char *szName, bool bValue)
{
	int idx = m_aVariables.indexOf(szName, [](const Param &a, const char *b){
		return(!fstrcmp(a.szName, b));
	});

	if(idx >= 0)
	{
		m_aVariables[idx].bValue = bValue;
	}
}

int CLogicExpression::getPriority(TOKEN token)
{
	switch(token)
	{
	case CLogicExpression::TOKEN_OR:
		return(7);
	case CLogicExpression::TOKEN_AND:
		return(8);
	case CLogicExpression::TOKEN_LBRACE:
		return(11);
	case CLogicExpression::TOKEN_NOT:
		return(10);
	case CLogicExpression::TOKEN_RBRACE:
		return(0);
	}
	return(0);
}

bool CLogicExpression::evalNode(Token *pToken)
{
	switch(pToken->type)
	{
	case TOKEN_TRUE:
		return(true);
	case TOKEN_FALSE:
		return(false);
	case TOKEN_IDENT:
		return(m_aVariables[pToken->idVar].bValue);
	case TOKEN_NOT:
		return(!evalNode(pToken->pLeft));
	case TOKEN_OR:
		return(evalNode(pToken->pLeft) || evalNode(pToken->pRight));
	case TOKEN_AND:
		return(evalNode(pToken->pLeft) && evalNode(pToken->pRight));
	}
	return(false);
}
