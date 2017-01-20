#ifndef _SXMath_H_
#define _SXMath_H_

#include <xmmintrin.h>
#include <math.h>

#define SX_ALIGNED_OP_MEM \
void* operator new(size_t size)\
{\
	return (_aligned_malloc(size, 16));\
};\
\
void operator delete(void* ptr)\
{\
	_aligned_free(ptr);\
};\
\
void* operator new[](size_t size)\
{\
	return (_aligned_malloc(size, 16));\
};\
\
void operator delete[](void* ptr)\
{\
	_aligned_free(ptr);\
};

#ifdef SM_D3D_CONVERSIONS
#include <d3dx9math.h>
#endif

#define SM_PI               3.141592654f
#define SM_2PI              6.283185307f
#define SM_1DIVPI           0.318309886f
#define SM_1DIV2PI          0.159154943f
#define SM_PIDIV2           1.570796327f
#define SM_PIDIV4           0.785398163f

#define lerp(x,y,s) (x + s*(y - x))
#define sign(x) (x >= 0 ? (x>0 ?1 :0) : -1)
#define sign2(x) (x >= 0 ? 1 : -1)
#define SMToRadian(degree)((degree)*(SM_PI / 180.0f))

inline float randf(float lowBound, float highBound)
{
	if( lowBound >= highBound ) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound; 
}

inline float lerpf(float x,float y,float s)
{
	return x + s*(y - x);
}

__declspec(align(16)) struct SMVECTOR
{
	union
	{
		__m128 mmv;
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};


	SX_ALIGNED_OP_MEM


	/*void* operator new(size_t size)
	{
		return (_aligned_malloc(size, 16));
	};
				
	void operator delete(void* ptr)
	{
		_aligned_free(ptr);
	};

	void* operator new[](size_t size)
	{
		return (_aligned_malloc(size, 16));
	};
				
	void operator delete[](void* ptr)
	{
		_aligned_free(ptr);
	};*/

	SMVECTOR & operator+=(const SMVECTOR & V)
	{
		mmv = _mm_add_ps(mmv, V.mmv);
		return(*this);
	};

	SMVECTOR & operator-=(const SMVECTOR & V)
	{
		mmv = _mm_sub_ps(mmv, V.mmv);
		return(*this);
	};

	SMVECTOR & operator*=(const SMVECTOR & V)
	{
		mmv = _mm_mul_ps(mmv, V.mmv);
		return(*this);
	};

	SMVECTOR & operator/=(const SMVECTOR & V)
	{
		mmv = _mm_div_ps(mmv, V.mmv);
		return(*this);
	};

	float & operator[](const int& key)
	{
			if(key == 0)
				return this->x;
			else if(key == 1)
				return this->y;
			else if(key == 2)
				return this->z;
			else if(key == 3)
				return this->w;
	};

	SMVECTOR & operator*=(const float & F);

	SMVECTOR & operator/=(const float & F);

	operator __m128&()
	{
		return(mmv);
	}

	operator __m128*()
	{
		return(&mmv);
	}

	operator const __m128&() const
	{
		return(mmv);
	}

	operator const __m128*() const
	{
		return(&mmv);
	}
};

__declspec(align(16)) struct SMVECTORI32
{
	union
	{
		int i[4];
		SMVECTOR v;
	};

	operator SMVECTOR&()
	{
		return(v);
	};

	operator const SMVECTOR&() const
	{
		return(v);
	};

	operator __m128&()
	{
		return(v.mmv);
	}

	operator __m128*()
	{
		return(&v.mmv);
	}

	operator const __m128&() const
	{
		return(v.mmv);
	}

	operator const __m128*() const
	{
		return(&v.mmv);
	}
};

__declspec(align(16)) struct float2: public SMVECTOR
{
	float2()
	{
		mmv = _mm_setzero_ps();
	};

	float2(float x, float y)
	{
		mmv = _mm_set_ps(1.0f, 0.0f, y, x);
	};

	float2(const float2 & V)
	{
		mmv = V.mmv;
	};

	float2(const SMVECTOR & V)
	{
		mmv = V.mmv;
	};

#ifdef SM_D3D_CONVERSIONS
	float2(const D3DXVECTOR2 & v)
	{
		mmv = _mm_set_ps(1.0f, 0.0f, v.y, v.x);
	}
#endif

	operator SMVECTOR()
	{
		SMVECTOR r;
		r.mmv = mmv;
		return(r);
	};

	float2 & operator=(const float2 & V)
	{
		mmv = V.mmv;
		return(*this);
	};

#ifdef SM_D3D_CONVERSIONS
	operator D3DXVECTOR2()
	{
		return(D3DXVECTOR2(x, y));
	}
#endif
};

__declspec(align(16)) struct float3: public SMVECTOR
{
	float3()
	{
		mmv = _mm_setzero_ps();
	};

	float3(float x, float y, float z)
	{
		mmv = _mm_set_ps(1.0f, z, y, x);
	};

	float3(const float3 & V)
	{
		mmv = V.mmv;
	};

	float3(const SMVECTOR & V)
	{
		mmv = V.mmv;
	};

	float3(const float2 & V, float _z)
	{
		mmv = V.mmv;
		z = _z;
	};

#ifdef SM_D3D_CONVERSIONS
	float3(const D3DXVECTOR3 & v)
	{
		mmv = _mm_set_ps(1.0, v.z, v.y, v.x);
	}
#endif

	operator SMVECTOR()
	{
		SMVECTOR r;
		r.mmv = mmv;
		return(r);
	};

	float3 & operator=(const float3 & V)
	{
		mmv = V.mmv;
		return(*this);
	};

#ifdef SM_D3D_CONVERSIONS
	operator D3DXVECTOR3()
	{
		return(D3DXVECTOR3(x, y, z));
	}
#endif
};

__declspec(align(16)) struct float4: public SMVECTOR
{
	float4()
	{
		mmv = _mm_setzero_ps();
	};

	float4(float x, float y, float z, float w)
	{
		mmv = _mm_set_ps(w, z, y, x);
	};

	float4(const float4 & V)
	{
		mmv = V.mmv;
	};

	float4(const float2 & V, float _z, float _w)
	{
		mmv = V.mmv;
		z = _z;
		w = _w;
	};

	float4(const float3 & V, float _w)
	{
		mmv = V.mmv;
		w = _w;
	};

	float4(const SMVECTOR & V)
	{
		mmv = V.mmv;
	};

	
#ifdef SM_D3D_CONVERSIONS
	float4(const D3DXVECTOR4 & v)
	{
		mmv = _mm_set_ps(v.w, v.z, v.y, v.x);
	}
#endif

	operator SMVECTOR()
	{
		SMVECTOR r;
		r.mmv = mmv;
		return(r);
	};

	float4 & operator=(const float4 & V)
	{
		mmv = V.mmv;
		return(*this);
	};

#ifdef SM_D3D_CONVERSIONS
	operator D3DXVECTOR4()
	{
		return(D3DXVECTOR4(x, y, z, w));
	}
#endif
};

struct float2_t
{
	float x;
	float y;

	float2_t():x(0.0f), y(0.0f)
	{
	}

	float2_t(float _x, float _y):x(_x), y(_y)
	{
	}

	float2_t(const float2 & f):x(f.x), y(f.y)
	{
	}

	__forceinline operator float2() const
	{
		return(float2(x, y));
	}
};

struct float3_t
{
	float x;
	float y;
	float z;

	float3_t():x(0.0f), y(0.0f), z(0.0f)
	{
	}

	float3_t(float _x, float _y, float _z):x(_x), y(_y), z(_z)
	{
	}

	float3_t(const float3 & f):x(f.x), y(f.y), z(f.z)
	{
	}

	__forceinline operator float3() const
	{
		return(float3(x, y, z));
	}
};

struct float4_t
{
	float x;
	float y;
	float z;
	float w;

	float4_t():x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{
	}

	float4_t(float _x, float _y, float _z, float _w):x(_x), y(_y), z(_z), w(_w)
	{
	}

	float4_t(const float4 & f):x(f.x), y(f.y), z(f.z), w(f.w)
	{
	}

	__forceinline operator float4() const
	{
		return(float4(x, y, z, w));
	}
};


__forceinline SMVECTOR operator+(const SMVECTOR & V1, const SMVECTOR & V2)
{
	SMVECTOR r(V1);
	r += V2;
	return(r);
};

__forceinline SMVECTOR operator-(const SMVECTOR & V1, const SMVECTOR & V2)
{
	SMVECTOR r(V1);
	r -= V2;
	return(r);
};

