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

protected:
	ID m_idModel;
};


#endif
