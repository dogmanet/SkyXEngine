#include "UIButton.h"

CUIButton::CUIButton(ULONG uID): 
	BaseClass(uID, L"button")
{

}

void CUIButton::createNode(gui::dom::IDOMdocument *pDomDocument, gui::dom::IDOMnode *pParentNode)
{
	m_pNode = pDomDocument->createNode(m_wsName.c_str());
	m_pNode->setAttribute(L"controld_id", StringW(m_id));
	m_pNode->setAttribute(L"onclick", m_wsName + m_id);
	pParentNode->appendChild(m_pNode, true);
}