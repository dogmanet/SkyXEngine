#ifndef LOADER_CONFIG_H
#define LOADER_CONFIG_H

#pragma once

class SXLoaderConfig : public ISXLConfig
{
public:
	struct value
	{
		String val;
		bool isModified;
		value():isModified(false)
		{
		}
	};
	struct section
	{
		String parent;
		AssotiativeArray<String, value> mValues;
		bool native;
		String Include;
		bool isModified;
		section():isModified(false)
		{
		}
	};
	SXLoaderConfig();
	//SXLoaderConfig(const char* file);
	int Open(const char* path);

	const char* GetKey(const char* section, const char* key);
	const char* GetKeyName(const char* section, int key);
	const char* GetSectionName(int num);

	void Set(const char* section, const char* key, const char* val);

	int Save();

	int GetSectionCount(bool acceptIncludes = false);
	int GetKeyCount(bool acceptIncludes = false);
	int GetKeyCount(const char* section,bool acceptIncludes = false);

	bool SectionExists(const char* section, bool acceptIncludes = false);
	bool KeyExists(const char* section, const char* key, bool acceptIncludes = false);

	void Release();

	AssotiativeArray<String, section> * GetSections();

	//inline const char* GetPath();
	const char* GetErrorFile();

protected:

	String ErrorFile;
	
	struct include
	{
		String name;
		SXLoaderConfig *pParser;
	};

	String BaseFile;
	
//protected:
	AssotiativeArray<String, value> m_mFinalValues;

	AssotiativeArray<String, section> m_mSections;

	Array<include> m_vIncludes;
//public:

	int WriteFile(const String & name, String section, String key, const String & val);
	

	int Parse(const String & file);

	int ParseInclude(String & file, const String & dir);


	String GetIncludeName(int i);

	String BaseDir(String dir);
	String BaseName(String dir);

	void Modify(AssotiativeArray<String, section> & sections, AssotiativeArray<String, value> & values, String IncName);
};

#endif