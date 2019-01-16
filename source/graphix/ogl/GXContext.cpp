#include "GXContext.h"

#if defined(_LINUX) || defined(_MAC)
#	include <gtk/gtk.h>
#	include <gtk/gtkx.h>
#endif

#include "GXIndexBuffer.h"
#include "GXVertexBuffer.h"
#include "GXVertexDeclaration.h"
#include "GXShader.h"
#include "GXRenderBuffer.h"

CGXContext::CGXContext():
	m_pGL(NULL),
	m_pCurIndexBuffer(NULL),
	m_drawPT(NULL),
	m_pCurRenderBuffer(NULL)
{
	/*for(int i = 0; i < MAXDSGVSTREAM; ++i)
	{
		m_pCurVertexBuffer[i] = NULL;
	}*/
	memset(&m_sync_state, 0, sizeof(m_sync_state));

	//m_pSPP->Define("_OGL");

	//char * declList[] = GXDECLLIST;

	//for(int j = 0, l = sizeof(declList) / sizeof(char*); j < l; ++j)
	//{
	//	m_pSPP->Define((DString(declList[j]) + " " + j).c_str());
	//}
}

BOOL CGXContext::initContext(SXWINDOW wnd)
{
	m_hWnd = wnd;
#if defined(_WINDOWS)
	m_hDC = GetDC((HWND)m_hWnd);

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int iFormat = ChoosePixelFormat(m_hDC, &pfd);
	SetPixelFormat(m_hDC, iFormat, &pfd);

	m_hRC = wglCreateContext(m_hDC);

	wglMakeCurrent(m_hDC, m_hRC);

	//glClearColor(0, 16.0f / 255.0f, 32.0f / 255.0f, 0);
	//glClear(GL_COLOR_BUFFER_BIT);

	//SwapBuffers();

	m_pGL = new IDSRGLPFN();

	//wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

	if(m_pGL->wglSwapIntervalEXT)
	{
		m_pGL->wglSwapIntervalEXT(0); ///< disables vsync
	}
	
	return(TRUE);
	//
#else
	//GtkWidget * da = gtk_drawing_area_new();

	//gtk_container_set_border_width(GTK_CONTAINER(wnd), 100);
	//gtk_container_add(GTK_CONTAINER(wnd), da);
	//gtk_widget_set_double_buffered(GTK_WIDGET(da), FALSE);
	//gtk_widget_show(da);

	GdkDisplay * pGtkDisplay = gtk_widget_get_display(GTK_WIDGET(wnd));
	Display * pDisplay = gdk_x11_display_get_xdisplay(pGtkDisplay);
	//GdkVisual * visual;

	GLint attribs[] = {GLX_RGBA, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8, GLX_DOUBLEBUFFER, None};
	XVisualInfo * xvinfo = glXChooseVisual(pDisplay, XDefaultScreen(pDisplay),  attribs);

	GdkWindow * gdk_window = gtk_widget_get_window(GTK_WIDGET(wnd));

	if(!xvinfo)
	{
		return(FALSE);
	}

	//visual = gdkx_visual_get(xvinfo->visualid);

	GLXContext ctx = glXCreateContext(pDisplay, xvinfo, 0, True);

	if(ctx == NULL)
	{
		return(FALSE);
	}

	glXMakeCurrent(pDisplay, gdk_x11_window_get_xid(gdk_window), ctx);

	XFree(xvinfo);

	//GtkWidget * pGLarea = gtk_gl_area_new();
	//gtk_container_add(GTK_CONTAINER(wnd), pGLarea);
	//gtk_widget_show(pGLarea);

	//g_signal_connect(pGLarea, "render", G_CALLBACK(render), NULL);

	//printf("e: %s\n", gtk_gl_area_get_error((GtkGLArea*)(pGLarea))->message);


//glXCreateContext()


	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Exts: %s\n", glGetString(GL_EXTENSIONS));
	//

	glClearColor(0, 16.0f / 255.0f, 32.0f / 255.0f, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glXSwapBuffers(pDisplay, gdk_x11_window_get_xid(gdk_window));

	return(TRUE);
#endif
	return(FALSE);
}

CGXContext::~CGXContext()
{
#if defined(_WINDOWS)
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hRC);
	ReleaseDC((HWND)m_hWnd, m_hDC);
#endif

	mem_delete(m_pGL);
}


