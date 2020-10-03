#include "UIButton.h"

CUIButton::CUIButton(ULONG uID): 
	BaseClass(uID, L"button")
{

}

void XMETHODCALLTYPE CUIButton::createNode(gui::dom::IDOMdocument *pDomDocument, gui::dom::IDOMnode *pParentNode)
{
	m_pNode = pDomDocument->createNode(m_wsName.c_str());
	m_pNode->setAttribute(L"controld_id", StringW(m_id));
	m_pNode->setAttribute(L"onclick", m_wsName + m_id);
	pParentNode->appendChild(m_pNode, true);
}

void XMETHODCALLTYPE CUIButton::setPicture(const wchar_t *szName, int sizeX, int sizeY)
{
	wchar_t htmlNode[255];
	swprintf(htmlNode, L"<img src=\"%s\" height=\"%d\" width=\"%d\" />", szName, sizeX, sizeY);
	m_pNode->appendHTML(htmlNode);
}