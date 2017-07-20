
#include <particles/effect.h>

#pragma once

Effects::Effects()
{
	ArrSortSizeCurr = 0;
}

Effects::~Effects()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i])
		{
			mem_delete(ArrID[i]);
		}
	}

	ArrID.clear();
}

Effects::Effect::Effect()
{
	NullingInit();
}

Effects::Effect::Effect(Effect& eff)
{
	NullingInit();
	Position = eff.Position;
	Direction = eff.Direction;
	Rotation = eff.Rotation;
	
	Enable = eff.Enable;

	for (int i = 0; i < eff.Arr.size(); ++i)
	{
		Emitter* part = new Emitter(*eff.Arr[i]);
		Arr.push_back(part);
	}
	IDPool = eff.IDPool;
}

Effects::Effect::~Effect()
{
	for (int i = 0; i < Arr.size(); ++i)
	{
		mem_delete(Arr[i]);
	}

	Arr.clear();
}

Effects::Pool::Pool()
{
	ideff = -1;
}

void Effects::Effect::NullingInit()
{
	Id = Key = -1;
	ViewDist = 0.0f;
	ViewRender = false;

	Enable = false;
	Busy = false;
	IDPool = -1;
	Original = true;
}

void Effects::OnLostDevice()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i])
		{
			for (int k = 0; k < ArrID[i]->Arr.size(); ++k)
			{
				if (ArrID[i]->Arr[k])
					ArrID[i]->Arr[k]->OnLostDevice();
			}
		}
	}
}

void Effects::OnResetDevice()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i])
		{
			for (int k = 0; k < ArrID[i]->Arr.size(); ++k)
			{
				if (ArrID[i]->Arr[k])
					ArrID[i]->Arr[k]->OnResetDevice();
			}
		}
	}
}

void Effects::Clear()
{
	for (int i = 0; i < ArrKey.size(); ++i)
	{
		mem_delete(ArrKey[i]);
	}

	ArrKey.clear();
	ArrID.clear();

	for (int i = 0; i < Pools.size(); ++i)
	{
		mem_delete(Pools[i]);
	}

	Pools.clear();
}

