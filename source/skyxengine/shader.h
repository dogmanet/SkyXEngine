
#ifndef SHADER_H
#define SHADER_H

#pragma once

#define SHADER_COUNT_ARR_MACRO 12

struct ShaderVS
{
	ShaderVS(){VertexShader=0;ConstTable=0;};
	~ShaderVS(){ndx_release(VertexShader);ndx_release(ConstTable);};
	char Name[256];
	char Path[256];
	int CountConst;
	IDirect3DVertexShader9* VertexShader;
	ID3DXConstantTable* ConstTable;
	D3DXHANDLE ArrHandle[128];
	D3DXCONSTANT_DESC ArrConst[128];
	D3DXMACRO ArrMacro[SHADER_COUNT_ARR_MACRO];
};


struct ShaderPS
{
	ShaderPS(){PixelShader=0;ConstTable=0;};
	~ShaderPS(){ndx_release(PixelShader);ndx_release(ConstTable);};
	char Name[256];
	char Path[256];
	int CountConst;
	IDirect3DPixelShader9* PixelShader;
	ID3DXConstantTable* ConstTable;
	D3DXHANDLE ArrHandle[128];
	D3DXCONSTANT_DESC ArrConst[128];
	D3DXMACRO ArrMacro[SHADER_COUNT_ARR_MACRO];
};

void LoadVertexShader(const char *path,ShaderVS* shader,D3DXMACRO* macro = 0);
void LoadPixelShader(const char *path,ShaderPS* shader,D3DXMACRO* macro = 0);


class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	//is_check_double - 0 не искать среди загруженных, 1 - искать среди загруженных, 2 - обновить если имееться, если нет то грузим
	DWORD Load(int type_shader,const char* path,const char* name,int is_check_double,D3DXMACRO* macro = 0);
	DWORD Update(int type_shader,const char* name,D3DXMACRO macro[] = 0);
	void Update(int type_shader,DWORD id,D3DXMACRO macro[]=0);

	void ReloadAll();

	DWORD GetID(int type_shader,const char* shader);

	void Bind(int type_shader,const char* shader);
	void Bind(int type_shader,DWORD shader);

	void UnBind();

	void SetValueRegisterF(int type_shader,const char* name_shader,const char* name_var,void* data);
	void SetValueRegisterF(int type_shader,DWORD num_shader,const char* name_var,void* data);

	void SetValueRegisterI(int type_shader,const char* name_shader,const char* name_var,void* data);
	void SetValueRegisterI(int type_shader,DWORD num_shader,const char* name_var,void* data);

	void SetValueRegisterB(int type_shader,const char* name_shader,const char* name_var,void* data);
	void SetValueRegisterB(int type_shader,DWORD num_shader,const char* name_var,void* data);

	DWORD IsExist(int type_shader,const char* name);
	bool IsValidate(int type_shader,DWORD id);

	inline void GetPath(int type_shader,DWORD id,char* path);
	inline void GetName(int type_shader,DWORD id,char* name);

protected:
	Array<ShaderVS*> ArrVS;
	Array<ShaderPS*> ArrPS;
};

#endif