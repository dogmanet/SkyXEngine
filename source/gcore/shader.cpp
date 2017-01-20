
#pragma once

#include <gcore\shader.h>

void LoadVertexShader(const char *path,ShaderVS* shader,D3DXMACRO* macro)
{
	ID3DXBuffer* tmpShader	= 0;
	ID3DXBuffer* tmpError	= 0;
	ID3DXConstantTable* tmpConstTable;
	IDirect3DVertexShader9 *tmpVSShader;

	HRESULT hr;

	hr = D3DXCompileShaderFromFile(
									path,
									macro,
									0,
									"main",
									"vs_3_0",
									SHADER_FLAGS,
									&tmpShader,
									&tmpError,
									&tmpConstTable
									);

		if(tmpError && tmpShader == 0)
		{
			reportf(-1, "%s - failed to load shader [%s], msg: %s\n", gen_msg_location, path, (char*)tmpError->GetBufferPointer());
		}

		//!!!проанализировать
		if(FAILED(hr))
		{
			reportf(-1, "%s - download function shader fails, path [%s]", gen_msg_location, path);
		}

	hr = DXDevice->CreateVertexShader(
									(DWORD*)tmpShader->GetBufferPointer(),
									&tmpVSShader
									);
		if(FAILED(hr))
		{
			reportf(-1, "%s - error creating shader [%s]", gen_msg_location, path);
		}

		D3DXCONSTANTTABLE_DESC desc;
		tmpConstTable->GetDesc(&desc);
		

		for(int i=0;i<desc.Constants;i++)
		{
			shader->ArrHandle[i] = tmpConstTable->GetConstant(0,i);

			UINT count = 0;
			tmpConstTable->GetConstantDesc(shader->ArrHandle[i],&(shader->ArrConst[i]),&count);
		}

	shader->CountConst = desc.Constants;
	shader->ConstTable = tmpConstTable;
	shader->VertexShader = tmpVSShader;

		if(macro)
		{
			int tmpCountArrM = 0;

				for(int i=0;i<SHADER_COUNT_ARR_MACRO;i++)
				{
						if(macro[i].Name == 0)
						{
							tmpCountArrM = i;
							break;
						}
				}

				for(int i=0;i<tmpCountArrM;i++)
				{
					shader->ArrMacro[i] = macro[i];
				}
			shader->ArrMacro[tmpCountArrM].Name = 0;
			shader->ArrMacro[tmpCountArrM].Definition = 0;
		}
		else
		{
			shader->ArrMacro[0].Name = 0;
			shader->ArrMacro[0].Definition = 0;
		}
}

void LoadPixelShader(const char *path,ShaderPS* shader,D3DXMACRO* macro)
{
	ID3DXBuffer* tmpShader	= 0;
	ID3DXBuffer* tmpError	= 0;
	ID3DXConstantTable* tmpConstTable;
	IDirect3DPixelShader9 *tmpPSShader;

	HRESULT hr;

	hr = D3DXCompileShaderFromFile(
									path,
									macro,
									0,
									"main",
									"ps_3_0",
									SHADER_FLAGS,
									&tmpShader,
									&tmpError,
									&tmpConstTable
									);

		if(tmpError && tmpShader == 0)
		{
			reportf(-1, "%s - failed to load shader [%s], msg: %s\n", gen_msg_location, path, (char*)tmpError->GetBufferPointer());
		}

		//!!!проанализировать
		if(FAILED(hr))
		{
			reportf(-1, "%s - download function shader fails, path [%s]", gen_msg_location, path);
		}

	hr = DXDevice->CreatePixelShader(
									(DWORD*)tmpShader->GetBufferPointer(),
									&tmpPSShader
									);
		if(FAILED(hr))
		{
			reportf(-1, "%s - error creating shader [%s]", gen_msg_location, path);
		}

		D3DXCONSTANTTABLE_DESC desc;
		tmpConstTable->GetDesc(&desc);
		

		for(int i=0;i<desc.Constants;i++)
		{
			shader->ArrHandle[i] = tmpConstTable->GetConstant(0,i);

			UINT count = 0;
			tmpConstTable->GetConstantDesc(shader->ArrHandle[i],&(shader->ArrConst[i]),&count);
		}

	shader->CountConst = desc.Constants;
	shader->ConstTable = tmpConstTable;
	shader->PixelShader = tmpPSShader;

		if(macro)
		{
			int tmpCountArrM = 0;

				for(int i=0;i<SHADER_COUNT_ARR_MACRO;i++)
				{
						if(macro[i].Name == 0)
						{
							tmpCountArrM = i;
							break;
						}
				}

				for(int i=0;i<tmpCountArrM;i++)
				{
					shader->ArrMacro[i] = macro[i];
				}
			shader->ArrMacro[tmpCountArrM].Name = 0;
			shader->ArrMacro[tmpCountArrM].Definition = 0;
		}
		else
		{
			shader->ArrMacro[0].Name = 0;
			shader->ArrMacro[0].Definition = 0;
		}
}



