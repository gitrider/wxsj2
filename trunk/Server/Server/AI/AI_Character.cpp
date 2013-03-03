// AI_Character.cpp

#include "stdafx.h"
#include "Scene.h"
#include "Obj_Character.h"
#include "GameTable.h"
#include "PathFinder.h"
#include "AI_Character.h"

using namespace Combat_Module::Skill_Module;

#define RANDOM_TIME (2000)
#define RANDOM_DIST (15)

AI_Character::AI_Character( VOID )
{
	m_pCharacter = NULL;
	m_nIntervalTime = RANDOM_TIME;
	_state = g_StateList.InstanceState(ESTATE_IDLE);
}

AI_Character::~AI_Character( VOID )
{
}

BOOL AI_Character::Init( Obj_Character *pCharacter )
{
	m_pCharacter	= pCharacter;
	return TRUE;
}

VOID AI_Character::Term( VOID )
{
	m_pCharacter	= NULL;
}

// all interface of actions
ORESULT AI_Character::Obj_Move(const WORLD_POS* pTar)
{
	Obj_Character* pCharacter = GetCharacter();
	if (!pCharacter)
	{
		Assert(NULL && "AI_Character::MoveTo...pCharacter = NULL...");
		return OR_ERROR;
	}
	if (TRUE == pCharacter->IsLimitMove() )
	{
		return OR_LIMIT_MOVE;
	}
	if (pCharacter->IsDie())
	{
		return OR_DIE;
	}

	int numNode = 0;
	const WORLD_POS* pCur = pCharacter->getWorldPos();
	WORLD_POS posNode[MAX_CHAR_PATH_NODE_NUMBER];
	WORLD_POS posCur = *pCur;
	pCharacter->getScene()->GetMap()->VerifyPos( const_cast<WORLD_POS*>(pTar) );
	pCharacter->getScene()->GetMap()->GetPathFinder()->FindPath(&posCur,
								const_cast<WORLD_POS*>(pTar), posNode, numNode);	

	if( numNode == 0 )
		return OR_ERROR;

	return _state->Obj_Move(this, -1, numNode, posNode );
}

