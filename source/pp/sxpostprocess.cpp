
namespace SkyXEngine
{
namespace Graphics
{
namespace PostProcess
{
#pragma once

SXFileText::SXFileText()
{
	Position = 0; Size = 0;
}

int SXPostProcessGD::EngineTexture::AddTexture(const char* name, IDirect3DTexture9* tex)
{
		if(CountTextures >= SXPP_SETTINGS_SB_COUNT_TEX_FOR_USING-1)
			return 0;
		else
		{
			Texture[CountTextures] = tex;
			sprintf(Name[CountTextures],"%s",name);
			CountTextures++;
			return 1;
		}
}

SXPostProcessGD::EngineTexture::EngineTexture()
{
	CountTextures = 0;
}

SXPostProcessGD::SXPPGDData::SXPPGDData()
{
	Device = 0;
	PathForShaders = 0;
	PathForTextures = 0;
	PathForSave = 0;
	DataTex = EngineTexture();
}

void SXPostProcessGD::CreateScreenTexture()
{
	struct  VERTEX_SCREEN_TEXTURE {float x, y, z, tx, ty;};
	D3DXCreateMeshFVF(2, 4, D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_TEX1, Data.Device, &ScreenTexture);

	const float half_pixel_x = 0.5f / Data.SizeBackBuffer.x;
	const float half_pixel_y = 0.5f / Data.SizeBackBuffer.y;

	const float x = 1.0;
	const float y = 1.0;

	VERTEX_SCREEN_TEXTURE AddVertices[] =                                
	{                                                              
		{ -x, -y, 1.0f, 0.0f + half_pixel_x, 1.0f + half_pixel_y },                           
		{ -x,  y, 1.0f, 0.0f + half_pixel_x, 0.0f + half_pixel_y },                            
		{  x,  y, 1.0f, 1.0f + half_pixel_x, 0.0f + half_pixel_y },                            
		{  x, -y, 1.0f, 1.0f + half_pixel_x, 1.0f + half_pixel_y },  
	};  

	void* Vertices;                                                
 	SXPostProcessGD::ScreenTexture->LockVertexBuffer(0, (void**)&Vertices);
	memcpy (Vertices, AddVertices, sizeof(AddVertices));     
	SXPostProcessGD::ScreenTexture->UnlockVertexBuffer();

	WORD* i = 0;
	SXPostProcessGD::ScreenTexture->LockIndexBuffer(0, (void**)&i);
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;
	SXPostProcessGD::ScreenTexture->UnlockIndexBuffer();
}

void SXPostProcessGD::InitData(SXPPGDData *data)
{
	memcpy(&SXPostProcessGD::Data,data,sizeof(SXPostProcessGD::SXPPGDData));
	SXPostProcessGD::DataTex = EngineTexture();
}

IDirect3DTexture9* SXPostProcessGD::GetCurrentRenderTexture()
{
		if(SXPostProcessGD::IsCurrentTex)
			return SXPostProcessGD::Data.FinalScene2;
		else
			return SXPostProcessGD::Data.FinalScene;
}

IDirect3DTexture9** SXPostProcessGD::GetCurrentRenderTextureLP()
{
		if(SXPostProcessGD::IsCurrentTex)
			return &SXPostProcessGD::Data.FinalScene2;
		else
			return &SXPostProcessGD::Data.FinalScene;
}

IDirect3DTexture9* SXPostProcessGD::GetCurrentSendTexture()
{
		if(SXPostProcessGD::IsCurrentTex)
			return SXPostProcessGD::Data.FinalScene;
		else
			return SXPostProcessGD::Data.FinalScene2;
}

SXPostProcessError::SXPostProcessError()
{
	this->Code = SXPP_OK;
	this->Str[0] = 0;
	this->NumStr = 0;
}

SXPostProcessData::SXPostProcessData()
{
	this->RTNum = 0;
	this->SVSNum = 0;
	this->SPSNum = 0;
	this->SeqNum = 0;
	this->TexNum = 0;
	this->PassNum = 0;
}

SXPostProcessData::~SXPostProcessData()
{
		for(int i=0;i<SVSNum;i++)
			this->ShaderVS[i].~SXPPShader();

		for(int i=0;i<SPSNum;i++)
			this->ShaderPS[i].~SXPPShader();

		for(int i=0;i<RTNum;i++)
			this->RenderTarget[i].~SXPPRenderTarget();
}

SXPostProcessData::SXPPRenderTarget::SXPPRenderTarget()
{
	this->BlendScreen = false;
	this->Width = this->Height = 0;
	this->Format = 0;
	this->WidthStr[0] = this->HeightStr[0] = 0;
	this->Texture = 0;
}

SXPostProcessData::SXPPRenderTarget::~SXPPRenderTarget()
{
	SX_SAFE_RELEASE_DELETE(this->Texture);
}

SXPostProcessData::SXPPShader::SXPPShader()
{
	this->NumData = 0;
	this->CountMacro = 0;
}

SXPostProcessData::SXPPShader::~SXPPShader()
{
		for(int i=0;i<this->CountMacro-1;i++)
		{
			SX_SAFE_DELETE_A(this->Macro[i].Definition);
			SX_SAFE_DELETE_A(this->Macro[i].Name);
		}
}

void SXPostProcessData::SXPPShader::SetMacro(const char* name,const char* def)
{
	this->Macro[this->CountMacro].Name = new const char[strlen(name)+1];
	sscanf(name,"%s",this->Macro[this->CountMacro].Name);

	this->Macro[this->CountMacro].Definition = new const char[strlen(def)+1];
	sscanf(def,"%s",this->Macro[this->CountMacro].Definition);

	this->CountMacro++;
}

void SXPostProcessData::SXPPShader::SetEndMacro()
{
	this->Macro[this->CountMacro].Name = 0;
	this->Macro[this->CountMacro].Definition = 0;
	this->CountMacro++;
}

SXPostProcessData::SXPPTextures::SXPPTextures()
{
	this->IsOtherPP = false;
}

SXPostProcessData::SXPPTextures::~SXPPTextures()
{
	SX_SAFE_RELEASE_DELETE(this->Texture);
}

SXPostProcessData::SXPPPass::SXPPPass()
{
	this->CountTextures = 0;
}

SXRecValidate::SXRecValidate()
{
	this->Count=0;
}

void SXRecValidate::Add(int pp,int num)
{
	this->PP[this->Count] = pp;
	this->Num[this->Count] = num;
	this->Count++;
}

int SXRecValidate::Validate()
{
		for(int i=0;i<this->Count;i++)
		{
				for(int k=0;k<this->Count;k++)
				{
						if(i != k)
						{
								if(this->PP[i] == this->PP[k] && this->Num[i] == this->Num[k])
									return i;
						}
				}
		}
	return -1;
}

SXComTransferShaderData::SXComTransferShaderData()
{
	this->CountData = 0;
}

int SXComTransferShaderData::AddData(const char* name,void* data,long size)
{
		//проходим по массиву данных
		for(int i=0;i<this->CountData;i++)
		{
				//если вдруг внезапно находим переменную с таким же именем то заменяем ее значение
				if(strcmp(this->ShaderData[i].NameVar,name) == 0)
				{
					this->ShaderData[i].Data = data;
					this->ShaderData[i].CountBytes = size;
					return 1;
				}
		}
		
		if(this->CountData < 64-1)
		{
			sprintf(this->ShaderData[CountData].NameVar,"%s",name);
			this->ShaderData[this->CountData].Data = data;
			this->ShaderData[this->CountData].CountBytes = size;
			this->CountData++;
			return 0;
		}
		
	return -1;
}

int SXFileText::LoadFile(const char* path)
{
	FILE* file;

		if(!(file = fopen(path, "rt")))
		{
			return -1;
		}
	Size = 0;
		char text[1024];
		while(!feof(file))
		{
			fgets(text, 1024, file);
			Size += strlen(text);
		}

	fseek(file, 0, SEEK_SET);
	FileText = new char[Size+1];
	long tmpsize = 0;

		while(!feof(file))
		{
			fgets(text, 1024, file);
			long len = strlen(text);
			memcpy(FileText+tmpsize,text,len);
			tmpsize += len;
		}
	
	fseek(file, 0, SEEK_SET);
	fclose(file);

	FileText[Size] = 0;

	Position = 0;

	return 0;
}

SXFileText::~SXFileText()
{
	SX_SAFE_DELETE_A(FileText);
}

bool SXFileText::IsEndFile()
{
	return Position >= Size;
}

void SXFileText::SetPosition(DWORD pos)
{
	Position = pos;
}

DWORD SXFileText::GetPosition()
{
	return Position;
}

void SXFileText::GetText(char* buf,int size)
{
		if(size <= Size - Position)
		{
			memcpy((void*)buf,FileText + Position,size);
			buf[size] = 0;
			Position += size;
		}
		else
		{
			memcpy((void*)buf,FileText + Position,Size - Position);
			buf[Size - Position] = 0;
			Position = Size;
		}
}

void SXFileText::DeleteAll(char* str_b,char* str_e,bool size_end_elem,bool paste_new_str)
{
	bool GisEnd = true;
		while(GisEnd)
		{
			char* BCommentB = strstr(FileText,str_b);
				if(BCommentB)
				{
					char* BCommentE = strstr(BCommentB,str_e);

					int CountNewStr = 0;
						if(paste_new_str)
						{
							long sizenn = strlen(FileText) - (strlen(BCommentE) + (BCommentB - FileText)) + 1 + strlen(str_e);
							char* nn = new char[sizenn];
							memcpy(nn,FileText + (BCommentB - FileText),sizenn-1);
							nn[sizenn-1] = 0;
							char* strstrstr = nn;
								while((strstrstr = strstr(strstrstr,"\n")))
								{
									strstrstr += strlen("\n");
									CountNewStr++;
								}
							CountNewStr = strlen("\n")*CountNewStr;
						}
					long sizeNCB = (BCommentB - FileText) + CountNewStr + 1;
					char* NCB = new char[sizeNCB];
					
					memcpy(NCB,FileText,BCommentB - FileText);
						for(long i=0;i<CountNewStr;i++)
							NCB[(BCommentB - FileText) + i] = '\n';
					NCB[sizeNCB-1] = 0;

					char* NCE = new char[strlen(BCommentE) - (size_end_elem?strlen(str_e):0) + 1];
					memcpy(NCE,BCommentE+(size_end_elem?strlen(str_e):0),strlen(BCommentE) - (size_end_elem?strlen(str_e):0));
					NCE[strlen(BCommentE) - (size_end_elem?strlen(str_e):0)] = 0;

					memcpy(FileText,NCB,strlen(NCB));
					memcpy(FileText+strlen(NCB),NCE,strlen(NCE));
					FileText[strlen(NCB) + strlen(NCE)] = 0;

					//delete[] NCB,NCE; NCB = NCE = 0;
					SX_SAFE_DELETE_A(NCB);
					SX_SAFE_DELETE_A(NCE);
				}
				else
					GisEnd = false;
		}
	this->Size = strlen(this->FileText);
}

DWORD SXFileText::GetCurrentStr()
{
	DWORD CurrStr = 0;
		for(long i=0;i<this->Position;i++)
		{
				if(FileText[i] == '\n')
					CurrStr++;
		}
	return CurrStr;
}

void SXFileText::GetStr(char* buf,int size)
{
	DWORD nstr = SXStrFind(FileText,"\n",Position);
	bool IsEndFile = false;
		if(nstr == -1)
		{
			IsEndFile = true;
			nstr = Size;
		}

		if(nstr != -1 && nstr-Position <= size)
		{
			memcpy((void*)buf,FileText + Position,nstr-Position);
			buf[nstr-Position] = 0;
		}
		else if(nstr != -1 && nstr-Position > size)
		{
			memcpy((void*)buf,FileText + Position,size);
			buf[size] = 0;
		}

		if(IsEndFile)
			Position = Size;
		else
			Position = nstr + strlen("\n");
}

SXLoaderPPFile::SXLoaderPPFile()
{
	PPNum = 0;
	this->HowGlobalNSData = -1;
}

SXLoaderPPFile::~SXLoaderPPFile()
{
		for(int i=0;i<PPNum;i++)
		{
			PP[i].~SXPostProcessData();
		}
}

int SXLoaderPPFile::Load(const char* path,SXPostProcessError *error)
{
	char text[1024];

	bool GisEnd = true;

	SXFileText* TextFile = new SXFileText();
		// ERROR не удалось загрузить файл
		if(TextFile->LoadFile(path) == -1)
		{
				if(error != 0)
				{
					error->Code = SXPP_ERR_NOT_FILE;
					sprintf(error->Str,"%s",path);
					sprintf(error->GenFileError,"%s",__FILE__);
					error->GenStrError = __LINE__;
				}
			return SXPP_ERR_NOT_FILE;
		}
	//удаляем комментарии
	TextFile->DeleteAll("//","\n",false,false);
	TextFile->DeleteAll("/*","*/",true,true);

	GisEnd = true;
		
	GisEnd = true;
	bool IsPPNameFound = false;
	bool IsPPBeginFound = false;
		//находим 
		//{
		while(!TextFile->IsEndFile() && GisEnd)
		{

			TextFile->GetStr(text,1024);

				// ERROR если найдено имя постпроцесса и начало постпроцесса и вдруг находим начало следующего постпроцесса
				if(IsPPNameFound && IsPPBeginFound && strstr(text,"{{{"))
				{
						if(error != 0)
						{
							error->Code = SXPP_ERR_NEW_BEGIN_PP;
							error->NumStr = TextFile->GetCurrentStr();
							sprintf(error->Str,"%s",PP[PPNum].Name);
							sprintf(error->GenFileError,"%s",__FILE__);
							error->GenStrError = __LINE__;
						}
					return SXPP_ERR_NEW_BEGIN_PP;
				}

				//если не найдено имя постпроцесса, тогда ищем
				if(!IsPPNameFound)
				{
					char* strPP = strstr(text,"postprocess");

						if(strPP)
						{
							PP[PPNum].Name[0] = 0;
							sscanf(strPP + strlen("postprocess"), "%s", PP[PPNum].Name);
								// ERROR если не найдено имя пост-процесса
								if(PP[PPNum].Name[0] == 0)
								{
										if(error != 0)
										{
											error->Code = SXPP_ERR_NOT_FOUND_NAME;
											sprintf(error->Str, "%s",SXPP_DET_ITEM_POSTPROCESS);
											error->NumStr = TextFile->GetCurrentStr();
											sprintf(error->GenFileError,"%s",__FILE__);
											error->GenStrError = __LINE__;
										}
									return SXPP_ERR_NOT_FOUND_NAME;
								}

								if(strcmp(PP[PPNum].Name,SXPP_DET_ITEM_GDATA) == 0 && this->HowGlobalNSData > -1)
								{
										if(error != 0)
										{
											error->Code = SXPP_ERR_REDIFINITION_GDATA;
											sprintf(error->Str, "%s",path);
											sprintf(error->Str2, "%s",this->PathForGData);
											error->NumStr = TextFile->GetCurrentStr();
											sprintf(error->GenFileError,"%s",__FILE__);
											error->GenStrError = __LINE__;
										}
									return SXPP_ERR_REDIFINITION_GDATA;
								}
								else if(strcmp(PP[PPNum].Name,SXPP_DET_ITEM_GDATA) == 0)
								{
									sprintf(this->PathForGData,"%s",path);
									this->HowGlobalNSData = this->PPNum;
								}

							IsPPNameFound = true;
						}
				}
				//если не найдено начало постпроцесса, тогда ищем
				else if(!IsPPBeginFound)
				{
						if(strstr(text,"{{{"))
							IsPPBeginFound = true;
				}

				//если и имя и начало постпроцесса нет, но нашли конец его ... тогда он закончился
				if(IsPPNameFound && IsPPBeginFound && strstr(text,"}}}"))
				{
					IsPPNameFound = IsPPBeginFound = false;
					PPNum++;
				}


				if(IsPPNameFound && IsPPBeginFound)
				{
					bool tmpIsEnd = true;
					//TextFile->GetStr(text,1024);

					char tmptext[1024];
					tmptext[0] = 0;
					sscanf(text, "%s", tmptext);

 						if(strcmp(tmptext,SXPP_DET_ITEM_RENDER_TARGET) == 0)
						{
							//находим имя render_target
							char* tmpRTName = strstr(text,SXPP_DET_ITEM_RENDER_TARGET);
							PP[PPNum].RenderTarget[PP[PPNum].RTNum].Name[0] = 0;
							sscanf(tmpRTName + strlen(SXPP_DET_ITEM_RENDER_TARGET), "%s", PP[PPNum].RenderTarget[PP[PPNum].RTNum].Name);

								// ERROR если не найдено имя рендер тарджета
								if(strcmp(PP[PPNum].RenderTarget[PP[PPNum].RTNum].Name,"") == 0)
								{
										if(error != 0)
										{
											error->Code = SXPP_ERR_NOT_FOUND_NAME;
											sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
											error->NumStr = TextFile->GetCurrentStr();
											sprintf(error->GenFileError,"%s",__FILE__);
											error->GenStrError = __LINE__;
										}
									return SXPP_ERR_NOT_FOUND_NAME;
								}

							//находим начало 
							bool isFindBegin = false;
								while(!TextFile->IsEndFile() && !isFindBegin)
								{
									TextFile->GetStr(text,1024);
										for(int i=0;i<1024;i++)
										{
											if(text[i] == '{')
											{
												isFindBegin = true;
												break;
											}
											else
												isFindBegin = false;
										}
								}

								//если есть начало то юзаем
								if(isFindBegin)
								{
										// ERROR если превышен лимит элемента
										if(PP[PPNum].RTNum >= SXPP_SETTINGS_SB_RENDER_TARGET-1)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_MAX_NUM;
													sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
													sprintf(error->Str2, "%s",PP[PPNum].Name);
													error->NumStr = TextFile->GetCurrentStr();
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_MAX_NUM;
										}

										while(!TextFile->IsEndFile() && tmpIsEnd)
										{
											TextFile->GetStr(text,1024);
											char* tmpRTData;
												if(tmpRTData = strstr(text,SXPP_DET_ITEM_RENDER_TARGET_WIDTH))
												{

													sscanf(tmpRTData + strlen(SXPP_DET_ITEM_RENDER_TARGET_WIDTH), "%s", &PP[PPNum].RenderTarget[PP[PPNum].RTNum].WidthStr);
														if(strstr(PP[PPNum].RenderTarget[PP[PPNum].RTNum].WidthStr,SXPP_DET_ITEM_RENDER_TARGET_SIZE_FULL))
														{
															char* tmpFull;
															if(tmpFull = strstr(PP[PPNum].RenderTarget[PP[PPNum].RTNum].WidthStr,"/"))
															{
																float num = -1;
																sscanf(tmpFull+1, "%f", &num);
																	if(num <= 10 && num != 0 && num != -1)
																		PP[PPNum].RenderTarget[PP[PPNum].RTNum].Width = (SXPostProcessGD::Data.WidthHeightBackBuffer.x)/num;
																	// ERROR если num больше 10
																	else if(num > 0)
																	{
																			if(error != 0)
																			{
																				error->Code = SXPP_ERR_INVALID_VALUE;
																				sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
																				sprintf(error->Str2, "%s",SXPP_DET_ITEM_RENDER_TARGET_WIDTH);
																				sprintf(error->Str3, "%s","num > 10");
																				error->NumStr = TextFile->GetCurrentStr();
																				sprintf(error->GenFileError,"%s",__FILE__);
																				error->GenStrError = __LINE__;
																			}
																		return SXPP_ERR_INVALID_VALUE;
																	}
																	// ERROR не найден делитель num либо он отрицательный
																	else if(num == -1)
																	{
																			if(error != 0)
																			{
																				error->Code = SXPP_ERR_INVALID_VALUE;
																				sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
																				sprintf(error->Str2, "%s",SXPP_DET_ITEM_RENDER_TARGET_WIDTH);
																				sprintf(error->Str3, "%s","num = -1 or num is null");
																				error->NumStr = TextFile->GetCurrentStr();
																				sprintf(error->GenFileError,"%s",__FILE__);
																				error->GenStrError = __LINE__;
																			}
																		return SXPP_ERR_INVALID_VALUE;
																	}
															}
															else
																PP[PPNum].RenderTarget[PP[PPNum].RTNum].Width = SXPostProcessGD::Data.WidthHeightBackBuffer.x;
														}
														else
															sscanf(tmpRTData + strlen(SXPP_DET_ITEM_RENDER_TARGET_WIDTH), "%d", &PP[PPNum].RenderTarget[PP[PPNum].RTNum].Width);

														// ERROR если размер рендер тарджета в 2 раза превышает размеры бэк буфера
														if(PP[PPNum].RenderTarget[PP[PPNum].RTNum].Width/SXPostProcessGD::Data.WidthHeightBackBuffer.x > 2)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_INVALID_VALUE;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
																	sprintf(error->Str2, "%s",SXPP_DET_ITEM_RENDER_TARGET_WIDTH);
																	sprintf(error->Str3, "%s","width > 2*backbuffer");
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_INVALID_VALUE;
														}
												}
												else if(tmpRTData = strstr(text,SXPP_DET_ITEM_RENDER_TARGET_HEIGHT))
												{
													sscanf(tmpRTData + strlen(SXPP_DET_ITEM_RENDER_TARGET_HEIGHT), "%s", &PP[PPNum].RenderTarget[PP[PPNum].RTNum].HeightStr);
														if(strstr(PP[PPNum].RenderTarget[PP[PPNum].RTNum].HeightStr,"full"))
														{
															char* tmpFull;
																if(tmpFull = strstr(PP[PPNum].RenderTarget[PP[PPNum].RTNum].HeightStr,"/"))
																{
																	float num = -1;
																	sscanf(tmpFull+1, "%f", &num);
																		if(num <= 10 && num != 0 && num != -1)
																			PP[PPNum].RenderTarget[PP[PPNum].RTNum].Height = SXPostProcessGD::Data.WidthHeightBackBuffer.y/num;
																		// ERROR если num больше 10
																	else if(num > 0)
																	{
																			if(error != 0)
																			{
																				error->Code = SXPP_ERR_INVALID_VALUE;
																				sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
																				sprintf(error->Str2, "%s",SXPP_DET_ITEM_RENDER_TARGET_HEIGHT);
																				sprintf(error->Str3, "%s","num > 10");
																				error->NumStr = TextFile->GetCurrentStr();
																				sprintf(error->GenFileError,"%s",__FILE__);
																				error->GenStrError = __LINE__;
																			}
																		return SXPP_ERR_INVALID_VALUE;
																	}
																	// ERROR не найден делитель num либо он отрицательный
																	else if(num == -1)
																	{
																			if(error != 0)
																			{
																				error->Code = SXPP_ERR_INVALID_VALUE;
																				sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
																				sprintf(error->Str2, "%s",SXPP_DET_ITEM_RENDER_TARGET_HEIGHT);
																				sprintf(error->Str3, "%s","num = -1 or num is null");
																				error->NumStr = TextFile->GetCurrentStr();
																				sprintf(error->GenFileError,"%s",__FILE__);
																				error->GenStrError = __LINE__;
																			}
																		return SXPP_ERR_INVALID_VALUE;
																	}
																}
																else
																	PP[PPNum].RenderTarget[PP[PPNum].RTNum].Height = SXPostProcessGD::Data.WidthHeightBackBuffer.y;
														}
														else
															sscanf(tmpRTData + strlen(SXPP_DET_ITEM_RENDER_TARGET_HEIGHT), "%d", &PP[PPNum].RenderTarget[PP[PPNum].RTNum].Height);

														// ERROR если высота рендер тарджета превышает двойной размер бэкбуффера
														if(PP[PPNum].RenderTarget[PP[PPNum].RTNum].Height/SXPostProcessGD::Data.WidthHeightBackBuffer.y > 2)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_INVALID_VALUE;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
																	sprintf(error->Str2, "%s",SXPP_DET_ITEM_RENDER_TARGET_HEIGHT);
																	sprintf(error->Str3, "%s","height > 2*backbuffer");
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_INVALID_VALUE;
														}
												}
												else if(tmpRTData = strstr(text,SXPP_DET_ITEM_RENDER_TARGET_FORMAT))
												{
													char tmpFormat[256];
													tmpFormat[0] = 0;
													sscanf(tmpRTData + strlen(SXPP_DET_ITEM_RENDER_TARGET_FORMAT), "%s", tmpFormat);
														
														// ERROR если не удалось считать формат
														if(tmpFormat[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET_FORMAT);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}

														if(strcmp(tmpFormat,SXPP_RT_FORMAT_RGBA8) == 0)
															PP[PPNum].RenderTarget[PP[PPNum].RTNum].Format = D3DFMT_A8R8G8B8;
														else if(strcmp(tmpFormat,SXPP_RT_FORMAT_R5G6B5) == 0)
															PP[PPNum].RenderTarget[PP[PPNum].RTNum].Format = D3DFMT_R5G6B5;
														else if(strcmp(tmpFormat,SXPP_RT_FORMAT_R32F) == 0)
															PP[PPNum].RenderTarget[PP[PPNum].RTNum].Format = D3DFMT_R32F;
														else if(strcmp(tmpFormat,SXPP_RT_FORMAT_GR32F) == 0)
															PP[PPNum].RenderTarget[PP[PPNum].RTNum].Format = D3DFMT_G32R32F;
														else if(strcmp(tmpFormat,SXPP_RT_FORMAT_ABGR32F) == 0)
															PP[PPNum].RenderTarget[PP[PPNum].RTNum].Format = D3DFMT_A32B32G32R32F;
														else if(strcmp(tmpFormat,SXPP_RT_FORMAT_R16F) == 0)
															PP[PPNum].RenderTarget[PP[PPNum].RTNum].Format = D3DFMT_R16F;
														else if(strcmp(tmpFormat,SXPP_RT_FORMAT_GR16F) == 0)
															PP[PPNum].RenderTarget[PP[PPNum].RTNum].Format = D3DFMT_G16R16F;
														else if(strcmp(tmpFormat,SXPP_RT_FORMAT_ABGR16F) == 0)
															PP[PPNum].RenderTarget[PP[PPNum].RTNum].Format = D3DFMT_A16B16G16R16F;
														// ERROR если не найден формат из перечисленных
														else
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_INVALID_VALUE;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
																	sprintf(error->Str2, "%s",SXPP_DET_ITEM_RENDER_TARGET_FORMAT);
																	sprintf(error->Str3, "%s",tmpFormat);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_INVALID_VALUE;
														}
												}
												else if(tmpRTData = strstr(text,SXPP_DET_ITEM_RENDER_TARGET_BLEND_SCREEN))
												{
													PP[PPNum].RenderTarget[PP[PPNum].RTNum].BlendScreen = true;
												}
												else if(tmpRTData = strstr(text,"}"))
												{
													tmpIsEnd = false;
													break;
												}
										}

										// ERROR если ширина не найдена а смешивание не установлено
										if(PP[PPNum].RenderTarget[PP[PPNum].RTNum].WidthStr[0] == 0 && !PP[PPNum].RenderTarget[PP[PPNum].RTNum].BlendScreen)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_INVALID_VALUE;
													sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
													sprintf(error->Str2, "%s",SXPP_DET_ITEM_RENDER_TARGET_WIDTH);
													sprintf(error->Str3, "%s",SXPP_ERROR_MESSAGE_NO_VALUE);
													error->NumStr = TextFile->GetCurrentStr();
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_INVALID_VALUE;
										}

