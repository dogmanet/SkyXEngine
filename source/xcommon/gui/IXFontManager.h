#ifndef __IXFONTMANAGER_H
#define __IXFONTMANAGER_H

#include <gdefines.h>
#include <graphix/graphix.h>

enum XFONT_STYLE
{
	XFONT_STYLE_NONE = 0x00,
	XFONT_STYLE_BOLD = 0x01,
	XFONT_STYLE_ITALIC = 0x02
};
XDEFINE_ENUM_FLAG_OPERATORS(XFONT_STYLE);

enum XFONT_DECORATION
{
	XFONT_DECORATION_NONE = 0x00,
	XFONT_DECORATION_UNDERLINE = 0x01,
	XFONT_DECORATION_LINE_THROUGH = 0x02,
	XFONT_DECORATION_OVERLINE = 0x04
};
XDEFINE_ENUM_FLAG_OPERATORS(XFONT_DECORATION);

enum XFONT_TEXT_ALIGN
{
	XFONT_TEXT_ALIGN_LEFT,
	XFONT_TEXT_ALIGN_RIGHT,
	XFONT_TEXT_ALIGN_CENTER
};

struct XFontStringMetrics
{
	UINT uWidth;
	UINT uHeight;
	UINT uVertexCount;
	UINT uQuadCount;
	UINT uLineCount;
};

struct XFontGPUVertex
{
	float2_t vPos;
	float2_t vTex;
};

struct XFontVertex: public XFontGPUVertex
{
	UINT uTexIdx;
};

struct XCharRect
{
	int iTop;
	int iLeft;
	int iBottom;
	int iRight;
	UINT uChar;
};

struct XFontBuildParams
{
	XFONT_DECORATION decoration = XFONT_DECORATION_NONE;
	XFONT_TEXT_ALIGN textAlign = XFONT_TEXT_ALIGN_LEFT;
	UINT uAreaWidth = 0;
	UINT uFirstLineShift = 0;
	XFontVertex *pVertices = NULL;
	XCharRect *pCharRects = NULL;
};

class IXFont: public IXUnknown
{
public:
	virtual void XMETHODCALLTYPE getTexture(UINT uIdx, IGXTexture2D **ppTexture) = 0;

	virtual void XMETHODCALLTYPE buildString(const char *szString, const XFontBuildParams &params, XFontStringMetrics *pMetrics) = 0;
};

// {E5B73B75-6CA8-47D3-8AB2-E77BDB750A7D}
#define IXFONTMANAGER_GUID DEFINE_XGUID(0xe5b73b75, 0x6ca8, 0x47d3, 0x8a, 0xb2, 0xe7, 0x7b, 0xdb, 0x75, 0xa, 0x7d)

class IXFontManager: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE getFont(IXFont **ppOut, const char *szFile, UINT uSize, XFONT_STYLE style = XFONT_STYLE_NONE, int iBlurRadius = 0) = 0;

	virtual void XMETHODCALLTYPE getFontIndexBuffer(UINT uQuads, IGXIndexBuffer **ppIndexBuffer) = 0;

	virtual void XMETHODCALLTYPE getFontVertexDeclaration(IGXVertexDeclaration **ppVertexDeclaration) = 0;
};

#endif
