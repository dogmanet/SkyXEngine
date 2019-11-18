#ifndef __IXMATERIAL_H
#define __IXMATERIAL_H

#include <gdefines.h>
#include "IXTexture.h"

class IXMaterial: public IXUnknown
{
public:
	virtual void XMETHODCALLTYPE getMainTexture(IXTexture **ppTexture) = 0;

	virtual bool XMETHODCALLTYPE isTransparent() = 0;
	virtual bool XMETHODCALLTYPE isRefractive() = 0;
	virtual bool XMETHODCALLTYPE isBlurred() = 0;

	virtual void XMETHODCALLTYPE setFlag(const char *szFlag, bool isSet) = 0;
	virtual bool XMETHODCALLTYPE getFlag(const char *szFlag) = 0;

	virtual void XMETHODCALLTYPE setParam(const char *szFlag, float fValue) = 0;
	virtual float XMETHODCALLTYPE getParam(const char *szFlag) = 0;

	//@FIXME: DO NOT USE! Will be removed!
	virtual ID getInternalID() = 0;
};

#endif