void Effects::Save(const char* path)
{
	FILE* file = 0;
	file = fopen(path, "w");

	if (!file)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "%s - failed to save %s", gen_msg_location, path);
		return;
	}

	int eff_count = EffectCountGet();

	fprintf(file, "[effects]\n");
	fprintf(file, "count = %d\n\n", eff_count);

	for (int i = 0; i < eff_count; ++i)
	{
		int part_count = ArrKey[i]->Arr.size();
		
		fprintf(file, "[effect_%d]\n", i);
		fprintf(file, "name = %s\n", ArrKey[i]->Name);
		fprintf(file, "emitters_count = %d\n\n", part_count);

		for (int k = 0; k < part_count; ++k)
		{
			Emitter* part = ArrKey[i]->Arr[k];
			char tmpname[CONFIG_SECTION_MAX_LEN];
			part->NameGet(tmpname);
			char tmptex[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
			tmptex[0] = 0;

			if (part->TextureGetID() >= 0)
				SGCore_LoadTexGetName(part->TextureGetID(), tmptex);

			fprintf(file, "[effect_%d_emitter_%d]\n", i, k);
			fprintf(file, "name = %s\n", tmpname);
			fprintf(file, "count = %d\n", part->CountGet());

			if (tmptex[0] != 0)
				fprintf(file, "texture = %s\n", tmptex);

			fprintf(file, "BoundType = %d\n", part->GetData()->BoundType);

			fprintf(file, "BoundVec1X = %f\n", part->GetData()->BoundVec1.x);
			fprintf(file, "BoundVec1Y = %f\n", part->GetData()->BoundVec1.y);
			fprintf(file, "BoundVec1Z = %f\n", part->GetData()->BoundVec1.z);
			fprintf(file, "BoundVec1W = %f\n", part->GetData()->BoundVec1.w);

			fprintf(file, "BoundVec2X = %f\n", part->GetData()->BoundVec2.x);
			fprintf(file, "BoundVec2Y = %f\n", part->GetData()->BoundVec2.y);
			fprintf(file, "BoundVec2Z = %f\n", part->GetData()->BoundVec2.z);
			fprintf(file, "BoundVec2W = %f\n", part->GetData()->BoundVec2.w);


			fprintf(file, "SpawnPosType = %d\n", part->GetData()->SpawnPosType);
			fprintf(file, "SpawnOriginX = %f\n", part->GetData()->SpawnOrigin.x);
			fprintf(file, "SpawnOriginY = %f\n", part->GetData()->SpawnOrigin.y);
			fprintf(file, "SpawnOriginZ = %f\n", part->GetData()->SpawnOrigin.z);

			fprintf(file, "SpawnOriginDisp = %f\n", part->GetData()->SpawnOriginDisp);

			fprintf(file, "SpawnBoundBindCreateXNeg = %d\n", part->GetData()->SpawnBoundBindCreateXNeg);
			fprintf(file, "SpawnBoundBindCreateXPos = %d\n", part->GetData()->SpawnBoundBindCreateXPos);
			fprintf(file, "SpawnBoundBindCreateYNeg = %d\n", part->GetData()->SpawnBoundBindCreateYNeg);
			fprintf(file, "SpawnBoundBindCreateYPos = %d\n", part->GetData()->SpawnBoundBindCreateYPos);
			fprintf(file, "SpawnBoundBindCreateZNeg = %d\n", part->GetData()->SpawnBoundBindCreateZNeg);
			fprintf(file, "SpawnBoundBindCreateZPos = %d\n", part->GetData()->SpawnBoundBindCreateZPos);

			fprintf(file, "SpawnNextTime = %d\n", part->GetData()->SpawnNextTime);
			fprintf(file, "SpawnNextTimeDisp = %d\n", part->GetData()->SpawnNextTimeDisp);


			fprintf(file, "AnimTexCountCadrsX = %d\n", part->GetData()->AnimTexCountCadrsX);
			fprintf(file, "AnimTexCountCadrsY = %d\n", part->GetData()->AnimTexCountCadrsY);

			fprintf(file, "AnimTexRate = %d\n", part->GetData()->AnimTexRate);
			fprintf(file, "AnimTexRateDisp = %d\n", part->GetData()->AnimTexRateDisp);
			fprintf(file, "AnimTexStartCadr = %d\n", part->GetData()->AnimTexStartCadr);
			fprintf(file, "AnimTexStartCadrDisp = %d\n", part->GetData()->AnimTexStartCadrDisp);


			fprintf(file, "TimeLife = %d\n", part->GetData()->TimeLife);
			fprintf(file, "TimeLifeDisp = %d\n", part->GetData()->TimeLifeDisp);
			fprintf(file, "AlphaDependAge = %d\n", part->GetData()->AlphaDependAge);

			fprintf(file, "SizeX = %f\n", part->GetData()->Size.x);
			fprintf(file, "SizeY = %f\n", part->GetData()->Size.y);
			fprintf(file, "SizeDisp = %f\n", part->GetData()->SizeDisp);

			fprintf(file, "SizeDependAge = %d\n", part->GetData()->SizeDependAge);


			fprintf(file, "VelocityX = %f\n", part->GetData()->Velocity.x);
			fprintf(file, "VelocityY = %f\n", part->GetData()->Velocity.y);
			fprintf(file, "VelocityZ = %f\n", part->GetData()->Velocity.z);
			fprintf(file, "VelocityDisp = %f\n", part->GetData()->VelocityDisp);

			fprintf(file, "VelocityDispXNeg = %d\n", part->GetData()->VelocityDispXNeg);
			fprintf(file, "VelocityDispYNeg = %d\n", part->GetData()->VelocityDispYNeg);
			fprintf(file, "VelocityDispZNeg = %d\n", part->GetData()->VelocityDispZNeg);


			fprintf(file, "AccelerationX = %f\n", part->GetData()->Acceleration.x);
			fprintf(file, "AccelerationY = %f\n", part->GetData()->Acceleration.y);
			fprintf(file, "AccelerationZ = %f\n", part->GetData()->Acceleration.z);
			fprintf(file, "AccelerationDisp = %f\n", part->GetData()->AccelerationDisp);

			fprintf(file, "AccelerationDispXNeg = %d\n", part->GetData()->AccelerationDispXNeg);
			fprintf(file, "AccelerationDispYNeg = %d\n", part->GetData()->AccelerationDispYNeg);
			fprintf(file, "AccelerationDispZNeg = %d\n", part->GetData()->AccelerationDispZNeg);


			fprintf(file, "CharacterCircle = %d\n", part->GetData()->CharacterCircle);
			fprintf(file, "CharacterCircleAxis = %d\n", part->GetData()->CharacterCircleAxis);
			fprintf(file, "CharacterCircleAngle = %f\n", part->GetData()->CharacterCircleAngle);
			fprintf(file, "CharacterCircleAngleDisp = %f\n", part->GetData()->CharacterCircleAngleDisp);
			fprintf(file, "CharacterCircleAngleDispNeg = %d\n", part->GetData()->CharacterCircleAngleDispNeg);


			fprintf(file, "CharacterRotate = %d\n", part->GetData()->CharacterRotate);
			fprintf(file, "CharacterRotateAngle = %f\n", part->GetData()->CharacterRotateAngle);
			fprintf(file, "CharacterRotateAngleDisp = %f\n", part->GetData()->CharacterRotateAngleDisp);
			fprintf(file, "CharacterRotateAngleDispNeg = %d\n", part->GetData()->CharacterRotateAngleDispNeg);


			fprintf(file, "CharacterDeviation = %d\n", part->GetData()->CharacterDeviation);
			fprintf(file, "CharacterDeviationType = %d\n", part->GetData()->CharacterDeviationType);
			fprintf(file, "CharacterDeviationAmplitude = %f\n", part->GetData()->CharacterDeviationAmplitude);
			fprintf(file, "CharacterDeviationCoefAngle = %f\n", part->GetData()->CharacterDeviationCoefAngle);
			fprintf(file, "CharacterDeviationAxis = %d\n", part->GetData()->CharacterDeviationAxis);
			fprintf(file, "CharacterDeviationCountDelayMls = %d\n", part->GetData()->CharacterDeviationCountDelayMls);
			fprintf(file, "CharacterDeviationCoefAngleDisp = %f\n", part->GetData()->CharacterDeviationCoefAngleDisp);
			fprintf(file, "CharacterDeviationCoefAngleDispNeg = %d\n", part->GetData()->CharacterDeviationCoefAngleDispNeg);

			fprintf(file, "CharacterDeviationTapX = %d\n", part->GetData()->CharacterDeviationTapX);
			fprintf(file, "CharacterDeviationTapY = %d\n", part->GetData()->CharacterDeviationTapY);
			fprintf(file, "CharacterDeviationTapZ = %d\n", part->GetData()->CharacterDeviationTapZ);


			fprintf(file, "FigureType = %d\n", part->GetData()->FigureType);
			fprintf(file, "FigureCountQuads = %d\n", part->GetData()->FigureCountQuads);
			fprintf(file, "FigureRotRand = %d\n", part->GetData()->FigureRotRand);

			fprintf(file, "FigureTapX = %d\n", part->GetData()->FigureTapX);
			fprintf(file, "FigureTapY = %d\n", part->GetData()->FigureTapY);
			fprintf(file, "FigureTapZ = %d\n", part->GetData()->FigureTapZ);

			fprintf(file, "ReCreateCount = %d\n", part->GetData()->ReCreateCount);

			fprintf(file, "AlphaBlendType = %d\n", part->GetData()->AlphaBlendType);

			fprintf(file, "ColorCoef = %f\n", part->GetData()->ColorCoef);

			fprintf(file, "Soft = %d\n", part->GetData()->Soft);
			fprintf(file, "SoftCoef = %f\n", part->GetData()->SoftCoef);

			fprintf(file, "Refraction = %d\n", part->GetData()->Refraction);
			fprintf(file, "RefractionCoef = %f\n", part->GetData()->RefractionCoef);

			fprintf(file, "TransparencyCoef = %f\n", part->GetData()->TransparencyCoef);
			fprintf(file, "Lighting = %d\n", part->GetData()->Lighting);
			fprintf(file, "CollisionDelete = %d\n", part->GetData()->CollisionDelete);
			fprintf(file, "\n");
		}
		fprintf(file, "----------------------------------------------------------------------\n\n");
	}

	fclose(file);
}

