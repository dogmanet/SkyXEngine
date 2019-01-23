#include "GXContext.h"

#include "GXIndexBuffer.h"
#include "GXVertexBuffer.h"
#include "GXVertexDeclaration.h"
#include "GXShader.h"
#include "GXRenderBuffer.h"
#include "GXSamplerState.h"
#include "GXRasterizerState.h"
#include "GXDepthStencilState.h"
#include "GXDepthStencilSurface.h"
#include "GXSurface.h"
#include "GXBlendState.h"
#include "GXTexture.h"
#include <cstdio>

//флаги компиляции шейдеров


#ifdef _DEBUG
#	define SHADER_FLAGS (D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3DXSHADER_AVOID_FLOW_CONTROL | D3DXSHADER_SKIPOPTIMIZATION)
#else
#	define SHADER_FLAGS (D3DXSHADER_OPTIMIZATION_LEVEL3 | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3DXSHADER_PARTIALPRECISION | D3DXSHADER_PREFER_FLOW_CONTROL)
#endif

CGXContext::CGXContext():
	m_pCurIndexBuffer(NULL),
	m_drawPT(D3DPT_TRIANGLELIST),
	m_pCurRenderBuffer(NULL)
{
	memset(&m_sync_state, 0, sizeof(m_sync_state));
	memset(&m_dwCurrentSamplerStates, 0, sizeof(m_dwCurrentSamplerStates));
	memset(&m_pSamplerState, 0, sizeof(m_pSamplerState));
	memset(&m_pColorTarget, 0, sizeof(m_pColorTarget));
	memset(&m_pTextures, 0, sizeof(m_pTextures));
}

void CGXContext::Release()
{
	delete this;
}

bool CGXContext::beginFrame()
{
	if(!canBeginFrame())
	{
		return(false);
	}

	return(SUCCEEDED(m_pDevice->BeginScene()));
}
void CGXContext::endFrame()
{
	m_pDevice->EndScene();

	if(m_bDeviceWasReset)
	{
		m_bDeviceWasReset = false;
	}
}
bool CGXContext::canBeginFrame()
{
	if(m_bDeviceLost)
	{
		switch(m_pDevice->TestCooperativeLevel())
		{
		case D3D_OK:
			// device is no longer lost and reset has completed successfully; recover resources here
			m_bDeviceLost = false;
			return(true);  // note: this assumes that this test is the last thing in the function it's being called from
		case D3DERR_DEVICELOST:
			Sleep(5);
			// device is still lost; can't do anything so do nothing
			return(false);
		case D3DERR_DEVICENOTRESET:
			// device is ready to be reset; Release resources here and then Reset it
			// (be sure to test the hr from your Reset call too!)
			//ReleaseResources();
			onLostDevice();
			m_pDevice->Reset(&m_oD3DAPP);
			onResetDevice();
			m_bDeviceWasReset = true;
			return(false);
		default:
			MessageBox(NULL, "TestCooperativeLevel() failed", "Error", MB_OK | MB_ICONSTOP);
			PostQuitMessage(0);
			return(false);
		}
	}

	return(true);
}

bool CGXContext::wasReset()
{
	return(m_bDeviceWasReset);
}

void CGXContext::onLostDevice()
{
	for(UINT i = 0, l = m_aResettableVertexBuffers.size(); i < l; ++i)
	{
		((CGXVertexBuffer*)m_aResettableVertexBuffers[i])->onDevLost();
	}
	for(UINT i = 0, l = m_aResettableIndexBuffers.size(); i < l; ++i)
	{
		((CGXIndexBuffer*)m_aResettableVertexBuffers[i])->onDevLost();
	}
	for(UINT i = 0, l = m_aResettableDSSurfaces.size(); i < l; ++i)
	{
		((CGXDepthStencilSurface*)m_aResettableDSSurfaces[i])->onDevLost();
	}
	for(UINT i = 0, l = m_aResettableTextures2D.size(); i < l; ++i)
	{
		dynamic_cast<CGXTexture2D*>(m_aResettableTextures2D[i])->onDevLost();
	}
	for(UINT i = 0, l = m_aResettableTexturesCube.size(); i < l; ++i)
	{
		dynamic_cast<CGXTextureCube*>(m_aResettableTexturesCube[i])->onDevLost();
	}
}

void CGXContext::onResetDevice()
{
	for(UINT i = 0, l = m_aResettableVertexBuffers.size(); i < l; ++i)
	{
		((CGXVertexBuffer*)m_aResettableVertexBuffers[i])->onDevRst();
	}
	for(UINT i = 0, l = m_aResettableIndexBuffers.size(); i < l; ++i)
	{
		((CGXIndexBuffer*)m_aResettableVertexBuffers[i])->onDevRst();
	}
	for(UINT i = 0, l = m_aResettableDSSurfaces.size(); i < l; ++i)
	{
		((CGXDepthStencilSurface*)m_aResettableDSSurfaces[i])->onDevRst(m_oD3DAPP.BackBufferWidth, m_oD3DAPP.BackBufferHeight);
	}
	for(UINT i = 0, l = m_aResettableTextures2D.size(); i < l; ++i)
	{
		dynamic_cast<CGXTexture2D*>(m_aResettableTextures2D[i])->onDevRst(m_oD3DAPP.BackBufferWidth, m_oD3DAPP.BackBufferHeight);
	}
	for(UINT i = 0, l = m_aResettableTexturesCube.size(); i < l; ++i)
	{
		dynamic_cast<CGXTextureCube*>(m_aResettableTexturesCube[i])->onDevRst(m_oD3DAPP.BackBufferHeight);
	}
}

void CGXContext::resize(int iWidth, int iHeight, bool isWindowed)
{
	m_oD3DAPP.BackBufferWidth = iWidth;
	m_oD3DAPP.BackBufferHeight = iHeight;
	m_oD3DAPP.Windowed = isWindowed;

	m_pDevice->Reset(&m_oD3DAPP);
}



BOOL CGXContext::initContext(SXWINDOW wnd, int iWidth, int iHeight, bool isWindowed)
{
	m_hWnd = (HWND)wnd;

	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!m_pD3D)
	{
		return(FALSE);
	}

	memset(&m_oD3DAPP, 0, sizeof(m_oD3DAPP));
	m_oD3DAPP.BackBufferWidth = iWidth;
	m_oD3DAPP.BackBufferHeight = iHeight;
	m_oD3DAPP.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_oD3DAPP.BackBufferCount = 1;
	m_oD3DAPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_oD3DAPP.MultiSampleQuality = 0;
	m_oD3DAPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_oD3DAPP.hDeviceWindow = m_hWnd;
	m_oD3DAPP.Windowed = isWindowed;
	m_oD3DAPP.EnableAutoDepthStencil = true;
	m_oD3DAPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_oD3DAPP.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	m_oD3DAPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_oD3DAPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &m_oD3DAPP, &m_pDevice)))
	{
		return(FALSE);
	}
	m_pDevice->GetDeviceCaps(&m_dxCaps);


	GXSAMPLER_DESC samplerDesc;
	memset(&samplerDesc, 0, sizeof(samplerDesc));
	samplerDesc.uMaxAnisotropy = 1;
	m_pDefaultSamplerState = createSamplerState(&samplerDesc);

	GXRASTERIZER_DESC rasterizerDesc;
	memset(&rasterizerDesc, 0, sizeof(rasterizerDesc));
	rasterizerDesc.fillMode = GXFILL_SOLID;
	rasterizerDesc.cullMode = GXCULL_BACK;
	rasterizerDesc.bDepthClipEnable = TRUE;
	m_pDefaultRasterizerState = createRasterizerState(&rasterizerDesc);

	GXDEPTH_STENCIL_DESC depthStencilDesc;
	memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));
	depthStencilDesc.bDepthEnable = TRUE;
	depthStencilDesc.bEnableDepthWrite = TRUE;
	depthStencilDesc.depthFunc = GXCOMPARISON_LESS_EQUAL;
	depthStencilDesc.bStencilEnable = FALSE;

	m_pDefaultDepthStencilState = createDepthStencilState(&depthStencilDesc);

	GXBLEND_DESC blendDesc;
	memset(&blendDesc, 0, sizeof(blendDesc));
	blendDesc.renderTarget[0].u8RenderTargetWriteMask = GXCOLOR_WRITE_ENABLE_ALL;
	
	m_pDefaultBlendState = createBlendState(&blendDesc);

	DX_CALL(m_pDevice->GetDepthStencilSurface(&m_pDefaultDepthStencilSurface));
	DX_CALL(m_pDevice->GetRenderTarget(0, &m_pDefaultColorTarget));

	return(TRUE);
}

CGXContext::~CGXContext()
{
	mem_release(m_pDefaultColorTarget);
	mem_release(m_pDefaultDepthStencilSurface);
	mem_release(m_pDefaultRasterizerState);
	mem_release(m_pDefaultSamplerState);
	mem_release(m_pDefaultBlendState);
	mem_release(m_pDefaultDepthStencilState);
	mem_release(m_pDevice);
	mem_release(m_pD3D);
}


