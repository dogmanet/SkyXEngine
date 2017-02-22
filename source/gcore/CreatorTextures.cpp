
#pragma once

#include <gcore\\creatortextures.h>

CreatorTextures::CreatorTextures()
{

}

CreatorTextures::~CreatorTextures()
{
	for (DWORD i = 0; i<Arr.size(); i++)
	{
		if (Arr[i])
		{
			mem_release(Arr[i]->Texture);
		}
		mem_delete(Arr[i]);
	}
}

ID CreatorTextures::Add(UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, const char* name, float coeffullscreen)
{
	IDirect3DTexture9* objtex;
	DXDevice->CreateTexture(width, height, levels, usage, format,pool, &objtex, NULL);

	ID id = -1;
	bool isadd = true;

		for(int i=0;i<Arr.size();i++)
		{
				if(Arr[i] == 0)
				{
					Arr[i]->Texture = objtex;
					sprintf(Arr[i]->Name,"%s",name);
					Arr[i]->CoefFullScreen = coeffullscreen;
					Arr[i]->Level = levels;
					Arr[i]->Texture->GetLevelDesc(0,&(Arr[i]->Desc));
					isadd = false;
					id = i;
				}
		}

		if(isadd)
		{
			id = Arr.size();
			CreatedTexture* tmpCT = new CreatedTexture();
			tmpCT->Texture = objtex;
			tmpCT->Level = levels;
			sprintf(tmpCT->Name,"%s",name);
			tmpCT->CoefFullScreen = coeffullscreen;
			tmpCT->Texture->GetLevelDesc(0,&(tmpCT->Desc));
			Arr.push_back(tmpCT);
			isadd = false;
		}

		if(!isadd)
			reportf(REPORT_MSG_LEVEL_NOTICE, "sgcore: render target[%s] is created, id = %d\n", name, id);

	return id;
}

void CreatorTextures::Delete(const char* text)
{
		for(int i=0;i<Arr.size();i++)
		{
				if(strcmp(text,Arr[i]->Name) == 0)
				{
					Arr[i]->Texture->Release();
					Arr[i]->Texture = 0;
					sprintf(Arr[i]->Name,"%s","");
				}
		}
}

void CreatorTextures::Delete(ID num)
{
		if(num < Arr.size())
		{
			Arr[num]->Texture->Release();
			Arr[num]->Texture = 0;
			sprintf(Arr[num]->Name,"%s","");
		}
}

ID CreatorTextures::GetNum(const char* text)
{
		for(DWORD i=0;i<Arr.size();i++)
		{
				if(strcmp(text,Arr[i]->Name) == 0)
				{
					return i;
				}
		}
}

void CreatorTextures::OnLostDevice()
{
	reportf(REPORT_MSG_LEVEL_WARRNING, "sgcore: release render targets ...\n");
		for(DWORD i=0;i<Arr.size();i++)
		{
			CreatedTexture* tmpct = Arr[i];
				if(Arr[i] /*&& Arr[i]->Name[0] != 0*/)
				{
					mem_release_del(Arr[i]->Texture);
				}
		}
	reportf(REPORT_MSG_LEVEL_NOTICE, "sgcore: release render targets success\n");
}

void CreatorTextures::OnResetDevice()
{
	reportf(REPORT_MSG_LEVEL_WARRNING, "sgcore: reset render targets ...\n");
		for(int i=0;i<Arr.size();i++)
		{
				if(Arr[i]/*->Name[0] != 0*/)
				{
						if(Arr[i]->CoefFullScreen > 0.001f)
							DXDevice->CreateTexture(D3DAPP.BackBufferWidth * Arr[i]->CoefFullScreen, D3DAPP.BackBufferHeight * Arr[i]->CoefFullScreen, 1, Arr[i]->Desc.Usage, Arr[i]->Desc.Format, Arr[i]->Desc.Pool, &(Arr[i]->Texture), NULL);
						else
							DXDevice->CreateTexture(Arr[i]->Desc.Width, Arr[i]->Desc.Height, Arr[i]->Level, Arr[i]->Desc.Usage, Arr[i]->Desc.Format, Arr[i]->Desc.Pool, &(Arr[i]->Texture), NULL);
				}
		}
	reportf(REPORT_MSG_LEVEL_NOTICE, "sgcore: reset render targets success\n");
}

IDirect3DTexture9* CreatorTextures::GetTexture(const char* text)
{
		for(int i=0;i<Arr.size();i++)
		{
				if(strcmp(text,Arr[i]->Name) == 0)
				{
					return Arr[i]->Texture;
				}
		}
}

IDirect3DTexture9* CreatorTextures::GetTexture(ID num)
{
	if (num < Arr.size())
	{
		IDirect3DTexture9* tmptex = Arr[num]->Texture;
		return Arr[num]->Texture;
	}
	else
	{
		dbg_break;
		return 0;
	}
}