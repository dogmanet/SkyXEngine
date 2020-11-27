#ifndef __IXCSSPROPERTY_H
#define __IXCSSPROPERTY_H

#include <gdefines.h>

XENUM(XCSS_PROPERTY_TYPE,
	XCPT_UNSET = 0,
	XCPT_INHERIT,
	XCPT_INITIAL,
	XCPT_VAR_INT,
	XCPT_VAR_FLOAT,
	XCPT_VAR_STRING,
	XCPT_VAR_MATRIX,
	XCPT_ENUM_VARIANT
);

XENUM(XCSS_VAR_DIM,
	XCSS_VAR_DIM_NONE,
	XCSS_VAR_DIM_PX,
	XCSS_VAR_DIM_EM,
	XCSS_VAR_DIM_MM,
	XCSS_VAR_DIM_PC,
	XCSS_VAR_DIM_PT

	// deg
	// rad
	// grad
	// turn
);


class IXCSSProperty: public IXUnknown
{
public:
	virtual bool isList() const = 0;

	virtual void setInt(int iVal, XCSS_VAR_DIM d = XCSS_VAR_DIM_NONE) = 0;
	virtual void setFloat(float fVal, XCSS_VAR_DIM d = XCSS_VAR_DIM_NONE) = 0;
	virtual void setString(const char *szVal) = 0;
	virtual void setMatrix(const SMMATRIX &mVal) = 0;
	virtual void setVariant(int enumItem) = 0;

	virtual void setFromCSS(const char *szCSSString) = 0;


	virtual int getInt() const = 0;
	virtual float getFloat() const = 0;
	virtual const char* getString() const = 0;
	virtual const SMMATRIX& getMatrix() const = 0;
	virtual float4_t getColor() const = 0;
	virtual int getPX(int iBase) = 0;

	virtual XCSS_PROPERTY_TYPE getType() const = 0; //?
	virtual XCSS_VAR_DIM getDim() const = 0;

	virtual bool isSet() const = 0;

	virtual void unset() = 0;

};

class IXCSSListProperty: public IXCSSProperty
{
public:
	virtual UINT getListSize() const = 0;
	virtual void setListSize(UINT uSize) const = 0;
	virtual UINT getItemCount(UINT uListIndex) const = 0;
	virtual void setItemCount(UINT uSize, UINT uListIndex) const = 0;

	virtual void setInt(int iVal, UINT uListIndex, UINT uItemIndex, XCSS_VAR_DIM d = XCSS_VAR_DIM_NONE) = 0;
	virtual void setFloat(float fVal, UINT uListIndex, UINT uItemIndex, XCSS_VAR_DIM d = XCSS_VAR_DIM_NONE) = 0;
	virtual void setString(const char *szVal, UINT uListIndex, UINT uItemIndex) = 0;
	virtual void setMatrix(const SMMATRIX &mVal, UINT uListIndex, UINT uItemIndex) = 0;
	virtual void setVariant(int enumItem, UINT uListIndex, UINT uItemIndex) = 0;
	
	virtual int getInt(UINT uListIndex, UINT uItemIndex) const = 0;
	virtual float getFloat(UINT uListIndex, UINT uItemIndex) const = 0;
	virtual const char* getString(UINT uListIndex, UINT uItemIndex) const = 0;
	virtual const SMMATRIX& getMatrix(UINT uListIndex, UINT uItemIndex) const = 0;
	virtual float4_t getColor(UINT uListIndex, UINT uItemIndex) const = 0;
	virtual int getPX(int iBase, UINT uListIndex, UINT uItemIndex) = 0;

	virtual XCSS_PROPERTY_TYPE getType(UINT uListIndex, UINT uItemIndex) const = 0; //?
	virtual XCSS_VAR_DIM getDim(UINT uListIndex, UINT uItemIndex) const = 0;
};

#endif
