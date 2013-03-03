// AIScriptFunction.cpp

#include "stdafx.h"
#include "Obj.h"
#include "Obj_Pet.h"
#include "Obj_Character.h"
#include "Obj_Monster.h"
#include "Obj_Human.h"
#include "AI_Character.h"
#include "AI_Monster.h"
#include "AI_Human.h"
#include "AI_Pet.h"
#include "GameTable.h"
#include "TimeManager.h"
#include "Scene.h"
#include "AIScriptDef.h"
#include "AIScriptFunction.h"

using namespace Combat_Module::Skill_Module;

AISFuncMap g_AISFuncTable[] =
{
	REGISTERAISFUNC(AIS_GetBuffState)
	REGISTERAISFUNC(AIS_GetAIState)
	REGISTERAISFUNC(AIS_GetHP)
	REGISTERAISFUNC(AIS_GetMP)
	REGISTERAISFUNC(AIS_Rand)
	REGISTERAISFUNC(AIS_GetEBuffState)
	REGISTERAISFUNC(AIS_GetEMP)
	REGISTERAISFUNC(AIS_GetEHP)
	REGISTERAISFUNC(AIS_GetEMenPai)
	REGISTERAISFUNC(AIS_GetELevel)
	REGISTERAISFUNC(AIS_IsCanSpeak)
	REGISTERAISFUNC(AIS_IsHasEnemy)
	REGISTERAISFUNC(AIS_GetESex)
	REGISTERAISFUNC(AIS_GetCamp)
	REGISTERAISFUNC(AIS_GetPK)
	REGISTERAISFUNC(AIS_GetETeamNum)
	REGISTERAISFUNC(AIS_IsEHasSpecialItem)
	REGISTERAISFUNC(AIS_GetSingleDamage)
	REGISTERAISFUNC(AIS_GetEnemyNum)
	REGISTERAISFUNC(AIS_IsBoss)
	REGISTERAISFUNC(AIS_BeSkill)
	REGISTERAISFUNC(AIS_ToFlee)
	REGISTERAISFUNC(AIS_IsCanSkill)
	REGISTERAISFUNC(AIS_ToSkill)
	REGISTERAISFUNC(AIS_ToSummon)
	REGISTERAISFUNC(AIS_ToLongAttack)
	REGISTERAISFUNC(AIS_ToFollow)
	REGISTERAISFUNC(AIS_ToSpeak)
	REGISTERAISFUNC(AIS_SetTimes)
	REGISTERAISFUNC(AIS_SetPRI)
	REGISTERAISFUNC(AIS_ToAssistTeammate)
	REGISTERAISFUNC(AIS_SetPatrolID)
	REGISTERAISFUNC(AIS_StartPatrol)
	//REGISTERAISFUNC(AIS_PetToAttack)

};

AISMacroMap g_AISMacroTable[] = 
{
	"SIDLE",			SIDLE,
	"SAPPROACH",		SAPPROACH,
	"SFLEE",			SFLEE,
	//"SUSESKILL",		SUSESKILL,
	"SATTACK",			SATTACK,
	//"SCHANGEENEMY",	SCHANGEENEMY,
	//"SWAIT",			SWAIT,
	//"SFASTFLEE",		SFASTFLEE,
	//"SARRIVE",		SARRIVE,
	"SFOLLOW",			SFOLLOW,
	"SPATROL",			SPATROL,
	"SRETURN",			SRETURN,
	"SKILLSECTION",		SKILLSECTION,
	"ONBESKILLSECTION",	ONBESKILLSECTION,
	"ONDAMAGESECTION",	ONDAMAGESECTION,
	"ONDEADSECTION",	ONDEADSECTION,
	//"YUN",			YUN,
	//"ZHONGDU",		ZHONGDU,
	//"DEAD",			DEAD,
	//"SLEEP",			SLEEP,
	//"HAPPY",			HAPPY,
	//"ANGRY",			ANGRY,
	//"SORROW",			SORROW,
	//"ENJOY",			ENJOY,
	//"PHYSICSKILL",	PHYSICSKILL,
	//"MAGICSKILL",		MAGICSKILL,
	//"ASSISTSKILL",	ASSISTSKILL,

};

