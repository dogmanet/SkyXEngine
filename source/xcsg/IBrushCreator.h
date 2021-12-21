#ifndef __IBRUSHCREATOR_H
#define __IBRUSHCREATOR_H

#include <gdefines.h>

class IBrushCreator: public IXUnknown
{
public:
	virtual bool isInitiated() = 0;
	virtual void init(const float3 &vNormal) = 0;

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual bool onMouseDown(bool isPrimary) = 0;
	virtual bool onMouseMove() = 0;
	virtual void onMouseUp(bool isPrimary) = 0;

	virtual bool onKeyDown(UINT uKey) = 0;
	virtual void onKeyUp(UINT uKey) = 0;

	virtual void render(bool is3D) = 0;

	virtual UINT getClassCount() = 0;
	virtual const char* getClassAt(UINT idx) = 0;
	virtual bool useClass(const char *szClassName) = 0;
};

#endif
