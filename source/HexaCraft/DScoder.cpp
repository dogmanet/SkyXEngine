#include <gdefines.h>

#include "DScoder.h"



void DScoder::Pack(byte * pData, UINT iLen, byte ** pOut, UINT * pOutLen)
{
	UINT len1, len2;
	byte * pb1, * pb2;
	HuffmanCoder::Pack(pData, iLen, &pb1, &len1);
	//RLEcoder::Pack(pb1, len1, &pb2, &len2);
	//DS_SAFE_DELETE_A(pb1);
	pb2 = pb1;
	len2 = len1;
	HuffmanCoder::Pack(pb2, len2, &pb1, &len1);
	mem_delete_a(pb2);
	*pOut = new byte[len1];
	memcpy(*pOut, pb1, len1);
	mem_delete_a(pb1);
	*pOutLen = len1;
}

void DScoder::UnPack(byte * pData, UINT iLen, byte ** pOut, UINT * pOutLen)
{
	UINT len1, len2;
	byte * pb1 = NULL, *pb2 = NULL;
	*pOut = NULL;
	
	HuffmanCoder::UnPack(pData, iLen, &pb1, &len1);
	//RLEcoder::UnPack(pb1, len1, &pb2, &len2);
	//DS_SAFE_DELETE_A(pb1);
	pb2 = pb1;
	len2 = len1;
	HuffmanCoder::UnPack(pb2, len2, &pb1, &len1);
	mem_delete_a(pb2);
	*pOut = new byte[len1];
	memcpy(*pOut, pb1, len1);
	mem_delete_a(pb1);
	*pOutLen = len1;
}
