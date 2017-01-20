
#pragma once

void DialogSaveLevel(char* pathlevel)
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "SkyX level file(.lvl)\0*.lvl\0Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);
	ofn.lpstrInitialDir = GData::Pathes::Levels;

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetSaveFileName(&ofn) == TRUE)
		{
			sprintf(pathlevel,"%s",szFileName);
		}
	
}

void DialogLoadLevel(char* pathlevel)
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "SkyX level file(.lvl)\0*.lvl\0Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);
	ofn.lpstrInitialDir = GData::Pathes::Levels;

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetOpenFileName(&ofn) == TRUE)
		{
			sprintf(pathlevel,"%s",szFileName);
		}
	
}

void DialogLoadParticles(char* pathlevel)
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "SkyX effects file(.eff)\0*.eff\0Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);
	ofn.lpstrInitialDir = GData::Pathes::GameSource;

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetOpenFileName(&ofn) == TRUE)
		{
			sprintf(pathlevel,"%s",szFileName);
		}
	
}

void DialogSaveParticles(char* pathlevel)
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "SkyX effects file(.eff)\0*.eff\0Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);
	ofn.lpstrInitialDir = GData::Pathes::GameSource;

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetSaveFileName(&ofn) == TRUE)
		{
			sprintf(pathlevel,"%s",szFileName);
		}
	
}


void DialogLoadMesh(char* path_model)
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "dse file(.dse)\0*.dse\0Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);
	ofn.lpstrInitialDir = GData::Pathes::Meshes;

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetOpenFileName(&ofn) == TRUE && path_model)
		{
			sprintf(path_model,"%s",szFileName);
		}
	
}

void DialogSelectTexture(char* path_tex)
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "Все файлы\0*.*\0png file(.png)\0*.png\0dds file(.dds)\0*.dds\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);
	ofn.lpstrInitialDir = GData::Pathes::Textures;

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetOpenFileName(&ofn) == TRUE && path_tex)
		{
			sprintf(path_tex,"%s",szFileName);
		}
	
}

void DialogSelectSound(char* path_snd)
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "Все файлы\0*.*\0ogg file(.ogg)\0*.ogg\wave file(.wav)\0*.wav\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);
	ofn.lpstrInitialDir = GData::Pathes::Sounds;

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetOpenFileName(&ofn) == TRUE && path_snd)
		{
			sprintf(path_snd,"%s",szFileName);
		}
	
}

void DialogSelectVS(char* path_tex)
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "vertex shader file(.vs)\0*.vs\0Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);
	ofn.lpstrInitialDir = GData::Pathes::Shaders;

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetOpenFileName(&ofn) == TRUE && path_tex)
		{
			sprintf(path_tex,"%s",szFileName);
		}
	
}

void DialogSelectPS(char* path_tex)
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "pixel shader file(.ps)\0*.ps\0Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);
	ofn.lpstrInitialDir = GData::Pathes::Shaders;

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

		if(GetOpenFileName(&ofn) == TRUE && path_tex)
		{
			sprintf(path_tex,"%s",szFileName);
		}
	
}