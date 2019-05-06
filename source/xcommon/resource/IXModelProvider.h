#ifndef __IXRENDERABLE_H
#define __IXRENDERABLE_H

#include <gdefines.h>



class IXStaticModel;
class IXDynamicModel;
class IXAnimatedModel;
class IXModel: public IXUnknown
{
public:
	virtual IXAnimatedModel *asAnimatedModel() = 0;
	virtual IXDynamicModel *asDynamicModel() = 0;
	virtual IXStaticModel *asStaticModel() = 0;


};

class IXStaticModel: public IXModel
{
public:
};

class IXDynamicModel: public IXModel
{
public:
};

class IXAnimatedModel: public IXDynamicModel
{
public:
};



class IXStaticModelProvider: public IXUnknown
{
public:
};
class IXDynamicModelProvider: public IXUnknown
{
public:
};
class IXAnimatedModelProvider: public IXUnknown
{
public:
};

class IXModelProvider: public IXUnknown
{
public:
	virtual bool loadStaticModel(const char *szName, IXStaticModel **ppStaticModel) = 0;
	virtual bool loadDynamicModel(const char *szName, IXDynamicModel **ppStaticModel) = 0;
};


#endif
