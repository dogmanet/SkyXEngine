#ifndef _SXANIM_H_
#define _SXANIM_H_


#include "ModelFile.h"

#include <common/assotiativearray.h>
#include <common/array.h>
#include <common/string.h>

#define SX_DLL

#include <gdefines.h>

#ifdef _SERVER
#	define IDirect3DDevice9 void
#else
#	define D3D_DEBUG_INFO
#	include <d3d9.h>
#	include <d3dx9.h>
#	pragma comment(lib, "d3dx9.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

class IAnimPlayer;
class ISXFrustum;
class ISXBound;

typedef void(*AnimStateCB)(int slot, ANIM_STATE as, IAnimPlayer * pAnim);
typedef void(*AnimProgressCB)(int slot, float progress, IAnimPlayer * pAnim);

class IAnimPlayer
{
public:
	IAnimPlayer()
	{
	};
	virtual ~IAnimPlayer()
	{
	};

	SX_ALIGNED_OP_MEM;

	virtual void Advance(unsigned long int dt) = 0;

#ifndef _SERVER
	virtual void Render() = 0;
#endif

	virtual void SetModel(const char * file) = 0;

	virtual void Play(const char * name, UINT iFadeTime = 0, UINT slot = 0) = 0; // name: Animation name; changeTime: time to fade to this animation from previous
	virtual void Stop(UINT slot = 0) = 0;
	virtual void Resume(UINT slot = 0) = 0;

	virtual void SetProgress(float progress, UINT slot = 0) = 0;
	virtual void SetAdvance(bool set, UINT slot = 0) = 0;

	virtual void StartActivity(const String & name, UINT iFadeTime = 0, UINT slot = 0) = 0;

	virtual void SetBoneController(const String & name, float value, MODEL_BONE_CTL what) = 0;

	virtual SMMATRIX GetBoneTransform(UINT id) = 0;
	virtual UINT GetBone(const char * str) = 0;
	virtual UINT GetBoneCount() const = 0;
	virtual void GetBoneName(UINT id, char * name, int len) const = 0;

	virtual inline bool PlayingAnimations() = 0;
	virtual inline bool PlayingAnimations(const char* name) = 0;
	virtual void StopAll() = 0;


	virtual int GetActiveSkin() = 0;
	virtual void SetSkin(int n) = 0;

	virtual void SetPos(const float3 & pos) = 0;
	virtual float3 GetPos() = 0;

	virtual void SetOrient(const SMQuaternion & pos) = 0;
	virtual SMQuaternion GetOrient() = 0;


	virtual SMMATRIX GetWorldTM() = 0;

	virtual AnimStateCB SetCallback(AnimStateCB cb) = 0;
	virtual AnimProgressCB SetProgressCB(AnimProgressCB cb) = 0;

	virtual ModelSequence const * GetCurAnim(int slot) = 0;

	virtual const ISXBound * GetBound() const = 0;
};

SX_LIB_API void SXAnim_0Create();
SX_LIB_API void SXAnim_0Kill();

SX_LIB_API void SGCore_Dbg_Set(report_func rf);

SX_LIB_API void SXAnim_UpdateSetThreadNum(int num);
SX_LIB_API void SXAnim_Update(int thread = 0);
SX_LIB_API void SXAnim_Sync();
SX_LIB_API void SXAnim_Render(ID for_id=0);

SX_LIB_API IAnimPlayer * SXAnim_CreatePlayer(const char * mdl = NULL);

//просчитать видимость всех моделей для фрустума 
SX_LIB_API void SXAnim_ModelsComVisible(
	const ISXFrustum * frustum,	//фрустум для которого считаем видимость моделей
	const float3 * viewpos,		//позиция источника фрустума чтобы просчитать дистанцию
	ID id_arr = 0			//идентификатор массива информации о видимости для фрустума, создается через SXAnim_ModelsAddArrForCom, если 0 то считаем в дефолтный
	);

SX_LIB_API ID SXAnim_ModelsAddArrForCom();				//добавить массив просчетов, возвращает его идентификатор
SX_LIB_API void SXAnim_ModelsDelArrForCom(ID id_arr);	//удалить массив просчетов с номером id_arr


#endif
