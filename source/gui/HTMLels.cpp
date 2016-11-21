#include "GUIbase.h"

#include "IBODY.h"
#include "ITEXT.h"
#include "IBR.h"
#include "IBUTTON.h"
#include "IDIV.h"
#include "IHEAD.h"
#include "IHTML.h"
#include "IIMG.h"
#include "IINPUT.h"
#include "ILABEL.h"
#include "ILINK.h"
#include "IOPTION.h"
#include "IP.h"
#include "ISELECT.h"
#include "ISPAN.h"
#include "ITABLE.h"
#include "ITD.h"
#include "ITEXTAREA.h"
#include "ITH.h"
#include "ITR.h"
#include "IWINDOW.h"
#include "IVIDEO.h"
#include "ICHECKBOX.h"
#include "IRADIO.h"


namespace GUI
{
	namespace DOM
	{
		void HTMLelsInit()
		{
			LINK_DOM_NODE_TO_TAG(IDOMnodeText, L"text");
			LINK_DOM_NODE_TO_TAG(IBODY, L"body");
			LINK_DOM_NODE_TO_TAG(IBR, L"br");
			LINK_DOM_NODE_TO_TAG(IBUTTON, L"button");
			LINK_DOM_NODE_TO_TAG(IDIV, L"div");
			LINK_DOM_NODE_TO_TAG(IHEAD, L"head");
			LINK_DOM_NODE_TO_TAG(IHTML, L"html");
			LINK_DOM_NODE_TO_TAG(IIMG, L"img");
			LINK_DOM_NODE_TO_TAG(IINPUT, L"input");
			LINK_DOM_NODE_TO_TAG(ILABEL, L"label");
			LINK_DOM_NODE_TO_TAG(ILINK, L"link");
			LINK_DOM_NODE_TO_TAG(IOPTION, L"option");
			LINK_DOM_NODE_TO_TAG(IP, L"p");
			LINK_DOM_NODE_TO_TAG(ISELECT, L"select");
			LINK_DOM_NODE_TO_TAG(ISPAN, L"span");
			LINK_DOM_NODE_TO_TAG(ITABLE, L"table");
			LINK_DOM_NODE_TO_TAG(ITD, L"td");
			LINK_DOM_NODE_TO_TAG(ITEXTAREA, L"textarea");
			LINK_DOM_NODE_TO_TAG(ITH, L"th");
			LINK_DOM_NODE_TO_TAG(ITR, L"tr");
			LINK_DOM_NODE_TO_TAG(IWINDOW, L"window");
			LINK_DOM_NODE_TO_TAG(IVIDEO, L"video");
			LINK_DOM_NODE_TO_TAG(ICHECKBOX, L"checkbox");
			LINK_DOM_NODE_TO_TAG(IRADIO, L"radio");

		}
	};
};