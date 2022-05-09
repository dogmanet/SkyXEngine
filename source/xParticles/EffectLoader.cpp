#include "EffectLoader.h"

CEffectLoader::CEffectLoader(IXCore *pCore):
	m_pCore(pCore)
{
}

static char* WriteCurve(char* szCur, IXAnimationCurve *pCurve)
{
	// {"keys":[[fTime,fValue,fInTangent,fOutTangent],...],"pre":mode,"post":mode}
	szCur += sprintf(szCur, "{\"keys\":[");
	const XKeyframe *pFrame;
	for(UINT i = 0, l = pCurve->getKeyframeCount(); i < l; ++i)
	{
		pFrame = pCurve->getKeyAt(i);
		szCur += sprintf(szCur, "%s[%g,%g,%g,%g]", i == 0 ? "" : ",", pFrame->fTime, pFrame->fValue, pFrame->fInTangent, pFrame->fOutTangent);
	}
	szCur += sprintf(szCur, "],\"pre\":%d,\"post\":%d}", pCurve->getPreWrapMode(), pCurve->getPostWrapMode());

	return(szCur);
}

void ReadCurve(IXJSONObject *pObj, IXAnimationCurve *pCurve)
{
	if(!pObj)
	{
		return;
	}

	for(UINT i = 0, l = pObj->size(); i < l; ++i)
	{
		const char *szKey = pObj->getKey(i);
		if(!strcmp(szKey, "keys"))
		{
			IXJSONArray *pArr = pObj->at(i)->asArray();
			if(pArr)
			{
				pCurve->setKeyCount(pArr->size());
				XKeyframe keyFrame;
				for(UINT j = 0, jl = pArr->size(); j < jl; ++j)
				{
					IXJSONArray *pKeyArr = pArr->at(j)->asArray();
					if(pKeyArr)
					{
						keyFrame = {};
						pKeyArr->at(0) && pKeyArr->at(0)->getFloat(&keyFrame.fTime);
						pKeyArr->at(1) && pKeyArr->at(1)->getFloat(&keyFrame.fValue);
						pKeyArr->at(2) && pKeyArr->at(2)->getFloat(&keyFrame.fInTangent);
						pKeyArr->at(3) && pKeyArr->at(3)->getFloat(&keyFrame.fOutTangent);

						pCurve->setKey(j, keyFrame);
					}
				}
			}
		}
		else if(!strcmp(szKey, "pre"))
		{
			int64_t i64Val;
			if(pObj->at(i)->getInt64(&i64Val))
			{
				pCurve->setPreWrapMode((XCURVE_WRAP_MODE)i64Val);
			}
		}
		else if(!strcmp(szKey, "post"))
		{
			int64_t i64Val;
			if(pObj->at(i)->getInt64(&i64Val))
			{
				pCurve->setPostWrapMode((XCURVE_WRAP_MODE)i64Val);
			}
		}
	}
}

static void SaveMinMaxCurve(IXConfig *pConfig, const char *szSection, const char *szKey, IXMinMaxCurve *pCurve)
{
	char *szVal, *szCur;
	switch(pCurve->getMode())
	{
	case XMCM_CONSTANT:
		pConfig->setFloat(szSection, szKey, pCurve->getMax());
		break;
	case XMCM_TWO_CONSTANTS:
		pConfig->setVector2(szSection, szKey, float2_t(pCurve->getMin(), pCurve->getMax()));
		break;

	case XMCM_CURVE:
		szVal = szCur = (char*)alloca(sizeof(char) * (256 * (pCurve->getMaxCurve()->getKeyframeCount() + 1) + 64));
		// 256 * (frames + 1) + 64
		// {"max":{"keys":[[fTime,fValue,fInTangent,fOutTangent],...],"pre":mode,"post":mode},"m":fMultiplier}
		szCur += sprintf(szCur, "{\"max\":");
		szCur = WriteCurve(szCur, pCurve->getMaxCurve());
		szCur += sprintf(szCur, ",\"m\":%g}", pCurve->getMultiplier());
		pConfig->set(szSection, szKey, szVal);
		break;

	case XMCM_TWO_CURVES:
		szVal = szCur = (char*)alloca(sizeof(char) * (256 * (pCurve->getMinCurve()->getKeyframeCount() + pCurve->getMaxCurve()->getKeyframeCount() + 1) + 128));
		// 256 * (frames + 1) + 128
		// {"min":{...},"max":{"keys":[[fTime,fValue,fInTangent,fOutTangent],...],"pre":mode,"post":mode},"m":fMultiplier}
		szCur += sprintf(szCur, "{\"min\":");
		szCur = WriteCurve(szCur, pCurve->getMinCurve());
		szCur += sprintf(szCur, ",\"max\":");
		szCur = WriteCurve(szCur, pCurve->getMaxCurve());
		szCur += sprintf(szCur, ",\"m\":%g}", pCurve->getMultiplier());
		pConfig->set(szSection, szKey, szVal);
		break;
	}
}

