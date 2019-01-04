#ifndef CNETbuff_H_
#define CNETbuff_H_

#include <common/array.h>

#include "INETbuff.h"

class CNETbuff: public INETbuff
{
protected:
	UINT m_uBufPos;
	UINT m_uPosBit;
	Array<BYTE, 512> m_aBuf;

public:
	CNETbuff();

	void movR(int i = 0);
	void movL(int i = 0);
	void movE();
	void movB();
	BOOL isEnd();
	BOOL isEmpty();
	void reset();

	void setCursor(UINT i = 0);
	void setBuf(const BYTE * data, UINT size);
	void addBuf(const BYTE * data, UINT size);
	void getBuf(BYTE * data, UINT b = 0, UINT l = ~0);

	void readString(char *pBuf, UINT iBufSize);
	UINT readChar();
	void readBytes(BYTE *pBuf, UINT len);
	int32_t readInt32();
	uint32_t readUInt32();
	int64_t readInt64();
	uint64_t readUInt64();
	int16_t readInt16();
	uint16_t readUInt16();
	int8_t readInt8();
	BYTE readUInt8();
	float readFloat32();


	//UINT readBits(UINT count = 0);
	//int readSBits(BYTE numbits);
	//BOOL readOneBitBool();
	//void readBitsBegin();
	//void readBitsEnd();
	//void readBitsRelease();
	//char readBitChar();
	//const String & readBitPString();

	//UINT readBitsM(UINT $n);
	//void readBitsEndM();

	void writeString(const char *str);
	void writeChar(UINT c);
	void writeBytes(const BYTE * bytes, UINT len);
	void writeInt64(int64_t i);
	void writeUInt64(uint64_t i);
	void writeInt32(int32_t i);
	void writeUInt32(uint32_t i);
	void writeInt16(int16_t i);
	void writeUInt16(uint16_t i);
	void writeInt8(int8_t i);
	void writeUInt8(BYTE i);
	void writeFloat32(float i);

	UINT getCursor();

	void * getPointer();

	UINT getSize();

	UINT getAllocatedSize();

private:
	UINT m_uReadBitNum;

	BOOL __ReadOneBit();

	UINT readBits_sys(UINT $count = 0);

	BOOL readBitOne_sys();
};

#endif
