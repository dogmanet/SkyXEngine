#ifndef __UICKECKBOX_H
#define __UICKECKBOX_H

#include "UIControl.h"
#include "IUICheckbox.h"

class CUICheckBox : public CUIControl<IUICheckbox>
{
public:
	CUICheckBox(ULONG uID);

	void createNode(gui::dom::IDOMdocument *pDomDocument, gui::dom::IDOMnode *pParentNode) override;

	void setLabel(const wchar_t *szTitle) override;

	bool isChecked() override;

private:
	gui::dom::IDOMnode *m_pText;
	gui::dom::IDOMnode *m_pCheckBox;
};

#endif
