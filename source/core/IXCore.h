#ifndef __IXCORE_H
#define __IXCORE_H

#include <gdefines.h>
#include <core/IFileSystem.h>

class IPluginManager;

class IXCore: public IXUnknown
{
public:
	virtual IPluginManager *getPluginManager() = 0;
	virtual IFileSystem *getFileSystem() = 0;
};

#endif
