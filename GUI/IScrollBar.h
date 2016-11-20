#ifndef _IScrollBar_H_
#define _IScrollBar_H_

#include "CDOM.h"


namespace GUI
{
	enum SCROLLBAR_DIR
	{
		SCROLLBAR_DIR_VERTICAL = 0,
		SCROLLBAR_DIR_HORIZONTAL
	};
	namespace DOM
	{
		namespace Render
		{
			class IScrollBar
			{
			public:
				IScrollBar(IRenderFrame * _parent, SCROLLBAR_DIR _dir);
				void SetLength(int len);
				virtual void Render() = 0;

				virtual void DispatchEvent(IEvent & ev) = 0;
				virtual int GetWidth() = 0;

				void UpdateData();
			protected:
				IRenderFrame * m_pParent;

				int m_iScrollMax;
				int m_iScrollCur;
				int m_iLength;
				SCROLLBAR_DIR m_eDir;
			};
		};
	};
};


#endif
