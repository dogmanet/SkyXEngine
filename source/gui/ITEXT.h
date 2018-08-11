#ifndef _ITEXT_H_
#define _ITEXT_H_

#include "DOM.h"
#include "IRenderFrame.h"

namespace gui
{
	namespace dom
	{
		class IDOMnodeText: public CDOMnode
		{
		public:
			DECLARE_CLASS(IDOMnodeText, CDOMnode);
			DECLARE_DOM_NODE(IDOMnodeText);

			void setText(const StringW & text, BOOL build = FALSE)
			{
				int l0 = m_wsText.length();
				m_wsText = text;
				int l = m_wsText.length();
				if(l0 > l)
				{
					render::IRenderTextNew * rf = ((render::IRenderTextNew*)m_pRenderFrame);
					if(rf)
					{

						rf->setCaretPos(min((int)(rf->getCaretPos()), l ? l + 1 : 0));
					}
				}
			};

			const StringW & getText()
			{
				return(m_wsText);
			}

			void debugPrintMe(UINT lvl = 0);

			bool isTextNode()
			{
				return(true);
			}


			void insertChar(UINT ch);
			void deleteChar(bool back);

			void fromClipboard();
			void toClipboard(bool cut = false);

			void deleteSelection();

			bool hasSelection();
			void selectAll();

					

		protected:
			StringW m_wsText;
		};
	};
};

#endif
