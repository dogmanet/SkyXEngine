#ifndef __TEXTUREPROXY_H
#define __TEXTUREPROXY_H

#include <xcommon/IXTextureProxy.h>
#include <xcommon/IFileSystem.h>

class CTextureProxy;
class CTextureProxyScanList final: public IXUnknownImplementation<IXTextureProxyScanList>
{
public:
	CTextureProxyScanList(CTextureProxy *pTextureProxy);
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
	CTextureProxy *m_pTextureProxy;
};

class CTextureProxy final: public IXUnknownImplementation<IXTextureProxy>
{
	friend class CTextureProxyScanList;
public:
	CTextureProxy(IFileSystem *pFileSystem);

	XIMPLEMENT_VERSION(IXTEXTUREPROXY_VERSION);

	const char* XMETHODCALLTYPE getDescription() const override;

	bool XMETHODCALLTYPE resolveName(const char *szName, char *szOutput, UINT *puBufSize, bool *pbWantLoad = NULL) override;
	
	bool XMETHODCALLTYPE loadTexture(const char *szName, IXResourceTexture **ppOut) override;

	bool XMETHODCALLTYPE scanForTextures(IXTextureProxyScanList **ppOut) override;

private:
	IFileSystem *m_pFileSystem;

	struct WADRecord
	{
		char szName[16];
	};

	struct WADFile
	{
		String sName;
		Array<WADRecord> aRecords;
	};

	Array<WADFile> m_aWADCache;

private:
	void scanForWADs();
};

#endif