__forceinline SMVECTOR operator-(const SMVECTOR & V1)
{
	SMVECTOR r(V1);
	r *= float4(-1.0f, -1.0f, -1.0f, -1.0f);
	return(r);
};

__forceinline SMVECTOR operator*(const SMVECTOR & V1, const SMVECTOR & V2)
{
	SMVECTOR r(V1);
	r *= V2;
	return(r);
};

__forceinline SMVECTOR operator/(const SMVECTOR & V1, const SMVECTOR & V2)
{
	SMVECTOR r(V1);
	r /= V2;
	return(r);
};

__forceinline SMVECTOR operator*(const SMVECTOR & V1, const float & F)
{
	SMVECTOR r(V1);
	r *= F;
	return(r);
};
__forceinline SMVECTOR operator*(const float & F, const SMVECTOR & V1)
{
	SMVECTOR r(V1);
	r *= F;
	return(r);
};


__forceinline SMVECTOR operator/(const SMVECTOR & V1, const float & F)
{
	SMVECTOR r(V1);
	r /= F;
	return(r);
};


__forceinline SMVECTOR & SMVECTOR::operator *= (const float & F)
{
	mmv = _mm_mul_ps(mmv, float4(F, F, F, F));
	return(*this);
};

__forceinline SMVECTOR & SMVECTOR::operator /= (const float & F)
{
	mmv = _mm_div_ps(mmv, float4(F, F, F, F));
	return(*this);
};

__declspec(align(16)) struct SMMATRIX
{
	union
	{
		SMVECTOR r[4];
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

	SX_ALIGNED_OP_MEM


	/*void* operator new(size_t size)
	{
		return (_aligned_malloc(size, 16));
	};
				
	void operator delete(void* ptr)
	{
		_aligned_free(ptr);
	};

	void* operator new[](size_t size)
	{
		return (_aligned_malloc(size, 16));
	};
				
	void operator delete[](void* ptr)
	{
		_aligned_free(ptr);
	};*/

	SMMATRIX()
	{
		r[0] = float4(1.0f, 0.0f, 0.0f, 0.0f);
		r[1] = float4(0.0f, 1.0f, 0.0f, 0.0f);
		r[2] = float4(0.0f, 0.0f, 1.0f, 0.0f);
		r[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
	};

	SMMATRIX(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	)
	{
		r[0] = float4(m00, m01, m02, m03);
		r[1] = float4(m10, m11, m12, m13);
		r[2] = float4(m20, m21, m22, m23);
		r[3] = float4(m30, m31, m32, m33);
	};
#ifdef SM_D3D_CONVERSIONS
	SMMATRIX(const D3DXMATRIX & m)
	{
		r[0] = float4(m._11, m._12, m._13, m._14);
		r[1] = float4(m._21, m._22, m._23, m._24);
		r[2] = float4(m._31, m._32, m._33, m._34);
		r[3] = float4(m._41, m._42, m._43, m._44);
	}
#endif

	SMMATRIX & operator=(const SMMATRIX & M)
	{
		r[0] = M.r[0];
		r[1] = M.r[1];
		r[2] = M.r[2];
		r[3] = M.r[3];
		return(*this);
	};

	SMMATRIX & operator*=(const SMMATRIX & M);
	SMMATRIX operator*(const SMMATRIX & M);
#ifdef SM_D3D_CONVERSIONS
	operator D3DXMATRIX()
	{
		return(D3DXMATRIX(_11, _12, _13, _14,
			_21, _22, _23, _24,
			_31, _32, _33, _34,
			_41, _42, _43, _44));
	}
#endif
};

typedef SMMATRIX float4x4;

__forceinline SMVECTOR SMVectorZero()
{
	SMVECTOR v;
	v.mmv = _mm_setzero_ps();
	return(v);
}

__forceinline SMVECTOR SMVectorMin(const SMVECTOR & V1, const SMVECTOR & V2)
{
	SMVECTOR v;
	v.mmv = _mm_min_ps(V1, V2);
	return(v);
}

__forceinline SMVECTOR SMVectorMax(const SMVECTOR & V1, const SMVECTOR & V2)
{
	SMVECTOR v;
	v.mmv = _mm_max_ps(V1, V2);
	return(v);
}

__forceinline SMVECTOR SMVectorLerp(const SMVECTOR & V1, const SMVECTOR & V2, float t)
{
	SMVECTOR L, S;
	SMVECTOR Result;

	L.mmv = _mm_sub_ps(V2, V1);

	S.mmv = _mm_set_ps1(t);

	Result.mmv = _mm_mul_ps(L, S);
	Result.mmv = _mm_add_ps(Result, V1);
	return(Result);
}

__forceinline float SMVector2Dot(const float2 & V1, const float2 & V2)
{
	SMVECTOR vLengthSq;
	vLengthSq .mmv = _mm_mul_ps(V1, V2);
	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);
	return(vLengthSq.x);
}

__forceinline float2 SMVector2Cross(const float2 & V1, const float2 & V2)
{
	SMVECTOR vResult;
	vResult.mmv = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(0, 1, 0, 1));
	vResult.mmv = _mm_mul_ps(vResult, V1);
	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vResult, vResult, _MM_SHUFFLE(1, 1, 1, 1));
	vResult.mmv = _mm_sub_ss(vResult, vTemp);
	vResult.mmv = _mm_shuffle_ps(vResult, vResult, _MM_SHUFFLE(0, 0, 0, 0));
	return(vResult);
}

__forceinline float SMVector2Length(const float2 & V)
{
	SMVECTOR vLengthSq;
	vLengthSq.mmv = _mm_mul_ps(V, V);
	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	vLengthSq.mmv = _mm_sqrt_ps(vLengthSq);
	return(vLengthSq.x);
}

__forceinline float2 SMVector2Normalize(const float2 & V)
{
	static const SMVECTORI32 maskInf = {0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000};
	static const SMVECTORI32 maskNaN = {0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000};

	SMVECTOR vLengthSq;
	vLengthSq.mmv = _mm_mul_ps(V, V);
	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	
	float2 vResult;
	vResult.mmv = _mm_sqrt_ps(vLengthSq);
	
	SMVECTOR vZeroMask = SMVectorZero();
	
	vZeroMask.mmv = _mm_cmpneq_ps(vZeroMask, vResult);
	
	
	vLengthSq.mmv = _mm_cmpneq_ps(vLengthSq, maskInf);
	
	vResult.mmv = _mm_div_ps(V, vResult);
	
	vResult.mmv = _mm_and_ps(vResult, vZeroMask);
	
	SMVECTOR vTemp1;
	vTemp1.mmv = _mm_andnot_ps(vLengthSq, maskNaN);
	SMVECTOR vTemp2;
	vTemp2.mmv = _mm_and_ps(vResult, vLengthSq);
	vResult.mmv = _mm_or_ps(vTemp1, vTemp2);
	return(vResult);
}



__forceinline SMVECTOR SMVector3DotV(const float3 & V1, const float3 & V2)
{
	SMVECTOR vDot;
	vDot.mmv = _mm_mul_ps(V1, V2);

	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vDot, vDot, _MM_SHUFFLE(2, 1, 2, 1));

	vDot.mmv = _mm_add_ss(vDot, vTemp);

	vTemp.mmv = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));

	vDot.mmv = _mm_add_ss(vDot, vTemp);

	vDot.mmv = _mm_shuffle_ps(vDot, vDot, _MM_SHUFFLE(0, 0, 0, 0));

	return(vDot);
}

__forceinline float SMVector3Dot(const float3 & V1, const float3 & V2)
{
	return(SMVector3DotV(V1, V2).x);
}

__forceinline float SMVector3Dot(const float3 & V1)
{
	return(SMVector3DotV(V1, V1).x);
}


__forceinline float3 SMVector3Cross(const float3 & V1, const float3 & V2)
{
	static const SMVECTORI32 mask = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000};
	SMVECTOR vTemp1;
	vTemp1.mmv = _mm_shuffle_ps(V1, V1, _MM_SHUFFLE(3, 0, 2, 1));
	
	SMVECTOR vTemp2;
	vTemp2.mmv = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(3, 1, 0, 2));
	
	float3 vResult;
	vResult.mmv = _mm_mul_ps(vTemp1, vTemp2);
	
	vTemp1.mmv = _mm_shuffle_ps(vTemp1, vTemp1, _MM_SHUFFLE(3, 0, 2, 1));
	
	vTemp2.mmv = _mm_shuffle_ps(vTemp2, vTemp2, _MM_SHUFFLE(3, 1, 0, 2));
	
	vTemp1.mmv = _mm_mul_ps(vTemp1, vTemp2);
	
	vResult.mmv = _mm_sub_ps(vResult, vTemp1);
	
	vResult.mmv = _mm_and_ps(vResult, mask);
	return(vResult);
}

