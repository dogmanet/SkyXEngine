
/*
gdefines.h
Макроопределения
*/
 
#ifndef __G_DEFINES
#define __G_DEFINES

//! координаты центра текстуры
static const half2 g_vTexUVcenter = half2(0.5, 0.5);

//! коэффициент освещения дальнего сплита теней для pssm [0,1]
#define PSSM_SHADING_LAST 		0.1

//! на сколько будут домножены координаты для смещения по текстуре со случайными значениями при генерации теней
#define SHADOWGEN_JITTER_SCALE 1024.0

//! дистанция от камеры, на которую не будет растространяться эффект motion blur (сделано для того чтобы не размывать худ)
#define PP_MOTION_BLUR_DISTNOBLUR 1.0

//! дистанция от камеры плавного появления SSAO, чтобы близко не было сильного затенения
#define PP_SSAO_DIST_EMERSION 3.0

//! количество принимаемого исходного цвета
#define PP_LENS_FLARE_COUNT_DEST_COLOR 0.8

//! PI :)
#define PI 3.141592654f

//! количество лодов текстур для сцены
#define SCENE_COUNT_TEXTURE_LOD	10


/*
С параметрами устранения shadow acne пришлось долго возиться подбирая наиболее оптимальные, надо иметь ввиду при правках
*/

//! коэффициент смещения вершин по нормали, для уменьшения объем модели, для устранения shadow acne
#define SHADOW_SCOPE_BIAS 0.01

//! добавочное смещение глубины для теней, для устранения shadow acne
#define SHADOW_SLOPE_BIAS 0.00001

//! множитель slope смещения теней, для устранения shadow acne
#define SHADOW_SLOPE_SCALE 1.8


//! минимально допустимое значение отражательной способности материала для принятия ambient цвета
#define F0_MIN_4_DEST_AMBIENT 0.25

//! максимальнео количество слоев
#define LAYERS_COUNT_MAX 255.0

static const half g_fUnit256 = 1.0/255.0;

static const half g_fUnit128 = 1.0/127.0;

//! слой непрозрачной геометрии
#define LAYER_OPAQUE 1

//##########################################################################

/*! \name Возможные слои
@{*/

//! непрозрачные неосвещаемые
#define MTLTYPE_UNLIT	0

//! прозрачные освещаемые
#define MTLTYPE_LIGHT	1


//! сравнение слоев
//#define EQUAL_LAYER(layer1, layer2) (abs(layer1 - layer2) < 0.1)

//!@}

//##########################################################################

/*! \name парметры для tone mapping
@{*/

//! прибавляемое смещение к коэфициенту адаптации глаза к освещению (чтобы не было деления на 0)
#define TONE_MAPPING_ADAPT_ADD_BIAS 		0.0001f

//! число которое будет поделено на коэффициент адаптации глаза к освещению
#define TONE_MAPPING_DENOMENATOR			0.1f

//!@}

//##########################################################################

//! значение для альфа теста растительности
#define GREEN_ALPHATEST_VALUE 	0.7

//! значение альфа теста при построении глубины, нужно учитывать что растительность будет отсекаться именно по этому значению, а не по #GREEN_ALPHATEST_VALUE
#define SHADOW_DEPTH_ALPHATEST_VALUE 	0.5

//регистры данных для растительности
//{
#define GREEN_R_LESSENING	c59
#define GREEN_R_VIEWPOS		c60
#define GREEN_R_BSPHERE		c61
#define GREEN_R_BBMAX		c62
#define GREEN_R_BBMIN		c63
//}

//##########################################################################

//! регистр данных для вершинного шейдера анимационных моделей
#define ANIM_R_DATA c16

//! дистанция на которую распространяется детализированность шейдеров mtrl_land_
#define MTRL_LAND_DIST	100.f

//! на сколько увеличиваем текстурные координаты для текстур микрорельефа и детальных текстур mtrl_land_
#define MTRL_LAND_TEXUV_SCALE	1024.f

//! на сколько увеличиваем текстурные координаты для текстур микрорельефа и детальных текстур
#define MTRL_TEXUV_SCALE	32.f

//! количество возможных лодов для отражений
#define MTRL_REF_LOD_COUNT	5.f

//! параметр освещения "шероховатость поверхности", минимальное значение
#define PARAM_LIGHTING_ROUGHNESS_MIN	0.05f

//! параметр освещения "шероховатость поверхности", максимальное значение
#define PARAM_LIGHTING_ROUGHNESS_MAX	0.4f

#else
	#error G_DEFINES double include
#endif