void Effects::Load(const char* path)
{
	if (!Core_0FileExists(path))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - failed to save, %s", gen_msg_location, path);
		return;
	}
		
	ISXLConfig* config = Core_OpLConfig(path);

	if (!config->SectionExists("effects"))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - main section 'effects' not found, %s", gen_msg_location, path);
		return;
	}

	if (!config->KeyExists("effects", "count"))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - main key 'count' in section 'effects' not found, %s", gen_msg_location, path);
		return;
	}

	int eff_count = String(config->GetKey("effects", "count")).ToInt();
	char eff_section_name[CONFIG_SECTION_MAX_LEN];
	char part_section_name[CONFIG_SECTION_MAX_LEN];

	for (int i = 0; i < eff_count; ++i)
	{
		sprintf(eff_section_name, "effect_%d", i);
		if (!config->SectionExists(eff_section_name))
		{
			reportf(REPORT_MSG_LEVEL_ERROR, "%s - not found effect [%s], %s", gen_msg_location, eff_section_name, path);
			return;
		}

		ID eff_id = this->EffectAdd(0);
		ArrID[eff_id]->Original = true;
		int eff_count_part = 0;

		if (config->KeyExists(eff_section_name, "name"))
			EffectNameSet(eff_id, config->GetKey(eff_section_name, "name"));

		if (config->KeyExists(eff_section_name, "emitters_count"))
			eff_count_part = String(config->GetKey(eff_section_name, "emitters_count")).ToInt();

		for (int k = 0; k < eff_count_part; ++k)
		{
			sprintf(part_section_name, "effect_%d_emitter_%d", i, k);
			if (!config->SectionExists(eff_section_name))
			{
				reportf(REPORT_MSG_LEVEL_ERROR, "%s - not found particles [%s], %s", gen_msg_location, part_section_name, path);
				return;
			}

			ParticlesData part;


			if (config->KeyExists(part_section_name, "BoundType"))
				part.BoundType = (ParticlesBoundType)String(config->GetKey(part_section_name, "BoundType")).ToInt();

			if (config->KeyExists(part_section_name, "BoundVec1X"))
				part.BoundVec1.x = String(config->GetKey(part_section_name, "BoundVec1X")).ToDouble();
			if (config->KeyExists(part_section_name, "BoundVec1Y"))
				part.BoundVec1.y = String(config->GetKey(part_section_name, "BoundVec1Y")).ToDouble();
			if (config->KeyExists(part_section_name, "BoundVec1Z"))
				part.BoundVec1.z = String(config->GetKey(part_section_name, "BoundVec1Z")).ToDouble();
			if (config->KeyExists(part_section_name, "BoundVec1W"))
				part.BoundVec1.w = String(config->GetKey(part_section_name, "BoundVec1W")).ToDouble();

			if (config->KeyExists(part_section_name, "BoundVec2X"))
				part.BoundVec2.x = String(config->GetKey(part_section_name, "BoundVec2X")).ToDouble();
			if (config->KeyExists(part_section_name, "BoundVec2Y"))
				part.BoundVec2.y = String(config->GetKey(part_section_name, "BoundVec2Y")).ToDouble();
			if (config->KeyExists(part_section_name, "BoundVec2Z"))
				part.BoundVec2.z = String(config->GetKey(part_section_name, "BoundVec2Z")).ToDouble();
			if (config->KeyExists(part_section_name, "BoundVec2W"))
				part.BoundVec2.w = String(config->GetKey(part_section_name, "BoundVec2W")).ToDouble();


			if (config->KeyExists(part_section_name, "SpawnPosType"))
				part.SpawnPosType = (ParticlesSpawnPosType)String(config->GetKey(part_section_name, "SpawnPosType")).ToInt();

			if (config->KeyExists(part_section_name, "SpawnOriginX"))
				part.SpawnOrigin.x = String(config->GetKey(part_section_name, "SpawnOriginX")).ToDouble();
			if (config->KeyExists(part_section_name, "SpawnOriginY"))
				part.SpawnOrigin.y = String(config->GetKey(part_section_name, "SpawnOriginY")).ToDouble();
			if (config->KeyExists(part_section_name, "SpawnOriginZ"))
				part.SpawnOrigin.z = String(config->GetKey(part_section_name, "SpawnOriginZ")).ToDouble();

			if (config->KeyExists(part_section_name, "SpawnOriginDisp"))
				part.SpawnOriginDisp = String(config->GetKey(part_section_name, "SpawnOriginDisp")).ToDouble();

			if (config->KeyExists(part_section_name, "SpawnBoundBindCreateXNeg"))
				part.SpawnBoundBindCreateXNeg = String(config->GetKey(part_section_name, "SpawnBoundBindCreateXNeg")).ToBool();
			if (config->KeyExists(part_section_name, "SpawnBoundBindCreateXPos"))
				part.SpawnBoundBindCreateXPos = String(config->GetKey(part_section_name, "SpawnBoundBindCreateXPos")).ToBool();
			if (config->KeyExists(part_section_name, "SpawnBoundBindCreateYNeg"))
				part.SpawnBoundBindCreateYNeg = String(config->GetKey(part_section_name, "SpawnBoundBindCreateYNeg")).ToBool();
			if (config->KeyExists(part_section_name, "SpawnBoundBindCreateYPos"))
				part.SpawnBoundBindCreateYPos = String(config->GetKey(part_section_name, "SpawnBoundBindCreateYPos")).ToBool();
			if (config->KeyExists(part_section_name, "SpawnBoundBindCreateZNeg"))
				part.SpawnBoundBindCreateZNeg = String(config->GetKey(part_section_name, "SpawnBoundBindCreateZNeg")).ToBool();
			if (config->KeyExists(part_section_name, "SpawnBoundBindCreateZPos"))
				part.SpawnBoundBindCreateZPos = String(config->GetKey(part_section_name, "SpawnBoundBindCreateZPos")).ToBool();

			if (config->KeyExists(part_section_name, "SpawnNextTime"))
				part.SpawnNextTime = String(config->GetKey(part_section_name, "SpawnNextTime")).ToUnsLongInt();
			if (config->KeyExists(part_section_name, "SpawnNextTimeDisp"))
				part.SpawnNextTimeDisp = String(config->GetKey(part_section_name, "SpawnNextTimeDisp")).ToUnsLongInt();


			if (config->KeyExists(part_section_name, "AnimTexCountCadrsX"))
				part.AnimTexCountCadrsX = String(config->GetKey(part_section_name, "AnimTexCountCadrsX")).ToInt();
			if (config->KeyExists(part_section_name, "AnimTexCountCadrsY"))
				part.AnimTexCountCadrsY = String(config->GetKey(part_section_name, "AnimTexCountCadrsY")).ToInt();
			if (config->KeyExists(part_section_name, "AnimTexRate"))
				part.AnimTexRate = String(config->GetKey(part_section_name, "AnimTexRate")).ToInt();
			if (config->KeyExists(part_section_name, "AnimTexRateDisp"))
				part.AnimTexRateDisp = String(config->GetKey(part_section_name, "AnimTexRateDisp")).ToInt();
			if (config->KeyExists(part_section_name, "AnimTexStartCadr"))
				part.AnimTexStartCadr = String(config->GetKey(part_section_name, "AnimTexStartCadr")).ToInt();
			if (config->KeyExists(part_section_name, "AnimTexStartCadrDisp"))
				part.AnimTexStartCadrDisp = String(config->GetKey(part_section_name, "AnimTexStartCadrDisp")).ToInt();


			if (config->KeyExists(part_section_name, "TimeLife"))
				part.TimeLife = String(config->GetKey(part_section_name, "TimeLife")).ToUnsLongInt();
			if (config->KeyExists(part_section_name, "TimeLifeDisp"))
				part.TimeLifeDisp = String(config->GetKey(part_section_name, "TimeLifeDisp")).ToUnsLongInt();

			if (config->KeyExists(part_section_name, "AlphaDependAge"))
				part.AlphaDependAge = (ParticlesDependType)String(config->GetKey(part_section_name, "AlphaDependAge")).ToInt();

			if (config->KeyExists(part_section_name, "SizeX"))
				part.Size.x = String(config->GetKey(part_section_name, "SizeX")).ToDouble();
			if (config->KeyExists(part_section_name, "SizeY"))
				part.Size.y = String(config->GetKey(part_section_name, "SizeY")).ToDouble();

			if (config->KeyExists(part_section_name, "SizeDisp"))
				part.SizeDisp = String(config->GetKey(part_section_name, "SizeDisp")).ToDouble();

			if (config->KeyExists(part_section_name, "SizeDependAge"))
				part.SizeDependAge = (ParticlesDependType)String(config->GetKey(part_section_name, "SizeDependAge")).ToInt();


			if (config->KeyExists(part_section_name, "VelocityX"))
				part.Velocity.x = String(config->GetKey(part_section_name, "VelocityX")).ToDouble();
			if (config->KeyExists(part_section_name, "VelocityY"))
				part.Velocity.y = String(config->GetKey(part_section_name, "VelocityY")).ToDouble();
			if (config->KeyExists(part_section_name, "VelocityZ"))
				part.Velocity.z = String(config->GetKey(part_section_name, "VelocityZ")).ToDouble();

			if (config->KeyExists(part_section_name, "VelocityDisp"))
				part.VelocityDisp = String(config->GetKey(part_section_name, "VelocityDisp")).ToDouble();

			if (config->KeyExists(part_section_name, "VelocityDispXNeg"))
				part.VelocityDispXNeg = String(config->GetKey(part_section_name, "VelocityDispXNeg")).ToDouble();
			if (config->KeyExists(part_section_name, "VelocityDispYNeg"))
				part.VelocityDispYNeg = String(config->GetKey(part_section_name, "VelocityDispYNeg")).ToDouble();
			if (config->KeyExists(part_section_name, "VelocityDispZNeg"))
				part.VelocityDispZNeg = String(config->GetKey(part_section_name, "VelocityDispZNeg")).ToDouble();


			if (config->KeyExists(part_section_name, "AccelerationX"))
				part.Acceleration.x = String(config->GetKey(part_section_name, "AccelerationX")).ToDouble();
			if (config->KeyExists(part_section_name, "AccelerationY"))
				part.Acceleration.y = String(config->GetKey(part_section_name, "AccelerationY")).ToDouble();
			if (config->KeyExists(part_section_name, "AccelerationZ"))
				part.Acceleration.z = String(config->GetKey(part_section_name, "AccelerationZ")).ToDouble();

			if (config->KeyExists(part_section_name, "AccelerationDisp"))
				part.AccelerationDisp = String(config->GetKey(part_section_name, "AccelerationDisp")).ToDouble();

			if (config->KeyExists(part_section_name, "AccelerationDispXNeg"))
				part.AccelerationDispXNeg = String(config->GetKey(part_section_name, "AccelerationDispXNeg")).ToBool();
			if (config->KeyExists(part_section_name, "AccelerationDispYNeg"))
				part.AccelerationDispYNeg = String(config->GetKey(part_section_name, "AccelerationDispYNeg")).ToBool();
			if (config->KeyExists(part_section_name, "AccelerationDispZNeg"))
				part.AccelerationDispZNeg = String(config->GetKey(part_section_name, "AccelerationDispZNeg")).ToBool();


			if (config->KeyExists(part_section_name, "CharacterCircle"))
				part.CharacterCircle = String(config->GetKey(part_section_name, "CharacterCircle")).ToBool();
			if (config->KeyExists(part_section_name, "CharacterCircleAxis"))
				part.CharacterCircleAxis = (ParticlesAxis)String(config->GetKey(part_section_name, "CharacterCircleAxis")).ToInt();
			if (config->KeyExists(part_section_name, "CharacterCircleAngle"))
				part.CharacterCircleAngle = String(config->GetKey(part_section_name, "CharacterCircleAngle")).ToDouble();
			if (config->KeyExists(part_section_name, "CharacterCircleAngleDisp"))
				part.CharacterCircleAngleDisp = String(config->GetKey(part_section_name, "CharacterCircleAngleDisp")).ToDouble();
			if (config->KeyExists(part_section_name, "CharacterCircleAngleDispNeg"))
				part.CharacterCircleAngleDispNeg = String(config->GetKey(part_section_name, "CharacterCircleAngleDispNeg")).ToBool();


			if (config->KeyExists(part_section_name, "CharacterRotate"))
				part.CharacterRotate = String(config->GetKey(part_section_name, "CharacterRotate")).ToBool();
			if (config->KeyExists(part_section_name, "CharacterRotateAngle"))
				part.CharacterRotateAngle = String(config->GetKey(part_section_name, "CharacterRotateAngle")).ToDouble();
			if (config->KeyExists(part_section_name, "CharacterRotateAngleDisp"))
				part.CharacterRotateAngleDisp = String(config->GetKey(part_section_name, "CharacterRotateAngleDisp")).ToDouble();
			if (config->KeyExists(part_section_name, "CharacterRotateAngleDispNeg"))
				part.CharacterRotateAngleDispNeg = String(config->GetKey(part_section_name, "CharacterRotateAngleDispNeg")).ToBool();


			if (config->KeyExists(part_section_name, "CharacterDeviation"))
				part.CharacterDeviation = String(config->GetKey(part_section_name, "CharacterDeviation")).ToBool();
			if (config->KeyExists(part_section_name, "CharacterDeviationType"))
				part.CharacterDeviationType = (ParticlesDeviationType)String(config->GetKey(part_section_name, "CharacterDeviationType")).ToInt();
			if (config->KeyExists(part_section_name, "CharacterDeviationAmplitude"))
				part.CharacterDeviationAmplitude = String(config->GetKey(part_section_name, "CharacterDeviationAmplitude")).ToDouble();
			if (config->KeyExists(part_section_name, "CharacterDeviationCoefAngle"))
				part.CharacterDeviationCoefAngle = String(config->GetKey(part_section_name, "CharacterDeviationCoefAngle")).ToDouble();
			if (config->KeyExists(part_section_name, "CharacterDeviationAxis"))
				part.CharacterDeviationAxis = (ParticlesAxis)String(config->GetKey(part_section_name, "CharacterDeviationAxis")).ToInt();
			if (config->KeyExists(part_section_name, "CharacterDeviationCountDelayMls"))
				part.CharacterDeviationCountDelayMls = String(config->GetKey(part_section_name, "CharacterDeviationCountDelayMls")).ToUnsLongInt();
			if (config->KeyExists(part_section_name, "CharacterDeviationCoefAngleDisp"))
				part.CharacterDeviationCoefAngleDisp = String(config->GetKey(part_section_name, "CharacterDeviationCoefAngleDisp")).ToDouble();
			if (config->KeyExists(part_section_name, "CharacterDeviationCoefAngleDispNeg"))
				part.CharacterDeviationCoefAngleDispNeg = String(config->GetKey(part_section_name, "CharacterDeviationCoefAngleDispNeg")).ToBool();

			if (config->KeyExists(part_section_name, "CharacterDeviationTapX"))
				part.CharacterDeviationTapX = String(config->GetKey(part_section_name, "CharacterDeviationTapX")).ToBool();
			if (config->KeyExists(part_section_name, "CharacterDeviationTapY"))
				part.CharacterDeviationTapY = String(config->GetKey(part_section_name, "CharacterDeviationTapY")).ToBool();
			if (config->KeyExists(part_section_name, "CharacterDeviationTapZ"))
				part.CharacterDeviationTapZ = String(config->GetKey(part_section_name, "CharacterDeviationTapZ")).ToBool();


			if (config->KeyExists(part_section_name, "FigureType"))
				part.FigureType = (ParticlesFigureType)String(config->GetKey(part_section_name, "FigureType")).ToInt();
			if (config->KeyExists(part_section_name, "CharacterDeviationAxis"))
				part.FigureCountQuads = String(config->GetKey(part_section_name, "FigureCountQuads")).ToInt();

			if (config->KeyExists(part_section_name, "FigureRotRand"))
				part.FigureRotRand = String(config->GetKey(part_section_name, "FigureRotRand")).ToBool();

			if (config->KeyExists(part_section_name, "FigureTapX"))
				part.FigureTapX = String(config->GetKey(part_section_name, "FigureTapX")).ToBool();
			if (config->KeyExists(part_section_name, "FigureTapY"))
				part.FigureTapY = String(config->GetKey(part_section_name, "FigureTapY")).ToBool();
			if (config->KeyExists(part_section_name, "FigureTapZ"))
				part.FigureTapZ = String(config->GetKey(part_section_name, "FigureTapZ")).ToBool();

			if (config->KeyExists(part_section_name, "ReCreateCount"))
				part.ReCreateCount = String(config->GetKey(part_section_name, "ReCreateCount")).ToInt();

			if (config->KeyExists(part_section_name, "AlphaBlendType"))
				part.AlphaBlendType = (ParticlesAlphaBlendType)String(config->GetKey(part_section_name, "AlphaBlendType")).ToInt();

			if (config->KeyExists(part_section_name, "ColorCoef"))
				part.ColorCoef = String(config->GetKey(part_section_name, "ColorCoef")).ToDouble();

			if (config->KeyExists(part_section_name, "Soft"))
				part.Soft = String(config->GetKey(part_section_name, "Soft")).ToBool();
			if (config->KeyExists(part_section_name, "SoftCoef"))
				part.SoftCoef = String(config->GetKey(part_section_name, "SoftCoef")).ToDouble();

			if (config->KeyExists(part_section_name, "Refraction"))
				part.Refraction = String(config->GetKey(part_section_name, "Refraction")).ToBool();
			if (config->KeyExists(part_section_name, "RefractionCoef"))
				part.RefractionCoef = String(config->GetKey(part_section_name, "RefractionCoef")).ToDouble();

			if (config->KeyExists(part_section_name, "TransparencyCoef"))
				part.TransparencyCoef = String(config->GetKey(part_section_name, "TransparencyCoef")).ToDouble();
			if (config->KeyExists(part_section_name, "Lighting"))
				part.Lighting = String(config->GetKey(part_section_name, "Lighting")).ToBool();
			if (config->KeyExists(part_section_name, "CollisionDelete"))
				part.CollisionDelete = String(config->GetKey(part_section_name, "CollisionDelete")).ToBool();

			
			ID part_id = this->EmitterAdd(eff_id, &part);

			if (config->KeyExists(part_section_name, "name"))
				EmitterNameSet(eff_id, part_id, config->GetKey(part_section_name, "name"));

			if (config->KeyExists(part_section_name, "count"))
				EmitterCountSet(eff_id, part_id, String(config->GetKey(part_section_name, "count")).ToInt());

			if (config->KeyExists(part_section_name, "texture"))
				EmitterTextureSet(eff_id, part_id, config->GetKey(part_section_name, "texture"));
		}
	}

	mem_release_del(config);
}



