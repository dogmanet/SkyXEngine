#include "CommandDelete.h"

CCommandDelete::~CCommandDelete()
{
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		mem_release(m_aObjects[i].pObject);
	}
}

bool XMETHODCALLTYPE CCommandDelete::exec()
{
	_del_obj *pObj;
	for(int i = m_aObjects.size() - 1; i >= 0; --i)
	{
		pObj = &m_aObjects[i];
		pObj->pObject->setSelected(false);
		pObj->pObject->remove();

		int idx = g_pLevelObjects.indexOf(pObj->pObject);
		if(idx >= 0)
		{
			mem_release(g_pLevelObjects[idx]);
			g_pLevelObjects.erase(idx);
		}
	}
	XUpdatePropWindow();
	return(m_aObjects.size());
}
bool XMETHODCALLTYPE CCommandDelete::undo()
{
	_del_obj *pObj;
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		pObj = &m_aObjects[i];

		pObj->pObject->create();

		/*pObj->pObject->setPos(pObj->vPos);
		pObj->pObject->setSize(pObj->vSize);
		pObj->pObject->setOrient(pObj->qRotate);*/

		pObj->pObject->preSetup();
		for(auto i = pObj->mKeyValues.begin(); i; ++i)
		{
			pObj->pObject->setKV(i.first->c_str(), i.second->c_str());
		}
		pObj->pObject->postSetup();

		add_ref(pObj->pObject);
		g_pLevelObjects.push_back(pObj->pObject);

		pObj->pObject->setSelected(pObj->wasSelected);
	}
	XUpdatePropWindow();
	return(true);
}

void CCommandDelete::addObject(IXEditorObject *pObj)
{
	_del_obj obj;
	add_ref(pObj);
	obj.pObject = pObj;
	obj.wasSelected = obj.pObject->isSelected();

	UINT uKeyCount = obj.pObject->getProperyCount();
	for(UINT i = 0; i < uKeyCount; ++i)
	{
		const char *szKey = obj.pObject->getPropertyMeta(i)->szKey;
		obj.mKeyValues[szKey] = obj.pObject->getKV(szKey);
	}

	/*float3 vMin, vMax;
	obj.pObject->getBound(&vMin, &vMax);

	obj.vPos = obj.pObject->getPos();
	obj.vSize = (float3)(vMax - vMin);
	obj.qRotate = obj.pObject->getOrient();*/

	m_aObjects.push_back(obj);
}
