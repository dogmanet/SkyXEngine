#ifndef XOBJECT_H_
#define XOBJECT_H_

#include <gdefines.h>
#include <common/math.h>

class CXObject
{
public:
	virtual ~CXObject()
	{
	}

	virtual float3_t getPos();
	virtual void setPos(const float3_t &pos);

	bool isSelected();
	void setSelected(bool set);

	virtual void renderSelection(bool is3D) = 0;

protected:
	bool m_isSelected = false;

	float3_t m_vPos;
};


#endif
