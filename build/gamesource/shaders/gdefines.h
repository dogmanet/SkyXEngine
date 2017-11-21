
/*
gdefines.h
Макроопределения
*/

#ifndef G_DEFINES
#define G_DEFINES

const half2 TexUV_Center = half2(0.5f,0.5f);

//максимальное значение для альфа теста растительности
#define GREEN_ALPHATEST_VALUE 	0.5

//коэфициент освещения дальнего сплита теней для pssm [0,1]
#define PSSM_SHADING_LAST 		0.1

//на сколько будут домножены координаты для смещения по текстуре со случайными значениями при генерации теней
#define SHADOWGEN_JITTER_SCALE 1024.0

//дистанция от камеры на которую не будет растространяться эффект motion blur (сделано для того чтобы не размывать худ)
#define PP_MOTION_BLUR_DISTNOBLUR 0.8

#define PI 3.141592654f

//свойства поверхности (освещаемая ли, прозрачная ли)
//{
	
//неосвещаемая и не прозрачная
#define SURFACE_NOTLIGHTING_NOTTRANSPARENCY 0.0

//освещаемая и непрозрачная
#define SURFACE_LIGHTING_NOTTRANSPARENCY 	0.333333

//неосвещаемая и прозрачная
#define SURFACE_NOTLIGHTING_TRANSPARENCY 	0.666667

//освещаемая и прозрачная
#define SURFACE_LIGHTING_TRANSPARENCY 		1.0

//}

//парметры для tone mapping
//{

//прибавляемое смещение к коэфициенту адаптации глаза к освещению (чтобы не было деления на 0)
#define TONE_MAPPING_ADAPT_ADD_BIAS 		0.0001f

//число которое будет поделено на коэфициент адаптации глаза к освещению
#define TONE_MAPPING_DENOMENATOR			0.1f

//}

//регистры данных для растительности
//{
#define GREEN_R_LESSENING	c59
#define GREEN_R_VIEWPOS		c60
#define GREEN_R_BSPHERE		c61
#define GREEN_R_BBMAX		c62
#define GREEN_R_BBMIN		c63
//}

//дистанция на которую распространяется детализированность шейдеров mtrl_land_
#define MTRL_LAND_DIST	100.f

//на сколько увеличиваем текстурные координаты для текстур микрорельефа и детальных текстур mtrl_land_
#define MTRL_LAND_TEXUV_SCALE	1024.f

//на сколько увеличиваем текстурные координаты для текстур микрорельефа и детальных текстур
#define MTRL_TEXUV_SCALE	32.f

//количество возможных лодов для отражений
#define MTRL_REF_LOD_COUNT	5.f

//параметр освещения "шероховатость поверхности", минимальное значение
#define PARAM_LIGHTING_ROUGHNESS_MIN	0.05f

//параметр освещения "шероховатость поверхности", максимальное значение
#define PARAM_LIGHTING_ROUGHNESS_MAX	0.4f

#else
	#error G_DEFINES double include
#endif