ORESULT AI_Character::Obj_UseSkill(ObjID_t idSkill, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, INT iHurtDelayTime, FLOAT fDir/* = -1.f*/, GUID_t guidTarget/*=INVALID_ID*/)
{
	__ENTER_FUNCTION
	return _state->Obj_UseSkill(this, idSkill, idTarget, fTargetX, fTargetZ, iHurtDelayTime, fDir, guidTarget);
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT AI_Character::Obj_UseItem(ID_t nBagIndex, ObjID_t nTargetObj, WORLD_POS const& posTarget, PET_GUID_t const& guidTargetPet, ID_t nTargetItem)
{
	__ENTER_FUNCTION
	return _state->Obj_UseItem(this, nBagIndex, nTargetObj, posTarget, guidTargetPet, nTargetItem);
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT AI_Character::Jump(VOID)
{
	__ENTER_FUNCTION
	return _state->Jump(this);
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT	AI_Character::Stop(VOID)
{
	__ENTER_FUNCTION
	return _state->Stop(this);
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT	AI_Character::Stall(VOID)
{
	__ENTER_FUNCTION
	return _state->Stall(this);
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT	AI_Character::UseAbility(VOID)
{
	__ENTER_FUNCTION
	return _state->UseAbility(this);
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT	AI_Character::Move(const WORLD_POS* pTar)
{
	__ENTER_FUNCTION
	Obj_Character* pCharacter = GetCharacter();
	if (!pCharacter)
	{
		Assert(NULL && "AI_Character::MoveTo...pCharacter = NULL...");
		return OR_ERROR;
	}
	if (TRUE == pCharacter->IsLimitMove() )
	{
		return OR_LIMIT_MOVE;
	}

	int numNode = 0;
	const WORLD_POS* pCur = pCharacter->getWorldPos();
	WORLD_POS posNode[MAX_CHAR_PATH_NODE_NUMBER];
	WORLD_POS posCur = *pCur;
	pCharacter->getScene()->GetMap()->VerifyPos( const_cast<WORLD_POS*>(pTar) );
	if (FALSE == pCharacter->getScene()->GetMap()->IsCanGo(*pTar))
	{// 为不可走区域
		return OR_IMPASSABLE_ZONE;
	}
	pCharacter->getScene()->GetMap()->GetPathFinder()->FindPath(&posCur,
								const_cast<WORLD_POS*>(pTar), posNode, numNode);	

	if( numNode == 0 )
		return OR_ERROR;

	return Move( -1, numNode, posNode );
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT AI_Character::Move(INT nHandleID, WORD wNumTargetPos, const WORLD_POS *paTargetPos)
{
	__ENTER_FUNCTION
	return _state->Move(this, nHandleID, wNumTargetPos, paTargetPos);
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT AI_Character::UseSkill(ObjID_t idSkill, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, INT iHurtDelayTime, FLOAT fDir/* = -1.f*/, GUID_t guidTarget)
{
	__ENTER_FUNCTION
	return _state->UseSkill(this, idSkill, idTarget, fTargetX, fTargetZ, iHurtDelayTime, fDir, guidTarget);
	__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT AI_Character::UseItem(ID_t nBagIndex, ObjID_t nTargetObj, WORLD_POS const& posTarget, PET_GUID_t const& guidTargetPet, ID_t nTargetItem)
{
	__ENTER_FUNCTION
	return _state->UseItem(this, nBagIndex, nTargetObj, posTarget, guidTargetPet, nTargetItem);
	__LEAVE_FUNCTION
	return OR_ERROR;
}

// common logic
VOID	AI_Character::AI_Logic_Terror(UINT uTime)
{
	__ENTER_FUNCTION
	Obj_Character* pCharacter = GetCharacter();
	if (!pCharacter) {
		return ;
	}

	//if (!_state->IsTerrorState(this) ) {
	//	Stop();
	//	ChangeState(ESTATE_IDLE);
	//}

	if (RANDOM_TIME == m_nIntervalTime || m_nIntervalTime <= 0) {
		FLOAT fLen = (FLOAT)(rand() % RANDOM_DIST);
		WORLD_POS pos = GetRandPosOfCircle(pCharacter->getWorldPos(), fLen);

        Obj_Move(&pos);
		m_nIntervalTime = rand() % RANDOM_TIME;

		return ;
	}

	m_nIntervalTime -= pCharacter->GetLogicTime();
	//if (pCharacter->IsMoving() ) {
	//	return ;
	//}

	__LEAVE_FUNCTION
}

// event interface.
VOID AI_Character::OnDie(Obj *pKiller/* = NULL*/)
{
	__ENTER_FUNCTION
	_state->OnDie(this, pKiller);
	__LEAVE_FUNCTION
}

VOID AI_Character::OnRelive(Obj *pKiller/* = NULL*/)
{
}

VOID AI_Character::OnDamage(INT nDamage, Obj_Character* pAttacker)
{
	__ENTER_FUNCTION
	_state->OnDamage(this, nDamage, pAttacker);
	__LEAVE_FUNCTION
}

VOID AI_Character::OnBeSkill(Obj_Character* pCharacter, INT nGoodEffect)
{
	__ENTER_FUNCTION
	_state->OnBeSkill(this, pCharacter, nGoodEffect);
	__LEAVE_FUNCTION
}

WORLD_POS AI_Character::GetRandPosOfCircle( const WORLD_POS* pPos,FLOAT fRadio )
{/** 生成以pPos为圆心，fRadio为半径的圆周上的随机一点 */
	WORLD_POS Tar;
__ENTER_FUNCTION
	/** 假设半径为10的圆 */
	FLOAT Vx = (FLOAT)(rand() % 11) ;
	INT nDir = rand() % 2 ;
	if ( !nDir )
		Vx = -Vx ;

	FLOAT	Vz = (FLOAT)sqrt( 100 - Vx*Vx );
	nDir = rand() % 2 ;
	if ( !nDir )
		Vz = - Vz ;

	Tar.m_fX = pPos->m_fX + (Vx/10)*fRadio;
	Tar.m_fZ = pPos->m_fZ + (Vz/10)*fRadio;
__LEAVE_FUNCTION
	return Tar;
}