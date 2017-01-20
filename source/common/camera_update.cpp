
#include <common\\camera_update.h>

namespace CameraUpdate
{
	//обработка вводы информации с клавиатуры
	void UpdateInputKeyBoard(DWORD timeDelta)
	{
		//обработка ходьбы
		if (SSInput_GetKeyState(DIK_W))
			GData::ObjCamera->PosFrontBack(100 * float(timeDelta) * 0.001f);

		if (SSInput_GetKeyState(DIK_S))
			GData::ObjCamera->PosFrontBack(-100 * float(timeDelta) * 0.001f);

		if (SSInput_GetKeyState(DIK_A))
			GData::ObjCamera->PosLeftRight(-100 * float(timeDelta) * 0.001f);

		if (SSInput_GetKeyState(DIK_D))
			GData::ObjCamera->PosLeftRight(100 * float(timeDelta) * 0.001f);
	}

	//обработка движения мыши вправо и влево
	void UpdateInputMouseRotate(DWORD timeDelta)
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
	void UpdateInputMouseUpDown(DWORD timeDelta)
	{
		UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
		UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
		POINT p;
		GetCursorPos(&p);
		POINT centr;
		centr.x = cx; centr.y = cy;

		if (cy != UINT(p.y))
		{
			GData::ObjCamera->PosUpDown(10 * float(timeDelta) * 0.001f * float(-int(p.y - cy)));
			//SetCursorPos(cx,centr.y);
		}
	}

	//центрирвоание курсора мыши
	void CentererCursor()
	{
		UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
		UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
		SetCursorPos(cx, cy);
	}

	void UpdateEditorial(DWORD timeDelta)
	{
		//обработка перемещения/вращения
		if (SSInput_GetKeyState(DIK_LCONTROL))
		{
			static bool IsFirstLBM = false;
			static bool IsFirstRBM = false;

			static bool IsSFirstLBM = false;
			static bool IsSFirstRBM = false;

			UpdateInputKeyBoard(timeDelta);

			//еси нажата левая кнопка мыши то можно вращать
			if (SSInput_GetButtonState(SXInputDev::LBUTTON))
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
			else if (SSInput_GetButtonState(SXInputDev::RBUTTON))
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
};