#ifndef __MATERIALPROXY_H
#define __MATERIALPROXY_H

#include <xcommon/IXMaterialProxy.h>
#include <xcommon/IFileSystem.h>

class CMaterialProxyScanList final: public IXUnknownImplementation<IXMaterialProxyScanList>
{
public:
	CMaterialProxyScanList(IFileSystem *pFileSystem);
	~CMaterialProxyScanList();

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

class CMaterialProxy final: public IXUnknownImplementation<IXMaterialProxy>
{
public:
	CMaterialProxy(IFileSystem *pFileSystem);

	const char* XMETHODCALLTYPE getDescription() const override;

	bool XMETHODCALLTYPE resolveName(const char *szName, char *szOutput, UINT *puBufSize, bool isForce = false) override;
	
	bool XMETHODCALLTYPE scanForMaterials(IXMaterialProxyScanList **ppOut) override;

protected:
	IFileSystem *m_pFileSystem;

};

#endif
