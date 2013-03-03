/********************************************************************
	created:	2008/03/18
	created:	18:3:2008   10:47
	filename: 	MapServer\Server\Ai\AI_Horse.h
	file path:	MapServer\Server\Ai
	file base:	AI_Horse
	file ext:	h
	author:		Richard
	
	purpose:	
*********************************************************************/
#ifndef AI_Horse_h__
#define AI_Horse_h__

#include "Type.h"
#include "OResultDef.h"
#include "AI_Character.h"
#include "AI_HorseDef.h"

class Obj_Horse;
class AI_Horse : public AI_Character
{
public:

	enum {	BODYSTAYTIME = 5000,};

	AI_Horse(VOID);
	virtual ~AI_Horse(VOID);

	virtual BOOL Init(Obj_Character *pCharacter);
	virtual VOID Term(VOID);

	virtual VOID Relive(BOOL bSkillRelive);
    
	Obj_Horse* GetCharacter(VOID) const { return ((Obj_Horse*)AI_Character::GetCharacter()); }
	// 处理客户端由主人发起的主动技能操作
	ORESULT PushCommand_UseSkill(ObjID_t idSkill, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, GUID_t guidTarget);

//=======================================================================================
// 逻辑状态、事件相关
//=======================================================================================    
protected:
	virtual VOID Event_OnDie(Obj *pKiller = NULL);
				
	virtual VOID AI_Logic_Idle(UINT uTime);
	virtual VOID AI_Logic_Dead(UINT uTime);
	virtual VOID AI_Logic_Combat(UINT uTime);

//=======================================================================================
// 行为相关
//=======================================================================================    
public:
	struct SLogicSkill_Param
	{
		SkillID_t	idSkill;
		ObjID_t		idTarget;
		WORLD_POS	TargetPos;
		FLOAT		fDir;
		GUID_t		guidTarget;

		SLogicSkill_Param()
		{
			Cleanup();
		}
		VOID Cleanup()
		{
			idSkill		= INVALID_ID;
			idTarget	= INVALID_ID;
			TargetPos.CleanUp();
			fDir		= 0.f;
			guidTarget	= INVALID_ID;
		}
		BOOL IsValid() const
		{
			if (idSkill != INVALID_ID)
			{
				return TRUE;
			}
			return FALSE;
		}
	};

	// 触发baby进行跟随
	VOID	Baby_Go(const WORLD_POS *paTargetPos);
	VOID	BabyToAttack(VOID);
	VOID	After_Die(VOID);
	// 处理缓存中的技能	
	BOOL	ProcessSkillInCache(BOOL IsAttackState = TRUE);

protected:
	VOID	SetSkillParamCache(const SLogicSkill_Param& rSkillParam) {
		m_SkillParamCache = rSkillParam;
	}
	const SLogicSkill_Param& GetSkillParamCache(VOID) const {
		return m_SkillParamCache;
	}

	VOID	ChangeMoveMode(VOID);
	FLOAT	CalcDistSqrOfToOwner(VOID);
	BOOL	ApproachToOwner(VOID);
	VOID	ToRandMove(VOID);
	BOOL	IsCombatBeOver(VOID);
    	

private:
	SLogicSkill_Param	m_SkillParamCache;
	CMyTimer			m_RandMoveTimer;			// 随机移动的计时器
	CMyTimer			m_BodyTimer;				// 尸体存在的时间
	CMyTimer			m_ScanImpactOfOwnerTimer;	// 扫描主人身上BUFF的计时器
	INT					m_nDelayTime;				// 临时延迟变量
    	
};

#endif // AI_Horse_h__
