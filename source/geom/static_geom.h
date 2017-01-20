
#ifndef __static_geom
#define __static_geom

//максимальное количество полигонов в буферах
//или максимальнео количество полигонов на одну подгруппу
#define STATIC_GEOM_MAX_POLY_IN_GROUP 256000

#define STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr) \
if (!(id_arr < ArrComFor.size()))\
{\
	reportf(REPORT_MSG_LEVEL_ERROR, "static: unresolved id '%d' for array of compute visible", id_arr); \
}

#define STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model) \
if (!(id_model < AllModels.size() && AllModels[id_model]))\
{\
	reportf(REPORT_MSG_LEVEL_ERROR, "static: unresolved id '%d' for array of models", id_model); \
}

#define STATIC_COUNT_TYPE_SEGMENTATION_QUAD 4
#define STATIC_COUNT_TYPE_SEGMENTATION_OCTO 8
#define STATIC_DIFFERENCE_SIDES_FOR_OCTO 0.3
#define STATIC_MIN_ALLVOLUME_FOR_SEGMENTATION 50
#define STATIC_MIN_POLYGONS_FOR_SEGMENTATION 5000
#define STATIC_MIN_COUNT_POLY 500
#define STATIC_MAX_COUNT_POLY 1000
#define STATIC_DEFAULT_RESERVE_COM 512

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
		DWORD** ArrPoly;	//двумерный массив по количеству подгрупп, вложенный массив - все полигоны для данной подгруппы
		DWORD* CountPoly;	//массив с количеством полигонов на каждую подгруппу
		DWORD* NumberGroup;	//массив с номерами подгрупп в контексте уровня
		DWORD* NumberGroupModel;//массив с номерами подгрупп в контексте модели
		DWORD CountSubSet;	//количество подгрупп
		DWORD CountAllPoly;	//общее количество полигонов

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
		SX_ALIGNED_OP_MEM

		Model();
		~Model();

		struct Lod
		{
			Lod();
			~Lod();
			char PathName[1024];
			DataStaticModel* model;
			Array<DWORD> IDsTexs;
		};

		struct GDataBuff
		{
			long idgroup;	//id подгруппы в контексте уровня
			long idbuff;	//id буфера (в подгруппе) в который заисаны данные геометрии модели

			long IndexStart;
			long IndexCount;
			long VertexStart;
			long VertexCount;
		};

		char Name[64];
		char PathName[1024];
		long CountPoly;
		float3 Position;
		float3 Rotation;
		float3 Scale;

		float3 OldPosition;
		float3 OldRotation;
		float3 OldScale;

		Lod Lod0;

		bool IsRenderLod;

		Array<GDataBuff> SubSet;	//описание каждой подгруппы модели
		Segment* ArrSplits;	//массив с сегментами	
		long SplitsIDs;	//общее количество сегментов/спилтов
		long SplitsIDsRender;	//количество рисубщихся сегментов
	};

	//подгруппа уровня
	struct Group
	{
		Group();
		~Group();
		String name;	//имя текстуры
		DWORD idtex;	//идентификатор текстуры
		long AllCountVertex;	//общее количество вершин
		long AllCountIndex;		//общее количество индексов
		Array<long, 4> CountVertex;	//количество вершин в буферах
		Array<long, 4> CountIndex;	//количество индексов в буферах
		Array<Array<Model*>> ArrModels;
		Array<IDirect3DVertexBuffer9*, 4> VertexBuffer;
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

	void Clear();
	void Save(const char* path);
	void Load(const char* path);
	
	inline long GetCountModel();

	void CPUFillingArrIndeces(ISXFrustum* frustum,float3* viewpos,long id_arr = 0);
	bool GetIntersectedRayY(float3* pos);
	
	void GPURender(DWORD timeDelta,long id_arr = 0);
	long AddModel(const char* path,const char* lod1,const char* name);
	void DelModel(long id);

	inline void GetMinMax(float3* min,float3* max);

	long AddArrForCom();
	void DelArrForCom(long id_arr);

	char* GetModelName(long id);
	const char* GetModelPathName(long id);
	long GetModelCountPoly(long id);

	float3* GetModelPosition(long id);
	float3* GetModelRotation(long id);
	float3* GetModelScale(long id);

	const char* GetModelLodPath(long id);
	void SetModelLodPath(long id,const char* path);

	void ApplyTransform(long id);

protected:

	Array<IRSData> ArrComFor; //информация о сегментах для рендера
	void AddModelInArrCom(long id_model);
	void DelModelInArrCom(long id_model);

	float4x4 WorldMat;
	void ApplyTransformLod(long id,float3* centerpos);

	ISXBound* BoundVolume;

	void GetIntersectedRayY2(float3* pos, Segment** arrsplits, DWORD *count, Segment* comsegment, DWORD curr_splits_ids_render);

	void SaveSplit(Segment* Split, FILE* file, Array<Segment*> * queue);
	void LoadSplit(Segment** Split, FILE* file, Array<Segment**> * queue);

	void PreSegmentation(Model* mesh, DataStaticModel* model);	//подготовительный этап сегментации
	void Segmentation(Segment* Split, Model* mesh, DataStaticModel* model, long CountSplitsSys, long CountPolyInSegment, Array<Segment*> * queue);	//сегментации модели
	void CycleSegmentation(Segment* Split, Model* mesh, DataStaticModel* model, long CountSplitsSys, long CountPolyInSegment);	//рекусривный вызов сегментации
	void EditVolume(Model* mesh, Segment* Split);
	void SetSplitID(Segment* Split, long* SplitsIDs, long* SplitsIDsRender);	//установка каждому куску идентификатора, удаление пустых кусков
	void SetSplitID2(Segment* Split, long* SplitsIDs, long* SplitsIDsRender, Array<Segment*>* queue);
	void ComRecArrIndeces(ISXFrustum* frustum, Segment** arrsplits, DWORD *count, Segment* comsegment, float3* viewpos, Array<Segment*, STATIC_DEFAULT_RESERVE_COM>* queue, DWORD curr_splits_ids_render);

	void UpdateArrMeshVertex2(long count_vertex, vertex_static* arrvertex);
	//рабочие данные, используются внутри в методах
	//{{
	float3* ArrMeshVertex;
	long CountVertex;
	float3* ArrMeshVertex2;
	long AllCountVertex2;

	D3DXVECTOR3 jpos;
	D3DXVECTOR3 jvevyp;
	D3DXVECTOR3 jvevyn;
	float3 jvec0, jvec1, jvec2;

	//}}

	//функции удаления и создания массивов RTCPUArrIndicesPtrs и RTCountDrawPoly
	//при добавлении новой модели
	void DelArrIndexPtr();
	void InitArrIndexPtr();

	DWORD*** RTCPUArrIndicesPtrs;//массив для хранения всех индексов которые будут отправлены на рендер сейчас
	DWORD** RTCountDrawPoly;	//массив для хранения размеров для каждого из массивов RTCPUArrIndicesPtrs

	Array<Model*> AllModels;	//массив моделей
	Array<Group*> AllGroups;		//массив подгрупп
	IDirect3DVertexDeclaration9* VertexDeclarationStatic;

	long SizeRenderIndexBuffer;	//размер в элементах RenderIndexBuffer
	IDirect3DIndexBuffer9* RenderIndexBuffer;	//индексный буфер, используется в изменяется в реайлтайме при рендере уровня	
};

bool StaticGeom::UseSortFrontToBackSplits = true;
bool StaticGeom::UseSortFrontToBackModels = true;
IDirect3DDevice9* StaticGeom::DXDevice = 0;
char StaticGeom::StdPath[1024];
//sprintf(StaticGeom::StdPath, "");
float StaticGeom::DistForLod = 200.f;

#endif