void Effects::EmitterNameSet(ID id, ID id_part, const char* name)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->NameSet(name);
}

void Effects::EmitterNameGet(ID id, ID id_part, char* name)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->NameGet(name);
}

ID Effects::EmitterAdd(ID id, ParticlesData* data)
{
	EFFECTS_EFFECT_PRECOND(id, -1);

	Emitter* tmppart = new Emitter();
	tmppart->Init(data);
	ArrID[id]->Arr.push_back(tmppart);
	return ArrID[id]->Arr.size() - 1;
}

void Effects::EmitterReInit(ID id, ID id_part, ParticlesData* data)
{
	EFFECTS_PARTICLES_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->Init(data);
}

int Effects::EmitterGetCount(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, -1);
	return ArrID[id]->Arr.size();
}

void Effects::EmitterDelete(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, _VOID);
	mem_delete(ArrID[id]->Arr[id_part]);
	ArrID[id]->Arr.erase(id_part);
}

ParticlesData* Effects::EmitterGetData(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->GetData();
}

void Effects::EmitterCountSet(ID id, ID id_part, int count)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->CountSet(count);
}

int Effects::EmitterCountGet(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->CountGet();
}

int Effects::EmitterCountLifeGet(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->CountLifeGet();
}

void Effects::EmitterEnableSet(ID id, ID id_part, bool enable)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->EnableSet(enable);
}

