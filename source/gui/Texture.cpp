#include "Texture.h"

namespace gui
{
	AssotiativeArray<StringW, CTexture> CTextureManager::m_mTextures;

	CPITexture CTextureManager::getTexture(const StringW & szTexture)
	{
		if(!m_mTextures.KeyExists(szTexture))
		{
			m_mTextures[szTexture].loadFromFile(szTexture);
			m_mTextures[szTexture].m_szName = szTexture;
		}
		else
		{
			if(!m_mTextures[szTexture].m_pTexture)
			{
				m_mTextures[szTexture].loadFromFile(szTexture);
				m_mTextures[szTexture].m_szName = szTexture;
			}
		}
		if(m_mTextures[szTexture].m_bFailed)
		{
			return(getTexture(L"/dev/error.png"));
		}
		return(&m_mTextures[szTexture]);
	}
	
	void CTextureManager::unloadTexture(CPITexture tex)
	{
		const_cast<CTexture*>(tex)->release();
		m_mTextures.erase(tex->m_szName);
	}

	CTexture * CTextureManager::createTexture(const StringW & szTexture, int w, int h, int bpp, bool isRT, void *pInitData, bool isAutoResizeRT)
	{
		CTexture bt;

		if(!(bt.m_pTexture = GetGUI()->getDevice()->createTexture2D(w, h, isRT ? 1 : 0, isRT ? GX_TEXFLAG_RENDERTARGET | (isAutoResizeRT ? GX_TEXFLAG_AUTORESIZE : 0) : 0, GXFMT_A8R8G8B8, pInitData)))
		{
			return(NULL);
		}

		
		bt.m_iHeight = h;
		bt.m_iWidth = w;
		bt.m_szName = szTexture;
		m_mTextures[szTexture] = bt;
		return(&m_mTextures[szTexture]);
	}

	void CTextureManager::addTexture(const StringW & name, CPITexture tex)
	{
		CTexture * ptex = const_cast<CTexture*>(tex);
		ptex->m_szName = name;
		m_mTextures[name] = *ptex;
	}

	void CTextureManager::release()
	{
		for(AssotiativeArray<StringW, CTexture>::Iterator i = m_mTextures.begin(); i != m_mTextures.end(); i++)
		{
			i.second->release();
		}
		m_mTextures.clear();
	}

	CPITexture CTextureManager::m_pCurrentTex = NULL;

	void CTextureManager::bindTexture(CPITexture tex)
	{
		if(!tex)
		{
			//FIXME: Set fallback texture
			GetGUI()->getDevice()->setTexture(NULL);
			m_pCurrentTex = NULL;
			return;
		}
		if(m_pCurrentTex != tex)
		{
			m_pCurrentTex = tex;
			GetGUI()->getDevice()->setTexture(m_pCurrentTex->m_pTexture);
		}


	}

	void CTextureManager::onLostDevice()
	{
		/*for(AssotiativeArray<StringW, CTexture>::Iterator i = m_mTextures.begin(); i != m_mTextures.end(); i++)
		{
			if(i.second->m_isRT)
			{
				i.second->m_pTexture->Release();
			}
		}*/
	}

	void CTextureManager::onResetDevice()
	{
		/*for(AssotiativeArray<StringW, CTexture>::Iterator i = m_mTextures.begin(); i != m_mTextures.end(); i++)
		{
			if(i.second->m_isRT)
			{
				DX_CALL(GetGUI()->getDevice()->CreateTexture(i.second->m_iWidth, i.second->m_iHeight, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &(i.second->m_pTexture), NULL));
			}
		}*/
	}

	//##########################################################################

	void CTexture::release()
	{
		mem_release(m_pTexture);
	}
	
	void CTexture::loadFromMem(byte * pData)
	{
		if(!pData)
		{
			return;
		}
		void *pTexData;
		if(m_pTexture->lock(&pTexData, GXTL_WRITE))
		{
			memcpy(pTexData, pData, m_iWidth * m_iHeight * 4);
			m_pTexture->unlock();
		}
		/*if(!FAILED(GetGUI()->GetDevice()->CreateTexture(m_iWidth, m_iHeight, 0, NULL, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &tex, NULL)))
		{
		D3DLOCKED_RECT lr;

		if(!FAILED(DX_CALL(tex->LockRect(0, &lr, NULL, NULL))))
		{
		memcpy(lr.pBits, pData, m_iWidth * m_iHeight * 4);
		tex->UnlockRect(0);

		if(FAILED(GetGUI()->GetDevice()->UpdateTexture(tex, m_pTexture)))
		{
		return;
		}
		}
		}
		SX_SAFE_RELEASE(tex);*/
	}

	void CTexture::loadFromFile(const StringW & pName)
	{
		StringW path = StringW(GetGUI()->getResourceDir()) + L"/textures/" + pName;
		m_pTexture = GetGUI()->getDevice()->createTexture2DFromFile(String(path).c_str(), true);
		//if(FAILED(DX_CALL(D3DXCreateTextureFromFileExW(GetGUI()->getDevice(), path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &m_pTexture))))
		if(!m_pTexture)
		{
			printf("Unable to load texture \"%s\"\n", String(path).c_str());
			m_iWidth = m_iHeight = 0;
			m_bFailed = true;
		}
		else
		{
			m_iWidth = m_pTexture->getWidth();
			m_iHeight = m_pTexture->getHeight();
		}
	}

	IGXTexture2D * CTexture::getAPItexture() const
	{
		return(m_pTexture);
	}
};