#ifndef __IUICONTROL_H
#define __IUICONTROL_H

#include "IUICommand.h"

class IUIControl : public IXUnknown
{
public:
	//! Метод установки надписи в контроле
	virtual void XMETHODCALLTYPE setLabel(const char *szTitle) = 0;

	//! Метод установки позиции в окне
	virtual void XMETHODCALLTYPE setPosition(int iPosX, int iPosY) = 0;

	//! Метод установки видимости для контрола
	virtual void XMETHODCALLTYPE setVisible(bool bVisible) = 0;

	//! Метод установки размера по высоте и ширине
	virtual void XMETHODCALLTYPE setSize(int iSizeX, int iSizeY) = 0;

	//! Метод возвращает видимость контрола
	virtual bool XMETHODCALLTYPE isVisible() = 0;

	//! Метод устанавливает имя элемента
	virtual bool XMETHODCALLTYPE setName(const char *szName) = 0;

	//! Метод позволяет "включить/выключить" контрол
	virtual void XMETHODCALLTYPE setEnableMode(bool bEnable) = 0;

	//! Возвращает состояние контрола, включен ли он или нет
	virtual bool XMETHODCALLTYPE isEnabled() = 0;

	//! Метод устанавливает ID элемента
	virtual void XMETHODCALLTYPE setElementID(UINT uID) = 0;
};

#endif
