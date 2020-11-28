#ifndef __UIBUTTON_H
#define __UIBUTTON_H

#include "UIControl.h"
#include "IUIButton.h"

class CUIButton : public CUIControl<IUIButton>
{
public:
	CUIButton(ULONG uID);

	void XMETHODCALLTYPE createNode(gui::dom::IDOMdocument *pDomDocument, gui::dom::IDOMnode *pParentNode) override;

	void XMETHODCALLTYPE setPicture(const wchar_t *szName, int sizeX, int sizeY) override;
};

#endif