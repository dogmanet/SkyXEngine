
#include <core\\file.h>

SXFile::SXFile()
{
	File = 0;
}

SXFile::~SXFile()
{
	
}

void SXFile::Release()
{
		if(File)
			fclose(File);
	mem_del(this);
}

int SXFile::Open(const char* path,int type)
{
	char* mode;
	if (type == CORE_FILE_BIN)
			mode = "rb";
	else if (type == CORE_FILE_TEXT)
			mode = "r+";
	File = fopen(path, mode);
		if(!File)
			return -1;
	return 0;
}

int SXFile::Create(const char* path,int type)
{
	char* mode;
	if (type == CORE_FILE_BIN)
			mode = "wb";
	else if (type == CORE_FILE_TEXT)
			mode = "w";
	File = fopen(path, mode);
	if (!File)
		return -1;
	return 0;
}

int SXFile::Add(const char* path,int type)
{
	char* mode;
	if (type == CORE_FILE_BIN)
			mode = "ab";
	else if (type == CORE_FILE_TEXT)
			mode = "a";
	File = fopen(path, mode);
	if (!File)
		return -1;
	return 0;
}

size_t SXFile::ReadB(void* dest, size_t size)
{
	return(fread(dest, size, 1, File));
}

size_t SXFile::WriteB(void* src, size_t size)
{
	return(fwrite(src, size, 1, File));
}

size_t SXFile::ReadT(const char* format, ...)
{
	va_list va;
	va_start(va, format);
	int* argn = va_arg(va, int*);
	int count = 0;
	int countwrite = 0;
	int cwr = 0;
	char tmpbuf[3];
	while (1)
	{
		if ((format + count * 2)[0] == '\0')
			break;

		memcpy(tmpbuf, format + count * 2,2);
		tmpbuf[2] = 0;
		int cwr = fscanf_s(File, tmpbuf, argn);
		if (cwr == EOF)
			break;
		countwrite += cwr;
		//int tll = ftell(File);
		argn = va_arg(va, int*);
		count++;
	}

	return countwrite;
}

size_t SXFile::WriteT(const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	int countout = vfprintf(File, format, ptr);
	va_end(ptr);
	return countout;
}

size_t SXFile::GetPos()
{
	if (IsEOF())
		return CORE_FILE_EOF;
	return ftell(File);
}

void SXFile::SetPos(size_t pos)
{
	fseek(File, SEEK_SET, pos);
}

size_t SXFile::GetSize()
{
	long size;
	long curr_pos = ftell(File);
	fseek(File, 0, SEEK_END);
	size = ftell(File);
	fseek(File, curr_pos, SEEK_SET);
	return size;
}

int SXFile::ReadChar()
{
	return fgetc(File);
}

void SXFile::Close()
{
	fclose(File);
}

bool SXFile::IsEOF()
{
	return feof(File);
}
