#ifndef __EDITOROBJECT_H
#define __EDITOROBJECT_H

#include <xcommon/editor/IXEditorObject.h>
#include "sxgeom.h"
#include <common/string.h>
#include <common/array.h>

class CEditable;
class CEditorObject: public IXEditorObject
{
	DECLARE_CLASS(CEditorObject, IXEditorObject);
public:
	CEditorObject(CEditable *pEditable);
	CEditorObject(CEditable *pEditable, ID idModel);
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
	ID m_idModel = -1;
	String m_sModelName;
	String m_sName;
	bool m_bSegmentation = false;
	char m_szSegmentation[2];

	bool m_isCreated = false;

	static Array<CEditorObject*> ms_aObjects;

	CEditable *m_pEditable;
};

#endif
