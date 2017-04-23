
#ifndef SHADER_H
#define SHADER_H

#include <common\array.h>
#include <common\string_api.cpp>

#pragma once

struct ShaderVS
{
	ShaderVS(){VertexShader=0;ConstTable=0;};
	~ShaderVS(){ mem_release(VertexShader); mem_release(ConstTable); };
	char Name[SXGC_SHADER_MAX_SIZE_NAME];
	char Path[SXGC_SHADER_MAX_SIZE_DIR];
	int CountConst;
	IDirect3DVertexShader9* VertexShader;
	ID3DXConstantTable* ConstTable;
	D3DXHANDLE ArrHandle[SXGC_SHADER_VAR_MAX_COUNT];
	D3DXCONSTANT_DESC ArrConst[SXGC_SHADER_VAR_MAX_COUNT];
	D3DXMACRO ArrMacro[SXGC_SHADER_COUNT_MACRO];
};


struct ShaderPS
{
	ShaderPS(){PixelShader=0;ConstTable=0;};
	~ShaderPS(){ mem_release(PixelShader); mem_release(ConstTable); };
	char Name[SXGC_SHADER_MAX_SIZE_NAME];
	char Path[SXGC_SHADER_MAX_SIZE_DIR];
	int CountConst;
	IDirect3DPixelShader9* PixelShader;
	ID3DXConstantTable* ConstTable;
	D3DXHANDLE ArrHandle[SXGC_SHADER_VAR_MAX_COUNT];
	D3DXCONSTANT_DESC ArrConst[SXGC_SHADER_VAR_MAX_COUNT];
	D3DXMACRO ArrMacro[SXGC_SHADER_COUNT_MACRO];
};

void LoadVertexShader(const char *path,ShaderVS* shader,D3DXMACRO* macro = 0);
void LoadPixelShader(const char *path,ShaderPS* shader,D3DXMACRO* macro = 0);


class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	ID Load(ShaderType type_shader, const char* path, const char* name, ShaderCheckDouble is_check_double, D3DXMACRO* macro = 0);
	void Update(ShaderType type_shader, const char* name, D3DXMACRO macro[] = 0);
	void Update(ShaderType type_shader, ID id, D3DXMACRO macro[] = 0);

	void ReloadAll();

	ID GetID(ShaderType type_shader, const char* shader);

	void Bind(ShaderType type_shader, const char* shader);
	void Bind(ShaderType type_shader, ID shader);

	void UnBind();

	void SetValueRegisterF(ShaderType type_shader, const char* name_shader, const char* name_var, void* data, int count_float4=0);
	void SetValueRegisterF(ShaderType type_shader, ID num_shader, const char* name_var, void* data, int count_float4 = 0);

	void SetValueRegisterI(ShaderType type_shader, const char* name_shader, const char* name_var, void* data, int count_int4 = 0);
	void SetValueRegisterI(ShaderType type_shader, ID num_shader, const char* name_var, void* data, int count_int4 = 0);

	void SetValueRegisterB(ShaderType type_shader, const char* name_shader, const char* name_var, void* data, int count_bool4 = 0);
	void SetValueRegisterB(ShaderType type_shader, ID num_shader, const char* name_var, void* data, int count_bool4 = 0);

	ID IsExistPath(ShaderType type_shader, const char* path);
	ID IsExistName(ShaderType type_shader, const char* name);
	bool IsValidate(ShaderType type_shader, ID id);

	inline void GetPath(ShaderType type_shader, ID id, char* path);
	inline void GetName(ShaderType type_shader, ID id, char* name);

	inline void SetStdPath(const char* path){ strcpy(StdPath, path); };
	inline void GetStdPath(char* path){		if(path) strcpy(path, StdPath); };
protected:
	char StdPath[SXGC_SHADER_MAX_SIZE_STDPATH];
	Array<ShaderVS*> ArrVS;
	Array<ShaderPS*> ArrPS;
};

#endif