void CGXContext::swapBuffers()
{
	if(DX_CALL(m_pDevice->Present(0, 0, 0, 0)) == D3DERR_DEVICELOST)
	{
		m_bDeviceLost = true;
	}
}

void CGXContext::clear(UINT what, GXCOLOR color, float fDepth, UINT uStencil)
{
	syncronize(GX_SYNCFLAG_NO_SHADER);
	DX_CALL(m_pDevice->Clear(0, 0, what, color, fDepth, uStencil));
}


IGXVertexBuffer * CGXContext::createVertexBuffer(size_t size, UINT flags, void * pInitData)
{
	CGXVertexBuffer * pBuff = new CGXVertexBuffer(this);

	UINT usage = 0;
	D3DPOOL pool = (flags & GX_BUFFER_ALLOWDISCARD) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

	if(flags & GX_BUFFER_USAGE_STATIC)
	{
		//usage = 0;
	}
	if(flags & GX_BUFFER_USAGE_DYNAMIC && pool == D3DPOOL_DEFAULT)
	{
		usage = D3DUSAGE_DYNAMIC;
	}
	if(flags & GX_BUFFER_USAGE_STREAM)
	{
		pool = D3DPOOL_DEFAULT;
		usage |= D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
	}

	pBuff->m_uSize = size;
	pBuff->m_uFlags = usage;
	if(FAILED(DX_CALL(m_pDevice->CreateVertexBuffer(size, usage, 0, pool, &pBuff->m_pBuffer, NULL))))
	{
		mem_delete(pBuff);
		return(NULL);
	}

	if(usage & D3DUSAGE_DYNAMIC)
	{
		pBuff->m_uLockFlagsWrite = D3DLOCK_DISCARD;
	}

	if(pool == D3DPOOL_DEFAULT)
	{
		pBuff->m_isResettable = true;
		m_aResettableVertexBuffers.push_back(pBuff);
	}

	if(pInitData)
	{
		void *pData;
		if(pBuff->lock(&pData, GXBL_WRITE))
		{
			memcpy(pData, pInitData, size);
			pBuff->unlock();
		}
	}

	return(pBuff);
}

IGXIndexBuffer * CGXContext::createIndexBuffer(size_t size, UINT flags, GXINDEXTYPE it, void * pInitData)
{
	CGXIndexBuffer * pBuff = new CGXIndexBuffer(this);

	UINT usage = 0;
	D3DPOOL pool = (flags & GX_BUFFER_ALLOWDISCARD) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;

	if(flags & GX_BUFFER_USAGE_STATIC)
	{
		//usage = 0;
	}
	if(flags & GX_BUFFER_USAGE_DYNAMIC && pool == D3DPOOL_DEFAULT)
	{
		usage = D3DUSAGE_DYNAMIC;
	}
	if(flags & GX_BUFFER_USAGE_STREAM)
	{
		pool = D3DPOOL_DEFAULT;
		usage |= D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
	}
	if(usage & D3DUSAGE_DYNAMIC)
	{
		pBuff->m_uLockFlagsWrite = D3DLOCK_DISCARD;
	}

	switch(it)
	{
	case GXIT_USHORT:
		pBuff->m_format = D3DFMT_INDEX16;
		break;
	case GXIT_UINT:
		pBuff->m_format = D3DFMT_INDEX32;
		break;
	}

	pBuff->m_uSize = size;
	pBuff->m_uFlags = usage;
	if(FAILED(DX_CALL(m_pDevice->CreateIndexBuffer(size, usage, pBuff->m_format, pool, &pBuff->m_pBuffer, NULL))))
	{
		mem_delete(pBuff);
		return(NULL);
	}

	if(pool == D3DPOOL_DEFAULT)
	{
		pBuff->m_isResettable = true;
		m_aResettableIndexBuffers.push_back(pBuff);
	}

	if(pInitData)
	{
		void *pData;
		if(pBuff->lock(&pData, GXBL_WRITE))
		{
			memcpy(pData, pInitData, size);
			pBuff->unlock();
		}
	}
	
	return(pBuff);
}

void CGXContext::destroyIndexBuffer(IGXIndexBuffer * pBuff)
{
	if(pBuff)
	{
		if(m_pCurIndexBuffer == pBuff)
		{
			m_pCurIndexBuffer = NULL;
			m_sync_state.bIndexBuffer = TRUE;
		}

		if(((CGXIndexBuffer*)pBuff)->m_isResettable)
		{
			for(UINT i = 0, l = m_aResettableIndexBuffers.size(); i < l; ++i)
			{
				if(m_aResettableIndexBuffers[i] == pBuff)
				{
					m_aResettableIndexBuffers.erase(i);
					break;
				}
			}
		}
	}
	mem_delete(pBuff);
}

void CGXContext::destroyVertexBuffer(IGXVertexBuffer * pBuff)
{
	if(pBuff)
	{
		/*if(m_pCurIndexBuffer == pBuff)
		{
			m_pCurIndexBuffer = NULL;
			m_sync_state.bIndexBuffer = TRUE;
		}*/

		if(((CGXVertexBuffer*)pBuff)->m_isResettable)
		{
			for(UINT i = 0, l = m_aResettableVertexBuffers.size(); i < l; ++i)
			{
				if(m_aResettableVertexBuffers[i] == pBuff)
				{
					m_aResettableVertexBuffers.erase(i);
					break;
				}
			}
		}
	}
	mem_delete(pBuff);
}

IGXVertexDeclaration * CGXContext::createVertexDeclaration(const GXVERTEXELEMENT * pDecl)
{
	CGXVertexDeclaration * vd = new CGXVertexDeclaration(m_pDevice, this, pDecl);

	return(vd);
}
void CGXContext::destroyVertexDeclaration(IGXVertexDeclaration * pDecl)
{
	if(m_pCurVertexDecl == pDecl)
	{
		//m_pGL->glDeleteBuffers(1, &((IDSGvertexDeclarationOgl*)m_pCurVertexDecl)->m_pVAO);
		m_pCurVertexDecl = NULL;
		//m_sync_state.bVertexLayout = TRUE;
	}
	mem_delete(pDecl);
	
}

void CGXContext::setIndexBuffer(IGXIndexBuffer * pBuff)
{
	mem_release(m_pCurIndexBuffer);
	m_pCurIndexBuffer = pBuff;
	if(pBuff)
	{
		pBuff->AddRef();
	}
	m_sync_state.bIndexBuffer = TRUE;
}

void CGXContext::drawIndexed(UINT uVertexCount, UINT uPrimitiveCount, UINT uStartIndexLocation, int iBaseVertexLocation)
{
	syncronize();
	
	DX_CALL(m_pDevice->DrawIndexedPrimitive(m_drawPT, iBaseVertexLocation, 0, uVertexCount, uStartIndexLocation, uPrimitiveCount));
}

void CGXContext::drawPrimitive(UINT uStartVertex, UINT uPrimitiveCount)
{
	syncronize();

	DX_CALL(m_pDevice->DrawPrimitive(m_drawPT, uStartVertex, uPrimitiveCount));
}

void CGXContext::_beginInstancing(UINT uInstanceCount)
{
	syncronize();
	CGXRenderBuffer *pRB = (CGXRenderBuffer*)m_pCurRenderBuffer;
	CGXVertexDeclaration *pVD = (CGXVertexDeclaration*)pRB->m_pVertexDeclaration;

	DX_CALL(m_pDevice->SetVertexDeclaration(pVD->m_pDeclaration));
	for(UINT i = 0; i < pRB->m_uStreamCount; ++i)
	{
		if(pVD->m_u8SpecSpec[i] == GXDECLSPEC_PER_VERTEX_DATA)
		{
			DX_CALL(m_pDevice->SetStreamSourceFreq(i, (D3DSTREAMSOURCE_INDEXEDDATA | uInstanceCount)));
		}
		else if(pVD->m_u8SpecSpec[i] == GXDECLSPEC_PER_INSTANCE_DATA)
		{
			DX_CALL(m_pDevice->SetStreamSourceFreq(i, (D3DSTREAMSOURCE_INSTANCEDATA | 1)));
		}
	}
}
void CGXContext::_endInstancing()
{
	CGXRenderBuffer *pRB = (CGXRenderBuffer*)m_pCurRenderBuffer;
	for(UINT i = 0; i < pRB->m_uStreamCount; ++i)
	{
		DX_CALL(m_pDevice->SetStreamSourceFreq(i, 1));
	}
}