__forceinline float SMVector3Length(const float3 & V)
{
	SMVECTOR vLengthSq;
	vLengthSq.mmv = _mm_mul_ps(V, V);
	
	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
	
	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);
	
	vTemp.mmv = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	
	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);
	
	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	
	vLengthSq.mmv = _mm_sqrt_ps(vLengthSq);
	return(vLengthSq.x);
}

__forceinline float SMVector3Distance(const float3 & V1,const float3 & V2)
{
	return SMVector3Length(V1-V2);
}

__forceinline float3 SMVector3Normalize(const float3 & V)
{
	static const SMVECTORI32 maskInf = {0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000};
	static const SMVECTORI32 maskNaN = {0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000};
	SMVECTOR vLengthSq;
	vLengthSq.mmv = _mm_mul_ps(V, V);
	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);
	vTemp.mmv = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	
	float3 vResult;
	vResult.mmv = _mm_sqrt_ps(vLengthSq);
	
	SMVECTOR vZeroMask = SMVectorZero();
	
	vZeroMask.mmv = _mm_cmpneq_ps(vZeroMask, vResult);
	
	vLengthSq.mmv = _mm_cmpneq_ps(vLengthSq, maskInf);
	
	vResult.mmv = _mm_div_ps(V, vResult);
	
	vResult.mmv = _mm_and_ps(vResult, vZeroMask);
	
	SMVECTOR vTemp1;
	vTemp1.mmv = _mm_andnot_ps(vLengthSq, maskNaN);
	SMVECTOR vTemp2;
	vTemp2.mmv = _mm_and_ps(vResult, vLengthSq);
	vResult.mmv = _mm_or_ps(vTemp1, vTemp2);
	return vResult;
}

__forceinline float3 SMVector3Reflect(const float3 & V, const float3 & Normal)
{
	float fDot = SMVector3Dot(V, Normal);
	float3 Result = float4(fDot, fDot, fDot, fDot);
	Result.mmv = _mm_add_ps(Result, Result);
	Result.mmv = _mm_mul_ps(Result, Normal);
	Result.mmv = _mm_sub_ps(V, Result);
	return(Result);
}

__forceinline float3 SMVector3RefractV(const float3 & V, const float3 & Normal, const SMVECTOR & RefractionIndex)
{
	float fDotN = SMVector3Dot(V, Normal);
	SMVECTOR IDotN = float4(fDotN, fDotN, fDotN, fDotN);
	
	SMVECTOR R;
	R.mmv = _mm_mul_ps(IDotN, IDotN);
	R.mmv = _mm_sub_ps(float4(1.0f, 1.0f, 1.0f, 1.0f), R);
	R.mmv = _mm_mul_ps(R, RefractionIndex);
	R.mmv = _mm_mul_ps(R, RefractionIndex);
	R.mmv = _mm_sub_ps(float4(1.0f, 1.0f, 1.0f, 1.0f), R);

	float3 vResult;
	vResult.mmv = _mm_cmple_ps(R, float4(0.0f, 0.0f, 0.0f, 0.0f));
	if(_mm_movemask_ps(vResult) == 0x0f)
	{
		vResult = float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		R.mmv = _mm_sqrt_ps(R);
		vResult.mmv = _mm_mul_ps(RefractionIndex, IDotN);
		R.mmv = _mm_add_ps(R, vResult);
		
		vResult.mmv = _mm_mul_ps(RefractionIndex, V);
		R.mmv = _mm_mul_ps(R, Normal);
		vResult.mmv = _mm_sub_ps(vResult, R);
	}
	return(vResult);
}

__forceinline float3 SMVector3Refract(const float3 & V, const float3 & Normal, float RefractionIndex)
{
	SMVECTOR Index;
	Index.mmv = _mm_set_ps1(RefractionIndex);
	return(SMVector3RefractV(V, Normal, Index));
}

__forceinline float3 SMVector3Transform(const float3 & V, const SMMATRIX & M)
{
	float3 vResult;
	vResult.mmv = _mm_shuffle_ps(V, V, _MM_SHUFFLE(0, 0, 0, 0));
	vResult.mmv = _mm_mul_ps(vResult, M.r[0]);
	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(V, V, _MM_SHUFFLE(1, 1, 1, 1));
	vTemp.mmv = _mm_mul_ps(vTemp, M.r[1]);
	vResult.mmv = _mm_add_ps(vResult, vTemp);
	vTemp.mmv = _mm_shuffle_ps(V, V, _MM_SHUFFLE(2, 2, 2, 2));
	vTemp.mmv = _mm_mul_ps(vTemp, M.r[2]);
	vResult.mmv = _mm_add_ps(vResult, vTemp);
	vResult.mmv = _mm_add_ps(vResult, M.r[3]);
	return(vResult);
}


__forceinline SMVECTOR SMVector4DotV(const float4 & V1, const float4 & V2)
{
	SMVECTOR vTemp2;
	vTemp2.mmv = V2;
	SMVECTOR vTemp;
	vTemp.mmv = _mm_mul_ps(V1, vTemp2);
	vTemp2.mmv = _mm_shuffle_ps(vTemp2, vTemp, _MM_SHUFFLE(1, 0, 0, 0));
	vTemp2.mmv = _mm_add_ps(vTemp2, vTemp);
	vTemp.mmv = _mm_shuffle_ps(vTemp, vTemp2, _MM_SHUFFLE(0, 3, 0, 0));
	vTemp.mmv = _mm_add_ps(vTemp, vTemp2);
	vTemp.mmv = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(2, 2, 2, 2));
	return(vTemp);
}

__forceinline float SMVector4Dot(const float4 & V1, const float4 & V2)
{
	return(SMVector4DotV(V1, V2).x);
}

__forceinline float4 SMVector4Cross(const float4 & V1, const float4 & V2, const float4 & V3)
{
	// V2zwyz * V3wzwy
	SMVECTOR vResult;
	vResult.mmv = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(2, 1, 3, 2));
	SMVECTOR vTemp3;
	vTemp3.mmv = _mm_shuffle_ps(V3, V3, _MM_SHUFFLE(1, 3, 2, 3));
	vResult;
	vResult.mmv = _mm_mul_ps(vResult, vTemp3);
	// - V2wzwy * V3zwyz
	SMVECTOR vTemp2;
	vTemp2.mmv = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(1, 3, 2, 3));
	vTemp3.mmv = _mm_shuffle_ps(vTemp3, vTemp3, _MM_SHUFFLE(1, 3, 0, 1));
	vTemp2.mmv = _mm_mul_ps(vTemp2, vTemp3);
	vResult.mmv = _mm_sub_ps(vResult, vTemp2);
	// term1 * V1yxxx
	SMVECTOR vTemp1;
	vTemp1.mmv = _mm_shuffle_ps(V1, V1, _MM_SHUFFLE(0, 0, 0, 1));
	vResult.mmv = _mm_mul_ps(vResult, vTemp1);

	// V2ywxz * V3wxwx
	vTemp2.mmv = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(2, 0, 3, 1));
	vTemp3.mmv = _mm_shuffle_ps(V3, V3, _MM_SHUFFLE(0, 3, 0, 3));
	vTemp3.mmv = _mm_mul_ps(vTemp3, vTemp2);
	// - V2wxwx * V3ywxz
	vTemp2.mmv = _mm_shuffle_ps(vTemp2, vTemp2, _MM_SHUFFLE(2, 1, 2, 1));
	vTemp1.mmv = _mm_shuffle_ps(V3, V3, _MM_SHUFFLE(2, 0, 3, 1));
	vTemp2.mmv = _mm_mul_ps(vTemp2, vTemp1);
	vTemp3.mmv = _mm_sub_ps(vTemp3, vTemp2);
	// vResult - temp * V1zzyy
	vTemp1.mmv = _mm_shuffle_ps(V1, V1, _MM_SHUFFLE(1, 1, 2, 2));
	vTemp1.mmv = _mm_mul_ps(vTemp1, vTemp3);
	vResult.mmv = _mm_sub_ps(vResult, vTemp1);

	// V2yzxy * V3zxyx
	vTemp2.mmv = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(1, 0, 2, 1));
	vTemp3.mmv = _mm_shuffle_ps(V3, V3, _MM_SHUFFLE(0, 1, 0, 2));
	vTemp3.mmv = _mm_mul_ps(vTemp3, vTemp2);
	// - V2zxyx * V3yzxy
	vTemp2.mmv = _mm_shuffle_ps(vTemp2, vTemp2, _MM_SHUFFLE(2, 0, 2, 1));
	vTemp1.mmv = _mm_shuffle_ps(V3, V3, _MM_SHUFFLE(1, 0, 2, 1));
	vTemp1.mmv = _mm_mul_ps(vTemp1, vTemp2);
	vTemp3.mmv = _mm_sub_ps(vTemp3, vTemp1);
	// vResult + term * V1wwwz
	vTemp1.mmv = _mm_shuffle_ps(V1, V1, _MM_SHUFFLE(2, 3, 3, 3));
	vTemp3.mmv = _mm_mul_ps(vTemp3, vTemp1);
	vResult.mmv = _mm_add_ps(vResult, vTemp3);
	return(vResult);
}

