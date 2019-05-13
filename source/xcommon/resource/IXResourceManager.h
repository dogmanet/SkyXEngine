#ifndef __IXRESOURCEMANAGER_H
#define __IXRESOURCEMANAGER_H

#include <gdefines.h>
#include "IXResourceModel.h"

// Implemented in core
class IXResourceManager: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE getModel(const char *szName, const IXResourceModel **ppOut, bool bForceReload = false) = 0;
	virtual bool XMETHODCALLTYPE getModelStatic(const char *szName, const IXResourceModelStatic **ppOut, bool bForceReload = false) = 0;
	virtual bool XMETHODCALLTYPE getModelAnimated(const char *szName, const IXResourceModelAnimated **ppOut, bool bForceReload = false) = 0;

	// void getModelInfo();
	// void getTextureInfo();
	// void getTexture();
};

#endif
