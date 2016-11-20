#ifndef _ITEXT_H_
#define _ITEXT_H_

#include "CDOM.h"
#include "IRenderFrame.h"

namespace GUI
{
	namespace DOM
	{
		class IDOMnodeText: public CDOMnode
		{
		public:
			DECLARE_CLASS(IDOMnodeText, CDOMnode);
			DECLARE_DOM_NODE(IDOMnodeText);

			void SetText(const StringW & text)
			{
				int l0 = m_wsText.length();
				m_wsText = text;
				int l = m_wsText.length();
				if(l0 > l)
				{
					Render::IRenderTextNew * rf = ((Render::IRenderTextNew*)m_pRenderFrame);
					if(rf)
					{

						rf->SetCaretPos(min((int)(rf->GetCaretPos()), l ? l + 1 : 0));
					}
				}
			};

			const StringW & GetText()
			{
				return(m_wsText);
			}

			void DebugPrintMe(UINT lvl = 0);

			bool IsTextNode()
			{
				return(true);
			}


			void InsertChar(UINT ch);
			void DeleteChar(bool back);

			void FromClipboard();
			void ToClipboard(bool cut = false);

			void DeleteSelection();

			bool HasSelection();
			void SelectAll();

					

		protected:
			StringW m_wsText;
		};
	};
};

#endif