const PLAISFunc GetFuncPtrbyName(const CHAR* funcName)
{
	__ENTER_FUNCTION
	for (INT i = 0; i < sizeof(g_AISFuncTable)/sizeof(AISFuncMap); ++i)
	{
		if (strcmp(funcName, g_AISFuncTable[i].funcname) == 0) 
		{
			return g_AISFuncTable[i].Func;
		}
	}
	__LEAVE_FUNCTION
	return NULL ;
}

const CHAR*	 GetFuncNamebyPtr(const PLAISFunc FuncPtr)
{
	__ENTER_FUNCTION
	for (INT i = 0; i < sizeof(g_AISFuncTable)/sizeof(AISFuncMap); ++i)
	{
		if (FuncPtr == g_AISFuncTable[i].Func) 
		{
			return g_AISFuncTable[i].funcname;
		}
	}
	__LEAVE_FUNCTION
	return NULL ;

}

const INT GetMacrobyName(const CHAR* MacroName)
{
	__ENTER_FUNCTION
	for (INT i = 0; i < sizeof(g_AISMacroTable)/sizeof(AISMacroMap); ++i)
	{
		if (strcmp(MacroName, g_AISMacroTable[i].Macroname) == 0) 
		{
			return g_AISMacroTable[i].MacroID;
		}
	}
	
	__LEAVE_FUNCTION
	return -1;
}

/******************************************************************************
 * Desc		: ��ȡ����״̬
 * pChar	: ���������ߣ��磺Monster��Pet
 * param0	: invalid
 * param1	: invalid
 * param2	: invalid
 */
