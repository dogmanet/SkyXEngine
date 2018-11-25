
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "effect.h"

CEffects::CEffects()
{
	ArrSortSizeCurr = 0;
}

CEffects::~CEffects()
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

CEffects::CEffect::CEffect()
{
	nullingInit();
}

CEffects::CEffect::CEffect(CEffect &oEffect)
{
	nullingInit();
	m_vPosition = oEffect.m_vPosition;
	m_vDirection = oEffect.m_vDirection;
	m_vRotation = oEffect.m_vRotation;
	
	m_isEnable = oEffect.m_isEnable;

	for (int i = 0; i < oEffect.m_aEmitters.size(); ++i)
	{
		CEmitter* part = new CEmitter(*oEffect.m_aEmitters[i]);
		m_aEmitters.push_back(part);
	}
	m_idPool = oEffect.m_idPool;
}

CEffects::CEffect::~CEffect()
{
	for (int i = 0; i < m_aEmitters.size(); ++i)
	{
		mem_delete(m_aEmitters[i]);
	}

	m_aEmitters.clear();
}

CEffects::CPool::CPool()
{
	ideff = -1;
}

void CEffects::CEffect::nullingInit()
{
	m_id = m_idKey = -1;
	m_fViewDist = 0.0f;
	m_isViewRender = false;

	m_isEnable = false;
	m_isBusy = false;
	m_idPool = -1;
	m_isOriginal = true;
}

void CEffects::onLostDevice()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i])
		{
			for (int k = 0; k < ArrID[i]->m_aEmitters.size(); ++k)
			{
				if (ArrID[i]->m_aEmitters[k])
					ArrID[i]->m_aEmitters[k]->onLostDevice();
			}
		}
	}
}

void CEffects::onResetDevice()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i])
		{
			for (int k = 0; k < ArrID[i]->m_aEmitters.size(); ++k)
			{
				if (ArrID[i]->m_aEmitters[k])
					ArrID[i]->m_aEmitters[k]->onResetDevice();
			}
		}
	}
}

void CEffects::clear()
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

