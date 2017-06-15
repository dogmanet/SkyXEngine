
#ifndef __green
#define __green

#include <common\array.h>
#include <common\\string_api.cpp>

#define GREEN_COUNT_TYPE_SEGMENTATION 4
#define GREEN_COUNT_LOD 3

#define GREEN_DEFAULT_RESERVE_GEN 1024
#define GREEN_DEFAULT_RESERVE_COM 1024

#define GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr) \
if (!(id_arr < ArrComFor.size()))\
{\
	reportf(REPORT_MSG_LEVEL_ERROR, "%s - green: unresolved id '%d' for array of compute visible", gen_msg_location, id_arr); \
}

#define GREEN_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model) \
if (!(id_model < ArrModels.size() && ArrModels[id_model]))\
{\
	reportf(REPORT_MSG_LEVEL_ERROR, "%s - green: unresolved id '%d' for array of models", gen_msg_location, id_model); \
}

class Green
{
public:

	static int CurrentFreqGrass;
	static float2_t DistLods;
	static float BeginEndLessening;
	static bool UseSortFrontToBackSplits;
	static IDirect3DDevice9* DXDevice;
	static char StdPath[1024];

	SX_ALIGNED_OP_MEM

	Green();
	~Green();

	ID Init(StaticGeom* geom, const char* name,
		const char* path_mask,
		float count_max,
		const char* path, const char* lod1, const char* lod2,
		const char* navmesh);

	ID AddObject(ID id, float3* pos);
	void DelObject(ID id, ID idsplit, ID idobj);

	void DelGreen(ID id);
	
	void OnLostDevice();
	void OnResetDevice();
	
	ID AddArrForCom();
	void DelArrForCom(ID id_arr);
	
	void Save(const char* path);
	void Load(const char* path);
	void Clear();
	void CPUFillingArrIndeces(ISXFrustum* frustum, float3* viewpos, ID id_arr = 0);
	void GPURender(DWORD timeDelta, float3* viewpos, GeomGreenType type, ID id_arr = 0);
	void GPURenderSingly(DWORD timeDelta, float3* viewpos, ID id, ID id_tex);
	void GPURenderObject(DWORD timeDelta, float3* viewpos, ID id, ID split, ID idobj, ID id_tex);

	inline ID GetCountGreen();
	inline char* GetGreenName(ID id);
	inline long GetGreenCountGen(ID id);
	inline long GetGreenCountPoly(ID id);
	inline int GetGreenTypeCountGen(ID id);
	inline const char* GetGreenModel(ID id);
	inline const char* GetGreenLod1(ID id);
	inline const char* GetGreenLod2(ID id);
	inline const char* GetGreenMask(ID id);
	inline const char* GetGreenNav(ID id);
	void SetGreenLod(ID id, int lod, const char* pathname);
	void SetGreenNav(ID id, const char* pathname);

	void GetNavMeshAndTransform(float3_t*** arr_vertex, int32_t** arr_count_vertex, uint32_t*** arr_index, ID*** arr_mtl, int32_t** arr_count_index, GreenDataVertex*** arr_transform, int32_t** arr_count_transform, int32_t* arr_count_green);
	bool GetOccurencessLeafGrass(float3* bbmin, float3* bbmax, int physic_mtl);
	
	bool TraceBeam(float3* start, float3* dir, float3* _res, ID* idgreen, ID* idsplits, ID* idobj, ID* idmtl);

	//
	struct Segment
	{
		SX_ALIGNED_OP_MEM

		Segment();
		~Segment();

		Segment* Splits[GREEN_COUNT_TYPE_SEGMENTATION]; //массив из 4 частей данного участка

		GreenDataVertex* Data;
		DWORD CountAllGreen;	//количество элементов

		ISXBound* BoundVolumeSys;	//выравненный ограничивающий объем для равномерного деления
		ISXBound* BoundVolumeP;		//облегающий ограничивающий объем

		float DistForCamera;

		ID Id;	//идентификатор куска
	
		//ID3DXMesh* BoundBox;	//ограничивающий параллелепипед (меш)
		bool BFNonEnd;//имеет ли кусок куски внутри себя?
	};