										// ERROR если высота не найдена а смешивание не установлено
										if(PP[PPNum].RenderTarget[PP[PPNum].RTNum].HeightStr[0] == 0 && !PP[PPNum].RenderTarget[PP[PPNum].RTNum].BlendScreen)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_INVALID_VALUE;
													sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
													sprintf(error->Str2, "%s",SXPP_DET_ITEM_RENDER_TARGET_HEIGHT);
													sprintf(error->Str3, "%s",SXPP_ERROR_MESSAGE_NO_VALUE);
													error->NumStr = TextFile->GetCurrentStr();
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_INVALID_VALUE;
										}

										// ERROR если формат не был найден а смешивание не установлено
										if(PP[PPNum].RenderTarget[PP[PPNum].RTNum].Format == 0 && !PP[PPNum].RenderTarget[PP[PPNum].RTNum].BlendScreen)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_INVALID_VALUE;
													sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
													sprintf(error->Str2, "%s",SXPP_DET_ITEM_RENDER_TARGET_FORMAT);
													sprintf(error->Str3, "%s",SXPP_ERROR_MESSAGE_NO_VALUE);
													error->NumStr = TextFile->GetCurrentStr();
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_INVALID_VALUE;
										}

									PP[PPNum].RTNum++;

										
								}
							tmpIsEnd = true;
						}
						else if(strcmp(tmptext,"textures") == 0)
						{
							//находим начало 
							bool isFindBegin = false;
								while(!TextFile->IsEndFile() && !isFindBegin)
								{
									TextFile->GetStr(text,1024);
										for(int i=0;i<1024;i++)
										{
											if(text[i] == '{')
											{
												isFindBegin = true;
												break;
											}
										}
								}

								//если есть начало то юзаем
								if(isFindBegin)
								{
										while(!TextFile->IsEndFile() && tmpIsEnd)
										{
											//fgets(text, 1024, file);
											TextFile->GetStr(text,1024);;
											char tmpTexData[256];
											sscanf(text, "%s", tmpTexData);
												if(strcmp(tmpTexData,"}") != 0)
												{
													char tmptmpTexType[256];
													char tmpTexName[256];
													char tmpTexID[256];
													tmpTexName[0] = 0;
													tmpTexID[0] = 0;
													tmptmpTexType[0] = 0;
													sscanf(text, "%s%s%s", tmpTexName,tmptmpTexType,tmpTexID);

														if(tmpTexName[0] != 0)
														{
																// ERROR если тип текстуры не удалось считать
																if(tmptmpTexType[0] == 0)
																{
																		if(error != 0)
																		{
																			error->Code = SXPP_ERR_NOT_FOUND_NAME;
																			sprintf(error->Str, "%s",SXPP_DET_ITEM_TEXTURES_NF_TYPE);
																			error->NumStr = TextFile->GetCurrentStr();
																			sprintf(error->GenFileError,"%s",__FILE__);
																			error->GenStrError = __LINE__;
																		}
																	return SXPP_ERR_NOT_FOUND_NAME;
																}

																// ERROR если id текстуры не удалось считать
																if(tmpTexID[0] == 0)
																{
																		if(error != 0)
																		{
																			error->Code = SXPP_ERR_NOT_FOUND_NAME;
																			sprintf(error->Str, "%s",SXPP_DET_ITEM_TEXTURES_NF_ID);
																			error->NumStr = TextFile->GetCurrentStr();
																			sprintf(error->GenFileError,"%s",__FILE__);
																			error->GenStrError = __LINE__;
																		}
																	return SXPP_ERR_NOT_FOUND_NAME;
																}

																// ERROR если превышен лимит элемента
																if(PP[PPNum].TexNum >= SXPP_SETTINGS_SB_TEXTURES-1)
																{
																		if(error != 0)
																		{
																			error->Code = SXPP_ERR_MAX_NUM;
																			sprintf(error->Str, "%s",SXPP_DET_ITEM_TEXTURES);
																			sprintf(error->Str2, "%s",PP[PPNum].Name);
																			error->NumStr = TextFile->GetCurrentStr();
																			sprintf(error->GenFileError,"%s",__FILE__);
																			error->GenStrError = __LINE__;
																		}
																	return SXPP_ERR_MAX_NUM;
																}

																sprintf(PP[PPNum].Textures[PP[PPNum].TexNum].Name,"%s",tmpTexName);
																sprintf(PP[PPNum].Textures[PP[PPNum].TexNum].ID,"%s",tmpTexID);

																if(strcmp(tmptmpTexType,SXPP_TEX_TYPE_LOAD_STR) == 0)
																	PP[PPNum].Textures[PP[PPNum].TexNum].Type = SXPP_TEX_TYPE_LOAD_NUM;
																else if(strcmp(tmptmpTexType,SXPP_TEX_TYPE_ENGINE_STR) == 0)
																	PP[PPNum].Textures[PP[PPNum].TexNum].Type = SXPP_TEX_TYPE_ENGINE_NUM;
																else if(strcmp(tmptmpTexType,SXPP_TEX_TYPE_PP_STR) == 0)
																	PP[PPNum].Textures[PP[PPNum].TexNum].Type = SXPP_TEX_TYPE_PP_NUM;
																// ERROR если тип текстуры не детализирован
																else
																{
																		if(error != 0)
																		{
																			error->Code = SXPP_ERR_INVALID_VALUE;
																			sprintf(error->Str, "%s",SXPP_DET_ITEM_TEXTURES);
																			sprintf(error->Str2, "%s",SXPP_DET_ITEM_TEXTURES_NF_TYPE);
																			sprintf(error->Str3, "%s",tmptmpTexType);
																			error->NumStr = TextFile->GetCurrentStr();
																			sprintf(error->GenFileError,"%s",__FILE__);
																			error->GenStrError = __LINE__;
																		}
																	return SXPP_ERR_INVALID_VALUE;
																}

															PP[PPNum].TexNum++;
														}
												}
												else if(strcmp(tmpTexData,"}") == 0)
												{
													tmpIsEnd = false;
												}
										}

									
								}
							tmpIsEnd = true;
						}
						else if(strcmp(tmptext,"shader_vs") == 0 || strcmp(tmptext,"shader_ps") == 0)
						{
							int* CountShaderNum;
							SXPostProcessData::SXPPShader* PPShader;
							char* tmpShaderName;
							bool IsShaderVS;
								if(strcmp(tmptext,"shader_vs") == 0)
								{
									IsShaderVS = true;
									CountShaderNum = &(PP[PPNum].SVSNum);
									PPShader = &(PP[PPNum].ShaderVS[*CountShaderNum]);
									tmpShaderName = strstr(text,"shader_vs");
								}
								else if(strcmp(tmptext,"shader_ps") == 0)
								{
									IsShaderVS = false;
									CountShaderNum = &(PP[PPNum].SPSNum);
									PPShader = &(PP[PPNum].ShaderPS[*CountShaderNum]);
									tmpShaderName = strstr(text,"shader_ps");
								}

							(*PPShader).Name[0] = 0;
							sscanf(tmpShaderName + strlen("shader_ps"), "%s", (*PPShader).Name);

								// ERROR если имя шейдера не удалось считать
								if((*PPShader).Name[0] == 0)
								{
										if(error != 0)
										{
											error->Code = SXPP_ERR_NOT_FOUND_NAME;
												if(IsShaderVS)
													sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_VS);
												else
													sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_PS);
											error->NumStr = TextFile->GetCurrentStr();
											sprintf(error->GenFileError,"%s",__FILE__);
											error->GenStrError = __LINE__;
										}
									return SXPP_ERR_NOT_FOUND_NAME;
								}

							//находим начало 
							bool isFindBegin = false;
								while(!TextFile->IsEndFile() && !isFindBegin)
								{
									//fgets(text, 1024, file);
									TextFile->GetStr(text,1024);
										for(int i=0;i<1024;i++)
										{
											if(text[i] == '{')
											{
												isFindBegin = true;
												break;
											}
										}
								}

								//если есть начало то юзаем
								if(isFindBegin)
								{
										// ERROR если количество элемента превышено
										if(*CountShaderNum >= SXPP_SETTINGS_SB_SHADER-1)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_MAX_NUM;
														if(IsShaderVS)
															sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_VS);
														else
															sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_PS);
													sprintf(error->Str2, "%s",PP[PPNum].Name);
													error->NumStr = TextFile->GetCurrentStr();
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_MAX_NUM;
										}

										while(!TextFile->IsEndFile() && tmpIsEnd)
										{
											//fgets(text, 1024, file);
											TextFile->GetStr(text,1024);
											char* tmpSVSData;
												if(tmpSVSData = strstr(text,SXPP_DET_ITEM_SHADER_PATH))
												{
													(*PPShader).Path[0] = 0;
													sscanf(tmpSVSData + strlen(SXPP_DET_ITEM_SHADER_PATH), "%s",(*PPShader).Path);
														// ERROR если не найден путь до шейдера
														if((*PPShader).Path[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																		/*if(IsShaderVS)
																			sprintf(error->Str, "%s%s",SXPP_DET_ITEM_SHADER_VS,SXPP_DET_ITEM_SHADER_PATH);
																		else
																			sprintf(error->Str, "%s%s",SXPP_DET_ITEM_SHADER_PS,SXPP_DET_ITEM_SHADER_PATH);*/
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_PATH);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}
												}
												else if(tmpSVSData = strstr(text,SXPP_DET_ITEM_SHADER_VERSION))
												{
													(*PPShader).Version[0] = 0;
													sscanf(tmpSVSData + strlen(SXPP_DET_ITEM_SHADER_VERSION), "%s", (*PPShader).Version);
														// ERROR если не найдена версия шейдера
														if((*PPShader).Version[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																		/*if(IsShaderVS)
																			sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_VS);
																		else
																			sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_PS);*/
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_PATH);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}
												}
												else if(tmpSVSData = strstr(text,SXPP_DET_ITEM_SHADER_MACRO))
												{
													char tmpMacroName[256], tmpMacroDef[256];
													tmpMacroName[0] = 0;
													tmpMacroDef[0] = 0;
													sscanf(tmpSVSData + strlen(SXPP_SHADER_DATA_TYPE_MACRO), "%s%s", tmpMacroName,tmpMacroDef);

														// ERROR имя маркоса не найдено
														if(tmpMacroName[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_MACRO);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}

														// WARNING определение маркоса не найдено
														if(tmpMacroDef[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																		/*if(IsShaderVS)
																			sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_VS);
																		else
																			sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_PS);*/
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_NF_DEF);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}
														
														// ERROR если превышаем лимит элемента
														if((*PPShader).CountMacro >= SXPP_SETTINGS_SB_SHADER_MACRO-1)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_MAX_NUM;
																		/*if(IsShaderVS)
																			sprintf(error->Str2, "%s",SXPP_DET_ITEM_SHADER_VS);
																		else
																			sprintf(error->Str2, "%s",SXPP_DET_ITEM_SHADER_PS);*/
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_MACRO);
																	sprintf(error->Str2, "%s",(*PPShader).Name);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_MAX_NUM;
														}

													(*PPShader).SetMacro(tmpMacroName,tmpMacroDef);
												}
												//инициализация данных глобальных данных шейдера из файла постпроцесса
												else if(tmpSVSData = strstr(text,SXPP_DET_ITEM_SHADER_INIT_DATA))
												{
													//находим начало 
													bool tmpisFindBegin = false;
														while(!TextFile->IsEndFile() && !tmpisFindBegin)
														{
															//fgets(text, 1024, file);
															TextFile->GetStr(text,1024);
																for(int i=0;i<1024;i++)
																{
																	if(text[i] == '{')
																	{
																		tmpisFindBegin = true;
																		break;
																	}
																}
														}
														
														//если нашли начало то юзаем
														if(tmpisFindBegin)
														{
															bool tmptmpIsEnd = true;
																while(!TextFile->IsEndFile() && tmptmpIsEnd)
																{
																	int tmpNumData = (*PPShader).NumData;
																	//fgets(text, 1024, file);
																	TextFile->GetStr(text,1024);
																	char tmpstrs[1024];
																	sscanf(text, "%s", tmpstrs);
																	
																		if(strcmp(tmpstrs,"}") != 0)
																		{
																			char tmpDataName[256];
																			char tmpDataType[256];
																			char tmpDataVsrt[256];
																			tmpDataName[0] = tmpDataType[0] = tmpDataVsrt[0] = 0;
																			sscanf(text, "%s%s%s", tmpDataName,tmpDataType,tmpDataVsrt);

																				if(tmpDataName[0] != 0)
																				{

																					sprintf((*PPShader).Data[tmpNumData].NameVar,"%s",tmpDataName);

																						// ERROR если не найден тип переменной для шейдера
																						if(tmpDataType[0] == 0)
																						{
																								if(error != 0)
																								{
																									error->Code = SXPP_ERR_NOT_FOUND_NAME;
																										/*if(IsShaderVS)
																											sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_VS);
																										else
																											sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_PS);*/
																									//sprintf(error->Str2, "%s",SXPP_DET_ITEM_SHADER_INIT_DATA);
																									sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_INIT_DATA_NF_TYPE);
																									error->NumStr = TextFile->GetCurrentStr();
																									sprintf(error->GenFileError,"%s",__FILE__);
																									error->GenStrError = __LINE__;
																								}
																							return SXPP_ERR_NOT_FOUND_NAME;	
																						}

																						// ERROR если не найдено значение переменной
																						if(tmpDataVsrt[0] == 0)
																						{
																								if(error != 0)
																								{
																									error->Code = SXPP_ERR_NOT_FOUND_NAME;
																										/*if(IsShaderVS)
																											sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_VS);
																										else
																											sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_PS);*/
																									//sprintf(error->Str2, "%s",SXPP_DET_ITEM_SHADER_INIT_DATA);
																									sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_INIT_DATA_NF_VALUE);
																									error->NumStr = TextFile->GetCurrentStr();
																									sprintf(error->GenFileError,"%s",__FILE__);
																									error->GenStrError = __LINE__;
																								}
																							return SXPP_ERR_NOT_FOUND_NAME;	
																						}

																					sprintf((*PPShader).Data[tmpNumData].Type,"%s",tmpDataType);
																					sprintf((*PPShader).Data[tmpNumData].ValueStr,"%s",tmpDataVsrt);

																						// ERROR если количество элемента превышено
																						if((*PPShader).NumData >= SXPP_SETTINGS_SB_SHADER_DATA-1)
																						{
																								if(error != 0)
																								{
																									error->Code = SXPP_ERR_MAX_NUM;
																										if(IsShaderVS)
																											sprintf(error->Str2, "%s",SXPP_DET_ITEM_SHADER_VS);
																										else
																											sprintf(error->Str2, "%s",SXPP_DET_ITEM_SHADER_PS);
																									sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_INIT_DATA);
																									error->NumStr = TextFile->GetCurrentStr();
																									sprintf(error->GenFileError,"%s",__FILE__);
																									error->GenStrError = __LINE__;
																								}
																							return SXPP_ERR_MAX_NUM;
																						}

																					char* tmpstr = strstr(text,(*PPShader).Data[tmpNumData].Type);
																						if(strcmp((*PPShader).Data[tmpNumData].Type,SXPP_SHADER_DATA_TYPE_FLOAT) == 0)
																						{
																							float* FloatValue = new float;
																							sscanf(tmpstr + strlen((*PPShader).Data[tmpNumData].Type), "%f", FloatValue);
																							(*PPShader).Data[tmpNumData].ValueData = FloatValue;
																							(*PPShader).Data[tmpNumData].CountBytes = sizeof(float);
																						}
																						else if(strcmp((*PPShader).Data[tmpNumData].Type,SXPP_SHADER_DATA_TYPE_FLOAT2) == 0)
																						{
																							float2* FloatValue = new float2;
																							sscanf(tmpstr + strlen((*PPShader).Data[tmpNumData].Type), "%f%f", &FloatValue->x,&FloatValue->y);

																							(*PPShader).Data[tmpNumData].ValueData = FloatValue;
																							(*PPShader).Data[tmpNumData].CountBytes = sizeof(float2);
																						}
																						else if(strcmp((*PPShader).Data[tmpNumData].Type,SXPP_SHADER_DATA_TYPE_FLOAT3) == 0)
																						{
																							float3* FloatValue = new float3;
																							sscanf(tmpstr + strlen((*PPShader).Data[tmpNumData].Type), "%f%f%f", &FloatValue->x,&FloatValue->y,&FloatValue->z);

																							(*PPShader).Data[tmpNumData].ValueData = FloatValue;
																							(*PPShader).Data[tmpNumData].CountBytes = sizeof(float3);
																						}
																						else if(strcmp((*PPShader).Data[tmpNumData].Type,SXPP_SHADER_DATA_TYPE_FLOAT4) == 0)
																						{
																							float4* FloatValue = new float4;
																							sscanf(tmpstr + strlen((*PPShader).Data[tmpNumData].Type), "%f%f%f%f", &FloatValue->x,&FloatValue->y,&FloatValue->z,&FloatValue->w);

																							(*PPShader).Data[tmpNumData].ValueData = FloatValue;
																							(*PPShader).Data[tmpNumData].CountBytes = sizeof(float4);
																						}
																						else if(strcmp((*PPShader).Data[tmpNumData].Type,SXPP_SHADER_DATA_TYPE_INT) == 0)
																						{
																							int* IntValue = new int;
																							sscanf(tmpstr + strlen((*PPShader).Data[tmpNumData].Type), "%d", IntValue);
																							(*PPShader).Data[tmpNumData].ValueData = IntValue;
																							(*PPShader).Data[tmpNumData].CountBytes = sizeof(int);
																						}
																						else if(strcmp((*PPShader).Data[tmpNumData].Type,SXPP_SHADER_DATA_TYPE_MATRIX) == 0)
																						{
																							float4x4* MatrixValue = new float4x4;
																							sscanf(tmpstr + strlen((*PPShader).Data[tmpNumData].Type), "%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f", 
																																															&MatrixValue->_11,&MatrixValue->_12,&MatrixValue->_13,&MatrixValue->_14 , 
																																															&MatrixValue->_21,&MatrixValue->_22,&MatrixValue->_23,&MatrixValue->_24 ,
																																															&MatrixValue->_31,&MatrixValue->_32,&MatrixValue->_33,&MatrixValue->_34 ,
																																															&MatrixValue->_41,&MatrixValue->_42,&MatrixValue->_43,&MatrixValue->_44);

																							(*PPShader).Data[tmpNumData].ValueData = MatrixValue;
																							(*PPShader).Data[tmpNumData].CountBytes = sizeof(float4x4);
																						}
																																										
																					(*PPShader).NumData++;
																				}
																		}
																		else if(strstr(text,"}"))
																		{
																			tmptmpIsEnd = false;
																		}
																}
														}
												}
												else if(tmpSVSData = strstr(text,"}"))
												{
													tmpIsEnd = false;
												}
										}
				
									(*PPShader).SetEndMacro();

									(*CountShaderNum)++;
								}
							tmpIsEnd = true;
						}
						else if(strcmp(tmptext,SXPP_DET_ITEM_PASS) == 0)
						{
							//находим имя pass
							char* tmpPassName = strstr(text,SXPP_DET_ITEM_PASS);
							PP[PPNum].Pass[PP[PPNum].PassNum].Name[0] = 0;
							sscanf(tmpPassName + strlen(SXPP_DET_ITEM_PASS), "%s", PP[PPNum].Pass[PP[PPNum].PassNum].Name);

								// ERROR если имя пасса не найдено
								if(PP[PPNum].Pass[PP[PPNum].PassNum].Name[0] == 0)
								{
										if(error != 0)
										{
											error->Code = SXPP_ERR_NOT_FOUND_NAME;
											sprintf(error->Str, "%s",SXPP_DET_ITEM_PASS);
											error->NumStr = TextFile->GetCurrentStr();
											sprintf(error->GenFileError,"%s",__FILE__);
											error->GenStrError = __LINE__;
										}
									return SXPP_ERR_NOT_FOUND_NAME;
								}

							//находим начало 
							bool isFindBegin = false;
								while(!TextFile->IsEndFile() && !isFindBegin)
								{
									//fgets(text, 1024, file);
									TextFile->GetStr(text,1024);
										for(int i=0;i<1024;i++)
										{
											if(text[i] == '{')
											{
												isFindBegin = true;
												break;
											}
										}
								}

								//если есть начало то юзаем
								if(isFindBegin)
								{

										// ERROR если это глобальное пространство имен, то в нем не допустимо наличие пассов
										if(strcmp(PP[PPNum].Name,SXPP_DET_ITEM_GDATA) == 0)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_GDATA_PASS;
													error->NumStr = TextFile->GetCurrentStr();
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_GDATA_PASS;
										}

										// ERROR если количество элемента превышено
										if(PP[PPNum].PassNum >= SXPP_SETTINGS_SB_PASS-1)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_MAX_NUM;
													sprintf(error->Str, "%s",SXPP_DET_ITEM_PASS);
													sprintf(error->Str2, "%s",PP[PPNum].Name);
													error->NumStr = TextFile->GetCurrentStr();
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_MAX_NUM;
										}

										while(!TextFile->IsEndFile() && tmpIsEnd)
										{
											//fgets(text, 1024, file);
											TextFile->GetStr(text,1024);
											char* tmpPassData;
												if(tmpPassData = strstr(text,SXPP_DET_ITEM_SHADER_VS))
												{
													PP[PPNum].Pass[PP[PPNum].PassNum].ShaderVS[0] = 0;
													sscanf(tmpPassData + strlen(SXPP_DET_ITEM_SHADER_VS), "%s", PP[PPNum].Pass[PP[PPNum].PassNum].ShaderVS);
														// ERROR если не найдено имя вершинного шейдера
														if(PP[PPNum].Pass[PP[PPNum].PassNum].ShaderVS[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																	//sprintf(error->Str, "%s",SXPP_DET_ITEM_PASS);
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_VS);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}
												}
												else if(tmpPassData = strstr(text,SXPP_DET_ITEM_SHADER_PS))
												{
													PP[PPNum].Pass[PP[PPNum].PassNum].ShaderPS[0] = 0;
													sscanf(tmpPassData + strlen(SXPP_DET_ITEM_SHADER_PS), "%s", PP[PPNum].Pass[PP[PPNum].PassNum].ShaderPS);
														// ERROR если не найдено имя пиксельного шейдера
														if(PP[PPNum].Pass[PP[PPNum].PassNum].ShaderPS[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																	//sprintf(error->Str, "%s",SXPP_DET_ITEM_PASS);
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_SHADER_PS);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}
												}
												else if(tmpPassData = strstr(text,SXPP_DET_ITEM_PASS_TEXTURE))
												{
													int CountTexture = PP[PPNum].Pass[PP[PPNum].PassNum].CountTextures;
													char tmpTexFilterMag[256];
													char tmpTexFilterMin[256];
													char tmpTexFilterMip[256];
													char tmpTexAdress[256];
													tmpTexFilterMag[0] = 0;tmpTexFilterMin[0] = 0;tmpTexFilterMip[0] = 0;
													tmpTexAdress[0] = 0;
													//PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Slot - 1;
													int tmpSlot = -1;
													char tmpTexName[256];
													tmpTexName[0] = 0;
													sscanf(tmpPassData + strlen(SXPP_DET_ITEM_TEXTURES), "%s%d%s%s%s%s", tmpTexName,&tmpSlot,tmpTexAdress,tmpTexFilterMag,tmpTexFilterMin,tmpTexFilterMip);

														// ERROR если имя текстуры не найдено
														if(tmpTexName[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																	//sprintf(error->Str, "%s",SXPP_DET_ITEM_TEXTURES);
																	//sprintf(error->Str2, "%s",SXPP_DET_ITEM_PASS);
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_TEXTURES_NF_NAME);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}

														// ERROR если слот для текстуры не найден
														if(tmpSlot == -1)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																	//sprintf(error->Str, "%s",SXPP_DET_ITEM_TEXTURES);
																	//sprintf(error->Str2, "%s",SXPP_DET_ITEM_PASS);
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_PASS_TEXTURE_NF_SLOT);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}

														// ERROR если адрессацию считать не удалось
														if(tmpTexAdress[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																	//sprintf(error->Str, "%s",SXPP_DET_ITEM_TEXTURES);
																	//sprintf(error->Str2, "%s",SXPP_DET_ITEM_PASS);
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_PASS_TEXTURE_NF_ADRESS);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}

														// ERROR если фильтрацию считать не удалось
														if(tmpTexFilterMag[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																	//sprintf(error->Str, "%s",SXPP_DET_ITEM_TEXTURES);
																	//sprintf(error->Str2, "%s",SXPP_DET_ITEM_PASS);
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_PASS_TEXTURE_NF_FILTER);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}

														// ERROR если количество элемента превышено
														if(PP[PPNum].Pass[PP[PPNum].PassNum].CountTextures >= SXPP_SETTINGS_SB_PASS_TEX-1)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_MAX_NUM;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_TEXTURES);
																	sprintf(error->Str2, "%s",SXPP_DET_ITEM_PASS);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_MAX_NUM;
														}

														//если все верно и это действиетльно текстура и все в норме (а иначе бы уже вернуло ошибку)
														//то записываем данные для текстуры
														sprintf(PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Name,"%s",tmpTexName);
														PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Slot = tmpSlot;

														if(strcmp(tmpTexAdress,SXPP_PASS_TEX_ADRESS_WRAP) == 0)
															PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Adress = D3DTADDRESS_WRAP;
														else if(strcmp(tmpTexAdress,SXPP_PASS_TEX_ADRESS_CLAMP) == 0)
															PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Adress = D3DTADDRESS_CLAMP;
														else if(strcmp(tmpTexAdress,SXPP_PASS_TEX_ADRESS_MIRROR) == 0)
															PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Adress = D3DTADDRESS_MIRROR;
														// ERROR если тип адрессации не определен
														else
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_INVALID_VALUE;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_PASS);
																	sprintf(error->Str2, "%s",SXPP_DET_ITEM_TEXTURES);
																	sprintf(error->Str3, "%s",SXPP_DET_ITEM_PASS_TEXTURE_NF_ADRESS);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_INVALID_VALUE;
														}

														if(strcmp(tmpTexFilterMag,SXPP_PASS_TEX_FILTER_NONE) == 0)
															PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[0] = D3DTEXF_NONE;
														else if(strcmp(tmpTexFilterMag,SXPP_PASS_TEX_FILTER_LINEAR) == 0)
															PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[0] = D3DTEXF_LINEAR;
														else if(strcmp(tmpTexFilterMag,SXPP_PASS_TEX_FILTER_POINT) == 0)
															PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[0] = D3DTEXF_POINT;
														else if(strcmp(tmpTexFilterMag,SXPP_PASS_TEX_FILTER_ANISOTROPY) == 0)
															PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[0] = D3DTEXF_ANISOTROPIC;
														// ERROR если тип фильтрации не определен
														else
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_INVALID_VALUE;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_PASS);
																	sprintf(error->Str2, "%s",SXPP_DET_ITEM_TEXTURES);
																	sprintf(error->Str3, "%s",SXPP_DET_ITEM_PASS_TEXTURE_NF_FILTER);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_INVALID_VALUE;
														}

														if(tmpTexFilterMin[0] != 0 && tmpTexFilterMip[0] != 0)
														{
																if(strcmp(tmpTexFilterMin,SXPP_PASS_TEX_FILTER_NONE) == 0)
																	PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[1] = D3DTEXF_NONE;
																else if(strcmp(tmpTexFilterMin,SXPP_PASS_TEX_FILTER_LINEAR) == 0)
																	PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[1] = D3DTEXF_LINEAR;
																else if(strcmp(tmpTexFilterMin,SXPP_PASS_TEX_FILTER_POINT) == 0)
																	PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[1] = D3DTEXF_POINT;
																else if(strcmp(tmpTexFilterMin,SXPP_PASS_TEX_FILTER_ANISOTROPY) == 0)
																	PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[1] = D3DTEXF_ANISOTROPIC;

																if(strcmp(tmpTexFilterMip,SXPP_PASS_TEX_FILTER_NONE) == 0)
																	PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[2] = D3DTEXF_NONE;
																else if(strcmp(tmpTexFilterMip,SXPP_PASS_TEX_FILTER_LINEAR) == 0)
																	PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[2] = D3DTEXF_LINEAR;
																else if(strcmp(tmpTexFilterMip,SXPP_PASS_TEX_FILTER_POINT) == 0)
																	PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[2] = D3DTEXF_POINT;
																else if(strcmp(tmpTexFilterMip,SXPP_PASS_TEX_FILTER_ANISOTROPY) == 0)
																	PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[2] = D3DTEXF_ANISOTROPIC;
														}
														else
														{
															PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[1] = PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[0];
															PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[2] = PP[PPNum].Pass[PP[PPNum].PassNum].Texture[CountTexture].Filter[0];
														}
													PP[PPNum].Pass[PP[PPNum].PassNum].CountTextures++;	
												}
												else if(tmpPassData = strstr(text,SXPP_DET_ITEM_RENDER_TARGET))
												{
													PP[PPNum].Pass[PP[PPNum].PassNum].RT[0] = 0;
													sscanf(tmpPassData + strlen(SXPP_DET_ITEM_RENDER_TARGET), "%s", PP[PPNum].Pass[PP[PPNum].PassNum].RT);
														// ERROR если имя рендер тарджета не удалось считать
														if(PP[PPNum].Pass[PP[PPNum].PassNum].RT[0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_RENDER_TARGET);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}
												}
												else if(tmpPassData = strstr(text,"}"))
												{
													tmpIsEnd = false;
												}
										}

									PP[PPNum].PassNum++;
								}
							tmpIsEnd = true;
						}
						else if(strcmp(tmptext,SXPP_DET_ITEM_SEQUENCE) == 0)
						{
							//находим начало 
							bool isFindBegin = false;
								while(!TextFile->IsEndFile() && !isFindBegin)
								{
									//fgets(text, 1024, file);
									TextFile->GetStr(text,1024);
										for(int i=0;i<1024;i++)
										{
											if(text[i] == '{')
											{
												isFindBegin = true;
												break;
											}
										}
								}

								if(isFindBegin)
								{
										// ERROR если это глобальное пространство имен, то в нем не допустимо наличие пассов
										if(strcmp(PP[PPNum].Name,SXPP_DET_ITEM_GDATA) == 0)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_GDATA_SEQUENCE;
													error->NumStr = TextFile->GetCurrentStr();
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_GDATA_SEQUENCE;
										}

										while(!TextFile->IsEndFile() && tmpIsEnd)
										{
											//fgets(text, 1024, file);
											TextFile->GetStr(text,1024);
											char* tmpSeqData;
												if(tmpSeqData = strstr(text,SXPP_DET_ITEM_PASS))
												{
														// ERROR если количество элементов превысило лимит
														if(PP[PPNum].SeqNum >= SXPP_SETTINGS_SB_SEQUENCE-1)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_MAX_NUM;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_SEQUENCE);
																	sprintf(error->Str2, "%s",PP[PPNum].Name);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_MAX_NUM;
														}

													PP[PPNum].Sequence.Pass[PP[PPNum].SeqNum][0] = 0;
													sscanf(tmpSeqData + strlen(SXPP_DET_ITEM_PASS), "%s", PP[PPNum].Sequence.Pass[PP[PPNum].SeqNum]);
														// ERROR если имя пасса не удалось считать
														if(PP[PPNum].Sequence.Pass[PP[PPNum].SeqNum][0] == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_NOT_FOUND_NAME;
																	sprintf(error->Str, "%s",SXPP_DET_ITEM_PASS);
																	error->NumStr = TextFile->GetCurrentStr();
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_NOT_FOUND_NAME;
														}
													PP[PPNum].SeqNum++;
												}
												else if(tmpSeqData = strstr(text,"}"))
												{
													tmpIsEnd = false;
												}
										}
								}
							tmpIsEnd = true;
						}
				}
		}
	SX_SAFE_DELETE(TextFile);
	return SXPP_OK;
}

