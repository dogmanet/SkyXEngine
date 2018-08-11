
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "effect.h"

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

void Effects::onLostDevice()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i])
		{
			for (int k = 0; k < ArrID[i]->Arr.size(); ++k)
			{
				if (ArrID[i]->Arr[k])
					ArrID[i]->Arr[k]->onLostDevice();
			}
		}
	}
}

void Effects::onResetDevice()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i])
		{
			for (int k = 0; k < ArrID[i]->Arr.size(); ++k)
			{
				if (ArrID[i]->Arr[k])
					ArrID[i]->Arr[k]->onResetDevice();
			}
		}
	}
}

void Effects::clear()
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

void Effects::save(const char* path)
{
	FILE* file = 0;
	file = fopen(path, "w");

	if (!file)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - failed to save %s", GEN_MSG_LOCATION, path);
		return;
	}

	int eff_count = effectGetCount();

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
			part->getName(tmpname);
			char tmptex[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
			tmptex[0] = 0;

			if (part->getTextureID() >= 0)
				SGCore_LoadTexGetName(part->getTextureID(), tmptex);

			fprintf(file, "[effect_%d_emitter_%d]\n", i, k);
			fprintf(file, "name = %s\n", tmpname);
			fprintf(file, "count = %d\n", part->getCount());

			if (tmptex[0] != 0)
				fprintf(file, "texture = %s\n", tmptex);

			if (part->getTextureTrackID() >= 0)
				SGCore_LoadTexGetName(part->getTextureTrackID(), tmptex);

			if (tmptex[0] != 0)
				fprintf(file, "texture_track = %s\n", tmptex);

			fprintf(file, "Track = %d\n", part->getData()->Track);
			fprintf(file, "TrackSize = %f\n", part->getData()->TrackSize);
			fprintf(file, "TrackTime = %d\n", part->getData()->TrackTime);

			fprintf(file, "BoundType = %d\n", part->getData()->BoundType);

			fprintf(file, "BoundVec1X = %f\n", part->getData()->BoundVec1.x);
			fprintf(file, "BoundVec1Y = %f\n", part->getData()->BoundVec1.y);
			fprintf(file, "BoundVec1Z = %f\n", part->getData()->BoundVec1.z);
			fprintf(file, "BoundVec1W = %f\n", part->getData()->BoundVec1.w);

			fprintf(file, "BoundVec2X = %f\n", part->getData()->BoundVec2.x);
			fprintf(file, "BoundVec2Y = %f\n", part->getData()->BoundVec2.y);
			fprintf(file, "BoundVec2Z = %f\n", part->getData()->BoundVec2.z);
			fprintf(file, "BoundVec2W = %f\n", part->getData()->BoundVec2.w);


			fprintf(file, "SpawnPosType = %d\n", part->getData()->SpawnPosType);
			fprintf(file, "SpawnOriginX = %f\n", part->getData()->SpawnOrigin.x);
			fprintf(file, "SpawnOriginY = %f\n", part->getData()->SpawnOrigin.y);
			fprintf(file, "SpawnOriginZ = %f\n", part->getData()->SpawnOrigin.z);

			fprintf(file, "SpawnOriginDisp = %f\n", part->getData()->SpawnOriginDisp);

			fprintf(file, "SpawnBoundBindCreateXNeg = %d\n", part->getData()->SpawnBoundBindCreateXNeg);
			fprintf(file, "SpawnBoundBindCreateXPos = %d\n", part->getData()->SpawnBoundBindCreateXPos);
			fprintf(file, "SpawnBoundBindCreateYNeg = %d\n", part->getData()->SpawnBoundBindCreateYNeg);
			fprintf(file, "SpawnBoundBindCreateYPos = %d\n", part->getData()->SpawnBoundBindCreateYPos);
			fprintf(file, "SpawnBoundBindCreateZNeg = %d\n", part->getData()->SpawnBoundBindCreateZNeg);
			fprintf(file, "SpawnBoundBindCreateZPos = %d\n", part->getData()->SpawnBoundBindCreateZPos);

			fprintf(file, "SpawnNextTime = %d\n", part->getData()->SpawnNextTime);
			fprintf(file, "SpawnNextTimeDisp = %d\n", part->getData()->SpawnNextTimeDisp);


			fprintf(file, "AnimTexCountCadrsX = %d\n", part->getData()->AnimTexCountCadrsX);
			fprintf(file, "AnimTexCountCadrsY = %d\n", part->getData()->AnimTexCountCadrsY);

			fprintf(file, "AnimTexRate = %d\n", part->getData()->AnimTexRate);
			fprintf(file, "AnimTexRateDisp = %d\n", part->getData()->AnimTexRateDisp);
			fprintf(file, "AnimTexStartCadr = %d\n", part->getData()->AnimTexStartCadr);
			fprintf(file, "AnimTexStartCadrDisp = %d\n", part->getData()->AnimTexStartCadrDisp);


			fprintf(file, "TimeLife = %d\n", part->getData()->TimeLife);
			fprintf(file, "TimeLifeDisp = %d\n", part->getData()->TimeLifeDisp);
			fprintf(file, "AlphaDependAge = %d\n", part->getData()->AlphaDependAge);

			fprintf(file, "SizeX = %f\n", part->getData()->Size.x);
			fprintf(file, "SizeY = %f\n", part->getData()->Size.y);
			fprintf(file, "SizeDisp = %f\n", part->getData()->SizeDisp);

			fprintf(file, "SizeDependAge = %d\n", part->getData()->SizeDependAge);


			fprintf(file, "VelocityX = %f\n", part->getData()->Velocity.x);
			fprintf(file, "VelocityY = %f\n", part->getData()->Velocity.y);
			fprintf(file, "VelocityZ = %f\n", part->getData()->Velocity.z);
			fprintf(file, "VelocityDisp = %f\n", part->getData()->VelocityDisp);

			fprintf(file, "VelocityDispXNeg = %d\n", part->getData()->VelocityDispXNeg);
			fprintf(file, "VelocityDispYNeg = %d\n", part->getData()->VelocityDispYNeg);
			fprintf(file, "VelocityDispZNeg = %d\n", part->getData()->VelocityDispZNeg);


			fprintf(file, "AccelerationX = %f\n", part->getData()->Acceleration.x);
			fprintf(file, "AccelerationY = %f\n", part->getData()->Acceleration.y);
			fprintf(file, "AccelerationZ = %f\n", part->getData()->Acceleration.z);
			fprintf(file, "AccelerationDisp = %f\n", part->getData()->AccelerationDisp);

			fprintf(file, "AccelerationDispXNeg = %d\n", part->getData()->AccelerationDispXNeg);
			fprintf(file, "AccelerationDispYNeg = %d\n", part->getData()->AccelerationDispYNeg);
			fprintf(file, "AccelerationDispZNeg = %d\n", part->getData()->AccelerationDispZNeg);


			fprintf(file, "CharacterCircle = %d\n", part->getData()->CharacterCircle);
			fprintf(file, "CharacterCircleAxis = %d\n", part->getData()->CharacterCircleAxis);
			fprintf(file, "CharacterCircleAngle = %f\n", part->getData()->CharacterCircleAngle);
			fprintf(file, "CharacterCircleAngleDisp = %f\n", part->getData()->CharacterCircleAngleDisp);
			fprintf(file, "CharacterCircleAngleDispNeg = %d\n", part->getData()->CharacterCircleAngleDispNeg);


			fprintf(file, "CharacterRotate = %d\n", part->getData()->CharacterRotate);
			fprintf(file, "CharacterRotateAngle = %f\n", part->getData()->CharacterRotateAngle);
			fprintf(file, "CharacterRotateAngleDisp = %f\n", part->getData()->CharacterRotateAngleDisp);
			fprintf(file, "CharacterRotateAngleDispNeg = %d\n", part->getData()->CharacterRotateAngleDispNeg);


			fprintf(file, "CharacterDeviation = %d\n", part->getData()->CharacterDeviation);
			fprintf(file, "CharacterDeviationType = %d\n", part->getData()->CharacterDeviationType);
			fprintf(file, "CharacterDeviationAmplitude = %f\n", part->getData()->CharacterDeviationAmplitude);
			fprintf(file, "CharacterDeviationCoefAngle = %f\n", part->getData()->CharacterDeviationCoefAngle);
			fprintf(file, "CharacterDeviationAxis = %d\n", part->getData()->CharacterDeviationAxis);
			fprintf(file, "CharacterDeviationCountDelayMls = %d\n", part->getData()->CharacterDeviationCountDelayMls);
			fprintf(file, "CharacterDeviationCoefAngleDisp = %f\n", part->getData()->CharacterDeviationCoefAngleDisp);
			fprintf(file, "CharacterDeviationCoefAngleDispNeg = %d\n", part->getData()->CharacterDeviationCoefAngleDispNeg);

			fprintf(file, "CharacterDeviationTapX = %d\n", part->getData()->CharacterDeviationTapX);
			fprintf(file, "CharacterDeviationTapY = %d\n", part->getData()->CharacterDeviationTapY);
			fprintf(file, "CharacterDeviationTapZ = %d\n", part->getData()->CharacterDeviationTapZ);


			fprintf(file, "FigureType = %d\n", part->getData()->FigureType);
			fprintf(file, "FigureCountQuads = %d\n", part->getData()->FigureCountQuads);
			fprintf(file, "FigureRotRand = %d\n", part->getData()->FigureRotRand);

			fprintf(file, "FigureTapX = %d\n", part->getData()->FigureTapX);
			fprintf(file, "FigureTapY = %d\n", part->getData()->FigureTapY);
			fprintf(file, "FigureTapZ = %d\n", part->getData()->FigureTapZ);

			fprintf(file, "ReCreateCount = %d\n", part->getData()->ReCreateCount);

			fprintf(file, "AlphaBlendType = %d\n", part->getData()->AlphaBlendType);

			fprintf(file, "ColorCoef = %f\n", part->getData()->ColorCoef);

			fprintf(file, "Soft = %d\n", part->getData()->Soft);
			fprintf(file, "SoftCoef = %f\n", part->getData()->SoftCoef);

			fprintf(file, "Refraction = %d\n", part->getData()->Refraction);
			fprintf(file, "RefractionCoef = %f\n", part->getData()->RefractionCoef);

			fprintf(file, "TransparencyCoef = %f\n", part->getData()->TransparencyCoef);
			fprintf(file, "Lighting = %d\n", part->getData()->Lighting);
			fprintf(file, "CollisionDelete = %d\n", part->getData()->CollisionDelete);
			fprintf(file, "\n");
		}
		fprintf(file, "----------------------------------------------------------------------\n\n");
	}

	fclose(file);
}

