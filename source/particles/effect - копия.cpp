
#include <particles/effect.h>

#pragma once

Effects::Effects()
{
	
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
	
	//IsAlife = eff.IsAlife;

	IsEnable = eff.IsEnable;

	for (int i = 0; i < eff.Arr.size(); ++i)
	{
		Particles* part = new Particles(*eff.Arr[i]);
		Arr.push_back(part);
	}
}

Effects::Effect::~Effect()
{
	for (int i = 0; i < Arr.size(); ++i)
	{
		mem_delete(Arr[i]);
	}

	Arr.clear();
}

void Effects::Effect::NullingInit()
{
	Id = Key = -1;
	DistForCamera = 0.0f;
	IsRenderForCamera = false;

	IDLight = -1;
	//IsAlife = false;
	IsEnable = false;
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
		fprintf(file, "particles_count = %d\n\n", part_count);

		for (int k = 0; k < part_count; ++k)
		{
			Particles* part = ArrKey[i]->Arr[k];
			char tmpname[CONFIG_SECTION_MAX_LEN];
			part->NameGet(tmpname);
			char tmptex[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
			tmptex[0] = 0;

			if (part->TextureGetID() >= 0)
				SGCore_LoadTexGetName(part->TextureGetID(), tmptex);

			fprintf(file, "[effect_%d_particles_%d]\n", i, k);
			fprintf(file, "name = %s\n", tmpname);
			fprintf(file, "count = %d\n", part->CountGet());

			if (tmptex[0] != 0)
				fprintf(file, "texture = %s\n", tmptex);

			if (part->GetData()->BoundType == ParticlesBoundType::pbt_box)
			{
				fprintf(file, "bvec1x = %f\n", part->GetData()->BoundBoxMin.x);
				fprintf(file, "bvec1y = %f\n", part->GetData()->BoundBoxMin.y);
				fprintf(file, "bvec1z = %f\n", part->GetData()->BoundBoxMin.z);

				fprintf(file, "bvec2x = %f\n", part->GetData()->BoundBoxMax.x);
				fprintf(file, "bvec2y = %f\n", part->GetData()->BoundBoxMax.y);
				fprintf(file, "bvec2z = %f\n", part->GetData()->BoundBoxMax.z);
			}
			else if (part->GetData()->BoundType == ParticlesBoundType::pbt_sphere)
			{
				fprintf(file, "bvec1x = %f\n", part->GetData()->BoundSphereCenter.x);
				fprintf(file, "bvec1y = %f\n", part->GetData()->BoundSphereCenter.y);
				fprintf(file, "bvec1z = %f\n", part->GetData()->BoundSphereCenter.z);
				fprintf(file, "bvec1w = %f\n", part->GetData()->BoundSphereRadius);
			}
			else if (part->GetData()->BoundType == ParticlesBoundType::pbt_cone)
			{
				fprintf(file, "bvec1x = %f\n", part->GetData()->BoundConeBeginPos.x);
				fprintf(file, "bvec1y = %f\n", part->GetData()->BoundConeBeginPos.y);
				fprintf(file, "bvec1z = %f\n", part->GetData()->BoundConeBeginPos.z);
				fprintf(file, "bvec1w = %f\n", part->GetData()->BoundConeBeginPos.w);

				fprintf(file, "bvec2x = %f\n", part->GetData()->BoundConeEndPos.x);
				fprintf(file, "bvec2y = %f\n", part->GetData()->BoundConeEndPos.y);
				fprintf(file, "bvec2z = %f\n", part->GetData()->BoundConeEndPos.z);
				fprintf(file, "bvec2w = %f\n", part->GetData()->BoundConeEndPos.w);
			}


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
			fprintf(file, "AlphaAgeDepend = %d\n", part->GetData()->AlphaAgeDepend);

			fprintf(file, "SizeParticleX = %f\n", part->GetData()->SizeParticle.x);
			fprintf(file, "SizeParticleY = %f\n", part->GetData()->SizeParticle.y);
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

			fprintf(file, "Soft = %d\n", part->GetData()->Soft);
			fprintf(file, "SoftCoef = %f\n", part->GetData()->SoftCoef);

			fprintf(file, "Refraction = %d\n", part->GetData()->Refraction);
			fprintf(file, "RefractionCoef = %f\n", part->GetData()->RefractionCoef);

			fprintf(file, "TransparencyCoef = %f\n", part->GetData()->TransparencyCoef);
			fprintf(file, "Lighting = %d\n", part->GetData()->Lighting);
			fprintf(file, "\n");
		}
		fprintf(file, "----------------------------------------------------------------------");
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
		int eff_count_part = 0;

		if (config->KeyExists(eff_section_name, "name"))
			EffectNameSet(eff_id, config->GetKey(eff_section_name, "name"));

		if (config->KeyExists(eff_section_name, "particles_count"))
			eff_count_part = String(config->GetKey(eff_section_name, "particles_count")).ToInt();

		for (int k = 0; k < eff_count_part; ++k)
		{
			sprintf(part_section_name, "effect_%d_particles_%d", i, k);
			if (!config->SectionExists(eff_section_name))
			{
				reportf(REPORT_MSG_LEVEL_ERROR, "%s - not found particles [%s], %s", gen_msg_location, part_section_name, path);
				return;
			}

			ParticlesData part;


			if (config->KeyExists(part_section_name, "BoundType"))
				part.BoundType = (ParticlesBoundType)String(config->GetKey(part_section_name, "BoundType")).ToInt();

			if (config->KeyExists(part_section_name, "bvec1x"))
				part.BoundSphereCenter.x = part.BoundBoxMin.x = part.BoundConeBeginPos.x = String(config->GetKey(part_section_name, "bvec1x")).ToDouble();
			if (config->KeyExists(part_section_name, "bvec1y"))
				part.BoundSphereCenter.y = part.BoundBoxMin.y = part.BoundConeBeginPos.y = String(config->GetKey(part_section_name, "bvec1y")).ToDouble();
			if (config->KeyExists(part_section_name, "bvec1z"))
				part.BoundSphereCenter.z = part.BoundBoxMin.z = part.BoundConeBeginPos.z = String(config->GetKey(part_section_name, "bvec1z")).ToDouble();
			if (config->KeyExists(part_section_name, "bvec1w"))
				part.BoundSphereRadius = part.BoundConeBeginRadius = String(config->GetKey(part_section_name, "bvec1w")).ToDouble();

			if (config->KeyExists(part_section_name, "bvec2x"))
				part.BoundBoxMax.x = part.BoundConeEndPos.x = String(config->GetKey(part_section_name, "bvec2x")).ToDouble();
			if (config->KeyExists(part_section_name, "bvec2y"))
				part.BoundBoxMax.y = part.BoundConeEndPos.y = String(config->GetKey(part_section_name, "bvec2y")).ToDouble();
			if (config->KeyExists(part_section_name, "bvec2z"))
				part.BoundBoxMax.z = part.BoundConeEndPos.z = String(config->GetKey(part_section_name, "bvec2z")).ToDouble();
			if (config->KeyExists(part_section_name, "bvec2w"))
				part.BoundConeEndRadius = String(config->GetKey(part_section_name, "bvec2w")).ToDouble();


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

			if (config->KeyExists(part_section_name, "AlphaAgeDepend"))
				part.AlphaAgeDepend = (ParticlesDependType)String(config->GetKey(part_section_name, "AlphaAgeDepend")).ToInt();

			if (config->KeyExists(part_section_name, "SizeParticleX"))
				part.SizeParticle.x = String(config->GetKey(part_section_name, "SizeParticleX")).ToDouble();
			if (config->KeyExists(part_section_name, "SizeParticleY"))
				part.SizeParticle.y = String(config->GetKey(part_section_name, "SizeParticleY")).ToDouble();

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

			
			ID part_id = this->ParticlesAdd(eff_id, &part);

			if (config->KeyExists(part_section_name, "name"))
				ParticlesNameSet(eff_id, part_id, config->GetKey(part_section_name, "name"));

			if (config->KeyExists(part_section_name, "count"))
				ParticlesCountSet(eff_id, part_id, String(config->GetKey(part_section_name, "count")).ToInt());

			if (config->KeyExists(part_section_name, "texture"))
				ParticlesTextureSet(eff_id, part_id, config->GetKey(part_section_name, "texture"));
		}
	}

	mem_release_del(config);
}



