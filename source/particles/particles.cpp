

namespace SkyXEngine
{
namespace Graphics
{
namespace Particles
{

EffParticles::EffParticles()
{
	OldTime = 0;
	ComOutBound = 0;
	IsBillboard = false;

	OldRotAngleX = OldRotAngleY = OldRotAngleZ = 0;
	RotAngleX = RotAngleY = RotAngleZ = 0;
	RotXHow = RotYHow = RotZHow = 0;

	AngleCircle = 0.0;

	RandOfAcceleration = 0;
	DispersionOfAcceleration = 0;
	Acceleration = float3(0,0,0);
	AngleRotateZ = 0;

	SizeParticle = float2(1,1);
	DispersionOfSize = 0;

	CharacterRotate = false;
	AngleRotate = 0;
	RandRotate = 0;
	DispersionOfRotate = 0;
	AlphaDisplay = 0;
	IsDependenceSizeAge = 0;

	HowSpawn = 0;
	DispersionOfOrigin = 0.f;
	IsRandOrigin = false;
	Origin = float3(0,0,0);

	TimeSpawn = 0;
	DistTimeSpawn = 0;

	TimeNextSpawnParticle = 0;

	CharacterCircle = false;
	CharacterRotate = false;
	CharacterMotion = false;

	RateAnimTex = DispersionOfRate = 0;

	IsSoft = false;
	SoftCoef = 1.f;
	IsRefraction = false;
	RefractionCoef = 1.f;
	IsLighting = false;
	LightingCoefAmbient = 0.1f;
	LightingCoefIntens = 0.3f;
}

EffParticles::~EffParticles()
{
	nmem_delete_a(Arr);
	ndx_release(TransVertBuf);
	ndx_release(VertexBuff);
	ndx_release(IndexBuff);
}

inline void EffParticles::SetOutOrigin(float3* origin,bool is_rand,float dispersion)
{
	Origin = *origin;
	IsRandOrigin = is_rand;
	DispersionOfOrigin = dispersion;

		if(is_rand)
			HowSpawn = 1;
		else
			HowSpawn = 0;
}

inline void EffParticles::SetBoundCone(float3* begin,float3* end,float begin_r,float end_r)
{
	BeginCone = *begin;
	EndCone = *end;
	BeginRadiusCone = begin_r;
	EndRadiusCone = end_r;
	ComOutBound = 3;
}

inline void EffParticles::SetSize(float2* size,bool is_rand,float dispersion,int depend)
{
	SizeParticle = *size;

		if(is_rand)
			DispersionOfSize = dispersion;
		else
			DispersionOfSize = false;

	IsDependenceSizeAge = depend;
}

inline void EffParticles::SetDependAlphaAge(int depend,float alpha_disp,WORD alpha_display)
{
	IsDependenceAlphaAge = depend;
	AlphaDispersion = alpha_disp;
	AlphaDisplay = alpha_display;
}

inline void EffParticles::SetTimeLife(DWORD time_life,bool is_rand,DWORD dispersion)
{
	TimeLife = time_life;

		if(is_rand)
			TimeLifeDispersion = dispersion;
		else
			TimeLifeDispersion = 0;
}

void EffParticles::SetTimeSpawn(WORD ttime,WORD dispersion_time)
{
	TimeSpawn = ttime;
	DistTimeSpawn = dispersion_time;
}


inline void EffParticles::SetRotateMarkX(int rot_how,float angle)
{
	IsBillboard = false;
	RotXHow = rot_how;
	RotAngleX = angle;
}

inline void EffParticles::SetRotateMarkY(int rot_how,float angle)
{
	IsBillboard = false;
	RotYHow = rot_how;
	RotAngleY = angle;
}

inline void EffParticles::SetRotateMarkZ(int rot_how,float angle)
{
	IsBillboard = false;
	RotZHow = rot_how;
	RotAngleZ = angle;
}

inline void EffParticles::SetRotateMarkBillboard(bool is_billboard)
{
	IsBillboard = is_billboard;
}


void EffParticles::SetBoundSphere(float3* center,float radius)
{
	ComOutBound = 1;
	BoundVolume.SetSphere(center,&radius);
}

void EffParticles::SetBoundBox(float3* center,float data_x,float data_y,float data_z)
{
	float3 min = *center;
	min.y -= data_y * 0.5f;
	min.x -= data_x * 0.5f;
	min.z -= data_z * 0.5f;

	float3 max = *center;
	max.y += data_y * 0.5f;
	max.x += data_x * 0.5f;
	max.z += data_z * 0.5f;

	BoundVolume.SetMinMax(&min,&max);

	ComOutBound = 2;
}


inline void EffParticles::SetVelocity(float3* veclocity,int is_rand,bool is_x_neg,bool is_y_neg,bool is_z_neg,float disp)
{
	Velocity = *veclocity;
	RandOfVelocity = is_rand;
	DispersionOfVelocity = disp;

	IsXNegVelocity = is_x_neg;
	IsYNegVelocity = is_y_neg;
	IsZNegVelocity = is_z_neg;
}


inline void EffParticles::SetAcceleration(float3* acceleration,int is_rand,bool is_x_neg,bool is_y_neg,bool is_z_neg,float disp)
{
	Acceleration = *acceleration;
	RandOfAcceleration = is_rand;
	DispersionOfAcceleration = disp;

	IsXNegAcceleration = is_x_neg;
	IsYNegAcceleration = is_y_neg;
	IsZNegAcceleration = is_z_neg;
}

//CHARACTER SET

inline void EffParticles::SetCharacterCircle(bool character_circle,float angle,int is_rand,float disp_angle)
{
	CharacterCircle = character_circle;
	AngleCircle = angle;
	RandCircle = is_rand;
	DispersionOfCircle = disp_angle;
}

inline void EffParticles::SetCharacterRotate(bool character_rotate,float angle,int is_rand,float disp_angle)
{
	CharacterRotate = character_rotate;
	AngleRotate = angle;
	RandRotate = is_rand;
	DispersionOfRotate = disp_angle;
}

inline void EffParticles::SetCharacterMotion(bool character_motion,float amplitude,float coef_angle,bool tap_x,bool tap_y,bool tap_z,int motion_axis,int is_rand,float disp,int delay,DWORD count_delay)
{
	CharacterMotion = character_motion;
	AmplitudeMotion = amplitude;
	CoefAngleMotion = coef_angle;
	RandMotion = is_rand;
	DispersionOfMotion = disp;
	DelayMotion = delay;
	CountDelayMls = count_delay;
	AxisMotion = motion_axis;

	TapMotionX = tap_x;
	TapMotionY = tap_y;
	TapMotionZ = tap_z;
}

////////////////////

void EffParticles::ReRun()
{
		for(int i=0;i<Count;i++)
		{
			Arr[i].IsAlife = false;
		}
	OldTime = 0;
	CreateParticles();
}

void EffParticles::ReInitArrParticles(int count_particles)
{
	Count = count_particles;
	nmem_delete_a(Arr);
	ndx_release(TransVertBuf);
	Arr = new CommonParticle[Count];

	Core::Data::Device->CreateVertexBuffer(
               Count * sizeof(CommonParticleDecl2),
               D3DUSAGE_WRITEONLY,
               0,
               D3DPOOL_MANAGED,
               &TransVertBuf,
               0);
}

void EffParticles::Create(int count_particles)
{
	Count = count_particles;
	Arr = new CommonParticle[Count];

	CreateParticles();

	Core::Data::Device->CreateVertexBuffer(
               Count * sizeof(CommonParticleDecl2),
               D3DUSAGE_WRITEONLY,
               0,
               D3DPOOL_MANAGED,
               &TransVertBuf,
               0);

	Core::Data::Device->CreateVertexBuffer(
               4 * sizeof(CommonParticleDecl),
               0,
               0,
               D3DPOOL_MANAGED,
               &VertexBuff,
               0);

	Core::Data::Device->CreateIndexBuffer(
               6 * sizeof(WORD),
               0,
               D3DFMT_INDEX16,
               D3DPOOL_MANAGED,
               &IndexBuff,
               0);


	CommonParticleDecl* vertices;
	VertexBuff->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = CommonParticleDecl(-0.5f, -0.5f, 0.f, 0.0f, 1.0f);
	vertices[1] = CommonParticleDecl(-0.5f,  0.5f, 0.f, 0.0f, 0.0f);
	vertices[2] = CommonParticleDecl( 0.5f,  0.5f, 0.f, 1.0f, 0.0f);
	vertices[3] = CommonParticleDecl( 0.5f, -0.5f, 0.f, 1.0f, 1.0f);

	/*vertices[0] = CommonParticleDecl(-1.f, -1.f, 0.f, 0.0f, AnimSizeCadr.w);
	vertices[1] = CommonParticleDecl(-1.f,  1.f, 0.f, 0.0f, 0.0f);
	vertices[2] = CommonParticleDecl( 1.f,  1.f, 0.f, AnimSizeCadr.z, 0.0f);
	vertices[3] = CommonParticleDecl( 1.f, -1.f, 0.f, AnimSizeCadr.z, AnimSizeCadr.w);*/

	VertexBuff->Unlock();


	WORD* indices = 0;
	IndexBuff->Lock(0, 0, (void**)&indices, 0);

	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;

	IndexBuff->Unlock();
}

void EffParticles::CreateParticles()
{
	CountReCreate2 = 0;
	CountLifeParticle = 0;
	IsAlife = true;

		for(int i=0;i<Count;i++)
		{
				if(abs(CountReCreate) > CountReCreate2)
				{
					ReCreateParticles(i);
					CountReCreate2++;
					CountLifeParticle++;
				}
				else
				{
					CountReCreate2 = 0;
					i=Count;
						if(TimeSpawn)
							TimeNextSpawnParticle = timeGetTime() + TimeSpawn;//(TimeSpawn + (DistTimeSpawn > 0 ? rand()%DistTimeSpawn : 0));
				}
		}

}

inline void EffParticles::ReCreateParticles(WORD id)
{
		//если разброс недопустим то спавним только в точке
		if(HowSpawn == 0)
		{
			Arr[id].Pos = Origin;
		}
		//разрешен рандомный спавн
		else if(HowSpawn == 1)
		{
				if(ComOutBound == 3)
				{
					Arr[id].Pos.y = SMRndf(BeginCone.y,EndCone.y);
					float tmplerp = (EndCone.y-Arr[id].Pos.y)/(EndCone.y - BeginCone.y);

					float tmpradius = lerp(EndRadiusCone,BeginRadiusCone,tmplerp);
					float3 tmpcoord = float3(lerp(EndCone.x,BeginCone.x,tmplerp),0,lerp(EndCone.z,BeginCone.z,tmplerp));
					
					Arr[id].Pos.x = SMRndf(tmpcoord.x - (tmpradius*0.5),tmpcoord.x + (tmpradius*0.5));
					Arr[id].Pos.z = SMRndf(tmpcoord.z - (tmpradius*0.5),tmpcoord.z + (tmpradius*0.5));
				}
				else if(ComOutBound == 2)
				{
					float3 tmpmin,tmpmax;
					BoundVolume.GetMinMax(&tmpmin,&tmpmax);

						if(BindCreateXNeg && BindCreateXPos)
							Arr[id].Pos.x = SMRndf(tmpmin.x,tmpmax.x);
						else if(BindCreateXNeg)
							Arr[id].Pos.x = tmpmin.x;
						else if(BindCreateXPos)
							Arr[id].Pos.x = tmpmax.x;
						else
							Arr[id].Pos.x = Origin.x;

						if(BindCreateYNeg && BindCreateYPos)
							Arr[id].Pos.y = SMRndf(tmpmin.y,tmpmax.y);
						else if(BindCreateYNeg)
							Arr[id].Pos.y = tmpmin.y;
						else if(BindCreateYPos)
							Arr[id].Pos.y = tmpmax.y;
						else
							Arr[id].Pos.y = Origin.y;

						if(BindCreateZNeg && BindCreateZPos)
							Arr[id].Pos.z = SMRndf(tmpmin.z,tmpmax.z);
						else if(BindCreateZNeg)
							Arr[id].Pos.z = tmpmin.z;
						else if(BindCreateZPos)
							Arr[id].Pos.z = tmpmax.z;
						else
							Arr[id].Pos.z = Origin.z;
				}
				else if(ComOutBound == 1)
				{
					float3 tmpcoord;
					float tmpradius;
					BoundVolume.GetSphere(&tmpcoord,&tmpradius);

					Arr[id].Pos.x = SMRndf(tmpcoord.x - (tmpradius*0.5),tmpcoord.x + (tmpradius*0.5));
					Arr[id].Pos.y = SMRndf(tmpcoord.y - (tmpradius*0.5),tmpcoord.y + (tmpradius*0.5));
					Arr[id].Pos.z = SMRndf(tmpcoord.z - (tmpradius*0.5),tmpcoord.z + (tmpradius*0.5));
				}
				else if(ComOutBound == 0)
				{
					Arr[id].Pos.x = SMRndf(Origin.x - (DispersionOfOrigin*0.5),Origin.x+(DispersionOfOrigin*0.5));
					Arr[id].Pos.z = SMRndf(Origin.z - (DispersionOfOrigin*0.5),Origin.z+(DispersionOfOrigin*0.5));
					Arr[id].Pos.y = SMRndf(Origin.y - (DispersionOfOrigin*0.5),Origin.y+(DispersionOfOrigin*0.5));
				}
		}

	Arr[id].CreatePos = Arr[id].Pos;
	Arr[id].IsAlife = true;
	Arr[id].IsAlifeAlpha = 1.f;

	Arr[id].Age = 0;
	Arr[id].TimeLife = TimeLife + (TimeLifeDispersion != 0 ? ((rand()%(TimeLifeDispersion/2)) - (TimeLifeDispersion/2)) : 0);//GetRandomFloat(5000,30000);

		if(CountCadrsX == 0 && CountCadrsY == 0)
		{
			Arr[id].AnimSizeCadrAndBias.x = Arr[id].AnimSizeCadrAndBias.y = 1.f;
			Arr[id].AnimSizeCadrAndBias.z = Arr[id].AnimSizeCadrAndBias.w = 0.f;
			Arr[id].RateMls = 0;
		}
		else
		{
			Arr[id].CurrentCadr = StartCadr + (DispersionOfStartCadr>0 ?((rand()%(DispersionOfStartCadr/2)) - (DispersionOfStartCadr/2)) : 0);
			Arr[id].CurrentMls = 0;
			Arr[id].RateMls = RateAnimTex + (DispersionOfRate>0 ? ((rand()%(DispersionOfRate/2)) - (DispersionOfRate/2)) : 0);

				if(Arr[id].CurrentCadr < CountCadrsX*CountCadrsY)
				{
					Arr[id].CurrentCadr++;

					WORD tmpy = Arr[id].CurrentCadr / CountCadrsX;
					WORD tmpx = Arr[id].CurrentCadr - (CountCadrsX * tmpy);
										
						if((int(Arr[id].CurrentCadr) - int(CountCadrsX * tmpy)) == 0)
							tmpy -= 1;

						if(tmpx == 0)
							tmpx = CountCadrsX;

					tmpx -= 1;

					Arr[id].AnimSizeCadrAndBias.x = AnimSizeCadr.z;
					Arr[id].AnimSizeCadrAndBias.y = AnimSizeCadr.w;
					Arr[id].AnimSizeCadrAndBias.z = (float(tmpx) * AnimSizeCadr.x) / AnimTexSize.x;
					Arr[id].AnimSizeCadrAndBias.w = (float(tmpy) * AnimSizeCadr.y) / AnimTexSize.y;
					//Core::InLog("CurrentCadr = %d,z = %.d | %f,w = %d | %f\n",Arr[i].CurrentCadr,tmpx,Arr[i].AnimSizeCadrAndBias.z,tmpy,Arr[i].AnimSizeCadrAndBias.w);
				}
		}

		//если не билборд
		if(!IsBillboard)
		{
			float4x4 world;
			float4x4 rot1;
			float4x4 rot2;
			float4x4 rot3;
				if(RotXHow == 1 && RotAngleX != 0)
					rot1 = SMMatrixRotationX(SMRndf(0,RotAngleX));
				else if(RotXHow == 2)
				{
					rot1 = SMMatrixRotationX(OldRotAngleX+RotAngleX);
					OldRotAngleX += RotAngleX;
				}
				else if(RotXHow == 0)
				{
					rot1 = SMMatrixRotationX(RotAngleX);
				}

				if(RotYHow == 1 && RotAngleY != 0)
					rot2 = SMMatrixRotationY(SMRndf(0,RotAngleY));
				else if(RotYHow == 2)
				{
					rot2 = SMMatrixRotationY(OldRotAngleY+RotAngleY);
					OldRotAngleY += RotAngleY;
				}
				else if(RotYHow == 0)
				{
					rot2 = SMMatrixRotationY(RotAngleY);
				}

				if(RotZHow == 1 && RotAngleZ != 0)
					rot3 = SMMatrixRotationZ(SMRndf(0,RotAngleZ));
				else if(RotZHow == 2)
				{
					rot3 = SMMatrixRotationZ(OldRotAngleZ+RotAngleZ);
					OldRotAngleZ += RotAngleZ;
				}
				else if(RotZHow == 0)
				{
					rot3 = SMMatrixRotationZ(RotAngleZ);
				}

			world = rot1 * rot2 * rot3;
			Arr[id].rot_1 = float3(world._11,world._12,world._13);
			Arr[id].rot_2 = float3(world._21,world._22,world._23);
			Arr[id].rot_3 = float3(world._31,world._32,world._33);
		}

	Arr[id].AlphaDispersion = SMRndf(-(AlphaDispersion*0.5f),AlphaDispersion*0.5f);

		if(DispersionOfSize > 0.f)
		{
			float tmprand = SMRndf(0.0,DispersionOfSize);
			Arr[id].Size.x = SizeParticle.x + tmprand;
			Arr[id].Size.y = SizeParticle.y + tmprand;

				if(IsDependenceSizeAge == 1)
				{
					Arr[id].Size.z = Arr[id].Size.x;
					Arr[id].Size.w = Arr[id].Size.y;
					Arr[id].Size.x -= SizeParticle.x + DispersionOfSize;
					Arr[id].Size.y -= SizeParticle.y + DispersionOfSize;
				}
		}
		else
		{
			Arr[id].Size.z = SizeParticle.x;
			Arr[id].Size.w = SizeParticle.y;
			Arr[id].Size.x = (IsDependenceSizeAge == 1 ? 0 : SizeParticle.x);
			Arr[id].Size.y = (IsDependenceSizeAge == 1 ? 0 : SizeParticle.y);
		}


		if(RandOfVelocity == 0)
		{
			Arr[id].Velocity.x = Velocity.x;
			Arr[id].Velocity.y = Velocity.y;
			Arr[id].Velocity.z = Velocity.z;
		}
		else if(RandOfVelocity == 1)
		{
				if(IsXNegVelocity)
					Arr[id].Velocity.x = Velocity.x + (SMRndf(-DispersionOfVelocity*0.5,DispersionOfVelocity*0.5));
				else
					Arr[id].Velocity.x = Velocity.x + (SMRndf(0,DispersionOfVelocity));

				if(IsYNegVelocity)
					Arr[id].Velocity.y = Velocity.y + (SMRndf(-DispersionOfVelocity*0.5,DispersionOfVelocity*0.5));
				else
					Arr[id].Velocity.y = Velocity.y + (SMRndf(0,DispersionOfVelocity));

				if(IsZNegVelocity)
					Arr[id].Velocity.z = Velocity.z + (SMRndf(-DispersionOfVelocity*0.5,DispersionOfVelocity*0.5));
				else
					Arr[id].Velocity.z = Velocity.z + (SMRndf(0,DispersionOfVelocity));
		}
		else if(RandOfVelocity == 2)
		{
				if(IsXNegVelocity)
					Arr[id].Velocity.x = SMRndf(-Velocity.x,Velocity.x);
				else
					Arr[id].Velocity.x = SMRndf(0,Velocity.x);

				if(IsYNegVelocity)
					Arr[id].Velocity.y = SMRndf(-Velocity.y,Velocity.y);
				else
					Arr[id].Velocity.y = SMRndf(0,Velocity.y);

				if(IsZNegVelocity)
					Arr[id].Velocity.z = SMRndf(-Velocity.z,Velocity.z);
				else
					Arr[id].Velocity.z = SMRndf(0,Velocity.z);
		}


		if(RandOfAcceleration == 0)
		{
			Arr[id].Acceleration.x = Acceleration.x;
			Arr[id].Acceleration.y = Acceleration.y;
			Arr[id].Acceleration.z = Acceleration.z;
		}
		else if(RandOfAcceleration == 1)
		{
				if(IsXNegAcceleration)
					Arr[id].Acceleration.x = Acceleration.x + (SMRndf(-DispersionOfAcceleration*0.5,DispersionOfAcceleration*0.5));
				else
					Arr[id].Acceleration.x = Acceleration.x + (SMRndf(0,DispersionOfAcceleration));

				if(IsYNegAcceleration)
					Arr[id].Acceleration.y = Acceleration.x + (SMRndf(-DispersionOfAcceleration*0.5,DispersionOfAcceleration*0.5));
				else
					Arr[id].Acceleration.y = Acceleration.x + (SMRndf(0,DispersionOfAcceleration));

				if(IsZNegAcceleration)
					Arr[id].Acceleration.z = Acceleration.z + (SMRndf(-DispersionOfAcceleration*0.5,DispersionOfAcceleration*0.5));
				else
					Arr[id].Acceleration.z = Acceleration.z + (SMRndf(0,DispersionOfAcceleration));
		}
		else if(RandOfAcceleration == 2)
		{
				if(IsXNegAcceleration)
					Arr[id].Acceleration.x = SMRndf(-Acceleration.x,Acceleration.x);
				else
					Arr[id].Acceleration.x = SMRndf(0,Acceleration.x);

				if(IsYNegAcceleration)
					Arr[id].Acceleration.y = SMRndf(-Acceleration.y,Acceleration.y);
				else
					Arr[id].Acceleration.y = SMRndf(0,Acceleration.y);

				if(IsZNegAcceleration)
					Arr[id].Acceleration.z = SMRndf(-Acceleration.z,Acceleration.z);
				else
					Arr[id].Acceleration.z = SMRndf(0,Acceleration.z);
		}
}

void EffParticles::SetAnimTexData(DWORD idtex,WORD count_cadrs_x,WORD count_cadrs_y,WORD rate,WORD dispersion,WORD start_cadr,WORD disp_of_cadr)
{
	IDTex = idtex;
	CountCadrsX = count_cadrs_x;
	CountCadrsY = count_cadrs_y;

	D3DSURFACE_DESC desc;
	Core::Data::LoadedTextures->GetTexture(idtex)->GetLevelDesc(0,&desc);

	AnimTexSize.x = desc.Width;
	AnimTexSize.y = desc.Height;

	AnimSizeCadr.x = (AnimTexSize.x / float(CountCadrsX));
	AnimSizeCadr.y = (AnimTexSize.y / float(CountCadrsY));
	AnimSizeCadr.z = AnimSizeCadr.x / AnimTexSize.x;
	AnimSizeCadr.w = AnimSizeCadr.y / AnimTexSize.y;

	RateAnimTex = rate;
	DispersionOfRate = dispersion;

	StartCadr = start_cadr;
	DispersionOfStartCadr = disp_of_cadr;
}

void EffParticles::UpdateAnimTex(WORD idparticle,DWORD tmptime)
{
		//если подошло время обновления анимации текстуры
		if(Arr[idparticle].RateMls <= Arr[idparticle].CurrentMls)
		{
			Arr[idparticle].CurrentMls = 0;

				//если номер текущего кадра меньше чем общее количество кадров, то обрабатываем
				if(Arr[idparticle].CurrentCadr < CountCadrsX*CountCadrsY)
				{
					Arr[idparticle].CurrentCadr++;

					WORD tmpy = Arr[idparticle].CurrentCadr / CountCadrsX;
					WORD tmpx = Arr[idparticle].CurrentCadr - (CountCadrsX * tmpy);
										
						if((int(Arr[idparticle].CurrentCadr) - int(CountCadrsX * tmpy)) == 0)
							tmpy -= 1;

						if(tmpx == 0)
							tmpx = CountCadrsX;

					tmpx -= 1;

					Arr[idparticle].AnimSizeCadrAndBias.x = AnimSizeCadr.z;
					Arr[idparticle].AnimSizeCadrAndBias.y = AnimSizeCadr.w;
					Arr[idparticle].AnimSizeCadrAndBias.z = (float(tmpx) * AnimSizeCadr.x) / AnimTexSize.x;
					Arr[idparticle].AnimSizeCadrAndBias.w = (float(tmpy) * AnimSizeCadr.y) / AnimTexSize.y;
					//Core::InLog("CurrentCadr = %d,z = %.d | %f,w = %d | %f\n",Arr[i].CurrentCadr,tmpx,Arr[i].AnimSizeCadrAndBias.z,tmpy,Arr[i].AnimSizeCadrAndBias.w);
				}
				else
					Arr[idparticle].CurrentCadr = 0;
		}
		else
		{
			Arr[idparticle].CurrentMls += tmptime - OldTime;
		}
}

inline bool EffParticles::IsPointInCone(float3* point)
{
		if(point->y >= BeginCone.y && point->y <= EndCone.y)
		{
			float tmplerp = (EndCone.y-point->y)/(EndCone.y - BeginCone.y);
			float tmpradius = lerp(EndRadiusCone,BeginRadiusCone,tmplerp) * 0.5f;
			float3 tmpcoord = float3(lerp(EndCone.x,BeginCone.x,tmplerp),0,lerp(EndCone.z,BeginCone.z,tmplerp));
				if(
					tmpcoord.x + tmpradius >= point->x && tmpcoord.x - tmpradius <= point->x &&
					tmpcoord.z + tmpradius >= point->z && tmpcoord.z - tmpradius <= point->z
					)
					return true;
		}

	return false;
}

void EffParticles::Compute()
{
	WORD tmpOldPercent=0,tmpNewPercent=0;
	CountReCreate2 = 0;
		if(OldTime > 0 && CountReCreate > 0 && CountReCreate <= Count - CountLifeParticle)
		{
			//DWORD tt = timeGetTime();
				if(timeGetTime() > TimeNextSpawnParticle)
				{
						for(int i=0;i<Count;i++)
						{
								if(!(Arr[i].IsAlife) && CountReCreate > CountReCreate2)
								{
									ReCreateParticles(i);
									CountReCreate2++;
								}
								else if(CountReCreate <= CountReCreate2)
								{
									CountReCreate2 = 0;
									i=Count;
										if(TimeSpawn)
											TimeNextSpawnParticle = timeGetTime() + (TimeSpawn + (DistTimeSpawn > 0 ? rand()%DistTimeSpawn : 0));
								}
						}
				}
			IsAlife = true;
		}

	DWORD tmptime = timeGetTime();
	CountLifeParticle = 0;

		for(int i=0;i<Count && OldTime != 0 && IsAlife;i++)
		{
				//если IsAlifeAlpha меньше либо равно 0 то частица мертва, отмечаем
				if(Arr[i].IsAlife && Arr[i].TimeLife > 0 && Arr[i].Age >= Arr[i].TimeLife)
					Arr[i].IsAlife = false;
				//иначе инкремент общего количества живых частиц
				else if(Arr[i].IsAlife)
				{
						if(ComOutBound == 1 && BoundVolume.IsPointInSphere(&Arr[i].Pos))
						{
							CountLifeParticle++;
						}
						else if(ComOutBound == 2 && BoundVolume.IsPointInBox(&Arr[i].Pos))
						{
							CountLifeParticle++;
						}
						else if(ComOutBound == 3 && this->IsPointInCone(&(Arr[i].Pos)))
						{
							CountLifeParticle++;
						}
						else if(ComOutBound != 0)
						{
							Arr[i].IsAlife = false;
						}
						else if(ComOutBound == 0)
						{
							CountLifeParticle++;
						}
				}


				//если частица жива то обрабатываем поведение
				if(Arr[i].IsAlife)
				{
					//обрабокта возраста
					Arr[i].Age += tmptime - OldTime;

					tmpNewPercent = ceil((1.f - (float(Arr[i].Age) / float(Arr[i].TimeLife))) * 100.f);
					tmpOldPercent = floor((1.f - (float(Arr[i].Age) / float(Arr[i].TimeLife))) * 100.f);

						if(IsDependenceAlphaAge == 1)
							Arr[i].IsAlifeAlpha = 1.f - (float(Arr[i].Age) / float(Arr[i].TimeLife));
						else if(IsDependenceAlphaAge == -1)
							Arr[i].IsAlifeAlpha = (float(Arr[i].Age) / float(Arr[i].TimeLife));
						else
							Arr[i].IsAlifeAlpha = 1;

						if(AlphaDisplay > Arr[i].Age)
						{
							Arr[i].IsAlifeAlpha *= float(Arr[i].Age) / float(AlphaDisplay);
						}

						if(IsDependenceSizeAge == 1)
						{
							Arr[i].Size.x = Arr[i].Size.z * (float(Arr[i].Age) / float(Arr[i].TimeLife));
							Arr[i].Size.y = Arr[i].Size.w * (float(Arr[i].Age) / float(Arr[i].TimeLife));
						}
						else if(IsDependenceSizeAge == -1)
						{
							Arr[i].Size.x = Arr[i].Size.z * (1.f - (float(Arr[i].Age) / float(Arr[i].TimeLife)));
							Arr[i].Size.y = Arr[i].Size.w * (1.f - (float(Arr[i].Age) / float(Arr[i].TimeLife)));
						}

					Arr[i].IsAlifeAlpha += Arr[i].AlphaDispersion;

						/*if(Arr[i].IsAlifeAlpha > 1.f)
							Arr[i].IsAlifeAlpha = 1.f;
						else if(Arr[i].IsAlifeAlpha < 0.f)
							Arr[i].IsAlifeAlpha = 0.f;*/

					float tmpcountsec = float(tmptime - OldTime) * 0.001f;
					//обработка скорости и ускорения
					Arr[i].Pos += Arr[i].Velocity * tmpcountsec + Arr[i].Acceleration*(float(Arr[i].Age) * tmpcountsec);

						//если назначена анимация текстуры то обрабатываем
						if(Arr[i].RateMls > 0)
							UpdateAnimTex(i,tmptime);

						//CHARACTER

						//движение по кругу
						if(CharacterCircle)
						{
							float tmpangle = 0;

								if(RandCircle == 1)
									tmpangle = SMRndf(-DispersionOfCircle*0.5,DispersionOfCircle*0.5);
								else if(RandCircle == 2)
									tmpangle = SMRndf(0.f,DispersionOfCircle);

							float tmpx = cosf(AngleCircle + tmpangle);
							float tmpz = sinf(AngleCircle + tmpangle);

							float tmpx2 = Arr[i].Pos.x * tmpx - Arr[i].Pos.z * tmpz;
							float tmpz2 = Arr[i].Pos.x * tmpz + Arr[i].Pos.z * tmpx;

							Arr[i].Pos.x = tmpx2;
							Arr[i].Pos.z = tmpz2;
						}

						//если это билборд и есть хараткер поворота
						if(IsBillboard && CharacterRotate)
						{
							float tmpangle = 0;

								if(RandRotate == 1)
									tmpangle = SMRndf(-DispersionOfRotate*0.5,DispersionOfRotate*0.5);
								else if(RandRotate == 2)
									tmpangle = SMRndf(0.f,DispersionOfRotate);

							float4x4 rot3 = SMMatrixRotationZ(AngleRotateZ+tmpangle);
							Arr[i].rot_1 = float3(rot3._11,rot3._12,rot3._13);
							Arr[i].rot_2 = float3(rot3._21,rot3._22,rot3._23);
							Arr[i].rot_3 = float3(rot3._31,rot3._32,rot3._33);

							AngleRotateZ += AngleRotate+tmpangle;

								if(AngleRotateZ >= 6.28)
									AngleRotateZ = AngleRotateZ - 6.28;
						}

						//если определен характер движения
						if(CharacterMotion)
						{
								//если определен волновой тип движения
								if(DelayMotion == 2)
								{
									float tmpdist = 0;

										//определяем ось для волны
										if(AxisMotion == 0)
											tmpdist = Arr[i].Pos.x;
										else if(AxisMotion == 1)
											tmpdist = Arr[i].Pos.y;
										else if(AxisMotion == 2)
											tmpdist = Arr[i].Pos.z;

										if(RandMotion == 1)
										{
											tmpdist += SMRndf(-DispersionOfMotion*0.5,DispersionOfMotion*0.5);
										}
										else if(RandMotion == 2)
										{
												if(AmplitudeMotion < 0)
													tmpdist += SMRndf(DispersionOfMotion,0);
												else
													tmpdist += SMRndf(0,DispersionOfMotion);
										}

									tmpdist *= CoefAngleMotion;

										if(TapMotionX != 0)
											Arr[i].DeviationVector.x = AmplitudeMotion * sinf(tmpdist);

										if(TapMotionY != 1)
											Arr[i].DeviationVector.y = AmplitudeMotion * sinf(tmpdist);

										if(TapMotionZ != 2)
											Arr[i].DeviationVector.z = AmplitudeMotion * sinf(tmpdist);

									Arr[i].Pos += Arr[i].DeviationVector;
								}


								//если пришло время обновления
								else if(CountDelayMls2 >= CountDelayMls)
								{
										if(DelayMotion == 0)
										{
												if(RandMotion == 1)
												{
														if(TapMotionX)
															Arr[i].DeviationVector.x = SMRndf(-AmplitudeMotion*0.5,AmplitudeMotion*0.5);
														else
															Arr[i].DeviationVector.x = 0;

														if(TapMotionY)
															Arr[i].DeviationVector.y = SMRndf(-AmplitudeMotion*0.5,AmplitudeMotion*0.5);
														else
															Arr[i].DeviationVector.y = 0;

														if(TapMotionZ)
															Arr[i].DeviationVector.z = SMRndf(-AmplitudeMotion*0.5,AmplitudeMotion*0.5);
														else
															Arr[i].DeviationVector.z = 0;
												}
												else if(RandMotion == 2)
												{
														if(AmplitudeMotion > 0)
														{
																if(TapMotionX)
																	Arr[i].DeviationVector.x = SMRndf(0,AmplitudeMotion);
																else
																	Arr[i].DeviationVector.x = 0;

																if(TapMotionY)
																	Arr[i].DeviationVector.y = SMRndf(0,AmplitudeMotion);
																else
																	Arr[i].DeviationVector.y = 0;

																if(TapMotionZ)
																	Arr[i].DeviationVector.z = SMRndf(0,AmplitudeMotion);
																else
																	Arr[i].DeviationVector.z = 0;
														}
														else
														{
																if(TapMotionX)
																	Arr[i].DeviationVector.x = SMRndf(AmplitudeMotion,0);
																else
																	Arr[i].DeviationVector.x = 0;

																if(TapMotionY)
																	Arr[i].DeviationVector.y = SMRndf(AmplitudeMotion,0);
																else
																	Arr[i].DeviationVector.y = 0;

																if(TapMotionZ)
																	Arr[i].DeviationVector.z = SMRndf(AmplitudeMotion,0);
																else
																	Arr[i].DeviationVector.z = 0;
														}
												}
									
										}
										else if(DelayMotion == 1)
										{
											AmplitudeMotion = -AmplitudeMotion;

												if(TapMotionX)
													Arr[i].DeviationVector.x = AmplitudeMotion;
												else
													Arr[i].DeviationVector.x = 0;

												if(TapMotionY)
													Arr[i].DeviationVector.y = AmplitudeMotion;
												else
													Arr[i].DeviationVector.y = 0;

												if(TapMotionZ)
													Arr[i].DeviationVector.z = AmplitudeMotion;
												else
													Arr[i].DeviationVector.z = 0;
										}

									CountDelayMls2 = 0;
								}
								else
								{
									Arr[i].Pos += Arr[i].DeviationVector * (float(CountDelayMls2)/float(CountDelayMls));
								}
							CountDelayMls2 += tmptime - OldTime;
						}
				}
				else
				{

				}
		}

		//все частицы отыграли свое
		if(OldTime != 0 && CountLifeParticle == 0)
			IsAlife = false;

	OldTime = tmptime;
}

void EffParticles::ComputeLighting()
{
		if(CountLifeParticle > 0 && IsLighting)
		{
			float4 tmpPosition;
			float4 tmpColor;
				for(DWORD i=0;i<Count;i++)
				{
						if(Arr[i].IsAlife)
						{
							Arr[i].Ambient.x = Arr[i].Ambient.y = Arr[i].Ambient.z = Arr[i].Ambient.w = 0;
								for(int k=0;k<Core::Data::Level::LightManager->Arr.size();k++)
								{
										if((Core::Data::Level::LightManager->Arr[k]->GetVisible() && Core::Data::Level::LightManager->Arr[k]->GetEnable()))
										{
											tmpColor = Core::Data::Level::LightManager->Arr[k]->Color;

												if(Core::Data::Level::LightManager->Arr[k]->IsGlobal)
												{
													//float tmpmi = tmpColor.w * (0.5 + (1.f-tmpColor.w)*0.3);
													Arr[i].Ambient.x += tmpColor.w * tmpColor.x * LightingCoefAmbient;
													Arr[i].Ambient.y += tmpColor.w * tmpColor.y * LightingCoefAmbient;
													Arr[i].Ambient.z += tmpColor.w * tmpColor.w * LightingCoefAmbient;
													Arr[i].Ambient.w += tmpColor.w * LightingCoefIntens;
												}
												else
												{
													Core::Data::Level::LightManager->Arr[k]->GetPositionSys(&tmpPosition);

													float dist = SMVector3Dot(Arr[i].Pos-float3(tmpPosition.x,tmpPosition.y,tmpPosition.z));
													float invdist = 1.f - (dist/(tmpPosition.w*tmpPosition.w));
														if(invdist > 1.f)
															invdist = 1.f;
														else if(invdist < 0.f)
															invdist = 0.f;
													Arr[i].Ambient.x += tmpColor.w *  invdist * tmpColor.x * LightingCoefAmbient;
													Arr[i].Ambient.y += tmpColor.w *  invdist * tmpColor.y * LightingCoefAmbient;
													Arr[i].Ambient.z += tmpColor.w *  invdist * tmpColor.z * LightingCoefAmbient;
													Arr[i].Ambient.w += tmpColor.w * invdist * LightingCoefIntens;
												}
										}
								}
						}
				}
		}
}

void EffParticles::Render(DWORD timeDelta,float4x4* worldmat,float4x4* matrot,float4x4* matpos)
{
		if(CountLifeParticle > 0)
		{
			CommonParticleDecl2* RTGPUArrVerteces;
			TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
			DWORD tmpcount = 0;
				for(DWORD i=0;i<Count;i++)
				{
						if(Arr[i].IsAlife)
						{
							RTGPUArrVerteces[tmpcount].pos = Arr[i].Pos;
							RTGPUArrVerteces[tmpcount].tex = Arr[i].AnimSizeCadrAndBias;
							RTGPUArrVerteces[tmpcount].alpha = Arr[i].IsAlifeAlpha;
							RTGPUArrVerteces[tmpcount].size.x = Arr[i].Size.x;
							RTGPUArrVerteces[tmpcount].size.y = Arr[i].Size.y;
							RTGPUArrVerteces[tmpcount].lighting.x = Arr[i].Ambient.x;
							RTGPUArrVerteces[tmpcount].lighting.y = Arr[i].Ambient.y;
							RTGPUArrVerteces[tmpcount].lighting.z = Arr[i].Ambient.z;
							RTGPUArrVerteces[tmpcount].lighting.w = Arr[i].Ambient.w;

								if(IsBillboard)
								{
									RTGPUArrVerteces[tmpcount].rot_1 = ConstPDRot_1;
									RTGPUArrVerteces[tmpcount].rot_2 = ConstPDRot_2;
									RTGPUArrVerteces[tmpcount].rot_3 = ConstPDRot_3;
								}
								else
								{
									RTGPUArrVerteces[tmpcount].rot_1 = Arr[i].rot_1;
									RTGPUArrVerteces[tmpcount].rot_2 = Arr[i].rot_2;
									RTGPUArrVerteces[tmpcount].rot_3 = Arr[i].rot_3;
								}
							tmpcount++;
						}
				}
	
			TransVertBuf->Unlock();

			Core::Data::Statistics::CountRenderParticles+=tmpcount;

			Core::Data::Device->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | tmpcount) );

