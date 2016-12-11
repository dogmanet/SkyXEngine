#ifndef CJSON_H
#define CJSON_H

#include "JSON.h"

#ifdef GetObject
#undef GetObject
#endif

#ifdef LoadString
#undef LoadString
#endif

class CJSON: public JSON
{
public:

	CJSON(const WCHAR * str);
	CJSON(const WCHAR ** str);

	~CJSON();

	TYPE GetType() const;
	int GetInt() const;
	float GetFloat() const;
	BOOL GetBool() const;
	const Array<JSON*> & GetArray() const;
	const AssotiativeArray<StringW, JSON*> & GetObject() const;
	const StringW & GetString() const;

private:
	void LoadValue(const WCHAR ** prm);

	void LoadString(const WCHAR ** prm);
	void LoadNum(const WCHAR ** prm);
	void LoadArr(const WCHAR ** prm);
	void LoadObj(const WCHAR ** prm);
	void LoadBool(const WCHAR ** prm);
	void LoadNull(const WCHAR ** prm);

	StringW * m_pString;
	int m_iVal;
	BOOL m_bVal;
	float m_fVal;
	Array<JSON*> * m_pArr;
	AssotiativeArray<StringW, JSON*> * m_pObj;

	TYPE m_type;
};

#endif
