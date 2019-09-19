#include "GUIbase.h"
#include "IVIDEO.h"

#include "DOMdocument.h"

namespace gui
{
	namespace dom
	{
		DEFINE_DOM_NODE(IVIDEO);
		LINK_DOM_NODE_TO_TAG(IVIDEO, L"video");

		StringW IVIDEO::initRenderer()
		{
			StringW vid = StringW(L"!video#") + StringW((int)m_renderer.getInstanceId());
			if(!m_renderer.isLoaded())
			{
				StringW src = getAttribute(L"src");
				StringW path = StringW(m_pDocument->getDesktopStack()->getResourceDir()) + L"/video/" + src;
				m_renderer.openFile(path.c_str());

				CTextureManager::addTexture(vid, m_renderer.getTexture());
				if(getAttribute(L"autoplay").toBool())
				{
					m_renderer.play();
				}
				m_renderer.setLooped(getAttribute(L"looped").toBool());
			}
			return(vid);
		}
	};
};
