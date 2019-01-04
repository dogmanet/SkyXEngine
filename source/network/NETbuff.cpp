#include "NETbuff.h"

CNETbuff::CNETbuff():
	m_uBufPos(0),
	m_uPosBit(0),
	m_uReadBitNum(0)
{
}

void CNETbuff::movR(int i)
{
	m_uBufPos += i;

	if(m_uBufPos >= m_aBuf.size())
	{
		m_aBuf.resize(m_uBufPos + 1);
	}
}
void CNETbuff::movL(int i)
{
	m_uBufPos -= i;
}
void CNETbuff::movE()
{
	m_uBufPos = m_aBuf.size();
}
void CNETbuff::movB()
{
	m_uBufPos = 0;
}
BOOL CNETbuff::isEnd()
{
	return(m_uBufPos >= m_aBuf.size());
}
BOOL CNETbuff::isEmpty()
{
	return(m_aBuf.size() == 0);
}
void CNETbuff::reset()
{
	m_aBuf.clearFast();
	m_uBufPos = 0;
	m_uReadBitNum = 0;
	m_uPosBit = 0;
}

void CNETbuff::setCursor(UINT i)
{
	m_uBufPos = i;
}
void CNETbuff::setBuf(const BYTE * data, UINT size)
{
	m_uBufPos = 0;
	//m_aBuf.reserve(size);
	if(size != 0)
	{
		m_aBuf[size - 1] = 0;
		memcpy(&(m_aBuf[0]), data, size);
	}
	else
	{
		m_aBuf.clearFast();
	}
}
void CNETbuff::addBuf(const BYTE * data, UINT size)
{
	UINT spos = m_aBuf.size();
	//m_aBuf.reserve(spos + size);
	m_aBuf[spos + size - 1] = 0;
	memcpy(&(m_aBuf[spos]), data, size);
}
void CNETbuff::getBuf(BYTE * data, UINT b, UINT l)
{
	UINT size = l;
	if(size > m_aBuf.size() - b)
	{
		size = m_aBuf.size() - b;
	}
	memcpy(data, &(m_aBuf[b]), size);
}


void CNETbuff::readString(char *pBuf, UINT iBufSize)
{
	if(iBufSize > getSize() - m_uBufPos)
	{
		iBufSize = getSize() - m_uBufPos;
	}

	strncpy(pBuf, (const char*)&(m_aBuf[m_uBufPos]), iBufSize);
	pBuf[iBufSize] = 0;
	
	while(!isEnd() && readInt8())
	{
	}
}
UINT CNETbuff::readChar()
{
	UINT ch = 0;
	BYTE byte1 = readInt8();
	if(byte1 < 0x80)
	{
		ch = byte1;
	}
	else if(byte1 >= 0xC2 && byte1 < 0xE0)
	{
		BYTE byte2 = readInt8();
		ch = (((byte1 & 0x1F) << 6) + (byte2 & 0x3F));
	}
	else if(byte1 >= 0xE0 && byte1 < 0xF0)
	{
		BYTE byte2 = readInt8();
		BYTE byte3 = readInt8();
		ch = (((byte1 & 0xFF) << 12) + ((byte2 & 0x3F) << 6) + (byte3 & 0x3F));
	}
	else if(byte1 >= 0xF0 && byte1 < 0xF5)
	{
		BYTE byte2 = readInt8();
		BYTE byte3 = readInt8();
		BYTE byte4 = readInt8();
		UINT codepoint = ((byte1 & 0x07) << 18) + ((byte2 & 0x3F) << 12) + ((byte3 & 0x3F) << 6) + (byte4 & 0x3F);
		codepoint -= 0x10000;
		ch = (codepoint >> 10) + 0xD800, (codepoint & 0x3FF) + 0xDC00;
	}
	return(ch);
}
void CNETbuff::readBytes(BYTE *pBuf, UINT iBufSize)
{
	if(iBufSize > getSize() - m_uBufPos)
	{
		iBufSize = getSize() - m_uBufPos;
	}

	memcpy(pBuf, &(m_aBuf[m_uBufPos]), iBufSize);
}
int32_t CNETbuff::readInt32()
{
	int32_t i;
	UINT len = m_aBuf.size() - m_uBufPos;
	if(len > 4)
	{
		len = 4;
	}
	memcpy(&i, &(m_aBuf[m_uBufPos]), len);
	m_uBufPos += len;
	return(i);
}
uint32_t CNETbuff::readUInt32()
{
	uint32_t i;
	UINT len = m_aBuf.size() - m_uBufPos;
	if(len > 4)
	{
		len = 4;
	}
	memcpy(&i, &(m_aBuf[m_uBufPos]), len);
	m_uBufPos += len;
	return(i);
}
int64_t CNETbuff::readInt64()
{
	int64_t i;
	UINT len = m_aBuf.size() - m_uBufPos;
	if(len > 8)
	{
		len = 8;
	}
	memcpy(&i, &(m_aBuf[m_uBufPos]), len);
	m_uBufPos += len;
	return(i);
}
uint64_t CNETbuff::readUInt64()
{
	uint64_t i;
	UINT len = m_aBuf.size() - m_uBufPos;
	if(len > 8)
	{
		len = 8;
	}
	memcpy(&i, &(m_aBuf[m_uBufPos]), len);
	m_uBufPos += len;
	return(i);
}
int16_t CNETbuff::readInt16()
{
	int16_t i;
	UINT len = m_aBuf.size() - m_uBufPos;
	if(len > 2)
	{
		len = 2;
	}
	memcpy(&i, &(m_aBuf[m_uBufPos]), len);
	m_uBufPos += len;
	return(i);
}
uint16_t CNETbuff::readUInt16()
{
	uint16_t i;
	UINT len = m_aBuf.size() - m_uBufPos;
	if(len > 2)
	{
		len = 2;
	}
	memcpy(&i, &(m_aBuf[m_uBufPos]), len);
	m_uBufPos += len;
	return(i);
}
int8_t CNETbuff::readInt8()
{
	int8_t i;
	UINT len = m_aBuf.size() - m_uBufPos;
	if(len > 1)
	{
		len = 1;
	}
	memcpy(&i, &(m_aBuf[m_uBufPos]), len);
	m_uBufPos += len;
	return(i);
}
uint8_t CNETbuff::readUInt8()
{
	uint8_t i;
	UINT len = m_aBuf.size() - m_uBufPos;
	if(len > 1)
	{
		len = 1;
	}
	memcpy(&i, &(m_aBuf[m_uBufPos]), len);
	m_uBufPos += len;
	return(i);
}
float CNETbuff::readFloat32()
{
	float32_t i;
	UINT len = m_aBuf.size() - m_uBufPos;
	if(len > 4)
	{
		len = 4;
	}
	memcpy(&i, &(m_aBuf[m_uBufPos]), len);
	m_uBufPos += len;
	return(i);
}