int SXLoaderPPFile::CheckValidate(const char* name_pp,SXPostProcessError *error)
{
	int HowPP = -1;
		for(int i=0;i<SXPP_SETTINGS_SB_PP;i++)
		{
				if(strcmp(name_pp,PP[i].Name) == 0)
					HowPP = i;
		}

		if(HowPP != -1)
			CheckValidate(HowPP,error);
		else
		{
				if(error != 0)
				{
					error->Code = SXPP_ERR_NOT_FOUND_PP;
					sprintf(error->Str,"%s",name_pp);
					sprintf(error->GenFileError,"%s",__FILE__);
					error->GenStrError = __LINE__;
				}
			return SXPP_ERR_NOT_FOUND_PP;
		}
	return SXPP_OK;
}


int SXLoaderPPFile::CheckValidate(int how_pp,SXPostProcessError *error)
{
		if(how_pp >= this->PPNum)
		{
				if(error != 0)
				{
					error->Code = SXPP_ERR_INVALID_NUMBER_OF_PP;
					sprintf(error->Str,"%d",how_pp);
					sprintf(error->Str2,"%d",this->PPNum);
					sprintf(error->GenFileError,"%s",__FILE__);
					error->GenStrError = __LINE__;
				}
			//ERROR
			return SXPP_ERR_INVALID_NUMBER_OF_PP;
		}

	int HowPP = how_pp;

	bool IsPassUnic = true;
	bool IsTextureUnic = true;
	bool IsRTUnic = true;
	bool IsSVSUnic = true;
	bool IsSPSUnic = true;

	bool IsTextureInPass = false;
	bool IsRTInPass = false;
	bool IsShaderVSInPass = false;
	bool IsShaderPSInPass = false;

	//{{проверка на уникальность итемов постэффекта
		//проверка на уникальность текстур
		for(int k=0;k<PP[HowPP].TexNum && IsTextureUnic;k++)
		{
				for(int j=0;j<PP[HowPP].TexNum && IsTextureUnic;j++)
				{
						if(strcmp(PP[HowPP].Textures[k].Name,PP[HowPP].Textures[j].Name) == 0 && k!=j)
						{
							IsTextureUnic = false;
								if(error != 0)
								{
									error->Code = SXPP_ERR_NON_UNIC_ITEM;
									//error->GenFileError = __FILE__;
									sprintf(error->Str,"%s",SXPP_DET_ITEM_TEXTURES);
									sprintf(error->Str2,"%s",PP[HowPP].Textures[k].Name);
									sprintf(error->GenFileError,"%s",__FILE__);
									error->GenStrError = __LINE__;
								}
							return SXPP_ERR_NON_UNIC_ITEM;
							//MessageBox(0,PP[HowPP].Textures[k].Name,"TextureUnic",0);
						}
				}
		}

		//проверка на уникальность рендер тарджетов
		for(int k=0;k<PP[HowPP].RTNum && IsRTUnic;k++)
		{
				for(int j=0;j<PP[HowPP].RTNum && IsRTUnic;j++)
				{
						if(strcmp(PP[HowPP].RenderTarget[k].Name,PP[HowPP].RenderTarget[j].Name) == 0 && k!=j)
						{
							IsRTUnic = false;
								if(error != 0)
								{
									error->Code = SXPP_ERR_NON_UNIC_ITEM;
									//error->GenFileError = __FILE__;
									sprintf(error->Str,"%s",SXPP_DET_ITEM_RENDER_TARGET);
									sprintf(error->Str2,"%s",PP[HowPP].RenderTarget[k].Name);
									sprintf(error->GenFileError,"%s",__FILE__);
									error->GenStrError = __LINE__;
								}
							return SXPP_ERR_NON_UNIC_ITEM;
							//ERROR 
							//MessageBox(0,PP[HowPP].RenderTarget[k].Name,"RTUnic",0);
						}
				}
		}

		//проверка на уникальность вершинных шейдеров
		for(int k=0;k<PP[HowPP].SVSNum && IsSVSUnic;k++)
		{
				for(int j=0;j<PP[HowPP].SVSNum && IsSVSUnic;j++)
				{
						if(strcmp(PP[HowPP].ShaderVS[k].Name,PP[HowPP].ShaderVS[j].Name) == 0 && k!=j)
						{
							IsSVSUnic = false;
								if(error != 0)
								{
									error->Code = SXPP_ERR_NON_UNIC_ITEM;
									//error->GenFileError = __FILE__;
									sprintf(error->Str,"%s",SXPP_DET_ITEM_SHADER_VS);
									sprintf(error->Str2,"%s",PP[HowPP].ShaderVS[k].Name);
									sprintf(error->GenFileError,"%s",__FILE__);
									error->GenStrError = __LINE__;
								}
							return SXPP_ERR_NON_UNIC_ITEM;
							//ERROR 
							//MessageBox(0,PP[HowPP].ShaderVS[k].Name,"SVSUnic",0);
						}
				}
		}

		//проверка на уникальность пиксельных шейдеров
		for(int k=0;k<PP[HowPP].SPSNum && IsSPSUnic;k++)
		{
				for(int j=0;j<PP[HowPP].SPSNum && IsSPSUnic;j++)
				{
						if(strcmp(PP[HowPP].ShaderPS[k].Name,PP[HowPP].ShaderPS[j].Name) == 0 && k!=j)
						{
							IsSPSUnic = false;
								if(error != 0)
								{
									error->Code = SXPP_ERR_NON_UNIC_ITEM;
									//error->GenFileError = __FILE__;
									sprintf(error->Str,"%s",SXPP_DET_ITEM_SHADER_PS);
									sprintf(error->Str2,"%s",PP[HowPP].ShaderPS[k].Name);
									sprintf(error->GenFileError,"%s",__FILE__);
									error->GenStrError = __LINE__;
								}
							return SXPP_ERR_NON_UNIC_ITEM;
							//ERROR 
							//MessageBox(0,PP[HowPP].ShaderPS[k].Name,"SPSUnic",0);
						}
				}
		}
		//}}

		//проходим по всем пассам в постэффекте и ищем ошибки
		for(int i=0;i<PP[HowPP].PassNum && IsPassUnic;i++)
		{
				//проверка на уникальность пассов
				for(int k=0;k<PP[HowPP].PassNum && IsPassUnic;k++)
				{
						if(strcmp(PP[HowPP].Pass[i].Name,PP[HowPP].Pass[k].Name) == 0 && i!=k)
						{
							IsPassUnic = false;
								if(error != 0)
								{
									error->Code = SXPP_ERR_NON_UNIC_ITEM;
									//error->GenFileError = __FILE__;
									sprintf(error->Str,"%s",SXPP_DET_ITEM_PASS);
									sprintf(error->Str2,"%s",PP[HowPP].Pass[k].Name);
									sprintf(error->GenFileError,"%s",__FILE__);
									error->GenStrError = __LINE__;
								}
							return SXPP_ERR_NON_UNIC_ITEM;
							//ERROR 
							//MessageBox(0,PP[HowPP].Pass[i].Name,"IsPassUnic",0);
						}
				}

				//проверка валидности текстур
				for(int k=0;k<PP[HowPP].Pass[i].CountTextures;k++)
				{
					IsTextureInPass = false;
						for(int j=0;j<PP[HowPP].TexNum && !IsTextureInPass;j++)
						{
								if(strcmp(PP[HowPP].Pass[i].Texture[k].Name,PP[HowPP].Textures[j].Name) == 0)
								{
									PP[HowPP].Pass[i].TextureNum[k] = j;
									PP[HowPP].Pass[i].TextureTypeNum[k] = 0;
									IsTextureInPass = true;
									PP[HowPP].Pass[i].IsCurrentPPTex[k] = true;
								}
						}

						//если указанную текстуру пасса не нашли в массиве текстур
						if(!IsTextureInPass)
						{
								//тогда может быть она находится среди рендер тарджетов
								for(int q=0;q<PP[HowPP].RTNum && !IsTextureInPass;q++)
								{
										//если нашли текстуру среди рендер тарджетов
										if(strcmp(PP[HowPP].Pass[i].Texture[k].Name,PP[HowPP].RenderTarget[q].Name) == 0)
										{
												//если этот рендер тарджет не есть тот в который сейчас рендерим тогда все ok
												if(strcmp(PP[HowPP].Pass[i].Texture[k].Name,PP[HowPP].Pass[i].RT) != 0)
												{
													PP[HowPP].Pass[i].TextureNum[k] = q;
													PP[HowPP].Pass[i].TextureTypeNum[k] = 1;
													IsTextureInPass = true;
													PP[HowPP].Pass[i].IsCurrentPPTex[k] = true;
												}
												//иначе это фэйл
												else
												{
														if(error != 0)
														{
															error->Code = SXPP_ERR_TEX_RT;
															//error->GenFileError = __FILE__;
															sprintf(error->Str,"%s",PP[HowPP].Pass[i].Texture[k].Name);
															sprintf(error->GenFileError,"%s",__FILE__);
															error->GenStrError = __LINE__;
														}
													return SXPP_ERR_TEX_RT;
												}
										}
								}
						}

						//если указанную текстуру пасса не нашли в текущем эффекте, то возможно она есть среди глобальных
						if(!IsTextureInPass)
						{
								//ищем среди текстур
								for(int j=0;j<PP[this->HowGlobalNSData].TexNum && !IsTextureInPass;j++)
								{
										if(strcmp(PP[HowPP].Pass[i].Texture[k].Name,PP[this->HowGlobalNSData].Textures[j].Name) == 0)
										{
											PP[HowPP].Pass[i].TextureNum[k] = j;
											PP[HowPP].Pass[i].TextureTypeNum[k] = 0;
											IsTextureInPass = true;
											PP[HowPP].Pass[i].IsCurrentPPTex[k] = false;
										}
								}

								//тогда может быть она находится среди рендер тарджетов
								for(int q=0;q<PP[this->HowGlobalNSData].RTNum && !IsTextureInPass;q++)
								{
										//если нашли текстуру среди рендер тарджетов
										if(strcmp(PP[HowPP].Pass[i].Texture[k].Name,PP[this->HowGlobalNSData].RenderTarget[q].Name) == 0)
										{
												//если этот рендер тарджет не есть тот в который сейчас рендерим тогда все ok
												if(strcmp(PP[HowPP].Pass[i].Texture[k].Name,PP[HowPP].Pass[i].RT) != 0)
												{
													PP[HowPP].Pass[i].TextureNum[k] = q;
													PP[HowPP].Pass[i].TextureTypeNum[k] = 1;
													IsTextureInPass = true;
													PP[HowPP].Pass[i].IsCurrentPPTex[k] = false;
												}
												//иначе это фэйл
												else
												{
														if(error != 0)
														{
															error->Code = SXPP_ERR_TEX_RT;
															//error->GenFileError = __FILE__;
															sprintf(error->Str,"%s",PP[HowPP].Pass[i].Texture[k].Name);
															sprintf(error->GenFileError,"%s",__FILE__);
															error->GenStrError = __LINE__;
														}
													return SXPP_ERR_TEX_RT;
												}
										}
								}
						}
						
						if(!IsTextureInPass)
						{
							//ERROR
							//MessageBox(0,PP[HowPP].Pass[i].Texture[k].Name,"Texture",0);
								if(error != 0)
								{
									error->Code = SXPP_ERR_NOT_FOUND_ITEM;
									//error->GenFileError = __FILE__;
									sprintf(error->Str,"%s",SXPP_DET_ITEM_TEXTURES);
									sprintf(error->Str2,"%s",PP[HowPP].Pass[i].Texture[k].Name);
									sprintf(error->Str3,"%s",PP[HowPP].Name);
									sprintf(error->GenFileError,"%s",__FILE__);
									error->GenStrError = __LINE__;
								}
							return SXPP_ERR_NOT_FOUND_ITEM;
						}
				}

			IsRTInPass = false;
				//проверка на валидность рендер тарджетов
				for(int k=0;k<PP[HowPP].RTNum && !IsRTInPass;k++)
				{
						if(strcmp(PP[HowPP].Pass[i].RT,PP[HowPP].RenderTarget[k].Name) == 0)
						{
							PP[HowPP].Pass[i].RTNum = k;
							IsRTInPass = true;
							PP[HowPP].Pass[i].IsCurrentPPRT = true;
						}
				}

				//если не найшли в текущем эффекте, то возможно есть в глобальном
				if(!IsRTInPass)
				{
						for(int k=0;k<PP[this->HowGlobalNSData].RTNum && !IsRTInPass;k++)
						{
								if(strcmp(PP[HowPP].Pass[i].RT,PP[this->HowGlobalNSData].RenderTarget[k].Name) == 0)
								{
									PP[HowPP].Pass[i].RTNum = k;
									IsRTInPass = true;
									PP[HowPP].Pass[i].IsCurrentPPRT = false;
								}
						}
				}
				if(!IsRTInPass)
				{
					//ERROR
					//MessageBox(0,PP[HowPP].Pass[i].RT,"Render target",0);
						if(error != 0)
						{
							error->Code = SXPP_ERR_NOT_FOUND_ITEM;
							//error->GenFileError = __FILE__;
							sprintf(error->Str,"%s",SXPP_DET_ITEM_RENDER_TARGET);
							sprintf(error->Str2,"%s",PP[HowPP].Pass[i].RT);
							sprintf(error->Str3,"%s",PP[HowPP].Name);
							sprintf(error->GenFileError,"%s",__FILE__);
							error->GenStrError = __LINE__;
						}
					return SXPP_ERR_NOT_FOUND_ITEM;
				}

			IsShaderVSInPass = false;
				//проверка на валидность вершинных шейдеров
				for(int k=0;k<PP[HowPP].SVSNum && !IsShaderVSInPass;k++)
				{
					//IsShaderVSInPass = false;

						if(strcmp(PP[HowPP].Pass[i].ShaderVS,PP[HowPP].ShaderVS[k].Name) == 0)
						{
							PP[HowPP].Pass[i].ShaderVSNum = k;
							IsShaderVSInPass = true;
							PP[HowPP].Pass[i].IsCurrentPPVS = true;
						}
				}

				//если в текущем эффекте не нашли то возможно он есть среди глобальных переменных
				if(!IsShaderVSInPass)
				{
						for(int k=0;k<PP[this->HowGlobalNSData].SVSNum && !IsShaderVSInPass;k++)
						{
							//IsShaderVSInPass = false;

								if(strcmp(PP[HowPP].Pass[i].ShaderVS,PP[this->HowGlobalNSData].ShaderVS[k].Name) == 0)
								{
									PP[HowPP].Pass[i].ShaderVSNum = k;
									IsShaderVSInPass = true;
									PP[HowPP].Pass[i].IsCurrentPPVS = false;
								}
						}
				}

				if(!IsShaderVSInPass)
				{
					//MessageBox(0,PP[HowPP].Pass[i].ShaderVS,"VS",0);
						if(error != 0)
						{
							error->Code = SXPP_ERR_NOT_FOUND_ITEM;
							//error->GenFileError = __FILE__;
							sprintf(error->Str,"%s",SXPP_DET_ITEM_SHADER_VS);
							sprintf(error->Str2,"%s",PP[HowPP].Pass[i].ShaderVS);
							sprintf(error->Str3,"%s",PP[HowPP].Name);
							sprintf(error->GenFileError,"%s",__FILE__);
							error->GenStrError = __LINE__;
						}
					return SXPP_ERR_NOT_FOUND_ITEM;
				}


			IsShaderPSInPass = false;
				//проверка на валидность пиксельных шейдеров
				for(int k=0;k<PP[HowPP].SPSNum && !IsShaderPSInPass;k++)
				{
						if(strcmp(PP[HowPP].Pass[i].ShaderPS,PP[HowPP].ShaderPS[k].Name) == 0)
						{
							PP[HowPP].Pass[i].ShaderPSNum = k;
							IsShaderPSInPass = true;
							PP[HowPP].Pass[i].IsCurrentPPPS = true;
						}
				}

				if(!IsShaderPSInPass)
				{
						for(int k=0;k<PP[this->HowGlobalNSData].SPSNum && !IsShaderPSInPass;k++)
						{
								if(strcmp(PP[HowPP].Pass[i].ShaderPS,PP[this->HowGlobalNSData].ShaderPS[k].Name) == 0)
								{
									PP[HowPP].Pass[i].ShaderPSNum = k;
									IsShaderPSInPass = true;
									PP[HowPP].Pass[i].IsCurrentPPPS = false;
								}
						}
				}

				if(!IsShaderPSInPass)
				{
					//MessageBox(0,PP[HowPP].Pass[i].ShaderPS,"PS",0);
						if(error != 0)
						{
							error->Code = SXPP_ERR_NOT_FOUND_ITEM;
							//error->GenFileError = __FILE__;
							sprintf(error->Str,"%s",SXPP_DET_ITEM_SHADER_PS);
							sprintf(error->Str2,"%s",PP[HowPP].Pass[i].ShaderPS);
							sprintf(error->Str3,"%s",PP[HowPP].Name);
							sprintf(error->GenFileError,"%s",__FILE__);
							error->GenStrError = __LINE__;
						}
					return SXPP_ERR_NOT_FOUND_ITEM;
				}

			//версии вершинного и пиксельного шейдеров для данного прохода в эффекте
			char vs_Ver[16];
			char ps_Ver[16];

			int tmpNumPPVS = HowPP;
				if(!PP[HowPP].Pass[i].IsCurrentPPVS)
					tmpNumPPVS = this->HowGlobalNSData;
			//{ находим версии шейдеров
				for(int ver=0;ver<PP[tmpNumPPVS].SVSNum;ver++)
				{
						if(strcmp(PP[HowPP].Pass[i].ShaderVS,PP[tmpNumPPVS].ShaderVS[ver].Name) == 0)
						{
							sprintf(vs_Ver,"%s",PP[tmpNumPPVS].ShaderVS[ver].Version);
							break;
						}
				}

			int tmpNumPPPS = HowPP;
				if(!PP[HowPP].Pass[i].IsCurrentPPPS)
					tmpNumPPPS = this->HowGlobalNSData;

				for(int ver=0;ver<PP[tmpNumPPPS].SPSNum;ver++)
				{
						if(strcmp(PP[HowPP].Pass[i].ShaderPS,PP[tmpNumPPPS].ShaderPS[ver].Name) == 0)
						{
							sprintf(ps_Ver,"%s",PP[tmpNumPPPS].ShaderPS[ver].Version);
							break;
						}
				}
			//}

				//если номера версий шейдеров не равны тогда углубляемся
				if(strcmp(vs_Ver+3,ps_Ver+3) != 0)
				{
						//если один из шейдеров имеет версию 3.0 а другой нет значит они не совместимы
						if(strcmp(vs_Ver,"vs_3_0") == 0 || strcmp(ps_Ver,"ps_3_0") == 0)
						{
								if(error != 0)
								{
									error->Code = SXPP_ERR_DISCREPANCY_VER_SHADER;
									//error->GenFileError = __FILE__;
									sprintf(error->Str,"%s",PP[HowPP].Pass[i].ShaderVS);
									sprintf(error->Str2,"%s",vs_Ver);
									sprintf(error->Str3,"%s",PP[HowPP].Pass[i].ShaderPS);
									sprintf(error->Str4,"%s",ps_Ver);
									sprintf(error->GenFileError,"%s",__FILE__);
									error->GenStrError = __LINE__;
								}
							return SXPP_ERR_DISCREPANCY_VER_SHADER;
						}
				}
		}


	//проверка на валидность пассов в последовательности
	bool IsPassInSeq = false;
		for(int i=0;i<PP[HowPP].SeqNum;i++)
		{
			IsPassInSeq = false;
				for(int k=0;k<PP[HowPP].PassNum && !IsPassInSeq;k++)
				{
						if(strcmp(PP[HowPP].Sequence.Pass[i],PP[HowPP].Pass[k].Name) == 0)
						{
							PP[HowPP].Sequence.PassNum[i] = k;
							IsPassInSeq = true;
						}
				}

				if(!IsPassInSeq)
				{
					//ERROR
					//MessageBox(0,PP[HowPP].Sequence.Pass[i],"Pass",0);
						if(error != 0)
						{
							error->Code = SXPP_ERR_NOT_FOUND_ITEM;
							//error->GenFileError = __FILE__;
							sprintf(error->Str,"%s",SXPP_DET_ITEM_PASS);
							sprintf(error->Str2,"%s",PP[HowPP].Sequence.Pass[i]);
							sprintf(error->Str3,"%s",PP[HowPP].Name);
							sprintf(error->GenFileError,"%s",__FILE__);
							error->GenStrError = __LINE__;
						}
					return SXPP_ERR_NOT_FOUND_ITEM;
				}
		}

		//проверка уникальности имен всех текстур и всех рендер тарджетов
		for(int pp=0;pp<this->PPNum;pp++)
		{
				for(int tex=0;tex<PP[pp].TexNum;tex++)
				{
						for(int tmppp=0;tmppp<this->PPNum;tmppp++)
						{
								if(pp != tmppp)
								{
										for(int tmptex=0;tmptex<PP[pp].TexNum;tmptex++)
										{
												if(tex != tmptex)
												{
														// ERROR найдены повторяющиеся имена текстур
														if(strcmp(PP[pp].Textures[tex].Name,PP[tmppp].Textures[tmptex].Name) == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_G_NON_UNIC_ITEM;
																	sprintf(error->Str,"%s",PP[pp].Name);
																	sprintf(error->Str2,"%s",PP[pp].Textures[tex].Name);
																	sprintf(error->Str3,"%s",SXPP_DET_ITEM_TEXTURES);
																	sprintf(error->Str4,"%s",PP[tmppp].Name);
																	sprintf(error->Str5,"%s",PP[tmppp].Textures[tmptex].Name);
																	sprintf(error->Str6,"%s",SXPP_DET_ITEM_TEXTURES);
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_G_NON_UNIC_ITEM;
														}
												}
										}
								}

								for(int tmprt=0;tmprt<PP[pp].RTNum;tmprt++)
								{
										// ERROR найдены повторяющиеся имена рендер тарджетов
										if(strcmp(PP[pp].Textures[tex].Name,PP[tmppp].RenderTarget[tmprt].Name) == 0)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_G_NON_UNIC_ITEM;
													sprintf(error->Str,"%s",PP[pp].Name);
													sprintf(error->Str2,"%s",PP[pp].Textures[tex].Name);
													sprintf(error->Str3,"%s",SXPP_DET_ITEM_TEXTURES);
													sprintf(error->Str4,"%s",PP[tmppp].Name);
													sprintf(error->Str5,"%s",PP[tmppp].RenderTarget[tmprt].Name);
													sprintf(error->Str6,"%s",SXPP_DET_ITEM_RENDER_TARGET);
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_G_NON_UNIC_ITEM;
										}
								}
						}
				}


				for(int rt=0;rt<PP[pp].RTNum;rt++)
				{
						for(int tmppp=0;tmppp<this->PPNum;tmppp++)
						{
								if(pp != tmppp)
								{
										for(int tmprt=0;tmprt<PP[pp].RTNum;tmprt++)
										{
												if(rt != tmprt)
												{
														// ERROR найдены повторяющиеся имена рендер тарджетов
														if(strcmp(PP[pp].RenderTarget[rt].Name,PP[tmppp].RenderTarget[tmprt].Name) == 0)
														{
																if(error != 0)
																{
																	error->Code = SXPP_ERR_G_NON_UNIC_ITEM;
																	sprintf(error->Str,"%s",PP[pp].Name);
																	sprintf(error->Str2,"%s",PP[pp].RenderTarget[rt].Name);
																	sprintf(error->Str3,"%s",SXPP_DET_ITEM_RENDER_TARGET);
																	sprintf(error->Str4,"%s",PP[tmppp].Name);
																	sprintf(error->Str5,"%s",PP[tmppp].RenderTarget[tmprt].Name);
																	sprintf(error->Str6,"%s",SXPP_DET_ITEM_RENDER_TARGET);
																	sprintf(error->GenFileError,"%s",__FILE__);
																	error->GenStrError = __LINE__;
																}
															return SXPP_ERR_G_NON_UNIC_ITEM;
														}
												}
										}
								}
						}
				}
		}

	return SXPP_OK;
}

