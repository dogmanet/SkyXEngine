#include <xcommon/IXPlugin.h>
#include "ITest.h"

class CTest: public ITest
{
public:
	void Release()
	{
		--m_uRefCount;
		if(!m_uRefCount)
		{
			delete this;
		}
	}

	void Foo()
	{
		MessageBox(NULL, "Foo called!", "", MB_OK);
	}
};

class CDSEPlugin: public IXPlugin
{
public:
	void Release()
	{
		--m_uRefCount;
		if(!m_uRefCount)
		{
			delete this;
		}
	}

	void startup(IXCore *pCore)
	{
	}

	UINT getInterfaceCount()
	{
		return(1);
	}
	const XGUID *getInterfaceGUID(UINT id)
	{
		static XGUID s_guid;
		switch(id)
		{
		case 0:
			s_guid = ITEST_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown *getInterface(const XGUID &guid)
	{
		if(guid == ITEST_GUID)
		{
			return(new CTest());
		}
		return(NULL);
	}
};

X_PLUGIN_API IXPlugin *X_PLUGIN_ENTRYPOINT()
{
	return(new CDSEPlugin());
}