			Core::Data::Device->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1) );
			Core::Data::Device->SetStreamSource(1, TransVertBuf, 0,  sizeof(CommonParticleDecl2) );

			Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationParticles);

			Core::Data::Device->SetStreamSource(0, VertexBuff, 0, sizeof(CommonParticleDecl));
			Core::Data::Device->SetIndices(IndexBuff);

			float4x4 world = ConstPDMatrixIdentity;
			float determ = 0;
			float4x4 invview = SMMatrixInverse(&determ,Core::Data::View);
				if(IsBillboard)
				{
					/*world._11 = Core::Data::View._11;
					world._12 = Core::Data::View._21;
					world._13 = Core::Data::View._31;
					world._44 = 1.0;*/
					world = invview;
					world._41 = world._42 = world._43 = 0;
				}

			//MatTransform = MatRotate*MatTranslation;
			float4x4 vp = /*MatTransform * */ Core::Data::View * Core::Data::ProjectionStencil;
			Core::Data::Device->SetTexture(0,SkyXEngine::Core::Data::LoadedTextures->GetTexture(IDTex));

			Core::Data::ShadersManager->Bind(0,"particles");

				if(IsSoft && !IsRefraction && !IsLighting)
				{
					Core::Data::ShadersManager->Bind(1,"particles_soft");
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft","SoftCoef",&SoftCoef);
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft","NearFar",&Core::Data::NearFar);
				}
				else if(IsSoft && IsRefraction && !IsLighting)
				{
					Core::Data::ShadersManager->Bind(1,"particles_soft_refraction");
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft_refraction","SoftCoef",&SoftCoef);
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft_refraction","NearFar",&Core::Data::NearFar);
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft_refraction","RefractCoef",&RefractionCoef);
				}
				else if(IsSoft && IsRefraction && IsLighting)
				{
					Core::Data::ShadersManager->Bind(1,"particles_soft_refraction_light");
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft_refraction_light","SoftCoef",&SoftCoef);
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft_refraction_light","NearFar",&Core::Data::NearFar);
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft_refraction_light","RefractCoef",&RefractionCoef);
				}
				else if(IsSoft && !IsRefraction && IsLighting)
				{
					Core::Data::ShadersManager->Bind(1,"particles_soft_light");
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft_light","SoftCoef",&SoftCoef);
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft_light","NearFar",&Core::Data::NearFar);
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_soft_light","RefractCoef",&RefractionCoef);
				}
				else if(!IsSoft && IsRefraction && IsLighting)
				{
					Core::Data::ShadersManager->Bind(1,"particles_refraction_light");
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_refraction_light","RefractCoef",&RefractionCoef);
				}
				else if(!IsSoft && !IsRefraction && IsLighting)
				{
					Core::Data::ShadersManager->Bind(1,"particles_light");
				}
				else if(!IsSoft && IsRefraction && !IsLighting)
				{
					Core::Data::ShadersManager->Bind(1,"particles_refraction");
					Core::Data::ShadersManager->SetValueRegisterF(1,"particles_refraction","RefractCoef",&RefractionCoef);
				}
				else
					Core::Data::ShadersManager->Bind(1,"particles");

			float4x4 tmpmatrot = *matrot;
			/*tmpmatrot._11 = 3.14 - tmpmatrot._11;
			tmpmatrot._12 = 3.14 - tmpmatrot._12;
			tmpmatrot._13 = 3.14 - tmpmatrot._13;

			tmpmatrot._21 = 3.14 - tmpmatrot._21;
			tmpmatrot._22 = 3.14 - tmpmatrot._22;
			tmpmatrot._23 = 3.14 - tmpmatrot._23;

			tmpmatrot._31 = 3.14 - tmpmatrot._31;
			tmpmatrot._32 = 3.14 - tmpmatrot._32;
			tmpmatrot._33 = 3.14 - tmpmatrot._33;*/

			/*tmpmatrot._11 *= -1;
			tmpmatrot._12 *= -1;
			tmpmatrot._13 *= -1;

			tmpmatrot._21 *= -1;
			tmpmatrot._22 *= -1;
			tmpmatrot._23 *= -1;

			tmpmatrot._31 *= -1;
			tmpmatrot._32 *= -1;
			tmpmatrot._33 *= -1;*/

			//tmpmatrot._41 = 3.14 - tmpmatrot._11;
			//tmpmatrot._42 = 3.14 - tmpmatrot._11;
			//tmpmatrot._43 = 3.14 - tmpmatrot._11;
			

			Core::Data::ShadersManager->SetValueRegisterF(0,"particles","ViewProjection",&SMMatrixTranspose(vp));
			Core::Data::ShadersManager->SetValueRegisterF(0,"particles","WorldMat",&SMMatrixTranspose(*worldmat));
			Core::Data::ShadersManager->SetValueRegisterF(0,"particles","World",&SMMatrixTranspose(world));
			Core::Data::ShadersManager->SetValueRegisterF(0,"particles","MatRot",&SMMatrixTranspose(tmpmatrot));
			Core::Data::ShadersManager->SetValueRegisterF(0,"particles","MatPos",&SMMatrixTranspose(*matpos));
			Core::Data::ShadersManager->SetValueRegisterF(0,"particles","PosCam",&Core::Data::ConstCurrentCameraPosition);

			Core::Data::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0, 0, 4, 0, 2);
			Core::Data::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			Core::Data::ShadersManager->UnBind();

			Core::Data::Device->SetStreamSourceFreq(0,1);
			Core::Data::Device->SetStreamSourceFreq(1,1);
		}
}


