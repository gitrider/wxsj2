
#include "StdAfx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"

#include "GMGameInterface_Script_Skill_Study.h"

#include "LuaPlus.h"
#include "Global.h"
#include "DBC/GMDataBase.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Procedure/GameProcedure.h"
#include "..\Object\Logic\Character\Obj_Character.h"
#include "..\Object\Logic\Character\Obj_PlayerMySelf.h"
#include "..\DataPool\GMDP_Struct_Skill.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\Network\NetManager.h"
#include "CGSkillUpgrade.h"


namespace SCRIPT_SANDBOX
{
	SkillStudy SkillStudy::s_SkillStudy;
	LuaPlus::LuaObject* SkillStudy::s_pMetaTable = NULL;

	/* ------------------------------------------------
	// 学习一个新技能
	参数1	INT		技能ID
	返回 0 -- 不成功, 1 -- 成功
	------------------------------------------------ */
	INT SkillStudy::StudySkill( LuaPlus::LuaState* state )
	{
		if (0 == GetStudyState( state ))
		{
			state->PushInteger(0);
			return 1;
		}

		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[2] param parameter error");
		}

		INT nSkillID	= args[2].GetInteger();

		// 要学习的技能级别
		INT nSkillLevel = 0;
		const SCLIENT_SKILL* pSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(nSkillID);
		if (pSkill)
		{
			nSkillLevel = pSkill->m_nLevel;
		}
		++nSkillLevel;


