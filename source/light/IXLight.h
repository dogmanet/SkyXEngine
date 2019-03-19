#ifndef __IXLIGHT_H
#define __IXLIGHT_H

#include <gdefines.h>
#include <common/math.h>

//! типы источников света
enum LIGHT_TYPE
{
	//! не установленный
	LIGHT_TYPE_NONE = -1,

	//! глобальный
	LIGHT_TYPE_GLOBAL,

	//! точечный
	LIGHT_TYPE_POINT,

	//! направленный
	LIGHT_TYPE_DIR,
	
	//! Непосредственно включаемый в LPV сетку
	LIGHT_TYPE_LPV
};

class IXLight: public IXUnknown
{
public:
	virtual LIGHT_TYPE getType() = 0;

	virtual float3 getColor() = 0;
	virtual void setColor(const float3 &vColor) = 0;

	virtual float3 getPosition() = 0;
	virtual void setPosition(const float3 &vPosition) = 0;

	virtual SMQuaternion getDirection() = 0;
	virtual void setDirection(const SMQuaternion &qDirection) = 0;

	virtual float getDistance() = 0;
	virtual void setDistance(float fDistance) = 0;

	virtual float getShadowIntencity() = 0;
	virtual void setShadowIntencity(float fShadowIntencity) = 0;

	virtual bool isEnabled() = 0;
	virtual void setEnabled(bool isEnabled) = 0;

	virtual bool isShadowDynamic() = 0;
	virtual void setShadowDynamic(bool isDynamic) = 0;
};

class IXLightPoint: public IXLight
{

};


//! Spotlights emit a cone of light that has two parts: a bright inner cone and an outer cone. 
class IXLightSpot: public IXLight
{
public:
	virtual float getInnerAngle() = 0;
	virtual void setInnerAngle(float fAngle) = 0;
	virtual float getOuterAngle() = 0;
	virtual void setOuterAngle(float fAngle) = 0;
};

#endif