///////////////////

EffEffect::EffEffect()
{
	IsActive = false;
	Name[0] = 0;
	IDLight = -1;
	IsEnable = false;
	ID = 1;
	NameEffect[0] = 0;
	DistForCamera = 0.f;
	IsRenderForCamera = true;
	MeshBound = 0;
	IDSound = -1;

	BindPos = 0;
}

EffEffect::EffEffect(EffEffect& eff)
{
	BindPos = 0;
	DistForCamera = 0.f;
	IsRenderForCamera = true;
	Position = eff.Position;
	Direction = eff.Direction;
	Rotation = eff.Rotation;
	MatTranslation = eff.MatTranslation;
	MatRotate = eff.MatRotate;
	MatTransform = eff.MatTransform;
	sprintf(NameEffect,"%s",eff.Name);
	MeshBound = 0;

		if(eff.IDSound != -1)
		{
			char tmpsndpath[1024];
			char tmpsndname[1024];
			Core::Data::SoundControl->GetSoundObject(eff.IDSound)->GetPathFile(tmpsndpath);
			StrCutSound(tmpsndpath,tmpsndname);
			Sound::SoundObject* tmpsnd = new Sound::SoundObject(tmpsndname,true,0,0,Core::Data::SoundControl->GetSoundObject(eff.IDSound)->GetMaxDist(),Core::Data::SoundControl->GetSoundObject(eff.IDSound)->Damping,Core::Data::SoundControl->GetSoundObject(eff.IDSound)->ShiftPan);
			tmpsnd->SetMinDist(Core::Data::SoundControl->GetSoundObject(eff.IDSound)->GetMinDist());
			IDSound = tmpsnd->GetID();
		}
		else
			IDSound = -1;

	IDLight = eff.IDLight;

		if(eff.IDLight != -1 && eff.IDLight < SkyXEngine::Core::Data::Level::LightManager->GetCount())
		{
			//SkyXEngine::Graphics::ThreeD::Light* tmplight = new SkyXEngine::Graphics::ThreeD::Light(*(SkyXEngine::Core::Data::Level::LightManager->Arr[eff.IDLight]));
			float4 pos;
			SkyXEngine::Core::Data::Level::LightManager->Arr[eff.IDLight]->GetPosition(&pos);
			SkyXEngine::Core::Data::Level::LightManager->AddPoint(
																	&float3(0,0,0),
																	&(SkyXEngine::Core::Data::Level::LightManager->Arr[eff.IDLight]->Color),
																	pos.w,false,SkyXEngine::Core::Data::Level::LightManager->Arr[eff.IDLight]->IsShadow,
																	0);
			
			IDLight = SkyXEngine::Core::Data::Level::LightManager->Arr.size()-1;
			sprintf(SkyXEngine::Core::Data::Level::LightManager->Arr[IDLight]->Name,"%s",SkyXEngine::Core::Data::Level::LightManager->Arr[eff.IDLight]->Name);
			SkyXEngine::Core::Data::Level::LightManager->Arr[eff.IDLight]->IDEffect = 1;
		}

	ID = eff.ID;
	IsActive = eff.IsActive;
	Name[0] = 0;//sprintf(Name,"%s",eff.Name);
	IsEnable = eff.IsEnable;

		for(int i=0;i<eff.Arr.size();i++)
		{
			Arr.push_back(eff.Arr[i]);
		}
	ComAllBound();
}

