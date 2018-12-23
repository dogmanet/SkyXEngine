#ifndef _IEvent_H_
#define _IEvent_H_

#ifdef GUI_DLL
#include "GUIbase.h"
#else
#include "guimain.h"
#endif
#include "keymap.h"

namespace gui
{
	namespace dom
	{
		class IDOMnode;
	};

	enum GUI_EVENT_TYPE
	{
		GUI_EVENT_TYPE_UNKNOWN = 0,
		GUI_EVENT_TYPE_MOUSEDOWN,
		GUI_EVENT_TYPE_MOUSEUP,
		GUI_EVENT_TYPE_CLICK,
		GUI_EVENT_TYPE_DBLCLICK,
		GUI_EVENT_TYPE_MOUSEENTER,
		GUI_EVENT_TYPE_MOUSELEAVE,
		GUI_EVENT_TYPE_BLUR,
		GUI_EVENT_TYPE_FOCUS,
		GUI_EVENT_TYPE_KEYDOWN,
		GUI_EVENT_TYPE_KEYUP,
		GUI_EVENT_TYPE_KEYPRESS,
		GUI_EVENT_TYPE_MOUSEMOVE,
		GUI_EVENT_TYPE_MOUSEWHEELDOWN,
		GUI_EVENT_TYPE_MOUSEWHEELUP
	};

	class IEvent
	{
	public:
		IEvent():propagate(true), key(0), syskey(false), target(NULL), relatedTarget(NULL), clientX(0), clientY(0),
			offsetX(0), offsetY(0), currentTarget(NULL), preventDefault(false)
		{
		}
		void stopPropagation() // cancel bubble
		{
			propagate = false;
		}
		//protected:
		GUI_EVENT_TYPE type;
		bool propagate;
		UINT key;
		bool syskey;
		bool preventDefault;

		dom::IDOMnode * target;
		dom::IDOMnode * relatedTarget;
		dom::IDOMnode * currentTarget;

		int clientX;
		int clientY;
		int offsetX;
		int offsetY;
	};
};

#endif
