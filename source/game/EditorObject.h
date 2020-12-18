#ifndef __EDITOROBJECT_H
#define __EDITOROBJECT_H

#include <xcommon/editor/IXEditorObject.h>
#include "BaseEntity.h"
#include <common/string.h>
#include <common/array.h>

class CEditable;
class CEditorObject final: public IXUnknownImplementation<IXEditorObject>
{
	DECLARE_CLASS(CEditorObject, IXEditorObject);
public:
	CEditorObject(CEditable *pEditable, const char *szClassName);
	CEditorObject(CEditable *pEditable, CBaseEntity *pEntity);
	~CEditorObject();

	void XMETHODCALLTYPE setPos(const float3_t &pos) override;
	void XMETHODCALLTYPE setOrient(const SMQuaternion &orient) override;
	void XMETHODCALLTYPE setScale(const float3_t &pos) override;

	void XMETHODCALLTYPE getBound(float3 *pvMin, float3 *pvMax) override;

	void XMETHODCALLTYPE renderSelection(bool is3D) override;

	bool XMETHODCALLTYPE rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl) override;

	void XMETHODCALLTYPE remove() override;
	void XMETHODCALLTYPE create() override;
	void XMETHODCALLTYPE preSetup() override;
	void XMETHODCALLTYPE postSetup() override;

	void resync();

	void XMETHODCALLTYPE setKV(const char *szKey, const char *szValue) override;
	const char* XMETHODCALLTYPE getKV(const char *szKey) override;
	const X_PROP_FIELD* XMETHODCALLTYPE getPropertyMeta(UINT uKey) override;
	UINT XMETHODCALLTYPE getProperyCount() override;

	const char* XMETHODCALLTYPE getTypeName() override;
	const char* XMETHODCALLTYPE getClassName() override;



	float3_t XMETHODCALLTYPE getPos() override;

	SMQuaternion XMETHODCALLTYPE getOrient() override;

	float3_t XMETHODCALLTYPE getScale() override
	{
		return(m_vScale);
	}

	bool XMETHODCALLTYPE isSelected() override
	{
		return(m_isSelected);
	}
	void XMETHODCALLTYPE setSelected(bool set) override;

	CBaseEntity* getEnt()
	{
		return(m_pEntity);
	}

protected:
	CBaseEntity *m_pEntity = NULL;
	const char *m_szClassName = NULL;

	CEditable *m_pEditable;

	Array<X_PROP_FIELD> m_aFields;
	AssotiativeArray<String, String> m_msPropCache;
	const char *m_aszFlags[16];

	void _iniFieldList();

	XGUID m_guidEnt;

protected:
	bool m_isSelected = false;

	float3_t m_vPos;
	SMQuaternion m_qRot;
	float3_t m_vScale = float3_t(1.0f, 1.0f, 1.0f);
};

#endif
