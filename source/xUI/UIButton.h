#ifndef __UIBUTTON_H
#define __UIBUTTON_H

#include "IUIButton.h"

class CUIButton : public IUIButton
{
public:
	void XMETHODCALLTYPE setClickCommand(IUICommand* command) override;

private:
	IUICommand *m_pCommand = NULL;
};

#endif
