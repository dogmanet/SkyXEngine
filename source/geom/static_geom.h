
/*
!!!!
GDataBuff
тут добавились нвоые данные, которые при трансформации надо менять
*/
#ifndef __static_geom
#define __static_geom

//#include <handler_dx_func.cpp>
#include <common\\string.cpp>
#include <common\array.h>
#include <common\\string_api.cpp>
//максимальное количество полигонов в буферах
//или максимальнео количество полигонов на одну подгруппу
#define STATIC_GEOM_MAX_POLY_IN_GROUP 400000

#define STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr) \
if (!(id_arr < ArrComFor.size()))\
{\
	reportf(REPORT_MSG_LEVEL_ERROR, "%s - static: unresolved id '%d' for array of compute visible", gen_msg_location, id_arr); \
}

#define STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model, retval) \
if (!(id_model < AllModels.size() && AllModels[id_model]))\
{\
	reportf(REPORT_MSG_LEVEL_ERROR, "%s - static: unresolved id '%d' for array of models", gen_msg_location, id_model); \
	return retval;\
}

#define STATIC_PRECOND_ERR_ID_GROUP(id_model,id_group, ret_val) \
if (!(id_model < AllModels.size() && AllModels[id_model] && id_group < AllModels[id_model]->SubSet.size()))\
{\
	reportf(REPORT_MSG_LEVEL_ERROR, "%s - static: unresolved id '%d' for array of group in model '%d'", gen_msg_location, id_group, id_model); \
	return ret_val; \
}

//типы деления
#define STATIC_COUNT_TYPE_SEGMENTATION_QUAD 4
#define STATIC_COUNT_TYPE_SEGMENTATION_OCTO 8

#define STATIC_DIFFERENCE_SIDES_FOR_OCTO 0.3	//минимальная разница между сторонами для окто деления
#define STATIC_MIN_ALLVOLUME_FOR_SEGMENTATION 20//минимальный общий объем модели для деления
#define STATIC_MIN_POLYGONS_FOR_SEGMENTATION 5000//минимальнео количество полигонов в модели для деления
#define STATIC_MIN_COUNT_POLY 500	//минимальное количество полигонов в сплите
#define STATIC_MAX_COUNT_POLY 1000	//максимальное количество полигонов в сплите

#define STATIC_DEFAULT_RESERVE_COM 512	//резервация для просчетов

class StaticGeom
{
public:

	static bool UseSortFrontToBackSplits;
	static bool UseSortFrontToBackModels;
	static IDirect3DDevice9* DXDevice;
	static char StdPath[1024];
	static float DistForLod;

	StaticGeom();
	~StaticGeom();

	SX_ALIGNED_OP_MEM
	
	//сегмент, кусок модели
	struct Segment
	{
		Segment();
		~Segment();

		Segment* Splits[STATIC_COUNT_TYPE_SEGMENTATION_OCTO]; //массив из 4/8 частей данного участка

		//для геометрии
		uint32_t** ArrPoly;	//двумерный массив по количеству подгрупп, вложенный массив - все полигоны для данной подгруппы
		uint32_t* CountPoly;	//массив с количеством полигонов на каждую подгруппу
		uint32_t* NumberGroup;	//массив с номерами подгрупп в контексте уровня
		uint32_t* NumberGroupModel;//массив с номерами подгрупп в контексте модели
		uint32_t CountSubSet;	//количество подгрупп
		uint32_t CountAllPoly;	//общее количество полигонов

		ISXBound* BoundVolumeSys;	//выравненный ограничивающий объем для равномерного деления
		ISXBound* BoundVolumeP;		//облегающий ограничивающий объем

		float DistForCamera;

		DWORD ID;	//идентификатор куска
		DWORD SID;	//порядковый номер куска из массива рисующихся кусков

		//ID3DXMesh* BoundBox;	//ограничивающий параллелепипед (меш)
		bool BFNonEnd;//имеет ли кусок куски внутри себя?
	};

	

	//модель, главный юнит уровня
	struct Model
	{
		Model();
		~Model();

		SX_ALIGNED_OP_MEM

