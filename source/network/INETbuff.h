#ifndef INETbuff_H
#define INETbuff_H

#include <common/sxtypes.h>

class INETbuff
{
public:
	virtual ~INETbuff()
	{
	}

	virtual void movR(int i = 0) = 0;
	virtual void movL(int i = 0) = 0;
	virtual void movE() = 0;
	virtual void movB() = 0;
	virtual BOOL isEnd() = 0;
	virtual BOOL isEmpty() = 0;
	virtual void reset() = 0;

	virtual void setCursor(UINT i = 0) = 0;
	virtual void setBuf(const BYTE * data, UINT size) = 0;
	virtual void addBuf(const BYTE * data, UINT size) = 0;
	virtual void getBuf(BYTE * data, UINT b = 0, UINT l = ~0) = 0;

	virtual void readString(char *pBuf, UINT iBufSize) = 0;
	virtual UINT readChar() = 0;
	virtual void readBytes(BYTE *pBuf, UINT len) = 0;
	virtual int32_t readInt32() = 0;
	virtual uint32_t readUInt32() = 0;
	virtual int64_t readInt64() = 0;
	virtual uint64_t readUInt64() = 0;
	virtual int16_t readInt16() = 0;
	virtual uint16_t readUInt16() = 0;
	virtual int8_t readInt8() = 0;
	virtual BYTE readUInt8() = 0;
	virtual float readFloat32() = 0;


	//virtual UINT readBits(UINT count = 0) = 0;
	//virtual int readSBits(BYTE numbits) = 0;
	//virtual BOOL readOneBitBool() = 0;
	//virtual void readBitsBegin() = 0;
	//virtual void readBitsEnd() = 0;
	//virtual void readBitsRelease() = 0;
	//virtual char readBitChar() = 0;
	//virtual const String & readBitPString() = 0;

	//virtual UINT readBitsM(UINT $n) = 0;
	//virtual void readBitsEndM() = 0;

	virtual void writeString(const char *str) = 0;
	virtual void writeChar(UINT c) = 0;
	virtual void writeBytes(const BYTE * bytes, UINT len) = 0;
	virtual void writeInt64(int64_t i) = 0;
	virtual void writeUInt64(uint64_t i) = 0;
	virtual void writeInt32(int32_t i) = 0;
	virtual void writeUInt32(uint32_t i) = 0;
	virtual void writeInt16(int16_t i) = 0;
	virtual void writeUInt16(uint16_t i) = 0;
	virtual void writeInt8(int8_t i) = 0;
	virtual void writeUInt8(BYTE i) = 0;
	virtual void writeFloat32(float i) = 0;

	virtual UINT getCursor() = 0;

	virtual void *getPointer() = 0;

	virtual UINT getSize() = 0;
	virtual UINT getAllocatedSize() = 0;
};

#endif
