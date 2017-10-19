
#include "camera_update.h"

//обработка вводы информации с клавиатуры
void CameraUpdate::UpdateInputKeyBoard(DWORD timeDelta)
{
	//обработка ходьбы
	if(SSInput_GetKeyState(SIK_W))
		GData::ObjCamera->PosFrontBack(
		GData::CamWalkParamEditor.x *
		(SSInput_GetKeyState(SIK_LSHIFT) ? GData::CamWalkParamEditor.y : 1) *
		float(timeDelta) * 0.001f
		);

	if(SSInput_GetKeyState(SIK_S))
		GData::ObjCamera->PosFrontBack(
		-GData::CamWalkParamEditor.x *
		(SSInput_GetKeyState(SIK_LSHIFT) ? GData::CamWalkParamEditor.y : 1) * GData::CamWalkParamEditor.w *
		float(timeDelta) * 0.001f
		);

	if(SSInput_GetKeyState(SIK_A))
		GData::ObjCamera->PosLeftRight(
		-GData::CamWalkParamEditor.x *
		(SSInput_GetKeyState(SIK_LSHIFT) ? GData::CamWalkParamEditor.y : 1) * GData::CamWalkParamEditor.z *
		float(timeDelta) * 0.001f
		);

	if(SSInput_GetKeyState(SIK_D))
		GData::ObjCamera->PosLeftRight(
		GData::CamWalkParamEditor.x *
		(SSInput_GetKeyState(SIK_LSHIFT) ? GData::CamWalkParamEditor.y : 1) * GData::CamWalkParamEditor.z *
		float(timeDelta) * 0.001f
		);
}

//обработка движения мыши вправо и влево
void CameraUpdate::UpdateInputMouseRotate(DWORD timeDelta)
{
	static const float * sense = GET_PCVAR_FLOAT("cl_mousesense");

	int dx, dy;

	SSInput_GetMouseDelta(&dx, &dy);

	if(dx)
	{
		GData::ObjCamera->RotRightLeft(float(timeDelta) * *sense * float(dx));
		//SetCursorPos(centr.x,cy);
	}

	if(dy)
	{
		GData::ObjCamera->RotUpDown(float(timeDelta) * *sense * float(dy));
		//SetCursorPos(cx,centr.y);
	}
}

//обработка движения мыши вверх вниз
void CameraUpdate::UpdateInputMouseUpDown(DWORD timeDelta)
{
	static const float * sense = GET_PCVAR_FLOAT("cl_mousesense");

	int dy;

	SSInput_GetMouseDelta(NULL, &dy);

	if(dy)
	{
		GData::ObjCamera->PosUpDown(5 * float(timeDelta) * *sense * float(-dy));
	}
}

//центрирвоание курсора мыши
void CameraUpdate::CentererCursor()
{
	RECT rc;
	GetWindowRect(GetForegroundWindow(), &rc);
	SetCursorPos((rc.right + rc.left) / 2, (rc.bottom + rc.top) / 2);
}

void CameraUpdate::UpdateEditorial(DWORD timeDelta)
{
	//обработка перемещения/вращения
	if(SSInput_GetKeyState(SIK_LCONTROL))
	{
		static bool IsFirstLBM = false;
		static bool IsFirstRBM = false;

		static bool IsSFirstLBM = false;
		static bool IsSFirstRBM = false;

		UpdateInputKeyBoard(timeDelta);

		//еси нажата левая кнопка мыши то можно вращать
		if(SSInput_GetKeyState(SIM_LBUTTON))
		{
			//если не первый раз нажата ЛКМ то совершаем действие
			if(IsFirstLBM)
				UpdateInputMouseRotate(timeDelta);
			//иначе просто говорим что впервые и устанавливаем курсор в центр
			else
				IsFirstLBM = true;
			CentererCursor();
		}
		//если нажата правая кнопка мыши то можно поднимать и опускать камеру
		else if(SSInput_GetKeyState(SIM_RBUTTON))
		{
			if(IsFirstRBM)
				UpdateInputMouseUpDown(timeDelta);
			else
				IsFirstRBM = true;
			CentererCursor();
		}
		else
		{
			IsFirstLBM = false;
			IsFirstRBM = false;
		}
	}
}