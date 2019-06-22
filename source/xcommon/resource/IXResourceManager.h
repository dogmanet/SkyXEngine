#ifndef __IXRESOURCEMANAGER_H
#define __IXRESOURCEMANAGER_H

#include <gdefines.h>
#include "IXResourceModel.h"


struct XFormatName
{
	const char *szDescription;
	const char *szExtension;
};

// Implemented in core
class IXResourceManager: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE getModel(const char *szName, IXResourceModel **ppOut, bool bForceReload = false) = 0;
	virtual bool XMETHODCALLTYPE getModelStatic(const char *szName, IXResourceModelStatic **ppOut, bool bForceReload = false) = 0;
	virtual bool XMETHODCALLTYPE getModelAnimated(const char *szName, IXResourceModelAnimated **ppOut, bool bForceReload = false) = 0;
	
	virtual UINT XMETHODCALLTYPE getModelSupportedFormats() = 0;
	virtual const XFormatName* XMETHODCALLTYPE getModelSupportedFormat(UINT uIndex) = 0;

	// void getModelInfo();
	// void getTextureInfo();
	// void getTexture();
};

#endif
