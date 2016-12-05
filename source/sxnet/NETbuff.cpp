#include "NETbuff.h"

namespace NET
{

	NETbuff::NETbuff():
		bufPos(0),
		posBit(0),
		ReadBitNum(0)
	{
	}

	void NETbuff::movR(int i)
	{
		this->bufPos += i;

		if(this->bufPos >= this->buf.size())
		{
			this->buf.resize(this->bufPos + 1);
		}
	}
	void NETbuff::movL(int i)
	{
		this->bufPos -= i;
	}
	void NETbuff::movE()
	{
		this->bufPos = this->buf.size();
	}
	void NETbuff::movB()
	{
		this->bufPos = 0;
	}
	BOOL NETbuff::ifEnd()
	{
		return(this->bufPos >= this->buf.size());
	}
	BOOL NETbuff::ifEmpty()
	{

		return(this->buf.size() == 0);

	}

	void NETbuff::setCursor(UINT i)
	{
		this->bufPos = i;
	}
	void NETbuff::setBuf(const BYTE * data, UINT size)
	{
		this->bufPos = 0;
		this->buf.resize(size);
		if(size != 0)
		{
			memcpy(&(this->buf[0]), data, size);
		}
	}
	void NETbuff::addBuf(const BYTE * data, UINT size)
	{
		UINT spos = this->buf.size();
		this->buf.resize(spos + size);
		memcpy(&(this->buf[spos]), data, size);
	}
	void NETbuff::getBuf(BYTE * data, UINT b, UINT l)
	{
		UINT size = l;
		if(size > this->buf.size() - b)
		{
			size = this->buf.size() - b;
		}
		memcpy(data, &(this->buf[b]), size);
	}
	int NETbuff::ReadSBits(BYTE numbits)
	{
		int nSignBit = this->readOneBitBool();
		int result = this->readBits(numbits - 1);
		if(nSignBit)
		{
			result = -result;
		}

		return(result);
	}

	const StringW & NETbuff::readPString()
	{
		m_wsTmp = L"";
		WCHAR c;
		while(!this->ifEnd() && (c = this->readChar()))
		{
			m_wsTmp += c;
		}

		return(m_wsTmp);
	}


	/*std::wstring UTF8to16(const char * in)
	{
	std::wstring out;
	if (in == NULL)
	return out;

	unsigned int codepoint;
	while (*in != 0)
	{
	unsigned char ch = static_cast<unsigned char>(*in);
	if (ch <= 0x7f)
	codepoint = ch;
	else if (ch <= 0xbf)
	codepoint = (codepoint << 6) | (ch & 0x3f);
	else if (ch <= 0xdf)
	codepoint = ch & 0x1f;
	else if (ch <= 0xef)
	codepoint = ch & 0x0f;
	else
	codepoint = ch & 0x07;
	++in;
	if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
	{
	if (codepoint > 0xffff)
	{
	out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
	out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
	}
	else if (codepoint < 0xd800 || codepoint >= 0xe000)
	out.append(1, static_cast<wchar_t>(codepoint));
	}
	}
	return out;
	}*/
	WCHAR NETbuff::readChar()
	{
		UINT ch = L'';
		BYTE byte1 = this->readInt8();
		if(byte1 < 0x80)
		{
			ch = byte1;
		}
		else if(byte1 >= 0xC2 && byte1 < 0xE0)
		{
			BYTE byte2 = this->readInt8();
			ch = (((byte1 & 0x1F) << 6) + (byte2 & 0x3F));
		}
		else if(byte1 >= 0xE0 && byte1 < 0xF0)
		{
			BYTE byte2 = this->readInt8();
			BYTE byte3 = this->readInt8();
			ch = (((byte1 & 0xFF) << 12) + ((byte2 & 0x3F) << 6) + (byte3 & 0x3F));
		}
		else if(byte1 >= 0xF0 && byte1 < 0xF5)
		{
			BYTE byte2 = this->readInt8();
			BYTE byte3 = this->readInt8();
			BYTE byte4 = this->readInt8();
			UINT codepoint = ((byte1 & 0x07) << 18) + ((byte2 & 0x3F) << 12) + ((byte3 & 0x3F) << 6) + (byte4 & 0x3F);
			codepoint -= 0x10000;
			ch = (codepoint >> 10) + 0xD800, (codepoint & 0x3FF) + 0xDC00;
		}
		return(ch);
	}
	const StringW & NETbuff::readString(UINT len)
	{
		m_wsTmp = L"";

		while(len-- && !this->ifEnd())
		{
			m_wsTmp += this->readChar();
		}

		return(m_wsTmp);

	}
	int NETbuff::readInt32()
	{
		int32_t i;
		UINT len = this->buf.size() - this->bufPos;
		if(len > 4)
		{
			len = 4;
		}
		memcpy(&i, &(this->buf[this->bufPos]), len);
		this->bufPos += len;
		return(i);
	}
	__int64 NETbuff::readInt64()
	{
		int64_t i;
		UINT len = this->buf.size() - this->bufPos;
		if(len > 8)
		{
			len = 8;
		}
		memcpy(&i, &(this->buf[this->bufPos]), len);
		this->bufPos += len;
		return(i);

	}
	short NETbuff::readInt16()
	{
		int16_t i;
		UINT len = this->buf.size() - this->bufPos;
		if(len > 2)
		{
			len = 2;
		}
		memcpy(&i, &(this->buf[this->bufPos]), len);
		this->bufPos += len;
		return(i);
	}
	BYTE NETbuff::readInt8()
	{
		int8_t i;
		UINT len = this->buf.size() - this->bufPos;
		if(len > 1)
		{
			len = 1;
		}
		memcpy(&i, &(this->buf[this->bufPos]), len);
		this->bufPos += len;
		return(i);
	}
	float NETbuff::readFloat32()
	{
		float32_t i;
		UINT len = this->buf.size() - this->bufPos;
		if(len > 4)
		{
			len = 4;
		}
		memcpy(&i, &(this->buf[this->bufPos]), len);
		this->bufPos += len;
		return(i);
	}


