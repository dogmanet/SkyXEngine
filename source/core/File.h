
#ifndef SXFILE_H
#define SXFILE_H

#include "sxcore.h"
#include <fstream>

class SXFile : public ISXFile
{
public:

	SXFile();
	~SXFile();
	void Release();
	int Open(const char* path, int type = CORE_FILE_TEXT);
	int Create(const char* path, int type = CORE_FILE_TEXT);
	int Add(const char* path, int type = CORE_FILE_TEXT);

	size_t ReadB(void* dest, size_t size);
	size_t WriteB(void* src, size_t size);

	size_t ReadT(const char* format, ...);
	size_t WriteT(const char* format, ...);

	size_t GetSize();
	int ReadChar();

	size_t GetPos();
	void SetPos(size_t pos);

	void Close();
	bool IsEOF();

protected:
	FILE * File;
};

#endif