bool Effects::EmitterEnableGet(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, false);

	return ArrID[id]->Arr[id_part]->EnableGet();
}

void Effects::EmitterTextureSet(ID id, ID id_part, const char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->TextureSet(tex);
}

void Effects::EmitterTextureSetID(ID id, ID id_part, ID tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->TextureSetID(tex);
}

ID Effects::EmitterTextureGetID(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, -1);

	return ArrID[id]->Arr[id_part]->TextureGetID();
}

void Effects::EmitterTextureGet(ID id, ID id_part, char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->TextureGet(tex);
}




ID Effects::EffectCopyID(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, -1);

	Effect* efforigin = ArrID[id];
	Effect* tmpeffect = new Effect(*efforigin);

	return AddEffect(tmpeffect);
}

ID Effects::EffectCopyName(const char* name)
{
	return EffectCopyID(EffectGetByName(name));
}

ID Effects::EffectGetByName(const char* name)
{
	for (int i = 0; i < ArrKey.size(); ++i)
	{
		if (strcmp(name, ArrKey[i]->Name) == 0)
			return ArrKey[i]->Id;
	}

	return -1;
}

ID Effects::EffectAdd(const char* name)
{
	Effect* tmpeffect = new Effect();
	if (name)
		strcpy(tmpeffect->Name, name);

	ID id = AddEffect(tmpeffect);
	tmpeffect->IDPool = PoolAdd(id);
	//PoolExtend(tmpeffect->Pool);
	return id;
}