INT AIS_GetBuffState(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/******************************************************************************
* Desc		: ��ȡ����AI״̬
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetAIState(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}
	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER) 
	{
		//return	((Obj_Monster*)pChar)->GetMonsterAI()->GetAIStatus();
		return pChar->GetAIObj()->GetAIState()->GetStateID();
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/******************************************************************************
* Desc		: ��ȡ����HP
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetHP(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}
	if(pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		return (const_cast<Obj_Character*>(pChar)->GetHP()*100) / const_cast<Obj_Character*>(pChar)->GetMaxHP();
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ��ȡ����MP
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetMP(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}
	if(pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		return (const_cast<Obj_Character*>(pChar)->GetMP()*100) / (const_cast<Obj_Character*>(pChar)->GetMaxHP());
	}
	
	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ���ȡֵ
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_Rand(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return RandGen::GetRand(0, 100);
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ����״̬
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetEBuffState(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;

}
/********************************************************************************
* Desc		: ����MP
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetEMP(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return const_cast<Obj_Character*>(pChar)->GetMP();
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ����HP
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetEHP(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return const_cast<Obj_Character*>(pChar)->GetHP();
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ��������
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetEMenPai(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	if(pChar->GetObjType() == Obj::OBJ_TYPE_HUMAN)
	{
		return ((Obj_Human*)pChar)->GetMenPai();
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ���˵ȼ�
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetELevel(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return pChar->GetLevel();
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: �Ƿ���˵��
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_IsCanSpeak(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;

}
/********************************************************************************
* Desc		: �Ƿ��е���
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_IsHasEnemy(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER) 
	{
		return ((Obj_Monster*)pChar)->GetMonsterAI()->HasEnemy();
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: �з��Ա�
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetESex(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}
	
	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		Obj_Character* pCurEnemy = ((Obj_Monster*)pChar)->GetMonsterAI()->GetCurEnemy();
		if (pCurEnemy && pCurEnemy->GetObjType() == Obj::OBJ_TYPE_HUMAN) 
		{
			return	((Obj_Human*)pCurEnemy)->GetSex();
		}
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ��Ӫ�ж�
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetCamp(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		Obj_Character* pCurEnemy = ((Obj_Monster*)pChar)->GetMonsterAI()->GetCurEnemy();
		return  ((Obj_Character*)pChar)->IsEnemy(pCurEnemy);
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: PKֵ
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetPK(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;

}
/********************************************************************************
* Desc		: �з���������
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetETeamNum(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	if (pChar->GetObjType() == Obj_Character::CHARACTER_CLASS_HUMAN) 
	{
		return ((Obj_Human*)pChar)->__GetTeamMemberCount();
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: �з��Ƿ���������Ʒ
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_IsEHasSpecialItem(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;

}
/********************************************************************************
* Desc		: �����˺�ֵ
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetSingleDamage(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER) 
	{
		return ((((Obj_Monster*)pChar)->GetMonsterAI()->GetSingleDamage())*100) / const_cast<Obj_Character*>(pChar)->GetMaxHP() ;
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ���˸���
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_GetEnemyNum(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER) 
	{
		return ((Obj_Monster*)pChar)->GetMonsterAI()->GetEnemyCount();
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: �Ƿ�ΪBOSS
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_IsBoss(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;

}
/********************************************************************************
* Desc		: ���з���������
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: ����ID
* param1	: invalid
* param2	: invalid
*/
INT AIS_BeSkill(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ����
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: ��������
* param1	: X����
* param2	: Z����
*/
INT AIS_ToFlee(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER) 
	{
		AI_Monster::SAICommand command;
		command.commandType = AI_Monster::E_COMMAND_TYPE_TOFLEE;
		command.param[0] = param0;
		command.param[1] = param1;
		command.param[2] = param2;
		((Obj_Monster*)pChar)->GetMonsterAI()->PushCommand(command);
	}
	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: �Ƿ����ͷż���
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: ����ID
* param1	: invalid
* param2	: invalid
*/
INT  AIS_IsCanSkill(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		Assert(NULL && "AIS_IsCanSkill...pChar=NULL...");
		return 0;
	}

	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER) 
	{
		Obj_Character* pCurEnemy = ((Obj_Monster*)pChar)->GetMonsterAI()->GetCurEnemy();

		SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(param0);
		if (NULL != pSkillTemplate) 
		{/** �ⲿ���Ժ��ɼ���ģ����֧�� */
			/* ����û�о�����������������ͷ� */
			AI_Monster* pAIMonster = ((Obj_Monster*)pChar)->GetMonsterAI();
			if (!pAIMonster)
			{
				Assert(NULL && "AIS_IsCanSkill...pAIMonster=NULL...");
				return 0;
			}

			if (pSkillTemplate->GetOptimalRangeMin() == -1 && pSkillTemplate->GetOptimalRangeMax() == -1) 
			{
				pAIMonster->SetSkillErrorCode(OR_OK);
				return 1;
			}

			if (!pCurEnemy)
			{
				pAIMonster->SetSkillErrorCode(OR_ERROR);
				return (1);
			}
			
			FLOAT fDist = MySqrt(const_cast<Obj_Character*>(pChar)->getWorldPos(),pCurEnemy->getWorldPos());
			if (fDist >= pSkillTemplate->GetOptimalRangeMin() && fDist <= pSkillTemplate->GetOptimalRangeMax()) 
			{
				pAIMonster->SetSkillErrorCode(OR_OK);
				return 1;
			}
			else
			{
				pAIMonster->SetSkillID(param0);
				pAIMonster->SetSkillErrorCode(OR_OUT_RANGE);
				return 0;
	        }
		}
	}
	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ʹ�ü���
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: ����ID
* param1	: invalid
* param2	: invalid
*/
INT AIS_ToSkill(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		Assert(NULL && "AIS_ToSkill...pChar=NULL...");
		return 0;
	}

	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER) 
	{
		Obj_Character* pCurEnemy = ((Obj_Monster*)pChar)->GetMonsterAI()->GetCurEnemy();
		if (!pCurEnemy) 
		{
			return 0;
		}
		AI_Character* pAIChar = pChar->GetAIObj();
		if (!pAIChar)
		{
			Assert(NULL && "AIS_ToSkill...pAIChar=NULL...");
			return 0;
		}
		ORESULT oResult = pAIChar->UseSkill(param0, pCurEnemy->GetID(),
			pCurEnemy->getWorldPos()->m_fX, pCurEnemy->getWorldPos()->m_fZ ,INVALID_ID) ;

		AI_Monster* pAIMonster = ((Obj_Monster*)pChar)->GetMonsterAI();
		if (pAIMonster)
		{
			pAIMonster->SetSkillID(param0);
			pAIMonster->SetSkillErrorCode(oResult);
		}
		else
		{
			Assert(NULL && "AIS_ToSkill...pAIMonster=NULL...");
			return 0;
		}
        if (oResult == OR_OK)
		{
			return 1;
		}
		else if (oResult ==OR_COOL_DOWNING)
		{// ����ǵ�ǰ��������ʹ���У��򽫸ü��ܻ��������� �´�ʩ�Ÿü��ܣ�
			pAIMonster->SetNextSkillID(param0);
			return (0);
		}
		else
		{
			return (0);
		}
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: �ٻ�����
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: �ٻ�����
* param1	: �ٻ�����
* param2	: invalid
*/
INT AIS_ToSummon(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	DWORD dwTime = g_pTimeManager->CurrentTime() ;
	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		((Obj_Monster*)pChar)->GetMonsterAI()->SummonTeammate(dwTime, param0, param1);
		return 0;
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: Զ�̹���
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_ToLongAttack(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;

}
/********************************************************************************
* Desc		: ����
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_ToFollow(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		return 0;
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;

}
/********************************************************************************
* Desc		: ����˵��
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: ������ʼID
* param1	: ����ID��Χ
* param2	: invalid
*/
INT AIS_ToSpeak(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		Assert(NULL && "AIS_ToSpeak...pChar=NULL...");
		return 0;
	}
	
	Obj* pObj = ((Obj*)pChar);

	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		((Obj_Monster*)pChar)->PaoPao(param0, param1);
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}
/********************************************************************************
* Desc		: ���ø�����Ϊ��ִ�д���
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT AIS_SetTimes(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	return 0;
}
/********************************************************************************
* Desc		: ���ø�����Ϊ�����ȼ�
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT  AIS_SetPRI(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	return 0;
}
/********************************************************************************
* Desc		: Э������
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: ����ID
* param1	: invalid
* param2	: invalid
*/
INT AIS_ToAssistTeammate(const Obj_Character *pChar, INT param0, INT param1, INT param2)
{
	return 0;
}

/********************************************************************************
* Desc		: ����Ѳ�߱��
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: Ѳ�߱��id
* param1	: invalid
* param2	: invalid
*/
INT  AIS_SetPatrolID( const Obj_Character *pChar, INT param0, INT param1, INT param2 )
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		Assert(NULL && "AIS_SetPatrolID...pChar=NULL...");
		return 0;
	}
	
	Obj* pObj = ((Obj*)pChar);

	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		((Obj_Monster*)pChar)->SetPatrolId(param0);
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}

