#ifndef __FONT_MANAGER_H
#define __FONT_MANAGER_H

#include <xcommon/gui/IXFontManager.h>
#include <xcommon/IFileSystem.h>

#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#ifdef _DEBUG
#	pragma comment(lib, "freetype.lib"
#else
#	pragma comment(lib, "freetype.lib")
#endif

class CFontManager final: public IXUnknownImplementation<IXFontManager>
{
public:
	CFontManager(IGXDevice *pDev, IFileSystem *pFileSystem);
	~CFontManager();

	bool XMETHODCALLTYPE getFont(IXFont **ppOut, const char *szFile, UINT uSize, XFONT_STYLE style = XFONT_STYLE_NONE, int iBlurRadius = 0) override;

	void XMETHODCALLTYPE getFontIndexBuffer(UINT uQuads, IGXIndexBuffer **ppIndexBuffer) override;

	void XMETHODCALLTYPE getFontVertexDeclaration(IGXVertexDeclaration **ppVertexDeclaration) override;

#if 0
	CFont* getFont(const WCHAR *szName, UINT size, CFont::STYLE style = CFont::STYLE_NONE, int iBlurRadius = 0);
	FT_Library requestFT();
#endif

	void onFontReleased(CFont *pFont);

private:
	FT_Library m_pFT;
	IGXDevice *m_pDev;
	IFileSystem *m_pFileSystem;
	IGXVertexDeclaration *m_pVertexDeclaration = NULL;
	AssotiativeArray<String, CFont> m_mFonts;

	UINT m_uFontIBChars = 0;
	IGXIndexBuffer *m_pFontIB = NULL;

private:
	void scanFonts();
};

#endif