void CGXContext::drawIndexedInstanced(UINT uInstanceCount, UINT uVertexCount, UINT uPrimitiveCount, UINT uStartIndexLocation, int iBaseVertexLocation)
{
	_beginInstancing(uInstanceCount);

	DX_CALL(m_pDevice->DrawIndexedPrimitive(m_drawPT, iBaseVertexLocation, 0, uVertexCount, uStartIndexLocation, uPrimitiveCount));

	_endInstancing();
}
void CGXContext::drawPrimitiveInstanced(UINT uInstanceCount, UINT uStartVertex, UINT uPrimitiveCount)
{
	_beginInstancing(uInstanceCount);

	DX_CALL(m_pDevice->DrawPrimitive(m_drawPT, uStartVertex, uPrimitiveCount));

	_endInstancing();
}

void CGXContext::syncronize(UINT flags)
{
	if(m_sync_state.bRenderBuffer)
	{
		if(m_pCurRenderBuffer)
		{
			CGXRenderBuffer *pRB = (CGXRenderBuffer*)m_pCurRenderBuffer;
			CGXVertexDeclaration *pVD = (CGXVertexDeclaration*)pRB->m_pVertexDeclaration;
			assert(pVD->m_u8StreamCount == pRB->m_uStreamCount);

			DX_CALL(m_pDevice->SetVertexDeclaration(pVD->m_pDeclaration));
			for(UINT i = 0; i < pRB->m_uStreamCount; ++i)
			{
				DX_CALL(m_pDevice->SetStreamSource(i, ((CGXVertexBuffer*)pRB->m_ppVertexBuffers[i])->m_pBuffer, 0, pVD->m_u8StreamStride[i]));
			}
		}

		m_sync_state.bRenderBuffer = FALSE;
	}
	if(m_sync_state.bIndexBuffer)
	{
		DX_CALL(m_pDevice->SetIndices(m_pCurIndexBuffer ? ((CGXIndexBuffer*)m_pCurIndexBuffer)->m_pBuffer : 0));
		m_sync_state.bIndexBuffer = FALSE;
	}
	if(m_sync_state.bRasterizerState)
	{
		CGXRasterizerState *pRS = (CGXRasterizerState*)m_pRasterizerState;
		if(!pRS)
		{
			pRS = (CGXRasterizerState*)m_pDefaultRasterizerState;
		}
		DX_CALL(pRS->m_pStateBlock->Apply());
		m_sync_state.bRasterizerState = FALSE;
	}
	for(UINT i = 0; i < MAX_GXSAMPLERS; ++i)
	{
		if(m_sync_state.bSamplerState[i])
		{
			CGXSamplerState *pSS = (CGXSamplerState*)m_pSamplerState[i];
			if(!pSS)
			{
				pSS = (CGXSamplerState*)m_pDefaultSamplerState;
			}
			for(UINT j = 1; j < D3DSAMP_SRGBTEXTURE; ++j)
			{
				if(m_dwCurrentSamplerStates[i][j] != pSS->m_dwCurrentStates[j])
				{
					m_dwCurrentSamplerStates[i][j] = pSS->m_dwCurrentStates[j];
					DX_CALL(m_pDevice->SetSamplerState(i, (D3DSAMPLERSTATETYPE)j, pSS->m_dwCurrentStates[j]));
				}
			}
			m_sync_state.bSamplerState[i] = FALSE;
		}
	}

	if(m_sync_state.bStencilRef)
	{
		DX_CALL(m_pDevice->SetRenderState(D3DRS_STENCILREF, m_uStencilRef));
		m_sync_state.bStencilRef = FALSE;
	}

	if(m_sync_state.bDepthStencilState)
	{
		CGXDepthStencilState *pDS = (CGXDepthStencilState*)m_pDepthStencilState;
		if(!pDS)
		{
			pDS = (CGXDepthStencilState*)m_pDefaultDepthStencilState;
		}
		DX_CALL(pDS->m_pStateBlock->Apply());
		m_sync_state.bDepthStencilState = FALSE;
	}

	if(m_sync_state.bBlendState)
	{
		CGXBlendState *pBS = (CGXBlendState*)m_pBlendState;
		if(!pBS)
		{
			pBS = (CGXBlendState*)m_pDefaultBlendState;
		}
		DX_CALL(pBS->m_pStateBlock->Apply());
		m_sync_state.bBlendState = FALSE;
	}

	if(m_sync_state.bBlendFactor)
	{
		DX_CALL(m_pDevice->SetRenderState(D3DRS_BLENDFACTOR, m_blendFactor));
		m_sync_state.bBlendFactor = FALSE;
	}

	if(m_sync_state.bDepthStencilSurface)
	{
		if(m_pDepthStencilSurface)
		{
			DX_CALL(m_pDevice->SetDepthStencilSurface(((CGXDepthStencilSurface*)m_pDepthStencilSurface)->m_pSurface));
		}
		else
		{
			DX_CALL(m_pDevice->SetDepthStencilSurface(m_pDefaultDepthStencilSurface));
		}
		m_sync_state.bDepthStencilSurface = FALSE;
	}

	for(UINT i = 0; i < MAXGXCOLORTARGETS; ++i)
	{
		if(m_sync_state.bColorTarget[i])
		{
			CGXSurface *pSurface = (CGXSurface*)m_pColorTarget[i];
			if(pSurface)
			{
				DX_CALL(m_pDevice->SetRenderTarget(i, pSurface->m_pSurface));
			}
			else if(i == 0)
			{
				DX_CALL(m_pDevice->SetRenderTarget(i, m_pDefaultColorTarget));
			}
			else
			{
				DX_CALL(m_pDevice->SetRenderTarget(i, NULL));
			}

			m_sync_state.bColorTarget[i] = FALSE;
		}
	}

	//@TODO: Handle a case shader constant changed but the shader wasn't rebound

	if(m_sync_state.bShader && !(flags & GX_SYNCFLAG_NO_SHADER))
	{
		if(!m_pShader)
		{
			debugMessage(GX_LOG_ERROR, "Shader is not set!");
		}
		else
		{
			CGXShader *pShader = (CGXShader*)m_pShader;
			if(pShader->m_pVShader)
			{
				CGXVertexShader *pVS = (CGXVertexShader*)pShader->m_pVShader;
				DX_CALL(m_pDevice->SetVertexShader(pVS->m_pShader));
				if(pVS->m_uConstBuffRegCountF)
				{
					m_pDevice->SetVertexShaderConstantF(0, pVS->m_pConstBufferF, pVS->m_uConstBuffRegCountF);
				}
				if(pVS->m_uConstBuffRegCountI)
				{
					m_pDevice->SetVertexShaderConstantI(0, pVS->m_pConstBufferI, pVS->m_uConstBuffRegCountI);
				}
			}
			else
			{
				DX_CALL(m_pDevice->SetVertexShader(NULL));
			}
			if(pShader->m_pPShader)
			{
				CGXPixelShader *pPS = (CGXPixelShader*)pShader->m_pPShader;
				DX_CALL(m_pDevice->SetPixelShader(pPS->m_pShader));
				if(pPS->m_uConstBuffRegCountF)
				{
					m_pDevice->SetPixelShaderConstantF(0, pPS->m_pConstBufferF, pPS->m_uConstBuffRegCountF);
				}
				if(pPS->m_uConstBuffRegCountI)
				{
					m_pDevice->SetPixelShaderConstantI(0, pPS->m_pConstBufferI, pPS->m_uConstBuffRegCountI);
				}
			}
			else
			{
				DX_CALL(m_pDevice->SetPixelShader(NULL));
			}
		}
		m_sync_state.bShader = FALSE;
	}

	for(UINT i = 0; i < MAXGXTEXTURES; ++i)
	{
		if(m_sync_state.bTexture[i])
		{
			if(m_pTextures[i])
			{
				IDirect3DBaseTexture9 *pTex = NULL;
				switch(m_pTextures[i]->getType())
				{
				case GXTEXTURE_TYPE_2D:
					pTex = ((CGXTexture2D*)m_pTextures[i])->getDXTexture();
					break;
				case GXTEXTURE_TYPE_CUBE:
					pTex = ((CGXTextureCube*)m_pTextures[i])->getDXTexture();
					break;
				}

				DX_CALL(m_pDevice->SetTexture(i, pTex));
			}
			else
			{
				DX_CALL(m_pDevice->SetTexture(i, NULL));
			}

			m_sync_state.bColorTarget[i] = FALSE;
		}
	}
}

void CGXContext::setPrimitiveTopology(GXPT pt)
{
	switch(pt)
	{
	case GXPT_POINTLIST:
		m_drawPT = D3DPT_POINTLIST;
		break;
	case GXPT_LINELIST:
		m_drawPT = D3DPT_LINELIST;
		break;
	case GXPT_LINESTRIP:
		m_drawPT = D3DPT_LINESTRIP;
		break;
	case GXPT_TRIANGLELIST:
		m_drawPT = D3DPT_TRIANGLELIST;
		break;
	case GXPT_TRIANGLESTRIP:
		m_drawPT = D3DPT_TRIANGLESTRIP;
		break;
	/*case GXPT_TRIANGLEFAN:
		m_drawPT = D3DPT_TRIANGLEFAN;
		break;*/
	}
}

