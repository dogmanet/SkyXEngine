


#ifndef min
#	define min(a,b) ((a)>(b)?(b):(a))
#endif

float UTIL_DistancePointBeam2(const float3 & p, const float3 & start, const float3 & dir)
{
 float3 v = dir;
 float3 w = p - start;
 float c1;
 if((c1 = SMVector3Dot(w, v)) <= 0.0f)
 {
  return(SMVector3Length2(p - start));
 }
 float c2 = SMVector3Dot(v, v);

 float b = c1 / c2;
 float3 Pb = start + b * v;
 return(SMVector3Length2(p - Pb));
}

float DistancePointLine(const float3 & P, const float3 & P0, const float3 & P1)
{
	float3 v = P1 - P0;
	float3 w = P - P0;
	float c1 = SMVector3Dot(w, v);
	if(c1 <= 0)
	{
		return(SMVector3Length(P - P0));
	}
	float c2 = SMVector3Dot(v, v);

	if(c2 <= c1)
	{
		return(SMVector3Length(P - P1));
	};
	float b = c1 / c2;
	float3 Pb = P0 + b * v;
	return(SMVector3Length(P - Pb));
}

float DistancePointLine2(const float3 & P, const float3 & P0, const float3 & P1)
{
	float3 v = P1 - P0;
	float3 w = P - P0;
	float c1 = SMVector3Dot(w, v);
	if(c1 <= 0)
	{
		return(SMVector3Length2(P - P0));
	}
	float c2 = SMVector3Dot(v, v);

	if(c2 <= c1)
	{
		return(SMVector3Length2(P - P1));
	};
	float b = c1 / c2;
	float3 Pb = P0 + b * v;
	return(SMVector3Length2(P - Pb));
}

/// Реализация из Christer Ericson's Real-Time Collision Detection, pp. 51-52.
inline float TriArea2D(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return((x1 - x2)*(y2 - y3) - (x2 - x3)*(y1 - y2));
}


class LineSegment
{
public:
	float3 a, b;

	LineSegment(const float3 &a_, const float3 &b_):a(a_), b(b_)
	{
	}

	float3 ClosestPoint(const float3 &point, float &d) const
	{
		float3 dir = b - a;
		d = Clamp01(SMVector3Dot(point - a, dir) / SMVector3Length2(dir));
		return(a + d * dir);
	}

	float3 GetPoint(float d) const
	{
		return((1.0f - d) * a + d * b);
	}

	void ClosestPointLineLine(const float3 &v0, const float3 &v10, const float3 &v2, const float3 &v32, float &d, float &d2) const
	{
		float3 v02 = v0 - v2;
		float d0232 = SMVector3Dot(v02, v32);
		float d3210 = SMVector3Dot(v32, v10);
		float d3232 = SMVector3Dot(v32, v32);

		float d0210 = SMVector3Dot(v02, v10);
		float d1010 = SMVector3Dot(v10, v10);
		float denom = d1010*d3232 - d3210*d3210;
		if(denom != 0.0f)
		{
			d = (d0232*d3210 - d0210*d3232) / denom;
		}
		else
		{
			d = 0.0f;
		}
		d2 = (d0232 + d * d3210) / d3232;
	}

	float3 ClosestPoint(const LineSegment &other, float &d, float &d2) const
	{
		float3 dir = b - a;
		ClosestPointLineLine(a, b - a, other.a, other.b - other.a, d, d2);
		if(d >= 0.f && d <= 1.f && d2 >= 0.f && d2 <= 1.f)
			return a + d * dir;
		else if(d >= 0.f && d <= 1.f)
		{
			float3 p;
			if(d2 < 0.f)
			{
				d2 = 0.f;
				p = other.a;
			}
			else
			{
				d2 = 1.f;
				p = other.b;
			}
			return(ClosestPoint(p, d));
		}
		else if(d2 >= 0.f && d2 <= 1.f)
		{
			float3 p;
			if(d < 0.f)
			{
				d = 0.f;
				p = a;
			}
			else
			{
				d = 1.f;
				p = b;
			}

			other.ClosestPoint(p, d2);
			return(p);
		}
		else
		{
			float3 p;
			if(d < 0.f)
			{
				p = a;
				d = 0.f;
			}
			else
			{
				p = b;
				d = 1.f;
			}

			float3 p2;
			if(d2 < 0.f)
			{
				p2 = other.a;
				d2 = 0.f;
			}
			else
			{
				p2 = other.b;
				d2 = 1.f;
			}

			float T, T2;
			float3 closestPoint = ClosestPoint(p2, T);
			float3 closestPoint2 = other.ClosestPoint(p, T2);

			if(SMVector3Length2(closestPoint - p2) <= SMVector3Length2(closestPoint2 - p))
			{
				d = T;
				return(closestPoint);
			}
			else
			{
				d2 = T2;
				return(p);
			}
		}
	}
};

struct triangle
{
	float3_t a;
	float3_t b;
	float3_t c;
	triangle()
	{
	};
	triangle(float3_t _a, float3_t _b, float3_t _c):a(_a), b(_b), c(_c)
	{
	};
	//Минимальное расстояние от точки до треугольника
	float DistanceFromPoint(const float3 & p)
	{
		float3 n = SMVector3Cross(b - a, c - a);

		float3 pt;
		if(IntersectLine(p, p - n * 1000.0f, &pt))
		{
			float d = -SMVector3Dot(a, n);

			float dt = abs(SMVector3Dot(p, n) + d) / sqrtf(SMVector3Dot(n, n));
			return(dt);
		}
		else
		{
			float d1 = DistancePointLine(p, a, b);
			float d2 = DistancePointLine(p, b, c);
			float d3 = DistancePointLine(p, c, a);
			return(min(min(d1, d2), d3));
		}
	};

	//Квадрат минимального расстояния от точки до треугольника
	float DistanceFromPoint2(const float3 & p)
	{

		float3 n = SMVector3Cross(b - a, c - a);

		float3 pt;
		if(IntersectLine(p, p - n * 1000.0f, &pt))
		{
			float d = -SMVector3Dot(a, n);
			float dt = SMVector3Dot(p, n) + d;
			dt = dt * dt / SMVector3Dot(n, n);
			return(dt);
		}
		else
		{
			float d1 = DistancePointLine2(p, a, b);
			float d2 = DistancePointLine2(p, b, c);
			float d3 = DistancePointLine2(p, c, a);
			return(min(min(d1, d2), d3));
		}
	};

	//Проверкка пересечения треугольника и отрезка
	bool IntersectLine(const float3 & l1, const float3 &l2, float3 * p)
	{
		float3 n = SMVector3Normalize(SMVector3Cross((b - a), (c - b)));
		float d1 = SMVector3Dot((l1 - a), n) / SMVector3Length(n);
		float d2 = SMVector3Dot((l2 - a), n) / SMVector3Length(n);
		if((d1 > 0 && d2 > 0) || (d1 < 0 && d2 < 0))
			return(false);
		*p = l1 + (l2 - l1) * (-d1 / (d2 - d1));
		if(SMVector3Dot(SMVector3Cross((b - a), (*p - a)), n) <= 0) return(false);
		if(SMVector3Dot(SMVector3Cross((c - b), (*p - b)), n) <= 0) return(false);
		if(SMVector3Dot(SMVector3Cross((a - c), (*p - c)), n) <= 0) return(false);
		return(true);
	}

	float3 Point(float u, float v) const
	{
		return(a + ((b - a) * u + (c - a) * v));
	}


	LineSegment Edge(int i) const
	{
		if(i == 0)
		{
			return(LineSegment(a, b));
		}
		else if(i == 1)
		{
			return(LineSegment(b, c));
		}
		else if(i == 2)
		{
			return(LineSegment(c, a));
		}
		return(LineSegment(float3(), float3()));
	}

	float3 BarycentricUVW(const float3 &point) const
	{
		// Реализация из Christer Ericson's Real-Time Collision Detection, pp. 51-52.
		float3 m = SMVector3Cross(b - a, c - a);

		float nu, nv, ood;

		float x = abs(m.x);
		float y = abs(m.y);
		float z = abs(m.z);

		if(x >= y && x >= z)
		{
			nu = TriArea2D(point.y, point.z, b.y, b.z, c.y, c.z);
			nv = TriArea2D(point.y, point.z, c.y, c.z, a.y, a.z);
			ood = 1.0f / m.x;
		}
		else if(y >= z)
		{
			nu = TriArea2D(point.x, point.z, b.x, b.z, c.x, c.z);
			nv = TriArea2D(point.x, point.z, c.x, c.z, a.x, a.z);
			ood = 1.f / -m.y;
		}
		else
		{
			nu = TriArea2D(point.x, point.y, b.x, b.y, c.x, c.y);
			nv = TriArea2D(point.x, point.y, c.x, c.y, a.x, a.y);
			ood = 1.f / m.z;
		}
		float u = nu * ood;
		float v = nv * ood;
		float w = 1.0f - u - v;
		return(float3(u, v, w));
	}

	float2 BarycentricUV(const float3 &point) const
	{
		float3 uvw = BarycentricUVW(point);
		return(float2(uvw.y, uvw.z));
	}

	float3 ClosestPointToTriangleEdge(const LineSegment &lineSegment, float *outU, float *outV, float *outD) const
	{
		float unused1, unused2, unused3;
		float d1, d2, d3;
		float3 pt1 = Edge(0).ClosestPoint(lineSegment, unused1, d1);
		float3 pt2 = Edge(1).ClosestPoint(lineSegment, unused2, d2);
		float3 pt3 = Edge(2).ClosestPoint(lineSegment, unused3, d3);
		float dist1 = SMVector3Length2(pt1 - lineSegment.GetPoint(d1));
		float dist2 = SMVector3Length2(pt2 - lineSegment.GetPoint(d2));
		float dist3 = SMVector3Length2(pt3 - lineSegment.GetPoint(d3));
		if(dist1 <= dist2 && dist1 <= dist3)
		{
			if(outU) *outU = BarycentricUV(pt1).x;
			if(outV) *outV = BarycentricUV(pt1).y;
			if(outD) *outD = d1;
			return(pt1);
		}
		else if(dist2 <= dist3)
		{
			if(outU) *outU = BarycentricUV(pt2).x;
			if(outV) *outV = BarycentricUV(pt2).y;
			if(outD) *outD = d2;
			return(pt2);
		}
		else
		{
			if(outU) *outU = BarycentricUV(pt3).x;
			if(outV) *outV = BarycentricUV(pt3).y;
			if(outD) *outD = d3;
			return(pt3);
		}
	}

