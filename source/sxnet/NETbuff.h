#ifndef NETbuff_H
#define NETbuff_H

#include "INETbuff.h"
namespace NET
{
	class NETbuff: public INETbuff
	{
	protected:
		UINT bufPos;
		UINT posBit;
		Array<BYTE, 4096> buf;

	public:

		NETbuff();

		void movR(int i = 0);
		void movL(int i = 0);
		void movE();
		void movB();
		BOOL ifEnd();
		BOOL ifEmpty();

		void setCursor(UINT i = 0);
		void setBuf(const BYTE * data, UINT size);
		void addBuf(const BYTE * data, UINT size);
		void getBuf(BYTE * data, UINT b = 0, UINT l = ~0);
		int ReadSBits(BYTE numbits);

		const StringW & readPString();
		WCHAR readChar();
		const StringW & readString(UINT len);
		int readInt32();
		__int64 readInt64();
		short readInt16();
		BYTE readInt8();
		float readFloat32();


		UINT readBits(UINT count = 0);
		BOOL readOneBitBool();
		void readBitsBegin();
		void readBitsEnd();
		void readBitsRelease();
		WCHAR readBitChar();
		const StringW & readBitPString();

		UINT readBitsM(UINT $n);
		void readBitsEndM();

		void writePString(const StringW & str);
		void writeChar(UINT c);
		void writeString(const StringW & str);
		void writeBytes(const BYTE * bytes, UINT len);
		void writeInt64(__int64 i);
		void writeInt32(int i);
		void writeInt16(short i);
		void writeInt8(BYTE i);
		void writeFloat32(float i);

		UINT getCursor();

		void * GetPointer();

		UINT GetSize();

	private:
		UINT ReadBitNum;

		BOOL __ReadOneBit();

		UINT readBits_sys(UINT $count = 0);

		BOOL readBitOne_sys();

		StringW m_wsTmp;
	};

};

#endif
