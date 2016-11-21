#ifndef _IScrollBarSimple_H_
#define _IScrollBarSimple_H_

#include "IScrollBar.h"

namespace GUI
{
	namespace DOM
	{
		namespace Render
		{
			class IScrollBarSimple: public IScrollBar
			{
				DECLARE_CLASS(IScrollBarSimple, IScrollBar);
			public:
				IScrollBarSimple(IRenderFrame * _parent, SCROLLBAR_DIR _dir);
				void Render();

				int GetWidth();

				void DispatchEvent(IEvent & ev);
			};
		}
	};
};


#endif
