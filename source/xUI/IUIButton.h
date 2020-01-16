#ifndef __IUIBUTTON_H
#define __IUIBUTTON_H

#include "IUIControl.h"

class IUIButton : public IUIControl
{
public:
	//Метод установки команды обработчика на клик кнопки
	virtual void XMETHODCALLTYPE setClickCommand(IUICommand* command) = 0;
};

#endif
