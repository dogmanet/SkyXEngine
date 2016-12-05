#ifndef INETbuff_H
#define INETbuff_H

#include <sxtypes.h>
#include <core/array.h>
#include <string/string.h>

namespace NET
{
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
		virtual BOOL ifEnd() = 0;
		virtual BOOL ifEmpty() = 0;

		virtual void setCursor(UINT i = 0) = 0;
		virtual void setBuf(const BYTE * data, UINT size) = 0;
		virtual void addBuf(const BYTE * data, UINT size) = 0;
		virtual void getBuf(BYTE * data, UINT b = 0, UINT l = ~0) = 0;
		virtual int ReadSBits(BYTE numbits) = 0;

		virtual const StringW & readPString() = 0;
		virtual WCHAR readChar() = 0;
		virtual const StringW & readString(UINT len) = 0;
		virtual int readInt32() = 0;
		virtual __int64 readInt64() = 0;
		virtual short readInt16() = 0;
		virtual BYTE readInt8() = 0;
		virtual float readFloat32() = 0;


		virtual UINT readBits(UINT count = 0) = 0;
		virtual BOOL readOneBitBool() = 0;
		virtual void readBitsBegin() = 0;
		virtual void readBitsEnd() = 0;
		virtual void readBitsRelease() = 0;
		virtual WCHAR readBitChar() = 0;
		virtual const StringW & readBitPString() = 0;

		virtual UINT readBitsM(UINT $n) = 0;
		virtual void readBitsEndM() = 0;

		virtual void writePString(const StringW & str) = 0;
		virtual void writeChar(UINT c) = 0;
		virtual void writeString(const StringW & str) = 0;
		virtual void writeBytes(const BYTE * bytes, UINT len) = 0;
		virtual void writeInt64(__int64 i) = 0;
		virtual void writeInt32(int i) = 0;
		virtual void writeInt16(short i) = 0;
		virtual void writeInt8(BYTE i) = 0;
		virtual void writeFloat32(float i) = 0;

		virtual UINT getCursor() = 0;

		virtual void * GetPointer() = 0;

		virtual UINT GetSize() = 0;
	};

};

#endif
