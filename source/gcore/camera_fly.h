
#pragma once

class CameraFly
{
public:

	SX_ALIGNED_OP_MEM

	CameraFly();
	~CameraFly();

	void LoadInit(const char* path);
	void LoadSecondSection(bool isstart=false);
	void Activate(const char* section);
	void Update(DWORD timeDelta);
	inline bool IsFly();

	SkyXEngine::Core::Loaders::LoaderConfig* ConfigFly;

	char ActiveSection[32];
	char CurrentSection[32];
	char SecondSection[32];

	//параметры камеры до того момента как полет взял управление
	float3 OldPos;
	float OldAngleUpDown;
	float OldAngleRightLeft;
	float OldAngleRoll;


	float3 ActivePos;
	float3 LastPos;

	float ActiveAngleUpDown;
	float ActiveAngleRightLeft;
	float ActiveAngleRoll;

	float LastAngleUpDown;
	float LastAngleRightLeft;
	float LastAngleRoll;

	bool LerpPos;
	bool LerpAngleUpDown;
	bool LerpAngleRightLeft;
	bool LerpAngleRoll;

	DWORD SecTimePos;
	DWORD SecTimeRot;
	DWORD CurrTime;
	bool bfIsFly;
};