ShaderManager::ShaderManager()
{
	
}

ShaderManager::~ShaderManager()
{
	for (DWORD i = 0; i < ArrVS.size(); i++)
	{
		mem_delete(ArrVS[i]);
	}
			

	for (DWORD i = 0; i < ArrPS.size(); i++)
	{
		mem_delete(ArrPS[i]);
	}
			
	ArrVS.clear();
	ArrPS.clear();
}

void ShaderManager::ReloadAll()
{
	Array<ShaderVS*> tmpArrVS;
	Array<ShaderPS*> tmpArrPS;

		for(int i=0;i<ArrVS.size();i++)
		{
			tmpArrVS.push_back(ArrVS[i]);
		}

		for(int i=0;i<ArrPS.size();i++)
		{
			tmpArrPS.push_back(ArrPS[i]);
		}

	ArrVS.clear();
	ArrPS.clear();

		for(int i=0;i<tmpArrVS.size();i++)
		{
			Load(0,tmpArrVS[i]->Path,tmpArrVS[i]->Name,false,tmpArrVS[i]->ArrMacro);
		}

		for(int i=0;i<tmpArrPS.size();i++)
		{
			Load(1,tmpArrPS[i]->Path,tmpArrPS[i]->Name,false,tmpArrPS[i]->ArrMacro);
		}
}

DWORD ShaderManager::IsExist(int type_shader,const char* path)
{
		if(type_shader == 0)
		{
				for(DWORD i=0;i<ArrVS.size();i++)
				{
						if(strcmp(ArrVS[i]->Path,path) == 0)
							return i;
				}
		}
		else if(type_shader == 1)
		{
				for(DWORD i=0;i<ArrPS.size();i++)
				{
						if(strcmp(ArrPS[i]->Path,path) == 0)
							return i;
				}
		}
	return -1;
}

DWORD ShaderManager::Load(int type_shader,const char* path,const char* name,int is_check_double,D3DXMACRO* macro)
{
		if(type_shader == 0)
		{
			DWORD id = (is_check_double ? IsExist(type_shader,path) : -1);

				if(id != -1 && is_check_double == 2)
					Update(0,id,0);

				if(id == -1)
				{
					char tmppath[1024];
					char tmpspath[1024];
					tmpspath[0] = 0;
					char tmpname[1024];
					ParseStrPathName(path,tmpspath,tmpname);

					ShaderVS* shader = new ShaderVS();
						if(tmpspath[0] != 0)
							sprintf(tmppath,"%s%s\\%s",StdPath,tmpspath,path);
						else
							sprintf(tmppath, "%s%s", StdPath, path);
					LoadVertexShader(tmppath,shader,macro);
					sprintf(shader->Name,"%s",name);
					sprintf(shader->Path,"%s",path);
					ArrVS.push_back(shader);
					id = ArrVS.size()-1;
					reportf(0,"\tshader [%s] is loaded, name[%s], id [%d]\n",path,ArrVS[id]->Name,id);
					return id;
				}
				else
				{
					reportf(0,"\tshader [%s] is detected, name[%s], id [%d]\n",path,ArrVS[id]->Name,id);
					return id;
				}
		}
		else if(type_shader == 1)
		{
			DWORD id = (is_check_double ? IsExist(type_shader,path) : -1);

				if(id != -1 && is_check_double == 2)
					Update(1,id,0);

				if(id == -1)
				{
					char tmppath[1024];
					char tmpspath[1024];
					tmpspath[0] = 0;
					char tmpname[1024];
					ParseStrPathName(path,tmpspath,tmpname);
					sprintf(tmppath,"%s%s\\%s",StdPath,tmpspath,path);
					ShaderPS* shader = /*mem_new(ShaderPS);//*/new ShaderPS();
					LoadPixelShader(tmppath,shader,macro);
					sprintf(shader->Name,"%s",name);
					sprintf(shader->Path,"%s",path);
					ArrPS.push_back(shader);
					id = ArrPS.size()-1;
					reportf(0,"\tshader [%s] is loaded, name[%s], id [%d]\n", path, ArrPS[id]->Name, id);
					return id;
				}
				else
				{
					reportf(0,"\tshader [%s] is detected, name[%s], id [%d]\n", path, ArrPS[id]->Name, id);
					return id;
				}
		}
	return -1;
}