EffEffect::~EffEffect()
{
		if(IDSound != -1)
			Core::Data::SoundControl->DeleteSoundObject(IDSound);
		if(IDLight!= -1 &&IDLight < SkyXEngine::Core::Data::Level::LightManager->GetCount())
			SkyXEngine::Core::Data::Level::LightManager->Delete(IDLight);
}

inline void EffEffect::Enable(bool isenable)
{
		if(!IsEnable && isenable)
		{
			for(int i=0;i<Arr.size();i++)
				Arr[i]->ReRun();
		}
	IsEnable = isenable;
	EnableLight(IsEnable);
}

inline void EffEffect::SetPosition(float3* pos)
{
	Position = *pos;
	MatTranslation = SMMatrixTranslation(Position.x,Position.y,Position.z);

		if(IDLight != -1 && IDLight < Core::Data::Level::LightManager->GetCount())
		{
			float4 tmppos;
			Core::Data::Level::LightManager->Arr[IDLight]->GetPosition(&tmppos);
			tmppos.x = Position.x;
			tmppos.y = Position.y;
			tmppos.z = Position.z;
			Core::Data::Level::LightManager->Arr[IDLight]->SetPosition(&tmppos);
		}

		if(IDSound != -1)
		{
			Core::Data::SoundControl->GetSoundObject(IDSound)->Position = Position;
		}
}