	struct Lod
	{
		Lod();
		~Lod();
		String path;
		Array<ID> idstex;
		ISXDataStaticModel* model;
	};

	struct Model
	{
		SX_ALIGNED_OP_MEM

		Model();
		~Model();

		struct NavMesh
		{
			NavMesh();
			~NavMesh();
			float3_t* arr_vertex;
			int32_t count_vertex;
			uint32_t* arr_index;
			ID* arr_mtl;
			int32_t count_index;
			String pathname;
		};

		GeomGreenType TypeGreen;	//
		String MaskName;
		uint32_t AllCountGreen;	//сколько всего сгенерировано объектов
		char Name[OBJECT_NAME_MAX_LEN];
		NavMesh* NavigateMesh;

		Segment* SplitsTree;
		Array<Segment*> SplitsArr;
		float3 BBMax, BBMin;
		GreenDataVertex* AllTrans;//
		Lod* ArrLod[GREEN_COUNT_LOD];
		ID SplitsIDs;	//общее количество сегментов/спилтов
	};

	//структура содержащая минимальную необходимую информацию о сегменте модели
	struct InfoRenderSegments
	{
		InfoRenderSegments();
		~InfoRenderSegments();
		Segment** Arr;	//массив хранящий в себе указатели на сегменты REALTIME
		DWORD Count;	//размер Arr 
		DWORD CountCom;	//сколько сегментов записано в Arr	REALTIME
	};

	struct IRSData
	{
		IRSData();
		~IRSData();
		Array<Segment*, GREEN_DEFAULT_RESERVE_COM> queue;
		Array<InfoRenderSegments*> arr;
	};
	
protected:
	Array<IRSData*> ArrComFor; //информация о сегментах для рендера
	void SaveSplit(Segment* Split, FILE* file, Array<Segment*> * queue);
	void LoadSplit(Segment** Split, FILE* file, Array<Segment**> * queue);
	void GPURender2(DWORD timeDelta, float3* viewpos, ID nm, int lod, ID id_tex);
	void AlignBound(Model* model, Segment* split);
	void GenByTex(StaticGeom* geom, Model* model, ID idmask, float3* min, float3* max, float count_max);

	void GetPartBeam(float3* pos, float3 * dir, Segment** arrsplits, DWORD *count, Segment* comsegment, ID curr_splits_ids_render);
	void GetPartBB(float3* bbmin, float3 * bbmax, Segment** arrsplits, DWORD *count, Segment* comsegment, ID curr_splits_ids_render);

	void ComRecArrIndeces(ISXFrustum* frustum, Segment** arrsplits, DWORD *count, Segment* comsegment, float3* viewpos, Array<Segment*, GREEN_DEFAULT_RESERVE_COM>* queue, ID curr_splits_ids_render);

	void AddModelInArrCom(ID id_model);
	void DelModelInArrCom(ID id_model);

	Array<Model*> ArrModels;
	IDirect3DVertexBuffer9* TransVertBuf;	//буфер вершин с трансформациями растительности
	IDirect3DVertexDeclaration9* VertexDeclarationGreen;
	
	void PreSegmentation(Model* model, float3* min_level, float3* max_level);
	void CycleSegmentation(Segment* Split, Model* mesh);
	void Segmentation(Segment* Split, Model* mesh);
	void SetSplitID(Model* model);
	void SetSplitID2(Model* model, Segment* Split, Array<Segment*, GREEN_DEFAULT_RESERVE_COM>* queue);

	long RTCountDrawObj;
	GreenDataVertex* RTGPUArrVerteces;
	D3DXMATRIX mat;

	Segment** jarrsplits;
	DWORD jcount;

	DWORD jCountIndex;
};

int Green::CurrentFreqGrass = 100;
float2_t Green::DistLods = float2_t(50, 100);
float Green::BeginEndLessening = 30;
bool Green::UseSortFrontToBackSplits = false;
IDirect3DDevice9* Green::DXDevice = 0;
char Green::StdPath[1024];

#endif