static void ReadMinMaxCurve(IXConfig *pConfig, const char *szSection, const char *szKey, IXMinMaxCurve *pCurve)
{
	float2_t vec;
	float fVal;
	IXJSONObject *pObj;
	if(pConfig->tryGetVector2(szSection, szKey, &vec))
	{
		pCurve->setMode(XMCM_TWO_CONSTANTS);
		pCurve->setMin(vec.x);
		pCurve->setMax(vec.y);
		return;
	}

	if(pConfig->tryGetFloat(szSection, szKey, &fVal))
	{
		pCurve->setMode(XMCM_CONSTANT);
		pCurve->setMax(fVal);
		return;
	}

	if(pConfig->tryGetJsonObject(szSection, szKey, &pObj))
	{
		int iCurves = 0;
		for(UINT i = 0, l = pObj->size(); i < l; ++i)
		{
			const char *szKey = pObj->getKey(i);
			if(!strcmp(szKey, "m"))
			{
				if(pObj->at(i)->getFloat(&fVal))
				{
					pCurve->setMultiplier(fVal);
				}
				continue;
			}
			
			if(!strcmp(szKey, "min"))
			{
				ReadCurve(pObj->at(i)->asObject(), pCurve->getMinCurve());
				++iCurves;
			}
			else if(!strcmp(szKey, "max"))
			{
				ReadCurve(pObj->at(i)->asObject(), pCurve->getMaxCurve());
				++iCurves;
			}
		}

		if(iCurves == 1)
		{
			pCurve->setMode(XMCM_CURVE);
		}
		else if(iCurves == 2)
		{
			pCurve->setMode(XMCM_TWO_CURVES);
		}

		mem_release(pObj);
	}
}