void CEffects::save(const char* path)
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
		int part_count = ArrKey[i]->m_aEmitters.size();
		
		fprintf(file, "[effect_%d]\n", i);
		fprintf(file, "name = %s\n", ArrKey[i]->m_szName);
		fprintf(file, "emitters_count = %d\n\n", part_count);

		for (int k = 0; k < part_count; ++k)
		{
			CEmitter* part = ArrKey[i]->m_aEmitters[k];
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

			fprintf(file, "Track = %d\n", part->getData()->m_useTrack);
			fprintf(file, "TrackSize = %f\n", part->getData()->m_fTrackSize);
			fprintf(file, "TrackTime = %d\n", part->getData()->m_uiTrackTime);

			fprintf(file, "m_typeBound = %d\n", part->getData()->m_typeBound);

			fprintf(file, "BoundVec1X = %f\n", part->getData()->m_vBoundVec1.x);
			fprintf(file, "BoundVec1Y = %f\n", part->getData()->m_vBoundVec1.y);
			fprintf(file, "BoundVec1Z = %f\n", part->getData()->m_vBoundVec1.z);
			fprintf(file, "BoundVec1W = %f\n", part->getData()->m_vBoundVec1.w);

			fprintf(file, "BoundVec2X = %f\n", part->getData()->m_vBoundVec2.x);
			fprintf(file, "BoundVec2Y = %f\n", part->getData()->m_vBoundVec2.y);
			fprintf(file, "BoundVec2Z = %f\n", part->getData()->m_vBoundVec2.z);
			fprintf(file, "BoundVec2W = %f\n", part->getData()->m_vBoundVec2.w);


			fprintf(file, "SpawnPosType = %d\n", part->getData()->m_typeSpawnPos);
			fprintf(file, "SpawnOriginX = %f\n", part->getData()->m_vSpawnOrigin.x);
			fprintf(file, "SpawnOriginY = %f\n", part->getData()->m_vSpawnOrigin.y);
			fprintf(file, "SpawnOriginZ = %f\n", part->getData()->m_vSpawnOrigin.z);

			fprintf(file, "SpawnOriginDisp = %f\n", part->getData()->m_fSpawnOriginDisp);

			fprintf(file, "SpawnBoundBindCreateXNeg = %d\n", part->getData()->m_shouldSpawnBoundBindCreateXNeg);
			fprintf(file, "SpawnBoundBindCreateXPos = %d\n", part->getData()->m_shouldSpawnBoundBindCreateXPos);
			fprintf(file, "SpawnBoundBindCreateYNeg = %d\n", part->getData()->m_shouldSpawnBoundBindCreateYNeg);
			fprintf(file, "SpawnBoundBindCreateYPos = %d\n", part->getData()->m_shouldSpawnBoundBindCreateYPos);
			fprintf(file, "SpawnBoundBindCreateZNeg = %d\n", part->getData()->m_shouldSpawnBoundBindCreateZNeg);
			fprintf(file, "SpawnBoundBindCreateZPos = %d\n", part->getData()->m_shouldSpawnBoundBindCreateZPos);

			fprintf(file, "SpawnNextTime = %d\n", part->getData()->m_uiSpawnNextTime);
			fprintf(file, "SpawnNextTimeDisp = %d\n", part->getData()->m_uiSpawnNextTimeDisp);


			fprintf(file, "AnimTexCountCadrsX = %d\n", part->getData()->m_iAnimTexCountCadrsX);
			fprintf(file, "AnimTexCountCadrsY = %d\n", part->getData()->m_iAnimTexCountCadrsY);

			fprintf(file, "AnimTexRate = %d\n", part->getData()->m_iAnimTexRate);
			fprintf(file, "AnimTexRateDisp = %d\n", part->getData()->m_iAnimTexRateDisp);
			fprintf(file, "AnimTexStartCadr = %d\n", part->getData()->m_iAnimTexStartCadr);
			fprintf(file, "AnimTexStartCadrDisp = %d\n", part->getData()->m_iAnimTexStartCadrDisp);


			fprintf(file, "TimeLife = %d\n", part->getData()->m_uiTimeLife);
			fprintf(file, "TimeLifeDisp = %d\n", part->getData()->m_uiTimeLifeDisp);
			fprintf(file, "AlphaDependAge = %d\n", part->getData()->m_typeAlphaDependAge);

			fprintf(file, "SizeX = %f\n", part->getData()->m_vSize.x);
			fprintf(file, "SizeY = %f\n", part->getData()->m_vSize.y);
			fprintf(file, "SizeDisp = %f\n", part->getData()->m_fSizeDisp);

			fprintf(file, "SizeDependAge = %d\n", part->getData()->m_typeSizeDependAge);


			fprintf(file, "VelocityX = %f\n", part->getData()->m_vVelocity.x);
			fprintf(file, "VelocityY = %f\n", part->getData()->m_vVelocity.y);
			fprintf(file, "VelocityZ = %f\n", part->getData()->m_vVelocity.z);
			fprintf(file, "VelocityDisp = %f\n", part->getData()->m_fVelocityDisp);

			fprintf(file, "VelocityDispXNeg = %d\n", part->getData()->m_shouldVelocityDispXNeg);
			fprintf(file, "VelocityDispYNeg = %d\n", part->getData()->m_shouldVelocityDispYNeg);
			fprintf(file, "VelocityDispZNeg = %d\n", part->getData()->m_shouldVelocityDispZNeg);


			fprintf(file, "AccelerationX = %f\n", part->getData()->m_vAcceleration.x);
			fprintf(file, "AccelerationY = %f\n", part->getData()->m_vAcceleration.y);
			fprintf(file, "AccelerationZ = %f\n", part->getData()->m_vAcceleration.z);
			fprintf(file, "AccelerationDisp = %f\n", part->getData()->m_fAccelerationDisp);

			fprintf(file, "AccelerationDispXNeg = %d\n", part->getData()->m_shouldAccelerationDispXNeg);
			fprintf(file, "AccelerationDispYNeg = %d\n", part->getData()->m_shouldAccelerationDispYNeg);
			fprintf(file, "AccelerationDispZNeg = %d\n", part->getData()->m_shouldAccelerationDispZNeg);


			fprintf(file, "CharacterCircle = %d\n", part->getData()->m_useCharacterCircle);
			fprintf(file, "CharacterCircleAxis = %d\n", part->getData()->m_typeCharacterCircleAxis);
			fprintf(file, "CharacterCircleAngle = %f\n", part->getData()->m_fCharacterCircleAngle);
			fprintf(file, "CharacterCircleAngleDisp = %f\n", part->getData()->m_fCharacterCircleAngleDisp);
			fprintf(file, "CharacterCircleAngleDispNeg = %d\n", part->getData()->m_useCharacterCircleAngleDispNeg);


			fprintf(file, "CharacterRotate = %d\n", part->getData()->m_useCharacterRotate);
			fprintf(file, "CharacterRotateAngle = %f\n", part->getData()->m_fCharacterRotateAngle);
			fprintf(file, "CharacterRotateAngleDisp = %f\n", part->getData()->m_fCharacterRotateAngleDisp);
			fprintf(file, "CharacterRotateAngleDispNeg = %d\n", part->getData()->m_useCharacterRotateAngleDispNeg);


			fprintf(file, "CharacterDeviation = %d\n", part->getData()->m_useCharacterDeviation);
			fprintf(file, "CharacterDeviationType = %d\n", part->getData()->m_typeCharacterDeviation);
			fprintf(file, "CharacterDeviationAmplitude = %f\n", part->getData()->m_fCharacterDeviationAmplitude);
			fprintf(file, "CharacterDeviationCoefAngle = %f\n", part->getData()->m_fCharacterDeviationCoefAngle);
			fprintf(file, "CharacterDeviationAxis = %d\n", part->getData()->m_typeCharacterDeviationAxis);
			fprintf(file, "CharacterDeviationCountDelayMls = %d\n", part->getData()->m_uiCharacterDeviationCountDelayMls);
			fprintf(file, "CharacterDeviationCoefAngleDisp = %f\n", part->getData()->m_fCharacterDeviationCoefAngleDisp);
			fprintf(file, "CharacterDeviationCoefAngleDispNeg = %d\n", part->getData()->m_useCharacterDeviationCoefAngleDispNeg);

			fprintf(file, "CharacterDeviationTapX = %d\n", part->getData()->m_useCharacterDeviationTapX);
			fprintf(file, "CharacterDeviationTapY = %d\n", part->getData()->m_useCharacterDeviationTapY);
			fprintf(file, "CharacterDeviationTapZ = %d\n", part->getData()->m_useCharacterDeviationTapZ);


			fprintf(file, "FigureType = %d\n", part->getData()->m_typeFigure);
			fprintf(file, "FigureCountQuads = %d\n", part->getData()->m_iFigureCountQuads);
			fprintf(file, "FigureRotRand = %d\n", part->getData()->m_useFigureRotRand);

			fprintf(file, "FigureTapX = %d\n", part->getData()->m_useFigureTapX);
			fprintf(file, "FigureTapY = %d\n", part->getData()->m_useFigureTapY);
			fprintf(file, "FigureTapZ = %d\n", part->getData()->m_useFigureTapZ);

			fprintf(file, "ReCreateCount = %d\n", part->getData()->m_iReCreateCount);

			fprintf(file, "AlphaBlendType = %d\n", part->getData()->m_typeAlphaBlend);

			fprintf(file, "ColorCoef = %f\n", part->getData()->m_fColorCoef);

			fprintf(file, "Soft = %d\n", part->getData()->m_isSoft);
			fprintf(file, "SoftCoef = %f\n", part->getData()->m_fSoftCoef);

			fprintf(file, "Refraction = %d\n", part->getData()->m_useRefraction);
			fprintf(file, "RefractionCoef = %f\n", part->getData()->m_fRefractionCoef);

			fprintf(file, "TransparencyCoef = %f\n", part->getData()->m_fTransparencyCoef);
			fprintf(file, "Lighting = %d\n", part->getData()->m_isLighting);
			fprintf(file, "CollisionDelete = %d\n", part->getData()->m_useCollisionDelete);
			fprintf(file, "\n");
		}
		fprintf(file, "----------------------------------------------------------------------\n\n");
	}

	fclose(file);
}

