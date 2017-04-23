
#include <gcore\baseobject.h>

float4x4* SXTransObject::CalcWorld()
{
	World = SMMatrixScaling(Scale) * /*Rotation.GetMatrix()*/SMMatrixRotationX(Rotation.x) * SMMatrixRotationY(Rotation.y) * SMMatrixRotationZ(Rotation.z) * SMMatrixTranslation(Position);
	return &World;
}

////

void SXBound::CalcBound(IDirect3DVertexBuffer9* vertex_buffer, DWORD count_vert, DWORD bytepervert)
{
	BYTE *V = 0;
	HRESULT hr = 0;
	
		if (vertex_buffer && SUCCEEDED(vertex_buffer->Lock(0, 0, (void **)&V, 0)))
		{
			float3_t tmppos = *(float3_t*)((char*)(V) + bytepervert * 0);
			Max = tmppos;
			Min = tmppos;

				for(DWORD i=0;i<count_vert;i++)
				{
					float3_t pos = *(float3_t*)((char*)(V) + bytepervert * i);

						if(pos.x > Max.x)
							Max.x = pos.x;

						if(pos.y > Max.y)
							Max.y = pos.y;

						if(pos.z > Max.z)
							Max.z = pos.z;


						if(pos.x < Min.x)
							Min.x = pos.x;

						if(pos.y < Min.y)
							Min.y = pos.y;

						if(pos.z < Min.z)
							Min.z = pos.z;
				}
			vertex_buffer->Unlock();
		}

	Center = (Min + Max) * 0.5f;
	Radius = SMVector3Length(Center-Max);
}

float4x4* SXBound::CalcWorldAndTrans()
{
	CalcWorld();

	Max = SMVector3Transform(Max, World);
	Min = SMVector3Transform(Min, World);

	Center = (Min + Max) * 0.5f;

	Radius = SMVector3Length(Center - Max);
	return &World;
}

