#include "CommandClip.h"
#include "Editable.h"

CCommandClip::CCommandClip(IXEditor *pEditor, CEditable *pEditable):
	m_pEditor(pEditor),
	m_pEditable(pEditable)
{
	m_clipPlaneState = m_pEditable->getClipPlaneState();
	m_clipPlane = m_pEditable->getClipPlane();

	const Array<CEditorObject*> aObjects = m_pEditable->getObjects();

	for(UINT i = 0, l = aObjects.size(); i < l; ++i)
	{
		CEditorObject *pObj = aObjects[i];
		if(pObj->isSelected())
		{
			int cls = pObj->classify(m_clipPlane);
			if(cls == 0)
			{
				// clip object
				clip_obj_t &clip = m_aClippedObjects[m_aClippedObjects.size()];
				clip.pDstObj = NULL;
				add_ref(pObj);
				clip.pSrcObj = pObj;
				clip.sBefore = pObj->getKV("brush");
			}
			else if(cls > 0)
			{
				// skip
			}
			else if(m_clipPlaneState == CPS_ONESIDE)
			{
				add_ref(pObj);
				m_aObjectsToRemove.push_back(pObj);
			}
		}
	}
}

CCommandClip::~CCommandClip()
{
	for(UINT i = 0, l = m_aObjectsToRemove.size(); i < l; ++i)
	{
		mem_release(m_aObjectsToRemove[i]);
	}
	for(UINT i = 0, l = m_aClippedObjects.size(); i < l; ++i)
	{
		mem_release(m_aClippedObjects[i].pSrcObj);
		mem_release(m_aClippedObjects[i].pDstObj);
	}
}

bool XMETHODCALLTYPE CCommandClip::exec()
{
	if(m_clipPlaneState == CPS_NONE)
	{
		return(false);
	}
	for(UINT i = 0, l = m_aObjectsToRemove.size(); i < l; ++i)
	{
		CEditorObject *pObj = m_aObjectsToRemove[i];
		pObj->setSelected(false);
		pObj->remove();
		m_pEditor->removeObject(pObj);
	}

	if(m_clipPlaneState == CPS_TWOSIDE)
	{
		for(UINT i = 0, l = m_aClippedObjects.size(); i < l; ++i)
		{
			clip_obj_t &co = m_aClippedObjects[i];
			if(!co.pDstObj)
			{
				co.pDstObj = new CEditorObject(m_pEditable);
				co.pDstObj->setPos(co.pSrcObj->getPos());
				co.pDstObj->setOrient(co.pSrcObj->getOrient());

				for(UINT j = 0, jl = co.pSrcObj->getProperyCount(); j < jl; ++j)
				{
					const X_PROP_FIELD *pField = co.pSrcObj->getPropertyMeta(j);
					co.pDstObj->setKV(pField->szKey, co.pSrcObj->getKV(pField->szKey), true);
				}

				co.pDstObj->clip(-m_clipPlane);

				co.pDstObj->setSelected(true);
			}
			else
			{
				co.pDstObj->create();
				co.pDstObj->setSelected(true);
			}
			m_pEditor->addObject(co.pDstObj);
		}
	}

	for(UINT i = 0, l = m_aClippedObjects.size(); i < l; ++i)
	{
		m_aClippedObjects[i].pSrcObj->clip(m_clipPlane);
	}

	return(true);
}
bool XMETHODCALLTYPE CCommandClip::undo()
{
	for(UINT i = 0, l = m_aClippedObjects.size(); i < l; ++i)
	{
		m_aClippedObjects[i].pSrcObj->setKV("brush", m_aClippedObjects[i].sBefore.c_str(), true);
	}

	if(m_clipPlaneState == CPS_TWOSIDE)
	{
		for(UINT i = 0, l = m_aClippedObjects.size(); i < l; ++i)
		{
			clip_obj_t &co = m_aClippedObjects[i];
			m_pEditor->removeObject(co.pDstObj);
			co.pDstObj->remove();
			co.pDstObj->setSelected(false);
		}
	}

	for(UINT i = 0, l = m_aObjectsToRemove.size(); i < l; ++i)
	{
		CEditorObject *pObj = m_aObjectsToRemove[i];
		pObj->create();
		pObj->setSelected(true);
		m_pEditor->addObject(pObj);
	}

	return(true);
}
