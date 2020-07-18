#ifndef __UITEXTBOX_H
#define __UITEXTBOX_H

#include "UIControl.h"
#include "IUITextBox.h"

class CUITextBox : public CUIControl<IUITextBox>
{
public:
	CUITextBox(ULONG uID);

	void createNode(gui::dom::IDOMdocument *pDomDocument, gui::dom::IDOMnode *pParentNode) override;
};

#endif
