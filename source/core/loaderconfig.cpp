
#include <common\\AssotiativeArray.h>
#include <common\\string.cpp>
#include <core\\LoaderConfig.h>
#pragma once
/*
1. Проверить обработку циклических зависимостей
2. Проверить привязку к переводу строки в конце файла
*/


SXLoaderConfig::SXLoaderConfig()
{
	ErrorFile = "";
	BaseFile = "";
}

int SXLoaderConfig::Open(const char* path)
{
	return Parse(path);
}

String SXLoaderConfig::BaseDir(String dir)
{
	int p1 = dir.find_last_of('/');
	int p2 = dir.find_last_of('\\');
	int pos = max(p1, p2);
	if(pos >= 0)
	{
		return(dir.substr(0, pos + 1));
	}
	else
	{
		return(dir);
	}
}

String SXLoaderConfig::BaseName(String dir)
{
	int p1 = dir.find_last_of('/');
	int p2 = dir.find_last_of('\\');
	int pos = max(p1, p2);
	if(pos >= 0)
	{
		return(dir.substr(pos + 1));
	}
	else
	{
		return(dir);
	}
}

int SXLoaderConfig::Parse(const char* file)
{
	Clear();
	BaseFile = String(file);
	FILE * fp = fopen(file, "rb");
		if(!fp)
		{
			return -1;
		}

	bool bComment = false;
	bool bPreprocessor = false;
	bool bInclude = false;
	bool bSectionName = false;
	bool bParentName = false;
	bool bKey = false;
	bool bValue = false;
	bool bFirstChar = true;
	bool bSectionNameDone = false;

	String s1 = ""; // some
	String s2 = ""; // section
	String s3 = ""; // parentS
	String s4 = ""; // val



	/*s1.reserve(256);
	s2.reserve(256);
	s3.reserve(256);
	s4.reserve(256);*/

	char c;
	char cn = 0;
	while(!feof(fp))
	{
		if(cn)
		{
			c = cn;
			cn = 0;
		}
		else
		{
			c = fgetc(fp);
		}
		if(bSectionNameDone)
		{
			bSectionName = false;
		}
		if(c == '\n' || c == '\r')
		{
			if(bInclude)
			{
				//printf("#include %s\n", s1.c_str());
				ParseInclude(s1, BaseDir(file));
			}
			if(bValue)
			{
				//int pos = min(s4.find_last_not_of(' '), s4.find_last_not_of('\t'));
				////////
				int pos = 0;
				int s4len = s4.length();
				for(int i = 0; i < s4len; ++i)
				{
					if(!isspace(s4[i]))
					{
						pos = i;
					}
				}
				////////
				s4 = s4.substr(0, pos + 1);
				if(s4[0] == '"' && s4[s4.length() - 1] == '"')
				{
					s4 = s4.substr(1, s4.length() - 2);
				}
				//printf("%s = %s\n", s1.c_str(), s4.c_str());
				
				if(!m_mSections.KeyExists(s2))
				{
					m_mSections[s2].parent = s3;
					m_mSections[s2].native = true;
					if(m_mSections.KeyExists(s3))
					{
						for(AssotiativeArray<String, value>::Iterator i = m_mSections[s3].mValues.begin(); i; i++)
						{
							m_mSections[s2].mValues[i.first].val = i.second;
						}
					}
				}
				m_mFinalValues[s1].val = s4;
				m_mSections[s2].mValues[s1].val = s4;
				//rtrim val;
				//key = s1
				//section = s2
				//parentS = s4
			}
			if(bKey) // we have only the key
			{
				//int pos = min(s4.find_last_not_of(' '), s4.find_last_not_of('\t'));
				////////
				int pos = 0;
				int s4len = s4.length();
				for(int i = 0; i < s4len; ++i)
				{
					if(!isspace(s4[i]))
					{
						pos = i;
					}
				}
				////////
				s1 = s1.substr(0, pos + 1);
				//printf("%s\n", s1.c_str());
				if(!m_mSections.KeyExists(s2))
				{
					m_mSections[s2].parent = s3;
					m_mSections[s2].native = true;
					if(m_mSections.KeyExists(s3))
					{
						for(AssotiativeArray<String, value>::Iterator i = m_mSections[s3].mValues.begin(); i; i++)
						{
							m_mSections[s2].mValues[i.first].val = i.second;
						}
					}
				}
				m_mFinalValues[s1].val = s1;
				m_mSections[s2].mValues[s1].val = s1;
			}
			bComment = false;
			bInclude = false;
			bSectionName = false;
			bParentName = false;
			bPreprocessor = false;
			bKey = false;
			bValue = false;
			bFirstChar = true;
			continue;
		}
		else if(bComment)
		{
			continue;
		}
		else if(bSectionNameDone)
		{
			s3 = "";
			if(c == ':')
			{
				bParentName = true;
				continue;
			}
			bSectionNameDone = false;
		}
		if(c == ';')
		{
			bComment = true;
		}
		else if(c == '/')
		{
			if((cn = getc(fp)) == '/')
			{
				bComment = true;
				continue;
			}
		}
		else if(c == '#' && bFirstChar)
		{
			s1 = "";
			s2 = "";
			bPreprocessor = true;
		}
		else if(bPreprocessor)
		{
			if(c != ' ' && c != '	')
			{
				s1 += c;
			}
			else
			{
				if(s1.length())
				{
					if(s1 == "include")
					{
						s1 = "";
						bInclude = true;
						bPreprocessor = false;
					}
				}
			}
		}
		else if(bInclude)
		{
			if(c != ' ' && c != '\t' && c != '"')
			{
				s1 += c;
			}
		}
		else if(bSectionName)
		{
			if(c == ']')
			{
				bSectionNameDone = true;
			}
			else
			{
				s2 += c;
			}
		}
		else if(bParentName)
		{
			if(c != ' ' && c != '\t')
			{
				s3 += c;
			}
		}
		else if(bValue)
		{
			if((c != ' ' && c != '\t') || s4.length())
			{
				s4 += c;
			}
		}
		else if(c == ' ' || c == '\t')
		{
			continue;
		}
		else if(c == '[' && bFirstChar)
		{
			s2 = "";
			bSectionName = true;
		}
		else if(bKey)
		{
			if(c != '=')
			{
				s1 += c;
			}
			else
			{
				bKey = false;
				bValue = true;
				s4 = "";
			}
		}
		else
		{
			bKey = true;
			s1 = c;
		}
		bFirstChar = false;
	}

	fclose(fp);
	return 0;
}

