
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "creatortextures.h"

CManagerRenderTarget::CManagerRenderTarget()
{

}

CManagerRenderTarget::~CManagerRenderTarget()
{
	for (int i = 0, il = m_aRenderTargets.size(); i<il; ++i)
	{
		CRenderTarget *pRenderTarget = m_aRenderTargets[i];

		if (pRenderTarget)
		{
			mem_release(pRenderTarget->m_pTexture);
			mem_release(pRenderTarget->m_pSurface);
			mem_release(pRenderTarget->m_pTextureCube);
		}
		mem_delete(pRenderTarget);
	}

	m_aRenderTargets.clear();
}

void CManagerRenderTarget::deleteGraphicsData(CRenderTarget *pRenderTarget)
{
	if (pRenderTarget)
	{
		mem_release(pRenderTarget->m_pTexture);
		mem_release(pRenderTarget->m_pSurface);
		mem_release(pRenderTarget->m_pTextureCube);
	}
}

void CManagerRenderTarget::createRenderTarget(CRenderTarget *pRT, RT_TYPE type, UINT uiWidth, UINT uiHeight, UINT uiLevels, DWORD dwUsage, D3DFORMAT format, D3DPOOL pool, const char *szName, float fSizeFactor)
{
	deleteGraphicsData(pRT);

	if (type == RT_TYPE::RT_TYPE_TEXTURE)
		DX_CALL(g_pDXDevice->CreateTexture(uiWidth, uiHeight, uiLevels, dwUsage, format, pool, &(pRT->m_pTexture), NULL));
	else if (type == RT_TYPE::RT_TYPE_TEXTURE_CUBE)
		DX_CALL(g_pDXDevice->CreateCubeTexture(uiWidth, uiLevels, dwUsage, format, pool, &(pRT->m_pTextureCube), NULL));
	else if (type == RT_TYPE::RT_TYPE_SURFACE)
		DX_CALL(g_pDXDevice->CreateRenderTarget(uiWidth, uiHeight, format, D3DMULTISAMPLE_NONE, 0, FALSE, &(pRT->m_pSurface), NULL));
	else if (type == RT_TYPE::RT_TYPE_SURFACE_DEPTH)
		DX_CALL(g_pDXDevice->CreateDepthStencilSurface(uiWidth, uiHeight, format, D3DMULTISAMPLE_NONE, 0, FALSE, &(pRT->m_pSurface), NULL));
	else if (type == RT_TYPE::RT_TYPE_OFF_SCREEN)
		DX_CALL(g_pDXDevice->CreateOffscreenPlainSurface(uiWidth, uiHeight, format, pool, &(pRT->m_pSurface), NULL));

	pRT->m_uiLevel = uiLevels;

	if (szName)
		pRT->m_sName = szName;

	if (fSizeFactor > 0.0001f)
		pRT->m_fSizeFactor = fSizeFactor;

	pRT->m_type = type;

	if (pRT->m_pTexture)
		pRT->m_pTexture->GetLevelDesc(0, &(pRT->m_oDesc));
	else if (pRT->m_pSurface)
		pRT->m_pSurface->GetDesc(&(pRT->m_oDesc));
	else if (pRT->m_pTextureCube)
		pRT->m_pTextureCube->GetLevelDesc(0, &(pRT->m_oDesc));
}

ID CManagerRenderTarget::add(RT_TYPE type, UINT uiWidth, UINT uiHeight, UINT uiLevels, DWORD dwUsage, D3DFORMAT format, D3DPOOL pool, const char *szName, float fSizeFactor)
{
	ID id = -1;

	for (int i = 0, il = m_aRenderTargets.size(); i<il; ++i)
	{
		if (m_aRenderTargets[i] == 0)
		{
			id = i;
			break;
		}
	}

	CRenderTarget *pRenderTarget;

	if (id < 0)
	{
		id = m_aRenderTargets.size();
		pRenderTarget = new CRenderTarget();
		m_aRenderTargets[id] = pRenderTarget;
	}
	else
		pRenderTarget = m_aRenderTargets[id];
	
	createRenderTarget(pRenderTarget, type, uiWidth, uiHeight, uiLevels, dwUsage, format, pool, szName, (fSizeFactor > 0 ? fSizeFactor : 0));

	return id;
}

void CManagerRenderTarget::reCreate(ID id, UINT uiWidth, UINT uiHeight, UINT uiLevels, DWORD dwUsage, D3DFORMAT format, D3DPOOL pool, const char *szName, float fSizeFactor)
{
	CRenderTarget *pRenderTarget = getRenderTargetById(id);

	if (!pRenderTarget)
		return;

	createRenderTarget(pRenderTarget, pRenderTarget->m_type, uiWidth, uiHeight, uiLevels, dwUsage, format, pool, szName, fSizeFactor);
}

CRenderTarget* CManagerRenderTarget::getRenderTargetById(ID id)
{
	if (id >= 0 && m_aRenderTargets.size() > id)
		return m_aRenderTargets[id];

	return 0;
}

void CManagerRenderTarget::deleteByName(const char *szName)
{
	for (int i = 0, il = m_aRenderTargets.size(); i<il; ++i)
	{
		CRenderTarget *pRenderTarget = m_aRenderTargets[i];

		if (strcmp(szName, pRenderTarget->m_sName.c_str()) == 0)
		{
			deleteById(i);
			return;
		}
	}
}

