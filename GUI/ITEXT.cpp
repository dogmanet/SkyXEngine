#include "GUIbase.h"
#include "ITEXT.h"

#include "IRenderFrame.h"


namespace GUI
{
	namespace DOM
	{
		DEFINE_DOM_NODE(IDOMnodeText);

		void IDOMnodeText::DebugPrintMe(UINT lvl)
		{
			wprintf(L"%s\n", m_wsText);
		}


		void IDOMnodeText::InsertChar(UINT ch)
		{
			Render::IRenderTextNew * t = (Render::IRenderTextNew*)m_pRenderFrame;
			m_wsText.insert(t->GetCaretPos(), (WCHAR)ch);
			t->SetCaretPos(t->GetCaretPos() + 1, true);
		}

		void IDOMnodeText::DeleteChar(bool back)
		{
			Render::IRenderTextNew * t = (Render::IRenderTextNew*)m_pRenderFrame;
			m_wsText.Delete(t->GetCaretPos() + (back ? -1 : 0), 1);
			t->SetCaretPos(t->GetCaretPos() + (back ? -1 : 0));
		}

		void IDOMnodeText::FromClipboard()
		{
			OpenClipboard(NULL);
			HANDLE h = GetClipboardData(CF_UNICODETEXT);
			WCHAR* s = (WCHAR*)GlobalLock(h);
			Render::IRenderTextNew * t = (Render::IRenderTextNew*)m_pRenderFrame;
			m_wsText.insert(t->GetCaretPos(), s);
			t->SetCaretPos(t->GetCaretPos() + wcslen(s), true);
			GlobalUnlock(h);
			CloseClipboard();
		}

		void IDOMnodeText::ToClipboard(bool cut)
		{
			Render::IRenderTextNew * t = (Render::IRenderTextNew*)m_pRenderFrame;
			if(t->GetSelectionEnd() == t->GetSelectionStart())
			{
				return;
			}
			if(OpenClipboard(0))
			{
				EmptyClipboard();
				StringW str = t->GetSelectionText();

				HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (str.length() + 1) * sizeof(WCHAR));
				if(hglbCopy == NULL)
				{
					CloseClipboard();
					return;
				}
				WCHAR * lptstrCopy = (WCHAR*)GlobalLock(hglbCopy);
				memcpy(lptstrCopy, str.c_str(), str.length() * sizeof(WCHAR));
				lptstrCopy[str.length()] = 0;
				GlobalUnlock(hglbCopy);
				SetClipboardData(CF_UNICODETEXT, hglbCopy);
				CloseClipboard();

				GlobalFree(hglbCopy);
				//	GlobalFree(lptstrCopy);

				if(cut)
				{
					DeleteSelection();
				}
			}
		}
		void IDOMnodeText::DeleteSelection()
		{
			Render::IRenderTextNew * t = (Render::IRenderTextNew*)m_pRenderFrame;
			if(t->GetSelectionEnd() == t->GetSelectionStart())
			{
				return;
			}
			m_wsText.Delete(t->GetSelectionStart(), t->GetSelectionEnd() - t->GetSelectionStart());
			t->SetCaretPos(t->GetSelectionStart(), true);
		}

		bool IDOMnodeText::HasSelection()
		{
			Render::IRenderTextNew * t = (Render::IRenderTextNew*)m_pRenderFrame;
			return(t->GetSelectionEnd() != t->GetSelectionStart());
		}


		void IDOMnodeText::SelectAll()
		{
			Render::IRenderTextNew * t = (Render::IRenderTextNew*)m_pRenderFrame;
			t->SetSelectionStart(0);
			t->SetSelectionEnd(t->GetCaretMaxPos());
		}
	};
};