	UINT NETbuff::readBits(UINT count)
	{//max 32 bit

		if(count > 32)
		{
			return(0);
		}

		if(count > 24)
		{
			return((this->readBits_sys(24) | (this->readBits_sys(count - 24) << 24)) & ((1 << count) - 1));
		}
		return(this->readBits_sys(count) & ((1 << count) - 1));

	}
	BOOL NETbuff::readOneBitBool()
	{
		return(this->readBits(1));
	}
	void NETbuff::readBitsBegin()
	{
		this->posBit = 0;
	}
	void NETbuff::readBitsEnd()
	{
		if(this->posBit > 0)
		{
			this->posBit = 0;
			this->movR(1);
		}
	}
	void NETbuff::readBitsRelease()
	{
		if(this->posBit > 0)
		{
			this->posBit = 0;
			this->movR(1);
		}
	}
	WCHAR NETbuff::readBitChar()
	{
		UINT ch = L'';
		BYTE byte1 = this->readBits(8);
		if(byte1 < 0x80)
		{
			ch = byte1;
		}
		else if(byte1 >= 0xC2 && byte1 < 0xE0)
		{
			BYTE byte2 = this->readBits(8);
			ch = (((byte1 & 0x1F) << 6) + (byte2 & 0x3F));
		}
		else if(byte1 >= 0xE0 && byte1 < 0xF0)
		{
			BYTE byte2 = this->readBits(8);
			BYTE byte3 = this->readBits(8);
			ch = (((byte1 & 0xFF) << 12) + ((byte2 & 0x3F) << 6) + (byte3 & 0x3F));
		}
		else if(byte1 >= 0xF0 && byte1 < 0xF5)
		{
			BYTE byte2 = this->readBits(8);
			BYTE byte3 = this->readBits(8);
			BYTE byte4 = this->readBits(8);
			UINT codepoint = ((byte1 & 0x07) << 18) + ((byte2 & 0x3F) << 12) + ((byte3 & 0x3F) << 6) + (byte4 & 0x3F);
			codepoint -= 0x10000;
			ch = (codepoint >> 10) + 0xD800, (codepoint & 0x3FF) + 0xDC00;
		}
		return(ch);
	}
	const StringW & NETbuff::readBitPString()
	{
		m_wsTmp = L"";
		WCHAR c;
		while((c = this->readBitChar()))
		{
			m_wsTmp += c;
		}

		return(m_wsTmp);
	}

