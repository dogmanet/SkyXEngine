#ifndef __WINDOWSYSTEM_H
#define __WINDOWSYSTEM_H

#include "IXWindowSystem.h"

class CWindowSystem: public IXWindowSystem
{
public:
	IXWindow* XMETHODCALLTYPE createWindow(const XWINDOW_DESC *pWindowDesc, IXWindowCallback *pCallback = NULL, IXWindow *pParent = NULL) override;

	bool XMETHODCALLTYPE processMessages(UINT uMaxMessages = 0) override;

protected:
	UINT m_uNextWindowId = 0;
};

#endif