void ShaderManager::Update(int type_shader,const char* name,D3DXMACRO* macro)
{
	bool isupdate = false;
		if(type_shader == 0)
		{	
				for(DWORD i=0;i<ArrVS.size();i++)
				{
						if(strcmp(ArrVS[i]->Name,name) == 0)
						{
							ShaderVS* shader = new ShaderVS();
							LoadVertexShader(ArrVS[i]->Path,shader,macro);

							mem_release(ArrVS[i]->VertexShader);
							mem_release(ArrVS[i]->ConstTable);

							mem_delete(ArrVS[i]);
							ArrVS[i] = shader;
							isupdate = true;
						}
				}	
		}
		else if(type_shader == 1)
		{
				for(DWORD i=0;i<ArrPS.size();i++)
				{
						if(strcmp(ArrPS[i]->Name,name) == 0)
						{
							ShaderPS* shader = new ShaderPS();
							LoadPixelShader(ArrPS[i]->Path,shader,macro);

							mem_release(ArrPS[i]->PixelShader);
							mem_release(ArrPS[i]->ConstTable);

							mem_delete(ArrPS[i]);
							ArrPS[i] = shader;
							isupdate = true;
						}
				}
		}

		if (!isupdate)
			reportf(-1, "%s - shader [%s] is not detected", gen_msg_location, name);
		else
			reportf(0, "shader [%s] is update", name);
}

void ShaderManager::Update(int type_shader,DWORD id,D3DXMACRO* macro)
{
	bool isupdate = false;
		if(type_shader == 0)
		{	
				if (ArrVS.size() > id && ArrVS[id])
				{
					char tmppath[1024];
					char tmpspath[1024];
					tmpspath[0] = 0;
					char tmpname[1024];
					ParseStrPathName(ArrVS[id]->Path,tmpspath,tmpname);

					ShaderVS* shader = /*mem_new(ShaderVS);//*/new ShaderVS();
						if(tmpspath[0] != 0)
							sprintf(tmppath,"%s%s\\%s",StdPath,tmpspath,ArrVS[id]->Path);
						else
							sprintf(tmppath, "%s%s", StdPath, ArrVS[id]->Path);

					LoadVertexShader(tmppath,shader,macro);
					sprintf(shader->Name,"%s",ArrVS[id]->Name);
					sprintf(shader->Path,"%s",ArrVS[id]->Path);

					mem_release(ArrVS[id]->VertexShader);
					mem_release(ArrVS[id]->ConstTable);

					mem_delete(ArrVS[id]);
					ArrVS[id] = shader;
					isupdate = true;
				}
		}	
		else if(type_shader == 1)
		{
				if (ArrPS.size() > id  && ArrVS[id])
				{
					char tmppath[1024];
					char tmpspath[1024];
					tmpspath[0] = 0;
					char tmpname[1024];
					ParseStrPathName(ArrPS[id]->Path,tmpspath,tmpname);

					ShaderPS* shader = /*mem_new(ShaderVS);//*/new ShaderPS();
						if(tmpspath[0] != 0)
							sprintf(tmppath,"%s%s\\%s",StdPath,tmpspath,ArrPS[id]->Path);
						else
							sprintf(tmppath, "%s%s", StdPath, ArrPS[id]->Path);

					LoadPixelShader(tmppath,shader,macro);

					sprintf(shader->Name,"%s",ArrPS[id]->Name);
					sprintf(shader->Path,"%s",ArrPS[id]->Path);

					mem_release(ArrPS[id]->PixelShader);
					mem_release(ArrPS[id]->ConstTable);

					mem_delete(ArrPS[id]);
					ArrPS[id] = shader;
					isupdate = true;
				}
		}

		if (!isupdate)
			reportf(-1, "%s - shader [%s] is not detected", gen_msg_location, ArrPS[id]->Name);
		else
			reportf(0, "shader [%s] is update", ArrPS[id]->Name);
}