__forceinline float SMVector4Length(const float4 & V)
{
	SMVECTOR vLengthSq;
	vLengthSq.mmv = _mm_mul_ps(V, V);
	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
	vLengthSq.mmv = _mm_add_ps(vLengthSq, vTemp);
	
	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
	
	vTemp.mmv = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
	
	vLengthSq.mmv = _mm_add_ps(vLengthSq, vTemp);
	
	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
	
	vLengthSq.mmv = _mm_sqrt_ps(vLengthSq);
	return(vLengthSq.x);
}

__forceinline float4 SMVector4Normalize(const float4 & V)
{
	static const SMVECTORI32 maskInf = {0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000};
	static const SMVECTORI32 maskNaN = {0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000};

	SMVECTOR vLengthSq;
	vLengthSq.mmv = _mm_mul_ps(V, V);
	
	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(3, 2, 3, 2));
	
	vLengthSq.mmv = _mm_add_ps(vLengthSq, vTemp);
	
	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 0, 0, 0));
	
	vTemp.mmv = _mm_shuffle_ps(vTemp, vLengthSq, _MM_SHUFFLE(3, 3, 0, 0));
	
	vLengthSq.mmv = _mm_add_ps(vLengthSq, vTemp);
	
	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(2, 2, 2, 2));
	
	SMVECTOR vResult;
	vResult.mmv = _mm_sqrt_ps(vLengthSq);
	
	SMVECTOR vZeroMask = SMVectorZero();
	
	vZeroMask.mmv = _mm_cmpneq_ps(vZeroMask, vResult);
	
	
	vLengthSq.mmv = _mm_cmpneq_ps(vLengthSq, maskInf);
	
	vResult.mmv = _mm_div_ps(V, vResult);
	
	vResult.mmv = _mm_and_ps(vResult, vZeroMask);
	
	SMVECTOR vTemp1;
	vTemp1.mmv = _mm_andnot_ps(vLengthSq, maskNaN);
	SMVECTOR vTemp2;
	vTemp2.mmv = _mm_and_ps(vResult, vLengthSq);
	vResult.mmv = _mm_or_ps(vTemp1, vTemp2);
	return(vResult);
}

__forceinline float4 SMVector4Reflect(const float4 & V, const float4 & Normal)
{
	float fDot = SMVector4Dot(V, Normal);
	float4 Result = float4(fDot, fDot, fDot, fDot);
	Result.mmv = _mm_add_ps(Result, Result);
	Result.mmv = _mm_mul_ps(Result, Normal);
	Result.mmv = _mm_sub_ps(V, Result);
	return(Result);
}

__forceinline float4 SMVector4RefractV(const float4 & V, const float4 & Normal, const SMVECTOR & RefractionIndex)
{
	float fDot = SMVector4Dot(V, Normal);
	float4 IDotN = float4(fDot, fDot, fDot, fDot);

	// R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
	SMVECTOR R;
	R.mmv= _mm_mul_ps(IDotN, IDotN);
	R.mmv = _mm_sub_ps(float4(1.0f, 1.0f, 1.0f, 1.0f), R);
	R.mmv = _mm_mul_ps(R, RefractionIndex);
	R.mmv = _mm_mul_ps(R, RefractionIndex);
	R.mmv = _mm_sub_ps(float4(1.0f, 1.0f, 1.0f, 1.0f), R);

	SMVECTOR vResult;
	vResult.mmv = _mm_cmple_ps(R, float4(0.0f, 0.0f, 0.0f, 0.0f));
	if(_mm_movemask_ps(vResult) == 0x0f)
	{
		// Total internal reflection
		vResult = float4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
		// R = RefractionIndex * IDotN + sqrt(R)
		R.mmv = _mm_sqrt_ps(R);
		vResult.mmv = _mm_mul_ps(RefractionIndex, IDotN);
		R.mmv = _mm_add_ps(R, vResult);
		// Result = RefractionIndex * Incident - Normal * R
		vResult.mmv = _mm_mul_ps(RefractionIndex, V);
		R.mmv = _mm_mul_ps(R, Normal);
		vResult.mmv = _mm_sub_ps(vResult, R);
	}
	return(vResult);
}

__forceinline float4 SMVector4Refract(const float4 & V, const float4 & Normal, float RefractionIndex)
{
	SMVECTOR Index;
	Index.mmv = _mm_set_ps1(RefractionIndex);
	return(SMVector4RefractV(V, Normal, Index));
}

__forceinline float4 SMVector4Transform(const float4 & V, const SMMATRIX & M)
{
	SMVECTOR vTempX;
	vTempX.mmv = _mm_shuffle_ps(V, V, _MM_SHUFFLE(0, 0, 0, 0));
	SMVECTOR vTempY;
	vTempY.mmv = _mm_shuffle_ps(V, V, _MM_SHUFFLE(1, 1, 1, 1));
	SMVECTOR vTempZ;
	vTempZ.mmv = _mm_shuffle_ps(V, V, _MM_SHUFFLE(2, 2, 2, 2));
	SMVECTOR vTempW;
	vTempW.mmv = _mm_shuffle_ps(V, V, _MM_SHUFFLE(3, 3, 3, 3));
	// Mul by the matrix
	vTempX.mmv = _mm_mul_ps(vTempX, M.r[0]);
	vTempY.mmv = _mm_mul_ps(vTempY, M.r[1]);
	vTempZ.mmv = _mm_mul_ps(vTempZ, M.r[2]);
	vTempW.mmv = _mm_mul_ps(vTempW, M.r[3]);
	// Add them all together
	vTempX.mmv = _mm_add_ps(vTempX, vTempY);
	vTempZ.mmv = _mm_add_ps(vTempZ, vTempW);
	vTempX.mmv = _mm_add_ps(vTempX, vTempZ);
	return(vTempX);
}



__forceinline SMMATRIX SMMatrixTranspose(const SMMATRIX & M)
{
	// x.x,x.y,y.x,y.y
	SMVECTOR vTemp1;
	vTemp1.mmv = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
	// x.z,x.w,y.z,y.w
	SMVECTOR vTemp3;
	vTemp3.mmv = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
	// z.x,z.y,w.x,w.y
	SMVECTOR vTemp2;
	vTemp2.mmv = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
	// z.z,z.w,w.z,w.w
	SMVECTOR vTemp4;
	vTemp4.mmv = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
	SMMATRIX mResult;

	// x.x,y.x,z.x,w.x
	mResult.r[0].mmv = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
	// x.y,y.y,z.y,w.y
	mResult.r[1].mmv = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
	// x.z,y.z,z.z,w.z
	mResult.r[2].mmv = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
	// x.w,y.w,z.w,w.w
	mResult.r[3].mmv = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
	return(mResult);
}

