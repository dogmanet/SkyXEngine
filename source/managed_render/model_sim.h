
/*!
\file
Заголовочный файл model_sim - симуляционной модели имитируещей все сущности поддерживающие материальный рендер
*/

/*! \defgroup manager_render_model_sim model_sim - симуляционная модель имитируещая все сущности поддерживающие материальный рендер
 \ingroup managed_render
@{*/

#ifndef model_sim_h
#define model_sim_h

#include <common\array.h>

//симул¤ционна¤ модель всех возможных моделей материалов дл¤ редактора материалов

class ModelSim
{
public:
	ModelSim();
	~ModelSim();

	SX_ALIGNED_OP_MEM

	void Add(const char* path);

	void Render(DWORD timeDelta);

	MtlTypeModel TypeModel;
	int CurrRenderModel;
	float3 Rotation;

	inline ID GetIDMtl();
	inline void GetPlane(D3DXPLANE* plane);
	inline void GetCenter(float3_t* center);

protected:

	void RenderStatic(DWORD timeDelta);
	void RenderGreen(DWORD timeDelta);
	void RenderSkin(DWORD timeDelta);

	//структура трансформаций растительности
	struct DataVertex
	{
		float3_t Position;	//позици¤
		float3_t TexCoord;	//x - общий масштаб,y - поворот по оси y, z -
		float2_t SinCosRot;
	};

	IDirect3DVertexDeclaration9* VertexDeclarationStatic;
	IDirect3DVertexDeclaration9* VertexDeclarationGreen;
	IDirect3DVertexDeclaration9* VertexDeclarationSkin;

	struct DataModel
	{
		DataModel(){ Model = 0; }
		DataModel(ISXDataStaticModel* _model, float3_t* _center, float3_t* _max, float3_t* _min, D3DXPLANE* _plane)
		{
			Model = _model; Center = *_center; Max = *_max; Min = *_min; Plane = *_plane;
		}

		ISXDataStaticModel* Model;
		float3_t Min, Max, Center;
		D3DXPLANE Plane;
	};

	Array<DataModel*> ArrStaticModel;
	IDirect3DVertexBuffer9* TransVertBufGreen;
	DataVertex DVGreen;
	ID IDsMat;

	

	float4x4 WorldMat;
};

#endif

//!@} manager_render_model_sim