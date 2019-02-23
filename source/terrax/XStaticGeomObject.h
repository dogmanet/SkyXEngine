#ifndef XSTATICGEOMOBJECT_H_
#define XSTATICGEOMOBJECT_H_

#include "XObject.h"
#include <geom/sxgeom.h>
#include <common/array.h>
#include <common/string.h>

class CXStatixGeomObject: public CXObject
{
	DECLARE_CLASS(CXStatixGeomObject, CXObject);
public:
	CXStatixGeomObject();
	CXStatixGeomObject(ID idModel);
	~CXStatixGeomObject();

	void setPos(const float3_t &pos);

	SMQuaternion getOrient();
	void setOrient(const SMQuaternion &orient);

	float3_t getScale();
	void setScale(const float3_t &pos);

	void getBound(float3 *pvMin, float3 *pvMax);

	void renderSelection(bool is3D);

	bool rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl);

	void remove();
	void preCreate();
	void postCreate();

	void setKV(const char *szKey, const char *szValue);
	const char *getKV(const char *szKey);
	const char *getPropertyKey(UINT uKey);
	UINT getProperyCount();

protected:
	ID m_idModel = -1;
	String m_sModelName;
	String m_sName;
	bool m_bSegmentation = false;
	char m_szSegmentation[2];

	static Array<CXStatixGeomObject*> ms_aObjects;
};


#endif
