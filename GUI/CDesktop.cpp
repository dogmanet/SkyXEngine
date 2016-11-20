#include "CDesktop.h"
#include "IHTMLparser.h"
#include "IRenderFrame.h"
//#include "CDOM.h"

namespace GUI
{
	CDesktop::CDesktop():m_pFocusedNode(NULL)
	{
		m_pDoc = new DOM::CDOMdocument();
		m_pDoc->SetDesktop(this);
	}

	CDesktop::~CDesktop()
	{
		SX_SAFE_DELETE(m_pDoc);
	}

	void CDesktop::LoadFromFile(const WCHAR * str)
	{
		StringW file = StringW(GetGUI()->GetResourceDir()) + L"/" + str;
		DOM::IHTMLparser p;
		p.SetDocument(m_pDoc);

		DOM::IDOMnode * root = p.CreateFromFile(file.c_str());
		if(!root)
		{
			wprintf(L"Error: Unable to load resource \"%s\"\n", file.c_str());
			return;
		}
		m_pDoc->SetRootNode(root);
		m_pFocusedNode = root;
	}

	void CDesktop::SetWidth(UINT w)
	{
		m_iWidth = w;
	}
	void CDesktop::SetHeight(UINT h)
	{
		m_iHeight = h;
	}
	void CDesktop::UpdateSize()
	{

	}

	void CDesktop::Render()
	{
		m_pDoc->Render();
	}

	CPITexture CDesktop::GetTexture()
	{
		return(&m_txFinal);
	}

	DOM::IDOMnodeCollection CDesktop::CreateFromText(const StringW & html)
	{
		DOM::IHTMLparser p;
		p.SetDocument(GetDocument());
		UINT next = 0;
		int cur;
		DOM::IDOMnode * root;
		DOM::IDOMnodeCollection c;
		do
		{
			root = p.CreateFromString(html.c_str() + next, &cur);
			next += cur;
			c.push_back(root);
		}
		while(next < html.length());
		return(c);
	}

