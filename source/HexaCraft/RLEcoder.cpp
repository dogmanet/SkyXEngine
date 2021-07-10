#include "RLEcoder.h"

UINT RLEcoder::ComputeCompressedLength(byte * pData, UINT iLen)
{
	UINT res = 0;
	byte bCur = pData[0];
	UINT c = 1;
	UINT ws = 1;
	for(UINT i = 1; i < iLen; i++)
	{
		if(pData[i] == bCur && c < 129)
		{
			if(ws >= 2)
			{
				res += ws;
			}
			c++;
			ws = 0;
		}
		else
		{
			if(c >= 2)
			{
				res += 2;
			}
			c = 1;
			bCur = pData[i];
			ws++;
		}
	}
	if(c >= 1)
	{
		res += 2;
	}
	if(ws >= 2)
	{
		res += ws;
	}
	return(res);
}

void RLEcoder::Pack(byte * pData, UINT iLen, byte ** pOut, UINT * pOutLen)
{
	*pOutLen = ComputeCompressedLength(pData, iLen);//18
	*pOut = new byte[*pOutLen];
	byte * res = *pOut;

	byte bCur = pData[0];
	UINT c = 1;
	UINT ws = 1;
	UINT ptr = 0;
	UINT i;
	for(i = 1; i < iLen; i++)
	{
		if(pData[i] == bCur && c < 129)
		{
			if(ws >= 2)
			{
				res[ptr] = (ws - 2) & 127;
				ptr++;
				for(int j = i - ws; j < i - 1; j++)
				{
					res[ptr] = pData[j];
					ptr++;
				}
				//res += ws;
			}
			c++;
			ws = 0;
		}
		else
		{
			if(c >= 2)
			{
				res[ptr] = (c - 1) | 128;
				ptr++;
				res[ptr] = bCur;
				ptr++;
				//res += 2;
			}
			c = 1;
			bCur = pData[i];
			ws++;
		}
	}
	if(c >= 1)
	{
		res[ptr] = (c - 1) | 128;
		ptr++;
		res[ptr] = bCur;
		ptr++;
		//res += 2;
	}
	if(ws >= 2)
	{
		res[ptr] = (ws - 2) & 127;
		ptr++;
		for(int j = i - ws + 1; j < i; j++)
		{
			res[ptr] = pData[j];
			ptr++;
		}
		//res += ws;
	}
	
}

UINT RLEcoder::ComputeDecompressedLength(byte * pData, UINT iLen)
{
	UINT ptr = 0;
	UINT i;
	byte bCur;
	UINT c = 0, ws = 0;
	for(i = 0; i < iLen; i++)
	{
		if(c)
		{
			for(int k = 0; k < c; k++)
			{
				ptr++;
			}
			c = 0;
			continue;
		}
		if(ws)
		{
			ptr++;
			ws--;
			continue;
		}
		if(pData[i] & 128)
		{
			c = (pData[i] & 127) + 1;
			ws = 0;
		}
		else
		{
			ws = (pData[i] & 127) + 1;
			c = 0;
		}
	}
	return(ptr);
}

void RLEcoder::UnPack(byte * pData, UINT iLen, byte ** pOut, UINT * pOutLen)
{
	*pOutLen = ComputeDecompressedLength(pData, iLen);//18
	*pOut = new byte[*pOutLen];
	byte * res = *pOut;

	UINT ptr = 0;
	UINT i;
	byte bCur;
	UINT c = 0, ws = 0;
	for(i = 0; i < iLen; i++)
	{
		if(c)
		{
			for(int k = 0; k < c; k++)
			{
				res[ptr] = pData[i];
				ptr++;
			}
			c = 0;
			continue;
		}
		if(ws)
		{
			res[ptr] = pData[i];
			ptr++;
			ws--;
			continue;
		}
		if(pData[i] & 128)
		{
			c = (pData[i] & 127) + 1;
			ws = 0;
		}
		else
		{
			ws = (pData[i] & 127) + 1;
			c = 0;
		}
	}
}