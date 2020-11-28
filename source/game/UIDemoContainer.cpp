#include "UIDemoContainer.h"

UIDemoContainer::UIDemoContainer(IXUI* pXUI)
{
	XWINDOW_DESC wdesc;
	wdesc.iPosX = XCW_CENTER;
	wdesc.iPosY = XCW_CENTER;
	wdesc.iSizeX = 800;
	wdesc.iSizeY = 600;
	wdesc.szTitle = "Demo window";
	wdesc.flags = XWF_BUTTON_CLOSE | XWF_BUTTON_MINIMIZE | XWF_BUTTON_MAXIMIZE | XWF_TRANSPARENT;

	pDemo = pXUI->createWindow(&wdesc);
}