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

	virtual SMQuaternion getOrient();
	virtual void setOrient(const SMQuaternion &orient);

	virtual float3_t getScale();
	virtual void setScale(const float3_t &pos);

	virtual void getBound(float3 *pvMin, float3 *pvMax) = 0;

	bool isSelected();
	void setSelected(bool set);

	virtual void renderSelection(bool is3D) = 0;

	virtual bool rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, ID *pidMtrl) = 0;

	virtual void remove() = 0;
	virtual void preCreate() = 0; // вызывается при начале создания объекта до установки свойств
	virtual void postCreate() = 0; // вызывается после установки всех необходимых свойств, завершает создание объекта

	virtual void setKV(const char *szKey, const char *szValue) = 0;
	virtual const char *getKV(const char *szKey) = 0;
	virtual const char *getPropertyKey(UINT uKey) = 0;
	virtual UINT getProperyCount() = 0;


protected:
	bool m_isSelected = false;

	float3_t m_vPos;
};


#endif
