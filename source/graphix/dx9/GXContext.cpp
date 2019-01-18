#include "GXContext.h"

#include "GXIndexBuffer.h"
#include "GXVertexBuffer.h"
#include "GXVertexDeclaration.h"
#include "GXShader.h"
#include "GXRenderBuffer.h"

CGXContext::CGXContext():
	m_pCurIndexBuffer(NULL),
	m_drawPT(D3DPT_TRIANGLELIST),
	m_pCurRenderBuffer(NULL)
{
	memset(&m_sync_state, 0, sizeof(m_sync_state));
}

void CGXContext::beginFrame()
{
	m_pDevice->BeginScene();
}
void CGXContext::endFrame()
{
	m_pDevice->EndScene();
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

	return(TRUE);
}

CGXContext::~CGXContext()
{
	mem_release(m_pDevice);
	mem_release(m_pD3D);
}


void CGXContext::swapBuffers()
{
	m_pDevice->Present(0, 0, 0, 0);
}

void CGXContext::setClearColor(const float4_t & color)
{
	//m_clearColor = color;
	float4_t clr = color * 256.0f;
	m_clearColor = D3DCOLOR_RGBA((byte)clr.x, (byte)clr.y, (byte)clr.z, (byte)clr.w);
}
void CGXContext::clearTarget()
{
	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET, m_clearColor, 1.0f, 0);
}
void CGXContext::clearDepth(float val)
{
	m_pDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, m_clearColor, val, 0);
}
void CGXContext::clearStencil(UINT val)
{
	m_pDevice->Clear(0, 0, D3DCLEAR_STENCIL, m_clearColor, 1.0f, val);
}



IGXVertexBuffer * CGXContext::createVertexBuffer(size_t size, UINT flags, void * pInitData)
{
	CGXVertexBuffer * pBuff = new CGXVertexBuffer(this);

	UINT usage = 0;
	D3DPOOL pool = D3DPOOL_MANAGED;

	if(flags & GX_BUFFER_USAGE_STATIC)
	{
		//usage = 0;
	}
	if(flags & GX_BUFFER_USAGE_DYNAMIC)
	{
		usage = D3DUSAGE_DYNAMIC;
		pool = D3DPOOL_DEFAULT;
	}
	if(flags & GX_BUFFER_USAGE_STREAM)
	{
		pool = D3DPOOL_DEFAULT;
		usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
	}
	if(flags & GX_BUFFER_WRITEONLY)
	{
		usage = D3DUSAGE_WRITEONLY;
	}

	pBuff->m_uSize = size;
	if(FAILED(m_pDevice->CreateVertexBuffer(size, usage, 0, pool, &pBuff->m_pBuffer, NULL)))
	{
		mem_delete(pBuff);
		return(NULL);
	}

	if(usage & D3DUSAGE_DYNAMIC)
	{
		pBuff->m_uLockFlagsWrite = D3DLOCK_DISCARD;
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
	D3DPOOL pool = D3DPOOL_MANAGED;

	if(flags & GX_BUFFER_USAGE_STATIC)
	{
		//usage = 0;
	}
	if(flags & GX_BUFFER_USAGE_DYNAMIC)
	{
		usage = D3DUSAGE_DYNAMIC;
		pool = D3DPOOL_DEFAULT;
	}
	if(flags & GX_BUFFER_USAGE_STREAM)
	{
		pool = D3DPOOL_DEFAULT;
		usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
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
	if(FAILED(m_pDevice->CreateIndexBuffer(size, usage, pBuff->m_format, pool, &pBuff->m_pBuffer, NULL)))
	{
		mem_delete(pBuff);
		return(NULL);
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
	}
	mem_delete(pBuff);
}

void CGXContext::destroyVertexBuffer(IGXVertexBuffer * pBuff)
{
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
	m_pCurIndexBuffer = pBuff;
	m_sync_state.bIndexBuffer = TRUE;
}

void CGXContext::drawIndexed(UINT uVertexCount, UINT uPrimitiveCount, UINT uStartIndexLocation, int iBaseVertexLocation)
{
	syncronize();
	
	m_pDevice->DrawIndexedPrimitive(m_drawPT, iBaseVertexLocation, 0, uVertexCount, uStartIndexLocation, uPrimitiveCount);
}

void CGXContext::drawPrimitive(UINT uStartVertex, UINT uPrimitiveCount)
{
	syncronize();

	m_pDevice->DrawPrimitive(m_drawPT, uStartVertex, uPrimitiveCount);
}

void CGXContext::drawIndexedInstanced(UINT uInstanceCount, UINT uVertexCount, UINT uPrimitiveCount, UINT uStartIndexLocation, int iBaseVertexLocation)
{
	syncronize();
	// get from current vertex declaration numbers of vertex and instance stages

	m_pDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | uInstanceCount)); // mesh data
	m_pDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1)); // instance data

	m_pDevice->DrawIndexedPrimitive(m_drawPT, iBaseVertexLocation, 0, uVertexCount, uStartIndexLocation, uPrimitiveCount);

	m_pDevice->SetStreamSourceFreq(0, 1);
	m_pDevice->SetStreamSourceFreq(1, 1);
}
void CGXContext::drawPrimitiveInstanced(UINT uInstanceCount, UINT uStartVertex, UINT uPrimitiveCount)
{
	syncronize();

	m_pDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | uInstanceCount)); // mesh data
	m_pDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1)); // instance data

	m_pDevice->DrawPrimitive(m_drawPT, uStartVertex, uPrimitiveCount);

	m_pDevice->SetStreamSourceFreq(0, 1);
	m_pDevice->SetStreamSourceFreq(1, 1);
}

