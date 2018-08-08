
/*
pptm_common.h
Основные данные для tone mapping
*/

//! веса с которыми будут браться цвета сцены для выявления общей освещенности
static const half3 g_sLumsWeights = half3(0.298, 0.585, 0.117);

//! значение добавляемое к текущей общей освещенности, чем меньше тем светлее, и наоборот, (0, 1)
static half g_sLumFloor = 0.0025;

#define HDR_MAX_SAMPLES 16

//! смещения для выборки
half2 g_aOffsets[HDR_MAX_SAMPLES];