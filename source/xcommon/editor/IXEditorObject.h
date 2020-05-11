#ifndef XEDITOROBJECT_H_
#define XEDITOROBJECT_H_

#include <gdefines.h>
#include <common/math.h>

enum X_PROP_EDITOR_TYPE
{
	XPET_TEXT,
	XPET_FILE,
	XPET_COMBO,
	XPET_FLAGS,
	//XPET_YESNO,

	XPET__LAST
};

struct X_PROP_FIELD
{
	const char *szKey;
	const char *szName;
	X_PROP_EDITOR_TYPE editorType;
	const void *pEditorData;
	const char *szHelp;
};

class IXEditorObject: public IXUnknown
{
public:
	virtual float3_t getPos()
	{
		return(m_vPos);
	}
	virtual void setPos(const float3_t &pos)
	{
		m_vPos = pos;
	}

	virtual SMQuaternion getOrient()
	{
		return(m_qRot);
	}
	virtual void setOrient(const SMQuaternion &orient)
	{
		m_qRot = orient;
	}

	virtual float3_t getScale()
	{
		return(m_vScale);
	}
	virtual void setScale(const float3_t &vScale)
	{
		m_vScale = vScale;
	}

	virtual void getBound(float3 *pvMin, float3 *pvMax) = 0;

	bool isSelected()
	{
		return(m_isSelected);
	}
	void setSelected(bool set)
	{
		m_isSelected = set;
	}

	virtual void renderSelection(bool is3D) = 0;

	virtual bool rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl) = 0;

	virtual void remove() = 0;
	virtual void create() = 0;
	virtual void preSetup() = 0;
	virtual void postSetup() = 0;

	virtual void setKV(const char *szKey, const char *szValue) = 0;
	virtual const char *getKV(const char *szKey) = 0;
	virtual const X_PROP_FIELD *getPropertyMeta(UINT uKey) = 0;
	virtual UINT getProperyCount() = 0;

	virtual const char *getTypeName() = 0;
	virtual const char *getClassName() = 0;

protected:
	bool m_isSelected = false;

	float3_t m_vPos;
	SMQuaternion m_qRot;
	float3_t m_vScale = float3_t(1.0f, 1.0f, 1.0f);
};


#endif
