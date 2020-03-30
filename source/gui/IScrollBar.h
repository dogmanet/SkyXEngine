#ifndef _IScrollBar_H_
#define _IScrollBar_H_

#include "DOM.h"


namespace gui
{
	enum SCROLLBAR_DIR
	{
		SCROLLBAR_DIR_VERTICAL = 0,
		SCROLLBAR_DIR_HORIZONTAL
	};
	namespace dom
	{
		namespace render
		{
			class IScrollBar
			{
			public:
				IScrollBar(IRenderFrame * _parent, SCROLLBAR_DIR _dir);
				virtual ~IScrollBar() = default;
				void setLength(int len);
				virtual void render() = 0;

				virtual void dispatchEvent(IEvent & ev) = 0;
				virtual int getWidth() = 0;

				void updateData();
			protected:
				IRenderFrame *m_pParent;

				int m_iScrollMax = 0;
				int m_iScrollCur = 0;
				int m_iLength = 0;
				SCROLLBAR_DIR m_eDir;

				bool m_bDragging = false;
				int m_iDragPos = 0;
				int m_iScrollStart = 0;
			};
		};
	};
};


#endif
