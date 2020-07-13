#include "CommandDelete.h"

bool XMETHODCALLTYPE CCommandDelete::exec()
{
	_del_obj *pObj;
	for(int i = m_aObjects.size() - 1; i >= 0; --i)
	{
		pObj = &m_aObjects[i];

		pObj->pObject->remove();
		g_pLevelObjects.erase(pObj->idObject);
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

		pObj->pObject->setPos(pObj->vPos);
		pObj->pObject->setScale(pObj->vScale);
		pObj->pObject->setOrient(pObj->qRotate);

		pObj->pObject->preSetup();
		for(auto i = pObj->mKeyValues.begin(); i; ++i)
		{
			pObj->pObject->setKV(i.first->c_str(), i.second->c_str());
		}
		pObj->pObject->postSetup();

		for(UINT i = g_pLevelObjects.size(); i > pObj->idObject; --i)
		{
			g_pLevelObjects[i] = g_pLevelObjects[i - 1];
		}
		g_pLevelObjects[pObj->idObject] = pObj->pObject;
	}
	XUpdatePropWindow();
	return(true);
}

void CCommandDelete::addObject(ID idObject)
{
	_del_obj obj;
	obj.idObject = idObject;
	obj.pObject = g_pLevelObjects[idObject];

	UINT uKeyCount = obj.pObject->getProperyCount();
	for(UINT i = 0; i < uKeyCount; ++i)
	{
		const char *szKey = obj.pObject->getPropertyMeta(i)->szKey;
		obj.mKeyValues[szKey] = obj.pObject->getKV(szKey);
	}

	obj.vPos = obj.pObject->getPos();
	obj.vScale = obj.pObject->getScale();
	obj.qRotate = obj.pObject->getOrient();

	m_aObjects.push_back(obj);
}
