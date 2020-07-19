#ifndef __DDSFILE_H
#define __DDSFILE_H

#include <gdefines.h>

#pragma pack(push, 1)
struct TGA_Header
{
	int8_t idlength;
	int8_t colormaptype;
	int8_t datatypecode;
	int16_t colormaporigin;
	int16_t colormaplength;
	int8_t colormapdepth;
	int16_t x_origin;
	int16_t y_origin;
	int16_t width;
	int16_t height;
	int8_t bitsperpixel;
	int8_t imagedescriptor;
};
#pragma pack(pop)

static_assert(sizeof(TGA_Header) == 18, "TGA Header size mismatch");


#endif