void Effects::ParticlesNameSet(ID id, ID id_part, const char* name)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->NameSet(name);
}

void Effects::ParticlesNameGet(ID id, ID id_part, char* name)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->NameGet(name);
}

ID Effects::ParticlesAdd(ID id, ParticlesData* data)
{
	EFFECTS_EFFECT_PRECOND(id, -1);

	Particles* tmppart = new Particles();
	tmppart->Init(data);
	ArrID[id]->Arr.push_back(tmppart);
	return ArrID[id]->Arr.size() - 1;
}

void Effects::ParticlesReInit(ID id, ID id_part, ParticlesData* data)
{
	EFFECTS_PARTICLES_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->Init(data);
}

int Effects::ParticlesGetCount(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, -1);
	return ArrID[id]->Arr.size();
}

void Effects::ParticlesDelete(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, _VOID);
}

ParticlesData* Effects::ParticlesGetData(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->GetData();
}

void Effects::ParticlesCountSet(ID id, ID id_part, int count)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->CountSet(count);
}

int Effects::ParticlesCountGet(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->CountGet();
}

void Effects::ParticlesEnableSet(ID id, ID id_part, bool enable)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->CountGet();
}

bool Effects::ParticlesEnableGet(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, false);

	return ArrID[id]->Arr[id_part]->EnableGet();
}