		struct Lod
		{
			Lod();
			~Lod();

			int SortGroup;
			char PathName[1024];
			ISXDataStaticModel* model;
			Array<ID> IDsTexs;
			ISXBound* BoundVolume;
		};

		struct GDataBuff
		{
			int32_t idgroup;//id подгруппы в контексте уровня
			int32_t idbuff;	//id буфера (в подгруппе) в который заисаны данные геометрии модели
			int32_t IndexStart;
			int32_t IndexCount;
			int32_t VertexStart;
			int32_t VertexCount;

			D3DXPLANE Plane;
			float3_t Max;
			float3_t Min;
			float3_t Center;
		};

		
		float3 Position;
		float3 Rotation;
		float3 Scale;

		float3 OldPosition;
		float3 OldRotation;
		float3 OldScale;

		char Name[64];
		char PathName[1024];
		int32_t CountPoly;

		Lod Lod0;

		bool IsRenderLod;

		Array<GDataBuff> SubSet;	//описание каждой подгруппы модели
		Array<float> GroupDist;
		Array<ID> IDTex;
		Segment* ArrSplits;	//массив с сегментами	
		ID SplitsIDs;	//общее количество сегментов/спилтов
		ID SplitsIDsRender;	//количество рисубщихся сегментов
	};


	//подгруппа уровня
	struct Group
	{
		Group();
		~Group();

		/*struct VertexBuff
		{
			VertexBuff();
			~VertexBuff();

			float3_t* arr;
			long count;
		};*/

		bool IsRenderSingly;
		int SortGroup;//тип/вид/сорт подгруппы, для ранжирования рендера
		String name;//имя текстуры
		ID idtex;	//идентификатор текстуры
		long AllCountVertex;	//общее количество вершин
		long AllCountIndex;		//общее количество индексов
		Array<int32_t, 4> CountVertex;	//количество вершин в буферах
		Array<int32_t, 4> CountIndex;	//количество индексов в буферах
		Array<Array<Model*>> ArrModels;
		Array<IDirect3DVertexBuffer9*, 4> VertexBuffer;
		//Array<VertexBuff*, 4> VertexBufferOrigin;
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
		Array<Segment*, STATIC_DEFAULT_RESERVE_COM> queue;
		Array<InfoRenderSegments*> arr;
	};

	void OnLostDevice();
	void OnResetDevice();

	void Clear();
	void Save(const char* path);
	void Load(const char* path);

	void SortGroup(float3* viewpos, int sort_mtl);
	
	inline long GetCountModel();

	void CPUFillingArrIndeces(ISXFrustum* frustum, float3* viewpos, ID id_arr = 0);
	bool GetIntersectedRayY(float3* pos);
	
	void GPURender(DWORD timeDelta, int sort_mtl, ID id_arr = 0, ID exclude_model_id = -1, ID exclude_group_id = -1, bool is_sorted = false);
	ID AddModel(const char* path, const char* lod1, const char* name);
	void DelModel(ID id);

	inline void GetMinMax(float3* min,float3* max);

	ID AddArrForCom();
	void DelArrForCom(ID id_arr);

	char* GetModelName(ID id);
	const char* GetModelPathName(ID id);
	long GetModelCountPoly(ID id);

	float3* GetModelPosition(ID id);
	float3* GetModelRotation(ID id);
	float3* GetModelScale(ID id);

	const char* GetModelLodPath(ID id);
	void SetModelLodPath(ID id, const char* path);

	void ApplyTransform(ID id);

	ID GetModelCountGroups(ID id);
	ID GetModelGroupIDMat(ID id, ID group);
	void GetModelGroupCenter(ID id, ID group, float3_t* center);
	void GetModelGroupMin(ID id, ID group, float3_t* min);
	void GetModelGroupMax(ID id, ID group, float3_t* max);
	void GetModelGroupPlane(ID id, ID group, D3DXPLANE* plane);

