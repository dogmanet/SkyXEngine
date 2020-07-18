#ifndef __IUICONTROL_H
#define __IUICONTROL_H

#include "IUICommand.h"
#include <gui/guimain.h>

class IUIControl : public IXUnknown
{
public:
	//! Метод установки надписи в контроле
	virtual void XMETHODCALLTYPE setLabel(const wchar_t *szTitle) = 0;

	//! Метод установки позиции в окне
	virtual void XMETHODCALLTYPE setPosition(float fPosX, float fPosY) = 0;

	////! Метод установки видимости для контрола
	//virtual void XMETHODCALLTYPE setVisible(bool bVisible) = 0;

	//! Метод установки размера по высоте и ширине
	virtual void XMETHODCALLTYPE setSize(float fSizeX, float fSizeY) = 0;

	////! Метод возвращает видимость контрола
	//virtual bool XMETHODCALLTYPE isVisible() = 0;

	////! Метод позволяет "включить/выключить" контрол
	//virtual void XMETHODCALLTYPE setEnableMode(bool bEnable) = 0;

	////! Возвращает состояние контрола, включен ли он или нет
	//virtual bool XMETHODCALLTYPE isEnabled() = 0;

	//! Метод создает DOMnode для контрола и помещает его в DOMnode родителя
	virtual void XMETHODCALLTYPE createNode(gui::dom::IDOMdocument *pDomDocument, gui::dom::IDOMnode *pParentNode) = 0;

	//! Метод позволяет задавать дочерние и вложенные контролы 
//	virtual void XMETHODCALLTYPE addChildren(IUIControl *pChildren) = 0;

	//! Метод устанавливает обработчик на клик по контролу
	virtual void XMETHODCALLTYPE setClickCommand(IUICommand *pCommand) = 0;

	//! Возвращает ID элемента
	virtual ULONG XMETHODCALLTYPE getElementID() const = 0;

};

#endif