	float3 ClosestPoint(const float3 &p) const
	{
		/** Реализация из Christer Ericson's Real-Time Collision Detection, pp. 141-142. */

		// Check if P is in vertex region outside A.
		float3 ab = b - a;
		float3 ac = c - a;
		float3 ap = p - a;
		float d1 = SMVector3Dot(ab, ap);
		float d2 = SMVector3Dot(ac, ap);
		if(d1 <= 0.0f && d2 <= 0.0f)
		{
			return(a); // Barycentric coordinates are (1,0,0).
		}

		float3 bp = p - b;
		float d3 = SMVector3Dot(ab, bp);
		float d4 = SMVector3Dot(ac, bp);
		if(d3 >= 0.0f && d4 <= d3)
		{
			return(b); // Barycentric coordinates are (0,1,0).
		}

		// Check if P is in edge region of AB, and if so, return the projection of P onto AB.
		float vc = d1*d4 - d3*d2;
		if(vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
		{
			float v = d1 / (d1 - d3);
			return(a + v * ab); // The barycentric coordinates are (1-v, v, 0).
		}

		// Check if P is in vertex region outside C.
		float3 cp = p - c;
		float d5 = SMVector3Dot(ab, cp);
		float d6 = SMVector3Dot(ac, cp);
		if(d6 >= 0.f && d5 <= d6)
		{
			return(c); // The barycentric coordinates are (0,0,1).
		}

		// Check if P is in edge region of AC, and if so, return the projection of P onto AC.
		float vb = d5*d2 - d1*d6;
		if(vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
		{
			float w = d2 / (d2 - d6);
			return(a + w * ac); // The barycentric coordinates are (1-w, 0, w).
		}

		// Check if P is in edge region of BC, and if so, return the projection of P onto BC.
		float va = d3*d6 - d5*d4;
		if(va <= 0.0f && d4 - d3 >= 0.0f && d5 - d6 >= 0.0f)
		{
			float w = (d4 - d3) / (d4 - d3 + d5 - d6);
			return(b + w * (c - b)); // The barycentric coordinates are (0, 1-w, w).
		}

		// P must be inside the face region. Compute the closest point through its barycentric coordinates (u,v,w).
		float denom = 1.0f / (va + vb + vc);
		float v = vb * denom;
		float w = vc * denom;
		return(a + ab * v + ac * w);
	}

	float3 ClosestPoint(const LineSegment &lineSegment, float3 *otherPt)
	{
		float3 pt;
		if(IntersectLine(lineSegment.a, lineSegment.b, &pt))
		{
			if(otherPt)
			{
				*otherPt = pt;
			}
			return(pt);
		}

		float u1, v1, d1;
		float3 pt1 = ClosestPointToTriangleEdge(lineSegment, &u1, &v1, &d1);

		float3 pt2 = ClosestPoint(lineSegment.a);
		float3 pt3 = ClosestPoint(lineSegment.b);

		float3 pti = lineSegment.GetPoint(d1);

		float D1 = SMVector3Length2(pt1 - pti);
		float D2 = SMVector3Length2(pt2 - lineSegment.a);
		float D3 = SMVector3Length2(pt3 - lineSegment.b);

		if(D1 <= D2 && D1 <= D3)
		{
			if(otherPt)
			{
				*otherPt = pti;
			}
			return(pt1);
		}
		else if(D2 <= D3)
		{
			if(otherPt)
			{
				*otherPt = lineSegment.a;
			}
			return(pt2);
		}
		else
		{
			if(otherPt)
			{
				*otherPt = lineSegment.b;
			}
			return(pt3);
		}
	}
};

float3 TestCollision(const float3 & bbMin, const float3 & bbMax, Array<SkyXEngine::Graphics::ThreeD::SegmentationModel*> & Arr, Array<SkyXEngine::Graphics::ThreeD::InfoRenderSegments*> & ArrCollision/*Array<triangle> tris, int triCount*/, float radius = 0.4f, float3 * fOldPos = 0, float3 * fOrigPos = 0)
{
	float3 mm = (bbMax - bbMin) * 0.5f;
	float3 mc = (bbMax + bbMin) * 0.5f;
	float maxlength = mm.y + mm.x;
	bool tmpiscom = true;

	float3 p0(mc.x, bbMin.y, mc.z);
	float3 p1(mc.x, bbMax.y, mc.z);

	float rmin = min(min(mm.x, mm.y), mm.z);

	static float3 vUP = float3(0, 1, 0);

	radius = mm.x;
	float r = 2.0f * (mm.y + radius);

	float3 res, resm;
	//int c = 0;

	float r2 = r * r;

	triangle tmptri;

	float dist;
	float3 pt1, pt2;
	float3 d;
	float l;
	float3 n;
	float3 _res;
	float3 offs;

	if(fOldPos && fOrigPos)
	{
		float3 op = mc - *fOrigPos;
		if(SMVector3Length2(*fOldPos - *fOrigPos) > r2 * 0.5)
		{
			float3 pos;
			DWORD idMesh;
			if(SkyXEngine::Core::Data::Level::ArrStaticModel->TraceBeam(*fOldPos + op, mc, &pos, &idMesh,0, TRUE))
			{
				offs = pos - mc;
				p0 += offs;
				p1 += offs;
				mc = pos;
			}
		}
	}

	const float eps = 0.00001f;
	const float step_height = 0.8f + p0.y - radius;//68

	float yMax;
	float yMin;
	for(DWORD i = 0; i<Arr.size() && tmpiscom; i++)
	{
		ArrCollision[i]->CountCom = 0;
		Arr[i]->GetIntersected(&mc, ArrCollision[i]->Arr, &(ArrCollision[i]->CountCom), Arr[i]->ArrSegments, maxlength);
		DWORD tmpcount = ArrCollision[i]->CountCom;
		int qwert = 0;

		for(DWORD k = 0; k<tmpcount && tmpiscom; k++)
		{
			for(DWORD group = 0; group<ArrCollision[i]->Arr[k]->CountSubSet && tmpiscom; group++)
			{
				//triangle* tmptri = new triangle[ArrCollision[i]->Arr[k]->CountPoly[group]];
				for(DWORD numpoly = 0; numpoly<ArrCollision[i]->Arr[k]->CountPoly[group] * 3 && tmpiscom; numpoly += 3)
				{
					tmptri.a = Arr[i]->ArrMeshVertex[ArrCollision[i]->Arr[k]->ArrPoly[group][numpoly]];
					tmptri.b = Arr[i]->ArrMeshVertex[ArrCollision[i]->Arr[k]->ArrPoly[group][numpoly + 1]];
					tmptri.c = Arr[i]->ArrMeshVertex[ArrCollision[i]->Arr[k]->ArrPoly[group][numpoly + 2]];

					dist = tmptri.DistanceFromPoint2(mc);
					if(dist < r2)
					{
						pt1 = tmptri.ClosestPoint(LineSegment(p0, p1), &pt2);

						
						d = pt1 - pt2;
						l = SMVector3Length(d);
						if(l < radius)
						{
							n = SMVector3Normalize(SMVector3Cross(tmptri.c - tmptri.b, tmptri.a - tmptri.b));
							if(l < eps) // если треугольник пересекает ось
							{
								l = radius + SMVector3Length(p0 - pt1); // выталкивать относительно нижней точки будем
								d = -n;
							}
							else
							{
								l = (radius - l) / l;
							}
							d *= l;
							_res = -d;

							float cs = abs(SMVector3Dot(n, vUP));
							if(cs > 0.50f) // предельный cos угла наклона плоскости относительно вертикали, дальше будет соскальзывать. 0.86 ~= 30 deg. 0.70 ~= 45 deg
							{
								_res.x = _res.z = 0.0f;
							}
							else
							{
								yMax = max(tmptri.a.y, max(tmptri.b.y, tmptri.c.y));
								yMin = min(tmptri.a.y, min(tmptri.b.y, tmptri.c.y));
								if((cs < 0.30f || yMax - yMin < 0.1f) && yMax < step_height)
								{
									_res.x = _res.z = 0.0f;
								}
							}
							/*{
								if(abs(_res.x) > abs(res.x))
								{
									res.x = _res.x;
								}
								if(abs(_res.y) > abs(res.y))
								{
									res.y = _res.y;
								}
								if(abs(_res.z) > abs(res.z))
								{
									res.z = _res.z;
								}
								//res += _res;
								//c++;
							}*/
							{
								if(_res.x > 0.0f && _res.x > res.x)
								{
									res.x = _res.x;
								}
								else if(_res.x < 0.0f && _res.x < resm.x)
								{
									resm.x = _res.x;
								}
								if(_res.y > 0.0f && _res.y > res.y)
								{
									res.y = _res.y;
								}
								else if(_res.y < 0.0f && _res.y < resm.y)
								{
									resm.y = _res.y;
								}
								/*if(abs(_res.y) > abs(res.y))
								{
									res.y = _res.y;
								}*/
								if(_res.z > 0.0f && _res.z > res.z)
								{
									res.z = _res.z;
								}
								else if(_res.z < 0.0f && _res.z < resm.z)
								{
									resm.z = _res.z;
								}
								//res += _res;
								//c++;
							}
						}
					}
				}
			}
		}
	}
	if(res.y + resm.y < 0.1f)
	{
		resm.y = 0.0f;
	}
	res += resm;
	//sprintf(g_szText, "%.4f %.4f %.4f\n%.4f %.4f %.4f", res.x, res.y, res.z, mc.x, mc.y, mc.z);
	return(res + offs);
}

namespace SkyXEngine
{
namespace Graphics
{
namespace ThreeD
{


Segment::Segment()
{
		for(int i=0;i<8;i++)
			SplitMeshes[i] = 0;
	BoundBox = 0;
	NumberGroup = 0;
	TypeMaterial = 0;
	CountPoly = 0;
	ArrPoly = 0;
	GreenData = 0;
	ArrSerialNum = 0;
}

Segment::~Segment()
{
		for(int i=0;i<8;i++)
			nmem_delete(SplitMeshes[i]);
	nmem_delete_a(NumberGroup);
	nmem_delete_a(CountPoly);
	nmem_delete_a(TypeMaterial);

		if(ArrPoly)
		{
				for(DWORD i=0;i<CountSubSet;i++)
					nmem_delete_a(ArrPoly[i]);
		}

	nmem_delete_a(ArrPoly);
	nmem_delete_a(GreenData);
	ndx_release(BoundBox);
}

///////////////////

SegmentationModel::SegmentationModel()
{
	DistForLod = 100;
	SplitsIDs = 0;
	SplitsIDsRender = 0;
	IsOpen = true;
	MeshSound = 0;
	IsSegmentationModel = true;
}

SegmentationModel::~SegmentationModel()
{
	ndx_release(IndexBuffer);

	nmem_delete_a(ArrMeshVertex);
	nmem_delete(ArrSegments);

		for(DWORD i=0;i<Data->SubsetCount;i++)
			nmem_delete_a(RTCPUArrIndicesPtrs[i]);
	nmem_delete_a(RTCPUArrIndicesPtrs);

	nmem_delete_a(IsVisibleGroup);
	nmem_delete_a(RTCountDrawPoly);
	
}

inline bool SegmentationModel::IsLoadLod()
{
	return IsLoded;
}

inline DWORD SegmentationModel::GetCountSplits()
{
	return SplitsIDs;
}

inline DWORD SegmentationModel::GetCountSplitsRender()
{
	return SplitsIDsRender;
}

void SegmentationModel::Segmentation(int type,DWORD count_poly_in_segment)
{
	IsVisibleGroup = new bool[Data->SubsetCount];
	memset(IsVisibleGroup,0,sizeof(bool) * Data->SubsetCount);
	CountPolyInSegment = count_poly_in_segment;

	RTCPUArrIndicesPtrs = new DWORD*[Data->SubsetCount];
	RTCountDrawPoly = new DWORD[Data->SubsetCount];

		for(DWORD i=0;i<Data->SubsetCount;i++)
		{
			RTCPUArrIndicesPtrs[i] = new DWORD[Data->IndexCount[i]];
			RTCountDrawPoly[i] = 0;
		}

	Core::Data::Device->CreateIndexBuffer(
                   AllCountPoly * 3 * sizeof(DWORD),
                   D3DUSAGE_WRITEONLY,
                   D3DFMT_INDEX32,
                   D3DPOOL_MANAGED,
                   &IndexBuffer,
                   0);

	DWORD* tmpArrPoly = new DWORD[AllCountPoly * 3];

	DWORD* tmpindeces22 = 0;
	Data->IndexBuffer->Lock(0, 0, (void**)&tmpindeces22, 0);
	/*DWORD tmpCountNewPoly = 0;
		for(DWORD i=0;i<Data->SubsetCount;i++)
		{
				for(DWORD k=0;k<Data->IndexCount[i];k++)
				{
					tmpArrPoly[tmpCountNewPoly] = tmpindeces22[Data->StartIndex[i]+k] + Data->StartIndex[i];
					tmpCountNewPoly++;
				}
		}*/
	memcpy(tmpArrPoly,tmpindeces22,AllCountPoly * 3 * sizeof(DWORD));
	Data->IndexBuffer->Unlock();

	CountSplitsSys = type;

	//копируем все позиции вершин
	//{
	Graphics::ThreeD::vertex_static *CreateV;
	ArrMeshVertex = new float3[AllCountVertex];

	ArrMeshVertexRadius = new float[AllCountPoly];
	ArrMeshVertexCenter = new float3[AllCountPoly];
	
	Data->VertexBuffer->Lock(0,0,(void **)&CreateV,0);
		for(DWORD i=0;i<AllCountVertex;i++)
		{
			//CreateV[i].Pos;
			ArrMeshVertex[i] = (CreateV[i].Pos.operator float3());//*(D3DXVECTOR3*)((char*)(CreateV) + 76 * i);//
		}
	
	Data->VertexBuffer->Unlock();

	float3 pos0,pos1,pos2;
	float dist0,dist1,dist2;
	float3 CenterPoly;
	DWORD tmpKeyRadius = 0;
		for(DWORD i=0;i<AllCountPoly*3;i+=3)
		{
			pos0 = ArrMeshVertex[tmpArrPoly[i]];
			pos1 = ArrMeshVertex[tmpArrPoly[i+1]];
			pos2 = ArrMeshVertex[tmpArrPoly[i+2]];

			CenterPoly.x = CenterPoly.y = CenterPoly.z = 0;

			CenterPoly.x = (pos0.x + pos1.x + pos2.x) / 3.0;
			CenterPoly.y = (pos0.y + pos1.y + pos2.y) / 3.0;
			CenterPoly.z = (pos0.z + pos1.z + pos2.z) / 3.0;

			ArrMeshVertexCenter[tmpKeyRadius] = CenterPoly;

			dist0 = SMVector3Length(CenterPoly-pos0);
			dist1 = SMVector3Length(CenterPoly-pos1);
			dist2 = SMVector3Length(CenterPoly-pos2);
			//ArrMeshVertexRadius
				if(dist0 > dist1 && dist0 > dist2)
					ArrMeshVertexRadius[tmpKeyRadius] = dist0;
				else if(dist1 > dist0 && dist1 > dist2)
					ArrMeshVertexRadius[tmpKeyRadius] = dist1;
				else if(dist2 > dist0 && dist2 > dist1)
					ArrMeshVertexRadius[tmpKeyRadius] = dist2;
			tmpKeyRadius++;
		}

	nmem_delete_a(tmpArrPoly);

	this->CalculateBound(Data->VertexBuffer,AllCountVertex,sizeof(Graphics::ThreeD::vertex_static));
	//}


	ArrSegments = new Segment();
	ArrSegments->CountAllPoly = AllCountPoly;
	Core::WorkModel::ComputeBoundingBox(Data->VertexBuffer,&(ArrSegments->BoundVolumeP),AllCountVertex,sizeof(Graphics::ThreeD::vertex_static));

	float3 tmpMax,tmpMin;
	ArrSegments->BoundVolumeP.GetMinMax(&tmpMin,&tmpMax);
	float tmpX = tmpMax.x - tmpMin.x;
	float tmpY = tmpMax.y - tmpMin.y;
	float tmpZ = tmpMax.z - tmpMin.z;

	SkyXEngine::Core::WorkModel::CreateBoundingBoxMesh(&tmpMin,&tmpMax,&(ArrSegments->BoundBox));

	//выравниваем для равномерного делени
	//{
		if(this->CountSplitsSys == 8)
		{
				if(tmpX > tmpY && tmpX > tmpZ)
				{
					tmpY = tmpX - tmpY;
					tmpY /= 2.f;
					tmpMax.y += tmpY;
					tmpMin.y -= tmpY;

					tmpZ = tmpX - tmpZ;
					tmpZ /= 2.f;
					tmpMax.z += tmpZ;
					tmpMin.z -= tmpZ;
				}
				else if(tmpY > tmpX && tmpY > tmpZ)
				{
					tmpX = tmpY - tmpX;
					tmpX /= 2.f;
					tmpMax.x += tmpX;
					tmpMin.x -= tmpX;

					tmpZ = tmpY - tmpZ;
					tmpZ /= 2.f;
					tmpMax.z += tmpZ;
					tmpMin.z -= tmpZ;
				}
				else if(tmpZ > tmpX && tmpZ > tmpY)
				{
					tmpX = tmpZ - tmpX;
					tmpX /= 2.f;
					tmpMax.x += tmpX;
					tmpMin.x -= tmpX;

					tmpY = tmpZ - tmpY;
					tmpY /= 2.f;
					tmpMax.y += tmpY;
					tmpMin.y -= tmpY;
				}
		}
		else if(this->CountSplitsSys == 4)
		{
				if(tmpX > tmpZ)
				{
					tmpZ = tmpX - tmpZ;
					tmpZ /= 2.f;
					tmpMax.z += tmpZ;
					tmpMin.z -= tmpZ;
				}
				else if(tmpZ > tmpX)
				{
					tmpX = tmpZ - tmpX;
					tmpX /= 2.f;
					tmpMax.x += tmpX;
					tmpMin.x -= tmpX;
				}
		}
	
	ArrSegments->BoundVolumeSys.SetMinMax(&tmpMin,&tmpMax);
	//}}

	ArrSegments->CountSubSet = Data->SubsetCount;
	ArrSegments->NumberGroup = new DWORD[Data->SubsetCount];
	ArrSegments->TypeMaterial = new int[Data->SubsetCount];
	ArrSegments->CountPoly = new DWORD[Data->SubsetCount];

		//заполняем массив с номаерами подгрупп и массив с количествами полигонов
		for(DWORD i=0;i<Data->SubsetCount;i++)
		{
			ArrSegments->NumberGroup[i] = i;
				if(this->ArrMaterials[i] != -1 && this->ArrMaterials[i] < Core::Data::MaterialsManager->GetCount())
					ArrSegments->TypeMaterial[i] = Core::Data::MaterialsManager->GetMaterial(this->ArrMaterials[i])->PhysicsMaterial;
				else
					ArrSegments->TypeMaterial[i] = 0;
			//Core::InLog("PhysicsMaterial = %d\n",ArrSegments->TypeMaterial[i]);
			ArrSegments->CountPoly[i] = Data->IndexCount[i]/3;
		}


	//создаем массивы с полигонами, каждый полигон принадлежит своей подгруппе
	ArrSegments->ArrPoly = new DWORD*[ArrSegments->CountSubSet];
		for(DWORD i=0;i<ArrSegments->CountSubSet;i++)
		{
			ArrSegments->ArrPoly[i] = new DWORD[ArrSegments->CountPoly[i]*3];
		}

	ArrSegments->ArrSerialNum = new DWORD*[ArrSegments->CountSubSet];

	DWORD tmpSerialNum = 0;
		for(DWORD i=0;i<ArrSegments->CountSubSet;i++)
		{
			ArrSegments->ArrPoly[i] = new DWORD[ArrSegments->CountPoly[i]*3];
			ArrSegments->ArrSerialNum[i] = new DWORD[ArrSegments->CountPoly[i]];

				for(DWORD k=0;k<ArrSegments->CountPoly[i];k++)
				{
					ArrSegments->ArrSerialNum[i][k] = tmpSerialNum;
					tmpSerialNum++;
				}
		}

	//заполняем массивы с полигонами
	//{{
	DWORD* tmpindeces = 0;
	Data->IndexBuffer->Lock(0, 0, (void**)&tmpindeces, 0);
	DWORD tmpAllCountPoly = 0;

		for(DWORD i=0;i<Data->SubsetCount;i++)
		{
			memcpy(ArrSegments->ArrPoly[i],tmpindeces+Data->StartIndex[i],Data->IndexCount[i] * sizeof(DWORD));
				/*for(DWORD k=0;k<ArrSegments->CountPoly[i];k+=3)
				{
					Core::InLog("%d | %d | %d | %d\n",k,ArrSegments->ArrPoly[i][k],ArrSegments->ArrPoly[i][k+1],ArrSegments->ArrPoly[i][k+2]);
				}*/
			tmpAllCountPoly += Data->IndexCount[i];
		}
	Data->IndexBuffer->Unlock();
	//}}

		if(ArrSegments->CountAllPoly >= CountPolyInSegment && this->CountSplitsSys != 0)
			ArrSegments->BFNonEnd = true;
		else
			ArrSegments->BFNonEnd = false;

		//если указано что можно еще делить, то делим
		if(ArrSegments->BFNonEnd)
			CycleSegmentation(ArrSegments);

	//собираем статистику
	SplitsIDs = 0;			//общее количество сегментов
	SplitsIDsRender = 0;	//количество рисуемых сегментов
	SetSplitID(ArrSegments);//устанавливаем каждому сегменту идентификатор
	ArrSerialNum = new bool[SplitsIDsRender];
	memset(ArrSerialNum,0,sizeof(bool) * SplitsIDsRender);
}

void SegmentationModel::CycleSegmentation(Segment *SplitingMesh)
{
		if(SplitingMesh->BFNonEnd)
		{
			SecondSeparated(SplitingMesh);
				for(int i=0;i<this->CountSplitsSys;i++)
				{
						if(SplitingMesh->SplitMeshes[i])
							CycleSegmentation(SplitingMesh->SplitMeshes[i]);
				}
		}
}


void SegmentationModel::SecondSeparated(Segment* SecondMesh)
{
	Array<DWORD> ArrPoly[8];
	Array<DWORD> ArrGroup[8];
	Array<DWORD> ArrSerialNum[8];

		for(int i=0;i<this->CountSplitsSys;i++)
		{
			SecondMesh->SplitMeshes[i] = new Segment();
		}

		//получаем ограничивающие для деления и облегаемые объемы
		//{{
		if(CountSplitsSys == 4)
		{
			SkyXEngine::Graphics::ThreeD::Bound ArrBound[4];

			Core::WorkModel::ComputeBoundingBoxArr4(&(SecondMesh->BoundVolumeSys),(ArrBound));
				for(int i=0;i<this->CountSplitsSys;i++)
				{
					SecondMesh->SplitMeshes[i]->BoundVolumeSys = ArrBound[i];
					SecondMesh->SplitMeshes[i]->BoundVolumeP = ArrBound[i];
				}
		}
		else if(CountSplitsSys == 8)
		{
			SkyXEngine::Graphics::ThreeD::Bound ArrBound[8];

			Core::WorkModel::ComputeBoundingBoxArr8(&(SecondMesh->BoundVolumeSys),(ArrBound));
				for(int i=0;i<this->CountSplitsSys;i++)
				{
					SecondMesh->SplitMeshes[i]->BoundVolumeSys = ArrBound[i];
					SecondMesh->SplitMeshes[i]->BoundVolumeP = ArrBound[i];
				}
		}
		//}}

	bool *tmp_arr_mesh_poly = new bool[SecondMesh->CountAllPoly];
		for(DWORD i=0;i<SecondMesh->CountAllPoly;i++)
			tmp_arr_mesh_poly[i] = true;

	DWORD CountInPoly = 0;
	float3 tmpMin,tmpMax;
		for(int i=0;i<this->CountSplitsSys;i++)
		{
			SecondMesh->SplitMeshes[i]->BoundVolumeSys.GetMinMax(&tmpMin,&tmpMax);
			SkyXEngine::Core::WorkModel::CreateBoundingBoxMesh(&tmpMin,&tmpMax,&(SecondMesh->SplitMeshes[i]->BoundBox));
			DWORD tmpNumCurrentPoly = 0;
				for(DWORD j=0;j<SecondMesh->CountSubSet;j++)
				{
						for(DWORD k=0;k<SecondMesh->CountPoly[j]*3;k+=3)
						{
								if(Core::WorkModel::InPositionPoints2D(&tmpMin,&tmpMax,
														&ArrMeshVertex[SecondMesh->ArrPoly[j][k]],
														&ArrMeshVertex[SecondMesh->ArrPoly[j][k+1]], 
														&ArrMeshVertex[SecondMesh->ArrPoly[j][k+2]]
													)
									&& tmp_arr_mesh_poly[tmpNumCurrentPoly]
								)
								{	
									ArrPoly[i].push_back((SecondMesh->ArrPoly[j][k]));
									ArrPoly[i].push_back((SecondMesh->ArrPoly[j][k+1]));
									ArrPoly[i].push_back((SecondMesh->ArrPoly[j][k+2]));
									ArrGroup[i].push_back((SecondMesh->NumberGroup[j]));

									ArrSerialNum[i].push_back((SecondMesh->ArrSerialNum[j][k/3]));
								
									tmp_arr_mesh_poly[tmpNumCurrentPoly] = false;
									CountInPoly++;
								}
							tmpNumCurrentPoly++;
						}


				}
		}

		//ИНОГДА ТУТ БЫЛО ЧЕТО НЕПОНЯТНОЕ
		//ПОСЛЕ ТОГО КАК ГРАФИКА КАКИМ ТО МАГИЧЕСКИМ ОБРАЗОМ СТАЛА ЛУЧШЕ (КОСЯКИ С КУСТАМИ БЫЛИ ПРИ КОМПИЛЯЦИИ С /mt)
		//ПРОПАЛИ НЕКОТОЫРЕ УЧАСТКИ ОДНОГО ЗДАНИЯ, ПОСЛЕ ТОГО КАК УБРАЛ ЭТОТ КОД ВСЕ СТАЛО НОРМАЛЬНО
		if(CountInPoly < SecondMesh->CountAllPoly)
		{
			DWORD tmpNumCurrentPolyDec = 0;
			DWORD tmpNumCurrentPoly = 0;
			float3 pos0,pos1,pos2,CenterPoly,Length,SphereCenter;
			float ArrLength[8],ShereRadius;

				for(DWORD j=0;j<SecondMesh->CountSubSet;j++)
				{
						for(DWORD k=0;k<SecondMesh->CountPoly[j]*3;k+=3)
						{
								if(tmp_arr_mesh_poly[tmpNumCurrentPoly])
								{
									pos0 = ArrMeshVertex[SecondMesh->ArrPoly[j][k]];
									pos1 = ArrMeshVertex[SecondMesh->ArrPoly[j][k+1]];
									pos2 = ArrMeshVertex[SecondMesh->ArrPoly[j][k+2]];
									
									CenterPoly.x = CenterPoly.y = CenterPoly.z = 0;

									CenterPoly.x = (pos0.x + pos1.x + pos2.x) / 3.0;
									CenterPoly.y = (pos0.y + pos1.y + pos2.y) / 3.0;
									CenterPoly.z = (pos0.z + pos1.z + pos2.z) / 3.0;
									
									//memset(ArrLength,0,sizeof(float) * 8);

										for(int q=0;q<this->CountSplitsSys;q++)
										{
											SecondMesh->SplitMeshes[q]->BoundVolumeSys.GetSphere(&SphereCenter,&ShereRadius);
											Length = - SphereCenter + CenterPoly;        
											ArrLength[q] = SMVector3Length(Length);
										}

									WORD Key = 0;

										for(int q=0;q<this->CountSplitsSys;q++)
										{
											bool IsSmall = true;
												for(int n=0;n<this->CountSplitsSys;n++)
												{
														if(ArrLength[q] > ArrLength[n])
														{
															IsSmall = false;
															//break;
														}
												}

												if(IsSmall)
												{
													Key = q;
													//break;
												}
										}

									ArrPoly[Key].push_back((SecondMesh->ArrPoly[j][k]));
									ArrPoly[Key].push_back((SecondMesh->ArrPoly[j][k+1]));
									ArrPoly[Key].push_back((SecondMesh->ArrPoly[j][k+2]));
									ArrGroup[Key].push_back((SecondMesh->NumberGroup[j]));
									ArrSerialNum[Key].push_back((SecondMesh->ArrSerialNum[j][k/3]));

									tmp_arr_mesh_poly[tmpNumCurrentPolyDec] = false;
									//tmpNumCurrentPolyDec++;
								}
							tmpNumCurrentPoly++;
						}
				}	
		}



		for(int i=0;i<CountSplitsSys;i++)
		{
				if(ArrPoly[i].size() > 0)
				{
					Array<DWORD> tmpNumSubSet;
					SecondMesh->SplitMeshes[i]->CountSubSet = 0;

					//предварительно записываем одну подгруппу в массив tmpNumSubSet
					tmpNumSubSet.push_back(ArrGroup[i][0]);
					SecondMesh->SplitMeshes[i]->CountSubSet++;
						//вычисляем сколько всего у нас подгрупп
						for(DWORD k=0;k<ArrGroup[i].size();k++)
						{
							bool tmpIsThereGroup = false;
								for(DWORD j=0;j<tmpNumSubSet.size() && !tmpIsThereGroup;j++)
								{
										if(tmpNumSubSet[j] == ArrGroup[i][k])
											tmpIsThereGroup = true;
								}

								if(!tmpIsThereGroup)
								{
									tmpNumSubSet.push_back(ArrGroup[i][k]);
									SecondMesh->SplitMeshes[i]->CountSubSet++;
								}
						}

					//
					SecondMesh->SplitMeshes[i]->NumberGroup = new DWORD[SecondMesh->SplitMeshes[i]->CountSubSet];
					SecondMesh->SplitMeshes[i]->TypeMaterial = new int[SecondMesh->SplitMeshes[i]->CountSubSet];

						for(DWORD k=0;k<SecondMesh->SplitMeshes[i]->CountSubSet;k++)
						{
							SecondMesh->SplitMeshes[i]->NumberGroup[k] = (tmpNumSubSet[k]);
							SecondMesh->SplitMeshes[i]->TypeMaterial[k] = ArrSegments->TypeMaterial[tmpNumSubSet[k]];
							//Core::InLog("PhysicsMaterial[k] = %d\n",SecondMesh->SplitMeshes[i]->TypeMaterial[k]);
						}

					tmpNumSubSet.clear();

					SecondMesh->SplitMeshes[i]->CountPoly = new DWORD[SecondMesh->SplitMeshes[i]->CountSubSet];

						//обнуление данных GlobalMesh->SplitMeshes[i]->CountPoly
						for(DWORD j=0;j<SecondMesh->SplitMeshes[i]->CountSubSet;j++)
						{
							SecondMesh->SplitMeshes[i]->CountPoly[j] = 0;
						}
					SecondMesh->SplitMeshes[i]->CountAllPoly = 0;

						//вычисляем сколько полигонов в каждой подгруппе в данном сплите
						for(DWORD k=0;k<ArrPoly[i].size();k+=3)
						{
								for(DWORD j=0;j<SecondMesh->SplitMeshes[i]->CountSubSet;j++)
								{
										if(SecondMesh->SplitMeshes[i]->NumberGroup[j] == (ArrGroup[i][k/3]))
										{
											SecondMesh->SplitMeshes[i]->CountPoly[j]++;
											SecondMesh->SplitMeshes[i]->CountAllPoly++;
										}
								}
						}

					SecondMesh->SplitMeshes[i]->ArrPoly = new DWORD*[SecondMesh->SplitMeshes[i]->CountSubSet];
					SecondMesh->SplitMeshes[i]->ArrSerialNum = new DWORD*[SecondMesh->SplitMeshes[i]->CountSubSet];
						
						for(DWORD j=0;j<SecondMesh->SplitMeshes[i]->CountSubSet;j++)
						{
							SecondMesh->SplitMeshes[i]->ArrPoly[j] = new DWORD[SecondMesh->SplitMeshes[i]->CountPoly[j]*3];
							SecondMesh->SplitMeshes[i]->ArrSerialNum[j] = new DWORD[SecondMesh->SplitMeshes[i]->CountPoly[j]];
						}
					DWORD* tmpCountPG = new DWORD[SecondMesh->SplitMeshes[i]->CountSubSet];

						for(DWORD k=0;k<SecondMesh->SplitMeshes[i]->CountSubSet;k++)
						{
							tmpCountPG[k] = 0;
						}

						for(DWORD k=0;k<ArrPoly[i].size();k+=3)
						{
								for(DWORD j=0;j<SecondMesh->SplitMeshes[i]->CountSubSet;j++)
								{
									
										if(SecondMesh->SplitMeshes[i]->NumberGroup[j] == (ArrGroup[i][k/3]))
										{
											SecondMesh->SplitMeshes[i]->ArrSerialNum[j][tmpCountPG[j]/3] = ArrSerialNum[i][k/3];
											SecondMesh->SplitMeshes[i]->ArrPoly[j][tmpCountPG[j]] = (ArrPoly[i][k]);
											SecondMesh->SplitMeshes[i]->ArrPoly[j][tmpCountPG[j]+1] = (ArrPoly[i][k+1]);
											SecondMesh->SplitMeshes[i]->ArrPoly[j][tmpCountPG[j]+2] = (ArrPoly[i][k+2]);
											
											tmpCountPG[j]+=3;
										}
								}
						}

					ArrPoly[i].clear();
					ArrGroup[i].clear();

					nmem_delete_a(tmpCountPG);


						if(SecondMesh->SplitMeshes[i]->CountAllPoly > 0 && SecondMesh->SplitMeshes[i]->CountAllPoly > CountPolyInSegment)
							SecondMesh->SplitMeshes[i]->BFNonEnd = true;
						else
						{
							SecondMesh->SplitMeshes[i]->BFNonEnd = false;

							//оптимизация для Post TnL кэша
							//{{
							Array<DWORD> tmpVert;
							bool isunic = true;
								for(DWORD q=0;q<SecondMesh->SplitMeshes[i]->CountSubSet;q++)
								{
									tmpVert.clear();
									tmpVert.push_back(SecondMesh->SplitMeshes[i]->ArrPoly[q][0]);
										for(DWORD k=0;k<SecondMesh->SplitMeshes[i]->CountPoly[q]*3;k++)
										{
											isunic = true;
												for(DWORD j=0;j<tmpVert.size() && isunic;j++)
												{
														if(SecondMesh->SplitMeshes[i]->ArrPoly[q][k] == tmpVert[j])
														{
															isunic = false;
														}
												}
											
												if(isunic)
													tmpVert.push_back(SecondMesh->SplitMeshes[i]->ArrPoly[q][k]);
										}

									Core::OptimizeMesh::OptimizeIndecesInSubsetDword(SecondMesh->SplitMeshes[i]->ArrPoly[q],SecondMesh->SplitMeshes[i]->CountPoly[q],tmpVert.size());
								}
							//}}
						}
					EditVolume(SecondMesh->SplitMeshes[i]);
				}
				else
				{
					SecondMesh->SplitMeshes[i]->CountAllPoly = 0;
					SecondMesh->SplitMeshes[i]->BFNonEnd = false;

					nmem_delete(SecondMesh->SplitMeshes[i]);
				}
		}

	nmem_delete_a(SecondMesh->NumberGroup);
		if(SecondMesh != ArrSegments)
			nmem_delete_a(SecondMesh->TypeMaterial);
	nmem_delete_a(SecondMesh->CountPoly);

		for(DWORD k=0;k<SecondMesh->CountSubSet;k++)
		{
			nmem_delete_a(SecondMesh->ArrPoly[k]);
		}

	nmem_delete_a(SecondMesh->ArrPoly);
	nmem_delete_a(tmp_arr_mesh_poly);
}


void SegmentationModel::GPURender(Segment** arrsplits,DWORD count,bool iscomdistlod,bool render_alpha,bool ismaterial,bool is_old_render,bool is_one_dip,bool render_forward,bool is_camera,DWORD tex_selected)
{
		if(IsLoded && iscomdistlod && IsCurrentRenderLod)
		{
				if(!render_forward)
					Render(1,0,ismaterial,tex_selected);
		}
		else
		{
			DWORD tmp22CountCopy = 0;

				//если уже было все заполнено то остается только рендер, используется в случаях раздельного рендера прозрачных и непрозрачных объектов
				if(!is_old_render)
				{
						for(DWORD i=0;i<Data->SubsetCount;i++)
							RTCountDrawPoly[i] = 0;

						for(DWORD i=0;i<count;i++)
						{
								for(DWORD k=0;k<arrsplits[i]->CountSubSet;k++)
								{
										if(
											(is_camera ? arrsplits[i]->IsRenderForCamera : arrsplits[i]->IsRenderFor) &&	//если рендер для камеры и кусок помечен как видимый, либо рендер не для камеры и кусок помечен как видимый
											arrsplits[i]->CountPoly[k] > 0 &&	//если количество полигонов больше 0
											RTCountDrawPoly[arrsplits[i]->NumberGroup[k]] + arrsplits[i]->CountPoly[k] <= Data->IndexCount[arrsplits[i]->NumberGroup[k]]/3 &&	//если количество записанных полигонов в данную подгруппу меньше либо равно общему количеству полигонов которое содержит данная подгруппа
											(!is_one_dip || (is_one_dip && !(SkyXEngine::Core::Data::MaterialsManager->IsAlpha(ArrMaterials[arrsplits[i]->NumberGroup[k]]))))	//если рисуем не одним проходом а по подгруппам, либо одним проходом но данная подгруппа не прозрачная
										)
										{
											memcpy(RTCPUArrIndicesPtrs[arrsplits[i]->NumberGroup[k]]+(RTCountDrawPoly[arrsplits[i]->NumberGroup[k]]*3),
											arrsplits[i]->ArrPoly[k],
											arrsplits[i]->CountPoly[k] * sizeof(DWORD)*3);
																			
											RTCountDrawPoly[arrsplits[i]->NumberGroup[k]] += arrsplits[i]->CountPoly[k];
										}
								}
						}

					//заполняем индексный буфер
					//{{
					DWORD* RTGPUArrIndicesPtrs2;
					IndexBuffer->Lock(0, 0, (void**)&(RTGPUArrIndicesPtrs2),D3DLOCK_DISCARD);
						for(DWORD i=0;i<Data->SubsetCount;i++)
						{
								if(RTCountDrawPoly[i] > 0)
								{
									memcpy(RTGPUArrIndicesPtrs2+tmp22CountCopy,RTCPUArrIndicesPtrs[i],RTCountDrawPoly[i] * 3 * sizeof(DWORD));
									tmp22CountCopy += RTCountDrawPoly[i] * 3;
								}
						}
					IndexBuffer->Unlock();
					//}}
				}

			SkyXEngine::Core::Data::Device->SetStreamSource(0, Data->VertexBuffer, 0, sizeof(Graphics::ThreeD::vertex_static));
			SkyXEngine::Core::Data::Device->SetIndices(IndexBuffer);
			SkyXEngine::Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationStatic);
			
				//если рисуем одним проходом, то нахер текстуры
				if(is_one_dip)
				{
					//Core::Data::Device->SetTransform(D3DTS_WORLD,&(/*MatWorld*/World.operator D3DXMATRIX()));

					D3DXMATRIX tmpviewproj;
					Core::Data::Device->GetTransform(D3DTS_WORLD1,&tmpviewproj);

					float4x4 wvp = /*MatWorld*/World * float4x4(tmpviewproj);
					wvp = SMMatrixTranspose(wvp);
					Core::Data::Device->SetVertexShaderConstantF(0,(float*)&wvp,4);

					SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, AllCountVertex, 0, tmp22CountCopy);
					Core::Data::Statistics::CountPolyStaticGeom += tmp22CountCopy/3;
					Core::Data::Statistics::CountDips += 1;
				}
				else
				{
					tmp22CountCopy = 0;
						for(DWORD i=0;i<Data->SubsetCount;i++)
						{
								if(RTCountDrawPoly[i] > 0 && //если в данном подгруппе есть полигоны к отрисовке
									(//если данная подгруппа не прозрачна и сейчас мы рисуем не прозрачную геометрию, либо наоборот
									(!render_alpha && !(SkyXEngine::Core::Data::MaterialsManager->IsAlpha(ArrMaterials[i]))) ||
									(render_alpha && (SkyXEngine::Core::Data::MaterialsManager->IsAlpha(ArrMaterials[i])))
									) &&
									(//если подгруппа освещаема и мы рисуем освещаемую геометрию, либо наоборот
									(!render_forward && !(SkyXEngine::Core::Data::MaterialsManager->IsForward(ArrMaterials[i]))) ||
									(render_forward && (SkyXEngine::Core::Data::MaterialsManager->IsForward(ArrMaterials[i])))
									) &&
									(
									(SkyXEngine::Core::Data::Settings::IsComEditors && //если обработка редакторная
										(SkyXEngine::Core::Data::Settings::EditorsTypeRenderSubset == 0 ||	//если рисуем все подгруппы
										(SkyXEngine::Core::Data::Settings::EditorsTypeRenderSubset == 1 &&	//если рисуем выделенную подгруппу 
										i == SkyXEngine::Core::Data::Settings::EditorsSelectedSubset) ||	//и если текущая подгруппа есть выделенная
										(SkyXEngine::Core::Data::Settings::EditorsTypeRenderSubset == 2 &&	//если рисуем все кроме выделенной подгруппы 
										i != SkyXEngine::Core::Data::Settings::EditorsSelectedSubset))) ||	//и если текущая подгруппа не есть выделенная
									!SkyXEngine::Core::Data::Settings::IsComEditors	//если обработка не редакторная
									)
								)
								{
										//если рисуем с материалом
										if(ismaterial)
										{
												//если используется отражение то отправляем текстуру с отражением
												if(ArrReflection[i])
												{
														if(SkyXEngine::Core::Data::Settings::UseReflection)
														{
																if(ArrReflection[i]->TypeRef == 0)
																	SkyXEngine::Core::Data::Device->SetTexture(12,ArrReflection[i]->GetRefPlaneTex());
																else if(ArrReflection[i]->TypeRef == 1)
																	SkyXEngine::Core::Data::Device->SetTexture(12,ArrReflection[i]->GetRefCubeTex());
														}
														else
															SkyXEngine::Core::Data::Device->SetTexture(12,Core::Data::LoadedTextures->GetTexture(SkyXEngine::Core::Data::EngineID::Tex_Gray));
												}

												//и дистанция меньше чем дистанция с которой начианется лод
												if(ArrSegments->DistForCamera < DistForLod)
													SkyXEngine::Core::Data::MaterialsManager->Render(this->ArrMaterials[i],&/*MatWorld*/World,0);
												//иначе рисуем с базовым материалом
												else
												{
													SkyXEngine::Core::Data::MaterialsManager->Render(SkyXEngine::Core::Data::EngineID::ID_MtrlBase,&/*MatWorld*/World,0);
													SkyXEngine::Core::Data::MaterialsManager->SetMainTexture(0,this->ArrMaterials[i]);
												}
										}
										else
										{
											//если рисуем без материала то скорее всего это либо отражения либо тени, значит нужно отправить матрицы в шейдер
											//SkyXEngine::Core::Data::MaterialsManager->Render(SkyXEngine::Core::Data::EngineID::ID_MtrlBase,&MatWorld,0);
											SkyXEngine::Core::Data::MaterialsManager->SetMainTexture(0,this->ArrMaterials[i]);
											Core::Data::Device->SetTransform(D3DTS_WORLD,&(/*MatWorld*/World.operator D3DXMATRIX()));

											D3DXMATRIX tmpviewproj;
											Core::Data::Device->GetTransform(D3DTS_WORLD1,&tmpviewproj);

											float4x4 wvp = /*MatWorld*/World * float4x4(tmpviewproj);
											wvp = SMMatrixTranspose(wvp);
											Core::Data::Device->SetVertexShaderConstantF(0,(float*)&wvp,4);
										}

										//если необходимо выделить модель, накладываем указанную текстуру
										//используется в основном для ландшафта
										if(tex_selected != -1)
											SkyXEngine::Core::Data::Device->SetTexture(0,SkyXEngine::Core::Data::LoadedTextures->GetTexture(tex_selected));
									
									SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, Data->VertexCount[i]/*AllCountVertex*/, tmp22CountCopy, RTCountDrawPoly[i]);
									
									tmp22CountCopy += RTCountDrawPoly[i] * 3;
									Core::Data::Statistics::CountPolyStaticGeom += RTCountDrawPoly[i];
									Core::Data::Statistics::CountDips += 1;
								}
								else
								{
									tmp22CountCopy += RTCountDrawPoly[i] * 3;

										//если в объекте есть прозрачные подгруппы то помечаем что делаем второй проход
										if(!Core::Data::Statistics::IsExistTransparentObject && SkyXEngine::Core::Data::MaterialsManager->IsAlpha(ArrMaterials[i]))
											Core::Data::Statistics::IsExistTransparentObject = true;
								}
						}
				}

				/*SkyXEngine::Core::Data::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
									SkyXEngine::Core::Data::Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

									SkyXEngine::Core::Data::Device->SetTexture(0,0);

									ArrSegments->BoundBox->DrawSubset(0);*/
									//ArrSegments->SplitMeshes[0]->BoundBox->DrawSubset(0);
									//ArrSegments->SplitMeshes[1]->BoundBox->DrawSubset(0);
									//ArrSegments->SplitMeshes[2]->BoundBox->DrawSubset(0);
									//ArrSegments->SplitMeshes[3]->BoundBox->DrawSubset(0);

									//ArrSegments->SplitMeshes[0]->SplitMeshes[3]->BoundBox->DrawSubset(0);
										
										/*for(DWORD i=0;i<count;i++)
										{
												if(arrsplits[i]->IsRenderForCamera)
													arrsplits[i]->BoundBox->DrawSubset(0);
										}*/

									//SkyXEngine::Core::Data::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
									//SkyXEngine::Core::Data::Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		}
}

void SegmentationModel::SetSplitID(Segment* SplitingMesh)
{
		if(	SplitingMesh && 
			SplitingMesh->BFNonEnd
			)
		{
			SplitingMesh->ID = SplitsIDs;
			SplitsIDs++;
				for(int i=0;i<this->CountSplitsSys;i++)
					SetSplitID(SplitingMesh->SplitMeshes[i]);
		}
		else if(SplitingMesh)
		{
				//if(SplitingMesh->CountAllPoly > 0)
				//{
					SplitingMesh->ID = SplitsIDs;
					SplitsIDs++;
					SplitingMesh->SID = SplitsIDsRender;
					SplitsIDsRender++;
					SplitingMesh->WSID = Core::Data::Level::ArrStaticModel->ArrayWSIDs.size();
					Core::Data::Level::ArrStaticModel->ArrayWSIDs.push_back(SplitingMesh->WSID);
				/*}
				else
				{
					nmem_delete(SplitingMesh);

						if(SplitingMesh != 0)
							int wer=0;
				}*/
		}
}

void SegmentationModel::GetAllValidSegments(Segment* insplit,Segment** arrsplits,DWORD* count)
{
		if(insplit->BFNonEnd)
		{
				for(int i=0;i<this->CountSplitsSys;i++)
					GetAllValidSegments(insplit->SplitMeshes[i],arrsplits,count);
		}
		else if(insplit->CountAllPoly > 0)
		{
			arrsplits[(*count)] = insplit;
			(*count)++;
		}
}

bool SegmentationModel::IsVisible(Core::ControllMoving::Frustum* frustum)
{
	float3 center,center2;
	float radius;
	float dist;
	ArrSegments->BoundVolumeP.GetSphere(&center2,&radius);
	center = SMVector3Transform(center2,World/*MatWorld*/);

		if(frustum->SphereInFrustum(&center,radius))
			return true;
		else
			return false;
}



void SegmentationModel::CPUFillingArrIndeces(Core::ControllMoving::Frustum* frustum,Segment** arrsplits,DWORD *count,float3* viewpos,bool is_camera)
{
	ComRecArrIndeces(frustum,arrsplits,count,ArrSegments,is_camera,viewpos);
}

void SegmentationModel::ComRecArrIndeces(Core::ControllMoving::Frustum* frustum,Segment** arrsplits,DWORD *count,Segment* comsegment,bool is_camera,float3* viewpos)
{
	comsegment->BoundVolumeP.GetSphere(&jcenter2,&jradius);
	jcenter = SMVector3Transform(jcenter2,World/*MatWorld*/);

		if(comsegment->CountAllPoly > 0 && (frustum->SphereInFrustum(&jcenter,jradius)))
		{
			//comsegment->BoundVolumeP.GetMinMax(&jmin2,&jmax2);
			//jmin = SMVector3Transform(jmin2,World/*MatWorld*/);
			//jmax = SMVector3Transform(jmax2,World/*MatWorld*/);

				//if(frustum->BoxInFrustum(&jmin,&jmax))
				{
				if(comsegment->BFNonEnd && this->CountSplitsSys > 0)
				{
						//если установлена сортировка от ближних к дальним
						if(Core::Data::Settings::IsUseSortFrontToBack)
						{
								for(int q=0;q<this->CountSplitsSys;q++)
								{
									comsegment->SortId[q] = -1;
										if(comsegment->SplitMeshes[q])
										{
											comsegment->SplitMeshes[q]->BoundVolumeP.GetSphere(&jcenter,&jradius);
											comsegment->SplitMeshes[q]->DistFor = SMVector3Length2((jcenter - (*viewpos))) - jradius*jradius;
										}
								}

							float pl1,pl2;
							DWORD tmpCountGreater = 0;
								for(DWORD i=0;i<this->CountSplitsSys;i++)
								{
										if(comsegment->SplitMeshes[i])
										{
											tmpCountGreater = 0;
											pl1 = comsegment->SplitMeshes[i]->DistFor;

												for(DWORD k=0;k<this->CountSplitsSys;k++)
												{
														if(comsegment->SplitMeshes[k])
														{
															pl2 = comsegment->SplitMeshes[k]->DistFor;

																if(i != k && pl2 >= pl1)
																	tmpCountGreater++;
														}
												}

											bool tmpisnend = true;
												while(tmpisnend)
												{
														if(comsegment->SortId[tmpCountGreater] == -1)
														{
															comsegment->SortId[tmpCountGreater] = i;
															tmpisnend = false;
														}
														else
															tmpCountGreater--;
												}
										}
								}

								for(int j=0;j<this->CountSplitsSys;j++)
								{
										if(comsegment->SortId[(this->CountSplitsSys-1)-j] != -1 && comsegment->SplitMeshes[comsegment->SortId[(this->CountSplitsSys-1)-j]])
											ComRecArrIndeces(frustum,arrsplits,count,comsegment->SplitMeshes[comsegment->SortId[(this->CountSplitsSys-1)-j]],is_camera,viewpos);
								}
						}
						//иначе просто дальше считаем
						else
						{
								for(int j=0;j<this->CountSplitsSys;j++)
								{
										if(comsegment->SplitMeshes[j])
											ComRecArrIndeces(frustum,arrsplits,count,comsegment->SplitMeshes[j],is_camera,viewpos);
								}
						}

				}
				else
				{
						//если считаем для камеры значит посчитаем и расстоние до нее
						if(is_camera)
						{
								if(comsegment->SID < SplitsIDsRender)
									ArrSerialNum[comsegment->SID] = true;

								if(comsegment->WSID < Core::Data::Level::ArrStaticModel->ArrayWSIDs.size())
									Core::Data::Level::ArrStaticModel->ArrayWSIDs[comsegment->WSID] = true;

							comsegment->DistForCamera = SMVector3Length((jcenter - Core::Data::ConstCurrentCameraPosition)) - jradius;
						}

						if(comsegment->DistForCamera < 0)
							comsegment->DistForCamera = 0.f;

						if((*count) < SplitsIDsRender)
						{
							arrsplits[(*count)] = comsegment;
								
								if(is_camera)
									arrsplits[(*count)]->IsRenderForCamera = true;
								else
									arrsplits[(*count)]->IsRenderFor = true;
							(*count)++;
						}
				}
				}
		}
}

void SegmentationModel::EditVolume(Segment* SplitingMesh)
{
	float3 Max,Min;
	Max = ArrMeshVertex[(SplitingMesh->ArrPoly[0][0])];
	Min = ArrMeshVertex[(SplitingMesh->ArrPoly[0][0])];

	float3 Max2,Min2;
	SplitingMesh->BoundVolumeSys.GetMinMax(&Min2,&Max2);

	float3 tmpf;
		for(DWORD i=0;i<SplitingMesh->CountSubSet;i++)
		{
				for(DWORD k=0;k<SplitingMesh->CountPoly[i]*3;k++)
				{
					tmpf = ArrMeshVertex[(SplitingMesh->ArrPoly[i][k])];
						if(tmpf.x > Max.x)
							Max.x = tmpf.x;
						if(tmpf.y > Max.y)
							Max.y = tmpf.y;
						if(tmpf.z > Max.z)
							Max.z = tmpf.z;

						if(tmpf.x < Min.x)
							Min.x = tmpf.x;
						if(tmpf.y < Min.y)
							Min.y = tmpf.y;
						if(tmpf.z < Min.z)
							Min.z = tmpf.z;
				}
		}

	float3 Center = (Min + Max) * 0.5f;
	float Radius = SMVector3Length(Center-Max);

	SplitingMesh->BoundVolumeP.SetSphere(&Center,&Radius);
}

void SegmentationModel::GetIntersectedRayY(float3* pos,Segment** arrsplits,DWORD *count,Segment* comsegment)
{
	/*float3 center;
	float radius;
	comsegment->BoundVolumeP.GetSphere(&center,&radius);
	center = *pos - center;
	float distsqr = center.x * center.x + center.y * center.y + center.z * center.z;*/

	comsegment->BoundVolumeP.GetMinMax(&jmin2,&jmax2);

		if(comsegment->CountAllPoly > 0 && pos->x >= jmin2.x && pos->z >= jmin2.z && pos->x <= jmax2.x && pos->z <= jmax2.z)
		{
				if(comsegment->BFNonEnd && this->CountSplitsSys > 0)
				{
						for(int j=0;j<this->CountSplitsSys;j++)
						{
								if(comsegment->SplitMeshes[j])
									GetIntersectedRayY(pos,arrsplits,count,comsegment->SplitMeshes[j]);
						}
				}
				else
				{
						if((*count) < SplitsIDsRender)
						{
							arrsplits[(*count)] = comsegment;
								
							(*count)++;
						}
				}
		}
}

void SegmentationModel::GetIntersected(const float3* pos, Segment** arrsplits, DWORD *count, Segment* comsegment, float additionalRadius)
{
	float3 center;
	float radius;
	comsegment->BoundVolumeP.GetSphere(&center,&radius);
	//добавочный радиус, иначе не всегда корректно определяется набор треугольников для коллизии игрока в таком случае
	/*
		____
		|\ /|
		| X |
		|/_\|

	    ------	
	   /      \ - сфера
	*/
	radius += additionalRadius;
	center = *pos - center;
	float distsqr = SMVector3Length2(center);
		if(comsegment->CountAllPoly > 0 && distsqr <= radius*radius)
		{
				if(comsegment->BFNonEnd && this->CountSplitsSys > 0)
				{
						for(int j=0;j<this->CountSplitsSys;j++)
						{
								if(comsegment->SplitMeshes[j])
									GetIntersected(pos, arrsplits, count, comsegment->SplitMeshes[j], additionalRadius);
						}
				}
				else
				{
						if((*count) < SplitsIDsRender)
						{
							arrsplits[(*count)] = comsegment;
								
							(*count)++;
						}
				}
		}
}

void SegmentationModel::GetIntersected22(float3* pos,Segment** arrsplits,DWORD *count,Segment* comsegment)
{
	float3 center;
	float radius;
	comsegment->BoundVolumeP.GetSphere(&center,&radius);
	center = *pos - center;
	float distsqr = center.x * center.x + center.y * center.y + center.z * center.z;
		if(comsegment->CountAllPoly > 0 && distsqr <= radius*radius)
		{
				if(comsegment->BFNonEnd && this->CountSplitsSys > 0)
				{
						for(int j=0;j<this->CountSplitsSys;j++)
						{
								if(comsegment->SplitMeshes[j])
									GetIntersected22(pos,arrsplits,count,comsegment->SplitMeshes[j]);
						}
				}
				else
				{
						if((*count) < SplitsIDs)
						{
							arrsplits[(*count)] = comsegment;
								
							(*count)++;
						}
				}
		}
}

void SegmentationModel::SetSound(const char* path,float mindist,float dist)
{
	MeshSound = new SkyXEngine::Sound::SoundObject(path,true,1,0,dist,1,0.1);
	MeshSound->SetMinDist(mindist);
	MeshSound->SetVolume(-10000);
	MeshSound->Play();

	float3 tmppos;
	float tmpradius;
	ArrSegments->BoundVolumeP.GetSphere(&tmppos,&tmpradius);
	MeshSound->Position = tmppos;
}

void SegmentationModel::GetSound(char* path)
{
	
}

void SegmentationModel::GetPartFromBeam(const float3* pos, const float3 * dir, Segment** arrsplits, DWORD *count, Segment* comsegment)
{
	float3 center;
	float radius;
	comsegment->BoundVolumeP.GetSphere(&center, &radius);

	//center = *pos - center;
	float distsqr = UTIL_DistancePointBeam2(center, *pos, *dir);
		if(comsegment->CountAllPoly > 0 && distsqr <= radius*radius)
		{
				if(comsegment->BFNonEnd && this->CountSplitsSys > 0)
				{
					for(int j = 0; j<this->CountSplitsSys; j++)
					{
							if(comsegment->SplitMeshes[j])
								GetPartFromBeam(pos, dir, arrsplits, count, comsegment->SplitMeshes[j]);
					}
				}
				else
				{
						if((*count) < SplitsIDs)
						{
							arrsplits[(*count)] = comsegment;

							(*count)++;
						}
				}
		}
}

void SegmentationModel::GetIntersectedQuad(float3_t* max,float3_t* min,Segment** arrsplits, DWORD *count,Segment* comsegment)
{
	comsegment->BoundVolumeP.GetMinMax(&jmin,&jmax);
	
		if(comsegment->CountAllPoly > 0 && (((jmin.x <= min->x && jmax.x >= max->x) || (jmin.z <= min->z && jmax.z >= max->z))))
		{
				if(comsegment->BFNonEnd && this->CountSplitsSys > 0)
				{
						for(int j=0;j<this->CountSplitsSys;j++)
						{
								if(comsegment->SplitMeshes[j])
								{
									GetIntersectedQuad(max,min,arrsplits,count,comsegment->SplitMeshes[j]);
								}
						}
				}
				else
				{
						if((*count) < SplitsIDsRender)
						{
							arrsplits[(*count)] = comsegment;
								
							(*count)++;
						}
				}
		}
}

//////////////////////////////

bool ManagerStaticModel::GetIntersectedRayY(DWORD id,float3* pos)
{
	float3 tmppos0,tmppos1,tmppos2;
	BOOL  Hit;
	float u, v,Dist,CountDist;
	Dist = -1000;
	CountDist = -1000;
	bool tmpiscom = true;

		if(id >= 0 && id < Arr.size())
		{
			ArrCollision[id]->CountCom = 0;
			Arr[id]->GetIntersectedRayY(pos,ArrCollision[id]->Arr,&(ArrCollision[id]->CountCom),Arr[id]->ArrSegments);
			DWORD tmpcount = ArrCollision[id]->CountCom;

				for(DWORD k=0;k<tmpcount && tmpiscom;k++)
				{	
						for(DWORD group=0;group<ArrCollision[id]->Arr[k]->CountSubSet && tmpiscom;group++)
						{
								for(DWORD numpoly=0;numpoly<ArrCollision[id]->Arr[k]->CountPoly[group]*3 && tmpiscom;numpoly+=3)
								{
									Dist = -1000.f;
									//находим все 3 вершины
									tmppos0 = Arr[id]->ArrMeshVertex[ArrCollision[id]->Arr[k]->ArrPoly[group][numpoly]];
									tmppos1 = Arr[id]->ArrMeshVertex[ArrCollision[id]->Arr[k]->ArrPoly[group][numpoly+1]];
									tmppos2 = Arr[id]->ArrMeshVertex[ArrCollision[id]->Arr[k]->ArrPoly[group][numpoly+2]];

															
									Hit = D3DXIntersectTri(	&(tmppos0.operator D3DXVECTOR3()),
																		&(tmppos1.operator D3DXVECTOR3()),
																		&(tmppos2.operator D3DXVECTOR3()),
																		&(pos->operator D3DXVECTOR3()), 
																		&D3DXVECTOR3(0.0f, -1000.0f, 0.0f), &u, &v, &Dist);

										if(Hit == true)
										{
											pos->y-=Dist*1000;
											tmpiscom = false;
										}
										else
										{
											Hit = D3DXIntersectTri(	&(tmppos0.operator D3DXVECTOR3()),
																			&(tmppos1.operator D3DXVECTOR3()),
																			&(tmppos2.operator D3DXVECTOR3()),
																			&(pos->operator D3DXVECTOR3()), 
																			&D3DXVECTOR3(0.0f, 1000.0f, 0.0f), &u, &v, &Dist);
												if(Hit == true)
												{
													pos->y+=Dist*1000;
													tmpiscom = false;
												}
										}
								}
						}
				}
		}
	return !tmpiscom;
}

bool ManagerStaticModel::GetIntersectedRayYRain(float3* pos)
{
	jpos = pos->operator D3DXVECTOR3();
	BOOL  Hit;
	float u, v,Dist,CountDist;
	Dist = -1000;
	float tmpy = -1000;
	CountDist = -1000;
	bool tmpiscom = true;
	bool is_find = false;

		for(int id = 1; id < Arr.size();id++)
		{
			ArrCollision[id]->CountCom = 0;

				if(Arr[id]->IsOpen)
					Arr[id]->GetIntersectedRayY(pos,ArrCollision[id]->Arr,&(ArrCollision[id]->CountCom),Arr[id]->ArrSegments);
			DWORD tmpcount = ArrCollision[id]->CountCom;

				for(DWORD k=0;k<tmpcount && tmpiscom;k++)
				{	
						for(DWORD group=0;group<ArrCollision[id]->Arr[k]->CountSubSet && tmpiscom;group++)
						{
								for(DWORD numpoly=0;numpoly<ArrCollision[id]->Arr[k]->CountPoly[group]*3 && tmpiscom;numpoly+=3)
								{
									Dist = -1000.f;
									//находим все 3 вершины
									jvec0 = Arr[id]->ArrMeshVertex[ArrCollision[id]->Arr[k]->ArrPoly[group][numpoly]];
									jvec1 = Arr[id]->ArrMeshVertex[ArrCollision[id]->Arr[k]->ArrPoly[group][numpoly+1]];
									jvec2 = Arr[id]->ArrMeshVertex[ArrCollision[id]->Arr[k]->ArrPoly[group][numpoly+2]];

									Hit = D3DXIntersectTri(	&(jvec0.operator D3DXVECTOR3()),
																		&(jvec1.operator D3DXVECTOR3()),
																		&(jvec2.operator D3DXVECTOR3()),
																		&(jpos), 
																		&jvevyn, &u, &v, &Dist);

										if(Hit == true && tmpy < (pos->y-Dist*1000))
										{
											tmpy = pos->y-Dist*1000;
											is_find = true;
										}
										else
										{
											Hit = D3DXIntersectTri(	&(jvec0.operator D3DXVECTOR3()),
																			&(jvec1.operator D3DXVECTOR3()),
																			&(jvec2.operator D3DXVECTOR3()),
																			&(jpos), 
																			&jvevyp, &u, &v, &Dist);
												if(Hit == true)
												{
													pos->y+=Dist*1000;
													tmpy = pos->y;
													is_find = true;
													tmpiscom = false;
												}
										}
								}
						}
				}
		}
	pos->y = tmpy;
	return is_find;
}

bool ManagerStaticModel::GetIntersectedRayYRainBB(float3* pos)
{
	BOOL  Hit;
	float u, v,Dist,CountDist;
	Dist = -1000;
	float tmpy = -1000;
	CountDist = -1000;
	bool tmpiscom = true;
	bool is_find = false;

		for(int id = 1; id < Arr.size() && tmpiscom;id++)
		{
			Arr[id]->ArrSegments->BoundVolumeP.GetMinMax(&jvec0,&jvec1);

				if(Arr[id]->IsOpen && pos->x >= jvec0.x && pos->z >= jvec0.z && pos->x <= jvec1.x && pos->z <= jvec1.z)
				{		
						/*if(Arr[id]->Lod)
						{
							
						}
						else*/
						{
								if(jvec1.y >= pos->y)
								{
									tmpy = jvec1.y;
									tmpiscom = false;
								}
								else if(tmpy < jvec1.y)
									tmpy = jvec1.y;
							is_find = true;
						}
				}
		}
	pos->y = tmpy;
	return is_find;
}

void ManagerStaticModel::GetAIQuad(float3_t* max,float3_t* min,AIQuad** quad)
{
	/*float3 tmpmax,tmpmin;
	bool tmpiscom = true;

		for(int id=0;id<Arr.size();id++)
		{
				if(ArrIsSelected[id])
				{
					Arr[id]->ArrSegments->BoundVolumeP.GetMinMax(&tmpmin,&tmpmax);
						if(tmpmax.x >= (*quad)->pos.x && tmpmax.z >= (*quad)->pos.z && tmpmin.x <= (*quad)->pos.x && tmpmin.z <= (*quad)->pos.z)
						{
							tmpiscom = false;
							(*quad)->IsClose = true;			
						}
				}
		}*/

		/*else
		{
			//(*quad)->pos.y += 0.5;
				float3 tmpMin,tmpMax,bias;
				tmpMin = float3(-0.35,0,-0.35);// + (*quad)->pos;
				tmpMax = float3(0.35,1.8,0.35);// + (*quad)->pos;
				
				bias = TestCollision(tmpMin, tmpMax, Arr, ArrCollision);
			//(*quad)->pos.y -= 0.5;
				/*if(bias.y != 0)
				{
					(*quad)->pos.y += bias.y - 2;
					tmpMin = float3(-0.35,-1.8,-0.35) + (*quad)->pos;
					tmpMax = float3(0.35,0,0.35) + (*quad)->pos;
				}*/
				//(*quad)->pos.y += bias.y;
			//
					/*if(long(bias.x * 1000) != 0 || long(bias.z * 1000) != 0)
					{
						(*quad)->IsClose = true;	
					}
		}*/

		/*else
		{
			(*quad)->IsClose = true;
		}*/


		if(!(*quad)->IsClose)
		{

	float3 tmpMin,tmpMax,bias;
	(*quad)->pos.y += AIGRIDSIZEDIV2;
	tmpMin = float3(-AIGRIDSIZEDIV2,0,-AIGRIDSIZEDIV2) + float3((*quad)->pos.x,(*quad)->pos.y,(*quad)->pos.z);//(*quad)->pos.GetFloat3_t();
	tmpMax = float3(AIGRIDSIZEDIV2,1.5,AIGRIDSIZEDIV2) + float3((*quad)->pos.x,(*quad)->pos.y,(*quad)->pos.z);//(*quad)->pos.GetFloat3_t();

	bias = TestCollision(tmpMin, tmpMax, Arr, ArrCollision);
		
		if(long(bias.y * 1000.f) == 0)
		{
				for(int i=0;i<5;i++)
				{
					tmpMin.y -= 0.2f;
					tmpMax.y -= 0.2f;
					//(*quad)->pos.SetF(1,(*quad)->pos.GetF(1)-0.2f);
					(*quad)->pos.y -= 0.2f;
					bias = TestCollision(tmpMin, tmpMax, Arr, ArrCollision);
						if(long(bias.y * 1000.f) != 0)
							break;		
				}
			(*quad)->pos.y -= AIGRIDSIZEDIV2;
				if(long(bias.y * 1000.f) != 0)
				{
					//(*quad)->pos.SetF(1,(*quad)->pos.GetF(1)+bias.y);
					(*quad)->pos.y += bias.y;
						if(long(bias.x * 1000.f) != 0 || long(bias.z * 1000.f) != 0)
						{
							(*quad)->IsClose = true;	
						}
				}
				else
				{
					(*quad)->IsClose = true;
				}
		}
		else if(long(bias.y * 1000.f) > 0)
		{
				if(bias.y < AIGRIDMAXHEIGHTQUADS)
				{
					//(*quad)->pos.SetF(1,(*quad)->pos.GetF(1)+bias.y);
					(*quad)->pos.y += bias.y;
					tmpMin.y += bias.y;
					tmpMax.y += bias.y;
					bias = TestCollision(tmpMin, tmpMax, Arr, ArrCollision);
					//(*quad)->pos.Set(1,(*quad)->pos.Get(1)-LAIGRIDSIZEDIV2);
					(*quad)->pos.y -= AIGRIDSIZEDIV2;
					//(*quad)->pos.SetF(1,(*quad)->pos.GetF(1)+bias.y);
					(*quad)->pos.y += bias.y;
						if(long(bias.x * 1000.f) != 0 || long(bias.z * 1000.f) != 0)
						{
							(*quad)->IsClose = true;
						}
				}
				else
					(*quad)->IsClose = true;
		}
		else
		{
			(*quad)->IsClose = true;
			Core::InLog("qwerqwerqwer\n");
		}

		}
		

		if((*quad)->IsClose)
			Core::InLog("qwerqwerqwer\n");
}


bool ManagerStaticModel::GetIntersected(float3* pos)
{	
	float3 tmpMin,tmpMax,bias;
	tmpMin = Core::Data::NavigateCurMin;
	tmpMax = Core::Data::NavigateCurMax;
	
	bias = TestCollision(tmpMin, tmpMax, Arr, ArrCollision);
//	sprintf(g_szText, "%.4f %.4f %.4f\n%.4f %.4f %.4f", bias.x, bias.y, bias.z, mc.x, mc.y, mc.z);
	Core::Data::NavigateCurMax += bias;
	Core::Data::NavigateCurMin += bias;
	*pos += bias;
	return (bias.y != 0.f);
}

ManagerStaticModel::ManagerStaticModel()
{
	NumSelModel = -1;

	jvevyn = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);
	jvevyp = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
}

ManagerStaticModel::~ManagerStaticModel()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			nmem_delete(Arr[i]);
			nmem_delete_a(ArrComForCamera[i]->Arr);
			nmem_delete(ArrComForCamera[i]);
			nmem_delete(ArrCollision[i]);
		}

