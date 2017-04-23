

//простой объект с минимальным описанием
//дл€ корректного использовани€ необходимо сначала установить позицию/поворот/масштаб после чего CalculateWorld
struct SXTransObject : public virtual ISXTransObject
{
	SXTransObject(){};
	~SXTransObject(){};

	void Release(){ mem_del(this); };

	SX_ALIGNED_OP_MEM

	inline float4x4* CalcWorld();
};

#define TRANSFORM_COORD_SCREEN2(point,sizemapdepth)\
	point.x /= abs(point.w); \
	point.y /= abs(point.w); \
	point.z /= sizemapdepth->z; \
	point.x = point.x * 0.5 + 0.5; \
	point.y = point.y * (-0.5) + 0.5; \
	point.x *= sizemapdepth->x; \
	point.y *= sizemapdepth->y;

//класс ограничивающего объема
//дл€ создан€и минимума и максимума необходимо вызвать CalculateBound
//SetMinMax, GetMinMax до вызова CalculateWorldAndTrans возвращают нетрансформирвоанные данные
//конечным этапом построени€ Bound и Object €вл€етс€ CalculateWorldAndTrans
class SXBound : public SXTransObject, public virtual ISXBound
{
public:
	SXBound(){};
	~SXBound(){};

	void Release(){ mem_del(this); };

	SX_ALIGNED_OP_MEM

	void CalcBound(IDirect3DVertexBuffer9* vertex_buffer, DWORD count_vert, DWORD bytepervert);

	//функци€ просчета мировой матрицы и трансформации минимума и максимума
	float4x4* CalcWorldAndTrans();

	void GetPosBBScreen(SXPosBBScreen *res, float3* campos, float3* sizemapdepth, float4x4* mat);

	inline void SetMinMax(float3* min, float3* max);
	inline void GetMinMax(float3* min, float3* max);

	inline void SetSphere(float3* center, float* radius);
	inline void GetSphere(float3* center, float* radius) const;

	inline bool IsPointInSphere(float3* point);
	inline bool IsPointInBox(float3* point);
};