UINT CGXContext::getPTcount(UINT idxCount)
{
	switch(m_drawPT)
	{
	case D3DPT_POINTLIST:
		return(idxCount);
	case D3DPT_LINELIST:
		return(idxCount / 2);
	case D3DPT_LINESTRIP:
		return(idxCount - 1);
	case D3DPT_TRIANGLELIST:
		return(idxCount / 3);
	case D3DPT_TRIANGLESTRIP:
	case D3DPT_TRIANGLEFAN:
		return(idxCount - 2);
	}
	return(0);
}

UINT CGXContext::getIndexSize(D3DFORMAT idx)
{
	switch(idx)
	{
	case D3DFMT_INDEX16:
		return(2);
	case D3DFMT_INDEX32:
		return(4);
	}
	return(1);
}

/*IDSGshader * CGXContext::CreateShader(const char * pName, UINT flags)
{
	IDSFILE * pFile = m_pDS->FileOpen((L"/shaders/"+DStringW(DString(pName))+L".dss").c_str(), DSFM_RO | DSFM_TEXT);
	if(!pFile)
	{
		DS_ERROR((L"Unable to load shader \"" + DStringW(DString(pName)) + L"\"").c_str());
		return(NULL);
	}
	DSparser p(pFile);
	pFile->Close();
	pFile = NULL;
	DSparser * sh = p.GetNode("shader");
	if(!sh)
	{
		DS_ERROR((L"Unable to load shader \"" + DStringW(DString(pName)) + L"\"\nThere is no \"shader\" node").c_str());
		return(NULL);
	}
	DSparser * sources = sh->GetNode("source");
	if(!sources)
	{
		DS_ERROR((L"Unable to load shader \"" + DStringW(DString(pName)) + L"\"\nThere is no \"source\" node").c_str());
		return(NULL);
	}
	GLuint program = m_pGL->glCreateProgram();
	GLuint ps = 0;
	GLuint vs = 0;
	if(sources->KeyExists("vs"))
	{
		DString vsh = sources->Get("vs");
		vs = GetShaderPart(GL_VERTEX_SHADER, vsh, flags);
		m_pGL->glAttachShader(program, vs);
	}
	if(sources->KeyExists("ps"))
	{
		DString psh = sources->Get("ps");
		ps = GetShaderPart(GL_FRAGMENT_SHADER, psh, flags);
		m_pGL->glAttachShader(program, ps);
	}

	m_pGL->glLinkProgram(program);
	GLenum ErrorCheckValue = glGetError();
	if(ErrorCheckValue != GL_NO_ERROR)
	{
		DS_ERROR((L"Unable to link shader \"" + DStringW(DString(pName)) + L"\" Err: " + (DWORD)ErrorCheckValue).c_str());
		return(0);
	}
	if(vs)
	{
		m_pGL->glDetachShader(program, vs);
	}
	if(ps)
	{
		m_pGL->glDetachShader(program, ps);
	}

	IDSGshaderOgl * pShader = new IDSGshaderOgl(this);

	pShader->m_pProgram = program;

	return(pShader);
}
void CGXContext::destroyShader(IDSGshader * pSH)
{
	if(pSH)
	{
		m_pGL->glDeleteShader(((IDSGshaderOgl*)pSH)->m_pProgram);
		DSAFE_DELETE(pSH);
	}
}
void CGXContext::setShader(IDSGshader * pSH)
{
	if(pSH)
	{
		m_pGL->glUseProgram(((IDSGshaderOgl*)pSH)->m_pProgram);
	}
	else
	{
		m_pGL->glUseProgram(0);
	}
}

GLuint CGXContext::GetShaderPart(GLenum type, const DString & name, UINT flags, const char ** defs)
{
	DString _name = name;
	switch(type)
	{
	case GL_VERTEX_SHADER:
		_name += ".vsh";
		break;
	case GL_FRAGMENT_SHADER:
		_name += ".psh";
		break;
	}

	DString key = _name + "." + (int)flags;
	if(defs == NULL && m_mShaderCache.KeyExists(key))
	{
		return(m_mShaderCache[key].pShader);
	}

	const char * source;

	if(m_mShaderSources.KeyExists(_name))
	{
		source = m_mShaderSources[_name].source;
	}
	else
	{
		IDSFILE * fp = m_pDS->FileOpen((L"/shaders/" + DStringW(_name)).c_str(), DSFM_BINARY | DSFM_RO);
		if(!fp)
		{
			DS_ERROR((L"Unable to open file /shaders/" + DStringW(_name)).c_str());
			return(0);
		}
		char * _source = new char[sizeof(char) * (fp->Size() + 1)];
		_source[fp->Read((void*)_source, fp->Size())] = 0;
		source = _source;
		m_mShaderSources[_name].source = source;
	}


	const char ** pszDef = defs;
	while(pszDef)
	{
		m_pSPP->Define(*pszDef, 0, TRUE);
	}

	DString srcpp = m_pSPP->Process(source);
	m_pSPP->UndefTemp();

	GLuint pSH = m_pGL->glCreateShader(type);
	
	const char * src = srcpp.c_str();

	m_pGL->glShaderSource(pSH, 1, &src, NULL);
	m_pGL->glCompileShader(pSH);

	GLint isCompiled;
	m_pGL->glGetShaderiv(pSH, GL_COMPILE_STATUS, &isCompiled);

	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		m_pGL->glGetShaderiv(pSH, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		DString errorLog;
		errorLog.Reserve(maxLength);
		m_pGL->glGetShaderInfoLog(pSH, maxLength, &maxLength, (GLchar*)(errorLog.c_str()));

		DS_ERROR(DStringW(errorLog).c_str());

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		m_pGL->glDeleteShader(pSH); // Don't leak the shader.
		return(0);
	}

	if(defs == NULL)
	{
		m_mShaderCache[key].pShader = pSH;
	}

	return(pSH);
}*/

IGXVertexShader * CGXContext::createVertexShader(const char * szFile, GXMACRO *pDefs)
{
	ID3DXBuffer *pShaderBlob;
	ID3DXBuffer *pErrorBlob;
	ID3DXConstantTable *pConstTable;

	if(FAILED(DX_CALL(D3DXCompileShaderFromFileA(szFile, (D3DXMACRO*)pDefs, NULL, "main", "vs_3_0", SHADER_FLAGS, &pShaderBlob, &pErrorBlob, &pConstTable))))
	{
		if(pErrorBlob)
		{
			char * pBuff = (char*)pErrorBlob->GetBufferPointer();
			int s = strlen(pBuff);
			char *str = (char*)alloca(s + 33);
			sprintf(str, "Unable to create vertex shader: %s", pBuff);
			debugMessage(GX_LOG_ERROR, str);
		}
		mem_release(pErrorBlob);
		return(NULL);
	}
	mem_release(pErrorBlob);

	CGXVertexShader *pShader = new CGXVertexShader(this);

	DX_CALL(m_pDevice->CreateVertexShader((DWORD*)pShaderBlob->GetBufferPointer(), &(pShader->m_pShader)));

	pShader->parseConstantTable(pConstTable);
	
	mem_release(pShaderBlob);
	mem_release(pConstTable);
	
	return(pShader);
}
IGXVertexShader * CGXContext::createVertexShaderFromString(const char * szCode, GXMACRO *pDefs)
{
	ID3DXBuffer *pShaderBlob;
	ID3DXBuffer *pErrorBlob;
	ID3DXConstantTable *pConstTable;

	if(FAILED(DX_CALL(D3DXCompileShader(szCode, strlen(szCode), (D3DXMACRO*)pDefs, NULL, "main", "vs_3_0", SHADER_FLAGS, &pShaderBlob, &pErrorBlob, &pConstTable))))
	{
		if(pErrorBlob)
		{
			char * pBuff = (char*)pErrorBlob->GetBufferPointer();
			int s = strlen(pBuff);
			char *str = (char*)alloca(s + 33);
			sprintf(str, "Unable to create vertex shader: %s", pBuff);
			debugMessage(GX_LOG_ERROR, str);
		}
		mem_release(pErrorBlob);
		return(NULL);
	}
	mem_release(pErrorBlob);

	CGXVertexShader *pShader = new CGXVertexShader(this);

	DX_CALL(m_pDevice->CreateVertexShader((DWORD*)pShaderBlob->GetBufferPointer(), &(pShader->m_pShader)));

	pShader->parseConstantTable(pConstTable);

	mem_release(pShaderBlob);
	mem_release(pConstTable);

	return(pShader);
}
IGXVertexShader * CGXContext::createVertexShader(void *_pData, UINT uSize)
{
	CGXVertexShader *pShader = new CGXVertexShader(this);

	byte *pData = (byte*)_pData;
	UINT uConstCount = *((UINT*)pData);
	pData += sizeof(UINT);
	for(UINT i = 0; i < uConstCount; ++i)
	{
		AAString name;
		name.setName((char*)pData);
		pData += strlen(name.getName()) + 1;
		pShader->m_mConstLocations[name] = *((D3DXCONSTANT_DESC*)pData);
		pData += sizeof(D3DXCONSTANT_DESC);
	}
	pShader->m_uConstBuffRegCountF = *((UINT*)pData);
	pData += sizeof(UINT);
	pShader->m_uConstBuffRegCountI = *((UINT*)pData);
	pData += sizeof(UINT);

	if(pShader->m_uConstBuffRegCountF)
	{
		pShader->m_pConstBufferF = new float[pShader->m_uConstBuffRegCountF * 4];
		memset(pShader->m_pConstBufferF, 0, sizeof(float) * pShader->m_uConstBuffRegCountF * 4);
	}
	if(pShader->m_uConstBuffRegCountI)
	{
		pShader->m_pConstBufferI = new int[pShader->m_uConstBuffRegCountI * 4];
		memset(pShader->m_pConstBufferI, 0, sizeof(int) * pShader->m_uConstBuffRegCountI * 4);
	}
	UINT uProgramSize = *((UINT*)pData);
	pData += sizeof(UINT);

	ID3DXBuffer *pShaderBlob;
	DX_CALL(D3DXCreateBuffer(uProgramSize, &pShaderBlob));
	memcpy(pShaderBlob->GetBufferPointer(), pData, uProgramSize);
	
	DX_CALL(m_pDevice->CreateVertexShader((DWORD*)pShaderBlob->GetBufferPointer(), &(pShader->m_pShader)));

	mem_release(pShaderBlob);

	return(pShader);
}
void CGXContext::destroyVertexShader(IGXVertexShader * pSH)
{
	mem_delete(pSH);
}