		for(DWORD i=0;i<ArrFrustums.size();i++)
		{
				for(DWORD k=0;k<Arr.size();k++)
				{
					nmem_delete(ArrFrustums[i][k]);
				}
		}

	Arr.clear();
	ArrComForCamera.clear();
	ArrCollision.clear();
	ArrFrustums.clear();
}


void ManagerStaticModel::OnLostDevice()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			Arr[i]->OnLostDevice();
		}
}

void ManagerStaticModel::OnResetDevice()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			Arr[i]->OnResetDevice();
		}
}

void ManagerStaticModel::Add(const char* name,const char* path,int type_segmentation,DWORD count_poly_in_segment,const char* lod,float dist_for_lod,float3* pos,float3* rot,bool is_open)
{
	SegmentationModel* tmpMesh = new SegmentationModel();
	tmpMesh->Load(path,is_open);
		if(StrValidate(lod))
			tmpMesh->LoadLod(lod);
	tmpMesh->DistForLod = dist_for_lod;
	tmpMesh->Segmentation(type_segmentation,count_poly_in_segment);
	tmpMesh->SetName(name);
	tmpMesh->SetPosition(pos);
	tmpMesh->SetRotation(rot);

	Arr.push_back(tmpMesh);

	InfoRenderSegments* tmpIRS = new InfoRenderSegments();
	tmpIRS->Count = tmpMesh->GetCountSplitsRender();
	tmpIRS->Arr = new Segment*[tmpMesh->GetCountSplitsRender()];
	tmpIRS->CountCom = 0;

	ArrComForCamera.push_back(tmpIRS);


	InfoRenderSegments* tmpIRS2 = new InfoRenderSegments();
	tmpIRS2->Count = tmpMesh->GetCountSplitsRender();
	tmpIRS2->Arr = new Segment*[tmpMesh->GetCountSplitsRender()];
	tmpIRS2->CountCom = 0;

	ArrCollision.push_back(tmpIRS2);

		for(DWORD i=0;i<ArrFrustums.size();i++)
		{
			InfoRenderSegments* tmpIRS3 = new InfoRenderSegments();
			tmpIRS3->Count = tmpMesh->GetCountSplitsRender();
			tmpIRS3->Arr = new Segment*[tmpMesh->GetCountSplitsRender()];
			tmpIRS3->CountCom = 0;

			ArrFrustums[i].push_back(tmpIRS3);
		}
	ArrIsSelected.push_back(false);
}

