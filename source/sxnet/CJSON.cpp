
#include "CJSON.h"
#include <ctype.h>

CJSON::CJSON(const WCHAR * _str):
	m_pString(NULL),
	m_pArr(NULL),
	m_pObj(NULL),
	m_type(NUL)
{
	const WCHAR * str = _str;
	LoadValue(&str);
}

CJSON::CJSON(const WCHAR ** str):
m_pString(NULL),
m_pArr(NULL),
m_pObj(NULL),
m_type(NUL)
{
	LoadValue(str);
}

void CJSON::LoadValue(const WCHAR ** str)
{
	while(**str && iswspace(**str))
	{
		*str++;
	}
	switch(**str)
	{
	case L'"':
		LoadString(str);
		break;
	case L'{':
		LoadObj(str);
		break;
	case L'[':
		LoadArr(str);
		break;
	case L'-':
	case L'0':
	case L'1':
	case L'2':
	case L'3':
	case L'4':
	case L'5':
	case L'6':
	case L'7':
	case L'8':
	case L'9':
		LoadNum(str);
		break;
	case L't':
	case L'f':
		LoadBool(str);
		break;
	case L'n':
		LoadNull(str);
		break;
	}
}

void CJSON::LoadString(const WCHAR ** prm)
{
	m_type = STRING;
	m_pString = new StringW;
	++*prm;
	while(**prm && **prm != L'"')
	{
		if(**prm == L'\\')
		{
			++*prm;
			switch(**prm)
			{
			case L'"':
			case L'\\':
			case L'/':
				*m_pString += **prm;
				break;
			case L'b':
				*m_pString += L'\b';
				break;
			case L'f':
				*m_pString += L'\f';
				break;
			case L'n':
				*m_pString += L'\n';
				break;
			case L'r':
				*m_pString += L'\r';
				break;
			case L't':
				*m_pString += L'\t';
				break;
			case L'u':
				{
					WCHAR code = 0;
					for(int i = 0; i < 4; i++)
					{
						code *= 16;
						++*prm;
						if(**prm >= L'0' && **prm <= L'9')
						{
							code += **prm - L'0';
						}
						if(**prm >= L'a' && **prm <= L'f')
						{
							code += **prm - L'a' + 10;
						}
					}
					*m_pString += code;
				}
				break;
			}
		}
		else
		{
			*m_pString += **prm;
		}
		++*prm;
	}
	++*prm;
}
void CJSON::LoadNum(const WCHAR ** prm)
{
	m_type = NUMBER;
	m_iVal = 0;
	m_fVal = 0;

	bool bNeg = false;

	if(**prm == L'-')
	{
		bNeg = true;
		++*prm;
	}

	while(**prm >= L'0' && **prm <= L'9')
	{
		m_iVal *= 10;
		m_iVal += **prm - L'0';
		++*prm;
	}

	if(**prm == '.')
	{
		int digc = 0;
		while(**prm >= L'0' && **prm <= L'9')
		{
			m_fVal *= 10.0f;
			m_fVal += (float)(**prm - L'0');
			++*prm;
			++digc;
		}
		while(digc--)
		{
			m_fVal /= 10.0f;
		}
	}
	if(**prm == 'e' || **prm == 'E')
	{
		int exp = 0;
		bool bExpNeg = false;
		if(**prm == L'+')
		{
			++*prm;
		}
		if(**prm == L'-')
		{
			bExpNeg = true;
			++*prm;
		}
		while(**prm >= L'0' && **prm <= L'9')
		{
			exp *= 10;
			exp += **prm - L'0';
			++*prm;
		}
		m_fVal += (float)m_iVal;
		while(exp--)
		{
			if(bExpNeg)
			{
				m_fVal /= 10.0f;
			}
			else
			{
				m_fVal *= 10.0f;
			}
		}
		
		m_iVal = (int)m_fVal;
		m_fVal -= (float)m_iVal;
	}

	if(bNeg)
	{
		m_iVal *= -1;
		m_fVal *= -1.0f;
	}
}
void CJSON::LoadArr(const WCHAR ** prm)
{
	m_type = ARRAY;
	m_pArr = new Array<JSON*>;
	++*prm;
	while(**prm != L']')
	{
		while(**prm && (iswspace(**prm) || **prm == ','))
		{
			++*prm;
		}
		JSON * json = new CJSON(prm);
		m_pArr->push_back(json);
	}
	++*prm;
}
void CJSON::LoadObj(const WCHAR ** prm)
{
	m_type = OBJECT;
	m_pObj = new AssotiativeArray<StringW, JSON*>;
	++*prm;
	while(**prm != L'}')
	{
		while(**prm && (iswspace(**prm) || **prm == ','))
		{
			++*prm;
		}
		JSON * key = new CJSON(prm);
		while(**prm && (iswspace(**prm) || **prm == ':'))
		{
			++*prm;
		}

		JSON * json = new CJSON(prm);
		m_pObj[0][key->GetString()] = json;
		delete key;
	}
	++*prm;
}
void CJSON::LoadBool(const WCHAR ** prm)
{
	m_type = BOOLEAN;
		
	m_bVal = **prm == L't';

	for(int i = 0, l = m_bVal ? 4 : 5; i < l; ++i)
	{
		++*prm;
	}
}
void CJSON::LoadNull(const WCHAR ** prm)
{
	m_type = NUL;
	for(int i = 0; i < 4; ++i)
	{
		++*prm;
	}
}

CJSON::~CJSON()
{
	if(m_pString)
	{
		delete m_pString;
	}
	if(m_pArr)
	{
		for(UINT i = 0, l = m_pArr->size(); i < l; ++i)
		{
			if(m_pArr[0][i])
			{
				delete m_pArr[0][i];
			}
		}
		delete m_pArr;
	}
	if(m_pObj)
	{
		for(AssotiativeArray<StringW, JSON*>::Iterator i = m_pObj->begin(); i; i++)
		{
			if(*i.second)
			{
				delete *i.second;
			}
		}
		delete m_pObj;
	}
}


CJSON::TYPE CJSON::GetType() const
{
	return(m_type);
}
int CJSON::GetInt() const
{
	return(m_iVal);
}
float CJSON::GetFloat() const
{
	return((float)m_iVal + m_fVal);
}
BOOL CJSON::GetBool() const
{
	return(m_bVal);
}
const Array<JSON*> & CJSON::GetArray() const
{
	return(*m_pArr);
}
const AssotiativeArray<StringW, JSON*> & CJSON::GetObject() const
{
	return(*m_pObj);
}
const StringW & CJSON::GetString() const
{
	return(*m_pString);
}