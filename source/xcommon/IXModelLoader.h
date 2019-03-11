#ifndef __IXMODELLOADER_H
#define __IXMODELLOADER_H

#include <gdefines.h>
#include <core/IFile.h>

class IXStaticModel
{
};

class IXAnimatedModel
{
};

// {8E7DA1D6-7965-41A1-BFB4-07B087E854A1}
#define XMODELLOADER_GUID DEFINE_XGUID(0x8e7da1d6, 0x7965, 0x41a1, 0xbf, 0xb4, 0x7, 0xb0, 0x87, 0xe8, 0x54, 0xa1)

class IXModelLoader: public IXUnknown
{
public:
	bool open(IFile *pFile);
	bool loadAsStatic(IXStaticModel *pModel);
	bool loadAsAnimated(IXAnimatedModel *pModel);
	void close();
};

#endif