void CEffects::load(const char* path)
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
		ArrID[eff_id]->m_isOriginal = true;
		int eff_count_part = 0;

		if (config->keyExists(eff_section_name, "name"))
			effectSetName(eff_id, config->getKey(eff_section_name, "name"));

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

			CParticlesData part;


			if (config->keyExists(part_section_name, "m_typeBound"))
				part.m_typeBound = (PARTICLESTYPE_BOUND)String(config->getKey(part_section_name, "m_typeBound")).toInt();

			if (config->keyExists(part_section_name, "BoundVec1X"))
				part.m_vBoundVec1.x = String(config->getKey(part_section_name, "BoundVec1X")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec1Y"))
				part.m_vBoundVec1.y = String(config->getKey(part_section_name, "BoundVec1Y")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec1Z"))
				part.m_vBoundVec1.z = String(config->getKey(part_section_name, "BoundVec1Z")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec1W"))
				part.m_vBoundVec1.w = String(config->getKey(part_section_name, "BoundVec1W")).toDouble();

			if (config->keyExists(part_section_name, "BoundVec2X"))
				part.m_vBoundVec2.x = String(config->getKey(part_section_name, "BoundVec2X")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec2Y"))
				part.m_vBoundVec2.y = String(config->getKey(part_section_name, "BoundVec2Y")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec2Z"))
				part.m_vBoundVec2.z = String(config->getKey(part_section_name, "BoundVec2Z")).toDouble();
			if (config->keyExists(part_section_name, "BoundVec2W"))
				part.m_vBoundVec2.w = String(config->getKey(part_section_name, "BoundVec2W")).toDouble();


			if (config->keyExists(part_section_name, "SpawnPosType"))
				part.m_typeSpawnPos = (PARTICLESTYPE_SPAWNPOS)String(config->getKey(part_section_name, "SpawnPosType")).toInt();

			if (config->keyExists(part_section_name, "SpawnOriginX"))
				part.m_vSpawnOrigin.x = String(config->getKey(part_section_name, "SpawnOriginX")).toDouble();
			if (config->keyExists(part_section_name, "SpawnOriginY"))
				part.m_vSpawnOrigin.y = String(config->getKey(part_section_name, "SpawnOriginY")).toDouble();
			if (config->keyExists(part_section_name, "SpawnOriginZ"))
				part.m_vSpawnOrigin.z = String(config->getKey(part_section_name, "SpawnOriginZ")).toDouble();

			if (config->keyExists(part_section_name, "SpawnOriginDisp"))
				part.m_fSpawnOriginDisp = String(config->getKey(part_section_name, "SpawnOriginDisp")).toDouble();

			if (config->keyExists(part_section_name, "SpawnBoundBindCreateXNeg"))
				part.m_shouldSpawnBoundBindCreateXNeg = String(config->getKey(part_section_name, "SpawnBoundBindCreateXNeg")).toBool();
			if (config->keyExists(part_section_name, "SpawnBoundBindCreateXPos"))
				part.m_shouldSpawnBoundBindCreateXPos = String(config->getKey(part_section_name, "SpawnBoundBindCreateXPos")).toBool();
			if (config->keyExists(part_section_name, "SpawnBoundBindCreateYNeg"))
				part.m_shouldSpawnBoundBindCreateYNeg = String(config->getKey(part_section_name, "SpawnBoundBindCreateYNeg")).toBool();
			if (config->keyExists(part_section_name, "SpawnBoundBindCreateYPos"))
				part.m_shouldSpawnBoundBindCreateYPos = String(config->getKey(part_section_name, "SpawnBoundBindCreateYPos")).toBool();
			if (config->keyExists(part_section_name, "SpawnBoundBindCreateZNeg"))
				part.m_shouldSpawnBoundBindCreateZNeg = String(config->getKey(part_section_name, "SpawnBoundBindCreateZNeg")).toBool();
			if (config->keyExists(part_section_name, "SpawnBoundBindCreateZPos"))
				part.m_shouldSpawnBoundBindCreateZPos = String(config->getKey(part_section_name, "SpawnBoundBindCreateZPos")).toBool();

			if (config->keyExists(part_section_name, "SpawnNextTime"))
				part.m_uiSpawnNextTime = String(config->getKey(part_section_name, "SpawnNextTime")).toUnsLongInt();
			if (config->keyExists(part_section_name, "SpawnNextTimeDisp"))
				part.m_uiSpawnNextTimeDisp = String(config->getKey(part_section_name, "SpawnNextTimeDisp")).toUnsLongInt();


			if (config->keyExists(part_section_name, "AnimTexCountCadrsX"))
				part.m_iAnimTexCountCadrsX = String(config->getKey(part_section_name, "AnimTexCountCadrsX")).toInt();
			if (config->keyExists(part_section_name, "AnimTexCountCadrsY"))
				part.m_iAnimTexCountCadrsY = String(config->getKey(part_section_name, "AnimTexCountCadrsY")).toInt();
			if (config->keyExists(part_section_name, "AnimTexRate"))
				part.m_iAnimTexRate = String(config->getKey(part_section_name, "AnimTexRate")).toInt();
			if (config->keyExists(part_section_name, "AnimTexRateDisp"))
				part.m_iAnimTexRateDisp = String(config->getKey(part_section_name, "AnimTexRateDisp")).toInt();
			if (config->keyExists(part_section_name, "AnimTexStartCadr"))
				part.m_iAnimTexStartCadr = String(config->getKey(part_section_name, "AnimTexStartCadr")).toInt();
			if (config->keyExists(part_section_name, "AnimTexStartCadrDisp"))
				part.m_iAnimTexStartCadrDisp = String(config->getKey(part_section_name, "AnimTexStartCadrDisp")).toInt();


			if (config->keyExists(part_section_name, "TimeLife"))
				part.m_uiTimeLife = String(config->getKey(part_section_name, "TimeLife")).toUnsLongInt();
			if (config->keyExists(part_section_name, "TimeLifeDisp"))
				part.m_uiTimeLifeDisp = String(config->getKey(part_section_name, "TimeLifeDisp")).toUnsLongInt();

			if (config->keyExists(part_section_name, "AlphaDependAge"))
				part.m_typeAlphaDependAge = (PARTICLESTYPE_DEPEND)String(config->getKey(part_section_name, "AlphaDependAge")).toInt();

			if (config->keyExists(part_section_name, "SizeX"))
				part.m_vSize.x = String(config->getKey(part_section_name, "SizeX")).toDouble();
			if (config->keyExists(part_section_name, "SizeY"))
				part.m_vSize.y = String(config->getKey(part_section_name, "SizeY")).toDouble();

			if (config->keyExists(part_section_name, "SizeDisp"))
				part.m_fSizeDisp = String(config->getKey(part_section_name, "SizeDisp")).toDouble();

			if (config->keyExists(part_section_name, "SizeDependAge"))
				part.m_typeSizeDependAge = (PARTICLESTYPE_DEPEND)String(config->getKey(part_section_name, "SizeDependAge")).toInt();


			if (config->keyExists(part_section_name, "VelocityX"))
				part.m_vVelocity.x = String(config->getKey(part_section_name, "VelocityX")).toDouble();
			if (config->keyExists(part_section_name, "VelocityY"))
				part.m_vVelocity.y = String(config->getKey(part_section_name, "VelocityY")).toDouble();
			if (config->keyExists(part_section_name, "VelocityZ"))
				part.m_vVelocity.z = String(config->getKey(part_section_name, "VelocityZ")).toDouble();

			if (config->keyExists(part_section_name, "VelocityDisp"))
				part.m_fVelocityDisp = String(config->getKey(part_section_name, "VelocityDisp")).toDouble();

			if (config->keyExists(part_section_name, "VelocityDispXNeg"))
				part.m_shouldVelocityDispXNeg = String(config->getKey(part_section_name, "VelocityDispXNeg")).toDouble();
			if (config->keyExists(part_section_name, "VelocityDispYNeg"))
				part.m_shouldVelocityDispYNeg = String(config->getKey(part_section_name, "VelocityDispYNeg")).toDouble();
			if (config->keyExists(part_section_name, "VelocityDispZNeg"))
				part.m_shouldVelocityDispZNeg = String(config->getKey(part_section_name, "VelocityDispZNeg")).toDouble();


			if (config->keyExists(part_section_name, "AccelerationX"))
				part.m_vAcceleration.x = String(config->getKey(part_section_name, "AccelerationX")).toDouble();
			if (config->keyExists(part_section_name, "AccelerationY"))
				part.m_vAcceleration.y = String(config->getKey(part_section_name, "AccelerationY")).toDouble();
			if (config->keyExists(part_section_name, "AccelerationZ"))
				part.m_vAcceleration.z = String(config->getKey(part_section_name, "AccelerationZ")).toDouble();

			if (config->keyExists(part_section_name, "AccelerationDisp"))
				part.m_fAccelerationDisp = String(config->getKey(part_section_name, "AccelerationDisp")).toDouble();

			if (config->keyExists(part_section_name, "AccelerationDispXNeg"))
				part.m_shouldAccelerationDispXNeg = String(config->getKey(part_section_name, "AccelerationDispXNeg")).toBool();
			if (config->keyExists(part_section_name, "AccelerationDispYNeg"))
				part.m_shouldAccelerationDispYNeg = String(config->getKey(part_section_name, "AccelerationDispYNeg")).toBool();
			if (config->keyExists(part_section_name, "AccelerationDispZNeg"))
				part.m_shouldAccelerationDispZNeg = String(config->getKey(part_section_name, "AccelerationDispZNeg")).toBool();


			if (config->keyExists(part_section_name, "CharacterCircle"))
				part.m_useCharacterCircle = String(config->getKey(part_section_name, "CharacterCircle")).toBool();
			if (config->keyExists(part_section_name, "CharacterCircleAxis"))
				part.m_typeCharacterCircleAxis = (PARTICLES_AXIS)String(config->getKey(part_section_name, "CharacterCircleAxis")).toInt();
			if (config->keyExists(part_section_name, "CharacterCircleAngle"))
				part.m_fCharacterCircleAngle = String(config->getKey(part_section_name, "CharacterCircleAngle")).toDouble();
			if (config->keyExists(part_section_name, "CharacterCircleAngleDisp"))
				part.m_fCharacterCircleAngleDisp = String(config->getKey(part_section_name, "CharacterCircleAngleDisp")).toDouble();
			if (config->keyExists(part_section_name, "CharacterCircleAngleDispNeg"))
				part.m_useCharacterCircleAngleDispNeg = String(config->getKey(part_section_name, "CharacterCircleAngleDispNeg")).toBool();


			if (config->keyExists(part_section_name, "CharacterRotate"))
				part.m_useCharacterRotate = String(config->getKey(part_section_name, "CharacterRotate")).toBool();
			if (config->keyExists(part_section_name, "CharacterRotateAngle"))
				part.m_fCharacterRotateAngle = String(config->getKey(part_section_name, "CharacterRotateAngle")).toDouble();
			if (config->keyExists(part_section_name, "CharacterRotateAngleDisp"))
				part.m_fCharacterRotateAngleDisp = String(config->getKey(part_section_name, "CharacterRotateAngleDisp")).toDouble();
			if (config->keyExists(part_section_name, "CharacterRotateAngleDispNeg"))
				part.m_useCharacterRotateAngleDispNeg = String(config->getKey(part_section_name, "CharacterRotateAngleDispNeg")).toBool();


			if (config->keyExists(part_section_name, "CharacterDeviation"))
				part.m_useCharacterDeviation = String(config->getKey(part_section_name, "CharacterDeviation")).toBool();
			if (config->keyExists(part_section_name, "CharacterDeviationType"))
				part.m_typeCharacterDeviation = (PARTICLESTYPE_DEVIATION)String(config->getKey(part_section_name, "CharacterDeviationType")).toInt();
			if (config->keyExists(part_section_name, "CharacterDeviationAmplitude"))
				part.m_fCharacterDeviationAmplitude = String(config->getKey(part_section_name, "CharacterDeviationAmplitude")).toDouble();
			if (config->keyExists(part_section_name, "CharacterDeviationCoefAngle"))
				part.m_fCharacterDeviationCoefAngle = String(config->getKey(part_section_name, "CharacterDeviationCoefAngle")).toDouble();
			if (config->keyExists(part_section_name, "CharacterDeviationAxis"))
				part.m_typeCharacterDeviationAxis = (PARTICLES_AXIS)String(config->getKey(part_section_name, "CharacterDeviationAxis")).toInt();
			if (config->keyExists(part_section_name, "CharacterDeviationCountDelayMls"))
				part.m_uiCharacterDeviationCountDelayMls = String(config->getKey(part_section_name, "CharacterDeviationCountDelayMls")).toUnsLongInt();
			if (config->keyExists(part_section_name, "CharacterDeviationCoefAngleDisp"))
				part.m_fCharacterDeviationCoefAngleDisp = String(config->getKey(part_section_name, "CharacterDeviationCoefAngleDisp")).toDouble();
			if (config->keyExists(part_section_name, "CharacterDeviationCoefAngleDispNeg"))
				part.m_useCharacterDeviationCoefAngleDispNeg = String(config->getKey(part_section_name, "CharacterDeviationCoefAngleDispNeg")).toBool();

			if (config->keyExists(part_section_name, "CharacterDeviationTapX"))
				part.m_useCharacterDeviationTapX = String(config->getKey(part_section_name, "CharacterDeviationTapX")).toBool();
			if (config->keyExists(part_section_name, "CharacterDeviationTapY"))
				part.m_useCharacterDeviationTapY = String(config->getKey(part_section_name, "CharacterDeviationTapY")).toBool();
			if (config->keyExists(part_section_name, "CharacterDeviationTapZ"))
				part.m_useCharacterDeviationTapZ = String(config->getKey(part_section_name, "CharacterDeviationTapZ")).toBool();


			if (config->keyExists(part_section_name, "FigureType"))
				part.m_typeFigure = (PARTICLESTYPE_FIGURE)String(config->getKey(part_section_name, "FigureType")).toInt();
			if (config->keyExists(part_section_name, "CharacterDeviationAxis"))
				part.m_iFigureCountQuads = String(config->getKey(part_section_name, "FigureCountQuads")).toInt();

			if (config->keyExists(part_section_name, "FigureRotRand"))
				part.m_useFigureRotRand = String(config->getKey(part_section_name, "FigureRotRand")).toBool();

			if (config->keyExists(part_section_name, "FigureTapX"))
				part.m_useFigureTapX = String(config->getKey(part_section_name, "FigureTapX")).toBool();
			if (config->keyExists(part_section_name, "FigureTapY"))
				part.m_useFigureTapY = String(config->getKey(part_section_name, "FigureTapY")).toBool();
			if (config->keyExists(part_section_name, "FigureTapZ"))
				part.m_useFigureTapZ = String(config->getKey(part_section_name, "FigureTapZ")).toBool();

			if (config->keyExists(part_section_name, "ReCreateCount"))
				part.m_iReCreateCount = String(config->getKey(part_section_name, "ReCreateCount")).toInt();

			if (config->keyExists(part_section_name, "AlphaBlendType"))
				part.m_typeAlphaBlend = (PARTICLESTYPE_ALPHABLEND)String(config->getKey(part_section_name, "AlphaBlendType")).toInt();

			if (config->keyExists(part_section_name, "ColorCoef"))
				part.m_fColorCoef = String(config->getKey(part_section_name, "ColorCoef")).toDouble();

			if (config->keyExists(part_section_name, "Soft"))
				part.m_isSoft = String(config->getKey(part_section_name, "Soft")).toBool();
			if (config->keyExists(part_section_name, "SoftCoef"))
				part.m_fSoftCoef = String(config->getKey(part_section_name, "SoftCoef")).toDouble();

			if (config->keyExists(part_section_name, "Refraction"))
				part.m_useRefraction = String(config->getKey(part_section_name, "Refraction")).toBool();
			if (config->keyExists(part_section_name, "RefractionCoef"))
				part.m_fRefractionCoef = String(config->getKey(part_section_name, "RefractionCoef")).toDouble();

			if (config->keyExists(part_section_name, "TransparencyCoef"))
				part.m_fTransparencyCoef = String(config->getKey(part_section_name, "TransparencyCoef")).toDouble();
			if (config->keyExists(part_section_name, "Lighting"))
				part.m_isLighting = String(config->getKey(part_section_name, "Lighting")).toBool();
			if (config->keyExists(part_section_name, "CollisionDelete"))
				part.m_useCollisionDelete = String(config->getKey(part_section_name, "CollisionDelete")).toBool();


			if (config->keyExists(part_section_name, "Track"))
				part.m_useTrack = String(config->getKey(part_section_name, "Track")).toBool();

			if (config->keyExists(part_section_name, "TrackSize"))
				part.m_fTrackSize = String(config->getKey(part_section_name, "TrackSize")).toDouble();

			if (config->keyExists(part_section_name, "TrackTime"))
				part.m_uiTrackTime = String(config->getKey(part_section_name, "TrackTime")).toUnsLongInt();

			
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


int CEffects::emitterGetTrackCount(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->m_aEmitters[id_part]->getTrackCount();
}

int CEffects::emitterGetTrackPos(ID id, ID id_part, float3** arr, int count)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->m_aEmitters[id_part]->getTrackPos(arr, count);
}


void CEffects::emitterSetName(ID id, ID id_part, const char* name)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->setName(name);
}

void CEffects::emitterGetName(ID id, ID id_part, char* name)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->getName(name);
}

ID CEffects::emitterAdd(ID id, CParticlesData* data)
{
	EFFECTS_EFFECT_PRECOND(id, -1);

	CEmitter* tmppart = new CEmitter();
	tmppart->init(data);
	ArrID[id]->m_aEmitters.push_back(tmppart);
	return ArrID[id]->m_aEmitters.size() - 1;
}

void CEffects::emitterReInit(ID id, ID id_part, CParticlesData* data)
{
	EFFECTS_PARTICLES_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->init(data);
}

int CEffects::emitterGetCount(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, -1);
	return ArrID[id]->m_aEmitters.size();
}

void CEffects::emitterDelete(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, _VOID);
	mem_delete(ArrID[id]->m_aEmitters[id_part]);
	ArrID[id]->m_aEmitters.erase(id_part);
}

