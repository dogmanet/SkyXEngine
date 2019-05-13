#include "Desktop.h"
#include "IHTMLparser.h"
#include "IRenderFrame.h"

//#include "CDOM.h"

namespace gui
{
	CDesktop::CDesktop(const StringW & sName):
		m_sName(sName)
	{
		m_pDoc = new dom::CDOMdocument();
		m_pDoc->setDesktop(this);

		m_pColorWhite = GetGUI()->getDevice()->createConstantBuffer(sizeof(float4));
		m_pColorWhite->update(&float4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	CDesktop::~CDesktop()
	{
		mem_release(m_pColorWhite);
		mem_delete(m_pDoc);
		releaseRenderTarget();
	}

	void CDesktop::loadFromFile(const WCHAR * str)
	{
		StringW file = StringW(GetGUI()->getResourceDir()) + L"/" + str;
		dom::IHTMLparser p;
		p.setDocument(m_pDoc);

		dom::IDOMnode * root = p.createFromFile(file.c_str());
		if(!root)
		{
			printf("Error: Unable to load resource \"%s\"\n", String(file).c_str());
			return;
		}
		m_pDoc->setRootNode(root);
		m_pFocusedNode = root;

		swscanf(root->getAttribute(L"parallax").c_str(), L"%f", &m_fParallaxFactor);
	}

	void CDesktop::setWidth(UINT w)
	{
		m_iWidth = w;
	}
	void CDesktop::setHeight(UINT h)
	{
		m_iHeight = h;
	}
	void CDesktop::updateSize()
	{
		releaseRenderTarget();
		createRenderTarget();
	}

	void CDesktop::createRenderTarget()
	{
		m_pRenderSurface = GetGUI()->getDevice()->createColorTarget(m_iWidth, m_iHeight, GXFMT_A8R8G8B8, GXMULTISAMPLE_4_SAMPLES, false);
		m_pDepthStencilSurface = GetGUI()->getDevice()->createDepthStencilSurface(m_iWidth, m_iHeight, GXFMT_D24S8, GXMULTISAMPLE_4_SAMPLES, false);

		m_txFinal = CTextureManager::createTexture(StringW(L"@") + m_sName, m_iWidth, m_iHeight, 32, true, NULL, false);

		struct point
		{
			float x;
			float y;
			float z;
			float tx;
			float ty;
		};

		point a[] = {
			{0, 0, 0, 0.0f, 0.0f},
			{m_iWidth, 0, 0, 1.0f, 0.0f},
			{m_iWidth, m_iHeight, 0, 1.0f, 1.0f},
			{0, m_iHeight, 0, 0.0f, 1.0f}
		};

		m_pVertices = GetGUI()->getDevice()->createVertexBuffer(sizeof(point) * 4, GX_BUFFER_USAGE_STATIC | GX_BUFFER_WRITEONLY, a);
		m_pRenderBuffer = GetGUI()->getDevice()->createRenderBuffer(1, &m_pVertices, GetGUI()->getVertexDeclarations()->m_pXYZTex);
	}
	void CDesktop::releaseRenderTarget()
	{
		mem_release(m_pRenderBuffer);
		mem_release(m_pVertices);

		mem_release(m_pRenderSurface);
		mem_release(m_pDepthStencilSurface);
		if(m_txFinal)
		{
			CTextureManager::unloadTexture(m_txFinal);
		}
	}
	void CDesktop::setDirty()
	{
		if(m_pDoc)
		{
			m_pDoc->markDirty();
		}
	}

	void CDesktop::render(float fTimeDelta, bool bPresent)
	{
		m_pDoc->update(fTimeDelta);
		if(m_pDoc->isDirty())
		{
			CTextureManager::bindTexture(NULL);
			IGXSurface *pOldRT;
			IGXDepthStencilSurface *pOldDS;
			pOldRT = GetGUI()->getDevice()->getColorTarget();
			GetGUI()->getDevice()->setColorTarget(m_pRenderSurface);
			pOldDS = GetGUI()->getDevice()->getDepthStencilSurface();
			GetGUI()->getDevice()->setDepthStencilSurface(m_pDepthStencilSurface);

			GetGUI()->getDevice()->clear(GXCLEAR_COLOR | GXCLEAR_STENCIL);

			//GetGUI()->getDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			//GetGUI()->getDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		//	GetGUI()->getDevice()->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
		//	GetGUI()->getDevice()->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
		//	GetGUI()->getDevice()->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
		//	GetGUI()->getDevice()->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_MAX);
			GetGUI()->getDevice()->setBlendState(GetGUI()->getBlendStates()->m_pDesktop);
			
			m_pDoc->render();

			GetGUI()->getDevice()->setBlendState(GetGUI()->getBlendStates()->m_pDefault);
		//	GetGUI()->getDevice()->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);

			GetGUI()->getDevice()->setColorTarget(pOldRT);
			GetGUI()->getDevice()->setDepthStencilSurface(pOldDS);
			mem_release(pOldRT);
			mem_release(pOldDS);

			//D3DXSaveSurfaceToFileA("../screenshots/gui.png", D3DXIFF_PNG, m_pRenderSurface, NULL, NULL);
			
			
			IGXSurface *pNewSurface = m_txFinal->getAPItexture()->getMipmap();
			GetGUI()->getDevice()->downsampleColorTarget(m_pRenderSurface, pNewSurface);
			mem_release(pNewSurface);

			/*if(GetAsyncKeyState('K'))
			{
				D3DXSaveSurfaceToFileW((StringW(L"../screenshots/") + m_sName + StringW(L".png")).c_str(), D3DXIFF_PNG, pNewSurface, NULL, NULL);
			}*/
		}

		if(bPresent)
		{
			//const CPITexture def_w = CTextureManager::getTexture(L"/img/map.jpg");
			CTextureManager::bindTexture(m_txFinal);


			/*if(GetAsyncKeyState('L'))
			{
				D3DXSaveTextureToFileA("../screenshots/gui3tx.png", D3DXIFF_PNG, m_txFinal->getAPItexture(), NULL);
			}*/

			//CTextureManager::bindTexture(def_w);
			
			SMMATRIX mi = SMMatrixIdentity();
			GetGUI()->setTransformWorld(mi);
		//	GetGUI()->getDevice()->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mi));
			SMMATRIX mOldViewProj/*, mOldProj*/;
			mOldViewProj = GetGUI()->getTransformViewProj();
		//	GetGUI()->getDevice()->GetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mOldView));
		//	GetGUI()->getDevice()->GetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&mOldProj));
		//	GetGUI()->getDevice()->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mi));
			
			SMMATRIX m(
				2.0f / (float)m_iWidth, 0.0f, 0.0f, 0.0f,
				0.0f, -2.0f / (float)m_iHeight, 0.0f, 0.0f,
				0.0f, 0.0f, 0.5f, 0.0f,
				-1.0f, 1.0f, 0.5f, 1.0f);
			m = SMMatrixTranslation(-0.5f, -0.5f, 0.0f) * m;
		//	GetGUI()->getDevice()->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&m));

			GetGUI()->setTransformViewProj(mi * m);

		/*	struct point
			{
				float x;
				float y;
				float z;
				float tx;
				float ty;
			};*/


		/*	point a[6] = {
				{0, 0, 0, 0.0f, 0.0f},
				{m_iWidth, 0, 0, 1.0f, 0.0f},
				{0, m_iHeight, 0, 0.0f, 1.0f},
				{0, m_iHeight, 0, 0.0f, 1.0f},
				{m_iWidth, 0, 0, 1.0f, 0.0f},
				{m_iWidth, m_iHeight, 0, 1.0f, 1.0f}
			};*/

			auto shader = GetGUI()->getShaders()->m_baseTexturedColored;
			SGCore_ShaderBind(shader.m_idShaderKit);

		//	static CSHADER def_sh = CTextureManager::loadShader(L"text");

			//GetGUI()->getDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			//GetGUI()->getDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//GetGUI()->getDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

		//	GetGUI()->getDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
		//	CTextureManager::bindShader(def_sh);

			GetGUI()->getDevice()->setRenderBuffer(m_pRenderBuffer);
			GetGUI()->getDevice()->setIndexBuffer(GetGUI()->getQuadIndexBuffer()); 
			GetGUI()->getDevice()->setPixelShaderConstant(m_pColorWhite);

			GetGUI()->updateTransformShader();

			GetGUI()->getDevice()->drawIndexed(4, 2, 0, 0);
		//	DX_CALL(GetGUI()->getDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point)));

		//	GetGUI()->getDevice()->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mOldView));
		//	GetGUI()->getDevice()->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&mOldProj));
			GetGUI()->setTransformViewProj(mOldViewProj);
		}
	}

	CPITexture CDesktop::getTexture()
	{
		return(m_txFinal);
	}

	const dom::IDOMnodeCollection & CDesktop::createFromText(const StringW & html)
	{
		return(getDocument()->createFromText(html));
	}

	void CDesktop::dispatchEvent(IEvent ev)
	{
		//__try
		{

			bool IsMouseEvent = (ev.type == GUI_EVENT_TYPE_MOUSEUP)
				|| (ev.type == GUI_EVENT_TYPE_MOUSEDOWN)
				|| (ev.type == GUI_EVENT_TYPE_MOUSEWHEELDOWN)
				|| (ev.type == GUI_EVENT_TYPE_MOUSEMOVE)
				|| (ev.type == GUI_EVENT_TYPE_MOUSEWHEELUP);

			dom::CDOMnode * pTarget = NULL;
			RECT rc;
			if(IsMouseEvent)
			{
				pTarget = (dom::CDOMnode*)m_pDoc->getElementByXY(ev.clientX, ev.clientY, true);
				if(!pTarget)
				{
					return;
				}
				rc = pTarget->getRenderFrame()->getVisibleRect();
			//	Config::TestRect = rc;
				ShowCursor(TRUE);
				switch(pTarget->getStyle()->cursor->getInt())
				{
				case css::ICSSproperty::CURSOR_AUTO:
				case css::ICSSproperty::CURSOR_DEFAULT:
					SetCursor(LoadCursor(NULL, IDC_ARROW));
					break;
				case css::ICSSproperty::CURSOR_POINTER:
				case css::ICSSproperty::CURSOR_GRAB:
					SetCursor(LoadCursor(NULL, IDC_HAND));
					break;

				case css::ICSSproperty::CURSOR_TEXT:
					SetCursor(LoadCursor(NULL, IDC_IBEAM));
					break;

				case css::ICSSproperty::CURSOR_CELL:
				case css::ICSSproperty::CURSOR_CROSSHAIR:
					SetCursor(LoadCursor(NULL, IDC_CROSS));
					break;

				case css::ICSSproperty::CURSOR_ROW_RESIZE:
				case css::ICSSproperty::CURSOR_S_RESIZE:
				case css::ICSSproperty::CURSOR_N_RESIZE:
				case css::ICSSproperty::CURSOR_NS_RESIZE:
					SetCursor(LoadCursor(NULL, IDC_SIZENS));
					break;

				case css::ICSSproperty::CURSOR_COL_RESIZE:
				case css::ICSSproperty::CURSOR_W_RESIZE:
				case css::ICSSproperty::CURSOR_E_RESIZE:
				case css::ICSSproperty::CURSOR_EW_RESIZE:
					SetCursor(LoadCursor(NULL, IDC_SIZEWE));
					break;

				case css::ICSSproperty::CURSOR_NE_RESIZE:
				case css::ICSSproperty::CURSOR_NESW_RESIZE:
				case css::ICSSproperty::CURSOR_SW_RESIZE:
					SetCursor(LoadCursor(NULL, IDC_SIZENESW));
					break;

				case css::ICSSproperty::CURSOR_NW_RESIZE:
				case css::ICSSproperty::CURSOR_NWSE_RESIZE:
				case css::ICSSproperty::CURSOR_SE_RESIZE:
					SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
					break;

				case css::ICSSproperty::CURSOR_NO_DROP:
				case css::ICSSproperty::CURSOR_NOT_ALLOWED:
					SetCursor(LoadCursor(NULL, IDC_NO));
					break;

				case css::ICSSproperty::CURSOR_WAIT:
					SetCursor(LoadCursor(NULL, IDC_WAIT));
					break;

				case css::ICSSproperty::CURSOR_PROGRESS:
					SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
					break;

				case css::ICSSproperty::CURSOR_ALL_SCROLL:
					SetCursor(LoadCursor(NULL, IDC_SIZEALL));
					break;


					//IDC_HELP

				case css::ICSSproperty::CURSOR_NONE:
					ShowCursor(FALSE);
					break;
				}
			}

			switch(ev.type)
			{
			case GUI_EVENT_TYPE_MOUSEMOVE:
				ev.currentTarget = ev.target = pTarget;
				ev.offsetX = ev.clientX - rc.left;
				ev.offsetY = ev.clientY - rc.top;
				pTarget->dispatchEvent(ev);
				break;

			case GUI_EVENT_TYPE_MOUSEDOWN:
				//		wprintf(L"%d x %d\n", ev.clientX, ev.clientY);
				ev.currentTarget = ev.target = pTarget;
				ev.offsetX = ev.clientX - rc.left;
				ev.offsetY = ev.clientY - rc.top;
				//		wprintf(L"EL: x: %d, y: %d, \nEV: x: %d, y: %d\n\n", rc.left, rc.top, ev.clientX, ev.clientY);
				pTarget->dispatchEvent(ev);
				break;

			case GUI_EVENT_TYPE_MOUSEUP:
				ev.currentTarget = ev.target = pTarget;
				ev.offsetX = ev.clientX - rc.left;
				ev.offsetY = ev.clientY - rc.top;
				pTarget->dispatchEvent(ev);
				if(m_pFocusedNode)
				{
					if(pTarget == m_pFocusedNode)
					{
						ev.type = GUI_EVENT_TYPE_CLICK;
						pTarget->dispatchEvent(ev);
					}
					else
					{
						m_pFocusedNode->dispatchEvent(ev);
					}
				}
				break;

			case GUI_EVENT_TYPE_KEYDOWN:
			case GUI_EVENT_TYPE_KEYUP:
			case GUI_EVENT_TYPE_KEYPRESS:
				if(m_pFocusedNode)
				{
					ev.target = m_pFocusedNode;
					m_pFocusedNode->dispatchEvent(ev);
				}
				break;

			case GUI_EVENT_TYPE_MOUSEWHEELUP:
			case GUI_EVENT_TYPE_MOUSEWHEELDOWN:
				//		wprintf(L"%d x %d\n", ev.clientX, ev.clientY);
				ev.offsetX = ev.clientX - rc.left;
				ev.offsetY = ev.clientY - rc.top;
				ev.currentTarget = ev.target = pTarget;
				pTarget->dispatchEvent(ev);
				break;
			}

		}
		//__except(EXCEPTION_EXECUTE_HANDLER)
		{
		//	OutputDebugStringA("EXCEPTION_EXECUTE_HANDLER");
		}
		//OutputDebugStringA("endtry\n");
	}

	dom::IDOMdocument * CDesktop::getDocument()
	{
		return(m_pDoc);
	}

	void CDesktop::requestFocus(dom::IDOMnode * pNode)
	{
		if(pNode != m_pFocusedNode)
		{
			IEvent ev;
			if(m_pFocusedNode)
			{
				ev.type = GUI_EVENT_TYPE_BLUR;
				ev.target = m_pFocusedNode;
				ev.relatedTarget = pNode;
				m_pFocusedNode->dispatchEvent(ev);
			}

			ev.type = GUI_EVENT_TYPE_FOCUS;
			ev.target = pNode;
			ev.relatedTarget = m_pFocusedNode;
			pNode->dispatchEvent(ev);

			m_pFocusedNode = pNode;
		}
	}

	dom::IDOMnode * CDesktop::getFocus()
	{
		return(m_pFocusedNode);
	}

	dom::IDOMnode * dom::CDOMdocument::getFocus()
	{
		return(m_pDesktop->getFocus());
	}

	void dom::CDOMdocument::requestFocus(IDOMnode * pn)
	{
		m_pDesktop->requestFocus(pn);
	}

	void CDesktop::release()
	{
		GetGUI()->destroyDesktop(this);
	}

	float CDesktop::getParallaxFactor()
	{
		return(m_fParallaxFactor);
	}
};
