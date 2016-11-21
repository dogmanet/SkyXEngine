#ifndef _IFont_H_
#define _IFont_H_

#include "GUIbase.h"


#include <ft2build.h>
#include FT_FREETYPE_H
#ifdef _DEBUG
#	pragma comment(lib, "freetype26d.lib")
#else
#	pragma comment(lib, "freetype26.lib")
#endif

#define SXF_VERSION 1
#define SXF_MAGICK 'DFXS'

namespace GUI
{
	// basePath/fonts/name.ttf // ttf font source file
	// basePath/fonts/name_<size>.sxf // sxf font cache file

	class IFont
	{
	public:
		IFont():m_pFTfontFace(NULL), m_bHasBeenChanged(false)
		{
		}
		~IFont();
		enum STYLE
		{
			STYLE_NONE = 0x00,
			STYLE_BOLD = 0x01,
			STYLE_ITALIC = 0x02,
			STYLE_BOLD_ITALIC = 0x03
		};

		enum DECORATION
		{
			DECORATION_NONE = 0x00,
			DECORATION_UNDERLINE = 0x01,
			DECORATION_LINE_THROUGH = 0x02,
			DECORATION_OVERLINE = 0x04
		};

		enum TEXT_ALIGN
		{
			TEXT_ALIGN_LEFT,
			TEXT_ALIGN_RIGHT,
			TEXT_ALIGN_CENTER
		};

		void Load(const WCHAR * szFont, UINT size, STYLE style);
		void Release();

		void Save();

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
			float3_t Pos;
			float2_t Tex;
			DWORD color;
		};

		struct char_rects
		{
			Array<RECT> * pResArray;
			int iXoffs;
			int iYoffs;
		};


		const CharDesc * GetChar(UINT id);


		CPITexture GetTexture(UINT i);

		void GetStringMetrics(const StringW & str, UINT * width, UINT * height, UINT * vertexCount, UINT * indexCount, UINT * strCount, char_rects * pcr = NULL);

		void BuildString(const StringW & str, UINT decoration, TEXT_ALIGN textAlign, IDirect3DVertexBuffer9 ** ppVertexBuffer, IDirect3DIndexBuffer9 ** ppIndexBuffer, UINT * vertexCount, UINT * indexCount, UINT * lineIndexCount, UINT iAreaWidth = 0, UINT iFirstShift = 0, UINT * pStrWidth = NULL);
	protected:

		struct SXFheader
		{
			SXFheader():version(SXF_VERSION), magick(SXF_MAGICK)
			{
			}
			DWORD magick;
			int version;
			int charCount;
			int size;
			int texCount;
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
			int c;
			byte * data;
			int like;
			int xo;
			int yo;
			int xa;
			bool operator<(const IFont::chardata & a)
			{
				return(h > a.h);
			}
			bool operator>(const IFont::chardata & a)
			{
				return(h < a.h);
			}
		};

		bool Place(Array<chardata> &list, int width, int height);

		FT_Face m_pFTfontFace;

		SXFheader m_header;
		Array<CharDesc> m_chars;

		STYLE m_style;
		bool m_bEmulateBold;
		bool m_bEmulateItalic;

		StringW m_szFontName;
		StringW m_szFontChars;

		UINT m_iFontSize;

		Array<ITexture*> m_vpTextures;
		Array<byte*> m_ppTextures;

		bool m_bHasBeenChanged;

		void BuildFont();

		void LoadChar(UINT id);

		void LoadFTfontFace();
		void FreeFTfontFace();

		void GenerateBase();
		void Regen();
		void AddChar(WCHAR c, bool full = true);

		bool IFont::imageEqual(const chardata & a, const chardata & b);

		static const symRange * GetSymRange(UINT sym);


	};



	typedef const IFont::CharDesc * CPFCHAR;

	class IFontManager
	{
	public:
		static IFont * GetFont(const WCHAR * szName, UINT size, IFont::STYLE style = IFont::STYLE_NONE);
		static FT_Library RequestFT();

		static void Release();

	protected:
		static AssotiativeArray<StringW, IFont> m_mFonts;

		static FT_Library m_pFT;
	};

};

#endif
