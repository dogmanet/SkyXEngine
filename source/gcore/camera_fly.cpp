
#pragma once

namespace SkyXEngine
{
namespace Core
{
namespace ControllMoving
{

CameraFly::CameraFly()
{
	ConfigFly = 0;
	bfIsFly = false;
	ActiveSection[0] = 0;
	CurrentSection[0] = 0;
	SecondSection[0] = 0;

	ActivePos = float3(0,0,0);
	LastPos = float3(0,0,0);

	ActiveAngleUpDown = 0;
	ActiveAngleRightLeft = 0;
	ActiveAngleRoll = 0;

	LastAngleUpDown = 0;
	LastAngleRightLeft = 0;
	LastAngleRoll = 0;

	LerpPos = 0;
	LerpAngleUpDown = 0;
	LerpAngleRightLeft = 0;
	LerpAngleRoll = 0;

	SecTimePos = 0;
	SecTimeRot = 0;
	CurrTime = 0;
}

CameraFly::~CameraFly()
{
	ConfigFly->Release();
}

void CameraFly::LoadInit(const char* path)
{
	ConfigFly = new SkyXEngine::Core::Loaders::LoaderConfig(path);
}

void CameraFly::Activate(const char* section)
{
		if(ConfigFly->SectionExists(section,true))
		{
			sprintf(ActiveSection,"%s",section);
			LoadSecondSection(true);
		}
		else
		{
			Core::InLog("FlyCamera: activate section not found [%s]\n",section);
			bfIsFly = false;
		}
}

void CameraFly::LoadSecondSection(bool isstart)
{
	bool is_section_exists = true;
	String second_section;
		if(isstart)
		{
				if(ConfigFly->SectionExists(ActiveSection,"start_section"))
				{
					second_section = ConfigFly->Get(ActiveSection,"start_section");
					Core::Data::ObjCamera->GetPosition(&OldPos);
					
					OldAngleUpDown = Core::Data::ObjCamera->GetRotationX();
					OldAngleRightLeft = Core::Data::ObjCamera->GetRotationY();
					OldAngleRoll = Core::Data::ObjCamera->GetRotationZ();
				}
				else
				{
					Core::InLog("FlyCamera: start section not found, active section [%s]\n",ActiveSection);
					is_section_exists = false;
				}
		}
		else
		{
				if(ConfigFly->SectionExists(ActiveSection,"second_section"))
					second_section = ConfigFly->Get(ActiveSection,"second_section");
				else
				{
					Core::InLog("FlyCamera: second section not found, active section [%s]\n",ActiveSection);
					is_section_exists = false;
				}
		}
		
		if(is_section_exists)
		{
			ActiveSection[0] = 0;
			sprintf(ActiveSection,"%s",second_section.c_str());
				if(StrValidate(ActiveSection))
				{
					SecTimePos = ConfigFly->Get(ActiveSection,"time_pos").ToUnsLongInt();
					SecTimeRot = ConfigFly->Get(ActiveSection,"time_rot").ToUnsLongInt();
					String strpos = ConfigFly->Get(ActiveSection,"pos");
					String strrot = ConfigFly->Get(ActiveSection,"rot");
					LerpPos = ConfigFly->Get(ActiveSection,"pos_lerp").ToBool();
					LerpAngleRoll = LerpAngleRightLeft = LerpAngleUpDown = ConfigFly->Get(ActiveSection,"rot_lerp").ToBool();
					String strsecond_section = ConfigFly->Get(ActiveSection,"second_section");

						
						if(strcmp(strpos.c_str(),"old") == 0)
						{
							ActivePos = OldPos;
						}
						else
						{
							char tmpstrpos[64];
							sprintf(tmpstrpos,"%s",strpos.c_str());
							char* tmppos = strtok(tmpstrpos," ,|");
								if(tmppos)
									ActivePos[0] = String(tmppos).ToDouble();

								for(int i=1;i<3 && (tmppos = strtok(0," ,|")) != NULL;i++)
								{
									ActivePos[i] = String(tmppos).ToDouble();
								}
						}
					
						if(strcmp(strrot.c_str(),"old") == 0)
						{
							ActiveAngleUpDown = OldAngleUpDown;
							ActiveAngleRightLeft = OldAngleRightLeft;
							ActiveAngleRoll = OldAngleRoll;
						}
						else
						{
							char tmpstrrot[64];
							sprintf(tmpstrrot,"%s",strrot.c_str());
							char* tmprot = strtok(tmpstrrot," ,|");
								if(tmprot)
									ActiveAngleUpDown = String(tmprot).ToDouble();

							tmprot = strtok(0," ,|");
								if(tmprot)
									ActiveAngleRightLeft = String(tmprot).ToDouble();

							tmprot = strtok(0," ,|");
								if(tmprot)
									ActiveAngleRoll = String(tmprot).ToDouble();
						}

					CurrTime = 0;
					bfIsFly = true;
				}
				else
				{
					bfIsFly = false;
				}
		}
		else
		{
			bfIsFly = false;
		}
}

inline bool CameraFly::IsFly()
{
	return bfIsFly;	
}

void CameraFly::Update(DWORD timeDelta)
{
		if(bfIsFly)
		{
			CurrTime += timeDelta;
			float percent_pos = float(CurrTime) / float(SecTimePos);
			
				if((SecTimePos >= SecTimeRot && CurrTime >= SecTimePos) || (SecTimePos <= SecTimeRot && CurrTime >= SecTimeRot))
				{
					Core::Data::ObjCamera->SetPosition(&ActivePos);
					LastPos = ActivePos;
					LastAngleUpDown = ActiveAngleUpDown;
					LastAngleRightLeft = ActiveAngleRightLeft;
					LastAngleRoll = ActiveAngleRoll;
					LoadSecondSection();
				}
				else
				{
						if(LerpPos)
							Core::Data::ObjCamera->SetPosition(&float3(lerp(LastPos.x,ActivePos.x,percent_pos),lerp(LastPos.y,ActivePos.y,percent_pos),lerp(LastPos.z,ActivePos.z,percent_pos)));
						else
							Core::Data::ObjCamera->SetPosition(&ActivePos);

						if(CurrTime < SecTimeRot)
						{
								if(LerpAngleUpDown)
									Core::Data::ObjCamera->RotUpDown((ActiveAngleUpDown - LastAngleUpDown) * ((float(timeDelta) * 0.001f) / (float(SecTimeRot) * 0.001f)));
								else
									Core::Data::ObjCamera->RotUpDown(ActiveAngleUpDown);

								if(LerpAngleRightLeft)
									Core::Data::ObjCamera->RotRightLeft((ActiveAngleRightLeft - LastAngleRightLeft) * ((float(timeDelta) * 0.001f) / (float(SecTimeRot) * 0.001f)));
								else
									Core::Data::ObjCamera->RotRightLeft(ActiveAngleRightLeft);

								if(LerpAngleRoll)
									Core::Data::ObjCamera->RotUpDown((ActiveAngleRoll - LastAngleRoll) * ((float(timeDelta) * 0.001f) / (float(SecTimeRot) * 0.001f)));
								else
									Core::Data::ObjCamera->RotUpDown(ActiveAngleRoll);
						}
				}
		}
}

};
};
};