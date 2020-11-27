#ifndef __IXMATERIALLOADER_H
#define __IXMATERIALLOADER_H

#include <xcommon/IXLoaderBase.h>
#include <core/IFile.h>
#include <mtrl/IXMaterial.h>

// {CF5F2740-111D-439F-BB90-EA4EFFAD66D3}
#define IXMATERIALLOADER_GUID DEFINE_XGUID(0xcf5f2740, 0x111d, 0x439f, 0xbb, 0x90, 0xea, 0x4e, 0xff, 0xad, 0x66, 0xd3)

class IXMaterialLoader: public IXLoaderBase
{
public:
	virtual bool XMETHODCALLTYPE canSave(UINT uIndex) const = 0;

	virtual bool XMETHODCALLTYPE open(const char *szFileName, const char *szArg, bool forSave = false) = 0;
	virtual bool XMETHODCALLTYPE load(IXMaterial *pMaterial) = 0;
	virtual bool XMETHODCALLTYPE save(IXMaterial *pMaterial) = 0;
	virtual void XMETHODCALLTYPE close() = 0;
};

#endif
