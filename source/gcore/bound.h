
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __BOUND_H
#define __BOUND_H

#include <common/Math.h>
#include "sxgcore.h"

void CreateCone(float fTopRadius, float fBottomRadius, float fHeight, IMesh ** ppMesh, IGXDevice * pDevice, UINT iSideCount);
void CreateSphere(float fRadius, UINT iSideCount, UINT iStackCount, IMesh ** ppMesh, IGXDevice * pDevice);

void CreateBoundingBoxMesh(const float3* min, const float3* max, IMesh** bbmesh, IGXDevice* device);

#endif
