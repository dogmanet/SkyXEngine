#ifndef __TEXTURELOADER_H
#define __TEXTURELOADER_H

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

class CTextureProxy2: public IXTextureProxy
{
public:
	CTextureProxy2(IFileSystem *pFileSystem);

	UINT XMETHODCALLTYPE getVersion() override;

	const char* XMETHODCALLTYPE getDescription() const override;

	bool XMETHODCALLTYPE resolveName(const char *szName, char *szOutput, UINT *puBufSize) override;

protected:
	IFileSystem *m_pFileSystem;

};

#endif
