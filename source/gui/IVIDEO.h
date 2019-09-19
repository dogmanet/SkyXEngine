#ifndef _IVIDEO_H_
#define _IVIDEO_H_

#include "DOM.h"
#include "VideoRenderer.h"

#define _GUI_NO_VIDEO

namespace gui
{
	namespace dom
	{
		class IVIDEO: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IVIDEO, IDOMnodeTag);
			DECLARE_DOM_NODE(IVIDEO);

			StringW initRenderer();

			CVideoRenderer* GetVideoRenderer()
			{
				return(&m_renderer);
			}

		protected:

			CVideoRenderer m_renderer;
		};
	};
};

#endif
