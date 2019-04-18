
#include "SettingsWriter.h"
#include <core/sxcore.h>

void CSettingsWriter::loadFile(const char *szFile)
{
	IFile * f = Core_CrFile();
	if(f->open(szFile, CORE_FILE_BIN) < 0)
	{
		printf(COLOR_LRED "Couldn't exec '%s'\n" COLOR_RESET, szFile);
		return;
	}
	int len = (int)f->getSize() + 1;
	char * buf, *cbuf = NULL;
	if(len <= 4096)
	{
		buf = (char*)alloca(sizeof(char)* len);
	}
	else
	{
		buf = cbuf = new char[len];
	}

	f->readBin(buf, len - 1);
	buf[len - 1] = 0;

	//buf

	char *cur = buf;
	char *szFirst = NULL, *szSecond = NULL;

	while(*cur)
	{
		while(*cur && isspace(*cur)){++cur;}
		szFirst = cur;
		while(*cur && !isspace(*cur)){++cur;}
		if(*cur == '\r' || *cur == '\n')
		{
			*cur = 0;
			m_mCvarList[szFirst] = "";
		}
		if(*cur)
		{
			*cur = 0; ++cur;
		}
		while(*cur && isspace(*cur)){++cur;}
		szSecond = cur;
		while(*cur && !(*cur == '\r' || *cur == '\n'))
		{
			++cur;
		}
		if(*cur)
		{
			*cur = 0; ++cur;
		}
		if(fstrcmp(szFirst, "bind"))
		{
			m_mCvarList[szFirst] = szSecond;
		}
		else
		{
			char *cur2 = szSecond;
			while(*cur2 && !isspace(*cur2)){++cur2;}
			if(*cur2){*cur2 = 0; ++cur2;}
			while(*cur2 && isspace(*cur2)){++cur2;}
			m_mBindList[szSecond] = cur2;
		}
	}

	f->Release();

	mem_delete_a(cbuf);
}

void CSettingsWriter::setCVar(const char *szKey, const char *szValue)
{
	m_mCvarList[szKey] = szValue;
}

void CSettingsWriter::setBind(const char *szKey, const char *szCmd)
{
	m_mBindList[szKey] = szCmd;
}

void CSettingsWriter::unBind(const char *szKey)
{
	m_mBindList[szKey] = "";
}

void CSettingsWriter::saveFile(const char *szFile)
{
	IFile * f = Core_CrFile();
	if(f->create(szFile, CORE_FILE_BIN) < 0)
	{
		printf(COLOR_LRED "Couldn't save file '%s'\n" COLOR_RESET, szFile);
		return;
	}

	char buff[1024];

	for(auto i = m_mCvarList.begin(); i; i++)
	{
		sprintf(buff, "%s %s\n", i.first->c_str(), i.second->c_str());
		f->writeBin(buff, strlen(buff));
	}

	for(auto i = m_mBindList.begin(); i; i++)
	{
		if(i.second->length())
		{
			sprintf(buff, "bind %s %s\n", i.first->c_str(), i.second->c_str());
			f->writeBin(buff, strlen(buff));
		}
	}

	f->Release();
}