bool CEffectLoader::loadFromFile(const char *szFile, CParticleEffect *pEffect)
{
	IXConfig *pConfig = m_pCore->newConfig();
	bool isLoaded = false;
	if(pConfig->open(szFile))
	{
		UINT uEmitterCount;
		if(pConfig->tryGetUint("effect", "emitters", &uEmitterCount) && uEmitterCount < 1000)
		{
			pEffect->setEmitterCount(uEmitterCount);
			char szSection[64], szKey[64];
			float fVal;
			int iVal;
			UINT uVal;
			bool bVal;
			float4_t vVal;

#define READ_VAL(mod, what, data, type, var, etype) if(pConfig->tryGet##type(szSection, mod "." #what, &var)){data->set##what(etype var);}
#define READ_FLOAT(mod, what, data) READ_VAL(mod, what, data, Float, fVal, _VOID)
#define READ_INT(mod, what, data, type) READ_VAL(mod, what, data, Int, iVal, (type))
#define READ_UINT(mod, what, data) READ_VAL(mod, what, data, Uint, uVal, _VOID)
#define READ_BOOL(mod, what, data) READ_VAL(mod, what, data, Bool, bVal, _VOID)
#define READ_V4(mod, what, data) READ_VAL(mod, what, data, Vector4, vVal, _VOID)

			for(UINT i = 0; i < uEmitterCount; ++i)
			{
				IXParticleEffectEmitter *pEmitter = pEffect->getEmitterAt(i);

				auto *pGenericData = pEmitter->getGenericData();
				READ_FLOAT("generic", Duration, pGenericData);
				READ_BOOL("generic", Looping, pGenericData);
				READ_BOOL("generic", Prewarm, pGenericData);
				READ_FLOAT("generic", StartDelay, pGenericData);
				ReadMinMaxCurve(pConfig, szSection, "generic.StartLifetimeCurve", pGenericData->getStartLifetimeCurve());
				ReadMinMaxCurve(pConfig, szSection, "generic.StartSpeedCurve", pGenericData->getStartSpeedCurve());
				ReadMinMaxCurve(pConfig, szSection, "generic.StartSizeCurve", pGenericData->getStartSizeCurve());
				READ_FLOAT("generic", FlipRotation, pGenericData);
				READ_INT("generic", CullingMode, pGenericData, XPARTICLE_CULLING_MODE);
				READ_FLOAT("generic", GravityModifier, pGenericData);
				READ_UINT("generic", MaxParticles, pGenericData);
				READ_INT("generic", RingBufferMode, pGenericData, XPARTICLE_RING_BUFFER_MODE);
				ReadMinMaxCurve(pConfig, szSection, "generic.RingBufferLoopRangeCurve", pGenericData->getRingBufferLoopRangeCurve());
				READ_INT("generic", SimulationSpace, pGenericData, XPARTICLE_SIMULATION_SPACE);
				READ_V4("generic", StartColor, pGenericData);
				ReadMinMaxCurve(pConfig, szSection, "generic.StartRotationCurve", pGenericData->getStartRotationCurve());
				READ_BOOL("generic", StartRotationSeparate, pGenericData);
				ReadMinMaxCurve(pConfig, szSection, "generic.StartRotationXCurve", pGenericData->getStartRotationXCurve());
				ReadMinMaxCurve(pConfig, szSection, "generic.StartRotationYCurve", pGenericData->getStartRotationYCurve());
				ReadMinMaxCurve(pConfig, szSection, "generic.StartRotationZCurve", pGenericData->getStartRotationZCurve());
				READ_BOOL("generic", StartSizeSeparate, pGenericData);
				ReadMinMaxCurve(pConfig, szSection, "generic.StartSizeXCurve", pGenericData->getStartSizeXCurve());
				ReadMinMaxCurve(pConfig, szSection, "generic.StartSizeYCurve", pGenericData->getStartSizeYCurve());
				ReadMinMaxCurve(pConfig, szSection, "generic.StartSizeZCurve", pGenericData->getStartSizeZCurve());

				auto *pEmissionData = pEmitter->getEmissionData();
				ReadMinMaxCurve(pConfig, szSection, "emission.RatePerSecondCurve", pEmissionData->getRatePerSecondCurve());
				ReadMinMaxCurve(pConfig, szSection, "emission.RatePerMeterCurve", pEmissionData->getRatePerMeterCurve());
				READ_UINT("emission", BurstsCount, pEmissionData);
				for(UINT j = 0, jl = pEmissionData->getBurstsCount(); j < jl; ++j)
				{
					IXParticleBurst *pBurst = pEmissionData->getBurstAt(j);
					sprintf(szKey, "emission.Burst_%u.Time", j);
					if(pConfig->tryGetFloat(szSection, szKey, &fVal))
					{
						pBurst->setTime(fVal);
					}
					sprintf(szKey, "emission.Burst_%u.Cycles", j);
					if(pConfig->tryGetUint(szSection, szKey, &uVal))
					{
						pBurst->setCycles(uVal);
					}
					sprintf(szKey, "emission.Burst_%u.Probability", j);
					if(pConfig->tryGetFloat(szSection, szKey, &fVal))
					{
						pBurst->setProbability(fVal);
					}
					sprintf(szKey, "emission.Burst_%u.IntervalCurve", j);
					ReadMinMaxCurve(pConfig, szSection, szKey, pBurst->getIntervalCurve());
				}

				auto *pShapeData = pEmitter->getShapeData();
				READ_INT("shape", Shape, pShapeData, XPARTICLE_SHAPE);
				READ_FLOAT("shape", Radius, pShapeData);
				READ_FLOAT("shape", RadiusThickness, pShapeData);
				READ_FLOAT("shape", Angle, pShapeData);
				READ_FLOAT("shape", Arc, pShapeData);
				READ_INT("shape", ArcMode, pShapeData, XPARTICLE_SHAPE_ARC_MODE);
				READ_FLOAT("shape", ArcSpread, pShapeData);
				ReadMinMaxCurve(pConfig, szSection, "shape.ArcSpeedCurve", pShapeData->getArcSpeedCurve());
				READ_FLOAT("shape", Length, pShapeData);
				READ_INT("shape", ConeEmitFrom, pShapeData, XPARTICLE_SHAPE_CONE_EMIT_FROM);
				READ_INT("shape", BoxEmitFrom, pShapeData, XPARTICLE_SHAPE_BOX_EMIT_FROM);
				READ_FLOAT("shape", DonutRadius, pShapeData);
				READ_BOOL("shape", AlignToDirection, pShapeData);
				READ_FLOAT("shape", RandomizeDirection, pShapeData);
				READ_FLOAT("shape", SpherizeDirection, pShapeData);
				READ_FLOAT("shape", RandomizePosition, pShapeData);

				auto *pVelocityLifetimeData = pEmitter->getVelocityLifetimeData();
				if(pConfig->tryGetBool(szSection, "velocityLifetime.Enabled", &bVal))
				{
					pVelocityLifetimeData->enable(bVal);
				}
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.LinearXCurve", pVelocityLifetimeData->getLinearXCurve());
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.LinearYCurve", pVelocityLifetimeData->getLinearYCurve());
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.LinearZCurve", pVelocityLifetimeData->getLinearZCurve());
				READ_INT("velocityLifetime", SimulationSpace, pVelocityLifetimeData, XPARTICLE_SIMULATION_SPACE);
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.OrbitalXCurve", pVelocityLifetimeData->getOrbitalXCurve());
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.OrbitalYCurve", pVelocityLifetimeData->getOrbitalYCurve());
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.OrbitalZCurve", pVelocityLifetimeData->getOrbitalZCurve());
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.OffsetXCurve", pVelocityLifetimeData->getOffsetXCurve());
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.OffsetYCurve", pVelocityLifetimeData->getOffsetYCurve());
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.OffsetZCurve", pVelocityLifetimeData->getOffsetZCurve());
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.RadialCurve", pVelocityLifetimeData->getRadialCurve());
				ReadMinMaxCurve(pConfig, szSection, "velocityLifetime.SpeedModifierCurve", pVelocityLifetimeData->getSpeedModifierCurve());

				auto *pLimitVelocityLifetimeData = pEmitter->getLimitVelocityLifetimeData();
				if(pConfig->tryGetBool(szSection, "limitVelocityLifetime.Enabled", &bVal))
				{
					pLimitVelocityLifetimeData->enable(bVal);
				}
				READ_BOOL("limitVelocityLifetime", SeparateAxes, pLimitVelocityLifetimeData);
				ReadMinMaxCurve(pConfig, szSection, "limitVelocityLifetime.SpeedCurve", pLimitVelocityLifetimeData->getSpeedCurve());
				ReadMinMaxCurve(pConfig, szSection, "limitVelocityLifetime.DampenCurve", pLimitVelocityLifetimeData->getDampenCurve());
				ReadMinMaxCurve(pConfig, szSection, "limitVelocityLifetime.DragCurve", pLimitVelocityLifetimeData->getDragCurve());
				READ_BOOL("limitVelocityLifetime", MultiplyBySize, pLimitVelocityLifetimeData);
				READ_BOOL("limitVelocityLifetime", MultiplyByVelocity, pLimitVelocityLifetimeData);
			}

#undef READ_V4
#undef READ_BOOL
#undef READ_UINT
#undef READ_INT
#undef READ_FLOAT
#undef READ_VAL
		}
		else
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "Invalid effect/emitters value in %s", szFile);
		}
	}

	mem_release(pConfig);
	return(isLoaded);
}