/********************************************************************************
* Desc		: ��ʼѲ��
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: invalid
* param1	: invalid
* param2	: invalid
*/
INT  AIS_StartPatrol( const Obj_Character *pChar, INT param0, INT param1, INT param2 )
{
	__ENTER_FUNCTION
	Assert(pChar);
	if (!pChar) 
	{
		Assert(NULL && "AIS_StartPatrol...pChar=NULL...");
		return 0;
	}
	
	Obj* pObj = ((Obj*)pChar);

	if (pChar->GetObjType() == Obj::OBJ_TYPE_MONSTER)
	{
		AI_Monster* pAIMonster = ((Obj_Monster*)pChar)->GetMonsterAI();
		if (pAIMonster)
		{
			pAIMonster->StartPatrol();
		}
	}

	return 0;
	__LEAVE_FUNCTION
	return 0;
}


/********************************************************************************
* Desc		: ��������ȥ����
* pChar		: ���������ߣ��磺Monster��Pet
* param0	: �������ͣ����﹦���������������� 
* param1	: invalid
* param2	: invalid
*/
//INT  AIS_PetToAttack(const Obj_Character *pChar, INT param0, INT param1, INT param2)
//{
//	__ENTER_FUNCTION
//	if (!pChar)
//	{
//		Assert(NULL && "AIS_PetToAttack...pChar=NULL...");
//		return 0;
//	}
//	if (pChar->GetObjType() == Obj::OBJ_TYPE_PET)
//	{
//		////temp code {
//		//param0 = 0;
//		//((Obj_Pet*)pChar)->GetPetAI()->GetSkillByType(param0);
//		////}
//	}
//
//	return 0;
//	__LEAVE_FUNCTION
//	return 0;
//}