ID Effects::PoolAdd(ID ideff)
{
	EFFECTS_EFFECT_PRECOND(ideff, -1);

	ID id = -1;
	for (int i = 0; i < Pools.size(); ++i)
	{
		if (Pools[i]->ideff < 0)
		{
			Pools[i]->arr.clear();
			id = i;
			break;
		}
	}

	if (id < 0)
	{
		Pool* tmparr = new Pool();
		tmparr->ideff = ideff;
		Pools.push_back(tmparr);
		id = Pools.size() - 1;
	}

	return Pools.size() - 1;
}

void Effects::PoolExtend(ID id)
{
	EFFECTS_POOL_PRECOND(id, _VOID);
	for (int i = 0; i < SXPARTICLES_POOL_RESERVE; ++i)
	{
		ID tmpid = EffectCopyID(Pools[id]->ideff);
		ArrID[tmpid]->Busy = false;
		ArrID[tmpid]->Original = false;
		Pools[id]->arr.push_back(tmpid);
	}
}

void Effects::PoolDelete(ID id)
{
	EFFECTS_POOL_PRECOND(id, _VOID);

	for (int i = 0; i < Pools[id]->arr.size(); ++i)
	{
		EffectDel(Pools[id]->arr[i]);
	}

	Pools[id]->ideff = -1;
	Pools[id]->arr.clear();
}

