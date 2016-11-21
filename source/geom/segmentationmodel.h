

//структура трансформаций растительности
struct GreenDataVertex
{
	float3_t Position;	//позиция
	float3_t TexCoord;	//x - общий масштаб,y - поворот по оси y, z - 
};

//описание одного сегмента поделенной модели
struct Segment
{
	Segment();
	~Segment();

	DWORD SortId[8];

	SX_ALIGNED_OP_MEM
	
	Segment* SplitMeshes[8]; //массив из 4/8 частей данного участка

	//для геометрии
	DWORD** ArrPoly;	//двумерный массив по количеству подгрупп, вложенный массив - все полигоны для данной подгруппы
	//DWORD** ArrSerialNum;
	DWORD* CountPoly;	//массив с количеством полигонов на каждую подгруппу
	DWORD* NumberGroup;	//массив с номерами подгрупп которые присутсвуют
	int* TypeMaterial;//массив с типами материалов подгрупп
	DWORD CountSubSet;	//количество подгрупп
	DWORD CountAllPoly;	//общее количество полигонов

	//для растительности
	GreenDataVertex* GreenData;
	
	Bound BoundVolumeSys;	//выравненный ограничивающий объем для равномерного деления
	Bound BoundVolumeP;		//облегающий ограничивающий объем
	
	//нужно ли?
	float DistForCamera;
	float DistFor;
	bool IsRenderForCamera;
	bool IsRenderFor;

	DWORD ID;	//идентификатор куска
	DWORD SID;	//порядковый номер куска из массива рисующихся кусков
	DWORD WSID;
	DWORD** ArrSerialNum;
	ID3DXMesh* BoundBox;	//ограничивающий параллелепипед (меш)
	bool BFNonEnd;//имеет ли кусок куски внутри себя?
};

//делимая модель
//!!! статическая
class SegmentationModel : public Model
{
public:
	SegmentationModel();
	~SegmentationModel();

	SX_ALIGNED_OP_MEM

	void Segmentation(
						int type,		//тип деления модели, 4 - quad, 8 - octo
						DWORD count_poly_in_segment	//минимальное количество полигонов в сегменте
						);

	void GPURender(
					Segment** arrsplits,	//массив с сегментами
					DWORD count,			//количество ключей массива
					bool iscomdistlod,		//если возможно то рисуем лод
					bool render_alpha,		//рисовать ли прозврачные объекты
					bool ismaterial,		//рисовать ли материал
					bool is_old_render,		//использовать старый рендер, для некоторых случаев полезно
					bool is_one_dip,		//рисовать ли одним проходом, можно для теней
					bool render_forward,	//рисовать ли тот материал который освещать не надо
					bool is_camera,
					DWORD tex_selected=-1); //iscomdistlod - если включен флаг рисования лода то нужно ли рисовать лод (true), false - рисуем максимальную детализацию

	bool IsVisible(Core::ControllMoving::Frustum* frustum);	//видан ли модель фрустуму
	void CPUFillingArrIndeces(
								Core::ControllMoving::Frustum* frustum,		//фрустум попадание в который считаем
								Segment** arrsplits,	//массив (динамический, обычно по количеству SplitsIDs) который заполняется указателями на входящие объекты
								DWORD *count,			//количество вошедших сегментов
								float3* viewpos,			//то нужно ли делать дальнейший расчет на попадание во фрустум
								bool is_camera//true и объект далеко то не считаем, false считаем в любом случае
								);
	
	inline DWORD GetCountSplits();		//возвращает общее количество сплитов
	inline DWORD GetCountSplitsRender();//возвращает коилчетсво сплитов которые рисуются
	void GetAllValidSegments(Segment* insplit,Segment** arrsplits,DWORD* count);

	//определение пересечения pos с моделью, определяет только пересечение по оси y и меняет координату y в pos
	void GetIntersectedRayY(float3* pos,Segment** arrsplits,DWORD *count,Segment* comsegment);
	void GetIntersected(const float3* pos, Segment** arrsplits, DWORD *count, Segment* comsegment, float additionalRadius);
	void GetIntersected22(float3* pos,Segment** arrsplits,DWORD *count,Segment* comsegment);

	void GetIntersectedQuad(float3_t* max,float3_t* min,Segment** arrsplits,DWORD *count,Segment* comsegment);

	float DistForLod;	//дистанция от камеры при которой рисуется лод, считается до крайней точки
	inline bool IsLoadLod();
	bool CollisionRain;

	void GetPartFromBeam(const float3* pos, const float3 * dir, Segment** arrsplits, DWORD *count, Segment* comsegment);

	void SetSound(const char* path,float mindist,float dist);
	void GetSound(char* path);

	/*char SounsPath[1024];
	float MinDistForSound;
	float DistForSound;*/
	SkyXEngine::Sound::SoundObject* MeshSound;

//protected:
	bool* ArrSerialNum;		//отрисован ли данный кусок геометрии или нет, идентификация по SID
	bool* IsVisibleGroup;	//массив, каждый ключ которого определяет видна ли подгруппа
	Segment* ArrSegments;	//все сегменты
	DWORD SplitsIDs;		//общее количество всех сегментов
	DWORD SplitsIDsRender;	//количество рисуемых сегментов
	
	bool IsCurrentRenderLod;	//после просчетов определяет будем лод рисовать или полную модель?

	DWORD** RTCPUArrIndicesPtrs;//массив[подгруппа][полигон]
	DWORD* RTCountDrawPoly;		//массив[подгруппа] - сколько полигонов отрисовано в данный момент