int SXLoaderPPFile::InitData(const char* name_pp,SXPostProcessError *error)
{
	int HowPP = -1;
		for(int i=0;i<SXPP_SETTINGS_SB_PP;i++)
		{
				if(strcmp(name_pp,PP[i].Name) == 0)
					HowPP = i;
		}

		if(HowPP != -1)
			this->InitData(HowPP);
		else
		{
				if(error != 0)
				{
					error->Code = SXPP_ERR_NOT_FOUND_PP;
					sprintf(error->Str,"%s",name_pp);
					sprintf(error->GenFileError,"%s",__FILE__);
					error->GenStrError = __LINE__;
				}
			return SXPP_ERR_NOT_FOUND_PP;
		}

	return SXPP_OK;
}

int SXLoaderPPFile::InitData(int how_pp,SXPostProcessError *error)
{
		if(how_pp >= this->PPNum)
		{
				// ERROR если значение указывающее на постпроцесс больше либо равно общему количеству постпроцессов
				if(error != 0)
				{
					error->Code = SXPP_ERR_INVALID_VALUE;
					sprintf(error->Str2,"%d",this->PPNum);
					sprintf(error->Str,"%d",how_pp);
					sprintf(error->GenFileError,"%s",__FILE__);
					error->GenStrError = __LINE__;
				}
			return SXPP_ERR_INVALID_VALUE;
		}

	int HowPP = how_pp;

		//инициализация рендер тарджетов
		for(int i=0;i<PP[HowPP].RTNum;i++)
		{
				if(!PP[HowPP].RenderTarget[i].BlendScreen)
				{
					SXPostProcessGD::Data.Device->CreateTexture(PP[HowPP].RenderTarget[i].Width, PP[HowPP].RenderTarget[i].Height, 1, D3DUSAGE_RENDERTARGET, (D3DFORMAT)PP[HowPP].RenderTarget[i].Format,D3DPOOL_DEFAULT, &PP[HowPP].RenderTarget[i].Texture, NULL);
				}
		}

		//инициализация вершинных шейдеров
		for(int i=0;i<PP[HowPP].SVSNum;i++)
		{
			char tmpPath[256];
			sprintf(tmpPath, "%s%s",SXPostProcessGD::Data.PathForShaders, PP[HowPP].ShaderVS[i].Path);
			D3DXMACRO* marco = 0;
				if(PP[HowPP].ShaderVS[i].CountMacro > 1)
					marco = (D3DXMACRO*)PP[HowPP].ShaderVS[i].Macro;
			PP[HowPP].ShaderVS[i].Shader = Core::Data::ShadersManager->Load(0,PP[HowPP].ShaderVS[i].Path,PP[HowPP].ShaderVS[i].Path,false,marco);//SXShaderManager::SXWrapShader(SXPostProcessGD::Data.Device);
			//PP[HowPP].ShaderVS[i].Shader.Load(SXPostProcessGD::Data.PathForShaders,PP[HowPP].ShaderVS[i].Path,PP[HowPP].ShaderVS[i].Version,"main",marco);
			//SXShaderManager::SXLoadVertexShader(0,SXPostProcessGD::Data.Device,tmpPath,PP[HowPP].ShaderVS[i].Version,"main",&tmpshader_vs,0,marco);
		}

		//инициализация пиксельных шейдеров
		for(int i=0;i<PP[HowPP].SPSNum;i++)
		{
			char tmpPath[256];
			sprintf(tmpPath, "%s%s",SXPostProcessGD::Data.PathForShaders, PP[HowPP].ShaderPS[i].Path);
			D3DXMACRO* marco = 0;
				if(PP[HowPP].ShaderPS[i].CountMacro > 1)
					marco = PP[HowPP].ShaderPS[i].Macro;
			PP[HowPP].ShaderPS[i].Shader = Core::Data::ShadersManager->Load(1,PP[HowPP].ShaderPS[i].Path,PP[HowPP].ShaderPS[i].Path,false,marco);//SXShaderManager::SXWrapShader(SXPostProcessGD::Data.Device);
			//PP[HowPP].ShaderPS[i].Shader.Load(SXPostProcessGD::Data.PathForShaders,PP[HowPP].ShaderPS[i].Path,PP[HowPP].ShaderPS[i].Version,"main",marco);
			//SXShaderManager::SXLoadPixelShader(0,SXPostProcessGD::Data.Device,tmpPath,PP[HowPP].ShaderPS[i].Version,"main",&tmpshader_ps,0,marco);
		}

		//инициализация текстур
		for(int i=0;i<PP[HowPP].TexNum;i++)
		{
				//если тип текстуры == загрузаить
				if(PP[HowPP].Textures[i].Type == SXPP_TEX_TYPE_LOAD_NUM)
				{
					char tmpPath[256];
					sprintf(tmpPath, "%s%s",SXPostProcessGD::Data.PathForTextures, PP[HowPP].Textures[i].ID);
					D3DXCreateTextureFromFile(SXPostProcessGD::Data.Device,tmpPath,&PP[HowPP].Textures[i].Texture);
				}
				//иначе если тип текстуры == брать из другого постэффекта
				else if(PP[HowPP].Textures[i].Type == SXPP_TEX_TYPE_PP_NUM)
				{
					bool isend = false;
						//ищем по всем эффектам
						for(int pp=0;pp<this->PPNum && !isend;pp++)
						{
								//среди рендер тарджетов
								for(int rt=0;rt<PP[pp].RTNum && !isend;rt++)
								{
										if(strcmp(PP[HowPP].Textures[i].ID,PP[pp].RenderTarget[rt].Name) == 0)
										{
											PP[HowPP].Textures[i].IsOtherPP = true;
											PP[HowPP].Textures[i].IsTex = false;
											PP[HowPP].Textures[i].HowPP = pp;
											PP[HowPP].Textures[i].HowNum = rt;
											isend = true;
										}
								}

								//среди текстур
								for(int tex=0;tex<PP[pp].TexNum && !isend;tex++)
								{
										if(strcmp(PP[HowPP].Textures[i].ID,PP[pp].Textures[tex].Name) == 0)
										{
											PP[HowPP].Textures[i].IsOtherPP = true;
											PP[HowPP].Textures[i].IsTex = true;
											PP[HowPP].Textures[i].HowPP = pp;
											PP[HowPP].Textures[i].HowNum = tex;
											isend = true;
										}
								}
						}

						// ERROR в других эффектах данная текстура не найдена
						if(!isend)
						{
								if(error != 0)
								{
									error->Code = SXPP_ERR_NOT_FOUND_ITEM;
									sprintf(error->Str,"%s",SXPP_DET_ITEM_TEXTURES);
									sprintf(error->Str2,"%s",PP[HowPP].Textures[i].ID);
									sprintf(error->Str3,"%s",PP[HowPP].Name);
									sprintf(error->GenFileError,"%s",__FILE__);
									error->GenStrError = __LINE__;
								}
							return SXPP_ERR_NOT_FOUND_ITEM;	
						}
				}
				else 
				{
					bool isend = false;
						for(int tex=0;tex<SXPostProcessGD::DataTex.CountTextures;tex++)
						{
								if(strcmp(PP[HowPP].Textures[i].ID,SXPostProcessGD::DataTex.Name[tex]) == 0)
								{
									isend = true;
									PP[HowPP].Textures[i].Texture = SXPostProcessGD::DataTex.Texture[tex];
								}
						}

						// ERROR среди движковых текстур данной нет
						if(!isend && strcmp(PP[HowPP].Textures[i].ID,SXPP_TEX_ID_SCREEN) != 0)
						{
								if(error != 0)
								{
									error->Code = SXPP_ERR_NOT_FOUND_ITEM;
									sprintf(error->Str,"%s",SXPP_DET_ITEM_TEXTURES);
									sprintf(error->Str2,"%s",PP[HowPP].Textures[i].ID);
									sprintf(error->Str3,"%s",PP[HowPP].Name);
									sprintf(error->GenFileError,"%s",__FILE__);
									error->GenStrError = __LINE__;
								}
							return SXPP_ERR_NOT_FOUND_ITEM;	
						}
				}
		}

	return SXPP_OK;
}