DWORD ShaderManager::GetID(int type_shader,const char* shader)
{
		if(type_shader == 0)
		{	
				for(DWORD i=0;i<ArrVS.size();i++)
				{
						if(strcmp(ArrVS[i]->Name,shader) == 0)
						{
							return i;
						}
				}	
		}
		else if(type_shader == 1)
		{
				for(DWORD i=0;i<ArrPS.size();i++)
				{
						if(strcmp(ArrPS[i]->Name,shader) == 0)
						{
							return i;
						}
				}
		}
	return -1;
}

void ShaderManager::Bind(int type_shader,const char* shader)
{
	bool isbind = false;
		if(type_shader == 0)
		{	
				for(DWORD i=0;i<ArrVS.size();i++)
				{
						if(strcmp(ArrVS[i]->Name,shader) == 0)
						{
							DXDevice->SetVertexShader(ArrVS[i]->VertexShader);
							isbind = true;
							break;
						}
				}	
		}
		else if(type_shader == 1)
		{
				for(DWORD i=0;i<ArrPS.size();i++)
				{
						if(strcmp(ArrPS[i]->Name,shader) == 0)
						{
							DXDevice->SetPixelShader(ArrPS[i]->PixelShader);
							isbind = true;
							break;
						}
				}
		}

		if(!isbind)
			reportf(1,"WARNING!!! shader not bind, type [%d], name [%s]\n",type_shader,shader);
}

void ShaderManager::Bind(int type_shader,DWORD shader)
{
	bool isbind = false;
		if(type_shader == 0)
		{	
				if(shader < ArrVS.size())
				{
					isbind = true;
					DXDevice->SetVertexShader(ArrVS[shader]->VertexShader);
				}
		}
		else if(type_shader == 1)
		{
				if(shader < ArrPS.size())
				{
					isbind = true;
					DXDevice->SetPixelShader(ArrPS[shader]->PixelShader);
				}
		}

		if(!isbind)
			reportf(1,"WARNING!!! shader not bind, type [%d], id [%d]\n",type_shader,shader);
}

void ShaderManager::UnBind()
{
	DXDevice->SetVertexShader(0);
	DXDevice->SetPixelShader(0);
}


void ShaderManager::SetValueRegisterF(int type_shader,const char* name_shader,const char* name_var,void* data)
{
		if(type_shader == 0)
		{
			int num_shader = -1;
			int num_var = -1;

				for(int i=0;i<ArrVS.size();i++)
				{
						if(strcmp(ArrVS[i]->Name,name_shader) == 0)
						{
							num_shader = i;
							i = ArrVS.size();
								for(int k=0;k<ArrVS[num_shader]->CountConst;k++)
								{
										if(strcmp(ArrVS[num_shader]->ArrConst[k].Name,name_var) == 0)
										{
											num_var = k;
											k = ArrVS[num_shader]->CountConst;
										}
								}
						}
				}

				if(num_shader != -1 && num_var != -1)
					DXDevice->SetVertexShaderConstantF(ArrVS[num_shader]->ArrConst[num_var].RegisterIndex,(float*)data,ArrVS[num_shader]->ArrConst[num_var].RegisterCount);
				/*else
				{
						if(num_shader == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, shader not found, type [%d], id [%d]\n",name_var,type_shader,name_shader);
						else if(num_var == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, constant not found, shader info: type [%d], id [%d], name [%s]\n",name_var,type_shader,name_shader);
				}*/
		}
		else if(type_shader == 1)
		{
			int num_shader = -1;
			int num_var = -1;

				for(int i=0;i<ArrPS.size();i++)
				{
						if(strcmp(ArrPS[i]->Name,name_shader) == 0)
						{
							num_shader = i;
							i = ArrPS.size();
								for(int k=0;k<ArrPS[num_shader]->CountConst;k++)
								{
										if(strcmp(ArrPS[num_shader]->ArrConst[k].Name,name_var) == 0)
										{
											num_var = k;
											k = ArrPS[num_shader]->CountConst;
										}
								}
						}
				}

				if(num_shader != -1 && num_var != -1)
					DXDevice->SetPixelShaderConstantF(ArrPS[num_shader]->ArrConst[num_var].RegisterIndex,(float*)data,ArrPS[num_shader]->ArrConst[num_var].RegisterCount);
				/*else
				{
						if(num_shader == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, shader not found, type [%d], id [%d]\n",name_var,type_shader,name_shader);
						else if(num_var == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, constant not found, shader info: type [%d], id [%d], name [%s]\n",name_var,type_shader,name_shader);
				}*/
		}
}