inline void EffEffect::SetDirection(float3* dir)
{
	Direction = *dir;
	float3 f(0,1,0);
	float3 a = SMVector3Cross(f,*dir);
	float ang = acosf(SMVector3Dot(f,*dir));
	MatRotate =  SMMatrixRotationAxis(a,ang);

	Rotation = SMMatrixToEuler(MatRotate);
}

inline void EffEffect::SetRotation(float3* rot)
{
	Rotation = *rot;
	Direction = SMEulerToVec(Rotation);

	float3 f(0,-1,0);
	float3 a = SMVector3Cross(f,Direction);
	float ang = acosf(SMVector3Dot(f,Direction));
	MatRotate =  SMMatrixRotationAxis(a,ang);

}
	
inline void EffEffect::GetPosition(float3* pos)
{
	*pos = Position;
}

inline void EffEffect::GetDirection(float3* dir)
{
	*dir = Direction;
}

inline void EffEffect::GetRotation(float3* rot)
{
	*rot = Rotation;
}

inline void EffEffect::SetBindBasePosAdrVector(float3* pos)
{
	BindBasePos = *pos;
}

inline void EffEffect::SetBindPosAdrVector(float3* pos)
{
	BindPos = pos;
}

inline float3* EffEffect::GetBindPosAdrVector()
{
	return BindPos;
}

inline void EffEffect::SetBindDirAdrVector(float3* dir)
{

}

inline void EffEffect::SetBindBaseDirAdrVector(float3* dir)
{

}

void EffEffect::SetLight(float4* color,float dist,bool is_shadow)
{
		if(IDLight != -1 && IDLight < Core::Data::Level::LightManager->GetCount())
		{
			Core::Data::Level::LightManager->Arr[IDLight]->Color = *color;
			Core::Data::Level::LightManager->Arr[IDLight]->IsShadow = is_shadow;

				if(is_shadow)
					Core::Data::Level::LightManager->Arr[IDLight]->SetShadowCube();

			Core::Data::Level::LightManager->Arr[IDLight]->ChangeRadiusHeight(dist,true);
			Core::Data::Level::LightManager->Arr[IDLight]->IDEffect = ID;
		}
		else
		{
			Core::Data::Level::LightManager->AddPoint(&float3(0,0,0),color,dist,false,is_shadow,0);
			IDLight = Core::Data::Level::LightManager->GetCount() - 1;

			char tmpname[256];
			sprintf(tmpname,"effect_%s",this->Name);
			Core::Data::Level::LightManager->SetName(IDLight,tmpname);
			Core::Data::Level::LightManager->Arr[IDLight]->IDEffect = ID;
		}
}

void EffEffect::DeleteLight()
{
		if(IDLight != -1 && IDLight < Core::Data::Level::LightManager->GetCount())
		{
			Core::Data::Level::LightManager->Delete(IDLight);
			IDLight = -1;
		}
}

void EffEffect::EnableLight(bool is_enable)
{
		if(IDLight != -1 && IDLight < Core::Data::Level::LightManager->GetCount())
		{
			Core::Data::Level::LightManager->SetEnable(IDLight,is_enable);
			Core::Data::Level::LightManager->Arr[IDLight]->IDEffect = ID;
		}
}

inline void EffEffect::SetSound(const char* path)
{
		if(IDSound != -1)
		{
			Core::Data::SoundControl->DeleteSoundObject(IDSound);
			IDSound = -1;
		}

	Sound::SoundObject* tmpsnd = new Sound::SoundObject(path,1,0,0,1,1,0.1);
	IDSound = tmpsnd->GetID();
}

inline void EffEffect::GetSound(char* path)
{
		if(IDSound != -1)
		{
			Core::Data::SoundControl->GetSoundObject(IDSound)->GetPathFile(path);
		}
}

inline void EffEffect::SetSoundRMin(float rmin)
{
		if(IDSound != -1)
		{
			Core::Data::SoundControl->GetSoundObject(IDSound)->SetMinDist(rmin);
		}
}

inline void EffEffect::SetSoundRMax(float rmax)
{
		if(IDSound != -1)
		{
			Core::Data::SoundControl->GetSoundObject(IDSound)->SetMaxDist(rmax);
		}
}

inline float EffEffect::GetSoundRMin()
{
		if(IDSound != -1)
		{
			return Core::Data::SoundControl->GetSoundObject(IDSound)->GetMinDist();
		}
}

inline float EffEffect::GetSoundRMax()
{
		if(IDSound != -1)
		{
			return Core::Data::SoundControl->GetSoundObject(IDSound)->GetMaxDist();
		}
}


