#ifndef __IXMODELPROVIDER_H
#define __IXMODELPROVIDER_H

#include <gdefines.h>
#include "IXModel.h"
#include "IXResourceModel.h"

// Implemented in geom plugin
class IXStaticModelProvider: public IXUnknown
{
public:
};

// - Can IXDynamicModelProvider and IXAnimatedModelProvider be implemented in different places?
// - Why not?

// Implemented in anim plugin
class IXDynamicModelProvider: public IXUnknown
{
public:
};
// Implemented in anim plugin
class IXAnimatedModelProvider: public IXUnknown
{
public:
	// build IXAnimatedModel from one or more IXResourceModelAnimated
};

// Implemented in core
class IXModelProvider: public IXUnknown
{
public:
	// static only
	virtual bool XMETHODCALLTYPE createStaticModel(IXResourceModel *pResource, IXStaticModel **ppModel) = 0;

	// can be dynamic or animated
	virtual bool XMETHODCALLTYPE createDynamicModel(IXResourceModel *pResource, IXDynamicModel **ppModel) = 0;
};


#endif