CParticlesData* CEffects::emitterGetData(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->m_aEmitters[id_part]->getData();
}

void CEffects::emitterSetCount(ID id, ID id_part, int count)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->setCount(count);
}

int CEffects::emitterGetCount(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->m_aEmitters[id_part]->getCount();
}

int CEffects::emitterGetCountLife(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->m_aEmitters[id_part]->getCountLife();
}

void CEffects::emitterSetEnable(ID id, ID id_part, bool enable)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->setEnable(enable);
}

bool CEffects::emitterGetEnable(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, false);

	return ArrID[id]->m_aEmitters[id_part]->getEnable();
}


void CEffects::emitterSetTexture(ID id, ID id_part, const char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->setTexture(tex);
}

void CEffects::emitterSetTextureID(ID id, ID id_part, ID tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->setTextureID(tex);
}

ID CEffects::emitterGetTextureID(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, -1);

	return ArrID[id]->m_aEmitters[id_part]->getTextureID();
}

void CEffects::emitterGetTexture(ID id, ID id_part, char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->getTexture(tex);
}


void CEffects::emitterSetTextureTrack(ID id, ID id_part, const char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->setTextureTrack(tex);
}

void CEffects::emitterSetTextureTrackID(ID id, ID id_part, ID tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->setTextureTrackID(tex);
}