IGXPixelShader * CGXContext::createPixelShader(const char * szFile, GXMACRO *pDefs)
{
	ID3DXBuffer *pShaderBlob;
	ID3DXBuffer *pErrorBlob;
	ID3DXConstantTable *pConstTable;

	if(FAILED(DX_CALL(D3DXCompileShaderFromFileA(szFile, (D3DXMACRO*)pDefs, NULL, "main", "ps_3_0", SHADER_FLAGS, &pShaderBlob, &pErrorBlob, &pConstTable))))
	{
		if(pErrorBlob)
		{
			char * pBuff = (char*)pErrorBlob->GetBufferPointer();
			int s = strlen(pBuff);
			char *str = (char*)alloca(s + 33);
			sprintf(str, "Unable to create vertex shader: %s", pBuff);
			debugMessage(GX_LOG_ERROR, str);
		}
		mem_release(pErrorBlob);
		return(NULL);
	}
	mem_release(pErrorBlob);

	CGXPixelShader *pShader = new CGXPixelShader(this);

	DX_CALL(m_pDevice->CreatePixelShader((DWORD*)pShaderBlob->GetBufferPointer(), &(pShader->m_pShader)));

	pShader->parseConstantTable(pConstTable);

	mem_release(pShaderBlob);
	mem_release(pConstTable);

	return(pShader);
}
IGXPixelShader * CGXContext::createPixelShader(void *_pData, UINT uSize)
{
	CGXPixelShader *pShader = new CGXPixelShader(this);

	byte *pData = (byte*)_pData;
	UINT uConstCount = *((UINT*)pData);
	pData += sizeof(UINT);
	for(UINT i = 0; i < uConstCount; ++i)
	{
		AAString name;
		name.setName((char*)pData);
		pData += strlen(name.getName()) + 1;
		pShader->m_mConstLocations[name] = *((D3DXCONSTANT_DESC*)pData);
		pData += sizeof(D3DXCONSTANT_DESC);
	}
	pShader->m_uConstBuffRegCountF = *((UINT*)pData);
	pData += sizeof(UINT);
	pShader->m_uConstBuffRegCountI = *((UINT*)pData);
	pData += sizeof(UINT);

	if(pShader->m_uConstBuffRegCountF)
	{
		pShader->m_pConstBufferF = new float[pShader->m_uConstBuffRegCountF * 4];
		memset(pShader->m_pConstBufferF, 0, sizeof(float) * pShader->m_uConstBuffRegCountF * 4);
	}
	if(pShader->m_uConstBuffRegCountI)
	{
		pShader->m_pConstBufferI = new int[pShader->m_uConstBuffRegCountI * 4];
		memset(pShader->m_pConstBufferI, 0, sizeof(int) * pShader->m_uConstBuffRegCountI * 4);
	}
	UINT uProgramSize = *((UINT*)pData);
	pData += sizeof(UINT);

	ID3DXBuffer *pShaderBlob;
	DX_CALL(D3DXCreateBuffer(uProgramSize, &pShaderBlob));
	memcpy(pShaderBlob->GetBufferPointer(), pData, uProgramSize);
	
	DX_CALL(m_pDevice->CreatePixelShader((DWORD*)pShaderBlob->GetBufferPointer(), &(pShader->m_pShader)));

	mem_release(pShaderBlob);

	return(pShader);
}
IGXPixelShader * CGXContext::createPixelShaderFromString(const char * szCode, GXMACRO *pDefs)
{
	ID3DXBuffer *pShaderBlob;
	ID3DXBuffer *pErrorBlob;
	ID3DXConstantTable *pConstTable;

	if(FAILED(DX_CALL(D3DXCompileShader(szCode, strlen(szCode), (D3DXMACRO*)pDefs, NULL, "main", "ps_3_0", SHADER_FLAGS, &pShaderBlob, &pErrorBlob, &pConstTable))))
	{
		if(pErrorBlob)
		{
			char * pBuff = (char*)pErrorBlob->GetBufferPointer();
			int s = strlen(pBuff);
			char *str = (char*)alloca(s + 33);
			sprintf(str, "Unable to create vertex shader: %s", pBuff);
			debugMessage(GX_LOG_ERROR, str);
		}
		mem_release(pErrorBlob);
		return(NULL);
	}
	mem_release(pErrorBlob);

	CGXPixelShader *pShader = new CGXPixelShader(this);

	DX_CALL(m_pDevice->CreatePixelShader((DWORD*)pShaderBlob->GetBufferPointer(), &(pShader->m_pShader)));

	pShader->parseConstantTable(pConstTable);

	mem_release(pShaderBlob);
	mem_release(pConstTable);

	return(pShader);
}
void CGXContext::destroyPixelShader(IGXPixelShader * pSH)
{
	mem_delete(pSH);
}

IGXShader *CGXContext::createShader(IGXVertexShader *pVS, IGXPixelShader *pPS)
{
	return(new CGXShader(this, pVS, pPS));
}
void CGXContext::destroyShader(IGXShader *pSH)
{
	if(pSH && pSH == m_pShader)
	{
		m_pShader = NULL;
		m_sync_state.bShader = TRUE;
	}
	mem_delete(pSH);
}
void CGXContext::setShader(IGXShader *pSH)
{
	if(m_pShader == pSH)
	{
		return;
	}
	mem_release(m_pShader);
	m_pShader = pSH;
	if(pSH)
	{
		pSH->AddRef();
	}
	m_sync_state.bShader = TRUE;
}
IGXShader *CGXContext::getShader()
{
	if(m_pShader)
	{
		m_pShader->AddRef();
	}
	return(m_pShader);
}

IGXRenderBuffer * CGXContext::createRenderBuffer(UINT countSlots, IGXVertexBuffer ** pBuff, IGXVertexDeclaration * pDecl)
{
	assert(countSlots);
	assert(pBuff);
	assert(pDecl);

	CGXRenderBuffer * pRB = new CGXRenderBuffer(this, countSlots, pBuff, pDecl);
	/*for(UINT i = 0; i < countSlots; ++i)
	{
		((CGXVertexBuffer*)pBuff[i])->m_pBuffer->AddRef();
	}
	((CGXVertexDeclaration*)pDecl)->m_pDeclaration->AddRef();
*/
	return(pRB);
}
void CGXContext::destroyRenderBuffer(IGXRenderBuffer * pBuff)
{
	if(pBuff)
	{
		//CGXRenderBuffer *pBuf = (CGXRenderBuffer*)pBuff;

		/*for(UINT i = 0; i < pBuf->m_uStreamCount; ++i)
		{
			mem_release(((CGXVertexBuffer*)pBuf->m_ppVertexBuffers[i])->m_pBuffer);
		}
		mem_release(((CGXVertexDeclaration*)pBuf->m_pVertexDeclaration)->m_pDeclaration);*/
	}
	mem_delete(pBuff);
}

void CGXContext::setRenderBuffer(IGXRenderBuffer * pBuff)
{
	if(m_pCurRenderBuffer == pBuff)
	{
		return;
	}
	mem_release(m_pCurRenderBuffer);
	m_pCurRenderBuffer = pBuff;
	if(pBuff)
	{
		pBuff->AddRef();
	}
	m_sync_state.bRenderBuffer = TRUE;
}

