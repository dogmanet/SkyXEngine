#ifndef _DScoder_H_
#define _DScoder_H_

#include "HuffmanCoder.h"
#include "RLEcoder.h"

class DScoder
{
public:
	static void Pack(byte * pData, UINT iLen, byte ** pOut, UINT * pOutLen);
	static void UnPack(byte * pData, UINT iLen, byte ** pOut, UINT * pOutLen);
};

#endif
