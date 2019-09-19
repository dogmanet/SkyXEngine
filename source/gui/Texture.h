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
		CTexture(CTextureManager *pMgr):
			m_pTextureManager(pMgr)
		{
		}

		void setGUI(CGUI *pGui);
		void release();
		IGXTexture2D* getAPItexture() const;

		void loadFromFile(const StringW &pName);
		void loadFromMem(byte *pData);

		UINT getWidth() const
		{
			return(m_iWidth);
		}
		UINT getHeight() const
		{
			return(m_iHeight);
		}

	protected:
		IGXTexture2D *m_pTexture = NULL;
		CTextureManager *m_pTextureManager;

		UINT m_iWidth = 0;
		UINT m_iHeight = 0;

		bool m_bFailed = false;

		StringW m_szName;

		bool m_isRT = false;
	};

	typedef const CTexture* CPITexture;

	class CTextureManager
	{
	public:
		CTextureManager(const WCHAR *szResourceDir):
			m_wsResourceDir(szResourceDir)
		{
		}
		CPITexture getTexture(const StringW &szTexture);
		void release();
		void bindTexture(CPITexture tex);
		void unloadTexture(CPITexture tex);
		CTexture* createTexture(const StringW &szTexture, int w, int h, int bpp, bool isRT = false, void *pInitData = NULL, bool isAutoResizeRT = true);
		
		void addTexture(const StringW &name, CPITexture tex);

		const StringW& getResourceDir()
		{
			return(m_wsResourceDir);
		}

	protected:
		StringW m_wsResourceDir;
		AssotiativeArray<StringW, CTexture*> m_mTextures;
		CPITexture m_pCurrentTex;
	};
};

#endif