__forceinline SMMATRIX SMMatrixMultiply(const SMMATRIX & M1, const SMMATRIX & M2)
{
	SMMATRIX mResult;
	// Use vW to hold the original row
	SMVECTOR vW;
	vW = M1.r[0];
	// Splat the component X,Y,Z then W
	SMVECTOR vX;
	vX.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
	SMVECTOR vY;
	vY.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
	SMVECTOR vZ;
	vZ.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
	// Perform the opertion on the first row
	vX.mmv = _mm_mul_ps(vX, M2.r[0]);
	vY.mmv = _mm_mul_ps(vY, M2.r[1]);
	vZ.mmv = _mm_mul_ps(vZ, M2.r[2]);
	vW.mmv = _mm_mul_ps(vW, M2.r[3]);
	// Perform a binary add to reduce cumulative errors
	vX.mmv = _mm_add_ps(vX, vZ);
	vY.mmv = _mm_add_ps(vY, vW);
	vX.mmv = _mm_add_ps(vX, vY);
	mResult.r[0] = vX;
	// Repeat for the other 3 rows
	vW = M1.r[1];
	vX.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX.mmv = _mm_mul_ps(vX, M2.r[0]);
	vY.mmv = _mm_mul_ps(vY, M2.r[1]);
	vZ.mmv = _mm_mul_ps(vZ, M2.r[2]);
	vW.mmv = _mm_mul_ps(vW, M2.r[3]);
	vX.mmv = _mm_add_ps(vX, vZ);
	vY.mmv = _mm_add_ps(vY, vW);
	vX.mmv = _mm_add_ps(vX, vY);
	mResult.r[1] = vX;
	vW = M1.r[2];
	vX.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX.mmv = _mm_mul_ps(vX, M2.r[0]);
	vY.mmv = _mm_mul_ps(vY, M2.r[1]);
	vZ.mmv = _mm_mul_ps(vZ, M2.r[2]);
	vW.mmv = _mm_mul_ps(vW, M2.r[3]);
	vX.mmv = _mm_add_ps(vX, vZ);
	vY.mmv = _mm_add_ps(vY, vW);
	vX.mmv = _mm_add_ps(vX, vY);
	mResult.r[2] = vX;
	vW = M1.r[3];
	vX.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW.mmv = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX.mmv = _mm_mul_ps(vX, M2.r[0]);
	vY.mmv = _mm_mul_ps(vY, M2.r[1]);
	vZ.mmv = _mm_mul_ps(vZ, M2.r[2]);
	vW.mmv = _mm_mul_ps(vW, M2.r[3]);
	vX.mmv = _mm_add_ps(vX, vZ);
	vY.mmv = _mm_add_ps(vY, vW);
	vX.mmv = _mm_add_ps(vX, vY);
	mResult.r[3] = vX;
	return(mResult);
}

__forceinline SMMATRIX SMMatrixIdentity()
{
	return(SMMATRIX());
}

__forceinline SMMATRIX SMMatrixTranslation(float x, float y, float z)
{
	SMMATRIX M;
	M.r[3] = float4(x, y, z, 1.0f);
	return(M);
}

__forceinline SMMATRIX SMMatrixTranslation(const float3 & V)
{
	SMMATRIX M;
	M.r[3] = float4(V, 1.0f);
	return(M);
}

__forceinline SMMATRIX SMMatrixScaling(float x, float y, float z)
{
	SMMATRIX M;
	M.r[0] = float4(x, 0.0f, 0.0f, 0.0f);
	M.r[1] = float4(0.0f, y, 0.0f, 0.0f);
	M.r[2] = float4(0.0f, 0.0f, z, 0.0f);
	return(M);
}

__forceinline SMMATRIX SMMatrixScaling(const float3 & V)
{
	return(SMMatrixScaling(V.x, V.y, V.z));
}

__forceinline SMMATRIX & SMMATRIX::operator *= (const SMMATRIX & M)
{
	*this = SMMatrixMultiply(*this, M);
	return(*this);
}

__forceinline SMMATRIX SMMATRIX::operator*(const SMMATRIX & M)
{
	return(SMMatrixMultiply(*this, M));
}

__forceinline SMMATRIX SMMatrixRotationX(float Angle)
{
	float SinAngle = sinf(Angle);
	float CosAngle = cosf(Angle);

	SMVECTOR vSin;
	vSin.mmv = _mm_set_ss(SinAngle);
	SMVECTOR vCos;
	vCos.mmv = _mm_set_ss(CosAngle);
	// x = 0,y = cos,z = sin, w = 0
	vCos.mmv = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
	SMMATRIX M;
	M.r[1] = vCos;
	// x = 0,y = sin,z = cos, w = 0
	vCos.mmv = _mm_shuffle_ps(vCos, vCos, _MM_SHUFFLE(3, 1, 2, 0));
	// x = 0,y = -sin,z = cos, w = 0
	vCos.mmv = _mm_mul_ps(vCos, float4(1.0f, -1.0f, 1.0f, 1.0f));
	M.r[2] = vCos;
	return(M);
}

__forceinline SMMATRIX SMMatrixRotationY(float Angle)
{
	float SinAngle = sinf(Angle);
	float CosAngle = cosf(Angle);

	SMVECTOR vSin;
	vSin.mmv = _mm_set_ss(SinAngle);
	SMVECTOR vCos;
	vCos.mmv = _mm_set_ss(CosAngle);
	// x = sin,y = 0,z = cos, w = 0
	vSin.mmv = _mm_shuffle_ps(vSin, vCos, _MM_SHUFFLE(3, 0, 3, 0));
	SMMATRIX M;
	M.r[2] = vSin;
	// x = cos,y = 0,z = sin, w = 0
	vSin.mmv = _mm_shuffle_ps(vSin, vSin, _MM_SHUFFLE(3, 0, 1, 2));
	// x = cos,y = 0,z = -sin, w = 0
	vSin.mmv = _mm_mul_ps(vSin, float4(1.0f, 1.0f, -1.0f, 1.0f));
	M.r[0] = vSin;
	return(M);
}

__forceinline SMMATRIX SMMatrixRotationZ(float Angle)
{
	float SinAngle = sinf(Angle);
	float CosAngle = cosf(Angle);

	SMVECTOR vSin;
	vSin.mmv = _mm_set_ss(SinAngle);
	SMVECTOR vCos;
	vCos.mmv = _mm_set_ss(CosAngle);
	// x = cos,y = sin,z = 0, w = 0
	vCos.mmv = _mm_unpacklo_ps(vCos, vSin);
	SMMATRIX M;
	M.r[0] = vCos;
	// x = sin,y = cos,z = 0, w = 0
	vCos.mmv = _mm_shuffle_ps(vCos, vCos, _MM_SHUFFLE(3, 2, 0, 1));
	// x = cos,y = -sin,z = 0, w = 0
	vCos.mmv = _mm_mul_ps(vCos, float4(-1.0f, 1.0f, 1.0f, 1.0f));
	M.r[1] = vCos;
	return(M);
}

__forceinline SMMATRIX SMMatrixRotationNormal(const float3 & NormalAxis, float Angle)
{
	SMVECTOR N0, N1;
	SMVECTOR V0, V1, V2;
	SMVECTOR R0, R1, R2;
	SMVECTOR C0, C1, C2;
	static const SMVECTORI32 mask = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000};
	SMMATRIX M;

	float fSinAngle = sinf(Angle);
	float fCosAngle = cosf(Angle);

	C2.mmv = _mm_set_ps1(1.0f - fCosAngle);
	C1.mmv = _mm_set_ps1(fCosAngle);
	C0.mmv = _mm_set_ps1(fSinAngle);

	N0.mmv = _mm_shuffle_ps(NormalAxis, NormalAxis, _MM_SHUFFLE(3, 0, 2, 1));
	N1.mmv = _mm_shuffle_ps(NormalAxis, NormalAxis, _MM_SHUFFLE(3, 1, 0, 2));

	V0.mmv = _mm_mul_ps(C2, N0);
	V0.mmv = _mm_mul_ps(V0, N1);

	R0.mmv = _mm_mul_ps(C2, NormalAxis);
	R0.mmv = _mm_mul_ps(R0, NormalAxis);
	R0.mmv = _mm_add_ps(R0, C1);

	R1.mmv = _mm_mul_ps(C0, NormalAxis);
	R1.mmv = _mm_add_ps(R1, V0);
	R2.mmv = _mm_mul_ps(C0, NormalAxis);
	R2.mmv = _mm_sub_ps(V0, R2);

	V0.mmv = _mm_and_ps(R0, mask);
	V1.mmv = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
	V1.mmv = _mm_shuffle_ps(V1, V1, _MM_SHUFFLE(0, 3, 2, 1));
	V2.mmv = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
	V2.mmv = _mm_shuffle_ps(V2, V2, _MM_SHUFFLE(2, 0, 2, 0));

	R2.mmv = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
	R2.mmv = _mm_shuffle_ps(R2, R2, _MM_SHUFFLE(1, 3, 2, 0));
	M.r[0] = R2;
	R2.mmv = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
	R2.mmv = _mm_shuffle_ps(R2, R2, _MM_SHUFFLE(1, 3, 0, 2));
	M.r[1] = R2;
	V2.mmv = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
	M.r[2] = V2;
	return(M);
}

__forceinline SMMATRIX SMMatrixRotationAxis(const float3 & Axis, float Angle)
{
	float3 normal = SMVector3Normalize(Axis);
	return(SMMatrixRotationNormal(normal, Angle));
}