ID CEffects::emitterGetTextureTrackID(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, -1);

	return ArrID[id]->m_aEmitters[id_part]->getTextureTrackID();
}

void CEffects::emitterGetTextureTrack(ID id, ID id_part, char* tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->m_aEmitters[id_part]->getTextureTrack(tex);
}




ID CEffects::effectCopyID(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, -1);

	CEffect* efforigin = ArrID[id];
	CEffect* tmpeffect = new CEffect(*efforigin);

	return addEffect(tmpeffect);
}

ID CEffects::effectCopyName(const char* name)
{
	return effectCopyID(effectGetByName(name));
}

ID CEffects::effectGetByName(const char* name)
{
	for (int i = 0; i < ArrKey.size(); ++i)
	{
		if (strcmp(name, ArrKey[i]->m_szName) == 0)
			return ArrKey[i]->m_id;
	}

	return -1;
}

ID CEffects::effectAdd(const char* name)
{
	CEffect* tmpeffect = new CEffect();
	if (name)
		strcpy(tmpeffect->m_szName, name);

	ID id = addEffect(tmpeffect);
	tmpeffect->m_idPool = poolAdd(id);
	//PoolExtend(tmpeffect->Pool);
	return id;
}

ID CEffects::poolAdd(ID ideff)
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
		CPool* tmparr = new CPool();
		tmparr->ideff = ideff;
		Pools.push_back(tmparr);
		id = Pools.size() - 1;
	}

	return Pools.size() - 1;
}

