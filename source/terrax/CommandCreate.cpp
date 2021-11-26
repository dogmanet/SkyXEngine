#include "CommandCreate.h"
#include <common/aastring.h>

extern AssotiativeArray<AAString, IXEditable*> g_mEditableSystems;

CCommandCreate::CCommandCreate(const float3_t vPos, const char *szTypeName, const char *szClassName, bool useRandomScaleYaw)
{
	m_vPos = vPos;
	m_sClassName = szClassName;

	if(useRandomScaleYaw)
	{
		m_fScale = randf(0.7, 1.3);
		m_qOrient = SMQuaternion(randf(0, SM_2PI), 'y');
	}

	const AssotiativeArray<AAString, IXEditable*>::Node *pNode;
	if(!g_mEditableSystems.KeyExists(AAString(szTypeName), &pNode))
	{
		assert(!"Invalid type name!");
		return;
	}

	m_pEditable = *(pNode->Val);
}

CCommandCreate::~CCommandCreate()
{
	mem_release(m_pObject);
}

bool XMETHODCALLTYPE CCommandCreate::exec()
{
	if(!m_pObject)
	{
		m_pObject = m_pEditable->newObject(m_sClassName.c_str());
	}
	m_pObject->setPos(m_vPos);
	//! @TODO Implement random scale?
	//m_pObject->setScale(float3(m_fScale));
	m_pObject->setOrient(m_qOrient);
	m_pObject->setSelected(true);
	m_pObject->create();

	g_pLevelObjects.push_back(m_pObject);
	add_ref(m_pObject);

	XUpdatePropWindow();
	return(true);
}
bool XMETHODCALLTYPE CCommandCreate::undo()
{
	m_pObject->remove();
	int idx = g_pLevelObjects.indexOf(m_pObject);
	if(idx >= 0)
	{
		mem_release(g_pLevelObjects[idx]);
		g_pLevelObjects.erase(idx);
	}

	XUpdatePropWindow();
	return(true);
}
