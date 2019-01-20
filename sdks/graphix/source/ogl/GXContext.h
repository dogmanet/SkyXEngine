#ifndef _CGXContext_H_
#define _CGXContext_H_

#include <graphix/graphix.h>

#include "GLPFN.h"

#if defined(_WINDOWS)
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	pragma comment(lib, "opengl32.lib")
#endif

class CGXContext: public IGXContext
{
public:
	BOOL initContext(SXWINDOW wnd);
	CGXContext();
	~CGXContext();

	void swapBuffers();

	void beginFrame()
	{
	};
	void endFrame()
	{
	};

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

	void drawIndexed(UINT uIndexCount, UINT uStartIndexLocation, int iBaseVertexLocation);

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

	IDSRGLPFN * m_pGL;

protected:
	
#if defined(_WINDOWS)
	HDC m_hDC;
	HGLRC m_hRC;
#endif

	SXWINDOW m_hWnd;

	//IDSRvertexBuffer * m_pCurVertexBuffer[MAXDSGVSTREAM];
	IGXRenderBuffer * m_pCurRenderBuffer;
	IGXIndexBuffer * m_pCurIndexBuffer;

	IGXVertexDeclaration * m_pCurVertexDecl;
	GLenum m_drawPT;

	struct _sync_state
	{
		//BOOL bVertexLayout;
		BOOL bIndexBuffer;
		BOOL bRenderBuffer;
		//BOOL bVertexBuffers[MAXDSGVSTREAM];
	};

	_sync_state m_sync_state;

	//struct _shcache_item
	//{
	//	GLuint pShader;
	//};

	//AssotiativeArray<DString, _shcache_item> m_mShaderCache;

	//struct _shader_source
	//{
	//	const char * source;
	//};

	//AssotiativeArray<DString, _shader_source> m_mShaderSources;

	//DSourcePreprocessor * m_pSPP;

	void syncronize();
	UINT getPTcount(UINT idxCount);
	UINT getIndexSize(UINT idx);
	GLuint getShaderPart(GLenum type, const char *szName, UINT flags = 0, const char ** defs = NULL);
};

#endif
