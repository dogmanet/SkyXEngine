#ifndef __XEDITOR_EXTENSION_H
#define __XEDITOR_EXTENSION_H

#include <gdefines.h>

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

class IXEditorExtension: public IXUnknown
{
public:
	// additional toolbar buttons
	// additional property pages for classes

	virtual UINT XMETHODCALLTYPE getPropertyTabCount() = 0;
	virtual IXEditorPropertyTab* XMETHODCALLTYPE getPropertyTab(UINT uId) = 0;
};

#endif
