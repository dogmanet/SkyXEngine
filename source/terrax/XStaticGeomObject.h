#ifndef XSTATICGEOMOBJECT_H_
#define XSTATICGEOMOBJECT_H_

#include "XObject.h"
#include <geom/sxgeom.h>

class CXStatixGeomObject: public CXObject
{
	DECLARE_CLASS(CXStatixGeomObject, CXObject);
public:
	CXStatixGeomObject(ID idModel);

	void setPos(const float3_t &pos);

	void getBound(float3 *pvMin, float3 *pvMax);

	void renderSelection(bool is3D);

	bool rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl);

protected:
	ID m_idModel;
};


#endif
