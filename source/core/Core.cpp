#include "Core.h"
#include <common/file_utils.h>
#include "AsyncFileReader.h"

CCore::CCore()
{
	m_pPluginManager = new CPluginManager();
}
CCore::~CCore()
{
	mem_delete(m_pPluginManager);
}

void CCore::Release()
{
	--m_uRefCount;
	assert(m_uRefCount && "You MUST NOT explicitly call Release on IXCore interface");
}

IPluginManager *CCore::getPluginManager()
{
	m_pPluginManager->AddRef();
	return(m_pPluginManager);
}
IFileSystem *CCore::getFileSystem()
{
	return(NULL);
}

IAsyncFileReader *CCore::getAsyncFileReader()
{
	return(new CAsyncFileReader(this));
}
IAsyncTaskRunner *CCore::getAsyncTaskRunner()
{
	return(NULL);
}

void CCore::loadPlugins()
{
	auto list = FileGetListFiles("../bin/plugins/*.dll");
	IXPlugin *pPlugin = NULL;
	for(UINT i = 0, l = list.size(); i < l; ++i)
	{
		printf("Loading plugin '%s'... ", list[i].c_str());
		pPlugin = m_pPluginManager->loadPlugin((String("../bin/plugins/") + list[i]).c_str());
		if(pPlugin)
		{
			printf(COLOR_GREEN "DONE!" COLOR_RESET "\n", list[i].c_str());
		}
		else
		{
			printf(COLOR_LRED "ERROR!" COLOR_RESET "\n", list[i].c_str());
		}
	}
	m_pPluginManager->invokeStartup(this);
}