bool CEffectLoader::saveToFile(const char *szFile, CParticleEffect *pEffect)
{
	IXConfig *pConfig = m_pCore->newConfig();

	bool isSaved = false;

	if(pConfig->open(szFile))
	{
		/*
		[effect]
		emitters = <int>

		[emitter_<n>]
		<module>.<key> = <val>
		*/
		pConfig->setUint("effect", "emitters", pEffect->getEmitterCount());

		char szSection[64], szKey[64];
		for(UINT i = 0, l = pEffect->getEmitterCount(); i < l; ++i)
		{
			sprintf(szSection, "emitter_%u", i);

			IXParticleEffectEmitter *pEmitter = pEffect->getEmitterAt(i);

			auto *pGenericData = pEmitter->getGenericData();
			pConfig->setFloat(szSection, "generic.Duration", pGenericData->getDuration());
			pConfig->setBool(szSection, "generic.Looping", pGenericData->getLooping());
			pConfig->setBool(szSection, "generic.Prewarm", pGenericData->getPrewarm());
			pConfig->setFloat(szSection, "generic.StartDelay", pGenericData->getStartDelay());
			SaveMinMaxCurve(pConfig, szSection, "generic.StartLifetimeCurve", pGenericData->getStartLifetimeCurve());
			SaveMinMaxCurve(pConfig, szSection, "generic.StartSpeedCurve", pGenericData->getStartSpeedCurve());
			SaveMinMaxCurve(pConfig, szSection, "generic.StartSizeCurve", pGenericData->getStartSizeCurve());
			pConfig->setFloat(szSection, "generic.FlipRotation", pGenericData->getFlipRotation());
			pConfig->setInt(szSection, "generic.CullingMode", pGenericData->getCullingMode());
			pConfig->setFloat(szSection, "generic.GravityModifier", pGenericData->getGravityModifier());
			pConfig->setUint(szSection, "generic.MaxParticles", pGenericData->getMaxParticles());
			pConfig->setInt(szSection, "generic.RingBufferMode", pGenericData->getRingBufferMode());
			SaveMinMaxCurve(pConfig, szSection, "generic.RingBufferLoopRangeCurve", pGenericData->getRingBufferLoopRangeCurve());
			pConfig->setInt(szSection, "generic.SimulationSpace", pGenericData->getSimulationSpace());
			pConfig->setVector4(szSection, "generic.StartColor", pGenericData->getStartColor());
			SaveMinMaxCurve(pConfig, szSection, "generic.StartRotationCurve", pGenericData->getStartRotationCurve());
			pConfig->setBool(szSection, "generic.StartRotationSeparate", pGenericData->getStartRotationSeparate());
			SaveMinMaxCurve(pConfig, szSection, "generic.StartRotationXCurve", pGenericData->getStartRotationXCurve());
			SaveMinMaxCurve(pConfig, szSection, "generic.StartRotationYCurve", pGenericData->getStartRotationYCurve());
			SaveMinMaxCurve(pConfig, szSection, "generic.StartRotationZCurve", pGenericData->getStartRotationZCurve());
			pConfig->setBool(szSection, "generic.StartSizeSeparate", pGenericData->getStartSizeSeparate());
			SaveMinMaxCurve(pConfig, szSection, "generic.StartSizeXCurve", pGenericData->getStartSizeXCurve());
			SaveMinMaxCurve(pConfig, szSection, "generic.StartSizeYCurve", pGenericData->getStartSizeYCurve());
			SaveMinMaxCurve(pConfig, szSection, "generic.StartSizeZCurve", pGenericData->getStartSizeZCurve());

			auto *pEmissionData = pEmitter->getEmissionData();
			SaveMinMaxCurve(pConfig, szSection, "emission.RatePerSecondCurve", pEmissionData->getRatePerSecondCurve());
			SaveMinMaxCurve(pConfig, szSection, "emission.RatePerMeterCurve", pEmissionData->getRatePerMeterCurve());
			pConfig->setUint(szSection, "emission.BurstsCount", pEmissionData->getBurstsCount());
			for(UINT j = 0, jl = pEmissionData->getBurstsCount(); j < jl; ++j)
			{
				IXParticleBurst *pBurst = pEmissionData->getBurstAt(j);
				sprintf(szKey, "emission.Burst_%u.Time", j);
				pConfig->setFloat(szSection, szKey, pBurst->getTime());
				sprintf(szKey, "emission.Burst_%u.Cycles", j);
				pConfig->setUint(szSection, szKey, pBurst->getCycles());
				sprintf(szKey, "emission.Burst_%u.Probability", j);
				pConfig->setFloat(szSection, szKey, pBurst->getProbability());
				sprintf(szKey, "emission.Burst_%u.IntervalCurve", j);
				SaveMinMaxCurve(pConfig, szSection, szKey, pBurst->getIntervalCurve());
			}

			auto *pShapeData = pEmitter->getShapeData();
			pConfig->setInt(szSection, "shape.Shape", pShapeData->getShape());
			pConfig->setFloat(szSection, "shape.Radius", pShapeData->getRadius());
			pConfig->setFloat(szSection, "shape.RadiusThickness", pShapeData->getRadiusThickness());
			pConfig->setFloat(szSection, "shape.Angle", pShapeData->getAngle());
			pConfig->setFloat(szSection, "shape.Arc", pShapeData->getArc());
			pConfig->setInt(szSection, "shape.ArcMode", pShapeData->getArcMode());
			pConfig->setFloat(szSection, "shape.ArcSpread", pShapeData->getArcSpread());
			SaveMinMaxCurve(pConfig, szSection, "shape.ArcSpeedCurve", pShapeData->getArcSpeedCurve());
			pConfig->setFloat(szSection, "shape.Length", pShapeData->getLength());
			pConfig->setInt(szSection, "shape.ConeEmitFrom", pShapeData->getConeEmitFrom());
			pConfig->setInt(szSection, "shape.BoxEmitFrom", pShapeData->getBoxEmitFrom());
			pConfig->setFloat(szSection, "shape.DonutRadius", pShapeData->getDonutRadius());
			pConfig->setBool(szSection, "shape.AlignToDirection", pShapeData->getAlignToDirection());
			pConfig->setFloat(szSection, "shape.RandomizeDirection", pShapeData->getRandomizeDirection());
			pConfig->setFloat(szSection, "shape.SpherizeDirection", pShapeData->getSpherizeDirection());
			pConfig->setFloat(szSection, "shape.RandomizePosition", pShapeData->getRandomizePosition());

			auto *pVelocityLifetimeData = pEmitter->getVelocityLifetimeData();
			pConfig->setBool(szSection, "velocityLifetime.Enabled", pVelocityLifetimeData->isEnabled());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.LinearXCurve", pVelocityLifetimeData->getLinearXCurve());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.LinearYCurve", pVelocityLifetimeData->getLinearYCurve());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.LinearZCurve", pVelocityLifetimeData->getLinearZCurve());
			pConfig->setInt(szSection, "velocityLifetime.SimulationSpace", pVelocityLifetimeData->getSimulationSpace());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.OrbitalXCurve", pVelocityLifetimeData->getOrbitalXCurve());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.OrbitalYCurve", pVelocityLifetimeData->getOrbitalYCurve());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.OrbitalZCurve", pVelocityLifetimeData->getOrbitalZCurve());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.OffsetXCurve", pVelocityLifetimeData->getOffsetXCurve());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.OffsetYCurve", pVelocityLifetimeData->getOffsetYCurve());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.OffsetZCurve", pVelocityLifetimeData->getOffsetZCurve());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.RadialCurve", pVelocityLifetimeData->getRadialCurve());
			SaveMinMaxCurve(pConfig, szSection, "velocityLifetime.SpeedModifierCurve", pVelocityLifetimeData->getSpeedModifierCurve());

			auto *pLimitVelocityLifetimeData = pEmitter->getLimitVelocityLifetimeData();
			pConfig->setBool(szSection, "limitVelocityLifetime.Enabled", pLimitVelocityLifetimeData->isEnabled());
			pConfig->setBool(szSection, "limitVelocityLifetime.SeparateAxes", pLimitVelocityLifetimeData->getSeparateAxes());
			SaveMinMaxCurve(pConfig, szSection, "limitVelocityLifetime.SpeedCurve", pLimitVelocityLifetimeData->getSpeedCurve());
			SaveMinMaxCurve(pConfig, szSection, "limitVelocityLifetime.DampenCurve", pLimitVelocityLifetimeData->getDampenCurve());
			SaveMinMaxCurve(pConfig, szSection, "limitVelocityLifetime.DragCurve", pLimitVelocityLifetimeData->getDragCurve());
			pConfig->setBool(szSection, "limitVelocityLifetime.MultiplyBySize", pLimitVelocityLifetimeData->getMultiplyBySize());
			pConfig->setBool(szSection, "limitVelocityLifetime.MultiplyByVelocity", pLimitVelocityLifetimeData->getMultiplyByVelocity());
		}

		isSaved = pConfig->save();
	}

	mem_release(pConfig);

	return(isSaved);
}
