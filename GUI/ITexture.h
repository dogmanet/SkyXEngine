#ifndef _ITexture_H_
#define _ITexture_H_

#include "GUIbase.h"
#include "String.h"

namespace GUI
{
	class ITextureManager;
	class IVideoRenderer;
	class ITexture
	{
		friend class ITextureManager;
		friend class IVideoRenderer;
	public:
		ITexture():
			m_pTexture(NULL), 
			m_bFailed(false), 
			isRT(false)
		{
		}
		void SetGUI(CGUI * pGui);
		void Release();
		IDirect3DTexture9 * GetAPItexture() const;

		void LoadFromFile(const StringW & pName);
		void LoadFromMem(byte * pData);

		UINT GetWidth() const
		{
			return(m_iWidth);
		}
		UINT GetHeight() const
		{
			return(m_iHeight);
		}

	protected:
		IDirect3DTexture9 * m_pTexture;

		UINT m_iWidth;
		UINT m_iHeight;

		bool m_bFailed;

		StringW m_szName;

		bool isRT;
	};

	typedef const ITexture * CPITexture;

	class ITextureManager
	{
	public:

		struct shader
		{
			//IDirect3DVertexShader9 * vs;
			IDirect3DPixelShader9 * ps;
			//IDirect3DVertexDeclaration9 * vertexLayout;
		};

		static CPITexture GetTexture(const StringW & szTexture);
		static void Release();
		static void BindTexture(CPITexture tex);
		static void UnloadTexture(CPITexture tex);
		static ITexture * CreateTexture(const StringW & szTexture, int w, int h, int bpp);

		static const shader * LoadShader(const StringW & name);
		static void BindShader(const shader * sh);
		static void UnbindShader();

		static void OnLostDevice();
		static void OnResetDevice();

		static void AddTexture(const StringW & name, CPITexture tex);

	protected:

		static void CompileShaderFromFile(const WCHAR * szFileName, ID3DXBuffer ** ppBlobOut);

		static AssotiativeArray<StringW, ITexture> m_mTextures;
		static CPITexture m_pCurrentTex;

		static AssotiativeArray<StringW, shader> m_mShaders;
	};
	typedef const ITextureManager::shader * CSHADER;
};

#endif