void SXLoaderConfig::Modify(AssotiativeArray<String, section> & sections, AssotiativeArray<String, value> & values, String IncName)
{
	for(AssotiativeArray<String, value>::Iterator i = m_mFinalValues.begin(); i; i++)
	{
		values[i.first].val = i.second->val;
	}

	for(AssotiativeArray<String, section>::Iterator i = m_mSections.begin(); i; i++)
	{
		if(!sections.KeyExists(i.first))
		{
			sections[i.first].parent = i.second->parent;
			sections[i.first].native = false;
			sections[i.first].Include = IncName;
		}
		for(AssotiativeArray<String, value>::Iterator j = m_mSections[i.first].mValues.begin(); j; j++)
		{
			sections[i.first].mValues[j.first].val = j.second->val;
		}
	}
}

int SXLoaderConfig::ParseInclude(String & file, const String & dir)
{
	//printf("Parsing include %s fromdir = %s\n", file.c_str(), dir.c_str());
	FILE * pF = fopen((dir + file).c_str(), "r");
	if(pF)
	{
		fclose(pF);
		include inc;

		inc.pParser = new SXLoaderConfig(/*(dir + file).c_str()*/);
		inc.pParser->Open((dir + file).c_str());

		inc.name = BaseName(dir + file);
/*		if(inc.name == "gl.ltx")
		{
			_asm
			{
				int 3;
			};
		}*/
		inc.pParser->Modify(m_mSections, m_mFinalValues, dir + file);
		m_vIncludes.push_back(inc);
		return(0);
	}
	else
	{
		//printf("[Error]: Unable to open include \"%s\"", (dir + file).c_str());
		ErrorFile = (dir + file);
		return -2;
	}
}

const char* SXLoaderConfig::GetErrorFile()
{
	return ErrorFile.c_str();
}

const char* SXLoaderConfig::GetKey(const char * section, const char * key)
{
	String keys(key);
	String sections(section);
		if(m_mSections.KeyExists(sections))
		{
				if(m_mSections[sections].mValues.KeyExists(keys))
				{
					return (m_mSections[sections].mValues[keys].val).c_str();
				}
			//return 0;
		}
	return 0;
}