		// 是否已经是最大级别
		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nSkillID);
		if (pSkillData && nSkillLevel > pSkillData->m_nMaxLevel)
		{
			state->PushInteger(0);
			return 1;
		}

		
		// 判断学习条件，爬技能树。
		if (ClimbStudyTree( nSkillID, nSkillLevel ))
		{
			CGSkillUpgrade msg;

			msg.SetSkillID(nSkillID);
			msg.SetLevel(nSkillLevel);

			CNetManager::GetMe()->SendPacket( &msg );

			state->PushInteger(1);
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}

	/* ------------------------------------------------
	// 得到 该技能的可学习状态
	参数1	INT		技能ID	
	返回 0 -- 不可学, 1 -- 可学
	------------------------------------------------ */
	INT SkillStudy::GetStudyState( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[2] param parameter error");
		}
		INT nSkillID = args[2].GetInteger();

		DBC_DEFINEHANDLE(s_pStudyDBC, DBC_SKILL_STUDY_DATA);
		const _DBC_SKILL_STUDY_DATA* pStudyData = (const _DBC_SKILL_STUDY_DATA*)s_pStudyDBC->Search_Index_EQU(nSkillID);

		if (NULL == pStudyData)
		{
			state->PushInteger(0);
			return 1;
		}
		

		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nSkillID);

		// 要学习的技能级别
		INT nSkillLevel = 0;
		const SCLIENT_SKILL* pSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(nSkillID);
		if (pSkill)
		{
			// 拥有该技能
			nSkillLevel = pSkill->m_nLevel;
		}
		++nSkillLevel;
		


		// 职业 条件
		{
			INT nMenPai = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MenPai();
			if (-1 != pSkillData->m_nMenPai)
			{
				if (nMenPai != pStudyData->m_nVocationQua)
				{
					state->PushInteger(0);
					return 1;
				}
			}
			
		}

		// 技能已经到达最大级别
		/*{
			if (nSkillLevel >= pSkillData->m_nMaxLevel)
			{
				state->PushInteger(0);
				return 1;
			}
		}*/

		
		// 拥有的技能点数 条件
		{
			INT skillPoint  = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentSkillPoint();	// 玩家当前拥有的技能点数	
			INT	nPointQua   = GetSkillData( pStudyData->m_szSkillPointQua, nSkillLevel);	// 解析表中的技能需求点数
			if (skillPoint < nPointQua)
			{
				state->PushInteger(0);
				return 1;
			}
		}
		

		// 该系别点数 条件
		{
			 // 从表中获取该技能的系别
			INT nSeriesPoint = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_SeriesPoint(pSkillData->m_nMenPai);
			INT	nSeriesPointQua   = GetSkillData( pStudyData->m_szSeriesSkillPointQua, nSkillLevel);	// 解析表中的技能需求点数
			if (nSeriesPoint < nSeriesPointQua)
			{
				state->PushInteger(0);
				return 1;
			}
		}
		

		// 金钱 条件
		{
			INT nMoney = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money();

			INT	nMoneyQua = GetSkillData( pStudyData->m_szMoneyQua, nSkillLevel );
			if (nMoney < nMoneyQua)
			{
				state->PushInteger(0);
				return 1;
			}
		}
		

		// 经验 条件
		{
			//if (1)
			//{
			//}
		}
		

		// 父技能 条件
		{
			const SCLIENT_SKILL* pFatherSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill( pStudyData->m_nFatherSkillIDQua );
			// 父技能拥有
			if (pFatherSkill)
			{
				// 父技能级别 条件
				INT nFatherSkillLevelQua = GetSkillData( pStudyData->m_szFatherSkillLevelQua, nSkillLevel );
				if (pFatherSkill->m_nLevel < nFatherSkillLevelQua)
				{
					state->PushInteger(0);
					return 1;
				}
			}
		}

		state->PushInteger(1);
		return 1;
	}


	/* ------------------------------------------------
	// 得到 该技能级别
	   参数1	INT		技能ID
	------------------------------------------------ */
	INT SkillStudy::GetSkillLevel( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[2] param parameter error");
		}
		INT nSkillID = args[2].GetInteger();

		const SCLIENT_SKILL* pSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(nSkillID);
		if (pSkill)
		{
			state->PushInteger(pSkill->m_nLevel);
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}


	/* ------------------------------------------------
	// 得到 该技能最大级别
	参数1	INT		技能ID
	------------------------------------------------ */
	INT SkillStudy::GetSkillMaxLevel(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[2] param parameter error");
		}
		INT nSkillID = args[2].GetInteger();

		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nSkillID);
		if (pSkillData)
		{
			state->PushInteger(pSkillData->m_nMaxLevel);
			return 1;
		}

		state->PushInteger(0);
		return 1;
	}

	/* ------------------------------------------------
	// 得到 已经分配到某一个系列的技能点数
	   参数1	INT		系别ID (取值0,1,2)
	------------------------------------------------ */
	INT SkillStudy::GetSeriesPoint( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[2] param parameter error");
		}
		INT nSeriesID = args[2].GetInteger();

		INT s = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_SeriesPoint( nSeriesID );
		state->PushInteger(s);
		return 1;
		;		
	}

	/* ------------------------------------------------
	// 得到 技能说明
	   参数1	INT		技能ID
	------------------------------------------------ */
	INT SkillStudy::GetSkillExplain( LuaPlus::LuaState* state )
	{
		return 0;
	}


	/* ------------------------------------------------
	// 得到 父技能ID
	参数1	INT		技能ID
	------------------------------------------------ */
	INT SkillStudy::GetFatherSkillID( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[2] param parameter error");
		}
		INT nSkillID = args[2].GetInteger();

		DBC_DEFINEHANDLE(s_pStudyDBC, DBC_SKILL_STUDY_DATA);
		const _DBC_SKILL_STUDY_DATA* pStudyData = (const _DBC_SKILL_STUDY_DATA*)s_pStudyDBC->Search_Index_EQU(nSkillID);
		if (pStudyData)
		{
			state->PushInteger(pStudyData->m_nFatherSkillIDQua);
			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}

	/* ------------------------------------------------
	// 得到 剩余技能点数ID
	------------------------------------------------ */
	INT SkillStudy::GetSkillPoint(LuaPlus::LuaState* state)
	{
		INT nPoint = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentSkillPoint();
		
		state->PushInteger(nPoint);
		return 1;

	}

	/* ------------------------------------------------
	// 递归技能树算法, 找到根节点位置
	------------------------------------------------ */
	BOOL SkillStudy::ClimbStudyTree(INT nSkillID, INT nSkillLevel)
	{
		DBC_DEFINEHANDLE(s_pStudyDBC, DBC_SKILL_STUDY_DATA);
		const _DBC_SKILL_STUDY_DATA* pStudyData = (const _DBC_SKILL_STUDY_DATA*)s_pStudyDBC->Search_Index_EQU(nSkillID);
		if (NULL == pStudyData)
		{
			return 0;
		}

		// 是否是根技能 (nFatherSkillIDQua小于0时为根技能)
		if (0 <= pStudyData->m_nFatherSkillIDQua)
		{			
			const SCLIENT_SKILL* pFatherSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill( pStudyData->m_nFatherSkillIDQua );
			
			// 角色拥有该父技能
			if (pFatherSkill)
			{
				// 父技能级别 条件
				INT nFatherSkillLevelQua = GetSkillData( pStudyData->m_szFatherSkillLevelQua, nSkillLevel);
				if (pFatherSkill->m_nLevel >= nFatherSkillLevelQua)
				{
					return ClimbStudyTree(pStudyData->m_nFatherSkillIDQua, nFatherSkillLevelQua);
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}

		return 1;
	}

	/* ------------------------------------------------
	// 解析技能数据项
	------------------------------------------------ */
	INT SkillStudy::GetSkillData( const CHAR* szSkillData, INT nLevel )
	{
		CHAR szOutData[32] = {0};
		BOOL bOk = CDataBase::GetSplitData(szSkillData, nLevel, szOutData);
		if (bOk)
		{
			return atoi(szOutData);
		}
		else if ( nLevel > 1)
		{
			bOk = CDataBase::GetSplitData(szSkillData, 1, szOutData);
			if (bOk)
			{
				return atoi(szOutData);
			}
		}

		return 0;
	}


	// 20100421 AddCodeBegin
	// 得到技能类型ENUM_SKILL_TYPE
	INT SkillStudy::GetSkillType(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[2] param parameter error");
		}

		INT nSkillID = args[2].GetInteger();

		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nSkillID);
		if (pSkillData)
		{
			state->PushNumber( pSkillData->m_nType );
			return 1;
		}

		state->PushNumber( -1 );
		return 1;
	}
	// 20100421 AddCodeEnd

	// 20100421 AddCodeBegin
	// 得到技能名称
	INT SkillStudy::GetSkillName(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[2] param parameter error");
		}

		INT nSkillID = args[2].GetInteger();

		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nSkillID);
		if (pSkillData)
		{
			if ( pSkillData->m_lpszName != NULL )
			{
				state->PushString( pSkillData->m_lpszName );
				return 1;
			}		
		}

		state->PushString("");
		return 1;
	}
	// 20100421 AddCodeEnd


	// 20100421 AddCodeBegin
	// 得到技能主/被动标记
	INT SkillStudy::GetSkillPassiveFlag(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			KLThrow("PlayerShop::EnumItem[2] param parameter error");
		}

		INT nSkillID = args[2].GetInteger();

		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nSkillID);
		if (pSkillData)
		{
			state->PushNumber( pSkillData->m_nPassiveFlag );
			return 1;
		}

		state->PushNumber( 1 );
		return 1;	
	}
	// 20100421 AddCodeEnd


	//向服务器请求是否可以学习技能
	INT SkillStudy::RequestIsStudySkill( LuaPlus::LuaState* state )
	{
		LuaStack args( state );

		if ( !(args[2].IsInteger()) )
		{
			KLThrow("RequestIsStudySkill::EnumItem[2] param parameter error");
		}

		INT nSkillID = args[2].GetInteger();

		//向服务器请求

		return 0;
	}
	
}














