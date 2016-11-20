#ifndef _IVIDEO_H_
#define _IVIDEO_H_

#include "CDOM.h"
#include "IVideoRenderer.h"

namespace GUI
{
	namespace DOM
	{
		class IVIDEO: public IDOMnodeTag
		{
		public:
			DECLARE_CLASS(IVIDEO, IDOMnodeTag);
			DECLARE_DOM_NODE(IVIDEO);

			StringW InitRenderer()
			{
				StringW vid = StringW(L"!video#") + StringW((int)m_renderer.GetInstanceId());
				if(!m_renderer.IsLoaded())
				{
					StringW src = GetAttribute(L"src");
					StringW path = StringW(GetGUI()->GetResourceDir()) + L"/video/" + src;
					m_renderer.OpenFile(path.c_str());

					ITextureManager::AddTexture(vid, m_renderer.GetTexture());
					if(GetAttribute(L"autoplay").ToBool())
					{
						m_renderer.Play();
					}
					m_renderer.SetLooped(GetAttribute(L"looped").ToBool());
				}
				return(vid);
			}

			IVideoRenderer * GetVideoRenderer()
			{
				return(&m_renderer);
			}

		protected:

			IVideoRenderer m_renderer;
		};
	};
};

#endif
