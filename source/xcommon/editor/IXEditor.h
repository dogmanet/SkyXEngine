#ifndef __XEDITOR_H
#define __XEDITOR_H

#include <gdefines.h>
#include "IXEditorGizmo.h"

enum X_WINDOW_POS
{
	XWP_TOP_LEFT,
	XWP_TOP_RIGHT,
	XWP_BOTTOM_LEFT,
	XWP_BOTTOM_RIGHT
};

//##########################################################################

// {84ECF1FC-4C03-4EB9-BC39-D991B83F73BA}
#define IXEDITOR_GUID DEFINE_XGUID(0x84ecf1fc, 0x4c03, 0x4eb9, 0xbc, 0x39, 0xd9, 0x91, 0xb8, 0x3f, 0x73, 0xba)
#define IXEDITOR_VERSION 1

class ICamera;
class IXEditor: public IXUnknown
{
public:
	virtual void XMETHODCALLTYPE getCameraForView(X_WINDOW_POS winPos, ICamera **ppCamera) = 0;

	virtual void XMETHODCALLTYPE newGizmoHandle(IXEditorGizmoHandle **ppOut) = 0;
	virtual void XMETHODCALLTYPE newGizmoRadius(IXEditorGizmoRadius **ppOut) = 0;
	virtual void XMETHODCALLTYPE newGizmoMove(IXEditorGizmoMove **ppOut) = 0;
	virtual void XMETHODCALLTYPE newGizmoRotate(IXEditorGizmoRotate **ppOut) = 0;
	virtual void XMETHODCALLTYPE newGizmoScale(IXEditorGizmoScale **ppOut) = 0;
};


#endif
