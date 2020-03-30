#include "GUIbase.h"
#include "IHTML.h"



namespace gui
{
	namespace dom
	{
		DEFINE_DOM_NODE(IHTML);
		LINK_DOM_NODE_TO_TAG(IHTML, L"html");
	};
};
