
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "Config.h"
#include "sxcore.h"

/*
1. Проверить обработку циклических зависимостей
2. Проверить привязку к переводу строки в конце файла
*/


CConfig::CConfig()
{
	ErrorFile = "";
	BaseFile = "";
}

int CConfig::open(const char* path)
{
	return parse(path);
}

void CConfig::New(const char* path)
{
	clear();
	BaseFile = path;
}

CConfigString CConfig::baseDir(CConfigString dir)
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

CConfigString CConfig::baseName(CConfigString dir)
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

int CConfig::parse(const char* file)
{
	clear();
	BaseFile = CConfigString(file);
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

	CConfigString s1 = ""; // some
	CConfigString s2 = ""; // section
	CConfigString s3 = ""; // parentS
	CConfigString s4 = ""; // val



	/*s1.reserve(256);
	s2.reserve(256);
	s3.reserve(256);
	s4.reserve(256);*/

	char c;
	char cn = 0;
	bool isRunning = true;
	while(isRunning)
	{
		if(cn)
		{
			c = cn;
			cn = 0;
		}
		else
		{
			c = fgetc(fp);
			if(feof(fp))
			{
				c = '\n';
				isRunning = false;
			}
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
				parseInclude(s1, baseDir(file));
			}
			if(bValue)
			{
				//int pos = min(s4.find_last_not_of(' '), s4.find_last_not_of('\t'));
				////////
				int pos = 0;
				int s4len = s4.length();
				for(int i = 0; i < s4len; ++i)
				{
					if(!isspace((unsigned char)s4[i]))
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
						for(AssotiativeArray<CConfigString, CValue>::Iterator i = m_mSections[s3].mValues.begin(); i; ++i)
						{
							m_mSections[s2].mValues[i.first].val = i.second->val;
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
					if(!isspace((unsigned char)s4[i]))
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
						for(AssotiativeArray<CConfigString, CValue>::Iterator i = m_mSections[s3].mValues.begin(); i; ++i)
						{
							m_mSections[s2].mValues[i.first].val = i.second->val;
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
			else if(bValue)
			{
				if((c != ' ' && c != '\t') || s4.length())
				{
					s4 += c;
				}
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

void CConfig::modify(AssotiativeArray<CConfigString, CSection> & sections, AssotiativeArray<CConfigString, CValue> & values, CConfigString IncName)
{
	for(AssotiativeArray<CConfigString, CValue>::Iterator i = m_mFinalValues.begin(); i; ++i)
	{
		values[i.first].val = i.second->val;
	}

	for(AssotiativeArray<CConfigString, CSection>::Iterator i = m_mSections.begin(); i; ++i)
	{
		if(!sections.KeyExists(i.first))
		{
			sections[i.first].parent = i.second->parent;
			sections[i.first].native = false;
			sections[i.first].Include = IncName;
		}
		for(AssotiativeArray<CConfigString, CValue>::Iterator j = m_mSections[i.first].mValues.begin(); j; ++j)
		{
			sections[i.first].mValues[j.first].val = j.second->val;
		}
	}
}

int CConfig::parseInclude(CConfigString & file, const CConfigString & dir)
{
	//printf("Parsing include %s fromdir = %s\n", file.c_str(), dir.c_str());
	FILE * pF = fopen((dir + file).c_str(), "r");
	if(pF)
	{
		fclose(pF);
		CInclude inc;

		inc.pParser = new CConfig(/*(dir + file).c_str()*/);
		inc.pParser->open((dir + file).c_str());

		inc.name = baseName(dir + file);
		/*		if(inc.name == "gl.ltx")
		{
		_asm
		{
		int 3;
		};
		}*/
		inc.pParser->modify(m_mSections, m_mFinalValues, dir + file);
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

const char* CConfig::getErrorFile()
{
	return ErrorFile.c_str();
}

const char* CConfig::getKey(const char * section, const char * key)
{
	CConfigString keys(key);
	CConfigString sections(section);
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

const char* CConfig::getKeyName(const char* section, int key)
{
	//CConfigString keys(key);
	CConfigString sections(section);
	if(m_mSections.KeyExists(sections))
	{
		if((int)m_mSections[sections].mValues.Size() > key)
		{
			int countiter = 0;
			AssotiativeArray<CConfigString, CConfig::CValue>::Iterator iter = m_mSections[sections].mValues.begin();
			for(int i = 0; i < key && iter; ++i)
			{
				++iter;
				++countiter;
			}
			if(countiter == key)
				return iter.first->c_str();
		}
		return 0;
	}
	return 0;
}

const char* CConfig::getSectionName(int num)
{
	if((int)m_mSections.Size() > num)
	{
		int countiter = 0;
		AssotiativeArray<CConfigString, CConfig::CSection>::Iterator iter = m_mSections.begin();
		for(int i = 0; i < num && iter; ++i)
		{
			++iter;
			++countiter;
		}

		if(countiter == num)
			return iter.first->c_str();
	}
	return(NULL);
}

void CConfig::set(const char * sectionp, const char * key, const char * val)
{
	CConfigString sections(sectionp);
	CConfigString keys(key);
	CConfigString vals(val);

	if(!m_mSections.KeyExists(sections))
	{
		CSection s;
		s.native = true;
		m_mSections[sections] = s;
		//create section
	}
	CSection *s = &m_mSections[sections];
	//s->isModified = true;
	if(s->mValues.KeyExists(keys))
	{
		CValue * v = &s->mValues[keys];
		if(v->val != vals)
		{
			s->isModified = true;
			v->isModified = true;
			v->val = vals;
		}
	}
	else
	{
		CValue v;
		v.isModified = true;
		v.val = vals;
		s->mValues[keys] = v;
		s->isModified = true;
	}
}

int CConfig::save()
{
	static const bool *s_pbDebug = GET_PCVAR_BOOL("dbg_config_save");
	if(*s_pbDebug)
	{
		printf(COLOR_GRAY "====== " COLOR_CYAN "CConfig::save() " COLOR_GRAY "======" COLOR_RESET "\n");
	}
	int terror = 0;
	for(AssotiativeArray<CConfigString, CSection>::Iterator i = m_mSections.begin(); i; ++i)
	{
		if(*s_pbDebug)
		{
			printf("Testing section: " COLOR_LGREEN "%s" COLOR_RESET "...", i.first->c_str());
		}
		if(i.second->isModified)
		{
			if(*s_pbDebug)
			{
				printf(COLOR_YELLOW " modified" COLOR_RESET "\n");
			}
			for(AssotiativeArray<CConfigString, CValue>::Iterator j = i.second->mValues.begin(); j; ++j)
			{
				if(*s_pbDebug)
				{
					printf("  testing key: " COLOR_LGREEN "%s" COLOR_RESET "...", j.first->c_str());
				}
				if(j.second->isModified)
				{
					if(*s_pbDebug)
					{
						printf(COLOR_YELLOW " modified" COLOR_RESET "\n");
					}
					if(i.second->native) // Write to BaseFile
					{
						if(*s_pbDebug)
						{
							printf("    writing to base file " COLOR_CYAN "%s" COLOR_RESET "...\n", BaseFile.c_str());
						}
						terror = writeFile(BaseFile, *i.first, *j.first, j.second->val);
						if(terror != 0)
							goto end;
					}
					else // Write to i.second->Include
					{
						if(*s_pbDebug)
						{
							printf("    writing to include file " COLOR_CYAN "%s" COLOR_RESET "...\n", i.second->Include.c_str());
						}
						terror = writeFile(i.second->Include, *i.first, *j.first, j.second->val);
						if(terror != 0)
							goto end;
					}
				}
				else
				{
					if(*s_pbDebug)
					{
						printf(COLOR_GRAY " not modified" COLOR_RESET "\n");
					}
				}
			}
		}
		else
		{
			if(*s_pbDebug)
			{
				printf(COLOR_GRAY " not modified" COLOR_RESET "\n");
			}
		}
	}
end:
	if(*s_pbDebug)
	{
		printf(COLOR_GRAY "=============================" COLOR_RESET "\n");
	}
	return(terror);
}

int CConfig::writeFile(const CConfigString & name, CConfigString section, CConfigString key, const CConfigString & val)
{
	static const bool *s_pbDebug = GET_PCVAR_BOOL("dbg_config_save");
	//printf("W: %s\t[%s]: %s = %s\n", name.c_str(), section.c_str(), key.c_str(), val.c_str());
	FILE * pF = fopen(name.c_str(), "rb");
	if(pF)
	{
		if(*s_pbDebug)
		{
			printf("    file opened\n");
		}

		fseek(pF, 0, SEEK_END);
		UINT fl = ftell(pF);
		fseek(pF, 0, SEEK_SET);
		char * szData = new char[fl + 1];
		if(!szData)
		{
			printf(COLOR_LRED "Unable to allocate memory (%d) in CConfig::writeFile()" COLOR_RESET "\n", fl + 1);
			fclose(pF);
			return(-1);
		}
		fread(szData, 1, fl, pF);
		szData[fl] = 0;
		fclose(pF);
		UINT sl = section.length();
		UINT kl = key.length();
		bool sf = false;
		bool kf = false;
		bool se = false;
		UINT sp = 0;
		for(UINT i = 0; i < fl; ++i)
		{
			if(szData[i] == '[' && ((i > 0 && (szData[i - 1] == '\r' || szData[i - 1] == '\n')) || i == 0))
			{
				bool cmp = true;
				UINT j;
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
					while(i < fl && (szData[i] == '\r' || szData[i] == '\n'))
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
						while(i < fl && (szData[i] == ' ' || szData[i] == '\t'))
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
						if(f && (isspace((unsigned char)szData[j]) || szData[j] == '='))//KeyFound!
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
								mem_delete_a(szData);
								return -1;
							}
							fwrite(szData, 1, i, pF); // First file part, including key
							fwrite(" = ", 1, 3, pF);
							fwrite(val.c_str(), 1, val.length(), pF);
							//fwrite("\n", 1, 1, pF);
							fwrite(&szData[j], 1, fl - j, pF);
							fclose(pF);
							mem_delete_a(szData);
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
							while(i < fl && (szData[i] == '\r' || szData[i] == '\n'))
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
			if(*s_pbDebug)
			{
				printf("    adding key to section(sp=" COLOR_LCYAN "%d" COLOR_RESET ")\n", sp);
			}
			FILE * pF = fopen(name.c_str(), "wb");
			if(!pF)
			{
				ErrorFile = name;
				mem_delete_a(szData);
				return -1;
			}
			fwrite(szData, 1, sp, pF); // First file part
			fwrite(key.c_str(), 1, key.length(), pF);
			fwrite(" = ", 1, 3, pF);
			fwrite(val.c_str(), 1, val.length(), pF);
			fwrite("\n", 1, 1, pF);
			fwrite(&szData[sp], 1, fl - sp, pF);
			fclose(pF);
			mem_delete_a(szData);
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
			fwrite((CConfigString("\n[") + section + "]\n" + key + " = " + val + "\n").c_str(), sizeof(char), section.length() + key.length() + val.length() + 8, pF);
			fclose(pF);
			mem_delete_a(szData);
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
		fwrite((CConfigString("[") + section + "]\n" + key + " = " + val + "\n").c_str(), sizeof(char), section.length() + key.length() + val.length() + 7, pF);
		fclose(pF);
		return 0;
	}
	return 0;
}

int CConfig::getSectionCount()
{
	int c = m_mSections.Size();
	int size = m_vIncludes.size();
	for(int i = 0; i < size; ++i)
	{
		//m_vIncludes
		c += m_vIncludes[i].pParser->getSectionCount();
	}
	return(c);
}

int CConfig::getKeyCount()
{
	int c = m_mFinalValues.Size();
	int size = m_vIncludes.size();
	for(int i = 0; i < size; ++i)
	{
		c += m_vIncludes[i].pParser->getKeyCount();
	}
	return(c);
}

int CConfig::getKeyCount(const char* section)
{
	CConfigString sections(section);

	if(m_mSections.KeyExists(sections))
	{
		return(m_mSections[sections].mValues.Size());
	}
	return -1;
}

bool CConfig::sectionExists(const char * section)
{
	CConfigString sections(section);
	if(m_mSections.KeyExists(sections))
		return(true);
	return(false);
	//	return(m_mSections.count(section) != 0 && m_mSections[section].native);
}

bool CConfig::keyExists(const char * section, const char * key)
{
	CConfigString sections(section);
	if(m_mSections.KeyExists(sections))
		return(m_mSections[sections].mValues.KeyExists(key));
	return(false);
}

void CConfig::Release()
{
	this->clear();
	delete this;
}

void CConfig::clear()
{
	int size = m_vIncludes.size();
	for(int i = 0; i < size; ++i)
	{
		mem_release(m_vIncludes[i].pParser);
		mem_delete(m_vIncludes[i].pParser);
	}
	m_vIncludes.clear();
	m_mFinalValues.clear();
	m_mSections.clear();
	BaseFile = "\0";
}

AssotiativeArray<CConfigString, CConfig::CSection> * CConfig::getSections()
{
	return(&m_mSections);
}

CConfigString CConfig::getIncludeName(int i)
{
	if(i >= 0 && i < (int)m_vIncludes.size())
	{
		return(m_vIncludes[i].name);
	}
	return("");
}

//#############################################################################

CXConfig::CXConfig()
{
	m_pConfig = new CConfig();
}
CXConfig::~CXConfig()
{
	mem_delete(m_pConfig);
}

bool XMETHODCALLTYPE CXConfig::open(const char *szPath)
{
	return(m_pConfig->open(szPath) == 0);
}
bool XMETHODCALLTYPE CXConfig::save()
{
	return(m_pConfig->save() == 0);
}

const char* XMETHODCALLTYPE CXConfig::getKey(const char *szSection, const char *szKey)
{
	return(m_pConfig->getKey(szSection, szKey));
}
const char* XMETHODCALLTYPE CXConfig::getKeyName(const char *szSection, UINT uIndex)
{
	return(m_pConfig->getKeyName(szSection, uIndex));
}
const char* XMETHODCALLTYPE CXConfig::getSectionName(UINT uIndex)
{
	return(m_pConfig->getSectionName(uIndex));
}
void XMETHODCALLTYPE CXConfig::set(const char *szSection, const char *szKey, const char *szValue)
{
	return(m_pConfig->set(szSection, szKey, szValue));
}
UINT XMETHODCALLTYPE CXConfig::getSectionCount()
{
	return(m_pConfig->getSectionCount());
}
UINT XMETHODCALLTYPE CXConfig::getKeyCount()
{
	return(m_pConfig->getKeyCount());
}
UINT XMETHODCALLTYPE CXConfig::getKeyCount(const char *szSection)
{
	return(m_pConfig->getKeyCount(szSection));
}
bool XMETHODCALLTYPE CXConfig::sectionExists(const char *szSection)
{
	return(m_pConfig->sectionExists(szSection));
}
bool XMETHODCALLTYPE CXConfig::keyExists(const char *szSection, const char *szKey)
{
	return(m_pConfig->keyExists(szSection, szKey));
}