void CGXContext::debugMessage(GX_LOG lvl, const char *msg)
{
	switch(lvl)
	{
	case GX_LOG_INFO:
		OutputDebugString("[GX]:INFO:");
		break;
	case GX_LOG_WARN:
		OutputDebugString("[GX]:WARN:");
		break;
	case GX_LOG_ERROR:
		OutputDebugString("[GX]:ERR:");
		break;
	}
	OutputDebugString(msg);
	OutputDebugString("\n");
}

void CGXContext::logDXcall(const char *szCondeString, HRESULT hr)
{
	static char str[4096];
	sprintf_s(str, "GX call failed: %s\n    %s, %s", szCondeString, DXGetErrorString9(hr), DXGetErrorDescription9(hr));
	debugMessage(GX_LOG_ERROR, str);
}

IGXSamplerState *CGXContext::createSamplerState(GXSAMPLER_DESC *pSamplerDesc)
{
	CGXSamplerState *pSS = new CGXSamplerState(this);

	//DX_CALL(m_pDevice->BeginStateBlock());

	UINT uSlot = 0;
	
	pSS->m_dwCurrentStates[D3DSAMP_ADDRESSU] = pSS->getD3DTADDRESS(pSamplerDesc->addressU);
	pSS->m_dwCurrentStates[D3DSAMP_ADDRESSV] = pSS->getD3DTADDRESS(pSamplerDesc->addressV);
	pSS->m_dwCurrentStates[D3DSAMP_ADDRESSW] = pSS->getD3DTADDRESS(pSamplerDesc->addressW);
	pSS->m_dwCurrentStates[D3DSAMP_BORDERCOLOR] = GXCOLOR_COLORVALUE_V4(pSamplerDesc->f4BorderColor);
	pSS->getMagMinMipFilter(pSamplerDesc->filter, &(pSS->m_dwCurrentStates[D3DSAMP_MAGFILTER]), &(pSS->m_dwCurrentStates[D3DSAMP_MINFILTER]), &(pSS->m_dwCurrentStates[D3DSAMP_MIPFILTER]));
	pSS->m_dwCurrentStates[D3DSAMP_MIPMAPLODBIAS] = pSamplerDesc->fMipLODBias;
	pSS->m_dwCurrentStates[D3DSAMP_MAXMIPLEVEL] = pSamplerDesc->fMinLOD;
	pSS->m_dwCurrentStates[D3DSAMP_MAXANISOTROPY] = pSamplerDesc->uMaxAnisotropy;
	//m_pDevice->SetSamplerState(uSlot, D3DSAMP_ADDRESSU, pSS->getD3DTADDRESS(pSamplerDesc->addressU));

	//DX_CALL(m_pDevice->EndStateBlock(&(pSS->m_pStateBlock)));
	return(pSS);
}
void CGXContext::destroySamplerState(IGXSamplerState *pState)
{
	if(pState)
	{
		for(UINT i = 0; i < MAX_GXSAMPLERS; ++i)
		{
			if(pState == m_pSamplerState[i])
			{
				m_pSamplerState[i] = NULL;
				m_sync_state.bSamplerState[i] = TRUE;
			}
		}
	}
	mem_delete(pState);
}
void CGXContext::setSamplerState(IGXSamplerState *pState, UINT uSlot)
{
	assert(uSlot < MAX_GXSAMPLERS);
	if(uSlot >= MAX_GXSAMPLERS)
	{
		debugMessage(GX_LOG_ERROR, "Unable to set sampler state: uSlot >= MAX_GXSAMPLERS!");
		return;
	}
	if(m_pSamplerState[uSlot] == pState)
	{
		return;
	}
	mem_release(m_pSamplerState[uSlot]);
	m_pSamplerState[uSlot] = pState;
	if(pState)
	{
		pState->AddRef();
	}
	m_sync_state.bSamplerState[uSlot] = TRUE;
}
IGXSamplerState *CGXContext::getSamplerState(UINT uSlot)
{
	assert(uSlot < MAX_GXSAMPLERS);
	if(m_pSamplerState[uSlot])
	{
		m_pSamplerState[uSlot]->AddRef();
	}
	return(m_pSamplerState[uSlot]);
}

IGXRasterizerState *CGXContext::createRasterizerState(GXRASTERIZER_DESC *pRSDesc)
{
	CGXRasterizerState *pRS = new CGXRasterizerState(this);

	DX_CALL(m_pDevice->BeginStateBlock());

	switch(pRSDesc->fillMode)
	{
	case GXFILL_SOLID:
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		break;
	case GXFILL_WIREFRAME:
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		break;
	}
	
	switch(pRSDesc->cullMode)
	{
	case GXCULL_NONE:
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		break;
	case GXCULL_FRONT:
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		break;
	case GXCULL_BACK:
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		break;
	}

	//m_pDevice->SetRenderState(D3DRS_DEPTHBIAS, pRSDesc->iDepthBias); // ?
	m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, pRSDesc->bScissorEnable);

	DX_CALL(m_pDevice->EndStateBlock(&(pRS->m_pStateBlock)));
	return(pRS);
}
void CGXContext::destroyRasterizerState(IGXRasterizerState *pState)
{
	if(pState)
	{
		if(pState == m_pRasterizerState)
		{
			m_pRasterizerState = NULL;
			m_sync_state.bRasterizerState = TRUE;
		}
	}
	mem_delete(pState);
}
void CGXContext::setRasterizerState(IGXRasterizerState *pState)
{
	if(m_pRasterizerState == pState)
	{
		return;
	}
	mem_release(m_pRasterizerState);
	m_pRasterizerState = pState;
	if(pState)
	{
		pState->AddRef();
	}
	m_sync_state.bRasterizerState = TRUE;
}
IGXRasterizerState *CGXContext::getRasterizerState()
{
	if(m_pRasterizerState)
	{
		m_pRasterizerState->AddRef();
	}
	return(m_pRasterizerState);
}


IGXDepthStencilState *CGXContext::createDepthStencilState(GXDEPTH_STENCIL_DESC *pDSDesc)
{
	CGXDepthStencilState *pDS = new CGXDepthStencilState(this);

	DX_CALL(m_pDevice->BeginStateBlock());
	m_pDevice->SetRenderState(D3DRS_ZENABLE, pDSDesc->bDepthEnable ? D3DZB_TRUE : D3DZB_FALSE);
	if(pDSDesc->bDepthEnable)
	{
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, pDSDesc->bEnableDepthWrite);
		m_pDevice->SetRenderState(D3DRS_ZFUNC, pDSDesc->depthFunc);
	}
	m_pDevice->SetRenderState(D3DRS_STENCILENABLE, pDSDesc->bStencilEnable);
	if(pDSDesc->bStencilEnable)
	{
		m_pDevice->SetRenderState(D3DRS_STENCILMASK, (DWORD)pDSDesc->u8StencilReadMask);
		m_pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, (DWORD)pDSDesc->u8StencilWriteMask);
		m_pDevice->SetRenderState(D3DRS_STENCILFAIL, pDSDesc->stencilFailOp);
		m_pDevice->SetRenderState(D3DRS_STENCILZFAIL, pDSDesc->stencilDepthFailOp);
		m_pDevice->SetRenderState(D3DRS_STENCILPASS, pDSDesc->stencilPassOp);
		m_pDevice->SetRenderState(D3DRS_STENCILFUNC, pDSDesc->stencilFunc);
	}

	DX_CALL(m_pDevice->EndStateBlock(&(pDS->m_pStateBlock)));
	return(pDS);
}
void CGXContext::destroyDepthStencilState(IGXDepthStencilState *pState)
{
	mem_delete(pState);
}
void CGXContext::setDepthStencilState(IGXDepthStencilState *pState)
{
	if(m_pDepthStencilState == pState)
	{
		return;
	}
	mem_release(m_pDepthStencilState);
	m_pDepthStencilState = pState;
	if(pState)
	{
		pState->AddRef();
	}
	m_sync_state.bDepthStencilState = TRUE;
}
IGXDepthStencilState *CGXContext::getDepthStencilState()
{
	if(m_pDepthStencilState)
	{
		m_pDepthStencilState->AddRef();
	}
	return(m_pDepthStencilState);
}
void CGXContext::setStencilRef(UINT uVal)
{
	m_uStencilRef = uVal;
	m_sync_state.bStencilRef = TRUE;
}


