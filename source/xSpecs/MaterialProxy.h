#ifndef __MATERIALPROXY_H
#define __MATERIALPROXY_H

#include <xcommon/IXMaterialProxy.h>
#include <xcommon/IFileSystem.h>

class CMaterialProxy: public IXMaterialProxy
{
public:
	CMaterialProxy(IFileSystem *pFileSystem);

	const char* XMETHODCALLTYPE getDescription() const override;

	bool XMETHODCALLTYPE resolveName(const char *szName, char *szOutput, UINT *puBufSize, bool isForce = false) override;
	
protected:
	IFileSystem *m_pFileSystem;

};

#endif
