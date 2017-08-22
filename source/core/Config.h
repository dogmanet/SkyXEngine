
#ifndef LOADER_CONFIG_H
#define LOADER_CONFIG_H

#include "sxcore.h"
#include <gdefines.h>
#include <common/String.h>
#include <common/Array.h>
#include <common/AssotiativeArray.h>

class CConfigString : public String
{
public:
	CConfigString() : String(){}
	CConfigString(const char * str) : String(str){}
	CConfigString(const String & str) : String(str){}
	CConfigString(const CConfigString & str) : String(str){}
	CConfigString(const CConfigString * str) : String(str){}
	CConfigString(const char sym) : String(sym){}
	~CConfigString() {}
	
	const char & operator[](const unsigned long & num) const
	{
		return(this->string[num]);
	}

	char & operator[](const unsigned long & num)
	{
		return(this->string[num]);
	}

	CConfigString & operator=(const CConfigString & str)
	{
		Release();
		string = new char[str.length() + 1];
		memcpy(string, str.c_str(), str.length() + 1);
		return(*this);
	}

	CConfigString & operator=(const CConfigString * str)
	{
		Release();
		string = new char[str->length() + 1];
		memcpy(string, str->c_str(), str->length() + 1);
		return(*this);
	}

	bool operator==(const CConfigString &str) const
	{
		return (stricmp(str.string, this->string) == 0);
	}

	bool operator==(const CConfigString * str) const
	{
		return (stricmp(str->string, this->string) == 0);
	}

	CConfigString & operator+=(const char &	sym)
	{
		char * newstring = new char[length() + 2];
		sprintf(newstring, "%s%c", string, sym);
		SAFE_DELETE_A(string);
		string = newstring;
		return(*this);
	}
};

class CConfig : public ISXConfig
{
public:
	struct CValue
	{
		CConfigString val;
		bool isModified;
		CValue():isModified(false)
		{
		}
	};
	struct CSection
	{
		CConfigString parent;
		AssotiativeArray<CConfigString, CValue> mValues;
		bool native;
		CConfigString Include;
		bool isModified;
		CSection():isModified(false)
		{
		}
	};
	CConfig();
	//SXLoaderConfig(const char* file);
	int open(const char* path);
	void New(const char* path);

	const char* getKey(const char* section, const char* key);
	const char* getKeyName(const char* section, int key);
	const char* getSectionName(int num);

	void set(const char* section, const char* key, const char* val);

	int save();

	int getSectionCount();
	int getKeyCount();
	int getKeyCount(const char* section);

	bool sectionExists(const char* section);
	bool keyExists(const char* section, const char* key);

	void Release();
	void clear();

	AssotiativeArray<CConfigString, CSection> * getSections();

	//inline const char* GetPath();
	const char* getErrorFile();

protected:

	CConfigString ErrorFile;
	
	struct CInclude
	{
		CConfigString name;
		CConfig *pParser;
	};

	CConfigString BaseFile;
	
//protected:
	AssotiativeArray<CConfigString, CValue> m_mFinalValues;

	AssotiativeArray<CConfigString, CSection> m_mSections;

	Array<CInclude> m_vIncludes;
//public:

	int writeFile(const CConfigString & name, CConfigString section, CConfigString key, const CConfigString & val);
	

	int parse(const char* file);

	int parseInclude(CConfigString & file, const CConfigString & dir);


	CConfigString getIncludeName(int i);

	CConfigString baseDir(CConfigString dir);
	CConfigString baseName(CConfigString dir);

	void modify(AssotiativeArray<CConfigString, CSection> & sections, AssotiativeArray<CConfigString, CValue> & values, CConfigString IncName);
};

#endif
