#ifndef __IUICHECKBOX_H
#define __IUICHECKBOX_H

#include "IUIControl.h"

class IUICheckbox : public IUIControl
{
public:
	virtual bool isChecked() = 0;
};

#endif