__forceinline SMMATRIX SMMatrixLookToLH(const float3 & EyePosition, const float3 & EyeDirection, const float3 & UpDirection)
{
	static const SMVECTORI32 mask3 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000};
	static const SMVECTORI32 maskW = {0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF};
	SMMATRIX M;
	SMVECTOR R2 = SMVector3Normalize(EyeDirection);
	SMVECTOR R0 = SMVector3Cross(UpDirection, R2);
	R0 = SMVector3Normalize(R0);
	SMVECTOR R1 = SMVector3Cross(R2, R0);
	SMVECTOR NegEyePosition;
	NegEyePosition.mmv = _mm_mul_ps(EyePosition, float4(-1.0f, -1.0f, -1.0f, -1.0f));
	SMVECTOR D0 = SMVector3DotV(R0, NegEyePosition);
	SMVECTOR D1 = SMVector3DotV(R1, NegEyePosition);
	SMVECTOR D2 = SMVector3DotV(R2, NegEyePosition);
	R0.mmv = _mm_and_ps(R0, mask3);
	R1.mmv = _mm_and_ps(R1, mask3);
	R2.mmv = _mm_and_ps(R2, mask3);
	D0.mmv = _mm_and_ps(D0, maskW);
	D1.mmv = _mm_and_ps(D1, maskW);
	D2.mmv = _mm_and_ps(D2, maskW);
	D0.mmv = _mm_or_ps(D0, R0);
	D1.mmv = _mm_or_ps(D1, R1);
	D2.mmv = _mm_or_ps(D2, R2);
	M.r[0] = D0;
	M.r[1] = D1;
	M.r[2] = D2;
	M = SMMatrixTranspose(M);
	return(M);
}

__forceinline SMMATRIX SMMatrixLookAtLH(const float3 & EyePosition, const float3 & FocusPosition, const float3 & UpDirection)
{
	return(SMMatrixLookToLH(EyePosition, FocusPosition - EyePosition, UpDirection));
}

__forceinline SMMATRIX SMMatrixPerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
{
	SMVECTORI32 maskY = {0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000};
	SMMATRIX M;
	float TwoNearZ = NearZ + NearZ;
	float fRange = FarZ / (FarZ - NearZ);
	
	SMVECTOR rMem = {
		TwoNearZ / ViewWidth,
		TwoNearZ / ViewHeight,
		fRange,
		-fRange * NearZ
	};
	
	SMVECTOR vValues = rMem;
	SMVECTOR vTemp = SMVectorZero();
	
	vTemp.mmv = _mm_move_ss(vTemp, vValues);
	
	M.r[0] = vTemp;
	
	vTemp = vValues;
	vTemp.mmv = _mm_and_ps(vTemp, maskY);
	M.r[1] = vTemp;
	
	vValues.mmv = _mm_shuffle_ps(vValues, float4(0.0f, 0.0f, 0.0f, 1.0f), _MM_SHUFFLE(3, 2, 3, 2));
	
	vTemp.mmv = _mm_setzero_ps();
	vTemp.mmv = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
	M.r[2] = vTemp;
	
	vTemp.mmv = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
	M.r[3] = vTemp;

	return(M);
}

__forceinline SMMATRIX SMMatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
{
	SMVECTORI32 maskY = {0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000};
	SMMATRIX M;
	float SinFov = sinf(0.5f * FovAngleY);
	float CosFov = cosf(0.5f * FovAngleY);
	
	float fRange = FarZ / (FarZ - NearZ);
	
	float Height = CosFov / SinFov;
	SMVECTOR rMem = {
		Height / AspectRatio,
		Height,
		fRange,
		-fRange * NearZ
	};
	
	SMVECTOR vValues = rMem;
	SMVECTOR vTemp = SMVectorZero();
	
	vTemp.mmv = _mm_move_ss(vTemp, vValues);
	
	M.r[0] = vTemp;
	
	vTemp = vValues;
	vTemp.mmv = _mm_and_ps(vTemp, maskY);
	M.r[1] = vTemp;
	
	vTemp = SMVectorZero();
	vValues.mmv = _mm_shuffle_ps(vValues, float4(0.0f, 0.0f, 0.0f, 1.0f), _MM_SHUFFLE(3, 2, 3, 2));
	
	vTemp.mmv = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
	M.r[2] = vTemp;
	
	vTemp.mmv = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
	M.r[3] = vTemp;
	return(M);
}

__forceinline SMMATRIX SMMatrixOrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ)
{
	SMVECTORI32 maskY = {0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000};
	SMMATRIX M;
	float fRange = 1.0f / (FarZ - NearZ);
	
	SMVECTOR rMem = {
		2.0f / ViewWidth,
		2.0f / ViewHeight,
		fRange,
		-fRange * NearZ
	};
	
	SMVECTOR vValues = rMem;
	SMVECTOR vTemp = SMVectorZero();
	
	vTemp.mmv = _mm_move_ss(vTemp, vValues);
	
	M.r[0] = vTemp;
	
	vTemp = vValues;
	vTemp.mmv = _mm_and_ps(vTemp, maskY);
	M.r[1] = vTemp;
	
	vTemp = SMVectorZero();
	vValues.mmv = _mm_shuffle_ps(vValues, float4(0.0f, 0.0f, 0.0f, 1.0f), _MM_SHUFFLE(3, 2, 3, 2));
	
	vTemp.mmv = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
	M.r[2] = vTemp;
	
	vTemp.mmv = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
	M.r[3] = vTemp;
	return(M);
}

