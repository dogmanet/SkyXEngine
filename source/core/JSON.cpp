#include "JSON.h"


bool XMETHODCALLTYPE CJSON::parse(const char *szString, IXJSONItem **ppOut, void *pReserved) const
{
	const char *str = szString;
	if(!Parse(&str, ppOut))
	{
		return(false);
	}
	if(*str)
	{
		mem_release(*ppOut);
		return(false);
	}
	return(true);
}

bool CJSON::Parse(const char **str, IXJSONItem **ppOut)
{
	while(**str && isspace((byte)**str))
	{
		++*str;
	}

	if(**str == '{')
	{
		CJSONObject *o = new CJSONObject();
		if(!o->load(str))
		{
			mem_release(o);
			return(false);
		}
		*ppOut = o;
	}
	else if(**str == '[')
	{
		CJSONArray *o = new CJSONArray();
		if(!o->load(str))
		{
			mem_release(o);
			return(false);
		}
		*ppOut = o;
	}
	else
	{
		CJSONValue *o = new CJSONValue();
		if(!o->load(str))
		{
			mem_release(o);
			return(false);
		}
		*ppOut = o;
	}
	return(true);
}

//##########################################################################

CJSONArray::~CJSONArray()
{
	for(UINT i = 0, l = m_aItems.size(); i < l; ++i)
	{
		mem_release(m_aItems[i]);
	}
}

UINT XMETHODCALLTYPE CJSONArray::size() const
{
	return(m_aItems.size());
}
IXJSONItem* XMETHODCALLTYPE CJSONArray::at(UINT idx) const
{
	if(idx < size())
	{
		return(m_aItems[idx]);
	}
	return(false);
}

bool CJSONArray::loadArr(const char **str)
{
	m_type = XJI_ARRAY;
	++*str;
	IXJSONItem *pItem = NULL;
	while(**str && **str != ']')
	{
		/*while(**str && isspace(**str))
		{
			++*str;
		}
		*/
		if(!CJSON::Parse(str, &pItem))
		{
			return(false);
		}

		m_aItems.push_back(pItem);

		if(**str == ',')
		{
			++*str;
		}
		else if(**str != ']')
		{
			return(false);
		}
	}

	if(**str != ']')
	{
		return(false);
	}
	++*str;

	return(true);
}

//##########################################################################

CJSONObject::~CJSONObject()
{
	for(UINT i = 0, l = m_aPairs.size(); i < l; ++i)
	{
		mem_release(m_aPairs[i].pVal);
	}
}

UINT XMETHODCALLTYPE CJSONObject::size() const
{
	return(m_aPairs.size());
}
IXJSONItem* XMETHODCALLTYPE CJSONObject::at(UINT idx) const
{
	if(idx < size())
	{
		return(m_aPairs[idx].pVal);
	}
	return(false);
}

const char* XMETHODCALLTYPE CJSONObject::getKey(UINT idx) const
{
	if(idx < size())
	{
		return(m_aPairs[idx].sKey.c_str());
	}
	return(false);
}

IXJSONItem* XMETHODCALLTYPE CJSONObject::getItem(const char *szKey) const
{
	//! TODO: Add accelerator!
	for(UINT i = 0, l = m_aPairs.size(); i < l; ++i)
	{
		const KeyValue &kv = m_aPairs[i];
		if(!strcmp(kv.sKey.c_str(), szKey))
		{
			return(kv.pVal);
		}
	}
	return(NULL);
}

bool CJSONObject::loadObj(const char **str)
{
	m_type = XJI_OBJECT;
	++*str;

	CJSONValue key;
	IXJSONItem *pItem = NULL;

	while(**str && **str != '}')
	{
		if(!key.load(str) || key.getType() != XJI_STRING)
		{
			return(false);
		}
		if(**str != ':')
		{
			return(false);
		}
		++*str;

		if(!CJSON::Parse(str, &pItem))
		{
			return(false);
		}

		m_aPairs.push_back({key.getString(), pItem});

		if(**str == ',')
		{
			++*str;
		}
		else if(**str != '}')
		{
			return(false);
		}
	}
	if(**str != '}')
	{
		return(false);
	}
	++*str;
	return(true);
}