DWORD ManagerStaticModel::AddFrustum()
{
	UINT id = ArrFrustums.size();
	Array<InfoRenderSegments*> tmpArr;
	ArrFrustums.push_back(tmpArr);
	return id;
}

void ManagerStaticModel::AllComForCamera(float3* campos)
{
	float4x4 mat = Core::Data::View * Core::Data::ProjectionStencil;
	float3 sizemapdepth = float3(Core::Data::WidthHeight.x * Core::Data::CoefSizeOC,Core::Data::WidthHeight.y * Core::Data::CoefSizeOC,Core::Data::NearFar.y);
	float tmpdepth;
	float3 center,center2;
	float radius;

	int tmpkey;
	DWORD sizewarrdepth = Core::Data::WidthHeight.x * Core::Data::CoefSizeOC;
	DWORD sizeharrdepth = Core::Data::WidthHeight.y * Core::Data::CoefSizeOC;
	PosBBScreen res;

	//memset(ArrayWSIDs,0,sizeof(bool) * ArrayWSIDs.size());
	memset(&(ArrayWSIDs[0]), 0, ArrayWSIDs.size() / sizeof(DWORD));
		/*for(DWORD i=0;i<ArrayWSIDs.size();i++)
		{
			ArrayWSIDs[i] = false;
		}*/

		for(DWORD i=0;i<Arr.size();i++)
		{
			//обнуляем видимость каждой подгруппы
			memset(Arr[i]->IsVisibleGroup,0,sizeof(bool) * Arr[i]->Data->SubsetCount);
			memset(Arr[i]->ArrSerialNum,0,sizeof(bool) * Arr[i]->SplitsIDsRender);
			Arr[i]->CalculateWorld();
				//если модель видна камере
				if(Arr[i]->IsVisible(Core::Data::ObjCamera->ObjFrustum))
				{
					
					Arr[i]->ArrSegments->BoundVolumeP.GetPosBBScreen(&res,campos,&sizemapdepth,&(Arr[i]->World/*MatWorld*/ * mat));
					
						if(Core::Data::Settings::IsUpdateCamera)
							Arr[i]->ArrSegments->IsRenderForCamera = false;
						
					tmpdepth = Core::Data::CurrentMaxDepth2;
						//если минимальная глубина модели меньше ем максимальная глубина сцены
						if(tmpdepth > res.mindepth*Core::Data::NearFar.y)
						{
							Arr[i]->ArrSegments->BoundVolumeP.GetSphere(&center2,&radius);
							center = SMVector3Transform(center2,Arr[i]->/*MatWorld*/World);
							//определяем расстояние до камеры
							Arr[i]->ArrSegments->DistForCamera = SMVector3Length((center - Core::Data::ConstCurrentCameraPosition)) - radius;;//res.mindepth*Core::Data::NearFar.y;

								if(Arr[i]->ArrSegments->DistForCamera < 0.f)
									Arr[i]->ArrSegments->DistForCamera = 0.f;

								if(Arr[i]->ArrSegments->DistForCamera > Arr[i]->DistForLod && Arr[i]->IsLoded)
									Arr[i]->IsCurrentRenderLod = true;
								else
									Arr[i]->IsCurrentRenderLod = false;


								if(res.x < 0 && res.x + res.width > 0)
								{
									res.width = res.width + res.x;
									res.x = 0;
								}

								if(res.y < 0 && res.y + res.height > 0)
								{
									res.height = res.height + res.y;
									res.y = 0;
								}

								if(res.IsIn)
								{
									if(Core::Data::Settings::IsUpdateCamera)
									Arr[i]->ArrSegments->IsRenderForCamera = true;
								}
								else if(res.maxdepth > 0.f)
								{
										for(int x=res.x;x<res.width+res.x;x++)
										{
												for(int y=res.y;y<res.height+res.y;y++)
												{
													tmpkey = ((y) * sizewarrdepth) + (x);
													tmpdepth = Core::Data::ArrDepthOC[tmpkey];

														if(tmpdepth >= res.mindepth)
														{
															x=res.width+res.x;
															y=res.height+res.y;

																if(Core::Data::Settings::IsUpdateCamera)
																	Arr[i]->ArrSegments->IsRenderForCamera = true;
														}
												}
										}

										if(!(Arr[i]->ArrSegments->IsRenderForCamera))
											Core::Data::Statistics::CountModelExcluded++;
								}
						}

						//если объект виден, но не делен тогда говорим что все его подгруппы видны
						if(Arr[i]->ArrSegments->IsRenderForCamera && !Arr[i]->ArrSegments->BFNonEnd)
						{
								for(DWORD j=0;j<Arr[i]->Data->SubsetCount;j++)
								{
										if(Arr[i]->ArrReflection[j])
											Arr[i]->IsVisibleGroup[j] = true;
								}

							ArrComForCamera[i]->Arr[0] = Arr[i]->ArrSegments;
							ArrComForCamera[i]->CountCom = 1;
						}
						
						//еси объект виден и сейчас будет рисоваться полная модель а не лод и делен тогда проверяем видимость каждого сплита
						if(Arr[i]->ArrSegments->IsRenderForCamera && !(Arr[i]->IsCurrentRenderLod) && Arr[i]->ArrSegments->BFNonEnd)
						{
							//Arr[i]->ComMatrixTrans();
							
							ArrComForCamera[i]->CountCom = 0;
							Arr[i]->CPUFillingArrIndeces(Core::Data::ObjCamera->ObjFrustum,ArrComForCamera[i]->Arr,&(ArrComForCamera[i]->CountCom),campos,true);

							DWORD tmptmpcount = ArrComForCamera[i]->CountCom;
							int werwer=0;

								for(DWORD k=0;k<ArrComForCamera[i]->CountCom;k++)
								{
									ArrComForCamera[i]->Arr[k]->BoundVolumeP.GetPosBBScreen(&res,campos,&sizemapdepth,&(Arr[i]->/*MatWorld*/World * mat));
										if(Core::Data::Settings::IsUpdateCamera)
											ArrComForCamera[i]->Arr[k]->IsRenderForCamera = false;

									//ArrComForCamera[i]->Arr[k]->IsRenderForCamera = true;

									tmpdepth = Core::Data::CurrentMaxDepth;
										if(tmpdepth > res.mindepth*Core::Data::NearFar.y)
										{
											//ArrComForCamera[i]->Arr[k]->IsRenderForCamera = true;
												if(res.x < 0 && res.x + res.width > 0)
												{
													res.width = res.width + res.x;
													res.x = 0;
												}

												if(res.y < 0 && res.y + res.height > 0)
												{
													res.height = res.height + res.y;
													res.y = 0;
												}


												if(res.IsIn)
												{
													if(Core::Data::Settings::IsUpdateCamera)
													ArrComForCamera[i]->Arr[k]->IsRenderForCamera = true;
												}
												else if(res.maxdepth > 0.f)
												{
														for(int x=res.x;x<res.width+res.x;x++)
														{
																for(int y=res.y;y<res.height+res.y;y++)
																{
																	tmpkey = ((y) * sizewarrdepth) + (x);
																	tmpdepth = Core::Data::ArrDepthOC[tmpkey];
																						
																		if(tmpdepth >= res.mindepth)
																		{
																			x=res.width+res.x;
																			y=res.height+res.y;
																				if(Core::Data::Settings::IsUpdateCamera)
																					ArrComForCamera[i]->Arr[k]->IsRenderForCamera = true;
																		}
																}
														}
													//ArrComForCamera[i]->Arr[k]->IsRenderForCamera = true;
														if(!ArrComForCamera[i]->Arr[k]->IsRenderForCamera)
														{
															Core::Data::Statistics::CountModelSplitsExcluded++;
															int qwer = 0;
														}

														
												}

												//если объект видим то првоеряем какие подгруппы видны
												if(ArrComForCamera[i]->Arr[k]->IsRenderForCamera)
												{
														for(DWORD j=0;j<ArrComForCamera[i]->Arr[k]->CountSubSet;j++)
														{
																if(Arr[i]->ArrReflection[ArrComForCamera[i]->Arr[k]->NumberGroup[j]])
																	Arr[i]->IsVisibleGroup[ArrComForCamera[i]->Arr[k]->NumberGroup[j]] = true;
														}
												}
										}
								}
						}	
				}
		}
}

