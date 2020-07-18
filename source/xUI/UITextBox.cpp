#include "UITextBox.h"

CUITextBox::CUITextBox(ULONG uID) :
	BaseClass(uID, L"input")
{

}

void CUITextBox::createNode(gui::dom::IDOMdocument *pDomDocument, gui::dom::IDOMnode *pParentNode)
{
	m_pNode = pDomDocument->createNode(m_wsName.c_str());
	m_pNode->setAttribute(L"controld_id", StringW(m_id));
	pParentNode->appendChild(m_pNode, true);
}