
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#if defined(_LINUX) || defined(_MAC)
#	include <wchar.h>
#	include <stdlib.h>
#	define wcscmpi wcscasecmp
#	define stricmp strcasecmp
#endif
#if defined(_WINDOWS)
#	define wcscmpi wcsicmp
#	include <Windows.h>
#endif

#include "string.h"

#if defined(_WINDOWS)
#	pragma warning(disable:4996)
#endif

#if defined(_LINUX) || defined(_MAC)
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wsign-compare"
#endif

//////////////////////

const char * String::c_str() const
{
	return(string);
}

String::String()
{
	string = new char[1];
	string[0] = 0;
}

String::String(const char * str)
{
	string = new char[strlen(str) + 1];
	//sprintf(string,"%s",str);
	memcpy(string, str, strlen(str));
	string[strlen(str)] = 0;
}

String::String(char sym)
{
	string = new char[2];
	//sprintf(string,"%s",&sym);
	string[0] = sym;
	string[1] = 0;
}

String::String(int num)
{
	char * tmp = new char[64];
	sprintf(tmp, "%d", num);
	string = new char[strlen(tmp) + 1];
	memcpy(string, tmp, strlen(tmp) + 1);
	SAFE_DELETE_A(tmp);
}

String::String(WORD num)
{
	char * tmp = new char[64];
	sprintf(tmp, "%u", num);
	string = new char[strlen(tmp) + 1];
	memcpy(string, tmp, strlen(tmp) + 1);
	SAFE_DELETE_A(tmp);
}

String::String(DWORD num)
{
	char * tmp = new char[64];
	sprintf(tmp, "%lu", num);
	string = new char[strlen(tmp) + 1];
	memcpy(string, tmp, strlen(tmp) + 1);
	SAFE_DELETE_A(tmp);
}

String::String(long num)
{
	char * tmp = new char[64];
	sprintf(tmp, "%ld", num);
	string = new char[strlen(tmp) + 1];
	memcpy(string, tmp, strlen(tmp) + 1);
	SAFE_DELETE_A(tmp);
}

String::String(double num)
{
	char * tmp = new char[64];
	sprintf(tmp, "%G", num);
	string = new char[strlen(tmp) + 1];
	memcpy(string, tmp, strlen(tmp) + 1);
	SAFE_DELETE_A(tmp);
}

String::String(bool bf)
{
	string = new char[bf ? 5 : 6];
	sprintf(string, "%s", bf ? "true" : "false");
}

String::String(const String & str)
{
	string = new char[str.length() + 1];
	memcpy(string, str.c_str(), str.length() + 1);
}

String::String(const String * str)
{
	string = new char[str->length() + 1];
	memcpy(string, str->c_str(), str->length() + 1);
}

String::String(String && other)
{
	//SAFE_DELETE_A(string);
	string = other.string;
	other.string = NULL;
}

String::~String()
{
	Release();
}


void String::Release()
{
	SAFE_DELETE_A(string);
}

////////////////////////////////////////////

