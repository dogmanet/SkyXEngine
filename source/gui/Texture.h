#ifndef _ITexture_H_
#define _ITexture_H_

#include "GUIbase.h"
#include "String.h"

namespace gui
{
	class CTextureManager;
	class CVideoRenderer;
	class CTexture
	{
		friend class CTextureManager;
		friend class CVideoRenderer;
	public:
		CTexture():
			m_pTexture(NULL), 
			m_bFailed(false), 
			m_isRT(false)
		{
		}
		void setGUI(CGUI * pGui);
		void release();
		IGXTexture2D * getAPItexture() const;

		void loadFromFile(const StringW & pName);
		void loadFromMem(byte * pData);

		UINT getWidth() const
		{
			return(m_iWidth);
		}
		UINT getHeight() const
		{
			return(m_iHeight);
		}

	protected:
		IGXTexture2D * m_pTexture;

		UINT m_iWidth;
		UINT m_iHeight;

		bool m_bFailed;

		StringW m_szName;

		bool m_isRT;
	};

	typedef const CTexture * CPITexture;

	class CTextureManager
	{
	public:
		
		static CPITexture getTexture(const StringW & szTexture);
		static void release();
		static void bindTexture(CPITexture tex);
		static void unloadTexture(CPITexture tex);
		static CTexture * createTexture(const StringW & szTexture, int w, int h, int bpp, bool isRT = false, void *pInitData = NULL);
		
		static void onLostDevice();
		static void onResetDevice();

		static void addTexture(const StringW & name, CPITexture tex);

	protected:
		static AssotiativeArray<StringW, CTexture> m_mTextures;
		static CPITexture m_pCurrentTex;

	};
};

#endif
