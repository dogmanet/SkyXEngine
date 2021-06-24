#ifndef __DESKTOP_STACK_H
#define __DESKTOP_STACK_H

#include <xcommon/gui/IXDesktopStack.h>

class CDesktopStack: public IXUnknownImplementation<IXDesktopStack>
{
public:
#if 0
	SX_ALIGNED_OP_MEM();

	CDesktopStack(CGUI *pGUI, IGXDevice *pDev, const char *szResPath, UINT uWidth, UINT uHeight);
	~CDesktopStack();
#endif

	BOOL XMETHODCALLTYPE putMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

	void XMETHODCALLTYPE render() override;

	bool XMETHODCALLTYPE newDesktop(const char *szName, const char *szFile, IXDesktop **ppOut) override;

	bool XMETHODCALLTYPE setActiveDesktop(IXDesktop *pDesktop, bool clearStack = true) override;
	bool XMETHODCALLTYPE setActiveDesktop(const char *name) override;

	void XMETHODCALLTYPE getActiveDesktop(IXDesktop **ppOut) override;

	void XMETHODCALLTYPE getDesktop(const char *szName, IXDesktop **ppOut) override;

#if 0
	void updateScreenSize(UINT uWidth, UINT uHeight, bool doResize = true);
	void showCursor(BOOL bShow);
	WCHAR* getResourceDir();
	IGXDevice* getDevice();

	UINT getScreenWidth();
	UINT getScreenHeight();

#endif
	void XMETHODCALLTYPE messageBox(const char *szTitle, const char *szText, ...) override;

	bool XMETHODCALLTYPE pushDesktop(IXDesktop *pDesktop) override;
	bool XMETHODCALLTYPE popDesktop(IXDesktop **ppOut = NULL) override;

#if 0
	void destroyDesktop(IDesktop *dp);

	void execCallback(const StringW &cmd, IEvent *ev);


	void setTransformWorld(const SMMATRIX &mat)
	{
		m_mTransformWorld = mat;
	}
	void setTransformViewProj(const SMMATRIX &mat)
	{
		m_mTransformViewProj = mat;
	}

	CTextureManager* getTextureManager()
	{
		return(m_pTextureManager);
	}

	IFontManager* getFontManager()
	{
		return(m_pFontManager);
	}

	const SMMATRIX& getTransformWorld()
	{
		return(m_mTransformWorld);
	}
	const SMMATRIX& getTransformViewProj()
	{
		return(m_mTransformViewProj);
	}

	void updateTransformShader()
	{
		m_pVSTransformConstant->update(&SMMatrixTranspose(m_mTransformWorld * m_mTransformViewProj));
		m_pDevice->getThreadContext()->setVSConstant(m_pVSTransformConstant, 0);
	}

	IGXRenderBuffer* getQuadRenderBufferXYZ(float3_t *pVertices);
	IGXRenderBuffer* getQuadRenderBufferXYZTex16(float *pVertices);

	IFont* getFont(const WCHAR * szName, UINT size, IFont::STYLE style, int iBlurRadius) override;
#endif
protected:
#if 0
	SMMATRIX m_mTransformWorld;
	SMMATRIX m_mTransformViewProj;

	const CTexture *m_pDefaultWhite;

	CGUI *m_pGUI;
	IGXDevice *m_pDevice;
	WCHAR *m_szResourceDir = NULL;

	CTextureManager *m_pTextureManager;
	IFontManager *m_pFontManager;

	bool m_isResizing = false;
	bool m_isScreenSizeChanged = false;
	UINT m_iScreenWidth = 0;
	UINT m_iScreenHeight = 0;

	IGXDepthStencilSurface *m_pOldDepthStencilSurface = NULL;
	IGXSurface *m_pDepthStencilSurface = NULL;

	IDesktop *m_pActiveDesktop = NULL;
	Array<IDesktop*> m_mDesktopStack;

	time_point m_tPrev = std::chrono::high_resolution_clock::now();;

	bool m_bShowCursor = true;
	AssotiativeArray<StringW, IDesktop*> m_mDesktops;

	struct SimpleCallback
	{
		GUI_CALLBACK fn;
		void *pUserData;
	};

	struct DefaultCallback
	{
		GUI_CALLBACK_WC fn;
		void *pUserData;
	};

	AssotiativeArray<StringW, /* Array< */SimpleCallback/* > */> m_mCallbacks;
	Array<DefaultCallback> m_mCallbacksDefaults;
	Array<DefaultCallback> m_mCallbacksDefaultsWC;

	IGXVertexBuffer *m_pQuadVerticesXYZ;
	IGXVertexBuffer *m_pQuadVerticesXYZTex16;

	IGXRenderBuffer *m_pQuadRenderXYZ;
	IGXRenderBuffer *m_pQuadRenderXYZTex16;

	IGXConstantBuffer *m_pVSTransformConstant = NULL;



	SimpleCallback* getFullCallbackByName(const char *cbName);
	SimpleCallback* getFullCallbackByName(const StringW &cbName);
#endif
};

#endif