ID Effects::PoolGet(ID id)
{
	EFFECTS_POOL_PRECOND(id, -1);

	for (int i = 0; i < Pools[id]->arr.size(); ++i)
	{
		if (!(ArrID[Pools[id]->arr[i]]->Busy) && !(ArrID[Pools[id]->arr[i]]->Enable))
			return ArrID[Pools[id]->arr[i]]->Id;
	}
	
	int oldsize = Pools[id]->arr.size();

	PoolExtend(id);
	return ArrID[Pools[id]->arr[oldsize]]->Id;
}

ID Effects::EffectInstanceByID(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, -1);

	ID ideff = PoolGet(ArrID[id]->IDPool);
	ArrID[ideff]->Busy = true;

	return ideff;
}

ID Effects::EffectInstanceByName(const char* name)
{
	return EffectInstanceByID(EffectGetByName(name));
}

void Effects::EffectPlayByID(ID id, float3* pos, float3* dir)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ID ideff = PoolGet(ArrID[id]->IDPool);

	if (pos)
		EffectPosSet(ideff, pos);

	if (dir)
		EffectDirSet(ideff, dir);

	EffectEnableSet(ideff, true);
}

void Effects::EffectPlayByName(const char* name, float3* pos, float3* dir)
{
	EffectPlayByID(EffectGetByName(name), pos, dir);
}

ID Effects::AddEffect(Effect* obj)
{
	ID idadd = -1;
	for (long i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i] == 0)
		{
			ArrID[i] = obj;
			idadd = i;
			break;
		}
	}

	if (idadd == -1)
	{
		ArrID.push_back(obj);
		idadd = ArrID.size() - 1;
	}

	obj->Id = idadd;
	ArrKey.push_back(obj);
	obj->Key = ArrKey.size() - 1;
	return idadd;
}

int Effects::EffectCountGet()
{
	return ArrKey.size();
}

ID Effects::EffectIdOfKey(ID key)
{
	EFFECTS_EFFECT_PRECOND_KEY(key, -1);

	return ArrKey[key]->Id;
}

void Effects::EffectDelete(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->Original)
	{
		PoolDelete(ArrID[id]->IDPool);
		EffectDel(id);
	}
	else
	{
		ArrID[id]->Busy = false;
		EffectEnableSet(id, false);
	}
}

void Effects::EffectDel(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	for (long i = ArrID[id]->Key + 1; i < ArrKey.size(); ++i)
	{
		--(ArrKey[i]->Key);
	}

	ArrID[id]->Enable = false;
	ArrKey.erase(ArrID[id]->Key);
	mem_delete(ArrID[id]);
}

void Effects::EffectNameSet(ID id, const char* name)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (name)
		strcpy(ArrID[id]->Name, name);
}

void Effects::EffectNameGet(ID id, char* name)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (name)
		strcpy(name, ArrID[id]->Name);
}

void Effects::EffectCompute(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (!ArrID[id]->Enable)
		return;

	Effect* eff = ArrID[id];
	int countlife = 0;

	static float4x4 mattrans;
	mattrans = eff->MatRotate * eff->MatTranslation;

	int countdead = 0;	//счетик живых партиклов
	for (int i = 0, l = eff->Arr.size(); i < l; ++i)
	{
		if (eff->Arr[i])
			eff->Arr[i]->Compute(&mattrans);

		//если партиклы метрвы то инкрементируем счетчик
		if (!eff->Arr[i]->EnableGet())
			++countdead;
		else //иначе партиклы живы, считаем объем эффекта
		{
			++countlife;
			if (countlife == 1)
			{
				eff->CurrMax = eff->Arr[i]->CurrMax;
				eff->CurrMin = eff->Arr[i]->CurrMin;
			}
			else
			{
				if (eff->Arr[i]->CurrMax.x > eff->CurrMax.x)
					eff->CurrMax.x = eff->Arr[i]->CurrMax.x;

				if (eff->Arr[i]->CurrMax.y > eff->CurrMax.y)
					eff->CurrMax.y = eff->Arr[i]->CurrMax.y;

				if (eff->Arr[i]->CurrMax.z > eff->CurrMax.z)
					eff->CurrMax.z = eff->Arr[i]->CurrMax.z;

				if (eff->Arr[i]->CurrMin.x < eff->CurrMin.x)
					eff->CurrMin.x = eff->Arr[i]->CurrMin.x;

				if (eff->Arr[i]->CurrMin.y < eff->CurrMin.y)
					eff->CurrMin.y = eff->Arr[i]->CurrMin.y;

				if (eff->Arr[i]->CurrMin.z < eff->CurrMin.z)
					eff->CurrMin.z = eff->Arr[i]->CurrMin.z;
			}
			
			eff->CurrMin2 = SMVector3Transform(eff->CurrMin, mattrans);
			eff->CurrMax2 = SMVector3Transform(eff->CurrMax, mattrans);
		}
	}

	//если количество мертвых объектов партиклов равно общему количесву объектов партиклов
	if (countdead == eff->Arr.size())
	{
		eff->Enable = false;	//эффект мертв
		eff->Alife = false;
	}
	else
		eff->Enable = true;
}

void Effects::EffectComputeLighting(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	Effect* eff = ArrID[id];

	if (!eff->ViewRender)
		return;

	if (!eff->Enable)
		return;

	for(int i = 0, l = eff->Arr.size(); i < l; ++i)
	{
		if (eff->Arr[i] && eff->Arr[i]->EnableGet())
		{
			eff->Arr[i]->ComputeLighting();
		}
	}
}

void Effects::EffectRender(ID id, DWORD timeDelta)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	Effect* eff = ArrID[id];

	if (!eff->ViewRender)
		return;

	if (!eff->Enable)
		return;

	for(int i = 0, l = eff->Arr.size(); i < l; ++i)
	{
		if(eff->Arr[i] && eff->Arr[i]->EnableGet())
		{
			eff->Arr[i]->Render(timeDelta, &eff->MatRotate, &eff->MatTranslation);
		}
	}
}

void Effects::EffectComputeAll()
{
	for (int i = 0, l = ArrID.size(); i < l; ++i)
	{
		if (ArrID[i])
			EffectCompute(ArrID[i]->Id);
	}
}