String String::operator+(const String &str) const
{
	char * ns = new char[length() + str.length() + 1];
	sprintf(ns, "%s%s", string, str.c_str());
	String newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

String String::operator+(const String *str)
{
	char * ns = new char[length() + str->length() + 1];
	sprintf(ns, "%s%s", string, str->c_str());
	String newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

String String::operator+(const char * str)
{
	char * ns = new char[length() + strlen(str) + 1];
	sprintf(ns, "%s%s", string, str);
	String newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

String String::operator+(const char & sym)
{
	char * ns = new char[length() + 2];
	sprintf(ns, "%s%c", string, sym);
	String newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

String String::operator+(const int & num)
{
	char * ns = new char[length() + 64];
	sprintf(ns, "%s%d", string, num);
	String newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

String String::operator+(const WORD & num)
{
	char * ns = new char[length() + 64];
	sprintf(ns, "%s%hu", string, num);
	String newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

String String::operator+(const DWORD & num)
{
	char * ns = new char[length() + 64];
	sprintf(ns, "%s%lu", string, num);
	String newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

String String::operator+(const long & num)
{
	char * ns = new char[length() + 64];
	sprintf(ns, "%s%ld", string, num);
	String newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

String String::operator+(const double & num)
{
	char * ns = new char[length() + 64];
	sprintf(ns, "%s%G", string, num);
	String newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

String String::operator+(const bool & bf)
{
	char * ns = new char[length() + (bf ? 5 : 6)];
	sprintf(ns, "%s%s", string, bf ? "true" : "false");
	String newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

//////////////////////////////////////////////////////


String & String::operator=(const String & str)
{
	Release();
	string = new char[str.length() + 1];
	memcpy(string, str.c_str(), str.length() + 1);
	return(*this);
}

String & String::operator=(const String * str)
{
	Release();
	string = new char[str->length() + 1];
	memcpy(string, str->c_str(), str->length() + 1);
	return(*this);
}

String & String::operator=(const char * str)
{
	Release();
	string = new char[strlen(str) + 1];
	memcpy(string, str, strlen(str) + 1);
	return(*this);
}

String & String::operator=(const char & sym)
{
	Release();
	string = new char[2];
	string[0] = sym;
	string[1] = 0;
	return(*this);
}

String & String::operator=(const int & num)
{
	Release();
	char * tmp = new char[64];
	sprintf(tmp, "%d", num);
	string = new char[strlen(tmp) + 1];
	memcpy(string, tmp, strlen(tmp) + 1);
	SAFE_DELETE_A(tmp);
	return(*this);
}

String & String::operator=(const WORD & num)
{
	Release();
	char * tmp = new char[64];
	sprintf(tmp, "%hu", num);
	string = new char[strlen(tmp) + 1];
	memcpy(string, tmp, strlen(tmp) + 1);
	SAFE_DELETE_A(tmp);
	return(*this);
}

String & String::operator=(const DWORD & num)
{
	Release();
	char * tmp = new char[64];
	sprintf(tmp, "%lu", num);
	string = new char[strlen(tmp) + 1];
	memcpy(string, tmp, strlen(tmp) + 1);
	SAFE_DELETE_A(tmp);
	return(*this);
}

String & String::operator=(const long & num)
{
	Release();
	char * tmp = new char[64];
	sprintf(tmp, "%ld", num);
	string = new char[strlen(tmp) + 1];
	memcpy(string, tmp, strlen(tmp) + 1);
	SAFE_DELETE_A(tmp);
	return(*this);
}

String & String::operator=(const double & num)
{
	Release();
	char * tmp = new char[64];
	sprintf(tmp, "%G", num);
	string = new char[strlen(tmp) + 1];
	memcpy(string, tmp, strlen(tmp) + 1);
	SAFE_DELETE_A(tmp);
	return(*this);
}

String & String::operator=(const bool & bf)
{
	Release();
	string = new char[bf ? 5 : 6];
	sprintf(string, "%s", bf ? "true" : "false");
	return(*this);
}

String & String::operator=(String && other)
{
	if(this != &other)
	{
		SAFE_DELETE(string);
		string = other.string;
		other.string = NULL;
	}
	return(*this);
}

///////////////////////////////////////////////////


String & String::operator+=(const String & str)
{
	char * ns = new char[length() + str.length() + 1];
	sprintf(ns, "%s%s", string, str.c_str());
	SAFE_DELETE_A(string);
	string = ns;
	return(*this);
}

String & String::operator+=(const String * str)
{
	char * ns = new char[length() + str->length() + 1];
	sprintf(ns, "%s%s", string, str->c_str());
	SAFE_DELETE_A(string);
	string = ns;
	return(*this);
}

String & String::operator+=(const char * str)
{
	char * newstring = new char[length() + strlen(str) + 1];
	sprintf(newstring, "%s%s", string, str);
	SAFE_DELETE_A(string);
	string = newstring;
	return(*this);
}

String & String::operator+=(const char & sym)
{
	char * newstring = new char[length() + 2];
	sprintf(newstring, "%s%c", string, sym);
	SAFE_DELETE_A(string);
	string = newstring;
	return(*this);
}

String & String::operator+=(const int & num)
{
	char * tmp = new char[length() + 64];
	sprintf(tmp, "%s%d", string, num);
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

String & String::operator+=(const WORD & num)
{
	char * tmp = new char[length() + 64];
	sprintf(tmp, "%s%hu", string, num);
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

String & String::operator+=(const DWORD & num)
{
	char * tmp = new char[length() + 64];
	sprintf(tmp, "%s%lu", string, num);
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

String & String::operator+=(const long & num)
{
	char * tmp = new char[length() + 64];
	sprintf(tmp, "%s%ld", string, num);
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

String & String::operator+=(const double & num)
{
	char * tmp = new char[length() + 64];
	sprintf(tmp, "%s%G", string, num);
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

String & String::operator+=(const bool & bf)
{
	char * tmp = new char[length() + (bf ? 5 : 6)];
	sprintf(tmp, "%s%s", string, bf ? "true" : "false");
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

/////////////////////////////////////////////////////////

String String::operator-(const String & str)
{
	String newStr = this;
	newStr.Replace(str, "", 0);
	return(newStr);
}

String String::operator-(const String * str)
{
	String newStr = this;
	newStr.Replace(str, "", 0);
	return(newStr);
}

String String::operator-(const char * str)
{
	String newStr = this;
	newStr.Replace(str, "", 0);
	return(newStr);
}

String String::operator-(const char & sym)
{
	String newStr = this;
	newStr.Replace(sym, "", 0);
	return(newStr);
}

String String::operator-(const int & num)
{
	String newStr = this;
	newStr.Replace(num, "", 0);
	return(newStr);
}

String String::operator-(const WORD & num)
{
	String newStr = this;
	newStr.Replace(num, "", 0);
	return(newStr);
}

String String::operator-(const DWORD & num)
{
	String newStr = this;
	newStr.Replace(num, "", 0);
	return(newStr);
}

String String::operator-(const long & num)
{
	String newStr = this;
	newStr.Replace(num, "", 0);
	return(newStr);
}

String String::operator-(const double & num)
{
	String newStr = this;
	newStr.Replace(num, "", 0);
	return(newStr);
}

String String::operator-(const bool & bf)
{
	String newStr = this;
	newStr.Replace(bf, "", 0);
	return(newStr);
}

/////////////////////////////////////////////////////////

String & String::operator-=(const String & str)
{
	Replace(str, "", 0);
	return(*this);
}

String & String::operator-=(const String * str)
{
	Replace(str, "", 0);
	return(*this);
}

String & String::operator-=(const char * str)
{
	Replace(str, "", 0);
	return(*this);
}

String & String::operator-=(const char & sym)
{
	Replace(sym, "", 0);
	return(*this);
}

String & String::operator-=(const int & num)
{
	Replace(num, "", 0);
	return(*this);
}

String & String::operator-=(const WORD & num)
{
	Replace(num, "", 0);
	return(*this);
}

String & String::operator-=(const DWORD & num)
{
	Replace(num, "", 0);
	return(*this);
}

String & String::operator-=(const long & num)
{
	Replace(num, "", 0);
	return(*this);
}

String & String::operator-=(const double & num)
{
	Replace(num, "", 0);
	return(*this);
}

String & String::operator-=(const bool & bf)
{
	Replace(bf, "", 0);
	return(*this);
}

/////////////////////////////////////////////////////////

String String::operator/(const String & str)
{
	String newStr = this;
	newStr.ReplaceAll(str, "");
	return(newStr);
}

String String::operator/(const String * str)
{
	String newStr = this;
	newStr.ReplaceAll(str, "");
	return(newStr);
}

String String::operator/(const char * str)
{
	String newStr = this;
	newStr.ReplaceAll(str, "");
	return(newStr);
}

String String::operator/(const char & sym)
{
	String newStr = this;
	newStr.ReplaceAll(sym, "");
	return(newStr);
}

String String::operator/(const int & num)
{
	String newStr = this;
	newStr.ReplaceAll(num, "");
	return(newStr);
}

String String::operator/(const WORD & num)
{
	String newStr = this;
	newStr.ReplaceAll(num, "");
	return(newStr);
}

String String::operator/(const DWORD & num)
{
	String newStr = this;
	newStr.ReplaceAll(num, "");
	return(newStr);
}

String String::operator/(const long & num)
{
	String newStr = this;
	newStr.ReplaceAll(num, "");
	return(newStr);
}

String String::operator/(const double & num)
{
	String newStr = this;
	newStr.ReplaceAll(num, "");
	return(newStr);
}

String String::operator/(const bool & bf)
{
	String newStr = this;
	newStr.ReplaceAll(bf, "");
	return(newStr);
}

/////////////////////////////////////////////////////////

String & String::operator/=(const String & str)
{
	ReplaceAll(str, "");
	return(*this);
}

String & String::operator/=(const String * str)
{
	ReplaceAll(str, "");
	return(*this);
}

String & String::operator/=(const char * str)
{
	ReplaceAll(str, "");
	return(*this);
}

String & String::operator/=(const char & sym)
{
	ReplaceAll(sym, "");
	return(*this);
}

String & String::operator/=(const int & num)
{
	ReplaceAll(num, "");
	return(*this);
}

String & String::operator/=(const WORD & num)
{
	ReplaceAll(num, "");
	return(*this);
}

String & String::operator/=(const DWORD & num)
{
	ReplaceAll(num, "");
	return(*this);
}

String & String::operator/=(const long & num)
{
	ReplaceAll(num, "");
	return(*this);
}

String & String::operator/=(const double & num)
{
	ReplaceAll(num, "");
	return(*this);
}

String & String::operator/=(const bool & bf)
{
	ReplaceAll(bf, "");
	return(*this);
}

///////////////////////////////////////////////////////

bool String::operator==(const String & str) const
{
	/*if(this->length() == str.length())
	{
	DWORD len = this->length();
	for(DWORD i = 0; i<len; i++)
	{
	if(this->string[i] != str.string[i])
	return false;
	}
	}
	else
	return false;

	return true;*/
	return(operator==(&str));
}

bool String::operator==(const String * str) const
{
	if(length() == str->length())
	{
		DWORD len = length();
		for(DWORD i = 0; i<len; i++)
		{
			if(string[i] != str->string[i])
				return false;
		}
	}
	else
		return false;

	return true;
}

bool String::operator==(const char * str) const
{
	/*char *newstring = new char[strlen(str)];
	sprintf(newstring, "%s", str);
	if(*this == newstring)
	return true;
	else
	return false;
	*/
	if(length() == strlen(str))
	{
		DWORD len = length();
		for(DWORD i = 0; i<len; i++)
		{
			if(string[i] != str[i])
				return false;
		}
	}
	else
		return false;

	return true;
}

bool String::operator==(const char & sym) const
{
	if(length() == 1)
	{
		if(string[0] != sym)
			return false;
	}
	else
		return false;

	return true;
}

bool String::operator==(const int & num) const
{
	/*int Value = ToInt();
	if(Value == num)
	return true;
	else
	return false;*/
	return(operator==((long)num));
}

bool String::operator==(const WORD & num) const
{
	return(operator==((DWORD)num));
}

bool String::operator==(const DWORD & num) const
{
	DWORD Value = ToUnsLongInt();
	return(Value == num);
}

bool String::operator==(const long & num) const
{
	long Value = ToLongInt();
	return(Value == num);
}

bool String::operator==(const double & num) const
{
	double Value = ToDouble();
	return(Value == num);
}

bool String::operator==(const bool & bf) const
{
	bool Value = ToBool();
	return(Value == bf);
}

/////////////////////////////////////////////////////////

bool String::operator!=(const String & str) const
{
	if(*this == str)
		return false;
	else
		return true;
}

bool String::operator!=(const String * str) const
{
	if(*this == *str)
		return false;
	else
		return true;
}

bool String::operator!=(const char * str) const
{
	if(*this == str)
		return false;
	else
		return true;
}

bool String::operator!=(const char & sym) const
{
	if(*this == sym)
		return false;
	else
		return true;
}

bool String::operator!=(const int & num) const
{
	if(*this == num)
		return false;
	else
		return true;
}

bool String::operator!=(const WORD & num) const
{
	if(*this == num)
		return false;
	else
		return true;
}

bool String::operator!=(const DWORD & num) const
{
	if(*this == num)
		return false;
	else
		return true;
}

bool String::operator!=(const long & num) const
{
	if(*this == num)
		return false;
	else
		return true;
}

bool String::operator!=(const double & num) const
{
	if(*this == num)
		return false;
	else
		return true;
}

bool String::operator!=(const bool & bf) const
{
	if(*this == bf)
		return false;
	else
		return true;
}

/////////////////////////////////////////////////////////

char & String::operator[](const DWORD & num)
{
	return(string[num]);
}

const char & String::operator[](const DWORD & num) const
{
	return(string[num]);
}

////////////////////////////////////////////////////////

DWORD String::length() const
{
	return(strlen(string));
}

DWORD String::find(const char * str, DWORD pos) const
{
	//DWORD BeginPos = pos;
	//char *newChar = new char[strlen(string+BeginPos)];
	//sprintf(newChar,"%s",string+BeginPos);
	char *Find = strstr(string + pos, str);

	if(Find != NULL)
		// return Find - (string + pos) + pos; // - pos + pos == 0 ? �����?
		return(Find - string);
	else
		return -1;
}

DWORD String::find(const char & c, DWORD pos) const
{
	char str[2];
	str[0] = c;
	str[1] = 0;
	return(find(str, pos));
}

DWORD String::find(const String & str, DWORD pos) const
{
	return(find(str.c_str(), pos));
}

DWORD String::Replace(const char * str, const char * replace, DWORD pos)
{
	DWORD PosBeginFmt = find(str, pos);
	DWORD StrLen = strlen(str);

	if(PosBeginFmt == ~0)
		return(~0);

	DWORD CountStrLen = strlen(replace) + length() - StrLen;
	char * CountStr = new char[CountStrLen + 1];

	//sprintf(CountStr,"%s%s%s",SubStr(0,PosBeginFmt),replace,SubStr(PosBeginFmt+StrLen,length() - (PosBeginFmt+StrLen)));
	memcpy(CountStr, string, PosBeginFmt);
	memcpy(CountStr + PosBeginFmt, replace, strlen(replace));
	memcpy(CountStr + PosBeginFmt + strlen(replace), string + PosBeginFmt + StrLen, length() - (PosBeginFmt + StrLen));
	SAFE_DELETE_A(string);
	string = CountStr;
	string[CountStrLen] = 0;

	return(PosBeginFmt);
}

DWORD String::Replace(const String & str, const String & replace, DWORD pos)
{
	return(Replace(str.c_str(), replace.c_str(), pos));
}

DWORD String::ReplaceAll(const char * str, const char * replace)
{
	DWORD PosCodeBegin = 0;
	DWORD PosCodeOld = 0;
	DWORD CountCodeFMT = 0;

	while(PosCodeBegin != ~0)
	{
		PosCodeBegin = find(str, PosCodeOld);
		if(PosCodeBegin != ~0)
		{
			CountCodeFMT++;
			PosCodeOld = PosCodeBegin + strlen(str);
		}
		else
			break;
	}


	DWORD StrLen = strlen(str);
	DWORD count_str_len = length() + (CountCodeFMT * (strlen(replace) - StrLen));

	//MessageBox(0,ToPointChar(ToString(length()) + "|" + ToString(CountCodeFMT) + "|" + ToString(StrLen)),ToPointChar(count_str_len),0);
	//MessageBox(0,string,ToPointChar(DWORD(strlen(string))),0);

	char * count_str = new char[count_str_len + 1];
	DWORD PosBeginFmt = 0;
	DWORD OldPosEndFmt = 0;
	DWORD count_str_last_len = 0;

	if((PosBeginFmt = find(str, OldPosEndFmt)) != DWORD(-1))
	{
		//sprintf(count_str,"%s%s",SubStr(OldPosEndFmt,PosBeginFmt-OldPosEndFmt),replace);
		//MessageBox(0,count_str,"count_str1",0);
		/*memcpy(count_str,string+OldPosEndFmt,count_str_last_len);
		count_str_last_len += PosBeginFmt-OldPosEndFmt;
		memcpy(count_str+count_str_last_len,replace,strlen(replace));
		count_str_last_len += strlen(replace);
		OldPosEndFmt = PosBeginFmt+StrLen;*/
		memcpy(count_str, string + OldPosEndFmt, PosBeginFmt - OldPosEndFmt);
		count_str_last_len += PosBeginFmt - OldPosEndFmt;
		memcpy(count_str + count_str_last_len, replace, strlen(replace));
		count_str_last_len += strlen(replace);
		OldPosEndFmt = PosBeginFmt + StrLen;
	}
	else
		return -1;

	//MessageBox(0,count_str,"count_str1",0);

	while((PosBeginFmt = find(str, OldPosEndFmt)) != DWORD(-1))
	{
		//sprintf(count_str,"%s%s%s",count_str,SubStr(OldPosEndFmt,PosBeginFmt-OldPosEndFmt),replace);
		//MessageBox(0,ToPointChar(PosBeginFmt),"PosBeginFmt",0);
		memcpy(count_str + count_str_last_len, string + OldPosEndFmt, PosBeginFmt - OldPosEndFmt);
		count_str_last_len += PosBeginFmt - OldPosEndFmt;

		memcpy(count_str + count_str_last_len, replace, strlen(replace));
		count_str_last_len += strlen(replace);

		OldPosEndFmt = PosBeginFmt + StrLen;
	}
	//MessageBox(0,SubStr(OldPosEndFmt,length()-OldPosEndFmt),"SubStr(OldPosEndFmt,length()-OldPosEndFmt)",0);
	//MessageBox(0,count_str,"count_str----",0);
	//sprintf(count_str,"%s%s",count_str,SubStr(OldPosEndFmt,length()-OldPosEndFmt));
	memcpy(count_str + count_str_last_len, string + OldPosEndFmt, length() - OldPosEndFmt);
	count_str[count_str_len] = 0;
	SAFE_DELETE_A(string);
	string = count_str;

	//MessageBox(0,count_str,"count_str++++++",0);

	return OldPosEndFmt - StrLen;
}

DWORD String::ReplaceAll(const String & str, const String & replace)
{
	return(ReplaceAll(str.c_str(), replace.c_str()));
}

/*char* String::SubStr(DWORD pos, DWORD lenght)
{
char *str = new char[lenght];
memcpy(str, string + pos, lenght);
str[lenght] = 0;
return str;
}*/

String String::substr(DWORD pos, DWORD len) const
{
	char * ns = string + pos;
	if(!len || len > length() - pos)
	{
		len = length() - pos;
	}
	char * s = new char[len + 1];
	memcpy(s, ns, len);
	s[len] = 0;
	String res = s;
	SAFE_DELETE_A(s);
	return(res);
}

UINT String::Delete(DWORD pos, DWORD len)
{
	/*char *str = SubStr(pos,lenght);
	char *OneSubStr = SubStr(0,pos);
	char *SecondSubStr = SubStr(pos + lenght,length() - (pos + lenght));
	char *newStr = new char[strlen(OneSubStr) + strlen(SecondSubStr)];
	sprintf(newStr,"%s%s",OneSubStr,SecondSubStr);
	string = newStr;
	return str;*/

	//char *str = SubStr(pos, lenght);

	DWORD PosLenght = pos + len;
	DWORD StrLen = length() - len;
	if(length() < len)
	{
		len = length() - pos;
		PosLenght = pos + len;
		StrLen = length() - len;
	}

	if(len == 0 || pos >= length())
	{
		return(0);
	}

	char * newStr = new char[StrLen + 1];
	//sprintf(string,"%s%s",SubStr(0,pos),SubStr(PosLenght,length() - PosLenght));
	memcpy(newStr, string, pos);
	memcpy(newStr + pos, string + PosLenght, length() - PosLenght);
	SAFE_DELETE_A(string);
	string = newStr;
	string[StrLen] = 0;

	return(len);
}

String String::trim()
{
	UINT pos = ~0;
	UINT len = 0;
	for(UINT i = 0, l = length(); i < l; ++i)
	{
		if(!isspace(string[i]))
		{
			if(i < pos)
			{
				pos = i;
			}
			len = i - pos + 1;
		}
	}
	return(substr(pos, len));
}

StringW StringW::trim()
{
	UINT pos = ~0;
	UINT len = 0;
	for(UINT i = 0, l = length(); i < l; ++i)
	{
		if(!iswspace(string[i]))
		{
			if(i < pos)
			{
				pos = i;
			}
			len = i - pos + 1;
		}
	}
	return(substr(pos, len));
}

////////////////////////////////////////////

int String::ToInt() const
{
	int out;
	sscanf(string, "%d", &out);
	return(out);
}

long String::ToLongInt() const
{
	long out;
	sscanf(string, "%ld", &out);
	return(out);
}

DWORD String::ToUnsLongInt() const
{
	DWORD num;
	sscanf(string, "%lu", &num);
	return(num);
}

double String::ToDouble() const
{
	float out;
	sscanf(string, "%f", &out);
	return(out);
}

bool String::ToBool() const
{
	return(ToInt() == 1 || stricmp(string, "true") == 0);
}

DWORD String::find_last_of(const char * str, DWORD pos) const
{

	char * Find = NULL;
	char * tmpFind = NULL;
	while((Find = strstr(string + pos, str)))
	{
		pos = Find - string + 1;
		tmpFind = Find;
	}

	if(tmpFind != NULL)
		// return Find - (string + pos) + pos; // - pos + pos == 0 ? �����?
		return(tmpFind - string);
	else
		return -1;
}

DWORD String::find_last_of(const char & c, DWORD pos) const
{
	char str[2];
	str[0] = c;
	str[1] = 0;
	return(find_last_of(str, pos));
}

DWORD String::find_last_of(const String & str, DWORD pos) const
{
	return(find_last_of(str.c_str(), pos));
}


bool operator<(const String & a, const String & b)
{
	return(strcmp(a.c_str(), b.c_str()) < 0);
}

////////////////////////////////////////////////////////
//
//
//
//
////////////////////////////////////////////////////////


const WCHAR * StringW::c_str() const
{
	return(string);
}

StringW::StringW()
{
	string = new WCHAR[1];
	string[0] = 0;
}

StringW::StringW(const WCHAR * str)
{

	string = new WCHAR[wcslen(str) + 1];
	//sprintf(string,"%s",str);
	memcpy(string, str, sizeof(WCHAR) * wcslen(str));
	string[wcslen(str)] = 0;
}

StringW::StringW(WCHAR sym)
{
	string = new WCHAR[2];
	//sprintf(string,"%s",&sym);
	string[0] = sym;
	string[1] = 0;
}

StringW::StringW(int num)
{
	WCHAR * tmp = new WCHAR[64];
	swprintf(tmp, 64, L"%d", num);
	string = new WCHAR[wcslen(tmp) + 1];
	memcpy(string, tmp, sizeof(WCHAR) * (wcslen(tmp) + 1));
	SAFE_DELETE_A(tmp);
}

StringW::StringW(WORD num)
{
	WCHAR * tmp = new WCHAR[64];
	swprintf(tmp, 64, L"%hu", num);
	string = new WCHAR[wcslen(tmp) + 1];
	memcpy(string, tmp, sizeof(WCHAR) * (wcslen(tmp) + 1));
	SAFE_DELETE_A(tmp);
}

StringW::StringW(DWORD num)
{
	WCHAR * tmp = new WCHAR[64];
	swprintf(tmp, 64, L"%lu", num);
	string = new WCHAR[wcslen(tmp) + 1];
	memcpy(string, tmp, sizeof(WCHAR) * (wcslen(tmp) + 1));
	SAFE_DELETE_A(tmp);
}

StringW::StringW(long num)
{
	WCHAR * tmp = new WCHAR[64];
	swprintf(tmp, 64, L"%ld", num);
	string = new WCHAR[wcslen(tmp) + 1];
	memcpy(string, tmp, sizeof(WCHAR) * (wcslen(tmp) + 1));
	SAFE_DELETE_A(tmp);
}

StringW::StringW(double num)
{
	WCHAR * tmp = new WCHAR[64];
	swprintf(tmp, 64, L"%G", num);
	string = new WCHAR[wcslen(tmp) + 1];
	memcpy(string, tmp, sizeof(WCHAR) * (wcslen(tmp) + 1));
	SAFE_DELETE_A(tmp);
}

StringW::StringW(bool bf)
{
	string = new WCHAR[bf ? 5 : 6];
	swprintf(string, bf ? 5 : 6, L"%ls", bf ? L"true" : L"false");
}

StringW::StringW(const StringW & str)
{
	string = new WCHAR[str.length() + 1];
	memcpy(string, str.c_str(), sizeof(WCHAR) * (str.length() + 1));
}

StringW::StringW(const StringW * str)
{
	string = new WCHAR[str->length() + 1];
	memcpy(string, str->c_str(), sizeof(WCHAR) * (str->length() + 1));
}

StringW::StringW(StringW && other)
{
	//SAFE_DELETE_A(string);
	string = other.string;
	other.string = NULL;
}

StringW::~StringW()
{
	Release();
}


void StringW::Release()
{
	SAFE_DELETE_A(string);
}

////////////////////////////////////////////

StringW StringW::operator+(const StringW &str) const
{
	size_t len = length() + str.length() + 1;
	WCHAR * ns = new WCHAR[len];
	swprintf(ns, len, L"%ls%ls", string, str.c_str());
	StringW newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

StringW StringW::operator+(const StringW *str)
{
	WCHAR * ns = new WCHAR[length() + str->length() + 1];
	swprintf(ns, length() + str->length() + 1, L"%ls%ls", string, str->c_str());
	StringW newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

StringW StringW::operator+(const WCHAR * str)
{
	WCHAR * ns = new WCHAR[length() + wcslen(str) + 1];
	swprintf(ns, length() + wcslen(str) + 1, L"%ls%ls", string, str);
	StringW newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

StringW StringW::operator+(const WCHAR & sym)
{
	WCHAR * ns = new WCHAR[length() + 2];
	swprintf(ns, length() + 2, L"%ls%lc", string, sym);
	StringW newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

StringW StringW::operator+(const int & num)
{
	WCHAR * ns = new WCHAR[length() + 64];
	swprintf(ns, length() + 64, L"%ls%d", string, num);
	StringW newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

StringW StringW::operator+(const WORD & num)
{
	WCHAR * ns = new WCHAR[length() + 64];
	swprintf(ns, length() + 64, L"%ls%hu", string, num);
	StringW newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

StringW StringW::operator+(const DWORD & num)
{
	WCHAR * ns = new WCHAR[length() + 64];
	swprintf(ns, length() + 64, L"%ls%lu", string, num);
	StringW newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

StringW StringW::operator+(const long & num)
{
	WCHAR * ns = new WCHAR[length() + 64];
	swprintf(ns, length() + 64, L"%ls%ld", string, num);
	StringW newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

StringW StringW::operator+(const double & num)
{
	WCHAR * ns = new WCHAR[length() + 64];
	swprintf(ns, length() + 64, L"%ls%G", string, num);
	StringW newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

StringW StringW::operator+(const bool & bf)
{
	WCHAR * ns = new WCHAR[length() + (bf ? 5 : 6)];
	swprintf(ns, length() + (bf ? 5 : 6), L"%ls%ls", string, bf ? L"true" : L"false");
	StringW newSXStr = ns;
	SAFE_DELETE_A(ns);
	return(newSXStr);
}

//////////////////////////////////////////////////////


StringW & StringW::operator=(const StringW & str)
{
	Release();
	string = new WCHAR[str.length() + 1];
	memcpy(string, str.c_str(), sizeof(WCHAR) * (str.length() + 1));
	return(*this);
}

StringW & StringW::operator=(const StringW * str)
{
	Release();
	string = new WCHAR[str->length() + 1];
	memcpy(string, str->c_str(), sizeof(WCHAR) * (str->length() + 1));
	return(*this);
}

StringW & StringW::operator=(const WCHAR * str)
{
	Release();
	string = new WCHAR[wcslen(str) + 1];
	memcpy(string, str, sizeof(WCHAR) * (wcslen(str) + 1));
	return(*this);
}

StringW & StringW::operator=(const WCHAR & sym)
{
	Release();
	string = new WCHAR[2];
	string[0] = sym;
	string[1] = 0;
	return(*this);
}

StringW & StringW::operator=(const int & num)
{
	Release();
	WCHAR * tmp = new WCHAR[64];
	swprintf(tmp, 64, L"%d", num);
	string = new WCHAR[wcslen(tmp) + 1];
	memcpy(string, tmp, sizeof(WCHAR) * (wcslen(tmp) + 1));
	SAFE_DELETE_A(tmp);
	return(*this);
}

StringW & StringW::operator=(const WORD & num)
{
	Release();
	WCHAR * tmp = new WCHAR[64];
	swprintf(tmp, 64, L"%hu", num);
	string = new WCHAR[wcslen(tmp) + 1];
	memcpy(string, tmp, sizeof(WCHAR) * (wcslen(tmp) + 1));
	SAFE_DELETE_A(tmp);
	return(*this);
}

StringW & StringW::operator=(const DWORD & num)
{
	Release();
	WCHAR * tmp = new WCHAR[64];
	swprintf(tmp, 64, L"%lu", num);
	string = new WCHAR[wcslen(tmp) + 1];
	memcpy(string, tmp, sizeof(WCHAR) * (wcslen(tmp) + 1));
	SAFE_DELETE_A(tmp);
	return(*this);
}

StringW & StringW::operator=(const long & num)
{
	Release();
	WCHAR * tmp = new WCHAR[64];
	swprintf(tmp, 64, L"%ld", num);
	string = new WCHAR[wcslen(tmp) + 1];
	memcpy(string, tmp, sizeof(WCHAR) * (wcslen(tmp) + 1));
	SAFE_DELETE_A(tmp);
	return(*this);
}

StringW & StringW::operator=(const double & num)
{
	Release();
	WCHAR * tmp = new WCHAR[64];
	swprintf(tmp, 64, L"%G", num);
	string = new WCHAR[wcslen(tmp) + 1];
	memcpy(string, tmp, sizeof(WCHAR) * (wcslen(tmp) + 1));
	SAFE_DELETE_A(tmp);
	return(*this);
}

StringW & StringW::operator=(const bool & bf)
{
	Release();
	string = new WCHAR[bf ? 5 : 6];
	swprintf(string, bf ? 5 : 6, L"%ls", bf ? L"true" : L"false");
	return(*this);
}

StringW & StringW::operator=(StringW && other)
{
	if(this != &other)
	{
		SAFE_DELETE(string);
		string = other.string;
		other.string = NULL;
	}
	return(*this);
}

///////////////////////////////////////////////////


StringW & StringW::operator+=(const StringW & str)
{
	WCHAR * ns = new WCHAR[length() + str.length() + 1];
	swprintf(ns, length() + str.length() + 1, L"%ls%ls", string, str.c_str());
	SAFE_DELETE_A(string);
	string = ns;
	return(*this);
}

StringW & StringW::operator+=(const StringW * str)
{
	WCHAR * ns = new WCHAR[length() + str->length() + 1];
	swprintf(ns, length() + str->length() + 1, L"%ls%ls", string, str->c_str());
	SAFE_DELETE_A(string);
	string = ns;
	return(*this);
}

StringW & StringW::operator+=(const WCHAR * str)
{
	WCHAR * newstring = new WCHAR[length() + wcslen(str) + 1];
	swprintf(newstring, length() + wcslen(str) + 1, L"%ls%ls", string, str);
	SAFE_DELETE_A(string);
	string = newstring;
	return(*this);
}

StringW & StringW::operator+=(const WCHAR & sym)
{
	WCHAR * newstring = new WCHAR[length() + 2];
	swprintf(newstring, length() + 2, L"%ls%lc", string, sym);
	SAFE_DELETE_A(string);
	string = newstring;
	return(*this);
}

StringW & StringW::operator+=(const int & num)
{
	WCHAR * tmp = new WCHAR[length() + 64];
	swprintf(tmp, length() + 64, L"%ls%d", string, num);
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

StringW & StringW::operator+=(const WORD & num)
{
	WCHAR * tmp = new WCHAR[length() + 64];
	swprintf(tmp, length() + 64, L"%ls%hu", string, num);
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

StringW & StringW::operator+=(const DWORD & num)
{
	WCHAR * tmp = new WCHAR[length() + 64];
	swprintf(tmp, length() + 64, L"%ls%lu", string, num);
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

StringW & StringW::operator+=(const long & num)
{
	WCHAR * tmp = new WCHAR[length() + 64];
	swprintf(tmp, length() + 64, L"%ls%ld", string, num);
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

StringW & StringW::operator+=(const double & num)
{
	WCHAR * tmp = new WCHAR[length() + 64];
	swprintf(tmp, length() + 64, L"%ls%G", string, num);
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

StringW & StringW::operator+=(const bool & bf)
{
	WCHAR * tmp = new WCHAR[length() + (bf ? 5 : 6)];
	swprintf(tmp, length() + (bf ? 5 : 6), L"%ls%ls", string, bf ? L"true" : L"false");
	SAFE_DELETE_A(string);
	string = tmp;
	return(*this);
}

/////////////////////////////////////////////////////////

StringW StringW::operator-(const StringW & str)
{
	StringW newStr = this;
	newStr.Replace(str, "", 0);
	return(newStr);
}

StringW StringW::operator-(const StringW * str)
{
	StringW newStr = this;
	newStr.Replace(str, "", 0);
	return(newStr);
}

StringW StringW::operator-(const WCHAR * str)
{
	StringW newStr = this;
	newStr.Replace(str, "", 0);
	return(newStr);
}

StringW StringW::operator-(const WCHAR & sym)
{
	StringW newStr = this;
	newStr.Replace(sym, "", 0);
	return(newStr);
}

StringW StringW::operator-(const int & num)
{
	StringW newStr = this;
	newStr.Replace(num, "", 0);
	return(newStr);
}

StringW StringW::operator-(const WORD & num)
{
	StringW newStr = this;
	newStr.Replace(num, "", 0);
	return(newStr);
}

StringW StringW::operator-(const DWORD & num)
{
	StringW newStr = this;
	newStr.Replace(num, "", 0);
	return(newStr);
}

StringW StringW::operator-(const long & num)
{
	StringW newStr = this;
	newStr.Replace(num, "", 0);
	return(newStr);
}

StringW StringW::operator-(const double & num)
{
	StringW newStr = this;
	newStr.Replace(num, "", 0);
	return(newStr);
}

StringW StringW::operator-(const bool & bf)
{
	StringW newStr = this;
	newStr.Replace(bf, "", 0);
	return(newStr);
}

/////////////////////////////////////////////////////////

StringW & StringW::operator-=(const StringW & str)
{
	Replace(str, "", 0);
	return(*this);
}

StringW & StringW::operator-=(const StringW * str)
{
	Replace(str, "", 0);
	return(*this);
}

StringW & StringW::operator-=(const WCHAR * str)
{
	Replace(str, "", 0);
	return(*this);
}

StringW & StringW::operator-=(const WCHAR & sym)
{
	Replace(sym, "", 0);
	return(*this);
}

StringW & StringW::operator-=(const int & num)
{
	Replace(num, "", 0);
	return(*this);
}

StringW & StringW::operator-=(const WORD & num)
{
	Replace(num, "", 0);
	return(*this);
}

StringW & StringW::operator-=(const DWORD & num)
{
	Replace(num, "", 0);
	return(*this);
}

StringW & StringW::operator-=(const long & num)
{
	Replace(num, "", 0);
	return(*this);
}

StringW & StringW::operator-=(const double & num)
{
	Replace(num, "", 0);
	return(*this);
}

StringW & StringW::operator-=(const bool & bf)
{
	Replace(bf, "", 0);
	return(*this);
}

/////////////////////////////////////////////////////////

StringW StringW::operator/(const StringW & str)
{
	StringW newStr = this;
	newStr.ReplaceAll(str, L"");
	return(newStr);
}

StringW StringW::operator/(const StringW * str)
{
	StringW newStr = this;
	newStr.ReplaceAll(str, L"");
	return(newStr);
}

StringW StringW::operator/(const WCHAR * str)
{
	StringW newStr = this;
	newStr.ReplaceAll(str, L"");
	return(newStr);
}

StringW StringW::operator/(const WCHAR & sym)
{
	StringW newStr = this;
	newStr.ReplaceAll(sym, L"");
	return(newStr);
}

StringW StringW::operator/(const int & num)
{
	StringW newStr = this;
	newStr.ReplaceAll(num, L"");
	return(newStr);
}

StringW StringW::operator/(const WORD & num)
{
	StringW newStr = this;
	newStr.ReplaceAll(num, L"");
	return(newStr);
}

StringW StringW::operator/(const DWORD & num)
{
	StringW newStr = this;
	newStr.ReplaceAll(num, L"");
	return(newStr);
}

StringW StringW::operator/(const long & num)
{
	StringW newStr = this;
	newStr.ReplaceAll(num, L"");
	return(newStr);
}

StringW StringW::operator/(const double & num)
{
	StringW newStr = this;
	newStr.ReplaceAll(num, L"");
	return(newStr);
}

StringW StringW::operator/(const bool & bf)
{
	StringW newStr = this;
	newStr.ReplaceAll(bf, L"");
	return(newStr);
}

/////////////////////////////////////////////////////////

StringW & StringW::operator/=(const StringW & str)
{
	ReplaceAll(str, L"");
	return(*this);
}

StringW & StringW::operator/=(const StringW * str)
{
	ReplaceAll(str, L"");
	return(*this);
}

StringW & StringW::operator/=(const WCHAR * str)
{
	ReplaceAll(str, L"");
	return(*this);
}

StringW & StringW::operator/=(const WCHAR & sym)
{
	ReplaceAll(sym, L"");
	return(*this);
}

StringW & StringW::operator/=(const int & num)
{
	ReplaceAll(num, L"");
	return(*this);
}

StringW & StringW::operator/=(const WORD & num)
{
	ReplaceAll(num, L"");
	return(*this);
}

StringW & StringW::operator/=(const DWORD & num)
{
	ReplaceAll(num, L"");
	return(*this);
}

StringW & StringW::operator/=(const long & num)
{
	ReplaceAll(num, L"");
	return(*this);
}

StringW & StringW::operator/=(const double & num)
{
	ReplaceAll(num, L"");
	return(*this);
}

StringW & StringW::operator/=(const bool & bf)
{
	ReplaceAll(bf, L"");
	return(*this);
}

///////////////////////////////////////////////////////

bool StringW::operator==(const StringW & str) const
{
	/*if(this->length() == str.length())
	{
	DWORD len = this->length();
	for(DWORD i = 0; i<len; i++)
	{
	if(this->string[i] != str.string[i])
	return false;
	}
	}
	else
	return false;

	return true;*/
	return(operator==(&str));
}

bool StringW::operator==(const StringW * str) const
{
	if(length() == str->length())
	{
		DWORD len = length();
		for(DWORD i = 0; i<len; i++)
		{
			if(string[i] != str->string[i])
				return false;
		}
	}
	else
		return false;

	return true;
}

bool StringW::operator==(const WCHAR * str) const
{
	/*WCHAR *newstring = new WCHAR[strlen(str)];
	sprintf(newstring, "%s", str);
	if(*this == newstring)
	return true;
	else
	return false;
	*/
	if(length() == wcslen(str))
	{
		DWORD len = length();
		for(DWORD i = 0; i<len; i++)
		{
			if(string[i] != str[i])
				return false;
		}
	}
	else
		return false;

	return true;
}

bool StringW::operator==(const WCHAR & sym) const
{
	if(length() == 1)
	{
		if(string[0] != sym)
			return false;
	}
	else
		return false;

	return true;
}

bool StringW::operator==(const int & num) const
{
	/*int Value = ToInt();
	if(Value == num)
	return true;
	else
	return false;*/
	return(operator==((long)num));
}

bool StringW::operator==(const WORD & num) const
{
	return(operator==((DWORD)num));
}

bool StringW::operator==(const DWORD & num) const
{
	DWORD Value = ToUnsLongInt();
	return(Value == num);
}

bool StringW::operator==(const long & num) const
{
	long Value = ToLongInt();
	return(Value == num);
}

bool StringW::operator==(const double & num) const
{
	double Value = ToDouble();
	return(Value == num);
}

bool StringW::operator==(const bool & bf) const
{
	bool Value = ToBool();
	return(Value == bf);
}

/////////////////////////////////////////////////////////

bool StringW::operator!=(const StringW & str) const
{
	if(*this == str)
		return false;
	else
		return true;
}

bool StringW::operator!=(const StringW * str) const
{
	if(*this == *str)
		return false;
	else
		return true;
}

bool StringW::operator!=(const WCHAR * str) const
{
	if(*this == str)
		return false;
	else
		return true;
}

bool StringW::operator!=(const WCHAR & sym) const
{
	if(*this == sym)
		return false;
	else
		return true;
}

bool StringW::operator!=(const int & num) const
{
	if(*this == num)
		return false;
	else
		return true;
}

bool StringW::operator!=(const WORD & num) const
{
	if(*this == num)
		return false;
	else
		return true;
}

bool StringW::operator!=(const DWORD & num) const
{
	if(*this == num)
		return false;
	else
		return true;
}

bool StringW::operator!=(const long & num) const
{
	if(*this == num)
		return false;
	else
		return true;
}

bool StringW::operator!=(const double & num) const
{
	if(*this == num)
		return false;
	else
		return true;
}

bool StringW::operator!=(const bool & bf) const
{
	if(*this == bf)
		return false;
	else
		return true;
}

/////////////////////////////////////////////////////////

WCHAR & StringW::operator[](const DWORD & num)
{
	return(string[num]);
}

const WCHAR & StringW::operator[](const DWORD & num) const
{
	return(string[num]);
}


////////////////////////////////////////////////////////

DWORD StringW::length() const
{
	return(wcslen(string));
}

DWORD StringW::find(const WCHAR * str, DWORD pos) const
{
	//DWORD BeginPos = pos;
	//WCHAR *newWCHAR = new WCHAR[strlen(string+BeginPos)];
	//sprintf(newWCHAR,"%s",string+BeginPos);

	WCHAR *Find = wcsstr(string + pos, str);

	if(Find != NULL)
		// return Find - (string + pos) + pos; // - pos + pos == 0 ? �����?
		return(Find - string);
	else
		return -1;
}

DWORD StringW::find(const WCHAR & c, DWORD pos) const
{
	WCHAR str[2];
	str[0] = c;
	str[1] = 0;
	return(find(str, pos));
}

DWORD StringW::find(const StringW & str, DWORD pos) const
{
	return(find(str.c_str(), pos));
}

DWORD StringW::Replace(const WCHAR * str, const WCHAR * replace, DWORD pos)
{
	DWORD PosBeginFmt = find(str, pos);
	DWORD StrLen = wcslen(str);

	if(PosBeginFmt == ~0)
		return(~0);

	DWORD CountStrLen = wcslen(replace) + length() - StrLen;
	WCHAR * CountStr = new WCHAR[CountStrLen + 1];

	//sprintf(CountStr,"%s%s%s",SubStr(0,PosBeginFmt),replace,SubStr(PosBeginFmt+StrLen,length() - (PosBeginFmt+StrLen)));
	memcpy(CountStr, string, PosBeginFmt);
	memcpy(CountStr + PosBeginFmt, replace, sizeof(WCHAR) * wcslen(replace));
	memcpy(CountStr + PosBeginFmt + wcslen(replace), string + PosBeginFmt + StrLen, sizeof(WCHAR) * (length() - (PosBeginFmt + StrLen)));
	SAFE_DELETE_A(string);
	string = CountStr;
	string[CountStrLen] = 0;

	return(PosBeginFmt);
}

DWORD StringW::Replace(const StringW & str, const StringW & replace, DWORD pos)
{
	return(Replace(str.c_str(), replace.c_str(), pos));
}

DWORD StringW::ReplaceAll(const WCHAR * str, const WCHAR * replace)
{
	DWORD PosCodeBegin = 0;
	DWORD PosCodeOld = 0;
	DWORD CountCodeFMT = 0;

	while(PosCodeBegin != ~0)
	{
		PosCodeBegin = find(str, PosCodeOld);
		if(PosCodeBegin != ~0)
		{
			CountCodeFMT++;
			PosCodeOld = PosCodeBegin + wcslen(str);
		}
		else
			break;
	}


	DWORD StrLen = wcslen(str);
	DWORD count_str_len = length() + (CountCodeFMT * (wcslen(replace) - StrLen));

	//MessageBox(0,ToPointWCHAR(ToString(length()) + "|" + ToString(CountCodeFMT) + "|" + ToString(StrLen)),ToPointWCHAR(count_str_len),0);
	//MessageBox(0,string,ToPointWCHAR(DWORD(strlen(string))),0);

	WCHAR * count_str = new WCHAR[count_str_len + 1];
	DWORD PosBeginFmt = 0;
	DWORD OldPosEndFmt = 0;
	DWORD count_str_last_len = 0;

	if((PosBeginFmt = find(str, OldPosEndFmt)) != DWORD(-1))
	{
		//sprintf(count_str,"%s%s",SubStr(OldPosEndFmt,PosBeginFmt-OldPosEndFmt),replace);
		//MessageBox(0,count_str,"count_str1",0);
		/*memcpy(count_str,string+OldPosEndFmt,count_str_last_len);
		count_str_last_len += PosBeginFmt-OldPosEndFmt;
		memcpy(count_str+count_str_last_len,replace,strlen(replace));
		count_str_last_len += strlen(replace);
		OldPosEndFmt = PosBeginFmt+StrLen;*/
		memcpy(count_str, string + OldPosEndFmt, sizeof(WCHAR) * (PosBeginFmt - OldPosEndFmt));
		count_str_last_len += PosBeginFmt - OldPosEndFmt;
		memcpy(count_str + count_str_last_len, replace, sizeof(WCHAR) * wcslen(replace));
		count_str_last_len += wcslen(replace);
		OldPosEndFmt = PosBeginFmt + StrLen;
	}
	else
		return -1;

	//MessageBox(0,count_str,"count_str1",0);

	while((PosBeginFmt = find(str, OldPosEndFmt)) != DWORD(-1))
	{
		//sprintf(count_str,"%s%s%s",count_str,SubStr(OldPosEndFmt,PosBeginFmt-OldPosEndFmt),replace);
		//MessageBox(0,ToPointWCHAR(PosBeginFmt),"PosBeginFmt",0);
		memcpy(count_str + count_str_last_len, string + OldPosEndFmt, sizeof(WCHAR) * (PosBeginFmt - OldPosEndFmt));
		count_str_last_len += PosBeginFmt - OldPosEndFmt;

		memcpy(count_str + count_str_last_len, replace, sizeof(WCHAR) * wcslen(replace));
		count_str_last_len += wcslen(replace);

		OldPosEndFmt = PosBeginFmt + StrLen;
	}
	//MessageBox(0,SubStr(OldPosEndFmt,length()-OldPosEndFmt),"SubStr(OldPosEndFmt,length()-OldPosEndFmt)",0);
	//MessageBox(0,count_str,"count_str----",0);
	//sprintf(count_str,"%s%s",count_str,SubStr(OldPosEndFmt,length()-OldPosEndFmt));
	memcpy(count_str + count_str_last_len, string + OldPosEndFmt, sizeof(WCHAR) * (length() - OldPosEndFmt));
	count_str[count_str_len] = 0;
	SAFE_DELETE_A(string);
	string = count_str;

	//MessageBox(0,count_str,"count_str++++++",0);

	return OldPosEndFmt - StrLen;
}

DWORD StringW::ReplaceAll(const StringW & str, const StringW & replace)
{
	return(ReplaceAll(str.c_str(), replace.c_str()));
}

/*WCHAR* StringW::SubStr(DWORD pos, DWORD lenght)
{
WCHAR *str = new WCHAR[lenght];
memcpy(str, string + pos, lenght);
str[lenght] = 0;
return str;
}*/

StringW StringW::substr(DWORD pos, DWORD len) const
{
	WCHAR * ns = string + pos;
	if(!len || len > length() - pos)
	{
		len = length() - pos;
	}
	WCHAR * s = new WCHAR[len + 1];
	memcpy(s, ns, sizeof(WCHAR) * len);
	s[len] = 0;
	StringW res = s;
	SAFE_DELETE_A(s);
	return(res);
}

UINT StringW::Delete(DWORD pos, DWORD len)
{
	/*WCHAR *str = SubStr(pos,lenght);
	WCHAR *OneSubStr = SubStr(0,pos);
	WCHAR *SecondSubStr = SubStr(pos + lenght,length() - (pos + lenght));
	WCHAR *newStr = new WCHAR[strlen(OneSubStr) + strlen(SecondSubStr)];
	sprintf(newStr,"%s%s",OneSubStr,SecondSubStr);
	string = newStr;
	return str;*/

	//WCHAR *str = SubStr(pos, lenght);

	DWORD PosLenght = pos + len;
	DWORD StrLen = length() - len;
	if(length() < len)
	{
		len = length() - pos;
		PosLenght = pos + len;
		StrLen = length() - len;
	}

	if(len == 0 || pos >= length())
	{
		return(0);
	}

	WCHAR * newStr = new WCHAR[StrLen + 1];
	//sprintf(string,"%s%s",SubStr(0,pos),SubStr(PosLenght,length() - PosLenght));
	memcpy(newStr, string, sizeof(WCHAR) * pos);
	memcpy(newStr + pos, string + PosLenght, sizeof(WCHAR) * (length() - PosLenght));
	SAFE_DELETE_A(string);
	string = newStr;
	string[StrLen] = 0;

	return(len);
}

////////////////////////////////////////////

int StringW::ToInt() const
{
	int tmp;
	swscanf(string, L"%d", &tmp);
	return(tmp);
}

long StringW::ToLongInt() const
{
	long tmp;
	swscanf(string, L"%ld", &tmp);
	return(tmp);
}

DWORD StringW::ToUnsLongInt() const
{
	DWORD num;
	swscanf(string, L"%lu", &num);
	return(num);
}

double StringW::ToDouble() const
{
	//return(atof(string));
	double tmp;
	swscanf(string, L"%G", &tmp);
	return(tmp);
}

bool StringW::ToBool() const
{
	return(ToInt() == 1 || wcscmpi(string, L"true") == 0);
}

DWORD StringW::find_last_of(const WCHAR * str, DWORD pos) const
{

	WCHAR * Find = NULL;
	WCHAR * tmpFind = NULL;
	while((Find = wcsstr(string + pos, str)))
	{
		pos = Find - string + 1;
		tmpFind = Find;
	}

	if(tmpFind != NULL)
		// return Find - (string + pos) + pos; // - pos + pos == 0 ? �����?
		return(tmpFind - string);
	else
		return -1;
}

DWORD StringW::find_last_of(const WCHAR & c, DWORD pos) const
{
	WCHAR str[2];
	str[0] = c;
	str[1] = 0;
	return(find_last_of(str, pos));
}

DWORD StringW::find_last_of(const StringW & str, DWORD pos) const
{
	return(find_last_of(str.c_str(), pos));
}

void StringW::insert(UINT pos, const StringW & data)
{
	insert(pos, data.c_str());
}

void StringW::insert(UINT pos, const WCHAR * data)
{
	WCHAR * tmpStr = new WCHAR[length() + wcslen(data) + 1];
	memcpy(tmpStr, string, sizeof(WCHAR) * pos);
	memcpy(tmpStr + pos, data, sizeof(WCHAR) * wcslen(data));
	memcpy(tmpStr + pos + wcslen(data), string + pos, sizeof(WCHAR) * (length() - pos));
	tmpStr[wcslen(data) + length()] = 0;
	SAFE_DELETE_A(string);
	string = tmpStr;
}


/*bool operator<(const StringW & a, const StringW & b)
{
	return(wcscmp(a.c_str(), b.c_str()) < 0);
}*/

String::operator StringW() const
{
	StringW dst;
	dst.Reserve(length());
#if defined(_WINDOWS)
	MultiByteToWideChar(1251, CP_ACP, string, length() + 1, dst.string, length() + 1);
#else
	mbstowcs(dst.string, string, length() + 1);
#endif
	return(dst);
}

StringW::operator String() const
{
	String dst;
	dst.Reserve(length());
#if defined(_WINDOWS)
	WideCharToMultiByte(1251, CP_ACP, string, length() + 1, dst.string, length() + 1, " ", NULL);
#else
	wcstombs(dst.string, string, sizeof(WCHAR) * (length() + 1));
#endif
	return(dst);
}

void String::Reserve(int length)
{
	SAFE_DELETE_A(string);
	string = new char[length + 1];
}

void StringW::Reserve(int length)
{
	SAFE_DELETE_A(string);
	string = new WCHAR[length + 1];
}
/*
StringW operator+(const WCHAR * a, const StringW & b)
{
	return(StringW(a) + b);
}
StringW operator+(const StringW & a, const WCHAR * b)
{
	return(a + StringW(b));
}*/


/*
#ifdef _DBG_MEM
#	define new DS_NEW
#endif*/

#if defined(_WINDOWS)
#	pragma warning(default:4996)
#endif
#if defined(_LINUX) || defined(_MAC)
#	pragma GCC diagnostic pop
#endif