void EffEffect::ComAllBound()
{
	IsComVisible = false;
		if(Arr.size() > 0)
		{
			float3 tmpmin,tmpmax;
			tmpmin = tmpmax = Arr[0]->Origin;

				for(int i=0;i<Arr.size();i++)
				{
						if(Arr[i]->ComOutBound == 0)
						{
								/*if((Arr[i]->Origin.x + Arr[i]->DispersionOfOrigin) > tmpmax.x)
									tmpmax.x = (Arr[i]->Origin.x + Arr[i]->DispersionOfOrigin);
								if((Arr[i]->Origin.y + Arr[i]->DispersionOfOrigin) > tmpmax.y)
									tmpmax.y = (Arr[i]->Origin.y + Arr[i]->DispersionOfOrigin);
								if((Arr[i]->Origin.z + Arr[i]->DispersionOfOrigin) > tmpmax.z)
									tmpmax.z = (Arr[i]->Origin.z + Arr[i]->DispersionOfOrigin);

								if((Arr[i]->Origin.x - Arr[i]->DispersionOfOrigin) < tmpmin.x)
									tmpmin.x = (Arr[i]->Origin.x - Arr[i]->DispersionOfOrigin);
								if((Arr[i]->Origin.y - Arr[i]->DispersionOfOrigin) < tmpmin.y)
									tmpmin.y = (Arr[i]->Origin.y - Arr[i]->DispersionOfOrigin);
								if((Arr[i]->Origin.z - Arr[i]->DispersionOfOrigin) < tmpmin.z)
									tmpmin.z = (Arr[i]->Origin.z - Arr[i]->DispersionOfOrigin);*/
						}
						else if(Arr[i]->ComOutBound == 1)
						{
							IsComVisible = true;
							float3 tmpcenter;
							float tmpradius;
							Arr[i]->BoundVolume.GetSphere(&tmpcenter,&tmpradius);

								if((tmpcenter.x + tmpradius) > tmpmax.x)
									tmpmax.x = (tmpcenter.x + tmpradius);
								if((tmpcenter.y + tmpradius) > tmpmax.y)
									tmpmax.y = (tmpcenter.y + tmpradius);
								if((tmpcenter.z + tmpradius) > tmpmax.z)
									tmpmax.z = (tmpcenter.z + tmpradius);

								if((tmpcenter.x - tmpradius) < tmpmin.x)
									tmpmin.x = (tmpcenter.x - tmpradius);
								if((tmpcenter.y - tmpradius) < tmpmin.y)
									tmpmin.y = (tmpcenter.y - tmpradius);
								if((tmpcenter.z - tmpradius) < tmpmin.z)
									tmpmin.z = (tmpcenter.z - tmpradius);
						}
						else if(Arr[i]->ComOutBound == 2)
						{
							IsComVisible = true;
							float3 tmptmpmin,tmptmpmax;
							Arr[i]->BoundVolume.GetMinMax(&tmptmpmin,&tmptmpmax);

								if(tmptmpmax.x > tmpmax.x)
									tmpmax.x = tmptmpmax.x;
								if(tmptmpmax.y > tmpmax.y)
									tmpmax.y = tmptmpmax.y;
								if(tmptmpmax.z > tmpmax.z)
									tmpmax.z = tmptmpmax.z;

								if(tmptmpmin.x < tmpmin.x)
									tmpmin.x = tmptmpmin.x;
								if(tmptmpmin.y < tmpmin.y)
									tmpmin.y = tmptmpmin.y;
								if(tmptmpmin.z < tmpmin.z)
									tmpmin.z = tmptmpmin.z;
						}
						else if(Arr[i]->ComOutBound == 3)
						{
							IsComVisible = true;
								if((Arr[i]->BeginCone.x - Arr[i]->BeginRadiusCone) < tmpmin.x)
									tmpmin.x = (Arr[i]->BeginCone.x - Arr[i]->BeginRadiusCone);
								if((Arr[i]->BeginCone.z - Arr[i]->BeginRadiusCone) < tmpmin.z)
									tmpmin.z = (Arr[i]->BeginCone.z - Arr[i]->BeginRadiusCone);

								if((Arr[i]->EndCone.x + Arr[i]->BeginRadiusCone) > tmpmax.x)
									tmpmax.x = (Arr[i]->EndCone.x + Arr[i]->BeginRadiusCone);
								if((Arr[i]->EndCone.z + Arr[i]->BeginRadiusCone) > tmpmax.z)
									tmpmax.z = (Arr[i]->EndCone.z + Arr[i]->BeginRadiusCone);

								if(Arr[i]->BeginCone.y < tmpmin.y)
									tmpmin.y = Arr[i]->BeginCone.y;

								if(Arr[i]->EndCone.y > tmpmax.y)
									tmpmax.y = Arr[i]->EndCone.y;
						}
				}

				if(IsComVisible)
				{
					BoundVolume.SetMinMax(&tmpmin,&tmpmax);

					float3 tmpcenter;
					float tmpradius;
					BoundVolume.GetSphere(&tmpcenter,&tmpradius);

					D3DXCreateSphere(Core::Data::Device,tmpradius,20,20,&MeshBound,0);
				}
		}
}

void EffEffect::Compute()
{
		if(IsEnable)
		{
				if(IDSound != -1)
				{
						if(!(Core::Data::SoundControl->GetSoundObject(IDSound)->Playing()))
							Core::Data::SoundControl->GetSoundObject(IDSound)->Play();
				}
		}
		else
		{
				if(IDSound != -1)
				{
					Core::Data::SoundControl->GetSoundObject(IDSound)->Stop();
					Core::Data::SoundControl->GetSoundObject(IDSound)->SetCurrentPos(0);
				}
		}
	
		if(IsRenderForCamera && IsEnable)
		{
			bool is_alife = false;
				for(int i=0;i<Arr.size();i++)
				{
						if(Arr[i]->IsAlife)
						{
							is_alife = true;
							Arr[i]->Compute();
							Arr[i]->ComputeLighting();
						}
				}
			IsEnable = is_alife;
			//Core::InLog("IsEnable = %d | %s | %d\n",IsEnable,this->Name,timeGetTime());
		}
	
}

void EffEffect::Render(DWORD timeDelta)
{
		if(IsEnable)
		{
				for(int i=0;i<Arr.size();i++)
				{
						//if(BindPos == 0)
							//MatTransform = MatRotate * MatTranslation;
						//else
						{
							//MatTransform = MatRotate * SMMatrixTranslation(BindPos->x+BindBasePos.x,BindPos->y+BindBasePos.y,BindPos->z+BindBasePos.z);

								if(IDLight != -1 && IDLight < Core::Data::Level::LightManager->GetCount())
								{
									float4 tmppos;
									Core::Data::Level::LightManager->Arr[IDLight]->GetPosition(&tmppos);
									tmppos.x = BindPos->x+BindBasePos.x;
									tmppos.y = BindPos->y+BindBasePos.y;
									tmppos.z = BindPos->z+BindBasePos.z;
									Core::Data::Level::LightManager->Arr[IDLight]->SetPosition(&tmppos);
								}
						}
					Arr[i]->Render(timeDelta,&MatTransform,&MatRotate,&MatTranslation);
				}
			Core::Data::Statistics::CountRenderEffects++;
		}
}

inline bool EffEffect::IsVisible(Core::ControllMoving::Frustum* frustum)
{
	float3 center,center2;
	float radius;
	float dist;
	BoundVolume.GetSphere(&center2,&radius);
	center = SMVector3Transform(center2,MatTransform);

		if(frustum->SphereInFrustum(&center,radius))
			return true;
		else
			return false;
}

/////////////////////

EffectManager::EffectManager()
{

}

EffectManager::~EffectManager()
{
		for(DWORD i=0;i<ArrEffects.size();i++)
		{
			nmem_delete(ArrEffects[i]);
			ArrEffects.erase(i);
		}

		for(DWORD i=0;i<ArrParticles.size();i++)
		{
			nmem_delete(ArrParticles[i]);
			ArrParticles.erase(i);
		}
}


inline DWORD EffectManager::GetCountParticles()
{
	return ArrParticles.size();
}

void EffectManager::DeleteParticles(DWORD id)
{
		if(id >= 0 && id < ArrParticles.size())
		{
			nmem_delete(ArrParticles[id]);
			ArrParticles.erase(id);
		}
}


inline DWORD EffectManager::GetCountEffects()
{
	return ArrEffects.size();
}

void EffectManager::DeleteEffects(DWORD id)
{
		if(id >= 0 && id < ArrEffects.size())
		{
			nmem_delete(ArrEffects[id]);
			ArrEffects.erase(id);
		}
}


void EffectManager::Compute()
{
		if(SkyXEngine::Core::Data::Settings::EditorHowRenderEffPart == 0 || SkyXEngine::Core::Data::Settings::EditorHowRenderEffPart == -2)
		{
				for(int i=0;i<ArrEffects.size();i++)
				{
						if(SkyXEngine::Core::Data::Settings::EditorHowRenderEffPart == -2 || SkyXEngine::Core::Data::Settings::EditorHowSelEffects == i)
						{
								//if(ArrEffects[i]->IsRenderForCamera)
									//ArrEffects[i]->Compute();
						}
						else
						{
							ArrEffects[i]->IsRenderForCamera = false;
							ArrEffects[i]->Enable(false);
						}
					ArrEffects[i]->Compute();
				}
		}
		//иначе мы воспроизводим партиклы, тада отключае все эффекты (сделано для отключения звука)
		else
		{
				for(int i=0;i<ArrEffects.size();i++)
				{
					ArrEffects[i]->IsRenderForCamera = false;
					ArrEffects[i]->Enable(false);
					ArrEffects[i]->Compute();
				}
		}

		if(SkyXEngine::Core::Data::Settings::EditorHowRenderEffPart == 1)
		{
				for(int i=0;i<ArrParticles.size();i++)
				{
						if(SkyXEngine::Core::Data::Settings::EditorHowSelParticles == -2 || SkyXEngine::Core::Data::Settings::EditorHowSelParticles == i)
						{
							ArrParticles[i]->Compute();
						}
				}
		}
}