//UINT CNETbuff::readBits(UINT count)
//{//max 32 bit
//
//	if(count > 32)
//	{
//		return(0);
//	}
//
//	if(count > 24)
//	{
//		return((readBits_sys(24) | (readBits_sys(count - 24) << 24)) & ((1 << count) - 1));
//	}
//	return(readBits_sys(count) & ((1 << count) - 1));
//
//}
//int CNETbuff::readSBits(BYTE numbits)
//{
//	int nSignBit = readOneBitBool();
//	int result = readBits(numbits - 1);
//	if(nSignBit)
//	{
//		result = -result;
//	}
//
//	return(result);
//}
//BOOL CNETbuff::readOneBitBool()
//{
//	return(readBits(1));
//}
//void CNETbuff::readBitsBegin()
//{
//	m_uPosBit = 0;
//}
//void CNETbuff::readBitsEnd()
//{
//	if(m_uPosBit > 0)
//	{
//		m_uPosBit = 0;
//		movR(1);
//	}
//}
//void CNETbuff::readBitsRelease()
//{
//	if(m_uPosBit > 0)
//	{
//		m_uPosBit = 0;
//		movR(1);
//	}
//}
//WCHAR CNETbuff::readBitChar()
//{
//	UINT ch = L'';
//	BYTE byte1 = readBits(8);
//	if(byte1 < 0x80)
//	{
//		ch = byte1;
//	}
//	else if(byte1 >= 0xC2 && byte1 < 0xE0)
//	{
//		BYTE byte2 = readBits(8);
//		ch = (((byte1 & 0x1F) << 6) + (byte2 & 0x3F));
//	}
//	else if(byte1 >= 0xE0 && byte1 < 0xF0)
//	{
//		BYTE byte2 = readBits(8);
//		BYTE byte3 = readBits(8);
//		ch = (((byte1 & 0xFF) << 12) + ((byte2 & 0x3F) << 6) + (byte3 & 0x3F));
//	}
//	else if(byte1 >= 0xF0 && byte1 < 0xF5)
//	{
//		BYTE byte2 = readBits(8);
//		BYTE byte3 = readBits(8);
//		BYTE byte4 = readBits(8);
//		UINT codepoint = ((byte1 & 0x07) << 18) + ((byte2 & 0x3F) << 12) + ((byte3 & 0x3F) << 6) + (byte4 & 0x3F);
//		codepoint -= 0x10000;
//		ch = (codepoint >> 10) + 0xD800, (codepoint & 0x3FF) + 0xDC00;
//	}
//	return(ch);
//}
//const StringW & CNETbuff::readBitPString()
//{
//	WCHAR c;
//	Array<WCHAR> tmp;
//	while((c = readBitChar()))
//	{
//		tmp.push_back(c);
//	}
//	tmp.push_back(0);
//
//	m_wsTmp.Reserve(tmp.size());
//	memcpy(&m_wsTmp[0], &tmp[0], tmp.size() * sizeof(WCHAR));
//
//	return(m_wsTmp);
//}
//
//UINT CNETbuff::readBitsM(UINT $n)
//{
//	UINT $res = 0;
//
//	for(UINT $i = 0; $i < $n; $i++)
//	{
//		$res |= (readBitOne_sys() << $i);
//	}
//	return($res);
//}
//void CNETbuff::readBitsEndM()
//{
//	if(!(m_uReadBitNum == 0))
//	{
//		m_uBufPos++;
//		m_uReadBitNum = 0;
//	}
//}

