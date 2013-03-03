// AI_Pet.h
// 
///////////////////////////////////////////////////////

#ifndef __PETAI_H__
#define __PETAI_H__

#include "Type.h"
#include "OResultDef.h"
#include "AI_Character.h"
#include "AI_PetDef.h"

class Obj_Pet;
class AI_Pet : public AI_Character
{
public:

	enum {	BODYSTAYTIME = 5000,};

	AI_Pet(VOID);
	virtual ~AI_Pet(VOID);

	virtual BOOL Init(Obj_Character *pCharacter);
	virtual VOID Term(VOID);
	//virtual BOOL Logic(UINT uTime);

	virtual VOID Relive(BOOL bSkillRelive);
    
	Obj_Pet* GetCharacter(VOID) const { return ((Obj_Pet*)AI_Character::GetCharacter()); }
	// ����ͻ��������˷�����������ܲ���
	ORESULT PushCommand_UseSkill(ObjID_t idSkill, ObjID_t idTarget, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, GUID_t guidTarget);

//=======================================================================================
// �߼�״̬���¼����
//=======================================================================================    
protected:
	virtual VOID Event_OnDie(Obj *pKiller = NULL);
				
	virtual VOID AI_Logic_Idle(UINT uTime);
	virtual VOID AI_Logic_Dead(UINT uTime);
	virtual VOID AI_Logic_Combat(UINT uTime);

//=======================================================================================
// ��Ϊ���
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

	// ����baby���и���
	VOID Baby_Go(const WORLD_POS *paTargetPos);

	VOID GetStandPos( Obj_Character* pChar, INT iIndex, const WORLD_POS* paTargetPos, FLOAT fDistanceRefix, WORLD_POS &oOutPos );

	VOID	BabyToAttack(VOID);
	VOID	After_Die(VOID);
	// �������еļ���	
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
    
	FLOAT	GetGoodRefix( FLOAT iBase, FLOAT iRefix );

private:
	SLogicSkill_Param	m_SkillParamCache;
	CMyTimer			m_RandMoveTimer;	// ����ƶ��ļ�ʱ��
	CMyTimer			m_BodyTimer;		// ʬ����ڵ�ʱ��
	CMyTimer			m_ScanImpactOfOwnerTimer;	// ɨ����������BUFF�ļ�ʱ��
	INT					m_nDelayTime;		// ��ʱ�ӳٱ���
    	
};

#endif	// __PETAI_H__