	IDirect3DIndexBuffer9* IndexBuffer;	//индексный буфер который заполняется каждый раз когда идет просчет на видимость

	int CountSplitsSys;			//система деления, 0 - неделимое, 4-квадро деление, 8-окто деление
	DWORD CountPolyInSegment;	//миимальнео количество полигонов в сегменте

	float3* ArrMeshVertex;		//массив всех вершин модели (только позиция)
	float* ArrMeshVertexRadius;
	float3* ArrMeshVertexCenter;

	void EditVolume(Segment* SplitingMesh);	//выравнивание боуд бокса куска, для самого первого делать не надо
	
	void SetSplitID(Segment* SplitingMesh);	//установка id на кусок
	void CycleSegmentation(Segment* SplitingMesh);	//запускаем рекурсию деления модели
	void SecondSeparated(Segment* SplitingMesh);
	void ComRecArrIndeces(Core::ControllMoving::Frustum* frustum,Segment** arrsplits,DWORD *count,Segment* comsegment,bool is_camera,float3* viewpos);

	float3 jcenter,jcenter2;
	float jradius;
	float jdist;

	float3 jmin,jmax;
	float3 jmin2,jmax2;
};


///////////////
//структура содержащая минимальную необходимую информацию о сегменте модели
struct InfoRenderSegments
{
	InfoRenderSegments(){Arr=0;Count=0;CountCom=0;};
	~InfoRenderSegments(){};
	Segment** Arr;	//массив хранящий в себе указатели на сегменты REALTIME
	DWORD Count;	//размер Arr 
	DWORD CountCom;	//сколько сегментов записано в Arr	REALTIME
};

class ManagerStaticModel
{
public:
	ManagerStaticModel();
	~ManagerStaticModel();

	void OnLostDevice();
	void OnResetDevice();

	SX_ALIGNED_OP_MEM

	void Clear();

	void Add(	
				const char* name,			//имя модели
				const char* path,			//путь до модели
				int type_segmentation,		//тип сегментации
				DWORD count_poly_in_segment,//минимальнео количество полигонов в сегменте
				const char* lod,			//путь до моедли с лодом
				float dist_for_lod,			//дистанция до лода
				float3* pos,				//позиция
				float3* rot,				//поворот
				bool is_open				//открытая ли модель? аля подземелье? подвал? и не надо рисовать в карту теней??
				);

	DWORD AddFrustum();

	void AllCom(Core::ControllMoving::Frustum* frustum,DWORD id,bool is_com_not_open,float3* viewpos);
	void AllRender(DWORD id,DWORD timeDelta,int render_alpha,bool ismaterial,bool is_com_not_open,bool is_one_dip,bool render_forward);

	inline bool GetOpen(DWORD id);			//возвращает открытость модели
	inline void SetOpen(DWORD id,bool is_open);	//устанавливает открытость для модели

	void AllComForCamera(float3* campos);
	void RenderForCamera(DWORD timeDelta,bool render_alpha,bool ismaterial,bool is_old_render,bool render_forward);

	inline DWORD GetCountPoly(DWORD id);	//возвращает количетсов полигонов модели

	

	//получение установка имени модели
	inline void GetName(DWORD id,char* str);
	inline void SetName(DWORD id,const char* str);

	//получение вектора позиции/вращения
	void GetMatPosition(DWORD id,float3* pos);
	void GetMatRotation(DWORD id,float3* rot);

	//установка вектора позиции/вращения
	void SetMatPosition(DWORD id,float3* pos);
	void SetMatRotation(DWORD id,float3* rot);

	//просчет матриц, необходимо вызывать после установки векторов и перед рендером
	void ComMatrixTrans(DWORD id);

	DWORD GetCount();		//количество загруженных моделей
	void Delete(DWORD id);	//удаляет модель
	bool GetIntersectedRayY(DWORD id,float3* pos);	//поиск пересечения по лучу с моделью на основе позиции pos,pos->y есть столкновение
	bool GetIntersectedRayYRain(float3* pos);
	bool GetIntersectedRayYRainBB(float3* pos);
	bool GetIntersected(float3* pos);
	void GetIntersected22(float3* pos);
	void GetIntersected33(float3* pos);

	void GetAIQuad(float3_t* max,float3_t* min,AIQuad** quad);

	inline DWORD GetSelected();				//возвращает id выделеной модели, или -1
	inline void SetSelected(DWORD id);		//выделяет модель

	inline int GetSysSpliting(DWORD id);
	inline DWORD GetCountPolyInSplit(DWORD id);

	inline void GetModelPath(DWORD id,char* mpath);
	inline void GetLodPath(DWORD id,char* mpath);
	bool TraceBeam(const float3 & start, const float3 & dir, float3 * _res,DWORD* idmesh,int* type_mat,BOOL useEndPos);
	bool TraceBeamY(const float3 & start, const float3 & dir, float3 * _res,DWORD* idmesh);

	Array<DWORD> ArrayWSIDs;

	DWORD NumSelModel;	//id выделенной модели
	
	Array<SegmentationModel*> Arr;				//массив всех моделей
	Array<bool> ArrIsSelected;
	Array<InfoRenderSegments*> ArrComForCamera;	//информация о сегментах для рендера для камеры
	Array<Array<InfoRenderSegments*>> ArrFrustums;	//массив фрустумов с инфой о сегментах для рендера
	Array<InfoRenderSegments*> ArrCollision;		//массив с инфой о сегментах для колизий

	D3DXVECTOR3 jpos;
	D3DXVECTOR3 jvevyp;
	D3DXVECTOR3 jvevyn;
	float3 jvec0,jvec1,jvec2;
};