
#pragma once

SkyXEngine::Core::Loaders::CreatorTextures::CreatorTextures()
{

}

DWORD SkyXEngine::Core::Loaders::CreatorTextures::Add(UINT width,UINT height,UINT levels,DWORD usage,D3DFORMAT format,D3DPOOL pool,const char* name,float coeffullscreen)
{
	IDirect3DTexture9* objtex;
	Data::Device->CreateTexture(width, height, levels, usage, format,pool, &objtex, NULL);

	DWORD id = -1;
	bool isadd = true;

		for(DWORD i=0;i<Arr.size();i++)
		{
				if(Arr[i] == 0)
				{
					Arr[i]->Texture = objtex;
					sprintf(Arr[i]->Name,"%s",name);
					Arr[i]->CoefFullScreen = coeffullscreen;
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
	
			sprintf(tmpCT->Name,"%s",name);
			tmpCT->CoefFullScreen = coeffullscreen;
			tmpCT->Texture->GetLevelDesc(0,&(tmpCT->Desc));
			Arr.push_back(tmpCT);
			isadd = false;
		}

		if(!isadd)
			InLog("\ttexture[%s] is created\n",name);

	return id;
}

void SkyXEngine::Core::Loaders::CreatorTextures::Delete(const char* text)
{
		for(DWORD i=0;i<Arr.size();i++)
		{
				if(strcmp(text,Arr[i]->Name) == 0)
				{
					Arr[i]->Texture->Release();
					Arr[i]->Texture = 0;
					sprintf(Arr[i]->Name,"%s","");
				}
		}
}

void SkyXEngine::Core::Loaders::CreatorTextures::Delete(DWORD num)
{
		if(num < Arr.size())
		{
			Arr[num]->Texture->Release();
			Arr[num]->Texture = 0;
			sprintf(Arr[num]->Name,"%s","");
		}
}

DWORD SkyXEngine::Core::Loaders::CreatorTextures::GetNum(const char* text)
{
		for(DWORD i=0;i<Arr.size();i++)
		{
				if(strcmp(text,Arr[i]->Name) == 0)
				{
					return i;
				}
		}
}

void SkyXEngine::Core::Loaders::CreatorTextures::OnLostDevice()
{
	InLog("!!! release textures for render ...");
		for(DWORD i=0;i<Arr.size();i++)
		{
				if(Arr[i] && Arr[i]->Name[0] != 0)
				{
					ndx_release(Arr[i]->Texture);
				}
		}
	InLog("is ok\n");
}

void SkyXEngine::Core::Loaders::CreatorTextures::OnResetDevice()
{
	InLog("!!! reset textures for render ...");
		for(DWORD i=0;i<Arr.size();i++)
		{
				if(Arr[i]->Name[0] != 0)
				{
						if(Arr[i]->CoefFullScreen > 0)
							SkyXEngine::Core::Data::Device->CreateTexture(SkyXEngine::Core::Data::WidthHeight.x * Arr[i]->CoefFullScreen, SkyXEngine::Core::Data::WidthHeight.y * Arr[i]->CoefFullScreen, 1, Arr[i]->Desc.Usage, Arr[i]->Desc.Format,Arr[i]->Desc.Pool, &(Arr[i]->Texture), NULL);
						else
							SkyXEngine::Core::Data::Device->CreateTexture(Arr[i]->Desc.Width, Arr[i]->Desc.Height, 1, Arr[i]->Desc.Usage, Arr[i]->Desc.Format,Arr[i]->Desc.Pool, &(Arr[i]->Texture), NULL);
				}
		}
	InLog("is ok\n");
}

IDirect3DTexture9* SkyXEngine::Core::Loaders::CreatorTextures::GetTexture(const char* text)
{
		for(DWORD i=0;i<Arr.size();i++)
		{
				if(strcmp(text,Arr[i]->Name) == 0)
				{
					return Arr[i]->Texture;
				}
		}
}

IDirect3DTexture9* SkyXEngine::Core::Loaders::CreatorTextures::GetTexture(DWORD num)
{
		if(num < Arr.size())
		{
			IDirect3DTexture9* tmptex = Arr[num]->Texture;
			return Arr[num]->Texture;
		}
}