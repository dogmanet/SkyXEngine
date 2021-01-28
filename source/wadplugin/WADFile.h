#ifndef __WADFILE_H
#define __WADFILE_H

#include <gdefines.h>

#pragma pack(push, 1)
struct WADHeader
{
	uint32_t uMagick;
	uint32_t uTextures;
	uint32_t uLumpsOffset;
};

struct WADLump
{
	uint32_t uTextureOffset;
	uint32_t uCompressedLength;
	uint32_t uFullLength;
	uint8_t u8Type;
	uint8_t u8CompressionType;
	uint16_t __padding;
	char szName[16];
};

// regular texture
struct WADTexture43
{
	char szName[16];
	uint32_t uWidth;
	uint32_t uHeight;
	uint32_t uImageOffset;
	uint32_t uMip1Offset;
	uint32_t uMip2Offset;
	uint32_t uMip3Offset;
	// indexed data
};

#pragma pack(pop)

static_assert(sizeof(WADHeader) == 12, "WAD Header size mismatch");


#endif
