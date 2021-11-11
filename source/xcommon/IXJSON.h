#ifndef __IXJSON_H
#define __IXJSON_H

#include <gdefines.h>

enum XJSON_ITEM_TYPE
{
	XJI_NULL,
	XJI_BOOL,
	XJI_STRING,
	XJI_NUMBER,
	XJI_OBJECT,
	XJI_ARRAY
};

class IXJSONArray;
class IXJSONObject;
class IXJSONItem: public IXUnknown
{
public:
	virtual XJSON_ITEM_TYPE XMETHODCALLTYPE getType() const = 0;
	
	virtual IXJSONArray* XMETHODCALLTYPE asArray() = 0;
	virtual const IXJSONArray* XMETHODCALLTYPE asArray() const = 0;
	
	virtual IXJSONObject* XMETHODCALLTYPE asObject() = 0;
	virtual const IXJSONObject* XMETHODCALLTYPE asObject() const = 0;
	
	virtual bool XMETHODCALLTYPE isNull() const = 0;
	virtual bool XMETHODCALLTYPE asBool() const = 0;
	virtual bool XMETHODCALLTYPE getFloat(float *pOut) const = 0;
	//virtual bool XMETHODCALLTYPE getInt(int *pOut) const = 0;
	virtual bool XMETHODCALLTYPE getInt64(int64_t *pOut) const = 0;
	virtual const char* XMETHODCALLTYPE getString() const = 0;
};

class IXJSONArray: public IXJSONItem
{
public:
	virtual UINT XMETHODCALLTYPE size() const = 0;
	virtual IXJSONItem* XMETHODCALLTYPE at(UINT idx) const = 0;
};

class IXJSONObject: public IXJSONItem
{
public:
	virtual UINT XMETHODCALLTYPE size() const = 0;
	virtual IXJSONItem* XMETHODCALLTYPE at(UINT idx) const = 0;
	
	virtual const char* XMETHODCALLTYPE getKey(UINT idx) const = 0;
	
	virtual IXJSONItem* XMETHODCALLTYPE getItem(const char *szKey) const = 0;
	
};


// {28C5EDF7-51CD-4F4F-A374-FAE4650B4037}
#define IXJSON_GUID DEFINE_XGUID(0x28c5edf7, 0x51cd, 0x4f4f, 0xa3, 0x74, 0xfa, 0xe4, 0x65, 0xb, 0x40, 0x37)

class IXJSON: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE parse(const char *szString, IXJSONItem **ppOut, void *pReserved=NULL) const = 0;
};

#endif
