#ifndef __TEXTURELOADER_H
#define __TEXTURELOADER_H

#include <xcommon/IXTextureLoader.h>
#include <xcommon/IFileSystem.h>
#include "WADFile.h"

class CTextureLoader: public IXUnknownImplementation<IXTextureLoader>
{
public:
	CTextureLoader(IFileSystem *pFileSystem);

	XIMPLEMENT_VERSION(IXTEXTURELOADER_VERSION);

	UINT XMETHODCALLTYPE getExtCount() const override;
	const char* XMETHODCALLTYPE getExt(UINT uIndex) const override;
	const char* XMETHODCALLTYPE getExtText(UINT uIndex) const override;
	const char* XMETHODCALLTYPE getAuthor() const override;
	const char* XMETHODCALLTYPE getCopyright() const override;
	const char* XMETHODCALLTYPE getDescription() const override;

	bool XMETHODCALLTYPE open(const char *szFileName, const char *szArg) override;
	GXTEXTURE_TYPE XMETHODCALLTYPE getType() const override;
	bool XMETHODCALLTYPE loadAs2D(IXResourceTexture2D *pResource) override;
	bool XMETHODCALLTYPE loadAsCube(IXResourceTextureCube *pResource) override;
	void XMETHODCALLTYPE getInfo(XTextureInfo *pTextureInfo) override;
	void XMETHODCALLTYPE close() override;

protected:

	bool loadIndexedRGB(UINT uMip, UINT uFrame, byte *aPalette, IXResourceTexture2D *pResource);
	
	bool isTextureExist(const char *szName, WADLump *pOutLump = NULL);

protected:
	IFileSystem *m_pFileSystem;

	IFile *m_pCurrentFile = NULL;
	WADHeader m_hdr;
	WADTexture43 m_texture43;
	GXFORMAT m_format;
	UINT m_uFrameCount = 1;

};

#endif