void Effects::load(const char* path)
{
	if (!FileExistsFile(path))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed to save, %s", GEN_MSG_LOCATION, path);
		return;
	}
		
	ISXConfig* config = Core_OpConfig(path);

	if (!config->sectionExists("effects"))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - main section 'effects' not found, %s", GEN_MSG_LOCATION, path);
		return;
	}

	if (!config->keyExists("effects", "count"))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - main key 'count' in section 'effects' not found, %s", GEN_MSG_LOCATION, path);
		return;
	}

	int eff_count = String(config->getKey("effects", "count")).toInt();
	char eff_section_name[CONFIG_SECTION_MAX_LEN];
	char part_section_name[CONFIG_SECTION_MAX_LEN];

	for (int i = 0; i < eff_count; ++i)
	{
		sprintf(eff_section_name, "effect_%d", i);
		if (!config->sectionExists(eff_section_name))
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not found effect [%s], %s", GEN_MSG_LOCATION, eff_section_name, path);
			return;
		}

		ID eff_id = this->effectAdd(0);
		ArrID[eff_id]->Original = true;
		int eff_count_part = 0;

		if (config->keyExists(eff_section_name, "name"))
			EffectNameSet(eff_id, config->getKey(eff_section_name, "name"));

		if (config->keyExists(eff_section_name, "emitters_count"))
			eff_count_part = String(config->getKey(eff_section_name, "emitters_count")).toInt();

		for (int k = 0; k < eff_count_part; ++k)
		{
			sprintf(part_section_name, "effect_%d_emitter_%d", i, k);
			if (!config->sectionExists(eff_section_name))
			{
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not found particles [%s], %s", GEN_MSG_LOCATION, part_section_name, path);
				return;
			}

			ParticlesData part;


			if (config->keyExists(part_section_name, "BoundType"))
				part.BoundType = (PARTICLESTYPE_BOUND)String(config->getKey(part_section_name, "BoundType")).toInt();

			if (config->keyExists(part_section_name, "BoundVec1X"))
				part.BoundVec1.x = String(config->getKey(part_section_name, "BoundVec1X")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec1Y"))
				part.BoundVec1.y = String(config->getKey(part_section_name, "BoundVec1Y")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec1Z"))
				part.BoundVec1.z = String(config->getKey(part_section_name, "BoundVec1Z")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec1W"))
				part.BoundVec1.w = String(config->getKey(part_section_name, "BoundVec1W")).toDouble();

			if (config->keyExists(part_section_name, "BoundVec2X"))
				part.BoundVec2.x = String(config->getKey(part_section_name, "BoundVec2X")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec2Y"))
				part.BoundVec2.y = String(config->getKey(part_section_name, "BoundVec2Y")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec2Z"))
				part.BoundVec2.z = String(config->getKey(part_section_name, "BoundVec2Z")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec2W"))
				part.BoundVec2.w = String(config->getKey(part_section_name, "BoundVec2W")).toDouble();


			if (config->keyExists(part_section_name, "SpawnPosType"))
				part.SpawnPosType = (PARTICLESTYPE_SPAWNPOS)String(config->getKey(part_section_name, "SpawnPosType")).toInt();

			if (config->keyExists(part_section_name, "SpawnOriginX"))
				part.SpawnOrigin.x = String(config->getKey(part_section_name, "SpawnOriginX")).toDouble();
			if (config->keyExists(part_section_name, "SpawnOriginY"))
				part.SpawnOrigin.y = String(config->getKey(part_section_name, "SpawnOriginY")).toDouble();
			if (config->keyExists(part_section_name, "SpawnOriginZ"))
				part.SpawnOrigin.z = String(config->getKey(part_section_name, "SpawnOriginZ")).toDouble();

			if (config->keyExists(part_section_name, "SpawnOriginDisp"))
				part.SpawnOriginDisp = String(config->getKey(part_section_name, "SpawnOriginDisp")).toDouble();

			if (config->keyExists(part_section_name, "SpawnBoundBindCreateXNeg"))
				part.SpawnBoundBindCreateXNeg = String(config->getKey(part_section_name, "SpawnBoundBindCreateXNeg")).toBool();
			if (config->keyExists(part_section_name, "SpawnBoundBindCreateXPos"))
				part.SpawnBoundBindCreateXPos = String(config->getKey(part_section_name, "SpawnBoundBindCreateXPos")).toBool();
			if (config->keyExists(part_section_name, "SpawnBoundBindCreateYNeg"))
				part.SpawnBoundBindCreateYNeg = String(config->getKey(part_section_name, "SpawnBoundBindCreateYNeg")).toBool();
			if (config->keyExists(part_section_name, "SpawnBoundBindCreateYPos"))
				part.SpawnBoundBindCreateYPos = String(config->getKey(part_section_name, "SpawnBoundBindCreateYPos")).toBool();
			if (config->keyExists(part_section_name, "SpawnBoundBindCreateZNeg"))
				part.SpawnBoundBindCreateZNeg = String(config->getKey(part_section_name, "SpawnBoundBindCreateZNeg")).toBool();
			if (config->keyExists(part_section_name, "SpawnBoundBindCreateZPos"))
				part.SpawnBoundBindCreateZPos = String(config->getKey(part_section_name, "SpawnBoundBindCreateZPos")).toBool();

			if (config->keyExists(part_section_name, "SpawnNextTime"))
				part.SpawnNextTime = String(config->getKey(part_section_name, "SpawnNextTime")).toUnsLongInt();
			if (config->keyExists(part_section_name, "SpawnNextTimeDisp"))
				part.SpawnNextTimeDisp = String(config->getKey(part_section_name, "SpawnNextTimeDisp")).toUnsLongInt();


			if (config->keyExists(part_section_name, "AnimTexCountCadrsX"))
				part.AnimTexCountCadrsX = String(config->getKey(part_section_name, "AnimTexCountCadrsX")).toInt();
			if (config->keyExists(part_section_name, "AnimTexCountCadrsY"))
				part.AnimTexCountCadrsY = String(config->getKey(part_section_name, "AnimTexCountCadrsY")).toInt();
			if (config->keyExists(part_section_name, "AnimTexRate"))
				part.AnimTexRate = String(config->getKey(part_section_name, "AnimTexRate")).toInt();
			if (config->keyExists(part_section_name, "AnimTexRateDisp"))
				part.AnimTexRateDisp = String(config->getKey(part_section_name, "AnimTexRateDisp")).toInt();
			if (config->keyExists(part_section_name, "AnimTexStartCadr"))
				part.AnimTexStartCadr = String(config->getKey(part_section_name, "AnimTexStartCadr")).toInt();
			if (config->keyExists(part_section_name, "AnimTexStartCadrDisp"))
				part.AnimTexStartCadrDisp = String(config->getKey(part_section_name, "AnimTexStartCadrDisp")).toInt();


			if (config->keyExists(part_section_name, "TimeLife"))
				part.TimeLife = String(config->getKey(part_section_name, "TimeLife")).toUnsLongInt();
			if (config->keyExists(part_section_name, "TimeLifeDisp"))
				part.TimeLifeDisp = String(config->getKey(part_section_name, "TimeLifeDisp")).toUnsLongInt();

			if (config->keyExists(part_section_name, "AlphaDependAge"))
				part.AlphaDependAge = (PARTICLESTYPE_DEPEND)String(config->getKey(part_section_name, "AlphaDependAge")).toInt();

			if (config->keyExists(part_section_name, "SizeX"))
				part.Size.x = String(config->getKey(part_section_name, "SizeX")).toDouble();
			if (config->keyExists(part_section_name, "SizeY"))
				part.Size.y = String(config->getKey(part_section_name, "SizeY")).toDouble();

			if (config->keyExists(part_section_name, "SizeDisp"))
				part.SizeDisp = String(config->getKey(part_section_name, "SizeDisp")).toDouble();

			if (config->keyExists(part_section_name, "SizeDependAge"))
				part.SizeDependAge = (PARTICLESTYPE_DEPEND)String(config->getKey(part_section_name, "SizeDependAge")).toInt();


			if (config->keyExists(part_section_name, "VelocityX"))
				part.Velocity.x = String(config->getKey(part_section_name, "VelocityX")).toDouble();
			if (config->keyExists(part_section_name, "VelocityY"))
				part.Velocity.y = String(config->getKey(part_section_name, "VelocityY")).toDouble();
			if (config->keyExists(part_section_name, "VelocityZ"))
				part.Velocity.z = String(config->getKey(part_section_name, "VelocityZ")).toDouble();

			if (config->keyExists(part_section_name, "VelocityDisp"))
				part.VelocityDisp = String(config->getKey(part_section_name, "VelocityDisp")).toDouble();

			if (config->keyExists(part_section_name, "VelocityDispXNeg"))
				part.VelocityDispXNeg = String(config->getKey(part_section_name, "VelocityDispXNeg")).toDouble();
			if (config->keyExists(part_section_name, "VelocityDispYNeg"))
				part.VelocityDispYNeg = String(config->getKey(part_section_name, "VelocityDispYNeg")).toDouble();
			if (config->keyExists(part_section_name, "VelocityDispZNeg"))
				part.VelocityDispZNeg = String(config->getKey(part_section_name, "VelocityDispZNeg")).toDouble();


			if (config->keyExists(part_section_name, "AccelerationX"))
				part.Acceleration.x = String(config->getKey(part_section_name, "AccelerationX")).toDouble();
			if (config->keyExists(part_section_name, "AccelerationY"))
				part.Acceleration.y = String(config->getKey(part_section_name, "AccelerationY")).toDouble();
			if (config->keyExists(part_section_name, "AccelerationZ"))
				part.Acceleration.z = String(config->getKey(part_section_name, "AccelerationZ")).toDouble();

			if (config->keyExists(part_section_name, "AccelerationDisp"))
				part.AccelerationDisp = String(config->getKey(part_section_name, "AccelerationDisp")).toDouble();

			if (config->keyExists(part_section_name, "AccelerationDispXNeg"))
				part.AccelerationDispXNeg = String(config->getKey(part_section_name, "AccelerationDispXNeg")).toBool();
			if (config->keyExists(part_section_name, "AccelerationDispYNeg"))
				part.AccelerationDispYNeg = String(config->getKey(part_section_name, "AccelerationDispYNeg")).toBool();
			if (config->keyExists(part_section_name, "AccelerationDispZNeg"))
				part.AccelerationDispZNeg = String(config->getKey(part_section_name, "AccelerationDispZNeg")).toBool();


			if (config->keyExists(part_section_name, "CharacterCircle"))
				part.CharacterCircle = String(config->getKey(part_section_name, "CharacterCircle")).toBool();
			if (config->keyExists(part_section_name, "CharacterCircleAxis"))
				part.CharacterCircleAxis = (PARTICLES_AXIS)String(config->getKey(part_section_name, "CharacterCircleAxis")).toInt();
			if (config->keyExists(part_section_name, "CharacterCircleAngle"))
				part.CharacterCircleAngle = String(config->getKey(part_section_name, "CharacterCircleAngle")).toDouble();
			if (config->keyExists(part_section_name, "CharacterCircleAngleDisp"))
				part.CharacterCircleAngleDisp = String(config->getKey(part_section_name, "CharacterCircleAngleDisp")).toDouble();
			if (config->keyExists(part_section_name, "CharacterCircleAngleDispNeg"))
				part.CharacterCircleAngleDispNeg = String(config->getKey(part_section_name, "CharacterCircleAngleDispNeg")).toBool();


			if (config->keyExists(part_section_name, "CharacterRotate"))
				part.CharacterRotate = String(config->getKey(part_section_name, "CharacterRotate")).toBool();
			if (config->keyExists(part_section_name, "CharacterRotateAngle"))
				part.CharacterRotateAngle = String(config->getKey(part_section_name, "CharacterRotateAngle")).toDouble();
			if (config->keyExists(part_section_name, "CharacterRotateAngleDisp"))
				part.CharacterRotateAngleDisp = String(config->getKey(part_section_name, "CharacterRotateAngleDisp")).toDouble();
			if (config->keyExists(part_section_name, "CharacterRotateAngleDispNeg"))
				part.CharacterRotateAngleDispNeg = String(config->getKey(part_section_name, "CharacterRotateAngleDispNeg")).toBool();


			if (config->keyExists(part_section_name, "CharacterDeviation"))
				part.CharacterDeviation = String(config->getKey(part_section_name, "CharacterDeviation")).toBool();
			if (config->keyExists(part_section_name, "CharacterDeviationType"))
				part.CharacterDeviationType = (PARTICLESTYPE_DEVIATION)String(config->getKey(part_section_name, "CharacterDeviationType")).toInt();
			if (config->keyExists(part_section_name, "CharacterDeviationAmplitude"))
				part.CharacterDeviationAmplitude = String(config->getKey(part_section_name, "CharacterDeviationAmplitude")).toDouble();
			if (config->keyExists(part_section_name, "CharacterDeviationCoefAngle"))
				part.CharacterDeviationCoefAngle = String(config->getKey(part_section_name, "CharacterDeviationCoefAngle")).toDouble();
			if (config->keyExists(part_section_name, "CharacterDeviationAxis"))
				part.CharacterDeviationAxis = (PARTICLES_AXIS)String(config->getKey(part_section_name, "CharacterDeviationAxis")).toInt();
			if (config->keyExists(part_section_name, "CharacterDeviationCountDelayMls"))
				part.CharacterDeviationCountDelayMls = String(config->getKey(part_section_name, "CharacterDeviationCountDelayMls")).toUnsLongInt();
			if (config->keyExists(part_section_name, "CharacterDeviationCoefAngleDisp"))
				part.CharacterDeviationCoefAngleDisp = String(config->getKey(part_section_name, "CharacterDeviationCoefAngleDisp")).toDouble();
			if (config->keyExists(part_section_name, "CharacterDeviationCoefAngleDispNeg"))
				part.CharacterDeviationCoefAngleDispNeg = String(config->getKey(part_section_name, "CharacterDeviationCoefAngleDispNeg")).toBool();

			if (config->keyExists(part_section_name, "CharacterDeviationTapX"))
				part.CharacterDeviationTapX = String(config->getKey(part_section_name, "CharacterDeviationTapX")).toBool();
			if (config->keyExists(part_section_name, "CharacterDeviationTapY"))
				part.CharacterDeviationTapY = String(config->getKey(part_section_name, "CharacterDeviationTapY")).toBool();
			if (config->keyExists(part_section_name, "CharacterDeviationTapZ"))
				part.CharacterDeviationTapZ = String(config->getKey(part_section_name, "CharacterDeviationTapZ")).toBool();


			if (config->keyExists(part_section_name, "FigureType"))
				part.FigureType = (PARTICLESTYPE_FIGURE)String(config->getKey(part_section_name, "FigureType")).toInt();
			if (config->keyExists(part_section_name, "CharacterDeviationAxis"))
				part.FigureCountQuads = String(config->getKey(part_section_name, "FigureCountQuads")).toInt();

			if (config->keyExists(part_section_name, "FigureRotRand"))
				part.FigureRotRand = String(config->getKey(part_section_name, "FigureRotRand")).toBool();

			if (config->keyExists(part_section_name, "FigureTapX"))
				part.FigureTapX = String(config->getKey(part_section_name, "FigureTapX")).toBool();
			if (config->keyExists(part_section_name, "FigureTapY"))
				part.FigureTapY = String(config->getKey(part_section_name, "FigureTapY")).toBool();
			if (config->keyExists(part_section_name, "FigureTapZ"))
				part.FigureTapZ = String(config->getKey(part_section_name, "FigureTapZ")).toBool();

			if (config->keyExists(part_section_name, "ReCreateCount"))
				part.ReCreateCount = String(config->getKey(part_section_name, "ReCreateCount")).toInt();

			if (config->keyExists(part_section_name, "AlphaBlendType"))
				part.AlphaBlendType = (PARTICLESTYPE_ALPHABLEND)String(config->getKey(part_section_name, "AlphaBlendType")).toInt();

			if (config->keyExists(part_section_name, "ColorCoef"))
				part.ColorCoef = String(config->getKey(part_section_name, "ColorCoef")).toDouble();

			if (config->keyExists(part_section_name, "Soft"))
				part.Soft = String(config->getKey(part_section_name, "Soft")).toBool();
			if (config->keyExists(part_section_name, "SoftCoef"))
				part.SoftCoef = String(config->getKey(part_section_name, "SoftCoef")).toDouble();

			if (config->keyExists(part_section_name, "Refraction"))
				part.Refraction = String(config->getKey(part_section_name, "Refraction")).toBool();
			if (config->keyExists(part_section_name, "RefractionCoef"))
				part.RefractionCoef = String(config->getKey(part_section_name, "RefractionCoef")).toDouble();

			if (config->keyExists(part_section_name, "TransparencyCoef"))
				part.TransparencyCoef = String(config->getKey(part_section_name, "TransparencyCoef")).toDouble();
			if (config->keyExists(part_section_name, "Lighting"))
				part.Lighting = String(config->getKey(part_section_name, "Lighting")).toBool();
			if (config->keyExists(part_section_name, "CollisionDelete"))
				part.CollisionDelete = String(config->getKey(part_section_name, "CollisionDelete")).toBool();


			if (config->keyExists(part_section_name, "Track"))
				part.Track = String(config->getKey(part_section_name, "Track")).toBool();

			if (config->keyExists(part_section_name, "TrackSize"))
				part.TrackSize = String(config->getKey(part_section_name, "TrackSize")).toDouble();

			if (config->keyExists(part_section_name, "TrackTime"))
				part.TrackTime = String(config->getKey(part_section_name, "TrackTime")).toUnsLongInt();

			
			ID part_id = this->emitterAdd(eff_id, &part);

			if (config->keyExists(part_section_name, "name"))
				emitterSetName(eff_id, part_id, config->getKey(part_section_name, "name"));

			if (config->keyExists(part_section_name, "count"))
				emitterSetCount(eff_id, part_id, String(config->getKey(part_section_name, "count")).toInt());

			if (config->keyExists(part_section_name, "texture"))
				emitterSetTexture(eff_id, part_id, config->getKey(part_section_name, "texture"));

			if (config->keyExists(part_section_name, "texture_track"))
				emitterSetTextureTrack(eff_id, part_id, config->getKey(part_section_name, "texture_track"));
		}
	}

	mem_release_del(config);
}


int Effects::emitterGetTrackCount(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->getTrackCount();
}

int Effects::emitterGetTrackPos(ID id, ID id_part, float3** arr, int count)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->getTrackPos(arr, count);
}


void Effects::emitterSetName(ID id, ID id_part, const char* name)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->setName(name);
}

void Effects::emitterGetName(ID id, ID id_part, char* name)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->getName(name);
}

ID Effects::emitterAdd(ID id, ParticlesData* data)
{
	EFFECTS_EFFECT_PRECOND(id, -1);

	Emitter* tmppart = new Emitter();
	tmppart->init(data);
	ArrID[id]->Arr.push_back(tmppart);
	return ArrID[id]->Arr.size() - 1;
}

void Effects::emitterReInit(ID id, ID id_part, ParticlesData* data)
{
	EFFECTS_PARTICLES_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->init(data);
}

int Effects::emitterGetCount(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, -1);
	return ArrID[id]->Arr.size();
}

