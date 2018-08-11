#include "Texture.h"
#include <d3dx9tex.h>

namespace gui
{
	AssotiativeArray<StringW, CTexture> CTextureManager::m_mTextures;
	AssotiativeArray<StringW, CTextureManager::shader> CTextureManager::m_mShaders;

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

	CTexture * CTextureManager::createTexture(const StringW & szTexture, int w, int h, int bpp, bool isRT)
	{
		CTexture bt;
		if(FAILED(GetGUI()->getDevice()->CreateTexture(w, h, isRT ? 1 : 0, isRT ? D3DUSAGE_RENDERTARGET : 0, D3DFMT_A8R8G8B8, isRT ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED, &bt.m_pTexture, NULL)))
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
			GetGUI()->getDevice()->SetTexture(0, NULL);
			m_pCurrentTex = NULL;
			return;
		}
		if(m_pCurrentTex != tex)
		{
			m_pCurrentTex = tex;
			GetGUI()->getDevice()->SetTexture(0, m_pCurrentTex->m_pTexture);
		}


	}

	void CTextureManager::compileShaderFromFile(const WCHAR * szFileName, ID3DXBuffer ** ppBlobOut)
	{
		DWORD dwShaderFlags = 0;
#ifdef _DEBUG
		dwShaderFlags |= D3DXSHADER_DEBUG | D3DXSHADER_AVOID_FLOW_CONTROL | D3DXSHADER_SKIPOPTIMIZATION;
#else
		dwShaderFlags |= D3DXSHADER_OPTIMIZATION_LEVEL3;
#endif
		ID3DXBuffer * pErrorBlob;
		HRESULT hr = D3DXCompileShaderFromFileW(szFileName, NULL, NULL, "main", "ps_2_0", dwShaderFlags, ppBlobOut, &pErrorBlob, NULL);
		//SHOW_ERROR;
		if(FAILED(hr))
		{
			if(pErrorBlob != NULL)
			{
				char * pBuff = (char*)pErrorBlob->GetBufferPointer();
				int s = strlen(pBuff);
				WCHAR * szErr = new WCHAR[s + 1];
				mbstowcs(szErr, pBuff, s);
				wprintf(L"[Error]: %s\n", szErr);
				mem_delete_a(szErr);
			}
			mem_release(pErrorBlob);
			return;
		}
		//multi
		mem_release(pErrorBlob);
	}

	CSHADER CTextureManager::loadShader(const StringW & name)
	{
		if(!m_mShaders.KeyExists(name))
		{
			shader sh;
			StringW path = StringW(GetGUI()->getResourceDir()) + L"/shaders/" + name + L".";
			FILE * pF;
			_wfopen_s(&pF, (path + L"cps").c_str(), L"rb");
			ID3DXBuffer * pShader = NULL;
			if(pF)
			{
				UINT size = 0;
				fseek(pF, 0, SEEK_END);
				size = ftell(pF);
				fseek(pF, 0, SEEK_SET);
				D3DXCreateBuffer(size, &pShader);
				fread(pShader->GetBufferPointer(), size, 1, pF);
				fclose(pF);
			}
			else
			{
				pShader = NULL;
				compileShaderFromFile((path + L"ps").c_str(), &pShader);
				if(pShader)
				{
					_wfopen_s(&pF, (path + L"cps").c_str(), L"wb");
					fwrite(pShader->GetBufferPointer(), pShader->GetBufferSize(), 1, pF);
					fclose(pF);
				}
			}
			GetGUI()->getDevice()->CreatePixelShader((DWORD *)pShader->GetBufferPointer(), &sh.ps);
			mem_release(pShader);
			m_mShaders[name] = sh;
		}
		return(&m_mShaders[name]);
	}

	void CTextureManager::bindShader(CSHADER sh)
	{
		GetGUI()->getDevice()->SetPixelShader(sh->ps);
	}

	void CTextureManager::unbindShader()
	{
		GetGUI()->getDevice()->SetPixelShader(NULL);
	}

	void CTextureManager::onLostDevice()
	{
		for(AssotiativeArray<StringW, CTexture>::Iterator i = m_mTextures.begin(); i != m_mTextures.end(); i++)
		{
			if(i.second->m_isRT)
			{
				i.second->m_pTexture->Release();
			}
		}
	}

	void CTextureManager::onResetDevice()
	{
		for(AssotiativeArray<StringW, CTexture>::Iterator i = m_mTextures.begin(); i != m_mTextures.end(); i++)
		{
			if(i.second->m_isRT)
			{
				GetGUI()->getDevice()->CreateTexture(i.second->m_iWidth, i.second->m_iHeight, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &(i.second->m_pTexture), NULL);
			}
		}
	}

	//##########################################################################

	void CTexture::release()
	{
		mem_release(m_pTexture);
	}
	
	void CTexture::loadFromMem(byte * pData)
	{
		IDirect3DTexture9 * tex = NULL;
		if(!pData)
		{
			return;
		}
		D3DLOCKED_RECT lr;
		if(!FAILED(m_pTexture->LockRect(0, &lr, NULL, NULL)))
		{
			memcpy(lr.pBits, pData, m_iWidth * m_iHeight * 4);
			m_pTexture->UnlockRect(0);
		}
		/*if(!FAILED(GetGUI()->GetDevice()->CreateTexture(m_iWidth, m_iHeight, 0, NULL, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &tex, NULL)))
		{
		D3DLOCKED_RECT lr;

		if(!FAILED(tex->LockRect(0, &lr, NULL, NULL)))
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
		D3DXIMAGE_INFO info;
		if(FAILED(D3DXCreateTextureFromFileExW(GetGUI()->getDevice(), path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &m_pTexture)))
			//if(FAILED(D3DXCreateTextureFromFileW(GetGUI()->GetDevice(), path.c_str(), &m_pTexture)))
		{
			printf("Unable to load texture \"%s\"\n", String(path).c_str());
			m_iWidth = m_iHeight = 0;
			m_bFailed = true;
		}
		else
		{
			D3DSURFACE_DESC _info;
			m_pTexture->GetLevelDesc(0, &_info);
			m_iWidth = info.Width;
			m_iHeight = info.Height;
		}
	}

	IDirect3DTexture9 * CTexture::getAPItexture() const
	{
		return(m_pTexture);
	}
};