void CGXContext::swapBuffers()
{
#if defined(_WINDOWS)
	::SwapBuffers(m_hDC);
#else
#	error Implement me
#endif
}

void CGXContext::setClearColor(const float4_t & color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}
void CGXContext::clearTarget()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
void CGXContext::clearDepth(float val)
{
	glClearDepth(val);
	glClear(GL_DEPTH_BUFFER_BIT);
}
void CGXContext::clearStencil(UINT val)
{
	glClearStencil(val);
	glClear(GL_STENCIL_BITS);
}



IGXVertexBuffer * CGXContext::createVertexBuffer(size_t size, UINT flags, void * pInitData)
{
	//PFNGLGENVERTEXARRAYSPROC
	
	CGXVertexBuffer * pBuff = new CGXVertexBuffer(this);
	m_pGL->glGenBuffers(1, &pBuff->m_pBuffer);

	m_pGL->glBindBuffer(GL_ARRAY_BUFFER, pBuff->m_pBuffer);
	UINT usage = GL_STATIC_DRAW;

	if(flags & GX_BUFFER_USAGE_STATIC)
	{
		usage = GL_STATIC_DRAW;
	}
	if(flags & GX_BUFFER_USAGE_DYNAMIC)
	{
		usage = GL_DYNAMIC_DRAW;
	}
	if(flags & GX_BUFFER_USAGE_STREAM)
	{
		usage = GL_STREAM_DRAW;
	}
	if(flags & GX_BUFFER_POOL_DEFAULT)
	{

	}

	m_pGL->glBufferData(GL_ARRAY_BUFFER, size, pInitData, usage);

	//restore pprev buffer
	return(pBuff);
}

IGXIndexBuffer * CGXContext::createIndexBuffer(size_t size, UINT flags, GXINDEXTYPE it, void * pInitData)
{
	CGXIndexBuffer * pBuff = new CGXIndexBuffer(this);
	m_pGL->glGenBuffers(1, &pBuff->m_pBuffer);

	m_pGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pBuff->m_pBuffer);
	UINT usage = GL_STATIC_DRAW;

	if(flags & GX_BUFFER_USAGE_STATIC)
	{
		usage = GL_STATIC_DRAW;
	}
	if(flags & GX_BUFFER_USAGE_DYNAMIC)
	{
		usage = GL_DYNAMIC_DRAW;
	}
	if(flags & GX_BUFFER_USAGE_STREAM)
	{
		usage = GL_STREAM_DRAW;
	}

	m_pGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pInitData, usage);

	m_sync_state.bIndexBuffer = TRUE;

	switch(it)
	{
	case GXIT_UBYTE:
		pBuff->m_uIndexSize = GL_UNSIGNED_BYTE;
		break;
	case GXIT_USHORT:
		pBuff->m_uIndexSize = GL_UNSIGNED_SHORT;
		break;
	case GXIT_UINT:
		pBuff->m_uIndexSize = GL_UNSIGNED_INT;
		break;
	}

	return(pBuff);
}

