#ifndef LOADER_CONFIG_H
#define LOADER_CONFIG_H

#pragma once

class ConfigString : public String
{
public:
	ConfigString() : String(){}
	ConfigString(const char * str) : String(str){}
	ConfigString(const String & str) : String(str){}
	ConfigString(const ConfigString & str) : String(str){}
	ConfigString(const ConfigString * str) : String(str){}
	ConfigString(const char sym) : String(sym){}
	~ConfigString() {}
	
	const char & operator[](const unsigned long & num) const
	{
		return(this->string[num]);
	}

	char & operator[](const unsigned long & num)
	{
		return(this->string[num]);
	}

	ConfigString & operator=(const ConfigString & str)
	{
		Release();
		string = new char[str.length() + 1];
		memcpy(string, str.c_str(), str.length() + 1);
		return(*this);
	}

	ConfigString & operator=(const ConfigString * str)
	{
		Release();
		string = new char[str->length() + 1];
		memcpy(string, str->c_str(), str->length() + 1);
		return(*this);
	}

	bool operator==(const ConfigString &str) const
	{
		return (stricmp(str.string, this->string) == 0);
	}

	bool operator==(const ConfigString * str) const
	{
		return (stricmp(str->string, this->string) == 0);
	}

	ConfigString & operator+=(const char &	sym)
	{
		char * newstring = new char[length() + 2];
		sprintf(newstring, "%s%c", string, sym);
		SAFE_DELETE_A(string);
		string = newstring;
		return(*this);
	}
};

class SXLoaderConfig : public ISXLConfig
{
public:
	struct value
	{
		ConfigString val;
		bool isModified;
		value():isModified(false)
		{
		}
	};
	struct section
	{
		ConfigString parent;
		AssotiativeArray<ConfigString, value> mValues;
		bool native;
		ConfigString Include;
		bool isModified;
		section():isModified(false)
		{
		}
	};
	SXLoaderConfig();
	//SXLoaderConfig(const char* file);
	int Open(const char* path);
	void New(const char* path);

	const char* GetKey(const char* section, const char* key);
	const char* GetKeyName(const char* section, int key);
	const char* GetSectionName(int num);

	void Set(const char* section, const char* key, const char* val);

	int Save();

	int GetSectionCount();
	int GetKeyCount();
	int GetKeyCount(const char* section);

	bool SectionExists(const char* section);
	bool KeyExists(const char* section, const char* key);

	void Release();
	void Clear();

	AssotiativeArray<ConfigString, section> * GetSections();

	//inline const char* GetPath();
	const char* GetErrorFile();

protected:

	ConfigString ErrorFile;
	
	struct include
	{
		ConfigString name;
		SXLoaderConfig *pParser;
	};

	ConfigString BaseFile;
	
//protected:
	AssotiativeArray<ConfigString, value> m_mFinalValues;

	AssotiativeArray<ConfigString, section> m_mSections;

	Array<include> m_vIncludes;
//public:

	int WriteFile(const ConfigString & name, ConfigString section, ConfigString key, const ConfigString & val);
	

	int Parse(const char* file);

	int ParseInclude(ConfigString & file, const ConfigString & dir);


	ConfigString GetIncludeName(int i);

	ConfigString BaseDir(ConfigString dir);
	ConfigString BaseName(ConfigString dir);

	void Modify(AssotiativeArray<ConfigString, section> & sections, AssotiativeArray<ConfigString, value> & values, ConfigString IncName);
};

#endif