	void GetArrBuffsGeom(float3_t*** arr_vertex, int32_t** arr_count_vertex, uint32_t*** arr_index, int32_t** arr_count_index, int32_t* count_models);
	/*
	(*arr_vertex)[num_model][num_vertex] - вершины модели
	(*arr_count_vertex)[num_model] - количество вершин для модели
	(*arr_index)[num_model][num_vertex] - индексы модели
	(*arr_count_index)[num_model] - количество индексов для модели
	(*count_model); количество моделей
	*/

protected:

	struct InfoGroup
	{
		ID model;
		ID group;
		ID g_group;
		float dist;
		int count;
	};
	Array<InfoGroup*> DistGroup;
	Array<IRSData*> ArrComFor; //информация о сегментах для рендера
	void AddModelInArrCom(ID id_model);
	void DelModelInArrCom(ID id_model);

	float4x4 WorldMat;
	float4x4 RotationMat;
	void ApplyTransformLod(ID id);

	ISXBound* BoundVolume;

	void GetIntersectedRayY2(float3* pos, Segment** arrsplits, DWORD *count, Segment* comsegment, ID curr_splits_ids_render);

	void SaveSplit(Segment* Split, FILE* file, Array<Segment*> * queue);
	void LoadSplit(Segment** Split, FILE* file, Array<Segment**> * queue);

	void PreSegmentation(Model* mesh, ISXDataStaticModel* model);	//подготовительный этап сегментации
	void Segmentation(Segment* Split, Model* mesh, ISXDataStaticModel* model, long CountSplitsSys, long CountPolyInSegment, Array<Segment*> * queue);	//сегментации модели
	void CycleSegmentation(Segment* Split, Model* mesh, ISXDataStaticModel* model, long CountSplitsSys, long CountPolyInSegment);	//рекусривный вызов сегментации
	void EditVolume(Model* mesh, Segment* Split);
	void SetSplitID(Segment* Split, ID* SplitsIDs, ID* SplitsIDsRender);	//установка каждому куску идентификатора, удаление пустых кусков
	void SetSplitID2(Segment* Split, ID* SplitsIDs, ID* SplitsIDsRender, Array<Segment*>* queue);
	void ComRecArrIndeces(ISXFrustum* frustum, Segment** arrsplits, DWORD *count, Segment* comsegment, float3* viewpos, Array<Segment*, STATIC_DEFAULT_RESERVE_COM>* queue, ID curr_splits_ids_render);

	//рабочие данные, используются внутри в методах
	//{{
	float3* ArrMeshVertex;
	long CountVertex;

	D3DXVECTOR3 jpos;
	D3DXVECTOR3 jvevyp;
	D3DXVECTOR3 jvevyn;
	float3 jvec0, jvec1, jvec2;

	//}}

	//функции удаления и создания массивов RTCPUArrIndicesPtrs и RTCountDrawPoly
	//при добавлении новой модели
	void DelArrIndexPtr();
	void InitArrIndexPtr();

	uint32_t*** RTCPUArrIndicesPtrs;	//массив для хранения всех индексов которые будут отправлены на рендер сейчас
	uint32_t** RTCountDrawPoly;			//массив для хранения размеров для каждого из массивов RTCPUArrIndicesPtrs
	uint32_t*** RTCountDrawPolyModel;	//массив для хранения количества рисуемых полигонов для каждой подгруппы для каждой модели
	uint32_t*** RTBeginIndexModel;		//массив для хранения начала индексов для каждой подгруппы для каждой модели

	Array<Model*> AllModels;	//массив моделей
	Array<Group*> AllGroups;	//массив подгрупп
	//IDirect3DVertexDeclaration9* VertexDeclarationStatic;

	long SizeRenderIndexBuffer;	//размер в элементах RenderIndexBuffer
	IDirect3DIndexBuffer9* RenderIndexBuffer;	//индексный буфер, используется и изменяется в реайлтайме при рендере уровня	
};

bool StaticGeom::UseSortFrontToBackSplits = true;
bool StaticGeom::UseSortFrontToBackModels = true;
IDirect3DDevice9* StaticGeom::DXDevice = 0;
char StaticGeom::StdPath[1024];
//sprintf(StaticGeom::StdPath, "");
float StaticGeom::DistForLod = 200.f;

#endif