void ManagerStaticModel::RenderForCamera(DWORD timeDelta,bool render_alpha,bool ismaterial,bool is_old_render,bool render_forward)
{
		for(DWORD i=0;i<Arr.size();i++)
		{
				if(Arr[i]->ArrSegments->IsRenderForCamera)
				{
					DWORD issel = ((NumSelModel == i ? Core::Data::EngineID::Tex_SelectedMesh : -1) == -1 ? (ArrIsSelected[i] ? Core::Data::EngineID::Tex_SelectedMesh : -1) : -1);
					Arr[i]->GPURender(ArrComForCamera[i]->Arr,ArrComForCamera[i]->CountCom,true,render_alpha,ismaterial,is_old_render,false,render_forward,true,issel);
				}
		}
}


void ManagerStaticModel::AllCom(Core::ControllMoving::Frustum* frustum,DWORD id,bool is_com_not_open,float3* viewpos)
{
		if(id >= 0 && id < ArrFrustums.size())
		{
				for(DWORD i=0;i<Arr.size();i++)
				{
					ArrFrustums[id][i]->CountCom = 0;
						if(Arr[i]->GetOpen() || (is_com_not_open && !Arr[i]->GetOpen()))
							Arr[i]->CPUFillingArrIndeces(frustum,ArrFrustums[id][i]->Arr,&(ArrFrustums[id][i]->CountCom),viewpos,false);
				}
		}
}