__forceinline SMMATRIX SMMatrixInverse(float * pDeterminant, const SMMATRIX & M)
{
	SMMATRIX MT = SMMatrixTranspose(M);
	SMVECTOR V00;
	V00.mmv = _mm_shuffle_ps(MT.r[2], MT.r[2], _MM_SHUFFLE(1, 1, 0, 0));
	SMVECTOR V10;
	V10.mmv = _mm_shuffle_ps(MT.r[3], MT.r[3], _MM_SHUFFLE(3, 2, 3, 2));
	SMVECTOR V01;
	V01.mmv = _mm_shuffle_ps(MT.r[0], MT.r[0], _MM_SHUFFLE(1, 1, 0, 0));
	SMVECTOR V11;
	V11.mmv = _mm_shuffle_ps(MT.r[1], MT.r[1], _MM_SHUFFLE(3, 2, 3, 2));
	SMVECTOR V02; 
	V02.mmv = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(2, 0, 2, 0));
	SMVECTOR V12;
	V12.mmv = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(3, 1, 3, 1));

	SMVECTOR D0;
	D0.mmv = _mm_mul_ps(V00, V10);
	SMVECTOR D1;
	D1.mmv = _mm_mul_ps(V01, V11);
	SMVECTOR D2;
	D2.mmv = _mm_mul_ps(V02, V12);

	V00.mmv = _mm_shuffle_ps(MT.r[2], MT.r[2], _MM_SHUFFLE(3, 2, 3, 2));
	V10.mmv = _mm_shuffle_ps(MT.r[3], MT.r[3], _MM_SHUFFLE(1, 1, 0, 0));
	V01.mmv = _mm_shuffle_ps(MT.r[0], MT.r[0], _MM_SHUFFLE(3, 2, 3, 2));
	V11.mmv = _mm_shuffle_ps(MT.r[1], MT.r[1], _MM_SHUFFLE(1, 1, 0, 0));
	V02.mmv = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(3, 1, 3, 1));
	V12.mmv = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(2, 0, 2, 0));

	V00.mmv = _mm_mul_ps(V00, V10);
	V01.mmv = _mm_mul_ps(V01, V11);
	V02.mmv = _mm_mul_ps(V02, V12);
	D0.mmv = _mm_sub_ps(D0, V00);
	D1.mmv = _mm_sub_ps(D1, V01);
	D2.mmv = _mm_sub_ps(D2, V02);
	
	V11.mmv = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
	V00.mmv = _mm_shuffle_ps(MT.r[1], MT.r[1], _MM_SHUFFLE(1, 0, 2, 1));
	V10.mmv = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
	V01.mmv = _mm_shuffle_ps(MT.r[0], MT.r[0], _MM_SHUFFLE(0, 1, 0, 2));
	V11.mmv = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
	
	SMVECTOR V13;
	V13.mmv = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
	V02.mmv = _mm_shuffle_ps(MT.r[3], MT.r[3], _MM_SHUFFLE(1, 0, 2, 1));
	V12.mmv = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
	SMVECTOR V03;
	V03.mmv = _mm_shuffle_ps(MT.r[2], MT.r[2], _MM_SHUFFLE(0, 1, 0, 2));
	V13.mmv = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

	SMVECTOR C0;
	C0.mmv = _mm_mul_ps(V00, V10);
	SMVECTOR C2;
	C2.mmv = _mm_mul_ps(V01, V11);
	SMVECTOR C4;
	C4.mmv = _mm_mul_ps(V02, V12);
	SMVECTOR C6;
	C6.mmv = _mm_mul_ps(V03, V13);


	V11.mmv = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
	V00.mmv = _mm_shuffle_ps(MT.r[1], MT.r[1], _MM_SHUFFLE(2, 1, 3, 2));
	V10.mmv = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
	V01.mmv = _mm_shuffle_ps(MT.r[0], MT.r[0], _MM_SHUFFLE(1, 3, 2, 3));
	V11.mmv = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
	
	V13.mmv = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
	V02.mmv = _mm_shuffle_ps(MT.r[3], MT.r[3], _MM_SHUFFLE(2, 1, 3, 2));
	V12.mmv = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
	V03.mmv = _mm_shuffle_ps(MT.r[2], MT.r[2], _MM_SHUFFLE(1, 3, 2, 3));
	V13.mmv = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

	V00.mmv = _mm_mul_ps(V00, V10);
	V01.mmv = _mm_mul_ps(V01, V11);
	V02.mmv = _mm_mul_ps(V02, V12);
	V03.mmv = _mm_mul_ps(V03, V13);
	C0.mmv = _mm_sub_ps(C0, V00);
	C2.mmv = _mm_sub_ps(C2, V01);
	C4.mmv = _mm_sub_ps(C4, V02);
	C6.mmv = _mm_sub_ps(C6, V03);

	V00.mmv = _mm_shuffle_ps(MT.r[1], MT.r[1], _MM_SHUFFLE(0, 3, 0, 3));
	
	V10.mmv = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
	V10.mmv = _mm_shuffle_ps(V10, V10, _MM_SHUFFLE(0, 2, 3, 0));
	V01.mmv = _mm_shuffle_ps(MT.r[0], MT.r[0], _MM_SHUFFLE(2, 0, 3, 1));
	
	V11.mmv = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
	V11.mmv = _mm_shuffle_ps(V11, V11, _MM_SHUFFLE(2, 1, 0, 3));
	V02.mmv = _mm_shuffle_ps(MT.r[3], MT.r[3], _MM_SHUFFLE(0, 3, 0, 3));
	
	V12.mmv = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
	V12.mmv = _mm_shuffle_ps(V12, V12, _MM_SHUFFLE(0, 2, 3, 0));
	V03.mmv = _mm_shuffle_ps(MT.r[2], MT.r[2], _MM_SHUFFLE(2, 0, 3, 1));
	
	V13.mmv = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
	V13.mmv = _mm_shuffle_ps(V13, V13, _MM_SHUFFLE(2, 1, 0, 3));

	V00.mmv = _mm_mul_ps(V00, V10);
	V01.mmv = _mm_mul_ps(V01, V11);
	V02.mmv = _mm_mul_ps(V02, V12);
	V03.mmv = _mm_mul_ps(V03, V13);
	SMVECTOR C1;
	C1.mmv = _mm_sub_ps(C0, V00);
	C0.mmv = _mm_add_ps(C0, V00);
	SMVECTOR C3;
	C3.mmv = _mm_add_ps(C2, V01);
	C2.mmv = _mm_sub_ps(C2, V01);
	SMVECTOR C5;
	C5.mmv = _mm_sub_ps(C4, V02);
	C4.mmv = _mm_add_ps(C4, V02);
	SMVECTOR C7;
	C7.mmv = _mm_add_ps(C6, V03);
	C6.mmv = _mm_sub_ps(C6, V03);

	C0.mmv = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
	C2.mmv = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
	C4.mmv = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
	C6.mmv = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
	C0.mmv = _mm_shuffle_ps(C0, C0, _MM_SHUFFLE(3, 1, 2, 0));
	C2.mmv = _mm_shuffle_ps(C2, C2, _MM_SHUFFLE(3, 1, 2, 0));
	C4.mmv = _mm_shuffle_ps(C4, C4, _MM_SHUFFLE(3, 1, 2, 0));
	C6.mmv = _mm_shuffle_ps(C6, C6, _MM_SHUFFLE(3, 1, 2, 0));
	
	SMVECTOR vTemp = SMVector4DotV(C0, MT.r[0]);
	*pDeterminant = vTemp.x;
	vTemp.mmv = _mm_div_ps(float4(1.0f, 1.0f, 1.0f, 1.0f), vTemp);
	SMMATRIX mResult;
	mResult.r[0].mmv = _mm_mul_ps(C0, vTemp);
	mResult.r[1].mmv = _mm_mul_ps(C2, vTemp);
	mResult.r[2].mmv = _mm_mul_ps(C4, vTemp);
	mResult.r[3].mmv = _mm_mul_ps(C6, vTemp);
	return(mResult);
}

__forceinline float3 operator*(const float3 & V, const SMMATRIX & M)
{
	return(SMVector3Transform(V, M));
}

__forceinline float3 operator*(const SMMATRIX & M, const float3 & V)
{
	return(SMVector3Transform(V, M));
}

__forceinline float4 operator*(const float4 & V, const SMMATRIX & M)
{
	return(SMVector4Transform(V, M));
}

__forceinline float4 operator*(const SMMATRIX & M, const float4 & V)
{
	return(SMVector4Transform(V, M));
}

class SMQuaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	SMQuaternion::SMQuaternion(const float3 & f)
	{
		x = f.x;
		y = f.y;
		z = f.z;
		Renormalize();
	}

	SMQuaternion::SMQuaternion(const float4 & f)
	{
		x = f.x;
		y = f.y;
		z = f.z;
		w = f.w;
	}

	SMQuaternion::SMQuaternion(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	SMQuaternion::SMQuaternion(float angle, char ax)
	{
		float sin_a = sin(angle / 2);
		float cos_a = cos(angle / 2);
		x = y = z = w = 0.0f;
		switch(ax)
		{
		case 'x':
			x = sin_a;
			break;
		case 'y':
			y = sin_a;
			break;
		case 'z':
			z = sin_a;
			break;
		}
		w = cos_a;
		SMQuaternion q = Normalize();
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}

	SMQuaternion::SMQuaternion():x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{
	}

	SMQuaternion Normalize();
	SMQuaternion Inverse();
	float Length();
	SMQuaternion Renormalize();

	SMMATRIX GetMatrix() const;

	SMQuaternion Conjugate() const;

	static float InnerProduct(const SMQuaternion & q1, const SMQuaternion & q2);

	SMQuaternion(const float3 & v, float angle)
	{
		float3 axis = SMVector3Normalize(v);
		float half_angle = angle * 0.5f;
		float sin_a = sinf(half_angle);
		x = v.x * sin_a;
		y = v.y * sin_a;
		z = v.z * sin_a;
		w = cosf(half_angle);
	}

	SMQuaternion(const SMMATRIX & mm)
	{
		float m[4][4] = {
			{mm._11, mm._12, mm._13, mm._14},
			{mm._21, mm._22, mm._23, mm._24},
			{mm._31, mm._32, mm._33, mm._34},
			{mm._41, mm._42, mm._43, mm._44}
		};
		{
			float  tr, s, q[4];
			int    i, j, k;

			int nxt[3] = {1, 2, 0};

			tr = m[0][0] + m[1][1] + m[2][2];

			if(tr > 0.0)
			{
				s = sqrt(tr + 1.0);
				w = s / 2.0;
				s = 0.5 / s;
				x = (m[1][2] - m[2][1]) * s;
				y = (m[2][0] - m[0][2]) * s;
				z = (m[0][1] - m[1][0]) * s;
			}
			else
			{
				i = 0;
				if(m[1][1] > m[0][0]) i = 1;
				if(m[2][2] > m[i][i]) i = 2;
				j = nxt[i];
				k = nxt[j];

				s = sqrt((m[i][i] - (m[j][j] + m[k][k])) + 1.0);

				q[i] = s * 0.5;

				if(s != 0.0) s = 0.5 / s;

				q[3] = (m[j][k] - m[k][j]) * s;
				q[j] = (m[i][j] + m[j][i]) * s;
				q[k] = (m[i][k] + m[k][i]) * s;

				x = q[0];
				y = q[1];
				z = q[2];
				w = q[3];
			}
		}
	}
};

