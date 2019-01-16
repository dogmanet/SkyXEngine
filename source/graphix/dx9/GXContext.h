#ifndef _CGXContext_H_
#define _CGXContext_H_

#include <graphix/graphix.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#if defined(_WINDOWS)
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	pragma comment(lib, "d3d9.lib")
#	pragma comment(lib, "DxErr9.lib")
#	pragma comment(lib, "d3dx9.lib")
#endif

class CGXContext: public IGXContext
{
public:
	BOOL initContext(SXWINDOW wnd, int iWidth, int iHeight, bool isWindowed);
	CGXContext();
	~CGXContext();

	void swapBuffers();

	void beginFrame();
	void endFrame();

	void setClearColor(const float4_t & color);
	void clearTarget();
	void clearDepth(float val = 1.0f);
	void clearStencil(UINT val = 0);

	IGXVertexBuffer * createVertexBuffer(size_t size, UINT flags, void * pInitData = NULL);
	IGXIndexBuffer * createIndexBuffer(size_t size, UINT flags, GXINDEXTYPE it, void * pInitData = NULL);


	void destroyIndexBuffer(IGXIndexBuffer * pBuff);
	void destroyVertexBuffer(IGXVertexBuffer * pBuff);

	IGXVertexDeclaration * createVertexDeclaration(const GXVERTEXELEMENT * pDecl);
	void destroyVertexDeclaration(IGXVertexDeclaration * pDecl);

	void setIndexBuffer(IGXIndexBuffer * pBuff);
	void setRenderBuffer(IGXRenderBuffer * pBuff);
	//void setVertexBuffers(UINT startSlot, UINT countSlots, IDSRvertexBuffer ** pBuff);

	//void setInputLayout(IDSGvertexDeclaration * pDecl);

	void drawIndexed(UINT uVertexCount, UINT uPrimitiveCount, UINT uStartIndexLocation, int iBaseVertexLocation);
	void drawPrimitive(UINT uStartVertex, UINT uPrimitiveCount);

	void setPrimitiveTopology(GXPT pt);

	//IGXShader * createShader(const char * pName, UINT flags = 0);
	//void destroyShader(IGXShader * pSH);
	//void setShader(IGXShader * pSH);

	IGXVertexShader * createVertexShader(const char * szFile);
	IGXVertexShader * createVertexShader(void *pData, UINT uSize);
	void destroyVertexShader(IGXVertexShader * pSH);

	IGXPixelShader * createPixelShader(const char * szFile);
	IGXPixelShader * createPixelShader(void *pData, UINT uSize);
	void destroyPixelShader(IGXPixelShader * pSH);

	void setVertexShader(IGXVertexShader * pSH);
	void setPixelShader(IGXPixelShader * pSH);

	IGXRenderBuffer * createRenderBuffer(UINT countSlots, IGXVertexBuffer ** pBuff, IGXVertexDeclaration * pDecl);
	void destroyRenderBuffer(IGXRenderBuffer * pDecl);
protected:
	
	IDirect3D9 *m_pD3D = NULL;
	IDirect3DDevice9 *m_pDevice = NULL;
	D3DPRESENT_PARAMETERS m_oD3DAPP;
	D3DCAPS9 m_dxCaps;

	HWND m_hWnd;

	D3DCOLOR m_clearColor;

	//IDSRvertexBuffer * m_pCurVertexBuffer[MAXDSGVSTREAM];
	IGXRenderBuffer * m_pCurRenderBuffer;
	IGXIndexBuffer * m_pCurIndexBuffer;

	IGXVertexDeclaration * m_pCurVertexDecl;
	D3DPRIMITIVETYPE m_drawPT;

	struct _sync_state
	{
		//BOOL bVertexLayout;
		BOOL bIndexBuffer;
		BOOL bRenderBuffer;
		//BOOL bVertexBuffers[MAXDSGVSTREAM];
	};

	_sync_state m_sync_state;

	void syncronize();
	UINT getPTcount(UINT idxCount);
	UINT getIndexSize(D3DFORMAT idx);
};

#endif
