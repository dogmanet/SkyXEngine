#ifndef __EDITOR_EXTENSION_H
#define __EDITOR_EXTENSION_H

#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include <xcommon/IXCore.h>
#include "EffectBrowserWindow.h"

class CEditable;
class CEditorExtension final: public IXUnknownImplementation<IXEditorExtension>
{
public:
	CEditorExtension(CEditable *pEditable, IXEditor *pEditor, IXCore *pCore);
	~CEditorExtension();

	UINT XMETHODCALLTYPE getPropertyTabCount() override;
	IXEditorPropertyTab* XMETHODCALLTYPE getPropertyTab(UINT uId) override;

	UINT XMETHODCALLTYPE getToolCount() override;
	bool XMETHODCALLTYPE getTool(UINT uId, IXEditorTool **ppOut) override;

	void XMETHODCALLTYPE render(bool is3D) override;

	UINT XMETHODCALLTYPE getResourceBrowserCount() override;
	bool XMETHODCALLTYPE getResourceBrowser(UINT uId, IXEditorResourceBrowser **ppOut) override;

private:
	CEditable *m_pEditable = NULL;

	CEffectBrowserWindow *m_pEffectBrowserWindow = NULL;
};

#endif
