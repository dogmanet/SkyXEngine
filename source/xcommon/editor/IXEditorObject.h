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
	bool isGeneric;
};

class IXTexture;
class IXGizmoRenderer;
class IXEditorObject: public IXUnknown
{
public:
	virtual float3_t XMETHODCALLTYPE getPos() = 0;
	virtual void XMETHODCALLTYPE setPos(const float3_t &pos) = 0;

	virtual SMQuaternion XMETHODCALLTYPE getOrient() = 0;
	virtual void XMETHODCALLTYPE setOrient(const SMQuaternion &orient) = 0;

	//virtual float3_t XMETHODCALLTYPE getScale() = 0;
	virtual void XMETHODCALLTYPE setSize(const float3_t &vSize) = 0;

	virtual void XMETHODCALLTYPE getBound(float3 *pvMin, float3 *pvMax) = 0;

	virtual bool XMETHODCALLTYPE isSelected() = 0;
	virtual void XMETHODCALLTYPE setSelected(bool set) = 0;

	virtual void XMETHODCALLTYPE renderSelection(bool is3D, IXGizmoRenderer *pGizmoRenderer) = 0;

	virtual bool XMETHODCALLTYPE rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut = NULL, float3 *pvNormal = NULL, ID *pidMtrl = NULL, bool bReturnNearestPoint = false) = 0;

	virtual void XMETHODCALLTYPE remove() = 0;
	virtual void XMETHODCALLTYPE create() = 0;
	virtual void XMETHODCALLTYPE preSetup() = 0;
	virtual void XMETHODCALLTYPE postSetup() = 0;

	virtual void XMETHODCALLTYPE setKV(const char *szKey, const char *szValue) = 0;
	virtual const char* XMETHODCALLTYPE getKV(const char *szKey) = 0;
	virtual const X_PROP_FIELD* XMETHODCALLTYPE getPropertyMeta(UINT uKey) = 0;
	virtual UINT XMETHODCALLTYPE getProperyCount() = 0;

	virtual const char* XMETHODCALLTYPE getTypeName() = 0;
	virtual const char* XMETHODCALLTYPE getClassName() = 0;

	virtual IXTexture* XMETHODCALLTYPE getIcon() = 0;

	virtual void XMETHODCALLTYPE setSimulationMode(bool set) = 0;

	virtual bool XMETHODCALLTYPE hasVisualModel() = 0;
};


#endif
