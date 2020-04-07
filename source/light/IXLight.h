#ifndef __IXLIGHT_H
#define __IXLIGHT_H

#include <gdefines.h>
#include <common/math.h>
#include <graphix/graphix.h>

//! типы источников света
enum LIGHT_TYPE
{
	//! не установленный
	LIGHT_TYPE_NONE = -1,

	//! глобальный
	LIGHT_TYPE_SUN,

	//! точечный
	LIGHT_TYPE_POINT,

	//! направленный
	LIGHT_TYPE_SPOT,
	
	//! Непосредственно включаемый в LPV сетку
	LIGHT_TYPE_LPV,


	LIGHT_TYPE__COUNT
};

/*! базовое направление направленноого источника света */
#define LIGHTS_DIR_BASE float3(0, -1, 0)

class IXRenderableVisibility;


enum LIGHT_RENDER_TYPE
{
	LRT_NONE = 0, //!< Не рендерится
	LRT_LIGHT = 1, //!< Только освещение, без LPV 
	LRT_LPV = 2, //!< Рендер для LPV в 32x32 
	LRT_FULL = 3, //!< Полноразмерный рендер 

	LRT_ALL = LRT_FULL
};
DEFINE_ENUM_FLAG_OPERATORS(LIGHT_RENDER_TYPE);


class IXLightPoint;
class IXLightSun;
class IXLightSpot;
class IXLight: public IXUnknown
{
public:
	virtual LIGHT_TYPE getType() = 0;

	virtual float3 getColor() = 0;
	virtual void setColor(const float3 &vColor) = 0;

	virtual float3 getPosition() = 0;
	virtual void setPosition(const float3 &vPosition) = 0;

	virtual float getShadowIntencity() = 0;
	virtual void setShadowIntencity(float fShadowIntencity) = 0;

	virtual bool isEnabled() = 0;
	virtual void setEnabled(bool isEnabled) = 0;

	virtual bool isShadowDynamic() = 0;
	virtual void setShadowDynamic(bool isDynamic) = 0;

	//@TODO: Remove this method
	virtual void drawShape(IGXDevice *pDevice) = 0;
	//@TODO: Remove this method
	virtual IGXConstantBuffer* getConstants(IGXDevice *pDevice) = 0;
	//@TODO: Remove this method
	virtual LIGHT_RENDER_TYPE getRenderType() = 0;
	//@TODO: Remove this method
	virtual bool isDirty(LIGHT_RENDER_TYPE type) = 0;
	//@TODO: Remove this method
	virtual void markClean(LIGHT_RENDER_TYPE type) = 0;
	//@TODO: Remove this method
	virtual void testDirty() = 0;

	virtual IXLightSpot *asSpot() = 0;
	virtual IXLightSun *asSun() = 0;
	virtual IXLightPoint *asPoint() = 0;

	virtual float getMaxDistance() = 0;

	virtual IXRenderableVisibility* getVisibility() = 0;
};

class IXLightPoint: public virtual IXLight
{

};

class IXLightSun: public virtual IXLight
{
public:
	virtual SMQuaternion getDirection() = 0;
	virtual void setDirection(const SMQuaternion &qDirection) = 0;
};


//! Spotlights emit a cone of light that has two parts: a bright inner cone and an outer cone. 
class IXLightSpot: public virtual IXLight
{
public:
	virtual float getInnerAngle() = 0;
	virtual void setInnerAngle(float fAngle) = 0;
	virtual float getOuterAngle() = 0;
	virtual void setOuterAngle(float fAngle) = 0;
	virtual SMQuaternion getDirection() = 0;
	virtual void setDirection(const SMQuaternion &qDirection) = 0;
};

#endif