void CEffects::poolExtend(ID id)
{
	EFFECTS_POOL_PRECOND(id, _VOID);
	for (int i = 0; i < SXPARTICLES_POOL_RESERVE; ++i)
	{
		ID tmpid = effectCopyID(Pools[id]->ideff);
		ArrID[tmpid]->m_isBusy = false;
		ArrID[tmpid]->m_isOriginal = false;
		Pools[id]->arr.push_back(tmpid);
	}
}

void CEffects::poolDelete(ID id)
{
	EFFECTS_POOL_PRECOND(id, _VOID);

	for (int i = 0; i < Pools[id]->arr.size(); ++i)
	{
		effectDel(Pools[id]->arr[i]);
	}

	Pools[id]->ideff = -1;
	Pools[id]->arr.clear();
}

ID CEffects::poolGet(ID id)
{
	EFFECTS_POOL_PRECOND(id, -1);

	for (int i = 0; i < Pools[id]->arr.size(); ++i)
	{
		if (!(ArrID[Pools[id]->arr[i]]->m_isBusy) && !(ArrID[Pools[id]->arr[i]]->m_isEnable))
			return ArrID[Pools[id]->arr[i]]->m_id;
	}
	
	int oldsize = Pools[id]->arr.size();

	poolExtend(id);
	return ArrID[Pools[id]->arr[oldsize]]->m_id;
}

