#ifndef _IScrollBarSimple_H_
#define _IScrollBarSimple_H_

#include "IScrollBar.h"

namespace gui
{
	namespace dom
	{
		namespace render
		{
			class CScrollBarSimple: public IScrollBar
			{
				DECLARE_CLASS(CScrollBarSimple, IScrollBar);
			public:
				CScrollBarSimple(IRenderFrame * _parent, SCROLLBAR_DIR _dir);
				~CScrollBarSimple();
				void render();

				int getWidth();

				void dispatchEvent(IEvent & ev);

				struct point
				{
					float x;
					float y;
					float z;
					float tx;
					float ty;
				};

			protected:
				IGXVertexBuffer *m_pVertices;
				IGXRenderBuffer *m_pRenderBuffer;
			};
		}
	};
};


#endif