void SXBound::GetPosBBScreen(SXPosBBScreen *res, float3* campos, float3* sizemapdepth, float4x4* mat)
{
	float3 max,min;
	max = Max;
	min = Min;

	float2 mins,maxs,minmaxdepth;

	float4 point0 = float4(max.x,max.y,max.z,1.0f);
	float4 point1 = float4(max.x,max.y,min.z,1.0f);
	float4 point2 = float4(max.x,min.y,max.z,1.0f);
	float4 point3 = float4(min.x,max.y,max.z,1.0f);
	float4 point4 = float4(max.x,min.y,min.z,1.0f);
	float4 point5 = float4(min.x,min.y,max.z,1.0f);
	float4 point6 = float4(min.x,max.y,min.z,1.0f);
	float4 point7 = float4(min.x,min.y,min.z,1.0f);

	point0 = SMVector4Transform(point0,*mat);
	TRANSFORM_COORD_SCREEN2(point0,sizemapdepth);

	point1 = SMVector4Transform(point1,*mat);
	TRANSFORM_COORD_SCREEN2(point1,sizemapdepth);

	point2 = SMVector4Transform(point2,*mat);
	TRANSFORM_COORD_SCREEN2(point2,sizemapdepth);

	point3 = SMVector4Transform(point3,*mat);
	TRANSFORM_COORD_SCREEN2(point3,sizemapdepth);

	point4 = SMVector4Transform(point4,*mat);
	TRANSFORM_COORD_SCREEN2(point4,sizemapdepth);

	point5 = SMVector4Transform(point5,*mat);
	TRANSFORM_COORD_SCREEN2(point5,sizemapdepth);

	point6 = SMVector4Transform(point6,*mat);
	TRANSFORM_COORD_SCREEN2(point6,sizemapdepth);

	point7 = SMVector4Transform(point7,*mat);
	TRANSFORM_COORD_SCREEN2(point7,sizemapdepth);

	maxs.x = point0.x;
	maxs.y = point0.y;

	mins.x = point0.x;
	mins.y = point0.y;

	minmaxdepth.x = point0.z;
	minmaxdepth.y = point0.z;

		if(point1.x > maxs.x)
			maxs.x = point1.x;
		if(point2.x > maxs.x)
			maxs.x = point2.x;
		if(point3.x > maxs.x)
			maxs.x = point3.x;
		if(point4.x > maxs.x)
			maxs.x = point4.x;
		if(point5.x > maxs.x)
			maxs.x = point5.x;
		if(point6.x > maxs.x)
			maxs.x = point6.x;
		if(point7.x > maxs.x)
			maxs.x = point7.x;

		if(point1.y > maxs.y)
			maxs.y = point1.y;
		if(point2.y > maxs.y)
			maxs.y = point2.y;
		if(point3.y > maxs.y)
			maxs.y = point3.y;
		if(point4.y > maxs.y)
			maxs.y = point4.y;
		if(point5.y > maxs.y)
			maxs.y = point5.y;
		if(point6.y > maxs.y)
			maxs.y = point6.y;
		if(point7.y > maxs.y)
			maxs.y = point7.y;




		if(point1.x < mins.x)
			mins.x = point1.x;
		if(point2.x < mins.x)
			mins.x = point2.x;
		if(point3.x < mins.x)
			mins.x = point3.x;
		if(point4.x < mins.x)
			mins.x = point4.x;
		if(point5.x < mins.x)
			mins.x = point5.x;
		if(point6.x < mins.x)
			mins.x = point6.x;
		if(point7.x < mins.x)
			mins.x = point7.x;

		if(point1.y < mins.y)
			mins.y = point1.y;
		if(point2.y < mins.y)
			mins.y = point2.y;
		if(point3.y < mins.y)
			mins.y = point3.y;
		if(point4.y < mins.y)
			mins.y = point4.y;
		if(point5.y < mins.y)
			mins.y = point5.y;
		if(point6.y < mins.y)
			mins.y = point6.y;
		if(point7.y < mins.y)
			mins.y = point7.y;


		if(point1.z > minmaxdepth.y)
			minmaxdepth.y = point1.z;
		if(point2.z > minmaxdepth.y)
			minmaxdepth.y = point2.z;
		if(point3.z > minmaxdepth.y)
			minmaxdepth.y = point3.z;
		if(point4.z > minmaxdepth.y)
			minmaxdepth.y = point4.z;
		if(point5.z > minmaxdepth.y)
			minmaxdepth.y = point5.z;
		if(point6.z > minmaxdepth.y)
			minmaxdepth.y = point6.z;
		if(point7.z > minmaxdepth.y)
			minmaxdepth.y = point7.z;

		if(point1.z < minmaxdepth.x)
			minmaxdepth.x = point1.z;
		if(point2.z < minmaxdepth.x)
			minmaxdepth.x = point2.z;
		if(point3.z < minmaxdepth.x)
			minmaxdepth.x = point3.z;
		if(point4.z < minmaxdepth.x)
			minmaxdepth.x = point4.z;
		if(point5.z < minmaxdepth.x)
			minmaxdepth.x = point5.z;
		if(point6.z < minmaxdepth.x)
			minmaxdepth.x = point6.z;
		if(point7.z < minmaxdepth.x)
			minmaxdepth.x = point7.z;

	mins.x = (mins.x);
	mins.y = (mins.y);

	maxs.x = (maxs.x);
	maxs.y = (maxs.y);

		if(mins.x < 0.f)
			mins.x = 0.f;

		if(mins.y < 0.f)
			mins.y = 0.f;

		if(maxs.x > sizemapdepth->x)
			maxs.x = sizemapdepth->x;

		if(maxs.y > sizemapdepth->y)
			maxs.y = sizemapdepth->y;

	res->x = (mins.x);
	res->y = (mins.y);

	res->width = (maxs.x-mins.x);
	res->height = (maxs.y-mins.y);


	res->mindepth = minmaxdepth.x;
	res->maxdepth = minmaxdepth.y;

		if(res->mindepth < 0.f)
			res->mindepth = 0.f;

	res->IsVisible = true;

		/*if(campos->x > Min.x && campos->y > Min.y && campos->z > Min.z   &&   campos->x < Max.x && campos->y < Max.y && campos->z < Max.z)
			res->IsIn = true;*/
}

void SXBound::SetMinMax(float3* min, float3* max)
{
	Min = *min;
	Max = *max;

	/*float3 vec = (Max - Min) * 0.5f;
	Radius = sqrt(vec.x * vec.x + vec.y * vec.y + vec.x * vec.z);*/

	Center = (Min + Max) * 0.5f;
	Radius = SMVector3Length(Center - Max);
};

void SXBound::GetMinMax(float3* min, float3* max)
{
	*min = Min; *max = Max;
};

void SXBound::SetSphere(float3* center, float* radius)
{
	Center = *center;
	Radius = *radius;

	Min = Center - float3(Radius, Radius, Radius);
	Max = Center + float3(Radius, Radius, Radius);
};

void SXBound::GetSphere(float3* center, float* radius) const
{
	*center = Center;
	*radius = Radius;
};

bool SXBound::IsPointInSphere(float3* point)
{
	float distsqr = SMVector3Dot(Center - *point);
	if (distsqr <= Radius*Radius)
		return true;
	else
		return false;
}

bool SXBound::IsPointInBox(float3* point)
{
	if (point->x >= Min.x && point->y >= Min.y && point->z >= Min.z && point->x <= Max.x && point->y <= Max.y && point->z <= Max.z)
		return true;
	else
		return false;
}