int SXLoaderPPFile::InitDataAll(SXPostProcessError *error)
{
	int code = SXPP_OK;
		for(int i=0;i<this->PPNum;i++)
		{
			code = this->InitData(i,error);
				if(error->Code != SXPP_OK)
					return error->Code;
		}

		for(int pp=0;pp<this->PPNum;pp++)
		{
				for(int i=0;i<PP[pp].TexNum;i++)
				{
						if(PP[pp].Textures[i].Type == SXPP_TEX_TYPE_PP_NUM)
						{
							SXRecValidate validate = SXRecValidate();
							int errornum = -1;
								if(!IsValidTexturePP(pp,i,&validate,&errornum))
								{
										//неправильная рекурсия
										if(errornum != -1)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_NON_END_REC;
													sprintf(error->Str,"%s",PP[pp].Name);
													sprintf(error->Str2,"%s",PP[pp].Textures[i].Name);
													sprintf(error->Str3,"%s",PP[validate.PP[validate.Count-1]].Name);
													sprintf(error->Str4,"%s",PP[validate.PP[validate.Count-1]].Textures[validate.Num[validate.Count-1]].Name);
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_NON_END_REC;
										}
										//превышен лимит рекурсивных действий
										else if(validate.Count >= 64-1)
										{
												if(error != 0)
												{
													error->Code = SXPP_ERR_MAX_NUM_REC;
													sprintf(error->Str2,"%s",PP[pp].Textures[i].Name);
													sprintf(error->Str,"%s",PP[pp].Name);
													sprintf(error->GenFileError,"%s",__FILE__);
													error->GenStrError = __LINE__;
												}
											return SXPP_ERR_MAX_NUM_REC;	
										}
										//чтото пошло не так
										else
										{

										}
								}
						}
				}
		}
	return code;
}

