#include "GXSamplerState.h"

void CGXSamplerState::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pRender->destroySamplerState(this);
	}
}

DWORD CGXSamplerState::getD3DTADDRESS(GXTEXTURE_ADDRESS_MODE mode)
{
	switch(mode)
	{
	case GXTEXTURE_ADDRESS_WRAP: return(D3DTADDRESS_WRAP);
	case GXTEXTURE_ADDRESS_MIRROR: return(D3DTADDRESS_MIRROR);
	case GXTEXTURE_ADDRESS_CLAMP: return(D3DTADDRESS_CLAMP);
	case GXTEXTURE_ADDRESS_BORDER: return(D3DTADDRESS_BORDER);
	case GXTEXTURE_ADDRESS_MIRROR_ONCE: return(D3DTADDRESS_MIRRORONCE);
	}
	return(D3DTADDRESS_WRAP);
}

void CGXSamplerState::getMagMinMipFilter(GXFILTER filter, DWORD *pdwMag, DWORD *pdwMin, DWORD *pdwMip)
{
	const D3DCAPS9 *dxCaps = m_pRender->getDXDeviceCaps();

	DWORD dwMagFilter = D3DTEXF_POINT;
	DWORD dwMinFilter = D3DTEXF_POINT;
	DWORD dwMipFilter = D3DTEXF_POINT;
	switch(filter)
	{
	case GXFILTER_MIN_MAG_MIP_POINT:
		break;
	case GXFILTER_MIN_MAG_POINT_MIP_LINEAR:
		dwMipFilter = D3DTEXF_LINEAR;
		break;
	case GXFILTER_MIN_POINT_MAG_LINEAR_MIP_POINT:
		dwMagFilter = D3DTEXF_LINEAR;
		break;
	case GXFILTER_MIN_POINT_MAG_MIP_LINEAR:
		dwMipFilter = D3DTEXF_LINEAR;
		dwMagFilter = D3DTEXF_LINEAR;
		break;
	case GXFILTER_MIN_LINEAR_MAG_MIP_POINT:
		dwMinFilter = D3DTEXF_LINEAR;
		break;
	case GXFILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
		dwMinFilter = D3DTEXF_LINEAR;
		dwMipFilter = D3DTEXF_LINEAR;
		break;
	case GXFILTER_MIN_MAG_LINEAR_MIP_POINT:
		dwMinFilter = D3DTEXF_LINEAR;
		dwMagFilter = D3DTEXF_LINEAR;
		break;
	case GXFILTER_MIN_MAG_MIP_LINEAR:
		dwMipFilter = D3DTEXF_LINEAR;
		dwMinFilter = D3DTEXF_LINEAR;
		dwMagFilter = D3DTEXF_LINEAR;
		break;
	case GXFILTER_ANISOTROPIC:
		dwMipFilter = D3DTEXF_ANISOTROPIC;
		dwMinFilter = D3DTEXF_ANISOTROPIC;
		dwMagFilter = D3DTEXF_ANISOTROPIC;
		break;
	}
	switch(dwMinFilter)
	{
	case D3DTEXF_ANISOTROPIC:
		if(!(dxCaps->TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC))
		{
			if((dxCaps->TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR))
			{
				dwMinFilter = D3DTEXF_LINEAR;
			}
			else
			{
				dwMinFilter = D3DTEXF_POINT;
			}
		}
		break;
	case D3DTEXF_LINEAR:
		if(!(dxCaps->TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR))
		{
			dwMinFilter = D3DTEXF_POINT;
		}
		break;
	}
	switch(dwMagFilter)
	{
	case D3DTEXF_ANISOTROPIC:
		if(!(dxCaps->TextureFilterCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC))
		{
			if((dxCaps->TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR))
			{
				dwMagFilter = D3DTEXF_LINEAR;
			}
			else
			{
				dwMagFilter = D3DTEXF_POINT;
			}
		}
		break;
	case D3DTEXF_LINEAR:
		if(!(dxCaps->TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR))
		{
			dwMagFilter = D3DTEXF_POINT;
		}
		break;
	}
	switch(dwMipFilter)
	{
	case D3DTEXF_ANISOTROPIC:
		if((dxCaps->TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR))
		{
			dwMipFilter = D3DTEXF_LINEAR;
		}
		else
		{
			dwMipFilter = D3DTEXF_POINT;
		}
		break;
	case D3DTEXF_LINEAR:
		if(!(dxCaps->TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR))
		{
			dwMipFilter = D3DTEXF_POINT;
		}
		break;
	}

	*pdwMag = dwMagFilter;
	*pdwMin = dwMinFilter;
	*pdwMip = dwMipFilter;
}