void ManagerStaticModel::AllRender(DWORD id,DWORD timeDelta,int render_alpha,bool ismaterial,bool is_com_not_open,bool is_one_dip,bool render_forward)
{
		if(id >= 0 && id < ArrFrustums.size())
		{
				for(DWORD i=0;i<Arr.size();i++)
				{
					Segment** tmpsegment = ArrFrustums[id][i]->Arr;
					DWORD tmpcount = ArrFrustums[id][i]->CountCom;
						if(tmpcount > 0 && (Arr[i]->GetOpen() || (is_com_not_open && !Arr[i]->GetOpen())))
						{
							Arr[i]->GPURender(
								ArrFrustums[id][i]->Arr,
								ArrFrustums[id][i]->CountCom,
								true,render_alpha,ismaterial,false,is_one_dip,render_forward,false);
						}
				}
		}
}

bool ManagerStaticModel::GetOpen(DWORD id)
{
		if(id >= 0 && id < Arr.size())
		{
			return Arr[id]->GetOpen();
		}
}

void ManagerStaticModel::SetOpen(DWORD id,bool is_open)
{
		if(id >= 0 && id < Arr.size())
		{
			Arr[id]->SetOpen(is_open);
		}
}

DWORD ManagerStaticModel::GetCountPoly(DWORD id)
{
	return Arr[id]->AllCountPoly;
}