void EffectManager::ComVisible()
{
	float4x4 mat = Core::Data::View * Core::Data::ProjectionStencil;
	float3 sizemapdepth = float3(Core::Data::WidthHeight.x * Core::Data::CoefSizeOC,Core::Data::WidthHeight.y * Core::Data::CoefSizeOC,Core::Data::NearFar.y);
	float tmpdepth;
	float3 center,center2;
	float radius;

	int tmpkey;
	DWORD sizewarrdepth = Core::Data::WidthHeight.x * Core::Data::CoefSizeOC;
	DWORD sizeharrdepth = Core::Data::WidthHeight.y * Core::Data::CoefSizeOC;
	Graphics::ThreeD::PosBBScreen res;

		for(DWORD i=0;i<ArrEffects.size();i++)
		{
			ArrEffects[i]->IsRenderForCamera = false;

				if(!(ArrEffects[i]->IsComVisible))
				{
					ArrEffects[i]->IsRenderForCamera = true;
				}

				if(ArrEffects[i]->BindPos == 0)
					ArrEffects[i]->MatTransform = ArrEffects[i]->MatRotate*ArrEffects[i]->MatTranslation;
				else
					ArrEffects[i]->MatTransform = ArrEffects[i]->MatRotate * SMMatrixTranslation(ArrEffects[i]->BindPos->x+ArrEffects[i]->BindBasePos.x,ArrEffects[i]->BindPos->y+ArrEffects[i]->BindBasePos.y,ArrEffects[i]->BindPos->z+ArrEffects[i]->BindBasePos.z);

				//если модель видна камере
				if(!ArrEffects[i]->IsRenderForCamera && ArrEffects[i]->IsVisible(Core::Data::ObjCamera->ObjFrustum) && ArrEffects[i]->Enable())
				{
					ArrEffects[i]->BoundVolume.GetPosBBScreen(&res,&(Core::Data::ConstCurrentCameraPosition),&sizemapdepth,&(ArrEffects[i]->MatTransform * mat));
					
						/*if(Core::Data::Settings::IsUpdateCamera)
							ArrEffects[i]->IsRenderForCamera = false;*/
						
					tmpdepth = Core::Data::CurrentMaxDepth2;
						//если минимальная глубина модели меньше ем максимальная глубина сцены
						if(tmpdepth > res.mindepth*Core::Data::NearFar.y)
						{
							ArrEffects[i]->BoundVolume.GetSphere(&center2,&radius);
							center = SMVector3Transform(center2,ArrEffects[i]->MatTransform);
							//определяем расстояние до камеры
							ArrEffects[i]->DistForCamera = SMVector3Length((center - Core::Data::ConstCurrentCameraPosition)) - radius;;//res.mindepth*Core::Data::NearFar.y;

								if(ArrEffects[i]->DistForCamera < 0.f)
									ArrEffects[i]->DistForCamera = 0.f;


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
											ArrEffects[i]->IsRenderForCamera = true;
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
																	ArrEffects[i]->IsRenderForCamera = true;
														}
												}
										}

										/*if(!(Arr[i]->IsRenderForCamera))
											Core::Data::Statistics::CountModelExcluded++;*/
								}
						}	
				}
		}
}

void EffectManager::Render(DWORD timeDelta)
{
		//если рисуем эффекты либо мы не в редакторе
		if(SkyXEngine::Core::Data::Settings::EditorHowRenderEffPart == 0 || SkyXEngine::Core::Data::Settings::EditorHowRenderEffPart == -2)
		{
				for(int i=0;i<ArrEffects.size();i++)
				{
						//если мы не в редакторе или выделенный эффект ето тот кторый щас будем рисовать
						if(SkyXEngine::Core::Data::Settings::EditorHowRenderEffPart == -2 || SkyXEngine::Core::Data::Settings::EditorHowSelEffects == i)
						{
								if(ArrEffects[i]->IsRenderForCamera)
									ArrEffects[i]->Render(timeDelta);
						}
				}
		}

		//если мы в редакторе и рисуем выделенный партикл
		if(SkyXEngine::Core::Data::Settings::EditorHowRenderEffPart == 1)
		{
				for(int i=0;i<ArrParticles.size();i++)
				{
						if(SkyXEngine::Core::Data::Settings::EditorHowSelParticles == -2 || SkyXEngine::Core::Data::Settings::EditorHowSelParticles == i)
							ArrParticles[i]->Render(timeDelta);
				}
		}
}

void EffectManager::RenderVolume()
{
	int numsel = SkyXEngine::Core::Data::Settings::EditorHowSelEffects;
		if(numsel >= 0 && ArrEffects.size() > numsel && SkyXEngine::Core::Data::Settings::EditorsIsSelectedEffect && ArrEffects[numsel]->MeshBound)
		{
			SkyXEngine::Core::Data::Device->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&(ArrEffects[numsel]->MatTransform));
			SkyXEngine::Core::Data::Device->SetTexture(0,SkyXEngine::Core::Data::LoadedTextures->GetTexture(SkyXEngine::Core::Data::EngineID::Tex_FreeSelectedMesh));
			ArrEffects[numsel]->MeshBound->DrawSubset(0);
		}
}

bool EffectManager::TraceBeam(const float3 & start, const float3 & dir, float3 * _res,DWORD* ideffect)
{
	triangle tmptri;
	float dist;
	bool tmpiscom = true;
	float3 ip;
	float3 res = start + float3(10000.0f, 10000.0f, 10000.0f);
	bool found = false;
	triangle arrtri[12];

		for(DWORD i = 0; i < ArrEffects.size() && tmpiscom; i++)
		{
			float3 tmpmin,tmpmax;
			ArrEffects[i]->BoundVolume.GetMinMax(&tmpmin,&tmpmax);
			tmpmin = SMVector3Transform(tmpmin,ArrEffects[i]->MatTransform);
			tmpmax = SMVector3Transform(tmpmax,ArrEffects[i]->MatTransform);
			arrtri[0] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmin.x,tmpmax.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmin.z));
			arrtri[1] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmin.z),float3_t(tmpmax.x,tmpmin.y,tmpmin.z));

			arrtri[2] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmax.z),float3_t(tmpmin.x,tmpmax.y,tmpmax.z),float3_t(tmpmax.x,tmpmax.y,tmpmax.z));
			arrtri[3] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmin.y,tmpmax.z));

			arrtri[4] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmin.x,tmpmax.y,tmpmin.z),float3_t(tmpmin.x,tmpmax.y,tmpmax.z));
			arrtri[5] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmin.x,tmpmax.y,tmpmax.z),float3_t(tmpmin.x,tmpmin.y,tmpmax.z));

			arrtri[6] = triangle(float3_t(tmpmax.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmax.z));
			arrtri[7] = triangle(float3_t(tmpmax.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmax.z),float3_t(tmpmax.x,tmpmin.y,tmpmax.z));

			arrtri[8] = triangle(float3_t(tmpmin.x,tmpmax.y,tmpmin.z),float3_t(tmpmin.x,tmpmax.y,tmpmax.z),float3_t(tmpmax.x,tmpmax.y,tmpmax.z));
			arrtri[9] = triangle(float3_t(tmpmin.x,tmpmax.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmax.z),float3_t(tmpmax.x,tmpmax.y,tmpmin.z));

			arrtri[10] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmin.x,tmpmin.y,tmpmax.z),float3_t(tmpmax.x,tmpmin.y,tmpmax.z));
			arrtri[11] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmin.y,tmpmax.z),float3_t(tmpmax.x,tmpmin.y,tmpmin.z));

				for(int k=0;k<12;k++)
				{
						if(arrtri[k].IntersectLine(start, dir * 10000.0f, &ip))
						{
								if(SMVector3Length2(start - res) > SMVector3Length2(start - ip))
								{
									res = ip;
									found = true;
										if(ideffect)
											*ideffect = i;
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

/////////////

EffectRain::EffectRain()
{
	Density = 0;
	CountLife = 0;
	TimeCountCreate = 0;
	CountIntersectBB = 10;
	PercentAudibility = 0;
	BMax = float3_t(7,20,20);
	BMin = float3_t(-7,15,-1);

	IDSound = -1;

	CountLifeAudibility = 0;
	OldAudibility = 0;
	CurAudibility = 0;
	TimeComAudibility = 0;

		for(int i=0;i<MAXPART_RAIN;i++)
		{
			IsLife[i] = false;
		}
}

EffectRain::~EffectRain()
{

}

void EffectRain::Init()
{
	Core::Data::Device->CreateVertexBuffer(
               MAXPART_RAIN * sizeof(float3_t),
               D3DUSAGE_WRITEONLY,
               0,
               D3DPOOL_MANAGED,
               &TransVertBuf,
               0);

	Core::Data::Device->CreateVertexBuffer(
               4 * sizeof(VertexEffectRain),
               0,
               0,
               D3DPOOL_MANAGED,
               &VertexBuff,
               0);

	Core::Data::Device->CreateIndexBuffer(
               6 * sizeof(WORD),
               0,
               D3DFMT_INDEX16,
               D3DPOOL_MANAGED,
               &IndexBuff,
               0);


	CommonParticleDecl* vertices;
	VertexBuff->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = CommonParticleDecl(-SIZEPART_RAIN_X, -SIZEPART_RAIN_Y, 0.f, 0.0f, 1.0f);
	vertices[1] = CommonParticleDecl(-SIZEPART_RAIN_X,  SIZEPART_RAIN_Y, 0.f, 0.0f, 0.0f);
	vertices[2] = CommonParticleDecl( SIZEPART_RAIN_X,  SIZEPART_RAIN_Y, 0.f, 1.0f, 0.0f);
	vertices[3] = CommonParticleDecl( SIZEPART_RAIN_X, -SIZEPART_RAIN_Y, 0.f, 1.0f, 1.0f);

	VertexBuff->Unlock();


	WORD* indices = 0;
	IndexBuff->Lock(0, 0, (void**)&indices, 0);

	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;

	IndexBuff->Unlock();
}

inline void EffectRain::ReCreate(int key)
{
	Position[key].x = SMRndf(BMin.x,BMax.x);
	Position[key].y = SMRndf(BMin.y,BMax.y);
	Position[key].z = SMRndf(BMin.z,BMax.z);
	Position[key] = SMVector3Transform(Position[key],RotMatrix);
	Position[key].x += Core::Data::ConstCurrentCameraPosition.x;
	Position[key].y += Core::Data::ConstCurrentCameraPosition.y;
	Position[key].z += Core::Data::ConstCurrentCameraPosition.z;

	float3 tmppos = Position[key];
		
		if(
			(CountIntersectBB == 0 && Core::Data::Level::ArrStaticModel->GetIntersectedRayYRain(&tmppos)) ||
			(CountIntersectBB != 0 && Core::Data::Level::ArrStaticModel->GetIntersectedRayYRainBB(&tmppos))
			)
			PositionY[key] = tmppos.y;
		else
			PositionY[key] = Core::Data::ConstCurrentCameraPosition.y-5;

	CountIntersectBB++;

	if(CountIntersectBB > Core::Data::Settings::RainHowIntersectGeom)
			CountIntersectBB = 0;
	IsLife[key] = true;
	CountLife++;
}

void EffectRain::Update(DWORD timeDelta)
{
	//PercentAudibility = 0;
	int tmpcurrcreate = 0;	//количество вновь созданных частиц
	TimeComAudibility += timeDelta;
	TimeCountCreate += timeDelta;
		if(Density > 0)
		{
			int tmpCountMax = float(MAXPART_RAIN) * (float(Density) / 100.f);

			CountLife = 0;
				for(int i=0;i<tmpCountMax;i++)
				{
						if(IsLife[i] && Position[i].y >= PositionY[i]+SIZEPART_RAIN_Y)
						{
							Position[i].y -= 20.f * float(timeDelta) * 0.001f;
								if(Position[i].y >= PositionY[i]+SIZEPART_RAIN_Y)
								{
									IsLife[i] = true;
									float tmpdist2 = SMVector3Dot(Core::Data::ConstCurrentCameraPosition-float3(Position[i].x,PositionY[i],Position[i].z)) / 400.f;
												
									tmpdist2 = 1.f - tmpdist2;

										if(tmpdist2 < 0.f)
											tmpdist2 = 0.0f;
									tmpdist2 /= float(tmpCountMax)*0.5f;

										//if(PercentAudibility < 1.f)
											PercentAudibility += tmpdist2;
									CountLife++;
											
								}
								else
									IsLife[i] = false;
						}
						else
							IsLife[i] = false;
				}
					
				for(int i=0;i<tmpCountMax;i++)
				{
						if(!IsLife[i])
						{
								if(TimeCountCreate >= 100)
								{
									ReCreate(i);
									tmpcurrcreate++;
								}

								if(tmpcurrcreate >= tmpCountMax/9)
									TimeCountCreate = 0;
						}
				}
		}
		else if(CountLife > 0)
		{
			CountLife = 0;
			PercentAudibility = 0;
				for(int i=0;i<MAXPART_RAIN;i++)
				{
						if(IsLife[i] && Position[i].y >= PositionY[i]+SIZEPART_RAIN_Y)
						{
							Position[i].y -= 20.f * float(timeDelta) * 0.001f;
								if(Position[i].y >= PositionY[i]+SIZEPART_RAIN_Y)
								{
									IsLife[i] = true;
									CountLife++;
											
								}
								else
									IsLife[i] = false;
						}
						else
							IsLife[i] = false;
				}
		}
	CountLifeAudibility++;
		
		//если подошло время обновления
		if(TimeComAudibility >= TIMERAINSNDUPDATE)
		{
			//Core::InLog("PercentAudibility = %f\n",PercentAudibility);
			PercentAudibility /= float(CountLifeAudibility);
			PercentAudibility*=100.f;
			
				if(PercentAudibility > 100)
					PercentAudibility = 100;

			OldAudibility = CurAudibility;
			CurAudibility = (((PercentAudibility * 100)-10000)+OldAudibility) / 2;
				if(PercentAudibility > 0)
					CurAudibility -= (CurAudibility) / 2;
			//Core::InLog("CurAudibility = %d\n",CurAudibility);
			
			CountLifeAudibility = 0;
			TimeComAudibility = 0;
			PercentAudibility = 0;
		}
		//если нет то продолжаем интерполировать от прошлой к текущей слышимости
		else if(IDSound != -1 && CountLife > 0)
		{
			int tmpaud = lerp(float(OldAudibility),float(CurAudibility),float(TimeComAudibility)/float(TIMERAINSNDUPDATE));
			SkyXEngine::Core::Data::SoundControl->GetSoundObject(IDSound)->SetVolume(tmpaud,SOUND_TV_DB);
			//Core::InLog("tmpaud = %d\n",tmpaud);
		}
		else if(IDSound != -1 && CountLife == 0)
			SkyXEngine::Core::Data::SoundControl->GetSoundObject(IDSound)->SetVolume(-10000,SOUND_TV_DB);
}

inline void EffectRain::SetDensity(int density)
{
	Density = density;
}

inline int EffectRain::GetDensity()
{
	return Density;
}

inline void EffectRain::SetSound(const char* path)
{
	//Core::InLog("EffectRain::SetSound = %s\n",path);
	bool isunic = true;
	IDSound = -1;
		for(int i=0;i<ArrPathSounds.size();i++)
		{
			//Core::InLog("--SetSound = %s\n",ArrPathSounds[i].c_str());
				if(strcmp(ArrPathSounds[i].c_str(),path) == 0)
				{
					isunic = false;
					IDSound = ArrIDSounds[i];
					break;
				}
		}

		if(isunic)
		{
			ArrPathSounds.push_back(String(path));
			
				if(IDSound != -1)
				{
					Core::Data::SoundControl->GetSoundObject(IDSound)->SetVolume(-10000);
					Core::Data::SoundControl->GetSoundObject(IDSound)->Stop();
				}
			Sound::SoundObject* tmpsnd = new Sound::SoundObject(path,true,0,0);
			tmpsnd->SetVolume(0,SOUND_TV_PCT);
			tmpsnd->Play();
			IDSound = tmpsnd->GetID();
			ArrIDSounds.push_back(tmpsnd->GetID());
		}
}

void EffectRain::Render()
{
		if(CountLife > 0)
		{
			float3_t* RTGPUArrVerteces;
			TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
			DWORD tmpcount = 0;
			int tmpCountMax = float(MAXPART_RAIN) * (float(Density) / 100.f);
				for(DWORD i=0;i<MAXPART_RAIN;i++)
				{
						if(IsLife[i])
						{
							RTGPUArrVerteces[tmpcount] = Position[i];
							
							tmpcount++;
						}
				}
	
			TransVertBuf->Unlock();

			Core::Data::Statistics::CountRenderParticles+=tmpcount;

			Core::Data::Device->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | tmpcount) );

			Core::Data::Device->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1) );
			Core::Data::Device->SetStreamSource(1, TransVertBuf, 0,  sizeof(float3_t) );

			Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationRain);

			Core::Data::Device->SetStreamSource(0, VertexBuff, 0, sizeof(VertexEffectRain));
			Core::Data::Device->SetIndices(IndexBuff);

			//считаем угол поворота камеры по оси Y
			float3 Direction = SMVector3Normalize(Core::Data::ConstCurrentCameraDirection);
			float3 tmpVec(Direction.x, 0.0f, Direction.z);
			float3 dirVec(0.0f, 0.0f, 1.0f);
			RotAng = acosf(SMVector3Dot(tmpVec, dirVec)) * sign(SMVector3Cross(tmpVec, dirVec).y);
			RotMatrix = SMMatrixRotationY(-RotAng);

			//поворачиваем капли к камере
			float4x4 world = SMMatrixIdentity();
			world._11 = Core::Data::View._11;
			world._12 = Core::Data::View._21;
			world._13 = Core::Data::View._31;
			world._44 = 1.0;

			float4x4 wvp = Core::Data::View * Core::Data::ProjectionStencil;
			Core::Data::Device->SetTexture(0,SkyXEngine::Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_Rain));

			Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_ParticleRain);
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_ParticleRain);

			Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ParticleRain,"ViewProjection",&SMMatrixTranspose(wvp));
			Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ParticleRain,"World",&SMMatrixTranspose(world));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_ParticleRain,"Color",&Color);

			Core::Data::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0, 0, 4, 0, 2);
			Core::Data::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			Core::Data::ShadersManager->UnBind();

			Core::Data::Device->SetStreamSourceFreq(0,1);
			Core::Data::Device->SetStreamSourceFreq(1,1);
		}
}