void CNETbuff::writeString(const char *str)
{
	writeBytes((const BYTE*)str, strlen(str) + 1);
}
void CNETbuff::writeChar(UINT c)
{
	UINT codepoint = 0;
	short * in = (short*)&c;
	for(int i = 0; i < 2; ++i, ++in)
	{
		if(c >= 0xd800 && *in <= 0xdbff)
			codepoint = ((*in - 0xd800) << 10) + 0x10000;
		else
		{
			if(*in >= 0xdc00 && *in <= 0xdfff)
				codepoint |= *in - 0xdc00;
			else
				codepoint = *in;

			if(codepoint <= 0x7f)
			{
				writeInt8(codepoint);
				break;
			}
			else if(codepoint <= 0x7ff)
			{
				writeInt8(0xc0 | ((codepoint >> 6) & 0x1f));
				writeInt8(0x80 | (codepoint & 0x3f));
				break;
			}
			else if(codepoint <= 0xffff)
			{
				writeInt8(0xe0 | ((codepoint >> 12) & 0x0f));
				writeInt8(0x80 | ((codepoint >> 6) & 0x3f));
				writeInt8(0x80 | (codepoint & 0x3f));
				break;
			}
			else
			{
				writeInt8(0xf0 | ((codepoint >> 18) & 0x07));
				writeInt8(0x80 | ((codepoint >> 12) & 0x3f));
				writeInt8(0x80 | ((codepoint >> 6) & 0x3f));
				writeInt8(0x80 | (codepoint & 0x3f));
				break;
			}
		}
	}
}
void CNETbuff::writeBytes(const BYTE * bytes, UINT len)
{
	m_aBuf[m_uBufPos + len - 1] = 0;
	memcpy(&(m_aBuf[m_uBufPos]), bytes, len);
	m_uBufPos += len;
}
void CNETbuff::writeInt64(int64_t _i)
{
	int64_t i = _i;
	writeBytes((BYTE*)&i, 8);
}
void CNETbuff::writeUInt64(uint64_t _i)
{
	uint64_t i = _i;
	writeBytes((BYTE*)&i, 8);
}
void CNETbuff::writeInt32(int32_t _i)
{
	int32_t i = _i;
	writeBytes((BYTE*)&i, 4);
}
void CNETbuff::writeUInt32(uint32_t _i)
{
	uint32_t i = _i;
	writeBytes((BYTE*)&i, 4);
}
void CNETbuff::writeInt16(int16_t _i)
{
	int16_t i = _i;
	writeBytes((BYTE*)&i, 2);
}
void CNETbuff::writeUInt16(uint16_t _i)
{
	uint16_t i = _i;
	writeBytes((BYTE*)&i, 2);
}
void CNETbuff::writeInt8(int8_t _i)
{
	int8_t i = _i;
	writeBytes((BYTE*)&i, 1);
}
void CNETbuff::writeUInt8(BYTE _i)
{
	BYTE i = _i;
	writeBytes((BYTE*)&i, 1);
}
void CNETbuff::writeFloat32(float _i)
{
	float32_t i = _i;
	writeBytes((BYTE*)&i, 4);
}

UINT CNETbuff::getCursor()
{
	return(m_uBufPos);
}

BOOL CNETbuff::__ReadOneBit()
{
	BYTE $byte = m_aBuf[m_uBufPos];

	BOOL $bit = ($byte >> (m_uPosBit++)) & 0x01;
	if(m_uPosBit == 8)
	{
		m_uPosBit = 0;
		m_uBufPos++;
	}
	return($bit);
}

UINT CNETbuff::readBits_sys(UINT $count)
{//max 24 bit

	if(($count < 1) || ($count > 24))
		return(0);

	BYTE $r_byte = $count >> 3;
	UINT $b;
	if(m_uBufPos + 4 > m_aBuf.size())
	{
		return(0);
	}
	else
	{
		$b = readInt32();
		movL(4 - $r_byte);
	}

	UINT $bits = ($b >> m_uPosBit) & (~(((int)0xFFFFFFFF) << $count));

	m_uPosBit += ($count - ($r_byte << 3));

	if(m_uPosBit >= 8)
	{
		m_uPosBit -= 8;
		movR(1);
	}

	return($bits);

}

BOOL CNETbuff::readBitOne_sys()
{

	BYTE $byte = m_aBuf[m_uBufPos];

	BOOL $bit = ($byte >> m_uReadBitNum) & 0x01;

	if(m_uReadBitNum == 8)
	{
		m_uBufPos++;
		m_uReadBitNum = 0;
	}

	return($bit);
}

void * CNETbuff::getPointer()
{
	return(&(m_aBuf[0]));
}

UINT CNETbuff::getSize()
{
	return(m_aBuf.size());
}

UINT CNETbuff::getAllocatedSize()
{
	return(m_aBuf.GetAllocSize());
}
