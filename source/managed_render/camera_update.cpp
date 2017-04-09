
#include <managed_render\\camera_update.h>


//обработка вводы информации с клавиатуры
void CameraUpdate::UpdateInputKeyBoard(DWORD timeDelta)
{
	//обработка ходьбы
	if (SSInput_GetKeyState(SIK_W))
		GData::ObjCamera->PosFrontBack(
		GData::CamWalkParamEditor.x *
		(SSInput_GetKeyState(SIK_LSHIFT) ? GData::CamWalkParamEditor.y : 1) *
		float(timeDelta) * 0.001f
		);

	if (SSInput_GetKeyState(SIK_S))
		GData::ObjCamera->PosFrontBack(
		-GData::CamWalkParamEditor.x *
		(SSInput_GetKeyState(SIK_LSHIFT) ? GData::CamWalkParamEditor.y : 1) * GData::CamWalkParamEditor.w *
		float(timeDelta) * 0.001f
		);

	if (SSInput_GetKeyState(SIK_A))
		GData::ObjCamera->PosLeftRight(
		-GData::CamWalkParamEditor.x *
		(SSInput_GetKeyState(SIK_LSHIFT) ? GData::CamWalkParamEditor.y : 1) * GData::CamWalkParamEditor.z *
		float(timeDelta) * 0.001f
		);

	if (SSInput_GetKeyState(SIK_D))
		GData::ObjCamera->PosLeftRight(
		GData::CamWalkParamEditor.x *
		(SSInput_GetKeyState(SIK_LSHIFT) ? GData::CamWalkParamEditor.y : 1) * GData::CamWalkParamEditor.z *
		float(timeDelta) * 0.001f
		);
}

//обработка движения мыши вправо и влево
void CameraUpdate::UpdateInputMouseRotate(DWORD timeDelta)
{
	UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
	UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
	POINT p;
	GetCursorPos(&p);
	POINT centr;
	centr.x = cx; centr.y = cy;

	if (cx != UINT(p.x))
	{
		GData::ObjCamera->RotRightLeft(float(timeDelta) * 0.001f * float(int(p.x - cx)));
		//SetCursorPos(centr.x,cy);
	}

	if (cy != UINT(p.y))
	{
		GData::ObjCamera->RotUpDown(float(timeDelta) * 0.001f * float(int(p.y - cy)));
		//SetCursorPos(cx,centr.y);
	}
}

//обработка движения мыши вверх вниз
void CameraUpdate::UpdateInputMouseUpDown(DWORD timeDelta)
{
	UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
	UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
	POINT p;
	GetCursorPos(&p);
	POINT centr;
	centr.x = cx; centr.y = cy;

	if (cy != UINT(p.y))
	{
		GData::ObjCamera->PosUpDown(5 * float(timeDelta) * 0.001f * float(-int(p.y - cy)));
		//SetCursorPos(cx,centr.y);
	}
}

//центрирвоание курсора мыши
void CameraUpdate::CentererCursor()
{
	UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
	UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
	SetCursorPos(cx, cy);
}

void CameraUpdate::UpdateEditorial(DWORD timeDelta)
{
	//обработка перемещения/вращения
	if (SSInput_GetKeyState(SIK_LCONTROL))
	{
		static bool IsFirstLBM = false;
		static bool IsFirstRBM = false;

		static bool IsSFirstLBM = false;
		static bool IsSFirstRBM = false;

		UpdateInputKeyBoard(timeDelta);

		//еси нажата левая кнопка мыши то можно вращать
		if (SSInput_GetButtonState(SIM_LBUTTON))
		{
			//если не первый раз нажата ЛКМ то совершаем действие
			if (IsFirstLBM)
				UpdateInputMouseRotate(timeDelta);
			//иначе просто говорим что впервые и устанавливаем курсор в центр
			else
				IsFirstLBM = true;
			CentererCursor();
		}
		//если нажата правая кнопка мыши то можно поднимать и опускать камеру
		else if (SSInput_GetButtonState(SIM_RBUTTON))
		{
			if (IsFirstRBM)
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