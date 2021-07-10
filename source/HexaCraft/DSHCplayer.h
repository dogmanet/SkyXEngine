#ifndef _DSHCplayer_H_
#define _DSHCplayer_H_

#include "DSbase.h"
#include "DSphysics.h"

#include "DSHCdefines.h"

class DSbaseTool;
class CChunk;

enum
{
	HC_PLAYER_MOVE_NONE = 0,
	HC_PLAYER_MOVE_FORWARD = 0x01,
	HC_PLAYER_MOVE_BACKWARD = 0x02,
	HC_PLAYER_MOVE_LEFT = 0x04,
	HC_PLAYER_MOVE_RIGHT = 0x08,
	HC_PLAYER_MOVE_CROUCH = 0x10,
	HC_PLAYER_MOVE_JUMP = 0x20,
	HC_PLAYER_MOVE_RUN = 0x40,

	HC_PLAYER_MOVE_STOP = 0xFFFF,
};

class DSHCplayer: public DSbaseEntity
{
	DECLARE_CLASS(DSHCplayer, DSbaseEntity);
	UINT m_uMoveDir;

	bool m_bOnGround;

	DScamera * m_pCamera;

	
	int m_iXblock;
	int m_iYblock;
	int m_iZblock;

	float m_fXblock;
	float m_fYblock;
	float m_fZblock;

	btCollisionShape * m_pCollideShape;
	btRigidBody * m_pRigidBody;
	btPairCachingGhostObject * m_pGhostObject;
	btKinematicCharacterController * m_pCharacter;

	bool m_bIsReady;

	DSbaseTool * m_pActiveTool;

	DSHCaimPos pAimPos;

	ID3D11Buffer * m_pLVB;

	btConvexHullShape * m_pBlockCollideShape;
	btRigidBody * m_pBlockRigidBody;

public:
	CChunk * m_pInChunk;

	DSHCplayer();

	void Move(UINT dir, bool start);

	void Advance(DS_TIME);

	bool IsOnGround();

	~DSHCplayer();

	DScamera * GetCamera();

	CChunk * GetChunk();

	void GetBlockIn(int * pX, int * pY, int * pZ);
	void GetBlockInPos(float * pX, float * pY, float * pZ);

	void Render();

	void PrimaryAttack(bool st);
	void SecondaryAttack(bool st);

	void PlaceBlock(UINT type);
	void RemoveBlock();
};

#endif
