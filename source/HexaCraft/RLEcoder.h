#ifndef _RLEcoder_H_
#define _RLEcoder_H_

typedef unsigned char byte;
typedef unsigned int UINT;

class RLEcoder
{
	static UINT ComputeCompressedLength(byte * pData, UINT iLen);
	static UINT ComputeDecompressedLength(byte * pData, UINT iLen);
public:
	static void Pack(byte * pData, UINT iLen, byte ** pOut, UINT * pOutLen);
	static void UnPack(byte * pData, UINT iLen, byte ** pOut, UINT * pOutLen);
};

#endif