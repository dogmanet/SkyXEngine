#ifndef __UIBUTTON_H
#define __UIBUTTON_H

#include "IUIControl.h"

class IUIButton : public IUIControl
{
public:
	//Метод установки команды обработчика на клик кнопки
	virtual void XMETHODCALLTYPE setCommand(IUICommand command) = 0;
};

#endif