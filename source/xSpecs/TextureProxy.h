#ifndef __TEXTUREPROXY_H
#define __TEXTUREPROXY_H

#include <xcommon/IXTextureProxy.h>
#include <xcommon/IFileSystem.h>

class CTextureProxy: public IXTextureProxy
{
public:
	CTextureProxy(IFileSystem *pFileSystem);

	UINT XMETHODCALLTYPE getVersion() override;

	const char* XMETHODCALLTYPE getDescription() const override;

	bool XMETHODCALLTYPE resolveName(const char *szName, char *szOutput, UINT *puBufSize) override;
	
protected:
	IFileSystem *m_pFileSystem;

};

#endif