int SXLoaderPPFile::CheckValidateAll(SXPostProcessError *error)
{
	int code = SXPP_OK;
		for(int i=0;i<this->PPNum;i++)
		{
			code = this->CheckValidate(i,error);
				if(error->Code != SXPP_OK)
					break;
		}
	return code;
}

int SXLoaderPPFile::GetIndexNumPP(const char* pp)
{
		for(int i=0;i<SXPP_SETTINGS_SB_PP;i++)
		{
				if(strcmp(pp,PP[i].Name) == 0)
					return i;
		}
	return -1;
}

int SXLoaderPPFile::GetTexInfo(const char* ppname,int ppnum,const char* name,D3DSURFACE_DESC* desc,int type)
{
	int pp = -1;
		if(ppnum != -1 && this->IsPPThere(ppnum))
		{
			pp = ppnum;
		}
		else if(ppname != 0)
		{
			pp = this->GetIndexNumPP(ppname);
				if(pp == -1)
					return -1;
		}
	IDirect3DTexture9* tmptex = this->GetTexture(0,pp,name,type);
		if(tmptex == 0)
			return -2;
		else
		{
			tmptex->GetLevelDesc(0,desc);
			return 0;
		}
}

//0 - добавленно
//1 - обновлено
//-1 - превышен лимит
//-2 - не найдено чтото
int SXLoaderPPFile::AddShaderData(const char* pp,int type_shader,const char* shader_name_pp,const char* shader_name,const char* pass,int passnum,const char* name_var,void* data,long size)
{
	int HowPP = GetIndexNumPP(pp);
		if(HowPP != -1)
		{
				//если тип вершинный шейдер то пробегаем по нему
				if(type_shader == SXPP_SHADER_VERTEX || type_shader == -1)
				{
						//если имя пасса и номер не заданы
						if((pass == 0) && passnum == -1)
						{
								//тогда ищем по всем пассам
								for(int i=0;i<PP[HowPP].SeqNum;i++)
								{
										//если нашли искомый шейдер
										if((shader_name_pp != 0 && strcmp(PP[HowPP].Pass[PP[HowPP].Sequence.PassNum[i]].ShaderVS,shader_name_pp) == 0) ||
											(shader_name != 0 && strcmp(PP[HowPP].ShaderVS[PP[HowPP].Pass[PP[HowPP].Sequence.PassNum[i]].ShaderVSNum].Path,shader_name) == 0))
										{
												return ComTrShaderDataVS[HowPP][i].AddData(name_var,data,size);
										}
								}
						}
						//иначе если номер пасса не задан но задано имя
						else if((pass != 0) && passnum == -1)
						{
								//ищем имя пасса
								for(int i=0;i<PP[HowPP].SeqNum;i++)
								{
										//если нашли искомый шейдер
										if((pass != 0 && strcmp(PP[HowPP].Pass[PP[HowPP].Sequence.PassNum[i]].Name,pass) == 0))
											return ComTrShaderDataVS[HowPP][i].AddData(name_var,data,size);
								}
						}
						//если задан только номер пасса, проверяем если такой вообще
						else if(passnum != -1)
						{
								for(int i=0;i<PP[HowPP].SeqNum;i++)
								{
										//если нашли искомый шейдер
										if(passnum == i)
											return ComTrShaderDataVS[HowPP][passnum].AddData(name_var,data,size);
								}
						}
				}
				//если тип пиксельный шейдер то пробегаем по нему
				if(type_shader == SXPP_SHADER_PIXEL || type_shader == -1)
				{
						//если имя пасса и номер не заданы
						if((pass == 0) && passnum == -1)
						{
								//тогда ищем по всем пассам
								for(int i=0;i<PP[HowPP].SeqNum;i++)
								{
										//если нашли искомый шейдер
										if((shader_name_pp != 0 && strcmp(PP[HowPP].Pass[PP[HowPP].Sequence.PassNum[i]].ShaderPS,shader_name_pp) == 0) ||
											(shader_name != 0 && strcmp(PP[HowPP].ShaderPS[PP[HowPP].Pass[PP[HowPP].Sequence.PassNum[i]].ShaderPSNum].Path,shader_name) == 0))
										{
												return ComTrShaderDataPS[HowPP][i].AddData(name_var,data,size);
										}
								}
						}
						//иначе если номер пасса не задан но задано имя
						else if((pass != 0) && passnum == -1)
						{
								//ищем имя пасса
								for(int i=0;i<PP[HowPP].SeqNum;i++)
								{
										//если нашли искомый шейдер
										if((pass != 0 && strcmp(PP[HowPP].Pass[PP[HowPP].Sequence.PassNum[i]].Name,pass) == 0))
												return ComTrShaderDataPS[HowPP][i].AddData(name_var,data,size);
								}
						}
						//если задан только номер пасса, проверяем если такой вообще
						else if(passnum != -1)
						{
								for(int i=0;i<PP[HowPP].SeqNum;i++)
								{
										//если нашли искомый шейдер
										if(passnum == i)
												return ComTrShaderDataPS[HowPP][passnum].AddData(name_var,data,size);
								}
						}
				}
		}
	return -2;
}