ID CEffects::effectInstanceByID(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, -1);

	ID ideff = poolGet(ArrID[id]->m_idPool);
	ArrID[ideff]->m_isBusy = true;

	return ideff;
}

ID CEffects::effectInstanceByName(const char* name)
{
	return effectInstanceByID(effectGetByName(name));
}

void CEffects::effectPlayByID(ID id, const float3* pos, const float3* dir)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ID ideff = poolGet(ArrID[id]->m_idPool);

	if (pos)
		effectSetPos(ideff, pos);

	if (dir)
		effectSetDir(ideff, dir);

	effectSetEnable(ideff, true);
}

void CEffects::effectPlayByName(const char* name, const float3* pos, const float3* dir)
{
	effectPlayByID(effectGetByName(name), pos, dir);
}

ID CEffects::addEffect(CEffect* obj)
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

	obj->m_id = idadd;
	ArrKey.push_back(obj);
	obj->m_idKey = ArrKey.size() - 1;
	return idadd;
}

int CEffects::effectGetCount()
{
	return ArrKey.size();
}

ID CEffects::effectGetIdOfKey(ID key)
{
	EFFECTS_EFFECT_PRECOND_KEY(key, -1);

	return ArrKey[key]->m_id;
}

void CEffects::effectDelete(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->m_isOriginal)
	{
		poolDelete(ArrID[id]->m_idPool);
		effectDel(id);
	}
	else
	{
		ArrID[id]->m_isBusy = false;
		effectSetEnable(id, false);
	}
}

void CEffects::effectDel(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	for (long i = ArrID[id]->m_idKey + 1; i < ArrKey.size(); ++i)
	{
		--(ArrKey[i]->m_idKey);
	}

	ArrID[id]->m_isEnable = false;
	ArrKey.erase(ArrID[id]->m_idKey);
	mem_delete(ArrID[id]);
}

void CEffects::effectSetName(ID id, const char* name)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (name)
		strcpy(ArrID[id]->m_szName, name);
}

void CEffects::effectGetName(ID id, char* name)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (name)
		strcpy(name, ArrID[id]->m_szName);
}

void CEffects::effectCompute(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (!ArrID[id]->m_isEnable)
		return;

	CEffect* eff = ArrID[id];
	int countlife = 0;

	static float4x4 mattrans;
	mattrans = eff->m_mRotate * eff->m_mTranslation;

	int countdead = 0;	//счетик живых партиклов
	for (int i = 0, l = eff->m_aEmitters.size(); i < l; ++i)
	{
		if (eff->m_aEmitters[i])
			eff->m_aEmitters[i]->compute(&mattrans);

		//если партиклы метрвы то инкрементируем счетчик
		if (!eff->m_aEmitters[i]->getEnable())
			++countdead;
		else //иначе партиклы живы, считаем объем эффекта
		{
			++countlife;
			if (countlife == 1)
			{
				eff->m_vCurrMax = eff->m_aEmitters[i]->m_vCurrMax;
				eff->m_vCurrMin = eff->m_aEmitters[i]->m_vCurrMin;
			}
			else
			{
				if (eff->m_aEmitters[i]->m_vCurrMax.x > eff->m_vCurrMax.x)
					eff->m_vCurrMax.x = eff->m_aEmitters[i]->m_vCurrMax.x;

				if (eff->m_aEmitters[i]->m_vCurrMax.y > eff->m_vCurrMax.y)
					eff->m_vCurrMax.y = eff->m_aEmitters[i]->m_vCurrMax.y;

				if (eff->m_aEmitters[i]->m_vCurrMax.z > eff->m_vCurrMax.z)
					eff->m_vCurrMax.z = eff->m_aEmitters[i]->m_vCurrMax.z;

				if (eff->m_aEmitters[i]->m_vCurrMin.x < eff->m_vCurrMin.x)
					eff->m_vCurrMin.x = eff->m_aEmitters[i]->m_vCurrMin.x;

				if (eff->m_aEmitters[i]->m_vCurrMin.y < eff->m_vCurrMin.y)
					eff->m_vCurrMin.y = eff->m_aEmitters[i]->m_vCurrMin.y;

				if (eff->m_aEmitters[i]->m_vCurrMin.z < eff->m_vCurrMin.z)
					eff->m_vCurrMin.z = eff->m_aEmitters[i]->m_vCurrMin.z;
			}
			
			eff->m_vCurrMin2 = SMVector3Transform(eff->m_vCurrMin, mattrans);
			eff->m_vCurrMax2 = SMVector3Transform(eff->m_vCurrMax, mattrans);
		}
	}

	//если количество мертвых объектов партиклов равно общему количесву объектов партиклов
	if (countdead == eff->m_aEmitters.size())
	{
		eff->m_isEnable = false;	//эффект мертв
		eff->m_isAlife = false;
	}
	else
		eff->m_isEnable = true;
}

void CEffects::effectComputeLighting(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	CEffect* eff = ArrID[id];

	if (!eff->m_isViewRender)
		return;

	if (!eff->m_isEnable)
		return;

	for(int i = 0, l = eff->m_aEmitters.size(); i < l; ++i)
	{
		if (eff->m_aEmitters[i] && eff->m_aEmitters[i]->getEnable())
		{
			eff->m_aEmitters[i]->computeLighting();
		}
	}
}

void CEffects::effectRender(ID id, DWORD timeDelta)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	CEffect* eff = ArrID[id];

	if (!eff->m_isViewRender)
		return;

	if (!eff->m_isEnable)
		return;

	for(int i = 0, l = eff->m_aEmitters.size(); i < l; ++i)
	{
		if(eff->m_aEmitters[i] && eff->m_aEmitters[i]->getEnable())
		{
			eff->m_aEmitters[i]->render(timeDelta, &eff->m_mRotate, &eff->m_mTranslation);
		}
	}
}

