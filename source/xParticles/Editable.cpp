#include "Editable.h"
#include <xcommon/IPluginManager.h>
#include <xcommon/IXModelWriter.h>

CEditable::CEditable(IXCore *pCore):
	m_pCore(pCore)
{
	IXEditor *pEditor = (IXEditor*)pCore->getPluginManager()->getInterface(IXEDITOR_GUID);
	m_pEditorExtension = new CEditorExtension(this, pEditor, pCore);
}

CEditable::~CEditable()
{
	mem_delete(m_pEditorExtension);
}

void XMETHODCALLTYPE CEditable::startup(IGXDevice *pDevice)
{
	m_pDevice = pDevice;
}
void XMETHODCALLTYPE CEditable::shutdown()
{
}
