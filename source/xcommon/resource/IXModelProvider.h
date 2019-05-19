#ifndef __IXMODELPROVIDER_H
#define __IXMODELPROVIDER_H

#include <gdefines.h>
#include "IXModel.h"
#include "IXResourceModel.h"


// {CA8A6553-8455-4C1C-A615-3F237AF26F16}
#define IXSTATICMODELPROVIDER_GUID DEFINE_XGUID(0xca8a6553, 0x8455, 0x4c1c, 0xa6, 0x15, 0x3f, 0x23, 0x7a, 0xf2, 0x6f, 0x16)

// {A5321165-79C7-4268-AC0B-12BAC57E3540}
#define IXDYNAMICMODELPROVIDER_GUID DEFINE_XGUID(0xa5321165, 0x79c7, 0x4268, 0xac, 0xb, 0x12, 0xba, 0xc5, 0x7e, 0x35, 0x40)

// {388012AF-B146-47E1-B45C-CA4031654408}
#define IXANIMATEDMODELPROVIDER_GUID DEFINE_XGUID(0x388012af, 0xb146, 0x47e1, 0xb4, 0x5c, 0xca, 0x40, 0x31, 0x65, 0x44, 0x8)

// {A58C6BF0-E954-42F4-B8EC-C510A3FCB597}
#define IXMODELPROVIDER_GUID DEFINE_XGUID(0xa58c6bf0, 0xe954, 0x42f4, 0xb8, 0xec, 0xc5, 0x10, 0xa3, 0xfc, 0xb5, 0x97)


// Implemented in geom plugin
class IXStaticModelProvider: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE createModel(const IXResourceModelStatic *pResource, IXStaticModel **ppModel) = 0;
};

// - Can IXDynamicModelProvider and IXAnimatedModelProvider be implemented in different places?
// - Why not?

// Implemented in anim plugin
class IXDynamicModelProvider: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE createModel(const IXResourceModel *pResource, IXDynamicModel **ppModel) = 0;
};
// Implemented in anim plugin
class IXAnimatedModelProvider: public IXUnknown
{
public:
	// build IXAnimatedModel from one or more IXResourceModelAnimated
	virtual bool XMETHODCALLTYPE createModel(UINT uResourceCount, const IXResourceModelAnimated **ppResources, IXAnimatedModel **ppModel) = 0;
};

// Implemented in core
class IXModelProvider: public IXUnknown
{
public:
	// static only
	virtual bool XMETHODCALLTYPE createStaticModel(const IXResourceModel *pResource, IXStaticModel **ppModel) = 0;

	// can be dynamic or animated
	virtual bool XMETHODCALLTYPE createDynamicModel(const IXResourceModel *pResource, IXDynamicModel **ppModel) = 0;
};


#endif