void ShaderManager::SetValueRegisterI(int type_shader,const char* name_shader,const char* name_var,void* data)
{
		if(type_shader == 0)
		{
			int num_shader = -1;
			int num_var = -1;

				for(int i=0;i<ArrVS.size();i++)
				{
						if(strcmp(ArrVS[i]->Name,name_shader) == 0)
						{
							num_shader = i;
							i = ArrVS.size();
								for(int k=0;k<ArrVS[num_shader]->CountConst;k++)
								{
										if(strcmp(ArrVS[num_shader]->ArrConst[k].Name,name_var) == 0)
										{
											num_var = k;
											k = ArrVS[num_shader]->CountConst;
										}
								}
						}
				}

				if(num_shader != -1 && num_var != -1)
					DXDevice->SetVertexShaderConstantI(ArrVS[num_shader]->ArrConst[num_var].RegisterIndex,(int*)data,ArrVS[num_shader]->ArrConst[num_var].RegisterCount);
				/*else
				{
						if(num_shader == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, shader not found, type [%d], id [%d]\n",name_var,type_shader,name_shader);
						else if(num_var == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, constant not found, shader info: type [%d], id [%d], name [%s]\n",name_var,type_shader,name_shader);
				}*/
		}
		else if(type_shader == 1)
		{
			int num_shader = -1;
			int num_var = -1;

				for(int i=0;i<ArrPS.size();i++)
				{
						if(strcmp(ArrPS[i]->Name,name_shader) == 0)
						{
							num_shader = i;
							i = ArrPS.size();
								for(int k=0;k<ArrPS[num_shader]->CountConst;k++)
								{
										if(strcmp(ArrPS[num_shader]->ArrConst[k].Name,name_var) == 0)
										{
											num_var = k;
											k = ArrPS[num_shader]->CountConst;
										}
								}
						}
				}

				if(num_shader != -1 && num_var != -1)
					DXDevice->SetPixelShaderConstantI(ArrPS[num_shader]->ArrConst[num_var].RegisterIndex,(int*)data,ArrPS[num_shader]->ArrConst[num_var].RegisterCount);
				/*else
				{
						if(num_shader == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, shader not found, type [%d], id [%d]\n",name_var,type_shader,name_shader);
						else if(num_var == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, constant not found, shader info: type [%d], id [%d], name [%s]\n",name_var,type_shader,name_shader);
				}*/
		}
}

void ShaderManager::SetValueRegisterI(int type_shader,DWORD num_shader,const char* name_var,void* data)
{
		if(IsValidate(type_shader,num_shader))
		{
				if(type_shader == 0)
				{
					int num_var = -1;

						for(int k=0;k<ArrVS[num_shader]->CountConst;k++)
						{
								if(strcmp(ArrVS[num_shader]->ArrConst[k].Name,name_var) == 0)
								{
									num_var = k;
									k = ArrVS[num_shader]->CountConst;
								}
						}

						if(num_shader != -1 && num_var != -1)
							DXDevice->SetVertexShaderConstantI(ArrVS[num_shader]->ArrConst[num_var].RegisterIndex,(int*)data,ArrVS[num_shader]->ArrConst[num_var].RegisterCount);
						/*else if(num_var == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, constant not found, type [%d], id [%d]\n",name_var,type_shader,num_shader);*/
				}
				else if(type_shader == 1)
				{
					int num_var = -1;

						for(int k=0;k<ArrPS[num_shader]->CountConst;k++)
						{
								if(strcmp(ArrPS[num_shader]->ArrConst[k].Name,name_var) == 0)
								{
									num_var = k;
									k = ArrPS[num_shader]->CountConst;
								}
						}

						if(num_shader != -1 && num_var != -1)
							DXDevice->SetPixelShaderConstantI(ArrPS[num_shader]->ArrConst[num_var].RegisterIndex,(int*)data,ArrPS[num_shader]->ArrConst[num_var].RegisterCount);
						/*else if(num_var == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, constant not found, type [%d], name [%s]\n",name_var,type_shader,ArrPS[num_shader]->Path);*/
				}
		}
}


