#ifndef __FONT_H
#define __FONT_H

#include <xcommon/gui/IXFontManager.h>
#include <xcommon/IFileSystem.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define SXF_VERSION 1
#define SXF_MAGICK 'DFXS'

class CFontManager;

class CFont final: public IXUnknownImplementation<IXFont>
{
public:
	~CFont();

	void XMETHODCALLTYPE getTexture(UINT uIdx, IGXTexture2D **ppTexture) override;

	void XMETHODCALLTYPE buildString(const char *szString, const XFontBuildParams &params, XFontStringMetrics *pMetrics) override;

	void load(CFontManager *pFontManager, IFileSystem *pFileSystem, FT_Library pFT, IGXDevice *pDev, const char *szIdent, const char *szFont, UINT uSize, XFONT_STYLE style = XFONT_STYLE_NONE, int iBlurRadius = 0);

	void getStringMetrics(const char *str, UINT uFirstLineShift, UINT * width, UINT * height, UINT * vertexCount, UINT * indexCount, UINT * strCount, XCharRect *pCharRects);

	const char* getIdent()
	{
		return(m_szIdent);
	}

private:
	void save();

	struct CharDesc
	{
		UINT id;
		short x;
		short y;
		short width;
		short height;
		float tx;
		float ty;
		float twidth;
		float theight;
		short xoffset;
		short yoffset;
		short xadvantage;
		short texid;
		CharDesc():id(0), xadvantage(0)
		{
		}
	};

	struct vertex
	{
		float2_t vPos;
		float2_t vTex;
		//DWORD color;
	};


	const CharDesc* getChar(UINT id);


	struct SXFheader
	{
		DWORD magick = SXF_MAGICK;
		int version = SXF_VERSION;
		int charCount = 0;
		int size = 0;
		int texCount = 0;
	};

	struct symRange
	{
		UINT start;
		UINT end;
	};

	struct chardata
	{
		int w;
		int h;
		int x;
		int y;
		bool ins;
		UINT c;
		byte *data;
		UINT like;
		int xo;
		int yo;
		int xa;
		bool operator<(const CFont::chardata & a)
		{
			return(h > a.h);
		}
		bool operator>(const CFont::chardata & a)
		{
			return(h < a.h);
		}
	};

	bool place(Array<chardata> &list, int width, int height);

	CFontManager *m_pFontManager = NULL;

	FT_Library m_pFT = NULL;

	IGXDevice *m_pDev = NULL;

	IFileSystem *m_pFileSystem = NULL;

	byte *m_pFontBlob = NULL;
	FT_Face m_pFTfontFace = NULL;

	SXFheader m_header;
	Array<CharDesc> m_chars;

	XFONT_STYLE m_style;
	bool m_bEmulateBold;
	bool m_bEmulateItalic;

	const char *m_szIdent;
	String m_sName;
	Array<UINT> m_aFontChars;

	UINT m_uFontSize;

	Array<IGXTexture2D*> m_apTextures;
	Array<byte*> m_ppTextures;

	bool m_bHasBeenChanged = false;

	void buildFont();

	void loadChar(UINT id);

	void loadFTfontFace();
	void freeFTfontFace();

	void generateBase();
	void regen();
	void addChar(UINT c, bool full = true);

	bool imageEqual(const chardata &a, const chardata &b);

	static const symRange* getSymRange(UINT sym);

	int m_iBlurRadius = 0;

private:
	void XMETHODCALLTYPE FinalRelease() override;
};

#endif
