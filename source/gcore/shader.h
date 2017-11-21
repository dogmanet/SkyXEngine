
#ifndef __SHADER_H
#define __SHADER_H

#include <gdefines.h>
#include <stdio.h>
#include <d3d9.h>
#include <common/array.h>
#include <common\String.h>
#include "sxgcore.h"

extern report_func g_fnReportf;
extern IDirect3DDevice9* DXDevice;
extern D3DPRESENT_PARAMETERS D3DAPP;

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

	bool FileExists(const char* path);

	ID Load(SHADER_TYPE type_shader, const char* path, const char* name, SHADER_CHECKDOUBLE is_check_double, D3DXMACRO* macro = 0);
	void Update(SHADER_TYPE type_shader, const char* name, D3DXMACRO macro[] = 0);
	void Update(SHADER_TYPE type_shader, ID id, D3DXMACRO macro[] = 0);

	void ReloadAll();

	ID GetID(SHADER_TYPE type_shader, const char* shader);

	void Bind(SHADER_TYPE type_shader, const char* shader);
	void Bind(SHADER_TYPE type_shader, ID shader);

	void UnBind();

	void SetValueRegisterF(SHADER_TYPE type_shader, const char* name_shader, const char* name_var, void* data, int count_float4=0);
	void SetValueRegisterF(SHADER_TYPE type_shader, ID num_shader, const char* name_var, void* data, int count_float4 = 0);

	void SetValueRegisterI(SHADER_TYPE type_shader, const char* name_shader, const char* name_var, void* data, int count_int4 = 0);
	void SetValueRegisterI(SHADER_TYPE type_shader, ID num_shader, const char* name_var, void* data, int count_int4 = 0);

	void SetValueRegisterB(SHADER_TYPE type_shader, const char* name_shader, const char* name_var, void* data, int count_bool4 = 0);
	void SetValueRegisterB(SHADER_TYPE type_shader, ID num_shader, const char* name_var, void* data, int count_bool4 = 0);

	ID IsExistPath(SHADER_TYPE type_shader, const char* path);
	ID IsExistName(SHADER_TYPE type_shader, const char* name);
	bool IsValidate(SHADER_TYPE type_shader, ID id);

	void GetPath(SHADER_TYPE type_shader, ID id, char* path);
	void GetName(SHADER_TYPE type_shader, ID id, char* name);

protected:
	Array<ShaderVS*> ArrVS;
	Array<ShaderPS*> ArrPS;
};

#endif