void CEffects::effectComputeAll()
{
	for (int i = 0, l = ArrID.size(); i < l; ++i)
	{
		if (ArrID[i])
			effectCompute(ArrID[i]->m_id);
	}
}

void CEffects::effectComputeLightingAll()
{
	for (int i = 0, l = ArrID.size(); i < l; ++i)
	{
		if (ArrID[i])
			effectComputeLighting(ArrID[i]->m_id);
	}
}

void CEffects::effectRenderAll(DWORD timeDelta)
{
	ID tmpid = -1;
	if (ArrSortSizeCurr <= 0)
		return;

	for (int i = ArrSortSizeCurr-1; i >= 0; --i)
	{
		tmpid = ArrSort[i];
		if (tmpid >= 0 && tmpid < ArrID.size() && ArrID[tmpid])
			effectRender(tmpid, timeDelta);
	}
}

bool CEffects::effectVisibleCom(ID id, const IFrustum* frustum, const float3* view)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	CEffect* eff = ArrID[id];
	if (!eff || !eff->m_isEnable)
		return false;

	float3 scenter;
	float sradius;

	scenter = (eff->m_vCurrMin2 + eff->m_vCurrMax2) * 0.5f;
	sradius = SMVector3Length(scenter - eff->m_vCurrMax2);
	
	eff->m_isViewRender = frustum->sphereInFrustum(&scenter, sradius);

	if (eff->m_isViewRender)
		eff->m_isViewRender = SGCore_OC_IsVisible(&(eff->m_vCurrMin2), &(eff->m_vCurrMax2));

	eff->m_fViewDist = SMVector3Length((scenter - (*view))) - sradius;

	return eff->m_isViewRender;
}

void CEffects::effectVisibleComAll(const IFrustum* frustum, const float3* view)
{
	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

	for(int i = 0, l = ArrID.size(); i < l; ++i)
	{
		if (ArrID[i])
			effectVisibleCom(i, frustum, view);
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
		if (!ArrKey[i]->m_isViewRender)
			continue;

		pos = 0;
		tmpdist = ArrKey[i]->m_fViewDist;
		for (int j = 0; j < ArrKey.size(); ++j)
		{
			if (i != j && ArrKey[j]->m_isViewRender && ArrKey[j]->m_fViewDist < tmpdist)
				++pos;
		}

		ArrSort[pos] = ArrKey[i]->m_id;

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

bool CEffects::effectGetVisible(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->m_isViewRender;
}

float CEffects::effectGetDistToView(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, 0);

	return ArrID[id]->m_fViewDist;
}


bool CEffects::effectGetEnable(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->m_isEnable;
}

void CEffects::effectSetEnable(ID id, bool isenable)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);
	CEffect* eff = ArrID[id];
	
	for (int i = 0; i < eff->m_aEmitters.size(); ++i)
	{
		eff->m_aEmitters[i]->setEnable(isenable);
	}

	eff->m_isEnable = isenable;
	
	if (!isenable)
		eff->m_isAlife = isenable;
}

bool CEffects::effectGetAlife(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->m_isAlife;
}

void CEffects::effectSetAlife(ID id, bool alife)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ArrID[id]->m_isAlife = alife;
	CEffect* eff = ArrID[id];

	if (!eff->m_isEnable && eff->m_isAlife)
		eff->m_isEnable = eff->m_isAlife;

	for (int i = 0; i < eff->m_aEmitters.size(); ++i)
	{
		eff->m_aEmitters[i]->setAlife(alife);
	}
}

void CEffects::effectSetPos(ID id, const float3* pos)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ArrID[id]->m_vPosition = *pos;
	ArrID[id]->m_mTranslation = SMMatrixTranslation(ArrID[id]->m_vPosition);
}

void CEffects::effectSetDir(ID id, const float3* dir)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);
	float3 tmpdir = *dir;
	//tmpdir.y = 0;
	tmpdir = SMVector3Normalize(tmpdir);
	ArrID[id]->m_vDirection = tmpdir;

	//static float3 f = SXPARTICLES_BASIS_DIR;
	static float3 f(0, 1, 0);
	//float3 f(0, 1, 0);
	float3 a = SMVector3Cross(f, tmpdir);
	float ang = acosf(SMVector3Dot(f, tmpdir));
	ArrID[id]->m_mRotate = SMMatrixRotationAxis(a, ang)/*SMQuaternion(a, ang).GetMatrix()*//*SMMatrixRotationX(SM_PIDIV2) * *//*SMMatrixRotationAxis(a, ang)*/;

	ArrID[id]->m_vRotation = SMMatrixToEuler(ArrID[id]->m_mRotate);
}

void CEffects::effectSetRot(ID id, const float3* rot)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ArrID[id]->m_vRotation = *rot;

	ArrID[id]->m_vDirection = SMEulerToVec(ArrID[id]->m_vRotation, SXPARTICLES_BASE_DIR);

	static float3 f = SXPARTICLES_BASE_DIR;
	//float3 f(0, -1, 0);
	float3 a = SMVector3Cross(f, ArrID[id]->m_vDirection);
	float ang = acosf(SMVector3Dot(f, ArrID[id]->m_vDirection));
	ArrID[id]->m_mRotate = SMMatrixRotationAxis(a, ang);
}

void CEffects::effectSetRot(ID id, const SMQuaternion & rot)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ArrID[id]->m_mRotate = rot.GetMatrix();

	ArrID[id]->m_vDirection = rot * float3(0, 1, 0);

	ArrID[id]->m_vRotation = SMMatrixToEuler(ArrID[id]->m_mRotate);
}

void CEffects::effectGetPos(ID id, float3* pos)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	*pos = ArrID[id]->m_vPosition;
}

void CEffects::effectGetDir(ID id, float3* dir)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	*dir = ArrID[id]->m_vDirection;
}

void CEffects::effectGetRot(ID id, float3* rot)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	*rot = ArrID[id]->m_vRotation;
}