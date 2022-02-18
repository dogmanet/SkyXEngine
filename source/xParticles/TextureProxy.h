#ifndef __TEXTUREPROXY_H
#define __TEXTUREPROXY_H

#include <xcommon/IXTextureProxy.h>
#include <xcommon/IFileSystem.h>

class CTextureProxyScanList final: public IXUnknownImplementation<IXTextureProxyScanList>
{
public:
	CTextureProxyScanList(IFileSystem *pFileSystem);
	~CTextureProxyScanList();

	UINT XMETHODCALLTYPE getCount() const override;
	const char* XMETHODCALLTYPE getItem(UINT uIdx, const char **pszFileName = NULL) const override;

private:
	struct Item
	{
		String sName;
		String sFile;
	};
	Array<Item> m_aItems;
	IFileSystem *m_pFileSystem;
};

class CTextureProxy: public IXUnknownImplementation<IXTextureProxy>
{
public:
	CTextureProxy(IFileSystem *pFileSystem);

	UINT XMETHODCALLTYPE getVersion() override;

	const char* XMETHODCALLTYPE getDescription() const override;

	bool XMETHODCALLTYPE resolveName(const char *szName, char *szOutput, UINT *puBufSize, bool *pbWantLoad = NULL) override;
	
	bool XMETHODCALLTYPE loadTexture(const char *szName, IXResourceTexture **ppOut) override;

	bool XMETHODCALLTYPE scanForTextures(IXTextureProxyScanList **ppOut) override;

protected:
	IFileSystem *m_pFileSystem;

};

#endif