	UINT NETbuff::readBitsM(UINT $n)
	{
		UINT $res = 0;

		for(UINT $i = 0; $i < $n; $i++)
		{
			$res |= (this->readBitOne_sys() << $i);
		}
		return($res);
	}
	void NETbuff::readBitsEndM()
	{
		if(!(this->ReadBitNum == 0))
		{
			this->bufPos++;
			this->ReadBitNum = 0;
		}
	}

	void NETbuff::writePString(const StringW & str)
	{
		this->writeString(str);
		this->writeChar(0);
	}
	void NETbuff::writeChar(UINT c)
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
					this->writeInt8(codepoint);
					break;
				}
				else if(codepoint <= 0x7ff)
				{
					this->writeInt8(0xc0 | ((codepoint >> 6) & 0x1f));
					this->writeInt8(0x80 | (codepoint & 0x3f));
					break;
				}
				else if(codepoint <= 0xffff)
				{
					this->writeInt8(0xe0 | ((codepoint >> 12) & 0x0f));
					this->writeInt8(0x80 | ((codepoint >> 6) & 0x3f));
					this->writeInt8(0x80 | (codepoint & 0x3f));
					break;
				}
				else
				{
					this->writeInt8(0xf0 | ((codepoint >> 18) & 0x07));
					this->writeInt8(0x80 | ((codepoint >> 12) & 0x3f));
					this->writeInt8(0x80 | ((codepoint >> 6) & 0x3f));
					this->writeInt8(0x80 | (codepoint & 0x3f));
					break;
				}
			}
		}
	}
	void NETbuff::writeString(const StringW & str)
	{
		for(UINT i = 0, l = str.length(); i < l; ++i)
		{
			this->writeChar(str[i]);
		}
	}
	void NETbuff::writeBytes(const BYTE * bytes, UINT len)
	{
		if(this->bufPos + len > this->buf.size())
		{
			this->buf.reserve(this->bufPos + len);
		}
		memcpy(&(this->buf[this->bufPos]), bytes, len);
		this->bufPos += len;
	}
	void NETbuff::writeInt64(__int64 _i)
	{
		int64_t i = _i;
		this->writeBytes((BYTE*)&i, 8);
	}
	void NETbuff::writeInt32(int _i)
	{
		int32_t i = _i;
		this->writeBytes((BYTE*)&i, 4);
	}
	void NETbuff::writeInt16(short _i)
	{
		int16_t i = _i;
		this->writeBytes((BYTE*)&i, 2);
	}
	void NETbuff::writeInt8(BYTE _i)
	{
		int8_t i = _i;
		this->writeBytes((BYTE*)&i, 1);
	}
	void NETbuff::writeFloat32(float _i)
	{
		float32_t i = _i;
		this->writeBytes((BYTE*)&i, 4);
	}

	UINT NETbuff::getCursor()
	{
		return(this->bufPos);
	}

	BOOL NETbuff::__ReadOneBit()
	{
		BYTE $byte = this->buf[this->bufPos];

		BOOL $bit = ($byte >> (this->posBit++)) & 0x01;
		if(this->posBit == 8)
		{
			this->posBit = 0;
			this->bufPos++;
		}
		return($bit);
	}

	UINT NETbuff::readBits_sys(UINT $count)
	{//max 24 bit

		if(($count < 1) || ($count > 24))
			return(0);

		BYTE $r_byte = $count >> 3;
		UINT $b;
		if(this->bufPos + 4 > this->buf.size())
		{
			return(0);
		}
		else
		{
			$b = this->readInt32();
			this->movL(4 - $r_byte);
		}

		UINT $bits = ($b >> this->posBit) & (~(((int)0xFFFFFFFF) << $count));

		this->posBit += ($count - ($r_byte << 3));

		if(this->posBit >= 8)
		{
			this->posBit -= 8;
			this->movR(1);
		}

		return($bits);

	}

	BOOL NETbuff::readBitOne_sys()
	{

		BYTE $byte = this->buf[this->bufPos];

		BOOL $bit = ($byte >> this->ReadBitNum) & 0x01;

		if(this->ReadBitNum == 8)
		{
			this->bufPos++;
			this->ReadBitNum = 0;
		}

		return($bit);
	}

	void * NETbuff::GetPointer()
	{
		return(&(this->buf[0]));
	}

	UINT NETbuff::GetSize()
	{
		return(this->buf.size());
	}
};