#ifndef __EDITOROBJECT_H
#define __EDITOROBJECT_H

#include <xcommon/editor/IXEditorObject.h>
#include "BaseEntity.h"
#include <common/string.h>
#include <common/array.h>

class CEditable;
class CEditorObject: public IXUnknownImplementation<IXEditorObject>
{
	DECLARE_CLASS(CEditorObject, IXEditorObject);
public:
	CEditorObject(CEditable *pEditable, const char *szClassName);
	CEditorObject(CEditable *pEditable, CBaseEntity *pEntity);
	~CEditorObject();

	void setPos(const float3_t &pos);
	void setOrient(const SMQuaternion &orient);
	void setScale(const float3_t &pos);

	void getBound(float3 *pvMin, float3 *pvMax);

	void renderSelection(bool is3D);

	bool rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl);

	void remove();
	void create();
	void preSetup();
	void postSetup();

	void setKV(const char *szKey, const char *szValue);
	const char *getKV(const char *szKey);
	const X_PROP_FIELD *getPropertyMeta(UINT uKey);
	UINT getProperyCount();

	const char *getTypeName();
	const char *getClassName();

protected:
	CBaseEntity *m_pEntity = NULL;
	const char *m_szClassName = NULL;

	CEditable *m_pEditable;

	Array<X_PROP_FIELD> m_aFields;
	AssotiativeArray<String, String> m_msPropCache;
	const char *m_aszFlags[16];


	void _iniFieldList();
};

#endif
