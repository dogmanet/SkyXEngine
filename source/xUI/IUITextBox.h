#ifndef __IUITEXTBOX_H
#define __IUITEXTBOX_H

#include "IUIControl.h"

class IUITextBox : public IUIControl
{
public:
	//Метод возвращает содержимое контрола
	char* XMETHODCALLTYPE getText();
};

#endif
