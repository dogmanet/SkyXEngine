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

	void setPos(const float3_t &pos) override;
	void setOrient(const SMQuaternion &orient) override;
	void setScale(const float3_t &pos) override;

	void getBound(float3 *pvMin, float3 *pvMax) override;

	void renderSelection(bool is3D) override;

	bool rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl) override;

	void remove() override;
	void create() override;
	void preSetup() override;
	void postSetup() override;

	void resync();

	void setKV(const char *szKey, const char *szValue) override;
	const char *getKV(const char *szKey) override;
	const X_PROP_FIELD *getPropertyMeta(UINT uKey) override;
	UINT getProperyCount() override;

	const char *getTypeName() override;
	const char *getClassName() override;

protected:
	CBaseEntity *m_pEntity = NULL;
	const char *m_szClassName = NULL;

	CEditable *m_pEditable;

	Array<X_PROP_FIELD> m_aFields;
	AssotiativeArray<String, String> m_msPropCache;
	const char *m_aszFlags[16];

	void _iniFieldList();

	ID m_idEnt = -1;
};

#endif
