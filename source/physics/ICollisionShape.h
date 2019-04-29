#ifndef __ICOLLISIONSHAPE_H
#define __ICOLLISIONSHAPE_H

#include <gdefines.h>

class ICollisionShape: public IXUnknown
{
public:
	
};

class IConcaveShape: public ICollisionShape
{
public:

};

class IConvexShape: public ICollisionShape
{
public:

};




class IBoxShape: public IConvexShape
{
public:

};

class ISphereShape: public IConvexShape
{
public:

};

class ICapsuleShape: public IConvexShape
{
public:

};

class IPlaneShape: public IConcaveShape
{
public:

};

class IHeightfieldShape: public IConcaveShape
{
public:

};

class IShapeConvex: public ICollisionShape
{
public:

};

class ITrimeshShape: public IConcaveShape
{
public:

};

#endif
