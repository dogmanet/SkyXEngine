#ifndef __XEDITOR_EXTENSION_H
#define __XEDITOR_EXTENSION_H

#include "IXEditor.h"

class IXEditorCommand: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE exec() = 0;
	virtual bool XMETHODCALLTYPE undo() = 0;

	virtual bool XMETHODCALLTYPE isEmpty() = 0;

	virtual const char* XMETHODCALLTYPE getText() = 0;
};

//##########################################################################

class IXEditorPropertyTabCallback
{
public:
	virtual void XMETHODCALLTYPE onAdd() = 0;
	virtual void XMETHODCALLTYPE onRemove() = 0;
};

//##########################################################################

class IXEditorPropertyTab: public IXUnknown
{
public:
	virtual void XMETHODCALLTYPE init(HWND hWndParent, int X, int Y, int cx, int cy) = 0;
	virtual const char* XMETHODCALLTYPE getTitle() = 0;

	virtual void XMETHODCALLTYPE setCallback(IXEditorPropertyTabCallback *pCallback) = 0;

	virtual void XMETHODCALLTYPE show() = 0;
	virtual void XMETHODCALLTYPE hide() = 0;

	virtual IXEditorCommand* XMETHODCALLTYPE startNewCommand() = 0;

	virtual bool XMETHODCALLTYPE isEnabled() = 0;
};

//##########################################################################

//! @todo find more appropriate place for that
enum XACCEL_FLAG
{
	XAF_ALT = 0x01, //!< The ALT key must be held down when the accelerator key is pressed.
	XAF_CTRL = 0x02, //!< The CTRL key must be held down when the accelerator key is pressed.
	XAF_SHIFT = 0x04, //!< The SHIFT key must be held down when the accelerator key is pressed.
	XAF_VIRTKEY = 0x08, //!< The key member specifies a virtual-key code. If this flag is not specified, key is assumed to specify a character code.
};
DEFINE_ENUM_FLAG_OPERATORS(XACCEL_FLAG);

struct XAccelItem
{
	XACCEL_FLAG flags;
	UINT uKey;
};

class IXEditorTool: public IXUnknown
{
public:
	// HBITMAP for now
	virtual void* XMETHODCALLTYPE getIcon() = 0;
	virtual const char* XMETHODCALLTYPE getTitle() = 0;
	virtual XAccelItem XMETHODCALLTYPE getAccel() = 0;

	virtual void XMETHODCALLTYPE activate() = 0;
	virtual void XMETHODCALLTYPE deactivate() = 0;

	virtual bool XMETHODCALLTYPE wantMouse2D() = 0;
	virtual bool XMETHODCALLTYPE wantMouse3D() = 0;


	virtual bool XMETHODCALLTYPE onMouseDown() = 0;
	virtual bool XMETHODCALLTYPE onMouseMove() = 0;
	virtual void XMETHODCALLTYPE onMouseUp() = 0;

	// virtual bool XMETHODCALLTYPE onMouseEvent(UINT msg, WPARAM wParam, LPARAM lParam, ) = 0;
};

//##########################################################################

class IXEditorExtension: public IXUnknown
{
public:
	// additional toolbar buttons
	// additional property pages for classes

	virtual UINT XMETHODCALLTYPE getPropertyTabCount() = 0;
	virtual IXEditorPropertyTab* XMETHODCALLTYPE getPropertyTab(UINT uId) = 0;

	virtual UINT XMETHODCALLTYPE getToolCount() = 0;
	virtual bool XMETHODCALLTYPE getTool(UINT uId, IXEditorTool **ppOut) = 0;

	virtual void XMETHODCALLTYPE render(bool is3D) = 0;
};

#endif
