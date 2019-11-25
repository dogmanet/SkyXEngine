#ifndef __LOGICEXPRESSION_H
#define __LOGICEXPRESSION_H

#include <gdefines.h>

class CLogicExpression
{
public:
	CLogicExpression();
	~CLogicExpression();

	bool setExpression(const char *szExpression);
	const char* getError();

	bool evaluate();
	void resetParams();
	void setParam(const char *szName, bool bValue);

protected:

	enum TOKEN
	{
		TOKEN_LBRACE,
		TOKEN_RBRACE,
		TOKEN_AND,
		TOKEN_NOT,
		TOKEN_OR,
		TOKEN_FALSE,
		TOKEN_TRUE,
		TOKEN_IDENT
	};

	struct Token
	{
		TOKEN type;
		ID idVar;
		Token *pLeft;
		Token *pRight;
	};

	struct Param
	{
		const char *szName;
		bool bValue;
	};

private:
	char *m_szExpression = NULL;
	const char *m_szError = NULL;
	Array<Token> m_aTokens;
	Array<Param> m_aVariables;
	Token *m_pRootNode;

	int getPriority(TOKEN token);
	bool evalNode(Token *pToken);
};

#endif