void Effects::EffectComputeLightingAll()
{
	for (int i = 0, l = ArrID.size(); i < l; ++i)
	{
		if (ArrID[i])
			EffectComputeLighting(ArrID[i]->Id);
	}
}

void Effects::EffectRenderAll(DWORD timeDelta)
{
	ID tmpid = -1;
	if (ArrSortSizeCurr <= 0)
		return;

	for (int i = ArrSortSizeCurr-1; i >= 0; --i)
	{
		tmpid = ArrSort[i];
		if (tmpid >= 0 && tmpid < ArrID.size() && ArrID[tmpid])
			EffectRender(tmpid, timeDelta);
	}
}

bool Effects::EffectVisibleCom(ID id, ISXFrustum* frustum, float3* view)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	Effect* eff = ArrID[id];
	if (!eff || !eff->Enable)
		return false;

	float3 scenter;
	float sradius;

	scenter = (eff->CurrMin2 + eff->CurrMax2) * 0.5f;
	sradius = SMVector3Length(scenter - eff->CurrMax2);
	
	eff->ViewRender = frustum->SphereInFrustum(&scenter, sradius);

	eff->ViewDist = SMVector3Length((scenter - (*view))) - sradius;

	return eff->ViewRender;
}

void Effects::EffectVisibleComAll(ISXFrustum* frustum, float3* view)
{
	for(int i = 0, l = ArrID.size(); i < l; ++i)
	{
		if (ArrID[i])
			EffectVisibleCom(i, frustum, view);
	}

	for (int i = 0; i < /*ArrSort.size()*/ArrKey.size(); ++i)
	{
		ArrSort[i] = -1;
	}

	ArrSortSizeCurr = 0;

	float tmpdist;
	int pos;
	for (int i = 0; i < ArrKey.size(); ++i)
	{
		if (!ArrKey[i]->ViewRender)
			continue;

		pos = 0;
		tmpdist = ArrKey[i]->ViewDist;
		for (int j = 0; j < ArrKey.size(); ++j)
		{
			if (i != j && ArrKey[j]->ViewRender && ArrKey[j]->ViewDist < tmpdist)
				++pos;
		}

		ArrSort[pos] = ArrKey[i]->Id;

		if (ArrSortSizeCurr <= pos)
			ArrSortSizeCurr = pos+1;
	}

	/*if (GetAsyncKeyState('Y'))
	{
		for (int i = 0; i < ArrSortSizeCurr; ++i)
		{
			//reportf(0, "ArrSort[%d] = %d\n", i, ArrSort[i]);
			if (ArrSort[i] >= 0)
				reportf(0, "ViewDist = %f\n", ArrID[ArrSort[i]]->ViewDist);
		}

		reportf(0, "%d---------\n", ArrSortSizeCurr);
	}*/
}

bool Effects::EffectVisibleGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->ViewRender;
}

float Effects::EffectDistToViewGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, 0);

	return ArrID[id]->ViewDist;
}


bool Effects::EffectEnableGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->Enable;
}

void Effects::EffectEnableSet(ID id, bool isenable)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);
	Effect* eff = ArrID[id];
	
	for (int i = 0; i < eff->Arr.size(); ++i)
	{
		eff->Arr[i]->EnableSet(isenable);
	}

	eff->Enable = isenable;
	
	if (!isenable)
		eff->Alife = isenable;
}

bool Effects::EffectAlifeGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->Alife;
}

void Effects::EffectAlifeSet(ID id, bool alife)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ArrID[id]->Alife = alife;
	Effect* eff = ArrID[id];

	if (!eff->Enable && eff->Alife)
		eff->Enable = eff->Alife;

	for (int i = 0; i < eff->Arr.size(); ++i)
	{
		eff->Arr[i]->AlifeSet(alife);
	}
}

void Effects::EffectPosSet(ID id, float3* pos)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ArrID[id]->Position = *pos;
	ArrID[id]->MatTranslation = SMMatrixTranslation(ArrID[id]->Position);
}

void Effects::EffectDirSet(ID id, float3* dir)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);
	float3 tmpdir = *dir;
	//tmpdir.y = 0;
	tmpdir = SMVector3Normalize(tmpdir);
	ArrID[id]->Direction = tmpdir;

	//static float3 f = SXPARTICLES_BASIS_DIR;
	static float3 f(0, 1, 0);
	//float3 f(0, 1, 0);
	float3 a = SMVector3Cross(f, tmpdir);
	float ang = acosf(SMVector3Dot(f, tmpdir));
	ArrID[id]->MatRotate = SMMatrixRotationAxis(a, ang)/*SMQuaternion(a, ang).GetMatrix()*//*SMMatrixRotationX(SM_PIDIV2) * *//*SMMatrixRotationAxis(a, ang)*/;

	ArrID[id]->Rotation = SMMatrixToEuler(ArrID[id]->MatRotate);
}

void Effects::EffectRotSet(ID id, float3* rot)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ArrID[id]->Rotation = *rot;

	ArrID[id]->Direction = SMEulerToVec(ArrID[id]->Rotation);

	static float3 f = SXPARTICLES_BASIS_DIR;
	//float3 f(0, -1, 0);
	float3 a = SMVector3Cross(f, ArrID[id]->Direction);
	float ang = acosf(SMVector3Dot(f, ArrID[id]->Direction));
	ArrID[id]->MatRotate = SMMatrixRotationAxis(a, ang);
}

void Effects::EffectRotSet(ID id, const SMQuaternion & rot)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ArrID[id]->MatRotate = rot.GetMatrix();

	ArrID[id]->Direction = rot * float3(0, 1, 0);

	ArrID[id]->Rotation = SMMatrixToEuler(ArrID[id]->MatRotate);
}

void Effects::EffectPosGet(ID id, float3* pos)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	*pos = ArrID[id]->Position;
}

void Effects::EffectDirGet(ID id, float3* dir)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	*dir = ArrID[id]->Direction;
}

void Effects::EffectRotGet(ID id, float3* rot)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	*rot = ArrID[id]->Rotation;
}