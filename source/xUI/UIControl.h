#ifndef __UICONTROL_H
#define __UICONTROL_H

#include "IUIControl.h"
#include <gui/guimain.h>

class СUIControl : public IUIControl
{
public:
	СUIControl();

private:
	gui::dom::IDOMnode* m_pNode;
};

#endif