__forceinline SMQuaternion operator*(const SMQuaternion & q1, const SMQuaternion & q2)
{
	float A, B, C, D, E, F, G, H;

	A = (q1.w + q1.x) * (q2.w + q2.x);
	B = (q1.z - q1.y) * (q2.y - q2.z);
	C = (q1.x - q1.w) * (q2.y + q2.z);
	D = (q1.y + q1.z) * (q2.x - q2.w);
	E = (q1.x + q1.z) * (q2.x + q2.y);
	F = (q1.x - q1.z) * (q2.x - q2.y);
	G = (q1.w + q1.y) * (q2.w - q2.z);
	H = (q1.w - q1.y) * (q2.w + q2.z);
	return(SMQuaternion(A - (E + F + G + H) * 0.5f, -C + (E - F + G - H) * 0.5f, -D + (E - F - G + H) * 0.5f, B + (-E - F + G + H) * 0.5f).Normalize());
}

__forceinline float3 operator *(const SMQuaternion & q, const float3 & p)
{
	float xxzz = q.x*q.x - q.z*q.z;
	float wwyy = q.w*q.w - q.y*q.y;
	float xw2 = q.x*q.w*2.0f;
	float xy2 = q.x*q.y*2.0f;
	float xz2 = q.x*q.z*2.0f;
	float yw2 = q.y*q.w*2.0f;
	float yz2 = q.y*q.z*2.0f;
	float zw2 = q.z*q.w*2.0f;
	float3_t oout = float3_t((xxzz + wwyy)*p.x + (xy2 + zw2)*p.y + (xz2 - yw2)*p.z,
		(xy2 - zw2)*p.x + (q.y*q.y + q.w*q.w - q.x*q.x - q.z*q.z)*p.y + (yz2 + xw2)*p.z,
		(xz2 + yw2)*p.x + (yz2 - xw2)*p.y + (wwyy - xxzz)*p.z);
	return(oout);

	SMMATRIX m = q.GetMatrix();

	float4 qqq = float4(p, 1.0f) * m;
	//float4 qqq = m * p;


	float4 qq = float4(p.x * q.w + p.z * q.y - p.y * q.z,
		p.y * q.w + p.x * q.z - p.z * q.x,
		p.z * q.w + p.y * q.x - p.x * q.y,
		p.x * q.x + p.y * q.y + p.z * q.z);
	float3 r = float3(q.w * qq.x + q.x * qq.w + q.y * qq.z - q.z * qq.y,
		q.w * qq.y + q.y * qq.w + q.z * qq.x - q.x * qq.z,
		q.w * qq.z + q.z * qq.w + q.x * qq.y - q.y * qq.x);
	r.x *= (1.0f / (q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w));
	r.y *= (1.0f / (q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w));
	r.z *= (1.0f / (q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w));

	r.y *= -1.0f;
	r.x *= -1.0f;
	r.z *= -1.0f;
	return(r);
}

__forceinline SMQuaternion SMQuaternion::Normalize()
{
	float l = Length();
	return(SMQuaternion(x / l, y / l, z / l, w / l));
}

__forceinline float SMQuaternion::Length()
{
	return(sqrt(x * x + y * y + z * z + w * w));
}

__forceinline SMQuaternion SMQuaternion::Inverse()
{
	float length;
	length = 1.0f / (x * x + y * y + z * z + w * w);
	x *= -length;
	y *= -length;
	z *= -length;
	w *= length;
	return(SMQuaternion(x * -length, y * -length, z * -length, w * length));
}

__forceinline SMQuaternion SMQuaternion::Renormalize()
{
	double len = 1 - x * x - y * y - z * z;
	if(len < 0.0001f)
	{
		return(SMQuaternion(x, y, z, 0.0f));
	}
	else
	{
		return(SMQuaternion(x, y, z, -sqrtf(len)));
	}
}

__forceinline SMQuaternion SMQuaternion::Conjugate() const
{
	return(SMQuaternion(-x, -y, -z, w));
}

__forceinline SMMATRIX SMQuaternion::GetMatrix() const
{
	SMMATRIX m;
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	x2 = x + x;
	y2 = y + y;
	z2 = z + z;
	xx = x * x2;   xy = x * y2;   xz = x * z2;
	yy = y * y2;   yz = y * z2;   zz = z * z2;
	wx = w * x2;   wy = w * y2;   wz = w * z2;

	m._11 = 1.0f - (yy + zz); m._12 = xy - wz;        m._13 = xz + wy;
	m._21 = xy + wz;        m._22 = 1.0f - (xx + zz); m._23 = yz - wx;
	m._31 = xz - wy;        m._32 = yz + wx;        m._33 = 1.0f - (xx + yy);

	m._14 = m._24 = m._34 = 0;
	m._41 = m._42 = m._43 = 0;
	m._44 = 1;
	return(m);
}

__forceinline SMQuaternion SMquaternionSlerp(const SMQuaternion &q, const SMQuaternion &p, float t)
{
	float p1[4];
	double omega, cosom, sinom, scale0, scale1;

	// косинус угла
	cosom = q.x*p.x + q.y*p.y + q.z*p.z + q.w*p.w;

	if(cosom < 0.0)
	{
		cosom = -cosom;
		p1[0] = -p.x;  p1[1] = -p.y;
		p1[2] = -p.z;  p1[3] = -p.w;
	}
	else
	{
		p1[0] = p.x;    p1[1] = p.y;
		p1[2] = p.z;    p1[3] = p.w;
	}

	if((1.0 - cosom) > 0.00001f)
	{
		// стандартный случай (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0 - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	}
	else
	{
		// если маленький угол - линейная интерполяция
		scale0 = 1.0 - t;
		scale1 = t;
	}
	return(SMQuaternion(scale0 * q.x + scale1 * p1[0], scale0 * q.y + scale1 * p1[1], scale0 * q.z + scale1 * p1[2], scale0 * q.w + scale1 * p1[3]));
}

__forceinline static float InnerProduct(const SMQuaternion & q1, const SMQuaternion & q2)
{
	return(q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w);
}

__forceinline float SM_FISQRT(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;  // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1); // what the fuck?
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y)); // 1st iteration
	// y  = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

	return(y);
}

__forceinline float SM_FSQRT(float number)
{
	return(1.0f / SM_FISQRT(number));
}

__forceinline float SMVector3FastLength(const float3 & V)
{
	SMVECTOR vLengthSq;
	vLengthSq.mmv = _mm_mul_ps(V, V);

	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));

	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);

	vTemp.mmv = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));

	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);

	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));

	return(SM_FSQRT(vLengthSq.x));
}

__forceinline float SMVector3NonSqrtLength(const float3 & V)
{
	SMVECTOR vLengthSq;
	vLengthSq.mmv = _mm_mul_ps(V, V);

	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));

	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);

	vTemp.mmv = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));

	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);

	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));

	return(vLengthSq.x);
}

_forceinline float Clamp01(const float &a)
{
	if(a < 0.0f)
	{
		return(0.0f);
	}
	if(a > 1.0f)
	{
		return(1.0f);
	}
	return(a);
}
__forceinline float3 Clamp01(const float3 &a)
{
	float3 v(Clamp01(a.x), Clamp01(a.y), Clamp01(a.z));
	return(v);
}

__forceinline float SMVector3Length2(const float3 & V)
{

	SMVECTOR vLengthSq;
	vLengthSq.mmv = _mm_mul_ps(V, V);

	SMVECTOR vTemp;
	vTemp.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));

	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);

	vTemp.mmv = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));

	vLengthSq.mmv = _mm_add_ss(vLengthSq, vTemp);

	vLengthSq.mmv = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));

	return(vLengthSq.x);
}

__forceinline float3 SMMatrixToEuler(const SMMATRIX & mat)
{
	float RADIANS = 180.0f / SM_PI;
	float D;
	float3 res;
	res.y = D = -asin(mat._13);
	float C = cos(res.y);
	res.y *= RADIANS;
	float tr_x, tr_y;

		if(fabs(C) > 0.005)
		{
			tr_x = mat._33 / C;
			tr_y = -mat._23 / C;

			res.x = atan2(tr_y, tr_x) * RADIANS;

			tr_x = mat._11 / C;
			tr_y = -mat._12 / C;

			res.z = atan2(tr_y, tr_x) * RADIANS;
		}
		else
		{
			res.x = 0.0f;

			tr_x = mat._22;
			tr_y = mat._21;

			res.z = atan2(tr_y, tr_x) * RADIANS;
		}
//D3DXToRadian
	res.x = -SMToRadian(res.x);
	res.y = -SMToRadian(res.y);
	res.z = -SMToRadian(res.z);
	return(res);
}

__forceinline float3 SMEulerToVec(const float3 & in)
{
	return(SMMatrixRotationX(in.x) * SMMatrixRotationY(in.y) * SMMatrixRotationZ(in.z) * float3(0.0f, -1.0f, 0.0f));
}


#define FLOAT_INF ((float)INFINITY)

#endif