int SXLoaderPPFile::AddShaderData(const char* pp,int type_shader,int passnum,const char* name_var,void* data,long size)
{
	return this->AddShaderData(pp,type_shader,0,0,0,passnum,name_var,data,size);
}

int SXLoaderPPFile::AddShaderData(const char* pp,int type_shader,const char* pass,const char* name_var,void* data,long size)
{
	return this->AddShaderData(pp,type_shader,0,0,pass,-1,name_var,data,size);
}

int SXLoaderPPFile::AddShaderData(const char* pp,int type_shader,const char* shader_name_pp,const char* shader_name,const char* name_var,void* data,long size)
{
	return this->AddShaderData(pp,type_shader,shader_name_pp,shader_name,0,-1,name_var,data,size);
}

bool SXLoaderPPFile::IsValidTexturePP(int pp,int numtex,SXRecValidate *rec_validate,int *error)
{
	int tmp_pp = this->PP[pp].Textures[numtex].HowPP;
	int tmp_num = this->PP[pp].Textures[numtex].HowNum;
		//если превышаем количество рекурсивных вызовов, тогда завершаем поиск
		if(rec_validate->Count >= SXPP_SETTINGS_SB_MAX_COUNT_REC-1)
			return false;
	rec_validate->Add(tmp_pp,tmp_num);
	*error = rec_validate->Validate();
		//если рекурсия бесконечная
		if(*error != -1)
			return false;

		//если это текстура а не рендер тарджет
		if(this->PP[pp].Textures[numtex].IsTex)
		{
				if(this->PP[tmp_pp].Textures[tmp_num].Type == SXPP_TEX_TYPE_LOAD_NUM)
					return true;
				else if(this->PP[tmp_pp].Textures[tmp_num].Type == SXPP_TEX_TYPE_ENGINE_NUM)
				{
						for(int dttex=0;dttex<SXPostProcessGD::DataTex.CountTextures;dttex++)
						{
								if(strcmp(PP[tmp_pp].Textures[tmp_num].ID,SXPostProcessGD::DataTex.Name[dttex]) == 0)
										return true;
						}

						//если не смогли найти имя двжиковой текстуры то скорее всего это screen
						if(strcmp(PP[tmp_pp].Textures[tmp_num].ID,SXPP_TEX_ID_SCREEN) == 0)
							return true;
				}
				else if(this->PP[tmp_pp].Textures[tmp_num].Type == SXPP_TEX_TYPE_PP_NUM)
					return IsValidTexturePP(tmp_pp,tmp_num,rec_validate,error);
		}
		//иначе это рендер тарджет
		else
		{
				//если не стоит смешивание со сценой значит рендер тарджет отдельный, его и передаем
				if(!this->PP[tmp_pp].RenderTarget[tmp_num].BlendScreen)
					return true;
				//иначе передаем текстуру в которую последний раз рисовали
				else
					return true;
		}

	return false;
}

int SXLoaderPPFile::GetPPNum()
{
	return this->PPNum;
}

IDirect3DTexture9* SXLoaderPPFile::GetTexturePP(int pp,int numtex)
{
	int tmp_pp = this->PP[pp].Textures[numtex].HowPP;
	int tmp_num = this->PP[pp].Textures[numtex].HowNum;
		//если это текстура а не рендер тарджет
		if(this->PP[pp].Textures[numtex].IsTex)
		{
				if(this->PP[tmp_pp].Textures[tmp_num].Type == SXPP_TEX_TYPE_LOAD_NUM)
					return this->PP[tmp_pp].Textures[tmp_num].Texture;
				else if(this->PP[tmp_pp].Textures[tmp_num].Type == SXPP_TEX_TYPE_ENGINE_NUM)
				{
						for(int dttex=0;dttex<SXPostProcessGD::DataTex.CountTextures;dttex++)
						{
								if(strcmp(PP[tmp_pp].Textures[tmp_num].ID,SXPostProcessGD::DataTex.Name[dttex]) == 0)
										return SXPostProcessGD::DataTex.Texture[dttex];
						}

						//если не смогил найти имя двжиковой текстуры то скорее всего это screen
						if(strcmp(PP[tmp_pp].Textures[tmp_num].ID,SXPP_TEX_ID_SCREEN) == 0)
							return SXPostProcessGD::GetCurrentSendTexture();
				}
				else if(this->PP[tmp_pp].Textures[tmp_num].Type == SXPP_TEX_TYPE_PP_NUM)
					return GetTexturePP(tmp_pp,tmp_num);
		}
		//иначе это рендер тарджет
		else
		{
				//если не стоит смешивание со сценой значит рендер тарджет отдельный, его и передаем
				if(!this->PP[tmp_pp].RenderTarget[tmp_num].BlendScreen)
					return this->PP[tmp_pp].RenderTarget[tmp_num].Texture;
				//иначе передаем текстуру в которую последний раз рисовали
				else
					return SXPostProcessGD::GetCurrentSendTexture();
		}

	return 0;
}

bool SXLoaderPPFile::IsPPThere(int pp)
{
		for(int i=0;i<PPNum;i++)
		{
				if(pp == i)
					return true;
		}
	return false;
}

IDirect3DTexture9* SXLoaderPPFile::GetTexture(const char* ppname,int ppnum,const char* name,int type)
{
	int pp = -1;
		if(ppnum != -1 && IsPPThere(ppnum))
		{
			pp = ppnum;
		}
		else if(ppname != 0)
		{
			pp = this->GetIndexNumPP(ppname);
				if(pp == -1)
					return 0;
		}
				if(type == -1 || type == 0)
				{
						//по всем текстурам в эффекте
						for(int tex=0;tex<this->PP[pp].TexNum;tex++)
						{
								if(PP[pp].Textures[tex].IsTex)
								{
										//если типы совпадают
										if(type == -1 || PP[pp].Textures[tex].Type == type)
										{
												//имена тоже значит может быть наша текстура
												if(strcmp(PP[pp].Textures[tex].Name,name) == 0)
												{
														//если тип загружаемые то сразу возвраащем
														if(PP[pp].Textures[tex].Type == SXPP_TEX_TYPE_LOAD_NUM)
															return PP[pp].Textures[tex].Texture;
														//иначе если тип использовать из другого эффекта
														else if(PP[pp].Textures[tex].Type == SXPP_TEX_TYPE_PP_NUM)
															return this->GetTexturePP(pp,tex);
														//если движковая текстура то возвращаем ... хотя юзер и так должен иметь доступ к ним, 
														//ибо он и устанавливает их движковыми, но юзеры всякие бывают
														else if(PP[pp].Textures[tex].Type == SXPP_TEX_TYPE_ENGINE_NUM)
														{
																for(int dttex=0;dttex<SXPostProcessGD::DataTex.CountTextures;dttex++)
																{
																		if(strcmp(PP[pp].Textures[tex].ID,SXPostProcessGD::DataTex.Name[dttex]) == 0)
																			return SXPostProcessGD::DataTex.Texture[dttex];
																}

																if(strcmp(PP[pp].Textures[tex].ID,SXPP_TEX_ID_SCREEN) == 0)
																	return SXPostProcessGD::GetCurrentSendTexture();
														}
												}
										}
								}
								//иначе это рендер тарджет
								else if(strcmp(PP[PP[pp].Textures[tex].HowPP].RenderTarget[PP[pp].Textures[tex].HowNum].Name,name) == 0)
								{
										//если не стоит смешивание со сценой значит рендер тарджет отдельный, его и передаем
										if(!PP[PP[pp].Textures[tex].HowPP].RenderTarget[PP[pp].Textures[tex].HowNum].BlendScreen)
											return PP[PP[pp].Textures[tex].HowPP].RenderTarget[PP[pp].Textures[tex].HowNum].Texture;
										//иначе передаем текстуру в которую последний раз рисовали
										else
											return SXPostProcessGD::GetCurrentSendTexture();
								}
						}
				}

				//иначе если не нашли среди текстур либо установлен поиск среди рендер тарджетов
				if(type != 0)
				{
						for(int i=0;i<PP[pp].RTNum;i++)
						{
								if(strcmp(PP[pp].RenderTarget[i].Name,name) == 0)
								{
										if(PP[pp].RenderTarget[i].BlendScreen)
											return SXPostProcessGD::GetCurrentSendTexture();
										else
											return PP[pp].RenderTarget[i].Texture;
								}
						}
				}
	return 0;
}

IDirect3DTexture9* SXLoaderPPFile::GetTexture(const char* name,int type)
{
	bool tmpIsG = false;
	IDirect3DTexture9* tmptex = 0;
		//проходим по всем эффектам
		for(int pp=this->HowGlobalNSData;pp<this->PPNum;pp++)
		{
				
				//если текущий на поиск эффект не глобальный, ибо в нем мы уже искали и не нашли
				if(!tmpIsG || pp != this->HowGlobalNSData)
				{
						tmptex = this->GetTexture(0,pp,name,type);
							if(tmptex != 0)
								return tmptex;
						/*if(type == -1 || type == 0)
						{
								//по всем текстурам в эффекте
								for(int tex=0;tex<this->PP[pp].TexNum;tex++)
								{
										if(PP[pp].Textures[tex].IsTex)
										{
												//если типы совпадают
												if(type == -1 || PP[pp].Textures[tex].Type == type)
												{
														//имена тоже значит может быть наша текстура
														if(strcmp(PP[pp].Textures[tex].Name,name) == 0)
														{
																//если тип загружаемые то сразу возвраащем
																if(PP[pp].Textures[tex].Type == SXPP_TEX_TYPE_LOAD_NUM)
																	return PP[pp].Textures[tex].Texture;
																//иначе если тип использовать из другого эффекта
																else if(PP[pp].Textures[tex].Type == SXPP_TEX_TYPE_PP_NUM)
																	return this->GetTexturePP(pp,tex);
																//если движковая текстура то возвращаем ... хотя юзер и так должен иметь доступ к ним, 
																//ибо он и устанавливает их движковыми, но юзеры всякие бывают
																else if(PP[pp].Textures[tex].Type == SXPP_TEX_TYPE_ENGINE_NUM)
																{
																		for(int dttex=0;dttex<SXPostProcessGD::DataTex.CountTextures;dttex++)
																		{
																				if(strcmp(PP[pp].Textures[tex].ID,SXPostProcessGD::DataTex.Name[dttex]) == 0)
																					return SXPostProcessGD::DataTex.Texture[dttex];
																		}

																		if(strcmp(PP[pp].Textures[tex].ID,SXPP_TEX_ID_SCREEN) == 0)
																			return SXPostProcessGD::GetCurrentSendTexture();
																}
														}
												}
										}
										//иначе это рендер тарджет
										else if(strcmp(PP[PP[pp].Textures[tex].HowPP].RenderTarget[PP[pp].Textures[tex].HowNum].Name,name) == 0)
										{
												//если не стоит смешивание со сценой значит рендер тарджет отдельный, его и передаем
												if(!PP[PP[pp].Textures[tex].HowPP].RenderTarget[PP[pp].Textures[tex].HowNum].BlendScreen)
													return PP[PP[pp].Textures[tex].HowPP].RenderTarget[PP[pp].Textures[tex].HowNum].Texture;
												//иначе передаем текстуру в которую последний раз рисовали
												else
													return SXPostProcessGD::GetCurrentSendTexture();
										}
								}
						}

						//иначе если не нашли среди текстур либо установлен поиск среди рендер тарджетов
						if(type != 0)
						{
								for(int i=0;i<PP[pp].RTNum;i++)
								{
										if(strcmp(PP[pp].RenderTarget[i].Name,name) == 0)
										{
												if(PP[pp].RenderTarget[i].BlendScreen)
													return SXPostProcessGD::GetCurrentSendTexture();
												else
													return PP[pp].RenderTarget[i].Texture;
										}
								}
						}*/
				}

				//если в глобальнмо пространстве уже искали тогда начинаем сначала и запрещаем глобальный поиск
				if(!tmpIsG)
				{
					tmpIsG = true;
					pp = 0;
				}
		}

	return 0;
}