	void CDesktop::DispatchEvent(IEvent ev)
	{
		__try
		{

			bool IsMouseEvent = (ev.type == GUI_EVENT_TYPE_MOUSEUP)
				|| (ev.type == GUI_EVENT_TYPE_MOUSEDOWN)
				|| (ev.type == GUI_EVENT_TYPE_MOUSEWHEELDOWN)
				|| (ev.type == GUI_EVENT_TYPE_MOUSEMOVE)
				|| (ev.type == GUI_EVENT_TYPE_MOUSEWHEELUP);

			DOM::CDOMnode * pTarget = NULL;
			RECT rc;
			if(IsMouseEvent)
			{
				pTarget = (DOM::CDOMnode*)m_pDoc->GetElementByXY(ev.clientX, ev.clientY, true);
				if(!pTarget)
				{
					return;
				}
				rc = pTarget->GetRenderFrame()->GetVisibleRect();
			//	Config::TestRect = rc;
				ShowCursor(TRUE);
				switch(pTarget->GetStyle()->cursor->GetInt())
				{
				case CSS::ICSSproperty::CURSOR_AUTO:
				case CSS::ICSSproperty::CURSOR_DEFAULT:
					SetCursor(LoadCursor(NULL, IDC_ARROW));
					break;
				case CSS::ICSSproperty::CURSOR_POINTER:
				case CSS::ICSSproperty::CURSOR_GRAB:
					SetCursor(LoadCursor(NULL, IDC_HAND));
					break;

				case CSS::ICSSproperty::CURSOR_TEXT:
					SetCursor(LoadCursor(NULL, IDC_IBEAM));
					break;

				case CSS::ICSSproperty::CURSOR_CELL:
				case CSS::ICSSproperty::CURSOR_CROSSHAIR:
					SetCursor(LoadCursor(NULL, IDC_CROSS));
					break;

				case CSS::ICSSproperty::CURSOR_ROW_RESIZE:
				case CSS::ICSSproperty::CURSOR_S_RESIZE:
				case CSS::ICSSproperty::CURSOR_N_RESIZE:
				case CSS::ICSSproperty::CURSOR_NS_RESIZE:
					SetCursor(LoadCursor(NULL, IDC_SIZENS));
					break;

				case CSS::ICSSproperty::CURSOR_COL_RESIZE:
				case CSS::ICSSproperty::CURSOR_W_RESIZE:
				case CSS::ICSSproperty::CURSOR_E_RESIZE:
				case CSS::ICSSproperty::CURSOR_EW_RESIZE:
					SetCursor(LoadCursor(NULL, IDC_SIZEWE));
					break;

				case CSS::ICSSproperty::CURSOR_NE_RESIZE:
				case CSS::ICSSproperty::CURSOR_NESW_RESIZE:
				case CSS::ICSSproperty::CURSOR_SW_RESIZE:
					SetCursor(LoadCursor(NULL, IDC_SIZENESW));
					break;

				case CSS::ICSSproperty::CURSOR_NW_RESIZE:
				case CSS::ICSSproperty::CURSOR_NWSE_RESIZE:
				case CSS::ICSSproperty::CURSOR_SE_RESIZE:
					SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
					break;

				case CSS::ICSSproperty::CURSOR_NO_DROP:
				case CSS::ICSSproperty::CURSOR_NOT_ALLOWED:
					SetCursor(LoadCursor(NULL, IDC_NO));
					break;

				case CSS::ICSSproperty::CURSOR_WAIT:
					SetCursor(LoadCursor(NULL, IDC_WAIT));
					break;

				case CSS::ICSSproperty::CURSOR_PROGRESS:
					SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
					break;

				case CSS::ICSSproperty::CURSOR_ALL_SCROLL:
					SetCursor(LoadCursor(NULL, IDC_SIZEALL));
					break;


					//IDC_HELP

				case CSS::ICSSproperty::CURSOR_NONE:
					ShowCursor(FALSE);
					break;
				}
			}

			switch(ev.type)
			{
			case GUI_EVENT_TYPE_MOUSEMOVE:
				ev.target = pTarget;
				ev.offsetX = ev.clientX - rc.left;
				ev.offsetY = ev.clientY - rc.top;
				pTarget->DispatchEvent(ev);
				break;

			case GUI_EVENT_TYPE_MOUSEDOWN:
				//		wprintf(L"%d x %d\n", ev.clientX, ev.clientY);
				ev.target = pTarget;
				ev.offsetX = ev.clientX - rc.left;
				ev.offsetY = ev.clientY - rc.top;
				//		wprintf(L"EL: x: %d, y: %d, \nEV: x: %d, y: %d\n\n", rc.left, rc.top, ev.clientX, ev.clientY);
				pTarget->DispatchEvent(ev);
				break;

			case GUI_EVENT_TYPE_MOUSEUP:
				ev.target = pTarget;
				ev.offsetX = ev.clientX - rc.left;
				ev.offsetY = ev.clientY - rc.top;
				pTarget->DispatchEvent(ev);
				if(m_pFocusedNode)
				{
					if(pTarget == m_pFocusedNode)
					{
						ev.type = GUI_EVENT_TYPE_CLICK;
						pTarget->DispatchEvent(ev);
					}
					else
					{
						m_pFocusedNode->DispatchEvent(ev);
					}
				}
				break;

			case GUI_EVENT_TYPE_KEYDOWN:
			case GUI_EVENT_TYPE_KEYUP:
			case GUI_EVENT_TYPE_KEYPRESS:
				if(m_pFocusedNode)
				{
					ev.target = m_pFocusedNode;
					m_pFocusedNode->DispatchEvent(ev);
				}
				break;

			case GUI_EVENT_TYPE_MOUSEWHEELUP:
			case GUI_EVENT_TYPE_MOUSEWHEELDOWN:
				//		wprintf(L"%d x %d\n", ev.clientX, ev.clientY);
				ev.offsetX = ev.clientX - rc.left;
				ev.offsetY = ev.clientY - rc.top;
				ev.target = pTarget;
				pTarget->DispatchEvent(ev);
				break;
			}

		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			OutputDebugStringA("EXCEPTION_EXECUTE_HANDLER");
		}
		OutputDebugStringA("endtry\n");
	}

	DOM::IDOMdocument * CDesktop::GetDocument()
	{
		return(m_pDoc);
	}

	void CDesktop::RequestFocus(DOM::IDOMnode * pNode)
	{
		if(pNode != m_pFocusedNode)
		{
			IEvent ev;
			if(m_pFocusedNode)
			{
				ev.type = GUI_EVENT_TYPE_BLUR;
				ev.target = m_pFocusedNode;
				ev.relatedTarget = pNode;
				m_pFocusedNode->DispatchEvent(ev);
			}

			ev.type = GUI_EVENT_TYPE_FOCUS;
			ev.target = pNode;
			ev.relatedTarget = m_pFocusedNode;
			pNode->DispatchEvent(ev);

			m_pFocusedNode = pNode;
		}
	}

	DOM::IDOMnode * CDesktop::GetFocus()
	{
		return(m_pFocusedNode);
	}

	DOM::IDOMnode * DOM::CDOMdocument::GetFocus()
	{
		return(m_pDesktop->GetFocus());
	}

	void DOM::CDOMdocument::RequestFocus(IDOMnode * pn)
	{
		m_pDesktop->RequestFocus(pn);
	}

	void CDesktop::Release()
	{
		GetGUI()->DestroyDesktop(this);
	}
};