void CGXContext::syncronize()
{
	if(m_sync_state.bRenderBuffer)
	{
		if(m_pCurRenderBuffer)
		{
			CGXRenderBuffer *pRB = (CGXRenderBuffer*)m_pCurRenderBuffer;
			CGXVertexDeclaration *pVD = (CGXVertexDeclaration*)pRB->m_pVertexDeclaration;
			assert(pVD->m_u8StreamCount == pRB->m_uStreamCount);

			m_pDevice->SetVertexDeclaration(pVD->m_pDeclaration);
			for(UINT i = 0; i < pRB->m_uStreamCount; ++i)
			{
				m_pDevice->SetStreamSource(i, ((CGXVertexBuffer*)pRB->m_ppVertexBuffers[i])->m_pBuffer, 0, pVD->m_u8StreamStride[i]);
			}
		}

		m_sync_state.bRenderBuffer = FALSE;
	}
	if(m_sync_state.bIndexBuffer)
	{
		m_pDevice->SetIndices(m_pCurIndexBuffer ? ((CGXIndexBuffer*)m_pCurIndexBuffer)->m_pBuffer : 0);
		m_sync_state.bIndexBuffer = FALSE;
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
	case GXPT_TRIANGLEFAN:
		m_drawPT = D3DPT_TRIANGLEFAN;
		break;
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

IGXVertexShader * CGXContext::createVertexShader(const char * szFile)
{
	assert(!"Not imlemented");
	return(NULL);
}
IGXVertexShader * CGXContext::createVertexShader(void *pData, UINT uSize)
{
	assert(!"Not imlemented");
	return(NULL);
}
void CGXContext::destroyVertexShader(IGXVertexShader * pSH)
{
	assert(!"Not imlemented");
}

IGXPixelShader * CGXContext::createPixelShader(const char * szFile)
{
	assert(!"Not imlemented");
	return(NULL);
}
IGXPixelShader * CGXContext::createPixelShader(void *pData, UINT uSize)
{
	assert(!"Not imlemented");
	return(NULL);
}
void CGXContext::destroyPixelShader(IGXPixelShader * pSH)
{
	assert(!"Not imlemented");
}

IGXRenderBuffer * CGXContext::createRenderBuffer(UINT countSlots, IGXVertexBuffer ** pBuff, IGXVertexDeclaration * pDecl)
{
	CGXRenderBuffer * pRB = new CGXRenderBuffer(this, countSlots, pBuff, pDecl);
	for(UINT i = 0; i < countSlots; ++i)
	{
		((CGXVertexBuffer*)pBuff[i])->m_pBuffer->AddRef();
	}
	((CGXVertexDeclaration*)pDecl)->m_pDeclaration->AddRef();

	return(pRB);
}
void CGXContext::destroyRenderBuffer(IGXRenderBuffer * pBuff)
{
	if(pBuff)
	{
		CGXRenderBuffer *pBuf = (CGXRenderBuffer*)pBuff;

		for(UINT i = 0; i < pBuf->m_uStreamCount; ++i)
		{
			mem_release(((CGXVertexBuffer*)pBuf->m_ppVertexBuffers[i])->m_pBuffer);
		}
		mem_release(((CGXVertexDeclaration*)pBuf->m_pVertexDeclaration)->m_pDeclaration);
	}
	mem_delete(pBuff);
}

void CGXContext::setRenderBuffer(IGXRenderBuffer * pBuff)
{
	m_pCurRenderBuffer = pBuff;
	m_sync_state.bRenderBuffer = TRUE;
}

void CGXContext::setVertexShader(IGXVertexShader * pSH)
{
	assert(!"Not imlemented");
}

void CGXContext::setPixelShader(IGXPixelShader * pSH)
{
	assert(!"Not imlemented");
}