////////////////////////////////////////

DWORD EffectCreate(const char* gname,const char* lname)
{
	bool isfinded = false;

		for(int i=0;i<SkyXEngine::Core::Data::Level::ArrEffects->ArrEffects.size();i++)
		{
				if(strcmp(SkyXEngine::Core::Data::Level::ArrEffects->ArrEffects[i]->Name,gname) == 0)
				{
					SkyXEngine::Graphics::Particles::EffEffect* tmpeffect = new SkyXEngine::Graphics::Particles::EffEffect(*(SkyXEngine::Core::Data::Level::ArrEffects->ArrEffects[i]));
					sprintf(tmpeffect->Name,"%s",lname);
					tmpeffect->Enable(false);
					tmpeffect->SetPosition(&float3(0,0,0));
					tmpeffect->SetRotation(&float3(0,0,0));
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.push_back(tmpeffect);
					isfinded = true;
					break;
				}
		}

		if(!isfinded)
		{
			return -1;
		}
		else
		{
			return SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size()-1;
		}
}

int EffectSetPos(const char* lname,float3* vec)
{
		for(DWORD i=0;i<SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size();i++)
		{
				if(strcmp(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Name,lname) == 0)
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->SetPosition(vec);
					i = -1;
					return 0;
				}
		}
	return -1;
}

int EffectSetPos(DWORD id,float3* vec)
{
		if(id >= 0 && id < SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size())
		{
				if(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id])
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id]->SetPosition(vec);
					return 0;
				}
				else
					return -2;
		}
		else
			return -3;
}


int EffectSetDir(const char* lname,float3* vec)
{
		for(DWORD i=0;i<SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size();i++)
		{
				if(strcmp(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Name,lname) == 0)
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->SetDirection(vec);
					i = -1;
					return 0;
				}
		}
	return -1;
}

int EffectSetDir(DWORD id,float3* vec)
{
		if(id >= 0 && id < SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size())
		{
				if(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id])
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id]->SetDirection(vec);
					return 0;
				}
				else
					return -2;
		}
		else
			return -3;
}

int EffectGetPos(const char* lname,float3* vec)
{
		for(DWORD i=0;i<SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size();i++)
		{
				if(strcmp(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Name,lname) == 0)
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->GetPosition(vec);
					i = -1;
					return 0;
				}
		}
	return -1;
}

int EffectGetPos(DWORD id,float3* vec)
{
		if(id >= 0 && id < SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size())
		{
				if(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id])
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id]->GetPosition(vec);
					return 0;
				}
				else
					return -2;
		}
		else
			return -3;
}

int EffectGetDir(const char* lname,float3* vec)
{
		for(DWORD i=0;i<SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size();i++)
		{
				if(strcmp(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Name,lname) == 0)
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->GetDirection(vec);
					i = -1;
					return 0;
				}
		}
	return -1;
}

int EffectGetDir(DWORD id,float3* vec)
{
		if(id >= 0 && id < SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size())
		{
				if(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id])
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id]->GetDirection(vec);
					return 0;
				}
				else
					return -2;
		}
		else
			return -3;
}

int EffectRecreate(const char* lname)
{
		for(DWORD i=0;i<SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size();i++)
		{
				if(strcmp(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Name,lname) == 0)
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Enable(false);
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Enable(true);
					i = -1;
					return 0;
				}
		}
	return -1;
}

int EffectRecreate(DWORD id)
{
		if(id >= 0 && id < SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size())
		{
				if(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id])
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id]->Enable(false);
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id]->Enable(true);
					return 0;
				}
				else
					return -2;
		}
		else
			return -3;
}



int EffectSetEnable(const char* lname,bool bf)
{
		for(DWORD i=0;i<SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size();i++)
		{
				if(strcmp(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Name,lname) == 0)
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Enable(bf);
					i = -1;
					return 0;
				}
		}
	return -1;
}

int EffectSetEnable(DWORD id,bool bf)
{
		if(id >= 0 && id < SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size())
		{
				if(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id])
				{
					SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id]->Enable(bf);
					return 0;
				}
				else
					return -2;
		}
		else
			return -3;
}

int EffectGetEnable(const char* lname)
{
		for(DWORD i=0;i<SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size();i++)
		{
				if(strcmp(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Name,lname) == 0)
				{
					return SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Enable();
				}
		}
	return -1;
}

int EffectGetEnable(DWORD id)
{
		if(id >= 0 && id < SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size())
		{
				if(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id])
				{
					return SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id]->Enable();
				}
				else
					return -2;
		}
		else
			return -3;
}


int EffectDelete(const char* lname)
{
		for(DWORD i=0;i<SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size();i++)
		{
				if(strcmp(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]->Name,lname) == 0)
				{
					nmem_delete(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[i]);
					i = -1;
					return 0;
				}
		}
	return -1;
}

int EffectDelete(DWORD id)
{
		if(id >= 0 && id < SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects.size())
		{
				if(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id])
				{
					nmem_delete(SkyXEngine::Core::Data::Level::ManagerEffects->ArrEffects[id]);
					return 0;
				}
				else
					return -2;
		}
		else
			return -3;
}

};
};
};