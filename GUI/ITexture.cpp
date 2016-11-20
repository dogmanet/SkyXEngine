#include "ITexture.h"
#include <d3dx9tex.h>

namespace GUI
{


	AssotiativeArray<StringW, ITexture> ITextureManager::m_mTextures;

	CPITexture ITextureManager::GetTexture(const StringW & szTexture)
	{
		if(!m_mTextures.KeyExists(szTexture))
		{
			m_mTextures[szTexture].LoadFromFile(szTexture);
			m_mTextures[szTexture].m_szName = szTexture;
		}
		else
		{
			if(!m_mTextures[szTexture].m_pTexture)
			{
				m_mTextures[szTexture].LoadFromFile(szTexture);
				m_mTextures[szTexture].m_szName = szTexture;
			}
		}
		if(m_mTextures[szTexture].m_bFailed)
		{
			return(GetTexture(L"/dev/error.png"));
		}
		return(&m_mTextures[szTexture]);
	}


	void ITextureManager::UnloadTexture(CPITexture tex)
	{
		const_cast<ITexture*>(tex)->Release();
		m_mTextures.erase(tex->m_szName);
	}

	ITexture * ITextureManager::CreateTexture(const StringW & szTexture, int w, int h, int bpp)
	{
		ITexture bt;
		if(FAILED(GetGUI()->GetDevice()->CreateTexture(w, h, 0, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &bt.m_pTexture, NULL)))
		{
			return(NULL);
		}
		bt.m_iHeight = h;
		bt.m_iWidth = w;
		bt.m_szName = szTexture;
		m_mTextures[szTexture] = bt;
		return(&m_mTextures[szTexture]);
	}

	

	void ITextureManager::AddTexture(const StringW & name, CPITexture tex)
	{
		ITexture * ptex = const_cast<ITexture*>(tex);
		ptex->m_szName = name;
		m_mTextures[name] = *ptex;
	}

	void ITexture::Release()
	{
		SX_SAFE_RELEASE(m_pTexture);
	}


	void ITexture::LoadFromMem(byte * pData)
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

	void ITextureManager::Release()
	{
		for(AssotiativeArray<StringW, ITexture>::Iterator i = m_mTextures.begin(); i != m_mTextures.end(); i++)
		{
			i.second->Release();
		}
		m_mTextures.clear();
	}

	CPITexture ITextureManager::m_pCurrentTex = NULL;

	void ITextureManager::BindTexture(CPITexture tex)
	{
		if(!tex)
		{
			//FIXME: Set fallback texture
			GetGUI()->GetDevice()->SetTexture(0, NULL);
			m_pCurrentTex = NULL;
			return;
		}
		if(m_pCurrentTex != tex)
		{
			m_pCurrentTex = tex;
			GetGUI()->GetDevice()->SetTexture(0, m_pCurrentTex->m_pTexture);
		}


	}

	void ITexture::LoadFromFile(const StringW & pName)
	{
		StringW path = StringW(GetGUI()->GetResourceDir()) + L"/textures/" + pName;
		D3DXIMAGE_INFO info;
		if(FAILED(D3DXCreateTextureFromFileExW(GetGUI()->GetDevice(), path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &m_pTexture)))
			//if(FAILED(D3DXCreateTextureFromFileW(GetGUI()->GetDevice(), path.c_str(), &m_pTexture)))
		{
			wprintf(L"Unable to load texture \"%s\"\n", path.c_str());
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


	void ITextureManager::CompileShaderFromFile(const WCHAR * szFileName, ID3DXBuffer ** ppBlobOut)
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
				SX_SAFE_DELETE_A(szErr);
			}
			SX_SAFE_RELEASE(pErrorBlob);
			return;
		}
		//multi
		SX_SAFE_RELEASE(pErrorBlob);
	}

	CSHADER ITextureManager::LoadShader(const StringW & name)
	{
		if(!m_mShaders.KeyExists(name))
		{
			shader sh;
			StringW path = StringW(GetGUI()->GetResourceDir()) + L"/shaders/" + name + L".";
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
				CompileShaderFromFile((path + L"ps").c_str(), &pShader);
				if(pShader)
				{
					_wfopen_s(&pF, (path + L"cps").c_str(), L"wb");
					fwrite(pShader->GetBufferPointer(), pShader->GetBufferSize(), 1, pF);
					fclose(pF);
				}
			}
			GetGUI()->GetDevice()->CreatePixelShader((DWORD *)pShader->GetBufferPointer(), &sh.ps);
			SX_SAFE_RELEASE(pShader);
			m_mShaders[name] = sh;
		}
		return(&m_mShaders[name]);
	}
	void ITextureManager::BindShader(CSHADER sh)
	{
		GetGUI()->GetDevice()->SetPixelShader(sh->ps);
	}
	void ITextureManager::UnbindShader()
	{
		GetGUI()->GetDevice()->SetPixelShader(NULL);
	}

	IDirect3DTexture9 * ITexture::GetAPItexture() const
	{
		return(m_pTexture);
	}

	void ITextureManager::OnLostDevice()
	{
		for(AssotiativeArray<StringW, ITexture>::Iterator i = m_mTextures.begin(); i != m_mTextures.end(); i++)
		{
			if(i.second->isRT)
			{
				i.second->m_pTexture->Release();
			}
		}
	}
	void ITextureManager::OnResetDevice()
	{
		for(AssotiativeArray<StringW, ITexture>::Iterator i = m_mTextures.begin(); i != m_mTextures.end(); i++)
		{
			if(i.second->isRT)
			{
				GetGUI()->GetDevice()->CreateTexture(i.second->m_iWidth, i.second->m_iHeight, 0, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &(i.second->m_pTexture), NULL);
			}
		}
	}

	AssotiativeArray<StringW, ITextureManager::shader> ITextureManager::m_mShaders;
};