DWORD ManagerStaticModel::GetCount()
{
	return Arr.size();
}

DWORD ManagerStaticModel::GetSelected()
{
	return NumSelModel;
}

void ManagerStaticModel::SetSelected(DWORD id)
{
		//если в растительности разрешено выделение маски на модели, и разрешено выделение моделей
		if(SkyXEngine::Core::Data::Level::ArrGreen->GetSelectedMesh() && SkyXEngine::Core::Data::Settings::EditorsIsSelectedModel)
		{
				//если выделенная растиельность есть или есть идентификатор в растительности модели и он не равен текущему идентификатору выделенной модели
				if(SkyXEngine::Core::Data::Level::ArrGreen->GetSelected() == -1 || SkyXEngine::Core::Data::Level::ArrGreen->GetMeshIn(SkyXEngine::Core::Data::Level::ArrGreen->GetSelected()) != id)
					SkyXEngine::Core::Data::EngineID::Tex_SelectedMesh = SkyXEngine::Core::Data::EngineID::Tex_FreeSelectedMesh;	//просто выделяем модель
				else
					//иначе накладыаем маску на модель
					SkyXEngine::Core::Data::EngineID::Tex_SelectedMesh = SkyXEngine::Core::Data::Level::ArrGreen->GetTexMask(SkyXEngine::Core::Data::Level::ArrGreen->GetSelected());
		}
	NumSelModel = id;
}

