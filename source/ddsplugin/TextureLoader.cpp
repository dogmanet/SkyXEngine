#include "TextureLoader.h"
#include "DDSFile.h"

CTextureLoader::CTextureLoader(IFileSystem* pFileSystem):
	m_pFileSystem(pFileSystem)
{
}

UINT XMETHODCALLTYPE CTextureLoader::getVersion()
{
	return(IXTEXTURELOADER_VERSION);
}

UINT XMETHODCALLTYPE CTextureLoader::getExtCount() const
{
	return(1);
}
const char* XMETHODCALLTYPE CTextureLoader::getExt(UINT uIndex) const
{
	assert(uIndex < getExtCount());
	switch(uIndex)
	{
	case 0:
		return("dds");
	}
	return(NULL);
}
const char* XMETHODCALLTYPE CTextureLoader::getExtText(UINT uIndex) const
{
	assert(uIndex < getExtCount());
	switch(uIndex)
	{
	case 0:
		return("DirectDraw Surface");
	}
	return(NULL);
}
const char* XMETHODCALLTYPE CTextureLoader::getAuthor() const
{
	return("D-AIRY @ DogmaNet");
}
const char* XMETHODCALLTYPE CTextureLoader::getCopyright() const
{
	return("Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2019");
}
const char* XMETHODCALLTYPE CTextureLoader::getDescription() const
{
	return("DDS texture loader");
}

void XMETHODCALLTYPE CTextureLoader::getInfo(XTextureInfo *pTextureInfo)
{
	pTextureInfo->type = getType();
	
	pTextureInfo->uWidth = m_ddsHeader.width;
	pTextureInfo->uHeight = m_ddsHeader.height;
	pTextureInfo->uDepth = m_ddsHeader.depth;

	pTextureInfo->uMipCount = 1;
	if(m_ddsHeader.flags & DDS_HEADER_FLAGS_MIPMAP)
	{
		pTextureInfo->uMipCount = m_ddsHeader.mipMapCount;
	}
}

bool XMETHODCALLTYPE CTextureLoader::open(const char *szFileName, const char *szArg)
{
	assert(!m_pCurrentFile && "File already opened!");
	if(m_pCurrentFile)
	{
		return(false);
	}
	
	m_pCurrentFile = m_pFileSystem->openFile(szFileName);
	if(!m_pCurrentFile)
	{
		return(false);
	}

	IFile *pFile = m_pCurrentFile;

	uint32_t uMagick = 0;
	memset(&m_ddsHeader, 0, sizeof(m_ddsHeader));
	memset(&m_dxt10Header, 0, sizeof(m_dxt10Header));
	
	pFile->readBin(&uMagick, sizeof(uMagick));
	if(uMagick != DDS_MAGIC)
	{
		return(false);
	}

	if(pFile->readBin(&m_ddsHeader, sizeof(m_ddsHeader)) != sizeof(m_ddsHeader))
	{
		return(false);
	}
	m_hasDXT10Header = (m_ddsHeader.ddspf.flags & DDS_FOURCC) && m_ddsHeader.ddspf.fourCC == MAKEFOURCC('D', 'X', '1', '0');
	
	if(m_hasDXT10Header && pFile->readBin(&m_dxt10Header, sizeof(m_dxt10Header)) != sizeof(m_dxt10Header))
	{
		return(false);
	}

	return(true);
}
GXTEXTURE_TYPE XMETHODCALLTYPE CTextureLoader::getType() const
{
	if(m_ddsHeader.caps2 & DDS_FLAGS_VOLUME)
	{
		return(GXTEXTURE_TYPE_3D);
	}
	if(m_ddsHeader.caps & DDS_SURFACE_FLAGS_CUBEMAP)
	{
		return(GXTEXTURE_TYPE_CUBE);
	}
	if(m_ddsHeader.caps & DDS_SURFACE_FLAGS_TEXTURE)
	{
		return(GXTEXTURE_TYPE_2D);
	}
	return(GXTEXTURE_TYPE_UNKNOWN);
}
bool XMETHODCALLTYPE CTextureLoader::loadAs2D(IXResourceTexture2D *pResource)
{
	if(getType() != GXTEXTURE_TYPE_2D)
	{
		return(false);
	}
	
	return(true);
}
bool XMETHODCALLTYPE CTextureLoader::loadAsCube(IXResourceTextureCube *pResource)
{
	if(getType() != GXTEXTURE_TYPE_CUBE)
	{
		return(false);
	}


	return(true);
}

void XMETHODCALLTYPE CTextureLoader::close()
{
	mem_release(m_pCurrentFile);
}