void ShaderManager::SetValueRegisterF(int type_shader,DWORD num_shader,const char* name_var,void* data)
{
		if(IsValidate(type_shader,num_shader))
		{
				if(type_shader == 0)
				{
					int num_var = -1;

						for(int k=0;k<ArrVS[num_shader]->CountConst;k++)
						{
								if(strcmp(ArrVS[num_shader]->ArrConst[k].Name,name_var) == 0)
								{
									num_var = k;
									k = ArrVS[num_shader]->CountConst;
								}
						}

						if (num_shader != -1 && num_var != -1)
							DXDevice->SetVertexShaderConstantF(ArrVS[num_shader]->ArrConst[num_var].RegisterIndex, (float*)data, ArrVS[num_shader]->ArrConst[num_var].RegisterCount);
						else
							int qwert = 0;
						/*else if(num_var == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, constant not found, type [%d], id [%d]\n",name_var,type_shader,num_shader);*/
				}
				else if(type_shader == 1)
				{
					int num_var = -1;

						for(int k=0;k<ArrPS[num_shader]->CountConst;k++)
						{
								if(strcmp(ArrPS[num_shader]->ArrConst[k].Name,name_var) == 0)
								{
									num_var = k;
									k = ArrPS[num_shader]->CountConst;
								}
						}

						if(num_shader != -1 && num_var != -1)
							DXDevice->SetPixelShaderConstantF(ArrPS[num_shader]->ArrConst[num_var].RegisterIndex,(float*)data,ArrPS[num_shader]->ArrConst[num_var].RegisterCount);
						/*else if(num_var == -1)
							SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, constant not found, type [%d], name [%s]\n",name_var,type_shader,ArrPS[num_shader]->Path);*/
				}
		}
		/*else
			SkyXEngine::Core::InLog("WARNING!!! set shader constant [%s] is failed, shader not validate, type [%d], id [%d]\n",name_var,type_shader,num_shader);*/
}

bool ShaderManager::IsValidate(int type_shader,DWORD id)
{
		if(type_shader == 0)
		{
				if(id < ArrVS.size())
				{
						if(ArrVS[id]->VertexShader)
							return true;
						else
							return false;
				}
				else 
					return false;
		}
		else if(type_shader == 1)
		{
				if(id < ArrPS.size())
				{
						if(ArrPS[id]->PixelShader)
							return true;
						else
							return false;
				}
				else 
					return false;
		}
}

inline void ShaderManager::GetPath(int type_shader,DWORD id,char* path)
{
		if(type_shader == 0)
		{
				if(id < ArrVS.size())
				{
						if(ArrVS[id]->VertexShader)
							sprintf(path,"%s",ArrVS[id]->Path);
				}
		}
		else if(type_shader == 1)
		{
				if(id < ArrPS.size())
				{
						if(ArrPS[id]->PixelShader)
							sprintf(path,"%s",ArrPS[id]->Path);
				}
		}
}

inline void ShaderManager::GetName(int type_shader,DWORD id,char* name)
{
		if(type_shader == 0)
		{
				if(id < ArrVS.size())
				{
						if(ArrVS[id]->VertexShader)
							sprintf(name,"%s",ArrVS[id]->Name);
				}
		}
		else if(type_shader == 1)
		{
				if(id < ArrPS.size())
				{
						if(ArrPS[id]->PixelShader)
							sprintf(name,"%s",ArrPS[id]->Name);
				}
		}
}