
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __CREATORTEXTURES_H
#define __CREATORTEXTURES_H

#include <gdefines.h>
#include <stdio.h>
#include <d3d9.h>
#include <common/string.h>
#include <common/array.h>
#include <gcore/sxgcore.h>

//##########################################################################

extern IDirect3DDevice9 *g_pDXDevice;
extern D3DCAPS9 g_dxCaps;
extern D3DPRESENT_PARAMETERS g_oD3DAPP;

//##########################################################################

//! структура описывающая render target
struct CRenderTarget
{
	CRenderTarget(){ m_sName = ""; m_pTexture = 0; m_pSurface = 0; m_pTextureCube = 0; m_uiLevel = 0; m_fSizeFactor = 0; }

	//! имя
	String m_sName;

	//! тип
	RT_TYPE m_type;

	//! текстура
	IDirect3DTexture9 *m_pTexture;

	IDirect3DCubeTexture9 *m_pTextureCube;

	//! 
	IDirect3DSurface9 *m_pSurface;

	//! описание текстуры, для восстановления
	D3DSURFACE_DESC m_oDesc;

	//! 
	UINT m_uiLevel;

	//! если меньше 0 то брать размер из #m_oDesc, иначе это коэфициент умножение размера экрана на это число
	float m_fSizeFactor;		
};

//##########################################################################

class CManagerRenderTarget
{
public:
	CManagerRenderTarget();
	~CManagerRenderTarget();

	void onLostDevice();

	void onResetDevice();


	ID add(RT_TYPE type, UINT uiWidth, UINT uiHeight, UINT uiLevels, DWORD dwUsage, D3DFORMAT format, D3DPOOL pool, const char *szName, float fSizeFactor);

	void reCreate(ID id, UINT uiWidth, UINT uiHeight, UINT uiLevels, DWORD dwUsage, D3DFORMAT format, D3DPOOL pool, const char *szName = 0, float fSizeFactor = -1);

	void deleteByName(const char *szName);
	void deleteById(ID id);

	ID getId(const char *szName);

	


	IDirect3DTexture9* getTextureByName(const char *szName);
	IDirect3DTexture9* getTextureById(ID id);

	IDirect3DCubeTexture9* getTextureCubeByName(const char *szName);
	IDirect3DCubeTexture9* getTextureCubeById(ID id);

	IDirect3DSurface9* getSurfaceByName(const char *szName);
	IDirect3DSurface9* getSurfaceById(ID id);

private:

	void deleteGraphicsData(CRenderTarget *pRenderTarget);

	CRenderTarget* getRenderTargetById(ID id);

	CRenderTarget* getRenderTargetByName(const char *szName);

	void createRenderTarget(CRenderTarget *pRT, RT_TYPE type, UINT uiWidth, UINT uiHeight, UINT uiLevels, DWORD dwUsage, D3DFORMAT format, D3DPOOL pool, const char *szName, float fSizeFactor);

	Array<CRenderTarget*> m_aRenderTargets;
};

#endif