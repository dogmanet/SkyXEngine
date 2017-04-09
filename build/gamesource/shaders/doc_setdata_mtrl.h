
vs:
float4x4 World;
float4x4 View;
float4x4 Projection;

float4x4 WorldView; //нужно дл€ отражени€ с кубической текстуры

float4x4 WorldViewProjection;

float4 CameraPosition;

float2 TimeDelta;
//x - прошедшее врем€ с начала рендера
//y - прошедшее врем€ рендера с прошедшего кадра
float4 Param;
float2 NearFar; //ближн€€ и дальн€€ плоскости отсечени€

ps:
float4x4 World;
float4x4 View;
float4x4 Projection;

float2 TimeDelta;
//x - прошедшее врем€ с начала рендера
//y - прошедшее врем€ рендера с прошедшего кадра

float4 CameraPosition;

float4 Param;
float2 NearFar; //ближн€€ и дальн€€ плоскости отсечени€

sampler2D ColorSampler:register(s0);	//текстура с основным цветом

sampler2D MaskSampler:register(s1);		//маска дл€ детальности/микрорельефа

//4 детальных текстуры
sampler2D DetailRSampler:register(s2);
sampler2D DetailGSampler:register(s3);
sampler2D DetailBSampler:register(s4);
sampler2D DetailASampler:register(s5);

//4 микрорельефных текстуры
sampler2D MDRSampler:register(s6);
sampler2D MDGSampler:register(s7);
sampler2D MDBSampler:register(s8);
sampler2D MDASampler:register(s9);

sampler2D RF0SSIsSampler:register(s10);	//параметры дл€ освещени€
//r - roughness - шероховатость
//g - f0 - отражательна€ способность поверхности
//b - soft specular - м€гкость переходов спекул€ра
//a - intens specular - интенсивность спекул€ра

sampler2D/samplerCUBE ReflectionSampler:register(s12);	//отражение
sampler2D RefractionSampler:register(s13);				//преломление
sampler2D DepthLinearSampler:register(s14);				//линейна€ глубина