const char* SXLoaderConfig::GetKeyName(const char* section, int key)
{
	//String keys(key);
	String sections(section);
	if (m_mSections.KeyExists(sections))
	{
		if (m_mSections[sections].mValues.Size() > key)
		{
			int countiter = 0;
			AssotiativeArray<String, SXLoaderConfig::value>::Iterator iter = m_mSections[sections].mValues.begin();
			for (int i = 0; i < key && iter; i++)
			{
				iter++;
				countiter++;
			}
			if (countiter == key)
				return iter.first->c_str();
		}
		return 0;
	}
	return 0;
}

const char* SXLoaderConfig::GetSectionName(int num)
{
	if (m_mSections.Size() > num)
	{
		int countiter = 0;
		AssotiativeArray<String, SXLoaderConfig::section>::Iterator iter = m_mSections.begin();
		for (int i = 0; i < num && iter; i++)
		{
			iter++;
			countiter++;
		}
			
		if (countiter == num)
			return iter.first->c_str();
	}
	return(NULL);
}

void SXLoaderConfig::Set(const char * sectionp, const char * key, const char * val)
{
	String sections(sectionp);
	String keys(key);
	String vals(val);

	if(!m_mSections.KeyExists(sections))
	{
		section s;
		s.native = true;
		m_mSections[sections] = s;
		//create section
	}
	section * s = &m_mSections[sections];
	//s->isModified = true;
	if(s->mValues.KeyExists(keys))
	{
		value * v = &s->mValues[keys];
		if(v->val != vals)
		{
			s->isModified = true;
			v->isModified = true;
			v->val = vals;
		}
	}
	else
	{
		value v;
		v.isModified = true;
		v.val = vals;
		s->mValues[keys] = v;
		s->isModified = true;
	}
}

int SXLoaderConfig::Save()
{
	int terror = 0;
	for(AssotiativeArray<String, section>::Iterator i = m_mSections.begin(); i; i++)
	{
		if(i.second->isModified)
		{
			for(AssotiativeArray<String, value>::Iterator j = i.second->mValues.begin(); j; j++)
			{
				if(j.second->isModified)
				{
					if(i.second->native) // Write to BaseFile
					{
						terror = WriteFile(BaseFile, *i.first, *j.first, j.second->val);
							if(terror!=0)
								return terror;
					}
					else // Write to i.second->Include
					{
						terror = WriteFile(i.second->Include, *i.first, *j.first, j.second->val);
							if(terror!=0)
								return terror;
					}
				}
			}
		}
	}

	return 0;
}

