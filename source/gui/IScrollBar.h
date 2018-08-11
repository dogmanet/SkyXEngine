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
				void setLength(int len);
				virtual void render() = 0;

				virtual void dispatchEvent(IEvent & ev) = 0;
				virtual int getWidth() = 0;

				void updateData();
			protected:
				IRenderFrame * m_pParent;

				int m_iScrollMax;
				int m_iScrollCur;
				int m_iLength;
				SCROLLBAR_DIR m_eDir;

				bool m_bDragging;
				int m_iDragPos;
				int m_iScrollStart;
			};
		};
	};
};


#endif
