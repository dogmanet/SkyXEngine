#include "CommandProperties.h"

bool CCommandProperties::exec()
{
	_prop_obj *pObj;
	for(UINT j = 0, jl = m_aObjects.size(); j < jl; ++j)
	{
		pObj = &m_aObjects[j];

		pObj->pObject->preSetup();
		for(auto i = pObj->mKeyValues.begin(); i; ++i)
		{
			if(i.second->isChanged)
			{
				pObj->pObject->setKV(i.first->c_str(), i.second->sNew.c_str());
			}
		}
		pObj->pObject->postSetup();
	}
	XUpdatePropWindow();
	return(true);
}
bool CCommandProperties::undo()
{
	_prop_obj *pObj;
	for(UINT j = 0, jl = m_aObjects.size(); j < jl; ++j)
	{
		pObj = &m_aObjects[j];

		pObj->pObject->preSetup();
		for(auto i = pObj->mKeyValues.begin(); i; ++i)
		{
			if(i.second->isChanged)
			{
				pObj->pObject->setKV(i.first->c_str(), i.second->sOld.c_str());
			}
		}
		pObj->pObject->postSetup();
	}
	XUpdatePropWindow();
	return(true);
}

void CCommandProperties::addObject(ID idObject)
{
	_prop_obj obj;
	obj.pObject = g_pLevelObjects[idObject];

	UINT uKeyCount = obj.pObject->getProperyCount();
	for(UINT i = 0; i < uKeyCount; ++i)
	{
		const char *szKey = obj.pObject->getPropertyMeta(i)->szKey;
		obj.mKeyValues[szKey].sOld = obj.pObject->getKV(szKey);
	}

	m_aObjects.push_back(obj);
}

void CCommandProperties::setKV(const char *szKey, const char *szValue)
{
	for(UINT i = 0, l = m_aObjects.size(); i < l; ++i)
	{
		_prop_kv *pKV = &m_aObjects[i].mKeyValues[szKey];
		pKV->sNew = szValue;
		pKV->isChanged = pKV->sOld != pKV->sNew;

		m_aObjects[i].pObject->setKV(szKey, szValue);
	}
}