IGXBlendState *CGXContext::createBlendState(GXBLEND_DESC *pBSDesc)
{
	CGXBlendState *pBS = new CGXBlendState(this);
	if(pBSDesc->bAlphaToCoverageEnable)
	{
		debugMessage(GX_LOG_WARN, "Ignoring bAlphaToCoverageEnable blend desc!");
	}
	if(pBSDesc->bIndependentBlendEnabled)
	{
		debugMessage(GX_LOG_WARN, "Ignoring bIndependentBlendEnabled blend desc!");
	}

	DX_CALL(m_pDevice->BeginStateBlock());
	
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, pBSDesc->renderTarget[0].bBlendEnable);
	if(pBSDesc->renderTarget[0].bBlendEnable)
	{
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, pBSDesc->renderTarget[0].srcBlend);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, pBSDesc->renderTarget[0].destBlend);
		m_pDevice->SetRenderState(D3DRS_BLENDOP, pBSDesc->renderTarget[0].blendOp);

		if(pBSDesc->renderTarget[0].srcBlend != pBSDesc->renderTarget[0].srcBlendAlpha ||
			pBSDesc->renderTarget[0].destBlend != pBSDesc->renderTarget[0].destBlendAlpha ||
			pBSDesc->renderTarget[0].blendOp != pBSDesc->renderTarget[0].blendOpAlpha)
		{
			m_pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);

			m_pDevice->SetRenderState(D3DRS_SRCBLENDALPHA, pBSDesc->renderTarget[0].srcBlendAlpha);
			m_pDevice->SetRenderState(D3DRS_DESTBLENDALPHA, pBSDesc->renderTarget[0].destBlendAlpha);
			m_pDevice->SetRenderState(D3DRS_BLENDOPALPHA, pBSDesc->renderTarget[0].blendOpAlpha);
		}
		else
		{
			m_pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
		}
	}
	m_pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, pBSDesc->renderTarget[0].u8RenderTargetWriteMask);

	

	DX_CALL(m_pDevice->EndStateBlock(&(pBS->m_pStateBlock)));
	return(pBS);
}
void CGXContext::destroyBlendState(IGXBlendState *pState)
{
	mem_delete(pState);
}
void CGXContext::setBlendState(IGXBlendState *pState)
{
	if(m_pBlendState == pState)
	{
		return;
	}
	mem_release(m_pBlendState);
	m_pBlendState = pState;
	if(pState)
	{
		pState->AddRef();
	}
	m_sync_state.bBlendState = TRUE;
}
IGXBlendState *CGXContext::getBlendState()
{
	if(m_pBlendState)
	{
		m_pBlendState->AddRef();
	}
	return(m_pBlendState);
}

void CGXContext::setBlendFactor(GXCOLOR val)
{
	m_blendFactor = val;
	m_sync_state.bBlendFactor = TRUE;
}

IGXDepthStencilSurface *CGXContext::createDepthStencilSurface(UINT uWidth, UINT uHeight, GXFORMAT format, GXMULTISAMPLE_TYPE multisampleType, bool bAutoResize)
{
	CGXDepthStencilSurface *pDSSurface = new CGXDepthStencilSurface(this);

	pDSSurface->m_multisampleType = (D3DMULTISAMPLE_TYPE)multisampleType;
	pDSSurface->m_format = getDXFormat(format);

	pDSSurface->m_uWidth = uWidth;
	pDSSurface->m_uHeight = uHeight;

	DX_CALL(m_pDevice->CreateDepthStencilSurface(uWidth, uHeight, pDSSurface->m_format, pDSSurface->m_multisampleType, 0, FALSE, &(pDSSurface->m_pSurface), NULL));

	m_aResettableDSSurfaces.push_back(pDSSurface);

	if(bAutoResize)
	{
		pDSSurface->m_bAutoResize = true;

		pDSSurface->m_fSizeCoeffH = (float)uHeight / (float)m_oD3DAPP.BackBufferHeight;
		pDSSurface->m_fSizeCoeffW = (float)uWidth / (float)m_oD3DAPP.BackBufferWidth;
	}

	return(pDSSurface);
}
void CGXContext::destroyDepthStencilSurface(IGXDepthStencilSurface *pSurface)
{
	if(pSurface)
	{
		if(pSurface == m_pDepthStencilSurface)
		{
			setDepthStencilSurface(NULL);
		}

		for(UINT i = 0, l = m_aResettableDSSurfaces.size(); i < l; ++i)
		{
			if(m_aResettableDSSurfaces[i] == pSurface)
			{
				m_aResettableDSSurfaces.erase(i);
				break;
			}
		}

		mem_delete(pSurface);
	}
}
void CGXContext::setDepthStencilSurface(IGXDepthStencilSurface *pSurface)
{
	if(m_pDepthStencilSurface == pSurface)
	{
		return;
	}
	mem_release(m_pDepthStencilSurface);
	m_pDepthStencilSurface = pSurface;
	if(pSurface)
	{
		pSurface->AddRef();
	}
	m_sync_state.bDepthStencilSurface = TRUE;
}
IGXDepthStencilSurface *CGXContext::getDepthStencilSurface()
{
	if(m_pDepthStencilSurface)
	{
		m_pDepthStencilSurface->AddRef();
	}
	return(m_pDepthStencilSurface);
}

void CGXContext::setColorTarget(IGXSurface *pSurf, UINT idx)
{
	assert(idx < MAXGXCOLORTARGETS);
	if(m_pColorTarget[idx] == pSurf)
	{
		return;
	}
	mem_release(m_pColorTarget[idx]);
	m_pColorTarget[idx] = pSurf;
	if(pSurf)
	{
		pSurf->AddRef();
	}
	m_sync_state.bColorTarget[idx] = TRUE;
}

IGXSurface *CGXContext::getColorTarget(UINT idx)
{
	assert(idx < MAXGXCOLORTARGETS);
	if(m_pColorTarget[idx])
	{
		m_pColorTarget[idx]->AddRef();
	}
	return(m_pColorTarget[idx]);
}

void CGXContext::setTexture(IGXBaseTexture *pTexture, UINT uStage)
{
	assert(uStage < MAXGXTEXTURES);
	if(m_pTextures[uStage] == pTexture)
	{
		return;
	}
	mem_release(m_pTextures[uStage]);
	m_pTextures[uStage] = pTexture;
	if(pTexture)
	{
		pTexture->AddRef();
	}
	m_sync_state.bTexture[uStage] = TRUE;
}
IGXBaseTexture *CGXContext::getTexture(UINT uStage)
{
	assert(uStage < MAXGXTEXTURES);
	if(m_pTextures[uStage])
	{
		m_pTextures[uStage]->AddRef();
	}
	return(m_pTextures[uStage]);
}

