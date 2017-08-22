
#ifndef __GEOM_OPTIMIZE_H
#define __GEOM_OPTIMIZE_H

#include <gdefines.h>
#include <d3dx9.h>

void OptimizeIndecesInSubsetUint16(uint16_t* ib, uint16_t numFaces, uint16_t numVerts);
void OptimizeIndecesInSubsetUint32(uint32_t* ib, uint32_t numFaces, uint32_t numVerts);

#endif