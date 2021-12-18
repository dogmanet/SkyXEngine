#include "CommandDelete.h"

CCommandDelete::~CCommandDelete()
{
	fora(i, m_aObjects)
	{
		mem_release(m_aObjects[i].pObject);
	}
}

bool XMETHODCALLTYPE CCommandDelete::exec()
{
	_del_obj *pObj;
	forar(i, m_aObjects)
	{
		pObj = &m_aObjects[i];

		if(pObj->pLocation)
		{
			pObj->pLocation->removeChildObject(pObj->pObject);
		}

		pObj->pObject->setSelected(false);
		pObj->pObject->remove();

		g_pEditor->removeObject(pObj->pObject);
	}
	XUpdatePropWindow();
	return(m_aObjects.size());
}
bool XMETHODCALLTYPE CCommandDelete::undo()
{
	_del_obj *pObj;
	fora(i, m_aObjects)
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

		g_pEditor->addObject(pObj->pObject);

		pObj->pObject->setSelected(pObj->wasSelected);

		if(pObj->pLocation)
		{
			pObj->pLocation->addChildObject(pObj->pObject);
		}
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
	obj.pLocation = XGetObjectParent(obj.pObject);

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
