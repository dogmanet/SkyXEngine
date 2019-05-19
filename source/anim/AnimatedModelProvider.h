#ifndef __ANIMATEDMODELPROVIDER_H
#define __ANIMATEDMODELPROVIDER_H

#include <xcommon/resource/IXModelProvider.h>

class CAnimatedModelProvider: public IXAnimatedModelProvider
{
public:
	bool XMETHODCALLTYPE createModel(UINT uResourceCount, const IXResourceModelAnimated **ppResources, IXAnimatedModel **ppModel) override;
protected:

};

#endif