void Effects::emitterDelete(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, _VOID);
	mem_delete(ArrID[id]->Arr[id_part]);
	ArrID[id]->Arr.erase(id_part);
}

ParticlesData* Effects::emitterGetData(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->getData();
}

void Effects::emitterSetCount(ID id, ID id_part, int count)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->setCount(count);
}

int Effects::emitterGetCount(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->getCount();
}

int Effects::emitterGetCountLife(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->getCountLife();
}

void Effects::emitterSetEnable(ID id, ID id_part, bool enable)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->setEnable(enable);
}

bool Effects::emitterGetEnable(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, false);

	return ArrID[id]->Arr[id_part]->getEnable();
}


void Effects::emitterSetTexture(ID id, ID id_part, const char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->setTexture(tex);
}

void Effects::emitterSetTextureID(ID id, ID id_part, ID tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->setTextureID(tex);
}

ID Effects::emitterGetTextureID(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, -1);

	return ArrID[id]->Arr[id_part]->getTextureID();
}

void Effects::emitterGetTexture(ID id, ID id_part, char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->getTexture(tex);
}


void Effects::emitterSetTextureTrack(ID id, ID id_part, const char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->setTextureTrack(tex);
}

void Effects::emitterSetTextureTrackID(ID id, ID id_part, ID tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->setTextureTrackID(tex);
}

