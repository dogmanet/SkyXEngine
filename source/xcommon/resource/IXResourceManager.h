#ifndef __IXRESOURCEMANAGER_H
#define __IXRESOURCEMANAGER_H

#include <gdefines.h>
#include "IXResourceModel.h"
#include "IXResourceTexture.h"
#include "../IXTextureLoader.h"
#include "../IXModelLoader.h"


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

	virtual bool XMETHODCALLTYPE getModelInfo(const char *szName, XModelInfo *pInfo) = 0;

	virtual IXResourceModelStatic* XMETHODCALLTYPE newResourceModelStatic() = 0;
	virtual IXResourceModelAnimated* XMETHODCALLTYPE newResourceModelAnimated() = 0;
	virtual void XMETHODCALLTYPE addModel(const char *szName, IXResourceModel *pModel) = 0;



	virtual bool XMETHODCALLTYPE getTexture(const char *szName, IXResourceTexture **ppOut, bool bForceReload = false) = 0;
	virtual bool XMETHODCALLTYPE getTexture2D(const char *szName, IXResourceTexture2D **ppOut, bool bForceReload = false) = 0;
	virtual bool XMETHODCALLTYPE getTextureCube(const char *szName, IXResourceTextureCube **ppOut, bool bForceReload = false) = 0;

	virtual UINT XMETHODCALLTYPE getTextureSupportedFormats() = 0;
	virtual const XFormatName* XMETHODCALLTYPE getTextureSupportedFormat(UINT uIndex) = 0;

	virtual bool XMETHODCALLTYPE getTextureInfo(const char *szName, XTextureInfo *pInfo) = 0;

	virtual IXResourceTexture2D* XMETHODCALLTYPE newResourceTexture2D() = 0;
	virtual IXResourceTextureCube* XMETHODCALLTYPE newResourceTextureCube() = 0;
	virtual void XMETHODCALLTYPE addTexture(const char *szName, IXResourceTexture *pTexture) = 0;
};

#endif