void ManagerStaticModel::GetName(DWORD id,char* str)
{
	Arr[id]->GetName(str);
}

void ManagerStaticModel::SetName(DWORD id,const char* str)
{
	Arr[id]->SetName(str);
}

void ManagerStaticModel::GetMatPosition(DWORD id,float3* pos)
{
	Arr[id]->GetPosition(pos);
}

void ManagerStaticModel::GetMatRotation(DWORD id,float3* rot)
{
	Arr[id]->GetRotation(rot);
}

void ManagerStaticModel::SetMatPosition(DWORD id,float3* pos)
{
	Arr[id]->SetPosition(pos);
}

void ManagerStaticModel::SetMatRotation(DWORD id,float3* rot)
{
	Arr[id]->SetRotation(rot);
}

void ManagerStaticModel::ComMatrixTrans(DWORD id)
{
		if(id >= 0 && id < Arr.size())
			Arr[id]->CalculateWorld();
}

void ManagerStaticModel::Delete(DWORD id)
{
		if(id >= 0 && id < Arr.size())
		{
			nmem_delete(Arr[id]);
			Arr.erase(id);

			nmem_delete(ArrComForCamera[id]);
			ArrComForCamera.erase(id);

			nmem_delete(ArrCollision[id]);
			ArrCollision.erase(id);

				for(DWORD i=0;i<ArrFrustums.size();i++)
				{
					nmem_delete(ArrFrustums[i][id]);
					ArrFrustums[i].erase(id);
				}
		}
}

void ManagerStaticModel::Clear()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			this->Delete(0);
		}
}

inline int ManagerStaticModel::GetSysSpliting(DWORD id)
{
		if(id >= 0 && id < Arr.size())
			return Arr[id]->CountSplitsSys;
}

inline DWORD ManagerStaticModel::GetCountPolyInSplit(DWORD id)
{
		if(id >= 0 && id < Arr.size())
			return Arr[id]->CountPolyInSegment;
}

inline void ManagerStaticModel::GetModelPath(DWORD id,char* mpath)
{
		if(id >= 0 && id < Arr.size())
			sprintf(mpath,"%s",Arr[id]->PathForModel);
			
}

inline void ManagerStaticModel::GetLodPath(DWORD id,char* mpath)
{
		if(id >= 0 && id < Arr.size())
			sprintf(mpath,"%s",Arr[id]->PathForLod);
}

bool ManagerStaticModel::TraceBeam(const float3 & start, const float3 & dir, float3 * _res,DWORD* idmesh,int* type_mat,BOOL useEndPos)
{
triangle tmptri;
	float dist;
	bool tmpiscom = true;
	float3 ip;
	float3 res;
	float3 il;
	if(useEndPos)
	{
		res = dir;
		il = dir;
	}
	else
	{
		res = start + float3(10000.0f, 10000.0f, 10000.0f);
		il = dir * 10000.0f;
	}
	bool found = false;
 //Core::Data::tris.clear();

		for(DWORD i = 0; i < Arr.size() && tmpiscom; i++)
		{
			ArrCollision[i]->CountCom = 0;
			Arr[i]->GetPartFromBeam(&start, &dir, ArrCollision[i]->Arr, &(ArrCollision[i]->CountCom), Arr[i]->ArrSegments);
			DWORD tmpcount = ArrCollision[i]->CountCom;
			int qwert = 0;

				for(DWORD k = 0; k < tmpcount && tmpiscom; k++)
				{
						for(DWORD group = 0; group < ArrCollision[i]->Arr[k]->CountSubSet && tmpiscom; group++)
						{
							//triangle* tmptri = new triangle[ArrCollision[i]->Arr[k]->CountPoly[group]];
								for(DWORD numpoly = 0; numpoly < ArrCollision[i]->Arr[k]->CountPoly[group] * 3 && tmpiscom; numpoly += 3)
								{
									tmptri.a = Arr[i]->ArrMeshVertex[ArrCollision[i]->Arr[k]->ArrPoly[group][numpoly]];
									tmptri.b = Arr[i]->ArrMeshVertex[ArrCollision[i]->Arr[k]->ArrPoly[group][numpoly + 1]];
									tmptri.c = Arr[i]->ArrMeshVertex[ArrCollision[i]->Arr[k]->ArrPoly[group][numpoly + 2]];
									// Core::Data::tris.push_back(tmptri.a);
									// Core::Data::tris.push_back(tmptri.b);
									// Core::Data::tris.push_back(tmptri.c);

									/*dist = tmptri.DistanceFromPoint2(start);
									if(dist < r2)
									{
									}*/
										if(tmptri.IntersectLine(start, il, &ip))
										{
												if(SMVector3Length2(start - res) > SMVector3Length2(start - ip))
												{
													res = ip;
													found = true;
														if(idmesh)
														{
															*idmesh = i;
														}

														if(type_mat)
														{
															*type_mat = ArrCollision[i]->Arr[k]->TypeMaterial[group];
															//Core::InLog("TypeMaterial = %d\n",ArrCollision[i]->Arr[k]->TypeMaterial[group]);
														}
												}
										}
								}
						}
				}
		}
		if(found && _res)
		{
			*_res = res;
		}
	return(found);
}

bool ManagerStaticModel::TraceBeamY(const float3 & start, const float3 & dir, float3 * _res,DWORD* idmesh)
{
	triangle tmptri;
	float dist;
	bool tmpiscom = true;
	float3 ip;
	float3 res = start + float3(10000.0f, 10000.0f, 10000.0f);
	bool found = false;
 //Core::Data::tris.clear();

		for(DWORD i = 0; i < Arr.size() && tmpiscom; i++)
		{
			ArrCollision[i]->CountCom = 0;
			Arr[i]->GetPartFromBeam(&start, &dir, ArrCollision[i]->Arr, &(ArrCollision[i]->CountCom), Arr[i]->ArrSegments);
			DWORD tmpcount = ArrCollision[i]->CountCom;
			int qwert = 0;

				for(DWORD k = 0; k < tmpcount && tmpiscom; k++)
				{
						for(DWORD group = 0; group < ArrCollision[i]->Arr[k]->CountSubSet && tmpiscom; group++)
						{
							//triangle* tmptri = new triangle[ArrCollision[i]->Arr[k]->CountPoly[group]];
								for(DWORD numpoly = 0; numpoly < ArrCollision[i]->Arr[k]->CountPoly[group] * 3 && tmpiscom; numpoly += 3)
								{
									tmptri.a = Arr[i]->ArrMeshVertex[ArrCollision[i]->Arr[k]->ArrPoly[group][numpoly]];
									tmptri.b = Arr[i]->ArrMeshVertex[ArrCollision[i]->Arr[k]->ArrPoly[group][numpoly + 1]];
									tmptri.c = Arr[i]->ArrMeshVertex[ArrCollision[i]->Arr[k]->ArrPoly[group][numpoly + 2]];
									// Core::Data::tris.push_back(tmptri.a);
									// Core::Data::tris.push_back(tmptri.b);
									// Core::Data::tris.push_back(tmptri.c);

									/*dist = tmptri.DistanceFromPoint2(start);
									if(dist < r2)
									{
									}*/
										if(tmptri.IntersectLine(start, dir * 10000.0f, &ip))
										{
												if(abs((start.y) - (res.y)) > abs((start.y) - (ip.y)))
												{
													res = ip;
													found = true;
														if(idmesh)
															*idmesh = i;
												}
										}
								}
						}
				}
		}
		if(found && _res)
		{
			*_res = res;
		}
	return(found);
}

};
};
};