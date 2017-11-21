
/*
pptm_common.h
Основные данные для tone mapping
*/

static const half3 HDRLumsWeights = half3(0.298f, 0.585f, 0.117f);
static half HDRLumFloor = 0.05f * 0.05f;

static const int HDR_MAX_SAMPLES = 16;
half2 ArrOffsets[HDR_MAX_SAMPLES];