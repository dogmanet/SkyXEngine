#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include <xcommon/IFileSystem.h>

class FileSystem final : public IFileSystem
{
	bool fileExists(const char *szPath) override;

	size_t fileGetSize(const char *szPath) override;

    bool isFile(const char *szPath) override;

	bool isDirectory(const char *szPath) override;
};

#endif
