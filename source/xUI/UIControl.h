#ifndef __UICONTROL_H
#define __UICONTROL_H

#include <gui/guimain.h>
#include "IUICommand.h"

template <typename T>
class CUIControl : public IXUnknownImplementation<T>
{
public:
	typedef CUIControl<T> BaseClass;

	CUIControl(ULONG uID, const StringW &sName)
		: m_id(uID), m_wsName(sName)
	{ }

	void setLabel(const wchar_t *szTitle) override
	{
		m_pNode->setText(szTitle, true);
	}

	void setPosition(float fSizeX, float fSizeY) override
	{
		m_pNode->getStyleSelf()->position->setExt(gui::css::ICSSproperty::POSITION_ABSOLUTE);
		m_pNode->getStyleSelf()->left->setDim(gui::css::ICSSproperty::DIM_PX);
		m_pNode->getStyleSelf()->top->setDim(gui::css::ICSSproperty::DIM_PX);
		m_pNode->getStyleSelf()->left->set(fSizeX);
		m_pNode->getStyleSelf()->top->set(fSizeY);
		m_pNode->updateStyles();
	}

	void setSize(float fSizeX, float fSizeY) override
	{
		m_pNode->getStyleSelf()->height->setDim(gui::css::ICSSproperty::DIM_PX);
		m_pNode->getStyleSelf()->width->setDim(gui::css::ICSSproperty::DIM_PX);
		m_pNode->getStyleSelf()->width->set(fSizeX);
		m_pNode->getStyleSelf()->height->set(fSizeY);
		m_pNode->updateStyles();
	}

	void setClickCommand(IUICommand *pCommand) override
	{
		m_pClick = pCommand;
	}

	ULONG getElementID() const override
	{
		return m_id;
	}

	~CUIControl()
	{
		mem_delete(m_pClick);
		mem_delete(m_pKeyUp);
	}

protected:
	IUICommand *m_pClick = NULL;
	IUICommand *m_pKeyUp = NULL;

	gui::dom::IDOMnode *m_pNode = NULL;
	const ULONG m_id;
	const StringW m_wsName;

	friend class CUIWindow;
};

#endif