/*void Effects::ParticlesCreate(ID id, ID id_part, int count)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->Create(count);
}

void Effects::ParticlesReCreate(ID id, ID id_part, int count)
{
	EFFECTS_PRECOND(id, id_part,  _VOID);

	ArrID[id]->Arr[id_part]->ReCreate(count);
}*/

void Effects::ParticlesTextureSet(ID id, ID id_part, const char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->TextureSet(tex);
}

void Effects::ParticlesTextureSetID(ID id, ID id_part, ID tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->TextureSetID(tex);
}

ID Effects::ParticlesTextureGetID(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, -1);

	return ArrID[id]->Arr[id_part]->TextureGetID();
}

void Effects::ParticlesTextureGet(ID id, ID id_part, char* tex)
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

	return AddEffect(tmpeffect);
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
	for (long i = ArrID[id]->Key + 1; i < ArrKey.size(); ++i)
	{
		--(ArrKey[i]->Key);
	}

	ArrID[id]->IsEnable = false;
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

	if (!ArrID[id]->IsEnable)
		return;

	int countdead = 0;	//счетик живых партиклов
	for (int i = 0; i < ArrID[id]->Arr.size(); ++i)
	{
		if (ArrID[id]->Arr[i])
			ArrID[id]->Arr[i]->Compute();

		//если партиклы метрвы то инкрементируем счетчик
		if (!ArrID[id]->Arr[i]->GetAlife())
			++countdead;
		else //иначе партиклы живы, считаем объем эффекта
		{
			if (ArrID[id]->Arr[i]->CurrMax.x > ArrID[id]->CurrMax.x)
				ArrID[id]->CurrMax.x = ArrID[id]->Arr[i]->CurrMax.x;
			
			if (ArrID[id]->Arr[i]->CurrMax.y > ArrID[id]->CurrMax.y)
				ArrID[id]->CurrMax.y = ArrID[id]->Arr[i]->CurrMax.y;

			if (ArrID[id]->Arr[i]->CurrMax.z > ArrID[id]->CurrMax.z)
				ArrID[id]->CurrMax.z = ArrID[id]->Arr[i]->CurrMax.z;

			if (ArrID[id]->Arr[i]->CurrMin.x < ArrID[id]->CurrMin.x)
				ArrID[id]->CurrMin.x = ArrID[id]->Arr[i]->CurrMin.x;

			if (ArrID[id]->Arr[i]->CurrMin.y < ArrID[id]->CurrMin.y)
				ArrID[id]->CurrMin.y = ArrID[id]->Arr[i]->CurrMin.y;

			if (ArrID[id]->Arr[i]->CurrMin.z < ArrID[id]->CurrMin.z)
				ArrID[id]->CurrMin.z = ArrID[id]->Arr[i]->CurrMin.z;

			static float4x4 mattrans;
			mattrans = ArrID[id]->MatRotate * ArrID[id]->MatTranslation;
			
			ArrID[id]->CurrMin = SMVector3Transform(ArrID[id]->CurrMin, mattrans);
			ArrID[id]->CurrMin = SMVector3Transform(ArrID[id]->CurrMin, mattrans);
		}
	}

	//если количество мертвых объектов партиклов равно общему количесву объектов партиклов
	if (countdead == ArrID[id]->Arr.size())
		ArrID[id]->IsEnable = false;	//эффект мертв
	else
		ArrID[id]->IsEnable = true;

	EffectLightEnable(id, ArrID[id]->IsEnable);
}

void Effects::EffectComputeLighting(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (!ArrID[id]->IsRenderForCamera)
		return;

	if (!ArrID[id]->IsEnable)
		return;

	for (int i = 0; i < ArrID[id]->Arr.size(); ++i)
	{
		if (ArrID[id]->Arr[i] && ArrID[id]->Arr[i]->GetAlife())
		{
			ArrID[id]->Arr[i]->ComputeLighting();
		}
	}
}

void Effects::EffectRender(ID id, DWORD timeDelta)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (!ArrID[id]->IsRenderForCamera)
		return;

	if (!ArrID[id]->IsEnable)
		return;

	for (int i = 0; i < ArrID[id]->Arr.size(); ++i)
	{
		if (ArrID[id]->Arr[i] && ArrID[id]->Arr[i]->GetAlife())
		{
			ArrID[id]->Arr[i]->Render(timeDelta, &ArrID[id]->MatRotate, &ArrID[id]->MatTranslation);
		}
	}
}

void Effects::EffectComputeAll()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		EffectCompute(ArrID[i]->Id);
	}
}

void Effects::EffectComputeLightingAll()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		EffectComputeLighting(ArrID[i]->Id);
	}
}

