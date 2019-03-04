#ifndef __IXTEXTURE_H
#define __IXTEXTURE_H

#include <gdefines.h>
#include <graphix/graphix.h>

class IXTexture: public IXUnknown
{
public:
	virtual void getAPITexture(IGXBaseTexture **ppTexture) = 0;
};

#endif
