
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
	// ѧϰһ���¼���
	����1	INT		����ID
	���� 0 -- ���ɹ�, 1 -- �ɹ�
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

		// Ҫѧϰ�ļ��ܼ���
		INT nSkillLevel = 0;
		const SCLIENT_SKILL* pSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(nSkillID);
		if (pSkill)
		{
			nSkillLevel = pSkill->m_nLevel;
		}
		++nSkillLevel;


		// �Ƿ��Ѿ�����󼶱�
		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nSkillID);
		if (pSkillData && nSkillLevel > pSkillData->m_nMaxLevel)
		{
			state->PushInteger(0);
			return 1;
		}

		
		// �ж�ѧϰ����������������
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
	// �õ� �ü��ܵĿ�ѧϰ״̬
	����1	INT		����ID	
	���� 0 -- ����ѧ, 1 -- ��ѧ
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

		// Ҫѧϰ�ļ��ܼ���
		INT nSkillLevel = 0;
		const SCLIENT_SKILL* pSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(nSkillID);
		if (pSkill)
		{
			// ӵ�иü���
			nSkillLevel = pSkill->m_nLevel;
		}
		++nSkillLevel;
		


		// ְҵ ����
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

		// �����Ѿ�������󼶱�
		/*{
			if (nSkillLevel >= pSkillData->m_nMaxLevel)
			{
				state->PushInteger(0);
				return 1;
			}
		}*/

		
		// ӵ�еļ��ܵ��� ����
		{
			INT skillPoint  = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentSkillPoint();	// ��ҵ�ǰӵ�еļ��ܵ���	
			INT	nPointQua   = GetSkillData( pStudyData->m_szSkillPointQua, nSkillLevel);	// �������еļ����������
			if (skillPoint < nPointQua)
			{
				state->PushInteger(0);
				return 1;
			}
		}
		

		// ��ϵ����� ����
		{
			 // �ӱ��л�ȡ�ü��ܵ�ϵ��
			INT nSeriesPoint = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_SeriesPoint(pSkillData->m_nMenPai);
			INT	nSeriesPointQua   = GetSkillData( pStudyData->m_szSeriesSkillPointQua, nSkillLevel);	// �������еļ����������
			if (nSeriesPoint < nSeriesPointQua)
			{
				state->PushInteger(0);
				return 1;
			}
		}
		

		// ��Ǯ ����
		{
			INT nMoney = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money();

			INT	nMoneyQua = GetSkillData( pStudyData->m_szMoneyQua, nSkillLevel );
			if (nMoney < nMoneyQua)
			{
				state->PushInteger(0);
				return 1;
			}
		}
		

		// ���� ����
		{
			//if (1)
			//{
			//}
		}
		

		// ������ ����
		{
			const SCLIENT_SKILL* pFatherSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill( pStudyData->m_nFatherSkillIDQua );
			// ������ӵ��
			if (pFatherSkill)
			{
				// �����ܼ��� ����
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
	// �õ� �ü��ܼ���
	   ����1	INT		����ID
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
	// �õ� �ü�����󼶱�
	����1	INT		����ID
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
	// �õ� �Ѿ����䵽ĳһ��ϵ�еļ��ܵ���
	   ����1	INT		ϵ��ID (ȡֵ0,1,2)
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
	// �õ� ����˵��
	   ����1	INT		����ID
	------------------------------------------------ */
	INT SkillStudy::GetSkillExplain( LuaPlus::LuaState* state )
	{
		return 0;
	}


	/* ------------------------------------------------
	// �õ� ������ID
	����1	INT		����ID
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
	// �õ� ʣ�༼�ܵ���ID
	------------------------------------------------ */
	INT SkillStudy::GetSkillPoint(LuaPlus::LuaState* state)
	{
		INT nPoint = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentSkillPoint();
		
		state->PushInteger(nPoint);
		return 1;

	}

	/* ------------------------------------------------
	// �ݹ鼼�����㷨, �ҵ����ڵ�λ��
	------------------------------------------------ */
	BOOL SkillStudy::ClimbStudyTree(INT nSkillID, INT nSkillLevel)
	{
		DBC_DEFINEHANDLE(s_pStudyDBC, DBC_SKILL_STUDY_DATA);
		const _DBC_SKILL_STUDY_DATA* pStudyData = (const _DBC_SKILL_STUDY_DATA*)s_pStudyDBC->Search_Index_EQU(nSkillID);
		if (NULL == pStudyData)
		{
			return 0;
		}

		// �Ƿ��Ǹ����� (nFatherSkillIDQuaС��0ʱΪ������)
		if (0 <= pStudyData->m_nFatherSkillIDQua)
		{			
			const SCLIENT_SKILL* pFatherSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill( pStudyData->m_nFatherSkillIDQua );
			
			// ��ɫӵ�иø�����
			if (pFatherSkill)
			{
				// �����ܼ��� ����
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
	// ��������������
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
	// �õ���������ENUM_SKILL_TYPE
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
	// �õ���������
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
	// �õ�������/�������
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


	//������������Ƿ����ѧϰ����
	INT SkillStudy::RequestIsStudySkill( LuaPlus::LuaState* state )
	{
		LuaStack args( state );

		if ( !(args[2].IsInteger()) )
		{
			KLThrow("RequestIsStudySkill::EnumItem[2] param parameter error");
		}

		INT nSkillID = args[2].GetInteger();

		//�����������

		return 0;
	}
	
}














