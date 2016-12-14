
#include <core\string.h>

#pragma once

#pragma warning(disable:4996)


		long StrFind(const char* src_str, char *str, DWORD pos)
		{
			const char *Find = strstr(src_str + pos, str);

			if(Find != NULL)
				return Find - (src_str + pos) + pos;
			else
				return -1;
		}

		long StrGetStr(const char* src_str, char *str,long size, unsigned long pos)
		{
			DWORD nstr = StrFind(src_str,"\n",pos);
			long Size = strlen(src_str);
			bool IsEndFile = false;
				if(nstr == -1)
				{
					IsEndFile = true;
					nstr = Size;
				}

				if(nstr != -1 && nstr-pos <= size)
				{
					memcpy((void*)str,src_str + pos,nstr-pos);
					str[nstr-pos] = 0;
				}
				else if(nstr != -1 && nstr-pos > size)
				{
					memcpy((void*)str,src_str + pos,size);
					str[size] = 0;
				}

				if(IsEndFile)
					return Size;
				else
					return nstr + strlen("\n");
		}

		long StrReplace(char* src_str, char *str, char *replace, DWORD pos)
		{
			DWORD PosBeginFmt = StrFind(src_str, str, pos);
			DWORD StrLen = strlen(str);

			if(PosBeginFmt == -1)
				return -1;

			DWORD CountStrLen = PosBeginFmt + strlen(replace) + strlen(src_str) - (PosBeginFmt + StrLen);
			char *CountStr = new char[CountStrLen];

			//sprintf(CountStr,"%s%s%s",SubStr(0,PosBeginFmt),replace,SubStr(PosBeginFmt+StrLen,length() - (PosBeginFmt+StrLen)));
			memcpy(CountStr, src_str, PosBeginFmt);
			memcpy(CountStr + PosBeginFmt, replace, strlen(replace));
			memcpy(CountStr + PosBeginFmt + strlen(replace), src_str + PosBeginFmt + StrLen, strlen(src_str) - (PosBeginFmt + StrLen));
			src_str = CountStr;
			src_str[CountStrLen] = 0;

			return PosBeginFmt;
		}

		long StrReplaceAll(char* src_str, char *str, char *replace)
		{
			DWORD PosCodeBegin = 0;
			DWORD PosCodeOld = 0;
			DWORD CountCodeFMT = 0;

			while(PosCodeBegin != -1)
			{
				PosCodeBegin = StrFind(src_str, str, PosCodeOld);
				if(PosCodeBegin != -1)
				{
					CountCodeFMT++;
					PosCodeOld = PosCodeBegin + strlen(str);
				}
				else
					break;
			}

			DWORD StrLen = strlen(str);
			DWORD count_str_len = strlen(src_str) + (CountCodeFMT * (strlen(replace) - StrLen));

			char* count_str = new char[count_str_len];
			DWORD PosBeginFmt = 0;
			DWORD OldPosEndFmt = 0;
			DWORD count_str_last_len = 0;

			if((PosBeginFmt = StrFind(src_str, str, OldPosEndFmt)) != DWORD(-1))
			{
				memcpy(count_str, src_str + OldPosEndFmt, PosBeginFmt - OldPosEndFmt);
				count_str_last_len += PosBeginFmt - OldPosEndFmt;
				memcpy(count_str + count_str_last_len, replace, strlen(replace));
				count_str_last_len += strlen(replace);
				OldPosEndFmt = PosBeginFmt + StrLen;
			}
			else
				return -1;

			while((PosBeginFmt = StrFind(src_str, str, OldPosEndFmt)) != DWORD(-1))
			{
				memcpy(count_str + count_str_last_len, src_str + OldPosEndFmt, PosBeginFmt - OldPosEndFmt);
				count_str_last_len += PosBeginFmt - OldPosEndFmt;

				memcpy(count_str + count_str_last_len, replace, strlen(replace));
				count_str_last_len += strlen(replace);

				OldPosEndFmt = PosBeginFmt + StrLen;
			}

			memcpy(count_str + count_str_last_len, src_str + OldPosEndFmt, strlen(src_str) - OldPosEndFmt);
			count_str[count_str_len] = 0;
			src_str = count_str;

			return OldPosEndFmt - StrLen;
		}

		char* StrSubStr(const char* src_str, DWORD pos, DWORD lenght)
		{
			char *str = new char[lenght];
			memcpy(str, src_str + pos, lenght);
			str[lenght] = 0;
			return str;
		}

		char* StrDelete(char* src_str, DWORD pos, DWORD lenght)
		{
			char *str = StrSubStr(src_str, pos, lenght);
			DWORD PosLenght = pos + lenght;
			DWORD StrLen = pos + (strlen(src_str) - PosLenght);
			char* newStr = new char[StrLen];

			memcpy(newStr, src_str, pos);
			memcpy(newStr + pos, src_str + PosLenght, strlen(src_str) - PosLenght);
			src_str = newStr;
			src_str[StrLen] = 0;

			return str;
		}
		


		inline bool StrValidate(const char* str)
		{	
				if(str && str[0]!=0 && str[0]!='0')
					return true;
			return false;
		}

		void StrCutMesh(const char* path,char* name)
		{
				for(int i=0;i<strlen(path);i++)
				{
						if(path[strlen(path)-i] == '\\')
						{
								if(i+6 > 0 && i+6 < strlen(path) && 
									(path[(strlen(path)-i)-1] == 's' || path[(strlen(path)-i)-1] == 'S') &&
									(path[(strlen(path)-i)-2] == 'e' || path[(strlen(path)-i)-2] == 'E') &&
									(path[(strlen(path)-i)-3] == 'h' || path[(strlen(path)-i)-3] == 'H') &&
									(path[(strlen(path)-i)-4] == 's' || path[(strlen(path)-i)-4] == 'S') &&
									(path[(strlen(path)-i)-5] == 'e' || path[(strlen(path)-i)-5] == 'E') &&
									(path[(strlen(path)-i)-6] == 'm' || path[(strlen(path)-i)-6] == 'M')
									)
								{
									memcpy(name,path+(strlen(path)-i)+1,i-1);
									name[(i-1)] = 0;
									break;
								}
						}
				}
		}

		void StrCutSound(const char* path,char* name)
		{
				for(int i=0;i<strlen(path);i++)
				{
						if(path[strlen(path)-i] == '\\')
						{
								if(i+6 > 0 && i+6 < strlen(path) && 
									(path[(strlen(path)-i)-1] == 's' || path[(strlen(path)-i)-1] == 'S') &&
									(path[(strlen(path)-i)-2] == 'd' || path[(strlen(path)-i)-2] == 'D') &&
									(path[(strlen(path)-i)-3] == 'n' || path[(strlen(path)-i)-3] == 'N') &&
									(path[(strlen(path)-i)-4] == 'u' || path[(strlen(path)-i)-4] == 'U') &&
									(path[(strlen(path)-i)-5] == 'o' || path[(strlen(path)-i)-5] == 'O') &&
									(path[(strlen(path)-i)-6] == 's' || path[(strlen(path)-i)-6] == 'S')
									)
								{
									memcpy(name,path+(strlen(path)-i)+1,i-1);
									name[(i-1)] = 0;
									break;
								}
						}
				}
		}

		void StrCutName(const char* path,char* name)
		{
			name[0] = 0;
				for(int i=0;i<strlen(path);i++)
				{
						if(path[strlen(path)-i] == '\\')
						{
							memcpy(name,path+(strlen(path)-i)+1,i-1);
							name[(i-1)] = 0;
							break;
						}
				}

				if(name[0] == 0)
				{
					sprintf(name,"%s",path);
				}
		}

		void StrCutNameNEx(const char* path,char* name)
		{
			name[0] = 0;
			int pointpoint = 0;
				for(int i=0;i<strlen(path);i++)
				{
						if(path[strlen(path)-i] == '\\')
						{
							memcpy(name,path+(strlen(path)-i)+1,(i-1) - pointpoint);
							name[(i-1) - pointpoint] = 0;
							break;
						}
						else if(path[strlen(path)-i] == '.')
							pointpoint = i;
				}

				if(name[0] == 0)
				{
					sprintf(name,"%s",path);
				}
		}

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
			SX_SAFE_DELETE_A(tmp);
		}

		String::String(WORD num)
		{
			char * tmp = new char[64];
			sprintf(tmp, "%u", num);
			string = new char[strlen(tmp) + 1];
			memcpy(string, tmp, strlen(tmp) + 1);
			SX_SAFE_DELETE_A(tmp);
		}

		String::String(DWORD num)
		{
			char * tmp = new char[64];
			sprintf(tmp, "%u", num);
			string = new char[strlen(tmp) + 1];
			memcpy(string, tmp, strlen(tmp) + 1);
			SX_SAFE_DELETE_A(tmp);
		}

		String::String(long num)
		{
			char * tmp = new char[64];
			sprintf(tmp, "%d", num);
			string = new char[strlen(tmp) + 1];
			memcpy(string, tmp, strlen(tmp) + 1);
			SX_SAFE_DELETE_A(tmp);
		}

		String::String(double num)
		{
			char * tmp = new char[64];
			sprintf(tmp, "%G", num);
			string = new char[strlen(tmp) + 1];
			memcpy(string, tmp, strlen(tmp) + 1);
			SX_SAFE_DELETE_A(tmp);
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

		String::~String()
		{
			Release();
		}

		void String::Release()
		{
			SX_SAFE_DELETE_A(string);
		}

		////////////////////////////////////////////

		String String::operator+(const String &str) const
		{
			char * ns = new char[length() + str.length() + 1];
			sprintf(ns, "%s%s", string, str.c_str());
			String newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		String String::operator+(const String *str)
		{
			char * ns = new char[length() + str->length() + 1];
			sprintf(ns, "%s%s", string, str->c_str());
			String newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		String String::operator+(const char * str)
		{
			char * ns = new char[length() + strlen(str) + 1];
			sprintf(ns, "%s%s", string, str);
			String newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		String String::operator+(const char & sym)
		{
			char * ns = new char[length() + 2];
			sprintf(ns, "%s%c", string, sym);
			String newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		String String::operator+(const int & num)
		{
			char * ns = new char[length() + 64];
			sprintf(ns, "%s%d", string, num);
			String newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		String String::operator+(const WORD & num)
		{
			char * ns = new char[length() + 64];
			sprintf(ns, "%s%u", string, num);
			String newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		String String::operator+(const DWORD & num)
		{
			char * ns = new char[length() + 64];
			sprintf(ns, "%s%u", string, num);
			String newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		String String::operator+(const long & num)
		{
			char * ns = new char[length() + 64];
			sprintf(ns, "%s%d", string, num);
			String newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		String String::operator+(const double & num)
		{
			char * ns = new char[length() + 64];
			sprintf(ns, "%s%G", string, num);
			String newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		String String::operator+(const bool & bf)
		{
			char * ns = new char[length() + (bf ? 5 : 6)];
			sprintf(ns, "%s%s", string, bf ? "true" : "false");
			String newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
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
			SX_SAFE_DELETE_A(tmp);
			return(*this);
		}

		String & String::operator=(const WORD & num)
		{
			Release();
			char * tmp = new char[64];
			sprintf(tmp, "%u", num);
			string = new char[strlen(tmp) + 1];
			memcpy(string, tmp, strlen(tmp) + 1);
			SX_SAFE_DELETE_A(tmp);
			return(*this);
		}

		String & String::operator=(const DWORD & num)
		{
			Release();
			char * tmp = new char[64];
			sprintf(tmp, "%u", num);
			string = new char[strlen(tmp) + 1];
			memcpy(string, tmp, strlen(tmp) + 1);
			SX_SAFE_DELETE_A(tmp);
			return(*this);
		}

		String & String::operator=(const long & num)
		{
			Release();
			char * tmp = new char[64];
			sprintf(tmp, "%d", num);
			string = new char[strlen(tmp) + 1];
			memcpy(string, tmp, strlen(tmp) + 1);
			SX_SAFE_DELETE_A(tmp);
			return(*this);
		}

		String & String::operator=(const double & num)
		{
			Release();
			char * tmp = new char[64];
			sprintf(tmp, "%G", num);
			string = new char[strlen(tmp) + 1];
			memcpy(string, tmp, strlen(tmp) + 1);
			SX_SAFE_DELETE_A(tmp);
			return(*this);
		}

		String & String::operator=(const bool & bf)
		{
			Release();
			string = new char[bf ? 5 : 6];
			sprintf(string, "%s", bf ? "true" : "false");
			return(*this);
		}

		///////////////////////////////////////////////////


		String & String::operator+=(const String & str)
		{
			char * ns = new char[length() + str.length() + 1];
			sprintf(ns, "%s%s", string, str.c_str());
			SX_SAFE_DELETE_A(string);
			string = ns;
			return(*this);
		}

		String & String::operator+=(const String * str)
		{
			char * ns = new char[length() + str->length() + 1];
			sprintf(ns, "%s%s", string, str->c_str());
			SX_SAFE_DELETE_A(string);
			string = ns;
			return(*this);
		}

		String & String::operator+=(const char * str)
		{
			char * newstring = new char[length() + strlen(str) + 1];
			sprintf(newstring, "%s%s", string, str);
			SX_SAFE_DELETE_A(string);
			string = newstring;
			return(*this);
		}

		String & String::operator+=(const char & sym)
		{
			char * newstring = new char[length() + 2];
			sprintf(newstring, "%s%c", string, sym);
			SX_SAFE_DELETE_A(string);
			string = newstring;
			return(*this);
		}

		String & String::operator+=(const int & num)
		{
			char * tmp = new char[length() + 64];
			sprintf(tmp, "%s%d", string, num);
			SX_SAFE_DELETE_A(string);
			string = tmp;
			return(*this);
		}

		String & String::operator+=(const WORD & num)
		{
			char * tmp = new char[length() + 64];
			sprintf(tmp, "%s%u", string, num);
			SX_SAFE_DELETE_A(string);
			string = tmp;
			return(*this);
		}

		String & String::operator+=(const DWORD & num)
		{
			char * tmp = new char[length() + 64];
			sprintf(tmp, "%s%u", string, num);
			SX_SAFE_DELETE_A(string);
			string = tmp;
			return(*this);
		}

		String & String::operator+=(const long & num)
		{
			char * tmp = new char[length() + 64];
			sprintf(tmp, "%s%d", string, num);
			SX_SAFE_DELETE_A(string);
			string = tmp;
			return(*this);
		}

		String & String::operator+=(const double & num)
		{
			char * tmp = new char[length() + 64];
			sprintf(tmp, "%s%G", string, num);
			SX_SAFE_DELETE_A(string);
			string = tmp;
			return(*this);
		}

		String & String::operator+=(const bool & bf)
		{
			char * tmp = new char[length() + (bf ? 5 : 6)];
			sprintf(tmp, "%s%s", string, bf ? "true" : "false");
			SX_SAFE_DELETE_A(string);
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
					// return Find - (string + pos) + pos; // - pos + pos == 0 ? зачем?
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

				if(PosBeginFmt == -1)
					return -1;

			DWORD CountStrLen = strlen(replace) + length() - StrLen;
			char * CountStr = new char[CountStrLen + 1];

			//sprintf(CountStr,"%s%s%s",SubStr(0,PosBeginFmt),replace,SubStr(PosBeginFmt+StrLen,length() - (PosBeginFmt+StrLen)));
			memcpy(CountStr, string, PosBeginFmt);
			memcpy(CountStr + PosBeginFmt, replace, strlen(replace));
			memcpy(CountStr + PosBeginFmt + strlen(replace), string + PosBeginFmt + StrLen, length() - (PosBeginFmt + StrLen));
			SX_SAFE_DELETE_A(string);
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

				while(PosCodeBegin != -1)
				{
					PosCodeBegin = find(str, PosCodeOld);
						if(PosCodeBegin != -1)
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
			SX_SAFE_DELETE_A(string);
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

		String String::substr(DWORD pos, DWORD len)
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
			SX_SAFE_DELETE_A(s);
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
			SX_SAFE_DELETE_A(string);
			string = newStr;
			string[StrLen] = 0;

			return(len);
		}

		void String::insert(unsigned long pos,const char* str)
		{
			DWORD StrLen = length() + strlen(str);
			char * newStr = new char[StrLen + 1];
			memcpy(newStr, string, pos);
			memcpy(newStr+pos, str, strlen(str));
			memcpy(newStr+pos+strlen(str), string+pos, length() - pos);
			SX_SAFE_DELETE_A(string);
			string = newStr;
			string[StrLen] = 0;
		}

		////////////////////////////////////////////

		int String::ToInt() const
		{
			return(atoi(string));
		}

		long String::ToLongInt() const
		{
			return(atol(string));
		}

		DWORD String::ToUnsLongInt() const
		{
			DWORD num;
			sscanf(string, "%I", &num);
			return(num);
		}

		double String::ToDouble() const
		{
			return(atof(string));
		}

		bool String::ToBool() const
		{
			return(ToInt() == 1 || strcmpi(string, "true") == 0);
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
					// return Find - (string + pos) + pos; // - pos + pos == 0 ? зачем?
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


		const wchar_t * StringW::c_str() const
		{
			return(string);
		}

		StringW::StringW()
		{
			string = new wchar_t[1];
			string[0] = 0;
		}

		StringW::StringW(const wchar_t * str)
		{
			string = new wchar_t[lstrlenW(str) + 1];
			//sprintf(string,"%s",str);
			memcpy(string, str, sizeof(wchar_t) * lstrlenW(str));
			string[lstrlenW(str)] = 0;
		}

		StringW::StringW(wchar_t sym)
		{
			string = new wchar_t[2];
			//sprintf(string,"%s",&sym);
			string[0] = sym;
			string[1] = 0;
		}

		StringW::StringW(int num)
		{
			wchar_t * tmp = new wchar_t[64];
			swprintf(tmp, L"%d", num);
			string = new wchar_t[lstrlenW(tmp) + 1];
			memcpy(string, tmp, sizeof(wchar_t) * (lstrlenW(tmp) + 1));
			SX_SAFE_DELETE_A(tmp);
		}

		StringW::StringW(WORD num)
		{
			wchar_t * tmp = new wchar_t[64];
			swprintf(tmp, L"%u", num);
			string = new wchar_t[lstrlenW(tmp) + 1];
			memcpy(string, tmp, sizeof(wchar_t) * (lstrlenW(tmp) + 1));
			SX_SAFE_DELETE_A(tmp);
		}

		StringW::StringW(DWORD num)
		{
			wchar_t * tmp = new wchar_t[64];
			swprintf(tmp, L"%u", num);
			string = new wchar_t[lstrlenW(tmp) + 1];
			memcpy(string, tmp, sizeof(wchar_t) * (lstrlenW(tmp) + 1));
			SX_SAFE_DELETE_A(tmp);
		}

		StringW::StringW(long num)
		{
			wchar_t * tmp = new wchar_t[64];
			swprintf(tmp, L"%d", num);
			string = new wchar_t[lstrlenW(tmp) + 1];
			memcpy(string, tmp, sizeof(wchar_t) * (lstrlenW(tmp) + 1));
			SX_SAFE_DELETE_A(tmp);
		}

		StringW::StringW(double num)
		{
			wchar_t * tmp = new wchar_t[64];
			swprintf(tmp, L"%G", num);
			string = new wchar_t[lstrlenW(tmp) + 1];
			memcpy(string, tmp, sizeof(wchar_t) * (lstrlenW(tmp) + 1));
			SX_SAFE_DELETE_A(tmp);
		}

		StringW::StringW(bool bf)
		{
			string = new wchar_t[bf ? 5 : 6];
			swprintf(string, L"%s", bf ? L"true" : L"false");
		}

		StringW::StringW(const StringW & str)
		{
			string = new wchar_t[str.length() + 1];
			memcpy(string, str.c_str(), sizeof(wchar_t) * (str.length() + 1));
		}

		StringW::StringW(const StringW * str)
		{
			string = new wchar_t[str->length() + 1];
			memcpy(string, str->c_str(), sizeof(wchar_t) * (str->length() + 1));
		}

		StringW::~StringW()
		{
			Release();
		}


		void StringW::Release()
		{
			SX_SAFE_DELETE_A(string);
		}

		////////////////////////////////////////////

		StringW StringW::operator+(const StringW &str) const
		{
			wchar_t * ns = new wchar_t[length() + str.length() + 1];
			swprintf(ns, L"%s%s", string, str.c_str());
			StringW newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		StringW StringW::operator+(const StringW *str)
		{
			wchar_t * ns = new wchar_t[length() + str->length() + 1];
			swprintf(ns, L"%s%s", string, str->c_str());
			StringW newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		StringW StringW::operator+(const wchar_t * str)
		{
			wchar_t * ns = new wchar_t[length() + lstrlenW(str) + 1];
			swprintf(ns, L"%s%s", string, str);
			StringW newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		StringW StringW::operator+(const wchar_t & sym)
		{
			wchar_t * ns = new wchar_t[length() + 2];
			swprintf(ns, L"%s%c", string, sym);
			StringW newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		StringW StringW::operator+(const int & num)
		{
			wchar_t * ns = new wchar_t[length() + 64];
			swprintf(ns, L"%s%d", string, num);
			StringW newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		StringW StringW::operator+(const WORD & num)
		{
			wchar_t * ns = new wchar_t[length() + 64];
			swprintf(ns, L"%s%u", string, num);
			StringW newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		StringW StringW::operator+(const DWORD & num)
		{
			wchar_t * ns = new wchar_t[length() + 64];
			swprintf(ns, L"%s%u", string, num);
			StringW newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		StringW StringW::operator+(const long & num)
		{
			wchar_t * ns = new wchar_t[length() + 64];
			swprintf(ns, L"%s%d", string, num);
			StringW newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		StringW StringW::operator+(const double & num)
		{
			wchar_t * ns = new wchar_t[length() + 64];
			swprintf(ns, L"%s%G", string, num);
			StringW newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		StringW StringW::operator+(const bool & bf)
		{
			wchar_t * ns = new wchar_t[length() + (bf ? 5 : 6)];
			swprintf(ns, L"%s%s", string, bf ? L"true" : L"false");
			StringW newSXStr = ns;
			SX_SAFE_DELETE_A(ns);
			return(newSXStr);
		}

		//////////////////////////////////////////////////////


		StringW & StringW::operator=(const StringW & str)
		{
			Release();
			string = new wchar_t[str.length() + 1];
			memcpy(string, str.c_str(), sizeof(wchar_t) * (str.length() + 1));
			return(*this);
		}

		StringW & StringW::operator=(const StringW * str)
		{
			Release();
			string = new wchar_t[str->length() + 1];
			memcpy(string, str->c_str(), sizeof(wchar_t) * (str->length() + 1));
			return(*this);
		}

		StringW & StringW::operator=(const wchar_t * str)
		{
			Release();
			string = new wchar_t[lstrlenW(str) + 1];
			memcpy(string, str, sizeof(wchar_t) * (lstrlenW(str) + 1));
			return(*this);
		}

		StringW & StringW::operator=(const wchar_t & sym)
		{
			Release();
			string = new wchar_t[2];
			string[0] = sym;
			string[1] = 0;
			return(*this);
		}

		StringW & StringW::operator=(const int & num)
		{
			Release();
			wchar_t * tmp = new wchar_t[64];
			swprintf(tmp, L"%d", num);
			string = new wchar_t[lstrlenW(tmp) + 1];
			memcpy(string, tmp, sizeof(wchar_t) * (lstrlenW(tmp) + 1));
			SX_SAFE_DELETE_A(tmp);
			return(*this);
		}

		StringW & StringW::operator=(const WORD & num)
		{
			Release();
			wchar_t * tmp = new wchar_t[64];
			swprintf(tmp, L"%u", num);
			string = new wchar_t[lstrlenW(tmp) + 1];
			memcpy(string, tmp, sizeof(wchar_t) * (lstrlenW(tmp) + 1));
			SX_SAFE_DELETE_A(tmp);
			return(*this);
		}

		StringW & StringW::operator=(const DWORD & num)
		{
			Release();
			wchar_t * tmp = new wchar_t[64];
			swprintf(tmp, L"%u", num);
			string = new wchar_t[lstrlenW(tmp) + 1];
			memcpy(string, tmp, sizeof(wchar_t) * (lstrlenW(tmp) + 1));
			SX_SAFE_DELETE_A(tmp);
			return(*this);
		}

		StringW & StringW::operator=(const long & num)
		{
			Release();
			wchar_t * tmp = new wchar_t[64];
			swprintf(tmp, L"%d", num);
			string = new wchar_t[lstrlenW(tmp) + 1];
			memcpy(string, tmp, sizeof(wchar_t) * (lstrlenW(tmp) + 1));
			SX_SAFE_DELETE_A(tmp);
			return(*this);
		}

		StringW & StringW::operator=(const double & num)
		{
			Release();
			wchar_t * tmp = new wchar_t[64];
			swprintf(tmp, L"%G", num);
			string = new wchar_t[lstrlenW(tmp) + 1];
			memcpy(string, tmp, sizeof(wchar_t) * (lstrlenW(tmp) + 1));
			SX_SAFE_DELETE_A(tmp);
			return(*this);
		}

		StringW & StringW::operator=(const bool & bf)
		{
			Release();
			string = new wchar_t[bf ? 5 : 6];
			swprintf(string, L"%s", bf ? L"true" : L"false");
			return(*this);
		}

		///////////////////////////////////////////////////


		StringW & StringW::operator+=(const StringW & str)
		{
			wchar_t * ns = new wchar_t[length() + str.length() + 1];
			swprintf(ns, L"%s%s", string, str.c_str());
			SX_SAFE_DELETE_A(string);
			string = ns;
			return(*this);
		}

		StringW & StringW::operator+=(const StringW * str)
		{
			wchar_t * ns = new wchar_t[length() + str->length() + 1];
			swprintf(ns, L"%s%s", string, str->c_str());
			SX_SAFE_DELETE_A(string);
			string = ns;
			return(*this);
		}

		StringW & StringW::operator+=(const wchar_t * str)
		{
			wchar_t * newstring = new wchar_t[length() + lstrlenW(str) + 1];
			swprintf(newstring, L"%s%s", string, str);
			SX_SAFE_DELETE_A(string);
			string = newstring;
			return(*this);
		}

		StringW & StringW::operator+=(const wchar_t & sym)
		{
			wchar_t * newstring = new wchar_t[length() + 2];
			swprintf(newstring, L"%s%c", string, sym);
			SX_SAFE_DELETE_A(string);
			string = newstring;
			return(*this);
		}

		StringW & StringW::operator+=(const int & num)
		{
			wchar_t * tmp = new wchar_t[length() + 64];
			swprintf(tmp, L"%s%d", string, num);
			SX_SAFE_DELETE_A(string);
			string = tmp;
			return(*this);
		}

		StringW & StringW::operator+=(const WORD & num)
		{
			wchar_t * tmp = new wchar_t[length() + 64];
			swprintf(tmp, L"%s%u", string, num);
			SX_SAFE_DELETE_A(string);
			string = tmp;
			return(*this);
		}

		StringW & StringW::operator+=(const DWORD & num)
		{
			wchar_t * tmp = new wchar_t[length() + 64];
			swprintf(tmp, L"%s%u", string, num);
			SX_SAFE_DELETE_A(string);
			string = tmp;
			return(*this);
		}

		StringW & StringW::operator+=(const long & num)
		{
			wchar_t * tmp = new wchar_t[length() + 64];
			swprintf(tmp, L"%s%d", string, num);
			SX_SAFE_DELETE_A(string);
			string = tmp;
			return(*this);
		}

		StringW & StringW::operator+=(const double & num)
		{
			wchar_t * tmp = new wchar_t[length() + 64];
			swprintf(tmp, L"%s%G", string, num);
			SX_SAFE_DELETE_A(string);
			string = tmp;
			return(*this);
		}

		StringW & StringW::operator+=(const bool & bf)
		{
			wchar_t * tmp = new wchar_t[length() + (bf ? 5 : 6)];
			swprintf(tmp, L"%s%s", string, bf ? L"true" : L"false");
			SX_SAFE_DELETE_A(string);
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

		StringW StringW::operator-(const wchar_t * str)
		{
			StringW newStr = this;
			newStr.Replace(str, "", 0);
			return(newStr);
		}

		StringW StringW::operator-(const wchar_t & sym)
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

		StringW & StringW::operator-=(const wchar_t * str)
		{
			Replace(str, "", 0);
			return(*this);
		}

		StringW & StringW::operator-=(const wchar_t & sym)
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

		StringW StringW::operator/(const wchar_t * str)
		{
			StringW newStr = this;
			newStr.ReplaceAll(str, L"");
			return(newStr);
		}

		StringW StringW::operator/(const wchar_t & sym)
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

		StringW & StringW::operator/=(const wchar_t * str)
		{
			ReplaceAll(str, L"");
			return(*this);
		}

		StringW & StringW::operator/=(const wchar_t & sym)
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

		bool StringW::operator==(const wchar_t * str) const
		{
			/*wchar_t *newstring = new wchar_t[strlen(str)];
			sprintf(newstring, "%s", str);
			if(*this == newstring)
			return true;
			else
			return false;
			*/
				if(length() == lstrlenW(str))
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

		bool StringW::operator==(const wchar_t & sym) const
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

		bool StringW::operator!=(const wchar_t * str) const
		{
				if(*this == str)
					return false;
				else
					return true;
		}

		bool StringW::operator!=(const wchar_t & sym) const
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

		wchar_t & StringW::operator[](const DWORD & num)
		{
			return(string[num]);
		}

		const wchar_t & StringW::operator[](const DWORD & num) const
		{
			return(string[num]);
		}


		////////////////////////////////////////////////////////

		DWORD StringW::length() const
		{
			return(lstrlenW(string));
		}

		DWORD StringW::find(const wchar_t * str, DWORD pos) const
		{
			//DWORD BeginPos = pos;
			//wchar_t *newwchar_t = new wchar_t[strlen(string+BeginPos)];
			//sprintf(newwchar_t,"%s",string+BeginPos);

			wchar_t *Find = wcsstr(string + pos, str);

				if(Find != NULL)
					// return Find - (string + pos) + pos; // - pos + pos == 0 ? зачем?
					return(Find - string);
				else
					return -1;
		}

		DWORD StringW::find(const wchar_t & c, DWORD pos) const
		{
			wchar_t str[2];
			str[0] = c;
			str[1] = 0;
			return(find(str, pos));
		}

		DWORD StringW::find(const StringW & str, DWORD pos) const
		{
			return(find(str.c_str(), pos));
		}

		DWORD StringW::Replace(const wchar_t * str, const wchar_t * replace, DWORD pos)
		{
			DWORD PosBeginFmt = find(str, pos);
			DWORD StrLen = lstrlenW(str);

				if(PosBeginFmt == -1)
					return -1;

			DWORD CountStrLen = lstrlenW(replace) + length() - StrLen;
			wchar_t * CountStr = new wchar_t[CountStrLen + 1];

			//sprintf(CountStr,"%s%s%s",SubStr(0,PosBeginFmt),replace,SubStr(PosBeginFmt+StrLen,length() - (PosBeginFmt+StrLen)));
			memcpy(CountStr, string, PosBeginFmt);
			memcpy(CountStr + PosBeginFmt, replace, sizeof(wchar_t) * lstrlenW(replace));
			memcpy(CountStr + PosBeginFmt + lstrlenW(replace), string + PosBeginFmt + StrLen, sizeof(wchar_t) * (length() - (PosBeginFmt + StrLen)));
			SX_SAFE_DELETE_A(string);
			string = CountStr;
			string[CountStrLen] = 0;

			return(PosBeginFmt);
		}

		DWORD StringW::Replace(const StringW & str, const StringW & replace, DWORD pos)
		{
			return(Replace(str.c_str(), replace.c_str(), pos));
		}

		DWORD StringW::ReplaceAll(const wchar_t * str, const wchar_t * replace)
		{
			DWORD PosCodeBegin = 0;
			DWORD PosCodeOld = 0;
			DWORD CountCodeFMT = 0;

				while(PosCodeBegin != -1)
				{
					PosCodeBegin = find(str, PosCodeOld);
						if(PosCodeBegin != -1)
						{
							CountCodeFMT++;
							PosCodeOld = PosCodeBegin + lstrlenW(str);
						}
						else
							break;
				}


			DWORD StrLen = lstrlenW(str);
			DWORD count_str_len = length() + (CountCodeFMT * (lstrlenW(replace) - StrLen));

			//MessageBox(0,ToPointwchar_t(ToString(length()) + "|" + ToString(CountCodeFMT) + "|" + ToString(StrLen)),ToPointwchar_t(count_str_len),0);
			//MessageBox(0,string,ToPointwchar_t(DWORD(strlen(string))),0);

			wchar_t * count_str = new wchar_t[count_str_len + 1];
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
					memcpy(count_str, string + OldPosEndFmt, sizeof(wchar_t) * (PosBeginFmt - OldPosEndFmt));
					count_str_last_len += PosBeginFmt - OldPosEndFmt;
					memcpy(count_str + count_str_last_len, replace, sizeof(wchar_t) * lstrlenW(replace));
					count_str_last_len += lstrlenW(replace);
					OldPosEndFmt = PosBeginFmt + StrLen;
				}
				else
					return -1;

			//MessageBox(0,count_str,"count_str1",0);

				while((PosBeginFmt = find(str, OldPosEndFmt)) != DWORD(-1))
				{
					//sprintf(count_str,"%s%s%s",count_str,SubStr(OldPosEndFmt,PosBeginFmt-OldPosEndFmt),replace);
					//MessageBox(0,ToPointwchar_t(PosBeginFmt),"PosBeginFmt",0);
					memcpy(count_str + count_str_last_len, string + OldPosEndFmt, sizeof(wchar_t) * (PosBeginFmt - OldPosEndFmt));
					count_str_last_len += PosBeginFmt - OldPosEndFmt;

					memcpy(count_str + count_str_last_len, replace, sizeof(wchar_t) * lstrlenW(replace));
					count_str_last_len += lstrlenW(replace);

					OldPosEndFmt = PosBeginFmt + StrLen;
				}
			//MessageBox(0,SubStr(OldPosEndFmt,length()-OldPosEndFmt),"SubStr(OldPosEndFmt,length()-OldPosEndFmt)",0);
			//MessageBox(0,count_str,"count_str----",0);
			//sprintf(count_str,"%s%s",count_str,SubStr(OldPosEndFmt,length()-OldPosEndFmt));
			memcpy(count_str + count_str_last_len, string + OldPosEndFmt, sizeof(wchar_t) * (length() - OldPosEndFmt));
			count_str[count_str_len] = 0;
			SX_SAFE_DELETE_A(string);
			string = count_str;

			//MessageBox(0,count_str,"count_str++++++",0);

			return OldPosEndFmt - StrLen;
		}

		DWORD StringW::ReplaceAll(const StringW & str, const StringW & replace)
		{
			return(ReplaceAll(str.c_str(), replace.c_str()));
		}

		/*wchar_t* StringW::SubStr(DWORD pos, DWORD lenght)
		{
		wchar_t *str = new wchar_t[lenght];
		memcpy(str, string + pos, lenght);
		str[lenght] = 0;
		return str;
		}*/

		StringW StringW::substr(DWORD pos, DWORD len) const
		{
			wchar_t * ns = string + pos;
				if(!len || len > length() - pos)
				{
					len = length() - pos;
				}
			wchar_t * s = new wchar_t[len + 1];
			memcpy(s, ns, sizeof(wchar_t) * len);
			s[len] = 0;
			StringW res = s;
			SX_SAFE_DELETE_A(s);
			return(res);
		}

		UINT StringW::Delete(DWORD pos, DWORD len)
		{
			/*wchar_t *str = SubStr(pos,lenght);
			wchar_t *OneSubStr = SubStr(0,pos);
			wchar_t *SecondSubStr = SubStr(pos + lenght,length() - (pos + lenght));
			wchar_t *newStr = new wchar_t[strlen(OneSubStr) + strlen(SecondSubStr)];
			sprintf(newStr,"%s%s",OneSubStr,SecondSubStr);
			string = newStr;
			return str;*/

			//wchar_t *str = SubStr(pos, lenght);

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

			wchar_t * newStr = new wchar_t[StrLen + 1];
			//sprintf(string,"%s%s",SubStr(0,pos),SubStr(PosLenght,length() - PosLenght));
			memcpy(newStr, string, sizeof(wchar_t) * pos);
			memcpy(newStr + pos, string + PosLenght, sizeof(wchar_t) * (length() - PosLenght));
			SX_SAFE_DELETE_A(string);
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
			swscanf(string, L"%I", &num);
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
			return(ToInt() == 1 || lstrcmpiW(string, L"true") == 0);
		}

		DWORD StringW::find_last_of(const wchar_t * str, DWORD pos) const
		{

			wchar_t * Find = NULL;
			wchar_t * tmpFind = NULL;
				while((Find = wcsstr(string + pos, str)))
				{
					pos = Find - string + 1;
					tmpFind = Find;
				}

				if(tmpFind != NULL)
					// return Find - (string + pos) + pos; // - pos + pos == 0 ? зачем?
					return(tmpFind - string);
				else
					return -1;
		}

		DWORD StringW::find_last_of(const wchar_t & c, DWORD pos) const
		{
			wchar_t str[2];
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

		void StringW::insert(UINT pos, const wchar_t * data)
		{
			wchar_t * tmpStr = new wchar_t[length() + lstrlenW(data) + 1];
			memcpy(tmpStr, string, sizeof(wchar_t) * pos);
			memcpy(tmpStr + pos, data, sizeof(wchar_t) * lstrlenW(data));
			memcpy(tmpStr + pos + lstrlenW(data), string + pos, sizeof(wchar_t) * (length() - pos));
			tmpStr[lstrlenW(data) + length()] = 0;
			SX_SAFE_DELETE_A(string);
			string = tmpStr;
		}


		bool operator<(const StringW & a, const StringW & b)
		{
			return(lstrcmpW(a.c_str(), b.c_str()) < 0);
		}

		String::operator StringW() const
		{
			StringW dst;
			dst.Reserve(length());
			MultiByteToWideChar(1251, CP_ACP, string, length()+1, dst.string, length()+1);
			return(dst);
		}

		StringW::operator String() const
		{
			String dst;
			dst.Reserve(length());
			WideCharToMultiByte(1251, CP_ACP, string, length()+1, dst.string, length()+1, " ", NULL);
			return(dst);
		}

		void String::Reserve(int length)
		{
			SX_SAFE_DELETE_A(string);
			string = new char[length + 1];
		}

		void StringW::Reserve(int length)
		{
			SX_SAFE_DELETE_A(string);
			string = new wchar_t[length + 1];
		}