int SXLoaderPPFile::Render(int how_pp,SXPostProcessError *error,int count_rt_save,int* rt_save_arr)
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

	bool IsSaveActive = false;

		if(how_pp >= this->PPNum)
		{
				// ERROR если значение указывающее на постпроцесс больше либо равно общему количеству постпроцессов
				if(error != 0)
				{
					error->Code = SXPP_ERR_INVALID_NUMBER_OF_PP;
					sprintf(error->Str,"%d",how_pp);
					sprintf(error->Str2,"%d",this->PPNum);
					sprintf(error->GenFileError,"%s",__FILE__);
					error->GenStrError = __LINE__;
				}
			return SXPP_ERR_INVALID_NUMBER_OF_PP;
		}
		
	int HowPP = how_pp;

		//проход по всем pass
		for(int i=0;i<PP[HowPP].SeqNum;i++)
		{
			int HowPass = PP[HowPP].Sequence.PassNum[i];
			int HowRT = PP[HowPP].Pass[HowPass].RTNum;
			int tmpHowPPRT = HowPP;

				if(!PP[HowPP].Pass[HowPass].IsCurrentPPRT)
					tmpHowPPRT = this->HowGlobalNSData;
				
				//если рендер таргет это итоговое изображение, то берем его
				if(PP[tmpHowPPRT].RenderTarget[HowRT].BlendScreen)
					SXPostProcessGD::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);
				else
					PP[tmpHowPPRT].RenderTarget[HowRT].Texture->GetSurfaceLevel(0, &RenderSurf);

			SXPostProcessGD::Data.Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
			SXPostProcessGD::Data.Device->SetRenderTarget(0,RenderSurf);

			//SXPostProcessGD::Data.Device->Clear(0, NULL, D3DCLEAR_TARGET/* | D3DCLEAR_ZBUFFER*/, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

				for(int tex=0;tex<PP[HowPP].Pass[HowPass].CountTextures;tex++)
				{
					int HowTex = PP[HowPP].Pass[HowPass].TextureNum[tex];
					int tmpHowPPTex = HowPP;
					//определяем рендер тарджет это аля нет
					bool IsTexRT = false;
						if(PP[HowPP].Pass[HowPass].TextureTypeNum[tex] == 1)
							IsTexRT = true;

						if(!PP[HowPP].Pass[HowPass].IsCurrentPPTex[tex])
							tmpHowPPTex = this->HowGlobalNSData;

						//отправляем текстуры для данного прохода
						if(IsTexRT)//если всетаки текстура есть рендер таргет
						{
							SXPostProcessGD::Data.Device->SetTexture(PP[HowPP].Pass[HowPass].Texture[tex].Slot,PP[tmpHowPPTex].RenderTarget[HowTex].Texture);
						}
						else
						{
								//если передаваемая текстура это итоговое изображение со сценной
								if(strcmp(PP[tmpHowPPTex].Textures[HowTex].ID,"screen") == 0)
									SXPostProcessGD::Data.Device->SetTexture(PP[HowPP].Pass[HowPass].Texture[tex].Slot,SXPostProcessGD::GetCurrentSendTexture());
								else
								{
										if(PP[tmpHowPPTex].Textures[HowTex].IsOtherPP)
										{
												if(PP[tmpHowPPTex].Textures[HowTex].IsTex)
													SXPostProcessGD::Data.Device->SetTexture(PP[HowPP].Pass[HowPass].Texture[tex].Slot,/*PP[PP[HowPP].Textures[HowTex].HowPP].Textures[PP[HowPP].Textures[HowTex].HowNum].Texture*/this->GetTexturePP(HowPP,HowTex));
												else
													SXPostProcessGD::Data.Device->SetTexture(PP[HowPP].Pass[HowPass].Texture[tex].Slot,/*PP[PP[HowPP].Textures[HowTex].HowPP].RenderTarget[PP[HowPP].Textures[HowTex].HowNum].Texture*/this->GetTexturePP(HowPP,HowTex));
										}
										else
											SXPostProcessGD::Data.Device->SetTexture(PP[HowPP].Pass[HowPass].Texture[tex].Slot,PP[tmpHowPPTex].Textures[HowTex].Texture);
								}
						}

					//установка фильтрации
					SXPostProcessGD::Data.Device->SetSamplerState(PP[HowPP].Pass[HowPass].Texture[tex].Slot, D3DSAMP_MAGFILTER, PP[HowPP].Pass[HowPass].Texture[tex].Filter[0]);
					SXPostProcessGD::Data.Device->SetSamplerState(PP[HowPP].Pass[HowPass].Texture[tex].Slot, D3DSAMP_MINFILTER, PP[HowPP].Pass[HowPass].Texture[tex].Filter[1]);
					SXPostProcessGD::Data.Device->SetSamplerState(PP[HowPP].Pass[HowPass].Texture[tex].Slot, D3DSAMP_MIPFILTER, PP[HowPP].Pass[HowPass].Texture[tex].Filter[2]);

					//установка адресации
					SXPostProcessGD::Data.Device->SetSamplerState(PP[HowPP].Pass[HowPass].Texture[tex].Slot,D3DSAMP_ADDRESSU, PP[HowPP].Pass[HowPass].Texture[tex].Adress);
					SXPostProcessGD::Data.Device->SetSamplerState(PP[HowPP].Pass[HowPass].Texture[tex].Slot,D3DSAMP_ADDRESSV, PP[HowPP].Pass[HowPass].Texture[tex].Adress);
				}


			int HowVS = PP[HowPP].Pass[HowPass].ShaderVSNum;
			int HowDataPPVS = HowPP;
				if(!PP[HowPP].Pass[HowPass].IsCurrentPPVS)
					HowDataPPVS = this->HowGlobalNSData;

			//SXPostProcessGD::Data.Device->SetVertexShader(PP[HowDataPPVS].ShaderVS[HowVS].Shader.GetVertexShader());
				Core::Data::ShadersManager->Bind(0,PP[HowDataPPVS].ShaderVS[HowVS].Shader);

				//устанвливаем данные шейдера
				for(int data=0;data<PP[HowDataPPVS].ShaderVS[HowVS].NumData;data++)
					Core::Data::ShadersManager->SetValueRegisterF(0,PP[HowDataPPVS].ShaderVS[HowVS].Shader,PP[HowDataPPVS].ShaderVS[HowVS].Data[data].NameVar,PP[HowDataPPVS].ShaderVS[HowVS].Data[data].ValueData);

				for(int i=0;i<ComTrShaderDataVS[HowPP][HowPass].CountData;i++)
					Core::Data::ShadersManager->SetValueRegisterF(0,PP[HowDataPPVS].ShaderVS[HowVS].Shader,ComTrShaderDataVS[HowPP][HowPass].ShaderData[i].NameVar,ComTrShaderDataVS[HowPP][HowPass].ShaderData[i].Data);

			int HowPS = PP[HowPP].Pass[HowPass].ShaderPSNum;
			int HowDataPPPS = HowPP;
				if(!PP[HowPP].Pass[HowPass].IsCurrentPPPS)
					HowDataPPPS = this->HowGlobalNSData;
			//SXPostProcessGD::Data.Device->SetPixelShader(PP[HowDataPPPS].ShaderPS[HowPS].Shader.GetPixelShader());
			Core::Data::ShadersManager->Bind(1,PP[HowDataPPPS].ShaderPS[HowPS].Shader);

				for(int data=0;data<PP[HowDataPPPS].ShaderPS[HowPS].NumData;data++)
					Core::Data::ShadersManager->SetValueRegisterF(1,PP[HowDataPPPS].ShaderPS[HowPS].Shader,PP[HowDataPPPS].ShaderPS[HowPS].Data[data].NameVar,PP[HowDataPPPS].ShaderPS[HowPS].Data[data].ValueData);

				for(int i=0;i<ComTrShaderDataPS[HowPP][HowPass].CountData;i++)
					Core::Data::ShadersManager->SetValueRegisterF(1,PP[HowDataPPPS].ShaderPS[HowPS].Shader,ComTrShaderDataPS[HowPP][HowPass].ShaderData[i].NameVar,ComTrShaderDataPS[HowPP][HowPass].ShaderData[i].Data);

			SXPostProcessGD::Data.Device->BeginScene();
			SXPostProcessGD::ScreenTexture->DrawSubset(0);
			SXPostProcessGD::Data.Device->EndScene();

			SXPostProcessGD::Data.Device->SetVertexShader(0);
			SXPostProcessGD::Data.Device->SetPixelShader(0);
				
			SXPostProcessGD::Data.Device->SetRenderTarget(0,BackBuf);
			RenderSurf->Release();
			BackBuf->Release();

				//если рендерили в итоговое изображение то нужно поменять порядок
				if(PP[tmpHowPPRT].RenderTarget[HowRT].BlendScreen)
					SXPostProcessGD::IsCurrentTex = !SXPostProcessGD::IsCurrentTex;

				if((GetAsyncKeyState(SXPostProcessGD::Data.KeySave) || IsSaveActive) && count_rt_save != 0 && rt_save_arr != 0)
				{
						if(!IsSaveActive)
							IsSaveActive = true;

						for(int rt=0;rt<count_rt_save;rt++)
						{
								if(i == rt_save_arr[rt])
								{
									char tmpPath[256];
									sprintf(tmpPath, "%s%s%s",SXPostProcessGD::Data.PathForSave, PP[tmpHowPPRT].RenderTarget[HowRT].Name,".png");

										if(PP[tmpHowPPRT].RenderTarget[HowRT].BlendScreen)
											D3DXSaveTextureToFile(tmpPath, D3DXIFF_PNG, SXPostProcessGD::GetCurrentSendTexture(), NULL);
										else
											D3DXSaveTextureToFile(tmpPath, D3DXIFF_PNG, PP[tmpHowPPRT].RenderTarget[HowRT].Texture, NULL);
								}
						}
				}
		}
	return SXPP_OK;
}

int SXLoaderPPFile::Render(const char* name_pp,const char* how_rt_save,SXPostProcessError *error)
{
	int HowPP = -1;
	int tmpCountRT = 0;
	int tmpRTArr[SXPP_SETTINGS_SB_RENDER_TARGET];
		for(int i=0;i<SXPP_SETTINGS_SB_PP;i++)
		{
				if(strcmp(name_pp,PP[i].Name) == 0)
				{
					HowPP = i;
					break;
				}
		}

		if(HowPP != -1 && how_rt_save != 0)
		{
			char tmpstr[256];
			DWORD Bpos = 0, Epos = 0;
				while((Epos = SXStrFind(how_rt_save,"\n",Bpos)) != -1)
				{
					memcpy((void*)tmpstr,how_rt_save + Bpos,Epos - Bpos);
					tmpstr[(Epos - Bpos)] = 0;

						for(int i=0;i<PP[HowPP].SeqNum;i++)
						{
								if(strcmp(tmpstr,PP[HowPP].Pass[PP[HowPP].Sequence.PassNum[i]].RT) == 0)
								{
									tmpRTArr[tmpCountRT] = PP[HowPP].Sequence.PassNum[i];
									tmpCountRT++;
								}
						}
					Bpos = Epos + strlen("\n");
				}

				if(tmpCountRT > 0 && how_rt_save != 0)
				{
					this->Render(HowPP,error,tmpCountRT,tmpRTArr);
				}

				else if(how_rt_save != 0)
				{
						if(error != 0)
						{
							error->Code = SXPP_ERR_INVALID_RT_SAVE;
							sprintf(error->Str,"%s",how_rt_save);
							//sprintf(error->Str2,"%I",this->PPNum);
							sprintf(error->GenFileError,"%s",__FILE__);
							error->GenStrError = __LINE__;
						}
					return SXPP_ERR_INVALID_RT_SAVE;
				}
		}
		else if(HowPP != -1 && how_rt_save == 0)
		{
			this->Render(HowPP,error,0,0);
		}
		else
		{
				if(error != 0)
				{
					error->Code = SXPP_ERR_NOT_FOUND_PP;
					sprintf(error->Str,"%s",name_pp);
					sprintf(error->GenFileError,"%s",__FILE__);
					error->GenStrError = __LINE__;
				}
			return SXPP_ERR_NOT_FOUND_PP;
		}
	return SXPP_OK;
}


bool SXLoaderPPFile::IsDefined(const char* pp,int shader_type,const char* shader,const char* define)
{
		for(int i=0;i<SXPP_SETTINGS_SB_PP;i++)
		{
				if(strcmp(pp,PP[i].Name) == 0)
				{
						if(shader_type == 0 || shader_type == -1)
						{
								for(int k=0;k<PP[i].SVSNum;k++)
								{
										if(strcmp(PP[i].ShaderVS[k].Name,shader) == 0)
										{
												for(int j=0;j<PP[i].ShaderVS[k].CountMacro-1;j++)
												{
														if(strcmp(PP[i].ShaderVS[k].Macro[j].Name,define) == 0)
															return true;
												}
										}
								}
						}
						
						if(shader_type == 1 || shader_type == -1)
						{
								for(int k=0;k<PP[i].SPSNum;k++)
								{
										if(strcmp(PP[i].ShaderPS[k].Name,shader) == 0)
										{
												for(int j=0;j<PP[i].ShaderPS[k].CountMacro-1;j++)
												{
														if(strcmp(PP[i].ShaderPS[k].Macro[j].Name,define) == 0)
															return true;
												}
										}
								}
						}
				}
		}

	return false;
}

const char* SXLoaderPPFile::GetDefine(const char* pp,int shader_type,const char* shader,const char* define)
{
		for(int i=0;i<SXPP_SETTINGS_SB_PP;i++)
		{
				if(strcmp(pp,PP[i].Name) == 0)
				{
						if(shader_type == 0 || shader_type == -1)
						{
								for(int k=0;k<PP[i].SVSNum;k++)
								{
										if(strcmp(PP[i].ShaderVS[k].Name,shader) == 0)
										{
												for(int j=0;j<PP[i].ShaderVS[k].CountMacro-1;j++)
												{
														if(strcmp(PP[i].ShaderVS[k].Macro[j].Name,define) == 0)
															return PP[i].ShaderVS[k].Macro[j].Definition;
												}
										}
								}
						}
						if(shader_type == 1 || shader_type == -1)
						{
								for(int k=0;k<PP[i].SPSNum;k++)
								{
										if(strcmp(PP[i].ShaderPS[k].Name,shader) == 0)
										{
												for(int j=0;j<PP[i].ShaderPS[k].CountMacro-1;j++)
												{
														if(strcmp(PP[i].ShaderPS[k].Macro[j].Name,define) == 0)
															return PP[i].ShaderVS[k].Macro[j].Definition;
												}
										}
								}
						}
				}
		}

	return 0;
}

};
};
};
