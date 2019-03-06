#ifndef __IXMATERIAL_H
#define __IXMATERIAL_H

#include <gdefines.h>
#include "IXTexture.h"

class IXMaterial: public IXUnknown
{
public:
	virtual void getMainTexture(IXTexture **ppTexture) = 0;

	virtual bool isTransparent() = 0;


	//@FIXME: DO NOT USE! Will be removed!
	virtual ID getInternalID() = 0;
};

#endif
