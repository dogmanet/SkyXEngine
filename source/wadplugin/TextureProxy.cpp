#include "TextureProxy.h"
#include <core/IFile.h>
#include "WADFile.h"

CTextureProxy::CTextureProxy(IFileSystem *pFileSystem):
	m_pFileSystem(pFileSystem)
{
	scanForWADs();
}

const char* XMETHODCALLTYPE CTextureProxy::getDescription() const
{
	return("Half-Life WAD proxy");
}

bool XMETHODCALLTYPE CTextureProxy::resolveName(const char *szName, char *szOutput, UINT *puBufSize, bool *pbWantLoad)
{
	if(pbWantLoad)
	{
		*pbWantLoad = false;
	}

	for(UINT i = 0, l = m_aWADCache.size(); i < l; ++i)
	{
		WADFile &file = m_aWADCache[i];

		for(UINT j = 0, jl = file.aRecords.size(); j < jl; ++j)
		{
			if(!strcmp(file.aRecords[j].szName, szName))
			{
				size_t sizeBuf = file.sName.length() + 1 + strlen(szName) + 1; // "#", "\0"
				char *szFullPath = (char*)alloca(sizeof(char) * sizeBuf);
				sprintf(szFullPath, "%s#%s", file.sName.c_str(), szName);

				if(szOutput)
				{
					strncpy(szOutput, szFullPath, *puBufSize);
					szOutput[*puBufSize - 1] = 0;
				}
				else
				{
					*puBufSize = (UINT)sizeBuf + 1;
				}
				return(true);
			}
		}
	}

	return(false);
}

bool XMETHODCALLTYPE CTextureProxy::loadTexture(const char *szName, IXResourceTexture **ppOut)
{
	return(false);
}

bool XMETHODCALLTYPE CTextureProxy::scanForTextures(IXTextureProxyScanList **ppOut)
{
	*ppOut = new CTextureProxyScanList(this);
	return(true);
}

void CTextureProxy::scanForWADs()
{
	m_aWADCache.clearFast();

	IFileIterator *pIterator = m_pFileSystem->getFileList("textures", "wad");
	if(pIterator)
	{
		const char *szFile;
		while((szFile = pIterator->next()))
		{
			IFile *pFile = m_pFileSystem->openFile(szFile);
			if(pFile)
			{
				WADFile &wadFile = m_aWADCache[m_aWADCache.size()];
				wadFile.sName = szFile;

				WADHeader hdr;
				if(pFile->readBin(&hdr, sizeof(hdr)) != sizeof(hdr))
				{
					LibReport(REPORT_MSG_LEVEL_WARNING, "Invalid file header '%s'\n", szFile);
					goto endFile;
				}

				WADLump lump;
				pFile->setPos(hdr.uLumpsOffset);
			
				for(UINT i = 0; i < hdr.uTextures; ++i)
				{
					if(pFile->readBin(&lump, sizeof(lump)) != sizeof(lump))
					{
						LibReport(REPORT_MSG_LEVEL_WARNING, "Unexpected EOF '%s'\n", szFile);
						goto endFile;
					}

					if(lump.szName[0] == '+' && !(lump.szName[1] == '0' || lump.szName[1] == 'a' || lump.szName[1] == 'A'))
					{
						continue;
					}

					lump.szName[sizeof(lump.szName) - 1] = 0;

					if(lump.u8CompressionType == 0 && lump.u8Type == 0x43)
					{
						WADRecord &rec = wadFile.aRecords[wadFile.aRecords.size()];
						memcpy(rec.szName, lump.szName, sizeof(lump.szName));
					}
				}

endFile:
				mem_release(pFile);
			}
		}

		mem_release(pIterator);
	}
}

//#############################################################################

CTextureProxyScanList::CTextureProxyScanList(CTextureProxy *pTextureProxy):
	m_pTextureProxy(pTextureProxy)
{
	m_pTextureProxy->scanForWADs();

	for(UINT i = 0, l = m_pTextureProxy->m_aWADCache.size(); i < l; ++i)
	{
		auto &file = m_pTextureProxy->m_aWADCache[i];

		for(UINT j = 0, jl = file.aRecords.size(); j < jl; ++j)
		{
			auto &item = m_aItems[m_aItems.size()];

			item.sFile = file.sName.c_str();
			item.sName = file.aRecords[j].szName;
		}
	}
}
CTextureProxyScanList::~CTextureProxyScanList()
{
}

UINT XMETHODCALLTYPE CTextureProxyScanList::getCount() const
{
	return(m_aItems.size());
}
const char* XMETHODCALLTYPE CTextureProxyScanList::getItem(UINT uIdx, const char **pszFileName) const
{
	assert(uIdx < m_aItems.size());

	const Item &item = m_aItems[uIdx];

	if(pszFileName)
	{
		*pszFileName = item.sFile.c_str();
	}

	return(item.sName.c_str());
}