void Effects::EffectRenderAll(DWORD timeDelta)
{
	ID tmpid = -1;
	for (int i = ArrSort.size()-1; i >= 0; --i)
	{
		tmpid = ArrSort[i];
		if (tmpid >= 0)
			EffectRender(tmpid, timeDelta);
	}
}

bool Effects::EffectVisibleCom(ID id, ISXFrustum* frustum, float3* view)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	float3 scenter;
	float sradius;

	scenter = (ArrID[id]->CurrMin + ArrID[id]->CurrMax) * 0.5f;
	sradius = SMVector3Length(scenter - ArrID[id]->CurrMax);

	ArrID[id]->IsRenderForCamera = frustum->SphereInFrustum(&scenter, sradius);

	ArrID[id]->DistForCamera = SMVector3Length((scenter - (*view))) - sradius;

	return ArrID[id]->IsRenderForCamera;
}

void Effects::EffectVisibleComAll(ISXFrustum* frustum, float3* view)
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		EffectVisibleCom(i, frustum, view);
	}

	for (int i = 0; i < ArrSort.size(); ++i)
	{
		ArrSort[i] = -1;
	}

	int tmpdist, pos;
	for (int i = 0; i < ArrKey.size(); ++i)
	{
		/*if (!ArrKey[i]->IsRenderForCamera)
			continue;*/

		pos = 0;
		tmpdist = ArrKey[i]->DistForCamera;
		for (int j = 0; j < ArrKey.size(); ++j)
		{
			if (i != j /*&& ArrKey[j]->IsRenderForCamera*/ && ArrKey[j]->DistForCamera < tmpdist)
				++pos;
		}

		ArrSort[pos] = ArrKey[i]->Id;
	}

	for (int i = 0; i < ArrSort.size(); ++i)
	{
		reportf(0, "ArrSort[%d] = %d\n", i, ArrSort[i]);
	}

	reportf(0, "---------\n");
}

bool Effects::EffectVisibleGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->IsRenderForCamera;
}

float Effects::EffectDistToViewGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, 0);

	return ArrID[id]->DistForCamera;
}


bool Effects::EffectAlifeGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->IsEnable;
}

bool Effects::EffectEnableGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->IsEnable;
}

void Effects::EffectEnableSet(ID id, bool isenable)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	//если предыдущее состояние эффекта было "выключен", а теперь включаем
	if (!ArrID[id]->IsEnable && isenable)
	{
		EffectLightEnable(id, true);
	}

	for (int i = 0; i < ArrID[id]->Arr.size(); ++i)
	{
		ArrID[id]->Arr[i]->EnableSet(isenable);
	}

	ArrID[id]->IsEnable = isenable;

	if (!ArrID[id]->IsEnable)
	{
		ArrID[id]->IsEnable = false;
		EffectLightEnable(id, false);
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
	tmpdir.y = 0;
	tmpdir = SMVector3Normalize(tmpdir);
	ArrID[id]->Direction = tmpdir;

	static float3 f = SXPARTICLES_BASIS_DIR;
	//float3 f(0, 1, 0);
	float3 a = SMVector3Cross(f, tmpdir);
	float ang = acosf(SMVector3Dot(f, tmpdir));
	ArrID[id]->MatRotate = /*SMMatrixRotationX(SM_PIDIV2) * */SMMatrixRotationAxis(a, ang);

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





void Effects::EffectLightSet(ID id, float3* color, float dist, bool is_shadow)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->IDLight >= 0)
	{
		SML_LigthsSetColor(ArrID[id]->IDLight, color);
		SML_LigthsSetDist(ArrID[id]->IDLight, dist, true);
		SML_LigthsSetPower(ArrID[id]->IDLight, dist / LIGHTS_LOCAL_MAX_POWER);
		SML_LigthsSetShadow(ArrID[id]->IDLight, is_shadow);
	}
	else
		ArrID[id]->IDLight = SML_LigthsCreatePoint(&ArrID[id]->Position, dist / LIGHTS_LOCAL_MAX_POWER, dist, color, false, is_shadow);
}

void Effects::EffectLightDelete(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->IDLight >= 0)
	{
		SML_LigthsDeleteLight(ArrID[id]->IDLight);
		ArrID[id]->IDLight = -1;
	}
}

void Effects::EffectLightEnable(ID id, bool is_enable)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->IDLight >= 0)
		SML_LigthsSetEnable(ArrID[id]->IDLight, is_enable);
}

void Effects::EffectLightRotationSet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->IDLight >= 0)
		SML_LigthsSetRot(ArrID[id]->IDLight, &ArrID[id]->Rotation, false);
}

void Effects::EffectLightPositionSet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->IDLight >= 0)
		SML_LigthsSetPos(ArrID[id]->IDLight, &ArrID[id]->Position);
}