void CManagerRenderTarget::deleteById(ID id)
{
	if (id >= 0 && id < m_aRenderTargets.size() && m_aRenderTargets[id])
	{
		CRenderTarget *pRenderTarget = m_aRenderTargets[id];

		deleteGraphicsData(pRenderTarget);
	}
}

ID CManagerRenderTarget::getId(const char *szName)
{
	for (int i = 0, il = m_aRenderTargets.size(); i<il; ++i)
	{
		CRenderTarget *pRenderTarget = m_aRenderTargets[i];

		if (strcmp(szName, pRenderTarget->m_sName.c_str()) == 0)
			return i;
	}

	return(-1);
}

void CManagerRenderTarget::onLostDevice()
{
	LibReport(REPORT_MSG_LEVEL_WARNING, "release render targets ...\n");

	for (int i = 0, il = m_aRenderTargets.size(); i<il; ++i)
	{
		CRenderTarget *pRenderTarget = m_aRenderTargets[i];

		if (pRenderTarget)
		{
			deleteGraphicsData(pRenderTarget);
		}
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "release render targets success\n");
}

void CManagerRenderTarget::onResetDevice()
{
	LibReport(REPORT_MSG_LEVEL_WARNING, "reset render targets ...\n");
	
	for (int i = 0, il = m_aRenderTargets.size(); i<il; ++i)
	{
		CRenderTarget *pRenderTarget = m_aRenderTargets[i];

		if (pRenderTarget)
		{
			if (pRenderTarget->m_fSizeFactor > 0.001f)
				createRenderTarget(pRenderTarget, pRenderTarget->m_type, g_oD3DAPP.BackBufferWidth * pRenderTarget->m_fSizeFactor, g_oD3DAPP.BackBufferHeight * pRenderTarget->m_fSizeFactor, pRenderTarget->m_uiLevel, pRenderTarget->m_oDesc.Usage, pRenderTarget->m_oDesc.Format, pRenderTarget->m_oDesc.Pool, pRenderTarget->m_sName.c_str(), -1);
			else
				createRenderTarget(pRenderTarget, pRenderTarget->m_type, pRenderTarget->m_oDesc.Width, pRenderTarget->m_oDesc.Height, pRenderTarget->m_uiLevel, pRenderTarget->m_oDesc.Usage, pRenderTarget->m_oDesc.Format, pRenderTarget->m_oDesc.Pool, pRenderTarget->m_sName.c_str(), -1);
		}
	}

	LibReport(REPORT_MSG_LEVEL_NOTICE, "reset render targets success\n");
}

CRenderTarget* CManagerRenderTarget::getRenderTargetByName(const char *szName)
{
	for (int i = 0, il = m_aRenderTargets.size(); i<il; ++i)
	{
		CRenderTarget *pRenderTarget = m_aRenderTargets[i];

		if (strcmp(szName, pRenderTarget->m_sName.c_str()) == 0)
			return pRenderTarget;
	}

	return(NULL);
}

IDirect3DTexture9* CManagerRenderTarget::getTextureByName(const char *szName)
{
	CRenderTarget *pRenderTarget = getRenderTargetByName(szName);

	if (pRenderTarget && pRenderTarget->m_pTexture)
		return pRenderTarget->m_pTexture;
	else
	{
		dbg_break;
		return 0;
	}
}

IDirect3DTexture9* CManagerRenderTarget::getTextureById(ID id)
{
	CRenderTarget *pRenderTarget = getRenderTargetById(id);

	if (pRenderTarget && pRenderTarget->m_pTexture)
		return pRenderTarget->m_pTexture;
	else
	{
		dbg_break;
		return 0;
	}
}

IDirect3DCubeTexture9* CManagerRenderTarget::getTextureCubeByName(const char *szName)
{
	CRenderTarget *pRenderTarget = getRenderTargetByName(szName);

	if (pRenderTarget && pRenderTarget->m_pTextureCube)
		return pRenderTarget->m_pTextureCube;
	else
	{
		dbg_break;
		return 0;
	}
}

IDirect3DCubeTexture9* CManagerRenderTarget::getTextureCubeById(ID id)
{
	CRenderTarget *pRenderTarget = getRenderTargetById(id);
	
	if (pRenderTarget && pRenderTarget->m_pTextureCube)
		return pRenderTarget->m_pTextureCube;
	else
	{
		dbg_break;
		return 0;
	}
}

IDirect3DSurface9* CManagerRenderTarget::getSurfaceByName(const char *szName)
{
	CRenderTarget *pRenderTarget = getRenderTargetByName(szName);

	if (pRenderTarget && pRenderTarget->m_pSurface)
		return pRenderTarget->m_pSurface;
	else
	{
		dbg_break;
		return 0;
	}
}

IDirect3DSurface9* CManagerRenderTarget::getSurfaceById(ID id)
{
	CRenderTarget *pRenderTarget = getRenderTargetById(id);

	if (pRenderTarget && pRenderTarget->m_pSurface)
		return pRenderTarget->m_pSurface;
	else
	{
		dbg_break;
		return 0;
	}
}