ID Effects::emitterGetTextureTrackID(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, -1);

	return ArrID[id]->Arr[id_part]->getTextureTrackID();
}

void Effects::emitterGetTextureTrack(ID id, ID id_part, char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->getTextureTrack(tex);
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
	return EffectCopyID(effectGetByName(name));
}

ID Effects::effectGetByName(const char* name)
{
	for (int i = 0; i < ArrKey.size(); ++i)
	{
		if (strcmp(name, ArrKey[i]->Name) == 0)
			return ArrKey[i]->Id;
	}

	return -1;
}

ID Effects::effectAdd(const char* name)
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
	return EffectInstanceByID(effectGetByName(name));
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
	EffectPlayByID(effectGetByName(name), pos, dir);
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

int Effects::effectGetCount()
{
	return ArrKey.size();
}

ID Effects::effectGetIdOfKey(ID key)
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
			eff->Arr[i]->compute(&mattrans);

		//если партиклы метрвы то инкрементируем счетчик
		if (!eff->Arr[i]->getEnable())
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
		if (eff->Arr[i] && eff->Arr[i]->getEnable())
		{
			eff->Arr[i]->computeLighting();
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
		if(eff->Arr[i] && eff->Arr[i]->getEnable())
		{
			eff->Arr[i]->render(timeDelta, &eff->MatRotate, &eff->MatTranslation);
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

bool Effects::EffectVisibleCom(ID id, const IFrustum* frustum, float3* view)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	Effect* eff = ArrID[id];
	if (!eff || !eff->Enable)
		return false;

	float3 scenter;
	float sradius;

	scenter = (eff->CurrMin2 + eff->CurrMax2) * 0.5f;
	sradius = SMVector3Length(scenter - eff->CurrMax2);
	
	eff->ViewRender = frustum->sphereInFrustum(&scenter, sradius);

	if (eff->ViewRender)
		eff->ViewRender = SGCore_OC_IsVisible(&(eff->CurrMin2), &(eff->CurrMax2));

	eff->ViewDist = SMVector3Length((scenter - (*view))) - sradius;

	return eff->ViewRender;
}

void Effects::EffectVisibleComAll(const IFrustum* frustum, float3* view)
{
	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

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
			//LibReport(0, "ArrSort[%d] = %d\n", i, ArrSort[i]);
			if (ArrSort[i] >= 0)
				LibReport(0, "ViewDist = %f\n", ArrID[ArrSort[i]]->ViewDist);
		}

		LibReport(0, "%d---------\n", ArrSortSizeCurr);
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
		eff->Arr[i]->setEnable(isenable);
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
		eff->Arr[i]->setAlife(alife);
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

	ArrID[id]->Direction = SMEulerToVec(ArrID[id]->Rotation, SXPARTICLES_BASE_DIR);

	static float3 f = SXPARTICLES_BASE_DIR;
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