int SXLoaderConfig::WriteFile(const String & name, String section, String key, const String & val)
{
	//printf("W: %s\t[%s]: %s = %s\n", name.c_str(), section.c_str(), key.c_str(), val.c_str());
	FILE * pF = fopen(name.c_str(), "rb");
	if(pF)
	{
		fseek(pF, 0, SEEK_END);
		UINT fl = ftell(pF);
		fseek(pF, 0, SEEK_SET);
		char * szData = new char[fl];
		fread(szData, 1, fl, pF);
		szData[fl] = 0;
		fclose(pF);
		UINT sl = section.length();
		UINT kl = key.length();
		bool sf = false;
		bool kf = false;
		bool se = false;
		UINT sp = 0;
		for(int i = 0; i < fl; ++i)
		{
			if(szData[i] == '[' && ((i > 0 && (szData[i - 1] == '\r' || szData[i - 1] == '\n')) || i == 0))
			{
				bool cmp = true;
				int j;
				for(j = i + 1; j < fl - 1 && j - i - 1 < sl; ++j)
				{
					if(szData[j] != section[j - i - 1])
					{
						cmp = false;
						break;
					}
				}
				if(cmp && szData[j] == ']')//Section Found!
				{
					sf = true;
					i = j;
					for(; i < fl; ++i)
					{
						if(szData[i] == '\r' || szData[i] == '\n')
						{
							break;
						}
					}
					while(szData[i] == '\r' || szData[i] == '\n')
					{
						++i;
					}
					sp = i;
					//We are inside the section
					//So, let's find the key
					while(i < fl)
					{
						if(szData[i] == '[') // New section begin. There is no our key, so, add it!
						{
							kf = false;
							se = true;
							break;
						}
						while((szData[i] == ' ' || szData[i] == '\t') && i < fl)
						{
							++i;
						}
						bool f = true;
						for(j = i; j < fl && j - i < kl; ++j)
						{
							if(szData[j] != key[j - i])
							{
								f = false;
								break;
							}
						}
						if(f && (isspace(szData[j]) || szData[j] == '='))//KeyFound!
						{
							i = j;
							kf = true;
							for(j = i; j < fl; ++j)
							{
								if(szData[j] == '\n' || szData[j] == '\r' || szData[j] == ';')
								{
									//f = false;
									break;
								}
							}
							//while(szData[j] == '\r' || szData[j] == '\n')
							//{
							//	++j;
							//}
							//Let's write the file
							FILE * pF = fopen(name.c_str(), "wb");
							if(!pF)
							{
								ErrorFile = name;
								return -1;
							}
							fwrite(szData, 1, i, pF); // First file part, including key
							fwrite(" = ", 1, 3, pF);
							fwrite(val.c_str(), 1, val.length(), pF);
							//fwrite("\n", 1, 1, pF);
							fwrite(&szData[j], 1, fl - j, pF);
							fclose(pF);
							return 0;
						}
						else // Skip current row
						{
							for(; i < fl; ++i)
							{
								if(szData[i] == '\n' || szData[i] == '\r')
								{
									//f = false;
									break;
								}
							}
							while(szData[i] == '\r' || szData[i] == '\n')
							{
								++i;
							}
						}
					}
					//
					if(se)
					{
						break;
					}
				}
			}
		}
		if(sf && !kf)
		{
			FILE * pF = fopen(name.c_str(), "wb");
			if(!pF)
			{
				ErrorFile = name;
				return -1;
			}
			fwrite(szData, 1, sp, pF); // First file part
			fwrite(key.c_str(), 1, key.length(), pF);
			fwrite(" = ", 1, 3, pF);
			fwrite(val.c_str(), 1, val.length(), pF);
			fwrite("\n", 1, 1, pF);
			fwrite(&szData[sp], 1, fl - sp, pF);
			fclose(pF);
			return 0;
		}
		if(!sf)//!(Section found) == Add new
		{
			FILE * pF = fopen(name.c_str(), "ab");
			if(!pF)
			{
				ErrorFile = name;
				return -1;
			}
			fwrite((String("\n[") + section + "]\n" + key + " = " + val + "\n").c_str(), sizeof(char), section.length() + key.length() + val.length() + 8, pF);
			fclose(pF);
			return 0;
		}
	}
	else
	{
		FILE * pF = fopen(name.c_str(), "wb");
		if(!pF)
		{
			ErrorFile = name;
			return -1;
		}
		fwrite((String("[")+section+"]\n"+key+" = "+val+"\n").c_str(), sizeof(char), section.length() + key.length() + val.length() + 7, pF);
		fclose(pF);
		return 0;
	}
return 0;
}

int SXLoaderConfig::GetSectionCount()
{
	int c = m_mSections.Size();
		int size = m_vIncludes.size();
		for(int i = 0; i < size; ++i)
		{
			//m_vIncludes
			c += m_vIncludes[i].pParser->GetSectionCount();
	}
	return(c);
}

int SXLoaderConfig::GetKeyCount()
{
	int c = m_mFinalValues.Size();
		int size = m_vIncludes.size();
		for(int i = 0; i < size; ++i)
		{
			c += m_vIncludes[i].pParser->GetKeyCount();
	}
	return(c);
}

int SXLoaderConfig::GetKeyCount(const char* section)
{
	String sections(section);

	if (m_mSections.KeyExists(sections))
	{
			return(m_mSections[sections].mValues.Size());
		}
				return -1;
		}

bool SXLoaderConfig::SectionExists(const char * section)
{
	String sections(section);
	if(m_mSections.KeyExists(sections))
			return(true);
	return(false);
//	return(m_mSections.count(section) != 0 && m_mSections[section].native);
}

bool SXLoaderConfig::KeyExists(const char * section, const char * key)
{
	String sections(section);
	if(m_mSections.KeyExists(sections))
			return(m_mSections[sections].mValues.KeyExists(key));
	return(false);
}

void SXLoaderConfig::Release()
{
	this->Clear();
	mem_del(this);
}

void SXLoaderConfig::Clear()
{
	int size = m_vIncludes.size();
	for (int i = 0; i < size; ++i)
	{
		mem_release(m_vIncludes[i].pParser);
		mem_delete(m_vIncludes[i].pParser);
	}
	m_vIncludes.clear();
	m_mFinalValues.clear();
	m_mSections.clear();
	BaseFile = "\0";
}

AssotiativeArray<String, SXLoaderConfig::section> * SXLoaderConfig::GetSections()
{
	return(&m_mSections);
}

String SXLoaderConfig::GetIncludeName(int i)
{
	if(i >= 0 && i < m_vIncludes.size())
	{
		return(m_vIncludes[i].name);
	}
	return("");
}
