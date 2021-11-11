#include "CommandCreate.h"
#include <common/aastring.h>

extern AssotiativeArray<AAString, IXEditable*> g_mEditableSystems;

CCommandCreate::CCommandCreate(IXEditor *pEditor, CEditorObject *pObject):
	m_pEditor(pEditor),
	m_pObject(pObject)
{
}

CCommandCreate::~CCommandCreate()
{
	mem_release(m_pObject);
}

bool XMETHODCALLTYPE CCommandCreate::exec()
{
	m_pObject->create();

	m_pEditor->addObject(m_pObject);

	return(true);
}
bool XMETHODCALLTYPE CCommandCreate::undo()
{
	m_pObject->remove();

	m_pEditor->removeObject(m_pObject);

	return(true);
}