void CGXContext::destroyIndexBuffer(IGXIndexBuffer * pBuff)
{
	if(pBuff)
	{
		m_pGL->glDeleteBuffers(1, &(((CGXIndexBuffer*)pBuff)->m_pBuffer));
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
	if(pBuff)
	{
		m_pGL->glDeleteBuffers(1, &(((CGXVertexBuffer*)pBuff)->m_pBuffer));
		/*for(int i = 0; i < MAXDSGVSTREAM; ++i)
		{
			if(m_pCurVertexBuffer[i] == pBuff)
			{
				m_sync_state.bVertexBuffers[i] = TRUE;
				m_sync_state.bVertexLayout = TRUE;
			}
		}*/
	}
	mem_delete(pBuff);
}

IGXVertexDeclaration * CGXContext::createVertexDeclaration(const GXVERTEXELEMENT * pDecl)
{
	CGXVertexDeclaration * vd = new CGXVertexDeclaration(this, pDecl);

//	m_pGL->glGenVertexArrays(1, &vd->m_pVAO);
	//m_pGL->glBindVertexArray(vd->m_pVAO);
	//m_sync_state.bVertexLayout = FALSE;
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

/*void CGXContext::IASetInputLayout(IDSGvertexDeclaration * pDecl)
{
	m_pCurVertexDecl = pDecl;
	m_sync_state.bVertexLayout = TRUE;
}*/

void CGXContext::setIndexBuffer(IGXIndexBuffer * pBuff)
{
	m_pCurIndexBuffer = pBuff;
	m_sync_state.bIndexBuffer = TRUE;
}
/*void CGXContext::IASetVertexBuffers(UINT startSlot, UINT countSlots, IDSRvertexBuffer ** pBuff)
{
#if defined(_DEBUG)
	if(startSlot + countSlots >= MAXDSGVSTREAM)
	{
		DS_FATAL((DStringW(L"Unable to bound vertex buffer") + (DWORD)startSlot + " " + (DWORD)countSlots).c_str());
	}
#endif

	for(UINT i = 0; i < countSlots; ++i)
	{
		m_pCurVertexBuffer[i + startSlot] = pBuff[i];

		m_sync_state.bVertexBuffers[i + startSlot] = TRUE;
		m_sync_state.bVertexLayout = TRUE;
	}
}
*/
void CGXContext::drawIndexed(UINT uIndexCount, UINT uStartIndexLocation, int iBaseVertexLocation)
{
	syncronize();

	// https://www.khronos.org/opengl/wiki/Vertex_Rendering

	assert(!"change uIndexCount to uPrimitiveCount!");

	//UINT ptC = GetPTcount(IndexCount);
	UINT ptC = uIndexCount;

	UINT uIDXtype = ((CGXIndexBuffer*)m_pCurIndexBuffer)->m_uIndexSize;

	if(iBaseVertexLocation == 0)
	{
		glDrawElements(m_drawPT, ptC, uIDXtype, (void*)(uStartIndexLocation * getIndexSize(uIDXtype)));
	}
	else
	{
		m_pGL->glDrawElementsBaseVertex(m_drawPT, ptC, uIDXtype, (void*)(uStartIndexLocation * getIndexSize(uIDXtype)), iBaseVertexLocation);
	}
}

void CGXContext::syncronize()
{
#if 0
	if(m_sync_state.bVertexLayout)
	{
		IDSGvertexDeclarationOgl * vd = (IDSGvertexDeclarationOgl*)m_pCurVertexDecl;
		//m_pGL->glBindVertexArray(vd->m_pVAO);


		UINT buff = 0;

		for(UINT i = 0; i < vd->m_uDeclCount; i++)
		{
			UINT cb = ((IDSRvertexBufferOgl*)m_pCurVertexBuffer[vd->m_pDeclItems[i].stream])->pBuffer;
			if(buff != cb || i == 0)
			{
				m_pGL->glBindBuffer(GL_ARRAY_BUFFER, cb);
				buff = cb;
			}
			m_pGL->glVertexAttribPointer(vd->m_pDeclItems[i].index, vd->m_pDeclItems[i].size, vd->m_pDeclItems[i].type​, vd->m_pDeclItems[i].normalized, vd->m_pDeclItems[i].stride​, vd->m_pDeclItems[i].pointer​);
			m_pGL->glEnableVertexAttribArray(vd->m_pDeclItems[i].index);
		}
		m_sync_state.bVertexLayout = FALSE;
	}
#endif
	if(m_sync_state.bRenderBuffer)
	{
		m_pGL->glBindVertexArray(m_pCurRenderBuffer ? ((CGXRenderBuffer*)m_pCurRenderBuffer)->m_uVAO : 0);
		m_sync_state.bRenderBuffer = FALSE;
	}
	if(m_sync_state.bIndexBuffer)
	{
		m_pGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pCurIndexBuffer ? ((CGXIndexBuffer*)m_pCurIndexBuffer)->m_pBuffer : 0);
		m_sync_state.bIndexBuffer = FALSE;
		//glDrawElements
	}
	
}

void CGXContext::setPrimitiveTopology(GXPT pt)
{
	switch(pt)
	{
	case GXPT_POINTLIST:
		m_drawPT = GL_POINTS;
		break;
	case GXPT_LINELIST:
		m_drawPT = GL_LINES;
		break;
	case GXPT_LINESTRIP:
		m_drawPT = GL_LINE_STRIP;
		break;
	case GXPT_TRIANGLELIST:
		m_drawPT = GL_TRIANGLES;
		break;
	case GXPT_TRIANGLESTRIP:
		m_drawPT = GL_TRIANGLE_STRIP;
		break;
	case GXPT_TRIANGLEFAN:
		m_drawPT = GL_TRIANGLE_FAN;
		break;
	}
}

UINT CGXContext::getPTcount(UINT idxCount)
{
	switch(m_drawPT)
	{
	case GL_POINTS:
		return(idxCount);
	case GL_LINES:
		return(idxCount / 2);
	case GL_LINE_STRIP:
		return(idxCount - 1);
	case GL_TRIANGLES:
		return(idxCount / 3);
	case GL_TRIANGLE_STRIP:
	case GL_TRIANGLE_FAN:
		return(idxCount - 2);
	}
	return(0);
}

UINT CGXContext::getIndexSize(UINT idx)
{
	switch(idx)
	{
	case GL_UNSIGNED_BYTE:
		return(1);
	case GL_UNSIGNED_SHORT:
		return(2);
	case GL_UNSIGNED_INT:
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
void CGXContext::setVertexShader(IGXVertexShader * pSH)
{
	assert(!"Not imlemented");
}
void CGXContext::setPixelShader(IGXPixelShader * pSH)
{
	assert(!"Not imlemented");
}

IGXRenderBuffer * CGXContext::createRenderBuffer(UINT countSlots, IGXVertexBuffer ** pBuff, IGXVertexDeclaration * pDecl)
{
	CGXRenderBuffer * pRB = new CGXRenderBuffer(this);

	m_pGL->glGenVertexArrays(1, &pRB->m_uVAO);

	m_pGL->glBindVertexArray(pRB->m_uVAO);
	m_sync_state.bRenderBuffer = TRUE;


	CGXVertexDeclaration * vd = (CGXVertexDeclaration*)pDecl;

	UINT buff = 0;

	for(UINT i = 0; i < vd->m_uDeclCount; i++)
	{
		if(countSlots <= vd->m_pDeclItems[i].stream)
		{
			m_pGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
			buff = 0;
		}
		else
		{
			UINT cb = ((CGXVertexBuffer*)pBuff[vd->m_pDeclItems[i].stream])->m_pBuffer;
			if(buff != cb || i == 0)
			{
				m_pGL->glBindBuffer(GL_ARRAY_BUFFER, cb);
				buff = cb;
			}
		}
		m_pGL->glVertexAttribPointer(vd->m_pDeclItems[i].index, vd->m_pDeclItems[i].size, vd->m_pDeclItems[i].type​, vd->m_pDeclItems[i].normalized, vd->m_pDeclItems[i].stride​, vd->m_pDeclItems[i].pointer​);
		m_pGL->glEnableVertexAttribArray(vd->m_pDeclItems[i].index);
	}
	return(pRB);
}
void CGXContext::destroyRenderBuffer(IGXRenderBuffer * pBuff)
{
	if(pBuff)
	{
		m_pGL->glDeleteBuffers(1, &((CGXRenderBuffer*)pBuff)->m_uVAO);
	}
	mem_delete(pBuff);
}

void CGXContext::setRenderBuffer(IGXRenderBuffer * pBuff)
{
	m_pCurRenderBuffer = pBuff;
	m_sync_state.bRenderBuffer = TRUE;
}