IGXTexture2D *CGXContext::createTexture2D(UINT uWidth, UINT uHeight, UINT uMipLevels, UINT uTexUsageFlags, GXFORMAT format, void * pInitData)
{
	CGXTexture2D *pTex = new CGXTexture2D(this);

	pTex->m_format = format;
	pTex->m_uHeight = uHeight;
	pTex->m_uWidth = uWidth;
	pTex->m_bWasReset = true;
	pTex->m_bAutoResize = !!(uTexUsageFlags & GX_TEXUSAGE_AUTORESIZE);
	pTex->m_uMipLevels = uMipLevels;
	if(pTex->m_bAutoResize)
	{
		pTex->m_fSizeCoeffH = (float)uHeight / (float)m_oD3DAPP.BackBufferHeight;
		pTex->m_fSizeCoeffW = (float)uWidth / (float)m_oD3DAPP.BackBufferWidth;
	}

	UINT usage = 0;
	D3DPOOL pool = D3DPOOL_MANAGED;

	if(uTexUsageFlags & GX_TEXUSAGE_ALLOWDISCARD)
	{
		pool = D3DPOOL_DEFAULT;
	}
	if(uTexUsageFlags & GX_TEXUSAGE_RENDERTARGET)
	{
		pool = D3DPOOL_DEFAULT;
		usage |= D3DUSAGE_RENDERTARGET;
	}
	if(uTexUsageFlags & GX_TEXUSAGE_AUTOGENMIPMAPS)
	{
		usage |= D3DUSAGE_AUTOGENMIPMAP;
	}
	pTex->m_uUsage = usage;
	if(FAILED(DX_CALL(m_pDevice->CreateTexture(uWidth, uHeight, uMipLevels, usage, getDXFormat(format), pool, &pTex->m_pTexture, NULL))))
	{
		mem_delete(pTex);
		return(NULL);
	}

	if(pool == D3DPOOL_DEFAULT)
	{
		pTex->m_isResettable = true;
		m_aResettableTextures2D.push_back(pTex);
	}

	if(!(usage & GX_TEXUSAGE_RENDERTARGET) && pInitData)
	{
		if(pool != D3DPOOL_DEFAULT)
		{
			D3DLOCKED_RECT lr;
			if(!FAILED(DX_CALL(pTex->m_pTexture->LockRect(0, &lr, NULL, NULL))))
			{
				memcpy(lr.pBits, pInitData, getTextureDataSize(lr.Pitch, uHeight, format));
				pTex->m_pTexture->UnlockRect(0);
			}
		}
		else
		{
			IDirect3DTexture9 * tex = NULL;
			if(!FAILED(DX_CALL(m_pDevice->CreateTexture(uWidth, uHeight, 0, NULL, getDXFormat(format), D3DPOOL_SYSTEMMEM, &tex, NULL))))
			{
				D3DLOCKED_RECT lr;

				if(!FAILED(DX_CALL(tex->LockRect(0, &lr, NULL, NULL))))
				{
					memcpy(lr.pBits, pInitData, getTextureDataSize(lr.Pitch, uHeight, format));
					tex->UnlockRect(0);

					DX_CALL(m_pDevice->UpdateTexture(tex, pTex->m_pTexture));
				}
			}
			mem_release(tex);
		}

		pTex->m_bWasReset = false;
	}

	return(pTex);
}
IGXTextureCube *CGXContext::createTextureCube(UINT uSize, UINT uMipLevels, UINT uTexUsageFlags, GXFORMAT format, void * pInitData)
{
	CGXTextureCube *pTex = new CGXTextureCube(this);

	pTex->m_format = format;
	pTex->m_uSize = uSize;
	pTex->m_bWasReset = true;
	pTex->m_bAutoResize = !!(uTexUsageFlags & GX_TEXUSAGE_AUTORESIZE); 
	pTex->m_uMipLevels = uMipLevels;
	if(pTex->m_bAutoResize)
	{
		pTex->m_fSizeCoeff = (float)uSize / (float)m_oD3DAPP.BackBufferHeight;
	}

	UINT usage = 0;
	D3DPOOL pool = D3DPOOL_MANAGED;

	if(uTexUsageFlags & GX_TEXUSAGE_ALLOWDISCARD)
	{
		pool = D3DPOOL_DEFAULT;
	}
	if(uTexUsageFlags & GX_TEXUSAGE_RENDERTARGET)
	{
		pool = D3DPOOL_DEFAULT;
		usage |= D3DUSAGE_RENDERTARGET;
	}
	if(uTexUsageFlags & GX_TEXUSAGE_AUTOGENMIPMAPS)
	{
		usage |= D3DUSAGE_AUTOGENMIPMAP;
	}
	pTex->m_uUsage = usage;
	if(FAILED(DX_CALL(m_pDevice->CreateCubeTexture(uSize, uMipLevels, usage, getDXFormat(format), pool, &pTex->m_pTexture, NULL))))
	{
		mem_delete(pTex);
		return(NULL);
	}

	if(pool == D3DPOOL_DEFAULT)
	{
		pTex->m_isResettable = true;
		m_aResettableTexturesCube.push_back(pTex);
	}

	debugMessage(GX_LOG_WARN, "Not implemented: Unable to implace init cube texture");

	return(pTex);
}
void CGXContext::destroyTexture2D(IGXTexture2D * pTexture)
{
	if(pTexture)
	{
		for(UINT i = 0; i < MAXGXTEXTURES; ++i)
		{
			if(pTexture == m_pTextures[i])
			{
				setTexture(NULL, i);
			}
		}

		if(((CGXTexture2D*)pTexture)->m_isResettable)
		{
			for(UINT i = 0, l = m_aResettableTextures2D.size(); i < l; ++i)
			{
				if(m_aResettableTextures2D[i] == pTexture)
				{
					m_aResettableTextures2D.erase(i);
					break;
				}
			}
		}
	}
	mem_delete(pTexture);
}
void CGXContext::destroyTextureCube(IGXTextureCube * pTexture)
{
	if(pTexture)
	{
		for(UINT i = 0; i < MAXGXTEXTURES; ++i)
		{
			if(pTexture == m_pTextures[i])
			{
				setTexture(NULL, i);
			}
		}

		if(((CGXTextureCube*)pTexture)->m_isResettable)
		{
			for(UINT i = 0, l = m_aResettableTexturesCube.size(); i < l; ++i)
			{
				if(m_aResettableTexturesCube[i] == pTexture)
				{
					m_aResettableTexturesCube.erase(i);
					break;
				}
			}
		}
	}
	mem_delete(pTexture);
}

UINT CGXContext::getTextureDataSize(UINT uPitch, UINT uHeight, GXFORMAT format)
{
	bool bc = true;
	int bcnumBytesPerBlock = 16;

	switch(format)
	{
	case GXFMT_DXT1:
		bc = true;
		bcnumBytesPerBlock = 8;
		break;
	case GXFMT_DXT3:
		bc = true;
		break;
	case GXFMT_DXT5:
		bc = true;
		break;

	default:
		bc = false;
	}

	if(bc)
	{
		int numBlocksHigh = 0;
		if(uHeight > 0)
		{
			numBlocksHigh = max(1, uHeight / 4);
		}
		return(uPitch * numBlocksHigh);
	}
	else
	{
		UINT bpp = getBitsPerPixel(format);
		//rowBytes = (width * bpp + 7) / 8; // round up to nearest byte
		return(uPitch * uHeight);
	}
}

UINT CGXContext::getBitsPerPixel(GXFORMAT format)
{
	switch(format)
	{
	case GXFMT_A32B32G32R32F:  
		return(128);

	case GXFMT_G32R32F:
	case GXFMT_A16B16G16R16F:
	case GXFMT_A16B16G16R16:
		return(64);

	case GXFMT_R32F:  
	case GXFMT_G16R16F:
	case GXFMT_D24X8:
	case GXFMT_D24S8:
	case GXFMT_D32:
	case GXFMT_X8R8G8B8:
	case GXFMT_A8R8G8B8:
		return(32);

	case GXFMT_R8G8B8:
		return(24);

	case GXFMT_D16:
	case GXFMT_R16F:
	case GXFMT_R5G6B5:
		return(16);

	case GXFMT_DXT1:
		return(4);

	case GXFMT_DXT3:
	case GXFMT_DXT5:
		return(8);
	}
	return(0);
}

IGXTexture2D *CGXContext::createTexture2DFromFile(const char *szFileName, UINT uTexUsageFlags, bool bAllowNonPowerOf2)
{
	CGXTexture2D *pTex = new CGXTexture2D(this);

	UINT uSize = bAllowNonPowerOf2 ? D3DX_DEFAULT_NONPOW2 : D3DX_DEFAULT;
	D3DXIMAGE_INFO info;
	if(FAILED(DX_CALL(D3DXCreateTextureFromFileExA(m_pDevice, szFileName, uSize, uSize, D3DX_DEFAULT, 0, D3DFMT_FROM_FILE, (uTexUsageFlags & GX_TEXUSAGE_ALLOWDISCARD) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &(pTex->m_pTexture)))))
	{
		mem_delete(pTex);
		return(NULL);
	}

	if(uTexUsageFlags & GX_TEXUSAGE_ALLOWDISCARD)
	{
		pTex->m_isResettable = true;
		m_aResettableTextures2D.push_back(pTex);
	}

	pTex->m_uHeight = info.Height;
	pTex->m_uWidth = info.Width;
	pTex->m_format = getGXFormat(info.Format);
	pTex->m_bWasReset = false;

	return(pTex);
}

IGXTextureCube *CGXContext::createTextureCubeFromFile(const char *szFileName, UINT uTexUsageFlags, bool bAllowNonPowerOf2)
{
	CGXTextureCube *pTex = new CGXTextureCube(this);

	UINT uSize = bAllowNonPowerOf2 ? D3DX_DEFAULT_NONPOW2 : D3DX_DEFAULT;
	D3DXIMAGE_INFO info;
	if(FAILED(DX_CALL(D3DXCreateCubeTextureFromFileExA(m_pDevice, szFileName, uSize, D3DX_DEFAULT, 0, D3DFMT_FROM_FILE, (uTexUsageFlags & GX_TEXUSAGE_ALLOWDISCARD) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &info, NULL, &(pTex->m_pTexture)))))
	{
		mem_delete(pTex);
		return(NULL);
	}

	if(uTexUsageFlags & GX_TEXUSAGE_ALLOWDISCARD)
	{
		pTex->m_isResettable = true;
		m_aResettableTexturesCube.push_back(pTex);
	}

	pTex->m_uSize = max(info.Height, info.Width);
	pTex->m_format = getGXFormat(info.Format);
	pTex->m_bWasReset = false;

	return(pTex);
}

const D3DCAPS9 *CGXContext::getDXDeviceCaps()
{
	return(&m_dxCaps);
}

D3DFORMAT CGXContext::getDXFormat(GXFORMAT format)
{
	return((D3DFORMAT)format);
}
GXFORMAT CGXContext::getGXFormat(D3DFORMAT format)
{
	return((GXFORMAT)format);
}

IDirect3DDevice9 *CGXContext::getDXDevice()
{
	return(m_pDevice);
}

GXTEXTURE_TYPE CGXContext::getTextureTypeFromFile(const char *szFile)
{
	D3DXIMAGE_INFO oTexInfo;
	memset(&oTexInfo, 0, sizeof(D3DXIMAGE_INFO));

	if(!FAILED(DX_CALL(D3DXGetImageInfoFromFileA(szFile, &oTexInfo))))
	{
		switch(oTexInfo.ResourceType)
		{
		case D3DRTYPE_TEXTURE:
			return(GXTEXTURE_TYPE_2D);
		case D3DRTYPE_CUBETEXTURE:
			return(GXTEXTURE_TYPE_CUBE);
		}
	}

	return(GXTEXTURE_TYPE_UNKNOWN);
}
