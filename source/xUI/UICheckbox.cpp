#include "UICheckbox.h"

CUICheckBox::CUICheckBox(ULONG uID) :
	BaseClass(uID, L"checkbox")
{

}

void CUICheckBox::createNode(gui::dom::IDOMdocument *pDomDocument, gui::dom::IDOMnode *pParentNode)
{
	m_pNode = pDomDocument->createNode(L"div");
	m_pText = pDomDocument->createNode(L"label");
	m_pCheckBox = pDomDocument->createNode(m_wsName.c_str());

	m_pNode->setAttribute(L"controld_id", StringW(m_id));
	m_pNode->setAttribute(L"onclick", m_wsName + m_id);
	//m_pCheckBox->setAttribute(L"onclick", m_wsName + m_id);

	m_pNode->appendChild(m_pCheckBox, true);
	m_pNode->appendChild(m_pText, true);
	pParentNode->appendChild(m_pNode, true);
}

void CUICheckBox::setLabel(const wchar_t *szTitle)
{
	m_pText->setText(szTitle, true);
}

bool CUICheckBox::isChecked()
{
	//! @fixme: добавить enum
	return m_pCheckBox->pseudoclassExists(1);
}
