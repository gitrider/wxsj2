
/**	GMGameInterface_Script_playerMySelf.cpp
 *	
 *	���ܣ�	
 *
 *	�޸ļ�¼��	
 *			080402	UpdateTitleState
 *			
 *			
 */

#include "stdafx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "Procedure/GamePro_Login.h"
#include "NetWork/NetManager.h"
#include "LuaPlus.h"
#include "Global.h"
#include "Event/GMEventSystem.h"
#include "GIException.h"
#include "Action/GMActionSystem.h"
#include "DataPool/GMUIDataPool.h"
#include "DataPool/GMDataPool.h"
#include "Object/Manager/ObjectManager.h"
#include "Object/Logic/Object.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Input/GMInputSystem.h"
#include "world/worldmanager.h"
#include "GIDbc_struct.h"
#include "object/Logic/character/obj_playermyself.h"
#include "GIUtil.h"
#include "ScriptCommandStruct.h"
#include "CGEventRequest.h"
#include "CGTeamInvite.h"								// ������������Ϣ				
#include "CGTeamRetInvite.h"							// ������������Ϣ����
#include "CGTeamRetApply.h"								// �ش�ĳ��������������Ϣ
#include "CGTeamLeaderRetInvite.h"						// �ش��Ա������������Ϣ
#include "CGReqManualAttr.h"							// �����ֶ���ӵ���.
#include "CGAskTeamFollow.h"
#include "CGReturnTeamFollow.h"
#include "CGStopTeamFollow.h"

#include "CGPlayerDieResult.h"
#include "..\..\Common\database\WXSJ_DBC.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "GIDBC_Struct.h"
#include "DataPool\GMDP_CharacterData.h"
#include "CGAskLeanAbility.h"
#include "DBC\GMDataBase.h"
#include "GMInterface_Script_Talk.h"
#include "CGCharUpdateCurTitle.h"
#include "CGMinorPasswd.h"
#include "CGBankAcquireList.h"
#include "..\..\Common\Packets\CGSetTeamDistribMode.h"



namespace SCRIPT_SANDBOX
{
	
	//===============================================================
	PlayerMySelf PlayerMySelf::s_MySelf;


	// �õ���ҵ�����
	INT	PlayerMySelf::GetName(LuaPlus::LuaState* state)
	{
		state->PushString( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		return 1;
	}

	// �õ���ɫ���ϵ�buff������
	INT PlayerMySelf::GetBuffNumber( LuaPlus::LuaState* state )
	{
		CDataPool *pDataPool = CDataPool::GetMe();

		state->PushInteger(pDataPool->BuffImpact_GetCount());

		return 1;
	}

	INT PlayerMySelf::GetBuffIconNameByIndex( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		const _BUFF_IMPACT_INFO *pBuffImpactInfo = pDataPool->BuffImpact_GetByIndex( nIndex );

		// 20100426 ModifyCodeBegin ��Ϊ����Ҳ���ͼ�꣬����ʾΪ�գ���Ҫ��assert�жϳ�����
		if ( pBuffImpactInfo != NULL )
		{
			_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( pBuffImpactInfo->m_nBuffID );
			if ( pBuffImpact != NULL )
			{
				if ( strlen( pBuffImpact->m_szIconName ) > 0 )
				{
					state->PushString(pBuffImpact->m_szIconName);
					return 1;
				}
				else
				{
					//Assert( strlen( pBuffImpact->m_szIconName ) > 0 && "PlayerMySelf::GetBuffIconNameByIndex" );
				}
			}
			else
			{
				//Assert( pBuffImpact != NULL > 0 && "PlayerMySelf::GetBuffIconNameByIndex" );
			}
		}
		else
		{
			//Assert( pBuffImpactInfo != NULL > 0 && "PlayerMySelf::GetBuffIconNameByIndex" );
		}
		// 20100426 ModifyCodeEnd

		state->PushString("");

		return 1;
	}

	INT PlayerMySelf::GetBuffToolTipsByIndex( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffToolTipsByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		const _BUFF_IMPACT_INFO *pBuffImpactInfo = pDataPool->BuffImpact_GetByIndex( nIndex );	

		if ( pBuffImpactInfo != NULL )
		{
			// 20100429 ModifyCodeBegin ���� ����playerimpactframe��buff��������1-10����ȡbuff����tip��һЩ����
			_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( pBuffImpactInfo->m_nBuffID );

			if ( pBuffImpact != NULL )
			{	
				if (  pBuffImpact->m_pszInfo != "" )
				{
					state->PushString( pBuffImpact->m_pszInfo );
					return 1;	
				}					
			}		

		}//if ( pBuffImpactInfo != NULL )
		// 20100429 ModifyCodeEnd

		state->PushString("");
		return 1;
	}

	INT PlayerMySelf::DispelBuffByIndex( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:DispelBuffByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		pDataPool->BuffImpact_DispelByIndex( nIndex );
		return 1;
	}
	INT PlayerMySelf::GetBuffTimeTextByIndex( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:GetBuffTimeTextByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		INT nTime = pDataPool->BuffImpact_GetTimeByIndex( nIndex );
		state->PushInteger(nTime/1000);
		return 1;
	}

	INT PlayerMySelf::SendReliveMessage_OutGhost( LuaPlus::LuaState* state )
	{
		CGPlayerDieResult msgDieResult;
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:SendReliveMessage_OutGhost param parameter error");
		}
		msgDieResult.SetResultCode( args[2].IsInteger() );
		CNetManager::GetMe()->SendPacket(&msgDieResult);

		return 1;
	}

	INT PlayerMySelf::SendReliveMessage_Relive( LuaPlus::LuaState* state )
	{
		//��ɫ�����󸴻ť��Ӧ�߼�
		LuaStack args(state);
		CGPlayerDieResult msgDieResult;

		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:SendReliveMessage_Relive param parameter error");
		}
		msgDieResult.SetResultCode( args[2].GetInteger() );
		CNetManager::GetMe()->SendPacket(&msgDieResult);

		return 1;
	}

	INT PlayerMySelf::GetData( LuaPlus::LuaState* state )
	{
		LuaStack args(state);
		if( !args[ 2 ].IsString() ) return 0;

		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		std::string  str = args[ 2 ].GetString();
		if( str == "LEVEL" )	//�ȼ�
		{
			state->PushInteger(pCharData->Get_Level());
			return 1;
		}
		else if( str == "HP" )					//����ֵ
		{
			state->PushInteger(pCharData->Get_HP());
			return 1;
		}
		else if( str == "NAME" )				//
		{
			state->PushString(pCharData->Get_Name());
			return 1;
		}
		else if( str == "PROFESSION" )			//
		{
			state->PushInteger(pCharData->Get_MenPai());
			return 1;
		}

		else if( str == "MP" )					//ħ��ֵ
		{
			state->PushInteger(pCharData->Get_MP());
			return 1;
		}
		else if( str == "PNEUMA" )					//Ԫ��ֵ
		{
			state->PushInteger(pCharData->Get_Penuma());
			return 1;
		}
		else if( str == "EXP" )					//����
		{
			if(pCharData->Get_Exp() < 0)
			{
				state->PushInteger(0);
			}
			else
			{
				state->PushInteger(pCharData->Get_Exp());
			}
			return 1;
		}
		else if( str == "NEEDEXP" )				//��������
		{
			state->PushInteger(pCharData->Get_MaxExp());
			return 1;
		}
		else if( str == "STRIKEPOINT" )				//������
		{
			state->PushInteger(pCharData->Get_StrikePoint());
			return 1;
		}
		else if( str == "MONEY" )				//����
		{
			state->PushInteger(pCharData->Get_Money());

			//ͬʱ���ת��Ϊ������ͭ�ҵ���ֵ
			INT nMoney = pCharData->Get_Money();

			INT nGoldCoin;	
			INT nSilverCoin;
			INT nCopperCoin;

			nCopperCoin = nMoney % 100;

			if( nMoney >= 100 )
			{
				nSilverCoin = ((nMoney-nCopperCoin)/100) % 100;
			}
			else
			{
				nSilverCoin = 0;
			}

			if ( nMoney >= 10000 )
			{
				nGoldCoin = (((nMoney-nCopperCoin)/100)-nSilverCoin)/100;
			}
			else
				nGoldCoin = 0;

			state->PushInteger(nGoldCoin);
			state->PushInteger(nSilverCoin);
			state->PushInteger(nCopperCoin);
			return 4;
		}
		else if( str == "STR" )					//���� ����
		{
			state->PushInteger(pCharData->Get_STR());
			return 1;
		}
//		else if( str == "SPR" )					//���� ����
//		{
//			state->PushInteger(pCharData->Get_SPR());
//			return 1;
//		}
		else if( str == "CON" )					//���� ����
		{
			state->PushInteger(pCharData->Get_CON());
			return 1;
		}
		else if( str == "INT" )					//���� ����
		{
			state->PushInteger(pCharData->Get_INT());
			return 1;
		}								   
		else if( str == "DEX" )					//�� ����
		{
			state->PushInteger(pCharData->Get_DEX());
			return 1;
		}								   
		else if( str == "POINT_REMAIN" )			//ʣ�����
		{
			state->PushInteger(pCharData->Get_PointRemain());
			return 1;
		}	

//		else if( str == "ATT_PHYSICS" )			//��������
//		{
//			state->PushInteger(pCharData->Get_AttPhysics());
//			return 1;
//		}								   
//		else if( str == "DEF_PHYSICS" )			//���������
//		{
//			state->PushInteger(pCharData->Get_DefPhysics());
//			return 1;
//		}

		else if( str == "ATT_NEAR" )			//��������
		{
			state->PushInteger(pCharData->Get_AttNear());
			return 1;
		}
		else if( str == "DEF_NEAR" )			//��
		{
			state->PushInteger(pCharData->Get_DefNear());
			return 1;
		}

		else if( str == "ATT_FAR" )			//Զ������
		{
			state->PushInteger(pCharData->Get_AttFar());
			return 1;
		}
		else if( str == "DEF_FAR" )			//Զ
		{
			state->PushInteger(pCharData->Get_DefFar());
			return 1;
		}

		else if( str == "ATT_MAGIC_NEAR" )			//ħ��������
		{
			state->PushInteger(pCharData->Get_AttMagic_Near());
			return 1;
		}								   
		else if( str == "DEF_MAGIC_NEAR" )			//ħ��������
		{
			state->PushInteger(pCharData->Get_DefMagic_Near());
			return 1;
		}
		else if( str == "ATT_MAGIC_FAR" )			//ħ��������
		{
			state->PushInteger(pCharData->Get_AttMagic_Far());
			return 1;
		}								   
		else if( str == "DEF_MAGIC_FAR" )			//ħ��������
		{
			state->PushInteger(pCharData->Get_DefMagic_Far());
			return 1;
		}
		else if( str == "MAXHP" )				//�������ֵ
		{
			state->PushInteger(pCharData->Get_MaxHP());
			return 1;
		}								   
		else if( str == "MAXMP" )				//���ħ��ֵ
		{			
			state->PushInteger(pCharData->Get_MaxMP());
			return 1;
		}
		else if( str == "MAXPNEUMA" )				//���Ԫ��ֵ
		{			
			state->PushInteger(pCharData->Get_MaxPneuma());
			return 1;
		}	
		else if( str == "RAGE" )				//ŭ��
		{			
			state->PushInteger(pCharData->Get_Rage());
			return 1;
		}								   
		else if( str == "MAXRAGE" )				//���ŭ��
		{			
			state->PushInteger(pCharData->Get_MaxRage());
			return 1;
		}								   
		else if( str == "HP_RESPEED" )			//HP�ָ��ٶ�  ��/��
		{			
			state->PushInteger(pCharData->Get_HPRespeed());
			return 1;
		}								   
		else if( str == "MP_RESPEED" )			//MP�ָ��ٶ�  ��/��
		{
			state->PushInteger(pCharData->Get_MPRespeed());
			return 1;
		}
		else if( str == "PNEUMA_RESPEED" )			//Ԫ���ָ��ٶ�  ��/��
		{
			state->PushInteger(pCharData->Get_PenumaPercent());
			return 1;
		}	
		else if( str == "HIT" )					//������
		{
			state->PushInteger(pCharData->Get_Hit());
			return 1;
		}								   
		else if( str == "MISS" )					//������
		{
			state->PushInteger(pCharData->Get_Miss());
			return 1;
		}								   
		else if( str == "CRITRATE" )				//������
		{
			state->PushInteger(pCharData->Get_CritRate());
			return 1;								   
		}	
		else if(str == "TOUGHNESS")                // ����
		{
			state->PushInteger(pCharData->Get_Toughness());
			return 1;	
		}
		else if( str == "MOVESPEED" )			//�ƶ��ٶ�
		{
			state->PushNumber(pCharData->Get_MoveSpeed());
			return 1;								   
		}								   
		else if( str == "ATTACKSPEED" )			//�����ٶ�
		{
			state->PushInteger(pCharData->Get_AttackSpeed());
			return 1;
		}
		//�𹥻�
		else if( str == "ATTACKGOLD" )			
		{
			state->PushInteger(pCharData->Get_AttGold());
			return 1;								   
		}
		//�����
		else if( str == "DEFENCEGOLD" )			
		{
			state->PushInteger(pCharData->Get_DefGold());
			return 1;								   
		}
		//�𹥻�
		else if( str == "ATTACKFIRE" )			
		{
			state->PushInteger(pCharData->Get_AttFire());
			return 1;								   
		}
		//�����
		else if( str == "DEFENCEFIRE" )			
		{
			state->PushInteger(pCharData->Get_DefFire());
			return 1;								   
		}
		//ˮ����
		else if( str == "ATTACKWATER" )			
		{
			state->PushInteger(pCharData->Get_AttWater());
			return 1;								   
		}
		//ˮ����
		else if( str == "DEFENCEWATER" )			
		{
			state->PushInteger(pCharData->Get_DefWater());
			return 1;								   
		}
		//ľ����
		else if( str == "ATTACKWOOD" )
		{
			state->PushInteger(pCharData->Get_AttWood());
			return 1;								   
		}
		// ľ����
		else if( str == "DEFENCEWOOD" )
		{
			state->PushInteger(pCharData->Get_DefWood());
			return 1;								   
		}
		//������
		else if( str == "ATTACKSOIL" )
		{
			state->PushInteger(pCharData->Get_AttSoil());
			return 1;								   
		}
		// ������
		else if( str == "DEFENCESOIL" )
		{
			state->PushInteger(pCharData->Get_DefSoil());
			return 1;								   
		}
		// ��Ӫ
		else if( str == "CAMP" )					
		{
			// �ӿڱ䶯
			Assert( FALSE && "str == \"CAMP\"" );
			state->PushInteger(-1);
			return 1;
		}
		else if( str == "VIGOR")
		{
			state->PushInteger(pCharData->Get_Vigor());
			return 1;
		}
		else if( str == "MAXVIGOR")	
		{
			state->PushInteger(pCharData->Get_MaxVigor());
			return 1;
		}
		// ���
		else if( str == "GUILD")		
		{
			state->PushInteger(pCharData->Get_Guild());
		}
		else if( str == "ENERGY")
		{
			state->PushInteger(pCharData->Get_Energy());
			return 1;
		}
		else if( str == "MAXENERGY")
		{
			state->PushInteger(pCharData->Get_MaxEnergy());
			return 1;
		}
		else if( str == "PORTRAIT" )			//����ͷ��
		{
			
			// �õ���Һ��Լ���ͷ����Ϣ
			LPCSTR pImageName = NULL;
			DBC_DEFINEHANDLE(s_pFaceImageDBC, DBC_CHARACTER_FACE)	// char_face.tab
			//�����ﶨ���
			const _DBC_CHAR_FACE* pFaceImage = NULL;
			pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_Index_EQU(pCharData->Get_PortraitID()));

			if( pFaceImage == NULL )
			{
				int nRaceID = pCharData->Get_RaceID();

				nRaceID = nRaceID % 2;

				pFaceImage = (const _DBC_CHAR_FACE*)( s_pFaceImageDBC->Search_First_Column_Equ( 2,  nRaceID ) );

			}	

			if(pFaceImage)
			{
				pImageName = pFaceImage->pImageSetName;
			}
			//else // Ĭ��״̬����ʾͷ��ͼ�꣬ ͷ��ͼ����ʾ��
			//{
			//	pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_LineNum_EQU(0));
			//	pImageName = pFaceImage->pImageSetName;
			//}//

		
					
			if(pImageName)
			{
				state->PushString(pImageName);
			}
			else
			{
				state->PushString(_T(""));
			}

			return 1;
		}

		state->PushInteger(-1);
		return 1;
	}
	
	// �õ��������� Nick 2007.11.23
	INT PlayerMySelf::GetSkillNum( LuaPlus::LuaState* state )
	{
		state->PushInteger( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_SkillNum() );
		return 1;
	}
	
	// �������õ���ɫ��ĳ��ս�����ܵ���Ϣ Nick 2007.11.23
	INT PlayerMySelf::GetIndexSkillInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;
		if( !args[3].IsString() ) return 0;

		INT	nIndexID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		SSKILL_MAP map( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill() );
		SSKILL_MAP::iterator iter = map.begin();
		
		for( int i = 0; i < nIndexID; ++i )
			++iter;
		
		SCLIENT_SKILL* pSkill = &(iter->second);

		if(str == "name")			// ����
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszName);
			else
				state->PushString("");
		}
		else if(str == "learn")		// �Ƿ�ѧ��
		{
			if(pSkill)
				state->PushBoolean(pSkill->m_bLeaned == TRUE);
			else
				state->PushBoolean(false);
		}
		else if(str == "iconname")	// ͼ���ļ��� 
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszIconName);
			else
				state->PushString("");
		}
		else if(str == "passivity")	// �Ƿ��Ǳ�������
		{	
			if(pSkill)
				state->PushInteger(pSkill->m_pDefine->m_nPassiveFlag);
		}
		else if(str == "explain")	// ��ϸ����
		{	
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszSkillExplainForInterface);
			else
				state->PushString("");
		}
		else if(str == "skilldata")	// ��������
		{	
			if(pSkill)
				state->PushString(pSkill->GetSkillDesc_Interface());
			else
				state->PushString("");
		}
		else if(str == "skilllevel")	// ���ܵȼ� Nick 2007.11.23
		{	
			if(pSkill)
				state->PushInteger(-1);
		}

		return 1;
	}

	// �����ҵ�ĳ�����Ϣ
	INT PlayerMySelf::GetSkillInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;

		INT	nSkillID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		const SCLIENT_SKILL* pSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(nSkillID);

		if(str == "name")			// ����
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszName);
			else
				state->PushString("");
		}
		else if(str == "learn")		// �Ƿ�ѧ��
		{
			if(pSkill)
				state->PushBoolean(pSkill->m_bLeaned == TRUE);
			else
				state->PushBoolean(false);
		}
		else if(str == "iconname")	// ͼ���ļ���
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszIconName);
			else
				state->PushString("");
		}
		else if(str == "passivity")	// �Ƿ��Ǳ�������
		{	
			if(pSkill)
				state->PushInteger(pSkill->m_pDefine->m_nPassiveFlag);
		}
		else if(str == "explain")	// ��ϸ����
		{	
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszSkillExplainForInterface);
			else
				state->PushString("");
		}
		else if(str == "skilldata")	// ��������
		{	
			if(pSkill)
				state->PushString(pSkill->GetSkillDesc_Interface());
			else
				state->PushString("");
		}
		else if(str == "skilllevel")	// ���ܵȼ� Nick 2007.11.23
		{	
			if(pSkill)
				state->PushInteger(-1);
		}

		return 1;
	}

	//�����ҵ�ĳ���ķ���Ϣ
	INT PlayerMySelf::GetXinfaInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;

		INT	nXinfaID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		//const SCLIENT_XINFA* pXinfa = 
		//	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa(nXinfaID);

		//if(str == "name")			// ����
		//{
		//	state->PushString(pXinfa->m_pDefine->pszName);
		//}
		//else if(str == "level")		// �ȼ�
		//{
		//	state->PushInteger(pXinfa->m_nLevel);
		//}
		//else if(str == "iconname")	// ͼ���ļ���
		//{
		//	state->PushString(pXinfa->m_pDefine->pszIconName);
		//}
		//else if(str == "explain")	// ��ϸ����
		//{	
		//	state->PushString(pXinfa->m_pDefine->pszDesc);
		//}
		return 1;
	}

	//�������Ƿ�ѧ��ĳ�������
	INT PlayerMySelf::GetAbilityStudy( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		INT nAbilityID = args[2].GetInteger();
		
		const SCLIENT_LIFEABILITY* pLifeAbility = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbilityID);

		if( pLifeAbility )
			state->PushInteger(1);
		else
			state->PushInteger(0);

		return 1;

	}

	//�����ҵ�ĳ���������Ϣ
	INT PlayerMySelf::GetAbilityInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;


		INT	nAbilityID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		const SCLIENT_LIFEABILITY* pLifeAbility = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbilityID);

		if( !pLifeAbility ) return 0;

		if(str == "name")			// ����
		{
			state->PushString(pLifeAbility->m_pDefine->szName);
		}
		else if(str == "level")		// �ȼ�
		{
			state->PushInteger(pLifeAbility->m_nLevel);
		}
		else if(str == "maxlevel")	// �ȼ�����
		{
			state->PushInteger(pLifeAbility->m_pDefine->nLevelMax);
		}
		else if(str == "skillexp")	// ������
		{
			state->PushInteger(pLifeAbility->m_nExp);
		}
		else if(str == "iconname")	// ͼ���ļ���
		{
			state->PushString(pLifeAbility->m_pDefine->szIconName);
		}
		else if(str == "explain")	// ��ϸ����
		{	
			state->PushString(pLifeAbility->m_pDefine->szExplain);
		}
		else if(str == "popup")	// �Ƿ񵯳��¼�����
		{	// 1.�����ϳɽ��棬2.������Ƕ���棬-1.��������
			state->PushInteger(pLifeAbility->m_pDefine->nPopup);
		}
		return 1;
	}

	// ��ɫ����ѧϰ�����
	INT PlayerMySelf::AskLeanAbility(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:PlayerMySelf param1 parameter error");
		}
		//if ( !(args[3].IsInteger()) )
		//{
		//	KLThrow("LUA:PlayerMySelf param2 parameter error");
		//}

		INT nAbilityID = args[2].GetInteger();
		//INT nNpcID = args[3].GetInteger();
		INT nNpcID = CUIDataPool::GetMe()->GetCurDialogNpcId();
  
		//// ������������
		//const SCLIENT_LIFEABILITY* pLifeAbility = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbilityID);

		//if(!pLifeAbility) 
		//	return 0;

		//// �ñ����������ж��Ƿ�������������
		//CObject_PlayerMySelf *pMyself = CObjectManager::GetMe()->GetMySelf();
		//if(!pMyself) return 0;
		//
		//CCharacterData* pData = pMyself->GetCharacterData();
		//if(!pData) return 0;

		//if( pData->Get_Exp() < CDataPool::GetMe()->StudyAbility_GetNeedExp() )
		//{
		//	STRING strTemp = "";
		//	strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_Exp_Not_Enough");
		//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����ֵ����");
		//	return 0;
		//}
		//if( pData->Get_Money() < CDataPool::GetMe()->StudyAbility_GetNeedMoney() )
		//{
		//	STRING strTemp = "";
		//	strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_Money_Not_Enough");
		//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "��Ǯ����" );
		//	return 0;
		//}
		//if( pLifeAbility->m_nExp < CDataPool::GetMe()->StudyAbility_GetSkillExp() )
		//{
		//	STRING strTemp = "";
		//	strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_Skill_Not_Enough");
		//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���������Ȳ���" );
		//	return 0;
		//}
		////����ȼ��Ƿ�����
		//if( CDataPool::GetMe()->StudyAbility_GetLevelLimit() > pData->Get_Level() )
		//{
		//	STRING strTemp = "";
		//	strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_Level_Not_Enough");
		//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "�ȼ�����" );
		//	return 0;
		//}

		//�������������������
		CGAskLeanAbility Msg;
		Msg.SetAbilityID(nAbilityID);
		Msg.SetNpcId(nNpcID);
		//Msg.SetAbilityTeacherInfo(nAbilityID);
		//Msg.SetNeedMoney(CDataPool::GetMe()->StudyAbility_GetNeedMoney());
		//Msg.SetNeedExpy(CDataPool::GetMe()->StudyAbility_GetNeedExp());
		//Msg.SetNeedSkillExpy(CDataPool::GetMe()->StudyAbility_GetSkillExp());

		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}


	INT PlayerMySelf::GetPos( LuaPlus::LuaState* state)
	{
		
		CObject_PlayerMySelf* pCharObj = CObjectManager::GetMe()->GetMySelf();
		if( pCharObj )
		{
			const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
			FLOAT x = pCharObj->GetPosition().x * 512 / pSceneDef->nXSize;
			FLOAT y = pCharObj->GetPosition().z * 512 / pSceneDef->nZSize;
			state->PushNumber( x );
			state->PushNumber( y );
			state->PushInteger( INT( pCharObj->GetFaceDir() * 180.f / KLU_PI ) );
		}
		else
		{
			state->PushNumber( 0 );
			state->PushNumber( 0 );
			state->PushInteger( 0 );
		}
		return 3;
	}

	INT PlayerMySelf::IsPresent(LuaPlus::LuaState* state)
	{
		if( CObjectManager::GetMe()->GetMySelf() )
		{
			state->PushBoolean(true);
		}
		else
		{
			state->PushBoolean(FALSE);
		}
		return 1;
	}
	
	//�ƺ����
	INT PlayerMySelf::GetAgnameNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: GetAgnameNum Wrong Param1");
		}

		INT nType = args[2].GetInteger();

		if( nType < _TITLE::GUOJIA_TITLE || nType > _TITLE::WANFA_TITLE )
            state->PushInteger( -1 );
		else
			state->PushInteger( CDataPool::GetMe()->Title_GetTitleNum( nType ) );

		return 1;
	}
	// 
	INT PlayerMySelf::GetAgnameType(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: GetAgnameType Wrong Param1");
		}

		INT nIndex = args[2].GetInteger();
		_TITLE_ title = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Title( nIndex );
		state->PushInteger( title.bType );

		return 1;
	}


	// ��ȡ����ȫ���ĳƺ�����
	INT PlayerMySelf::GetAllAgnameNum(LuaPlus::LuaState* state)
	{
		DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab
		UINT nRecordsNum = pTitleDBC->GetRecordsNum();	// ����
		UINT nFieldsNum  = pTitleDBC->GetFieldsNum();	// ����

		state->PushInteger( nRecordsNum );

		return 1;

	}
	// ���ౣ���������
	INT PlayerMySelf::GetTableAgnameInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: GetAgnameInfo Wrong Param2");
		}
		if( !(args[3].IsString()) )
		{
			KLThrow("LUA: GetAgnameInfo Wrong Param3");
		}

		INT nIndex		= args[2].GetInteger();
		STRING szFilter = args[3].GetString();


		DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab
		UINT nRecordsNum = pTitleDBC->GetRecordsNum();	// ����

		if( nIndex < 0 || nIndex > nRecordsNum )
			return 0;

		CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if( !pCharacterData )
			return 0;

		const _DBC_TITLE* pDbcTitle = (const _DBC_TITLE*)pTitleDBC->Search_LineNum_EQU( nIndex );
		if( pDbcTitle )
		{
			// �ƺ�
			if( szFilter == "title" )
			{
				// Ů
				if( 0 == pCharacterData->Get_RaceID() )
					state->PushString( pDbcTitle->szWomanTitle );
				// ��
				else if( 1 == pCharacterData->Get_RaceID() )
					state->PushString( pDbcTitle->szManTitle );
			}
			// ����
			else if( szFilter == "type" )
			{
				state->PushInteger( pDbcTitle->nType );
			}
			// �ƺ�ʱ��
			else if( szFilter == "tiemer" )
			{
				// �ӷ�������
				state->PushString( "" );
			}
			// �ƺ�����
			if( szFilter == "tip" )
			{
				state->PushString( pDbcTitle->szTitleDesc );
			}
			// ��ʾ��ɫ
			else if( szFilter == "color" )
			{
				state->PushString( pDbcTitle->szColor );
			}
			// Ĭ���Ƿ���ʾ
			else if( szFilter == "show" )
			{
				state->PushInteger( pDbcTitle->bShow );
			}
			// ��ȡ;��
			else if( szFilter == "acquire" )
			{
				state->PushString( pDbcTitle->szAcquireDesc );
			}
			// Ч��ID
			else if( szFilter == "effect" )
			{
				pDbcTitle->nEffectId;

				// �� std_impact.tab ���ж�����

				state->PushString( " effect" );
			}
			// ����ԭ��
			else if( szFilter == "reclaim" )
			{
				state->PushString( pDbcTitle->szReclaimCause );
			}

		}

		return 1;
	}



	INT PlayerMySelf::EnumAgname(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: EnumAgname Wrong Param1");
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: EnumAgname Wrong Param2");
		}
		if( !(args[4].IsString()) )
		{
			KLThrow("LUA: EnumAgname Wrong Param3");
		}

		INT nType			 = args[2].GetInteger();
		INT nIndex			 = args[3].GetInteger();
		const char* szFilter = args[4].GetString();

		if( nType < _TITLE::GUOJIA_TITLE || nType > _TITLE::WANFA_TITLE )
			return 0;
		if( nIndex == -1 )
			return 0;

		CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if( !pCharacterData )
			return 0;

		INT nTitleDataIndex = CDataPool::GetMe()->Title_GetTitle( nType, nIndex );
		_TITLE_ title = pCharacterData->Get_Title( nTitleDataIndex );


		if( _TITLE_::STRING_TITLE == title.bFlag )
		{
/*
			if( title.ID > 0 )
			{
				DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab
				const _DBC_TITLE* pDbcTitle = (const _DBC_TITLE*)pTitleDBC->Search_Index_EQU( title.ID );
				if( pDbcTitle )
				{
					// Ů
					if( 0 == pCharacterData->Get_RaceID() )
						state->PushString( pDbcTitle->szWomanTitle );
					// ��
					else if( 1 == pCharacterData->Get_RaceID() )
						state->PushString( pDbcTitle->szManTitle );
				}
			}
*/

			state->PushString( title.szTitleData );

			return 1;
		}

		return 0;
	}

	// ��ȡ��Ϣ
	INT PlayerMySelf::GetAgnameInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: GetAgnameInfo Wrong Param1");
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: GetAgnameInfo Wrong Param2");
		}
		if( !(args[4].IsString()) )
		{
			KLThrow("LUA: GetAgnameInfo Wrong Param3");
		}

		INT nType		= args[2].GetInteger();
		INT nIndex		= args[3].GetInteger();
		STRING szFilter = args[4].GetString();


		if( nType < _TITLE::GUOJIA_TITLE || nType > _TITLE::WANFA_TITLE )
			return 0;
		if( nIndex == -1 )
			return 0;

		CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if( !pCharacterData )
			return 0;


		// ��������ȡ����
		_TITLE_ title;

		INT nTitleDataIndex = CDataPool::GetMe()->Title_GetTitle( nType, nIndex );
		if( nTitleDataIndex != -1 )
			title = pCharacterData->Get_Title( nTitleDataIndex );


		// �ַ�����
		if( _TITLE_::STRING_TITLE == title.bFlag )
		{
			// �ƺ�
			if( szFilter == "title" )
			{
				state->PushString( title.szTitleData );
			}
			// �ƺ�ʱ��
			else if( szFilter == "tiemer" )
			{
				// ����ʣ��ʱ��
	//			INT tiem = pDbcTitle->nTimeLimit - title.nTime;
				INT tiem = title.nTime;

				// ������
				if( tiem == 0 )
				{
					state->PushString( "������" );
				}
				else
				{
					INT ss = 1000;
					INT mi = ss * 60;
					INT hh = mi * 60;
					INT dd = hh * 60;

					long day = tiem / dd;
					long hour = ( tiem - day * dd ) / hh;
					long minute = ( tiem - day * dd - hour * hh ) / mi;
					long second = ( tiem - day * dd - hour * hh - minute * mi ) / ss;
					long milliSecond = tiem - day * dd - hour * hh - minute * mi - second * ss;


					CHAR szText[_MAX_PATH];
					_snprintf( szText, _MAX_PATH, "%d��%dСʱ%d��%d��", day, hour, minute, second );

					// �ӷ�������
					state->PushString( szText );
				}

			}
			// ������Ҫ�����
			else if( title.ID != -1 )
			{
				// ����ȡ����
				DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab
				const _DBC_TITLE* pDbcTitle = (const _DBC_TITLE*)pTitleDBC->Search_Index_EQU( title.ID );
				if( pDbcTitle )
				{
					// �ƺ�����
					if( szFilter == "tip" )
					{
						state->PushString( pDbcTitle->szTitleDesc );
					}
					// ��ʾ��ɫ
					else if( szFilter == "color" )
					{
						state->PushString( pDbcTitle->szColor );
					}
					// Ĭ���Ƿ���ʾ
					else if( szFilter == "show" )
					{
						state->PushInteger( pDbcTitle->bShow );
					}
					// ��ȡ;��
					else if( szFilter == "acquire" )
					{
						state->PushString( pDbcTitle->szAcquireDesc );
					}
					// Ч��ID
					else if( szFilter == "effect" )
					{
						pDbcTitle->nEffectId;

						// �� std_impact.tab ���ж�����

						state->PushString( " effect" );
					}
					// ����ԭ��
					else if( szFilter == "reclaim" )
					{
						state->PushString( pDbcTitle->szReclaimCause );
					}
				}
			}
			else
			{
			}
		}
		// ˵�����Ƿ������������ģ� ֻ����ʾ�õģ� ���� 
		else
		{
			// �������� �� ������

			// ����ƺŵ�����
			INT titleNum = CDataPool::GetMe()->Title_GetTitleNum( nType );

			DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab
			UINT nRecordsNum = pTitleDBC->GetRecordsNum();	// ����

			_DBC_TITLE* pDbcTitle = NULL;
			for( INT num = 0; num < nRecordsNum; ++num )
			{
				pDbcTitle = (_DBC_TITLE*)pTitleDBC->Search_LineNum_EQU( num );
				if( pDbcTitle && ( pDbcTitle->nType == nType ) )
				{
					if( titleNum == nIndex )
					{
						// �ƺ�
						if( szFilter == "title" )
						{
							// Ů
							if( 0 == pCharacterData->Get_RaceID() )
								state->PushString( pDbcTitle->szWomanTitle );
							// ��
							else if( 1 == pCharacterData->Get_RaceID() )
								state->PushString( pDbcTitle->szManTitle );
						}
						// ����
						else if( szFilter == "type" )
						{
							state->PushInteger( pDbcTitle->nType );
						}
						// �ƺ�ʱ��
						else if( szFilter == "tiemer" )
						{
							// �ӷ�������
							state->PushString( "" );
						}
						// �ƺ�����
						if( szFilter == "tip" )
						{
							state->PushString( pDbcTitle->szTitleDesc );
						}
						// ��ʾ��ɫ
						else if( szFilter == "color" )
						{
							state->PushString( pDbcTitle->szColor );
						}
						// Ĭ���Ƿ���ʾ
						else if( szFilter == "show" )
						{
							state->PushInteger( pDbcTitle->bShow );
						}
						// ��ȡ;��
						else if( szFilter == "acquire" )
						{
							state->PushString( pDbcTitle->szAcquireDesc );
						}
						// Ч��ID
						else if( szFilter == "effect" )
						{
							pDbcTitle->nEffectId;

							// �� std_impact.tab ���ж�����

							state->PushString( " effect" );
						}
						// ����ԭ��
						else if( szFilter == "reclaim" )
						{
							state->PushString( pDbcTitle->szReclaimCause );
						}

						break;
					}

					titleNum++;
				}
			}
		}

		return 1;
	}

	INT PlayerMySelf::GetCurrentAgname(LuaPlus::LuaState* state)
	{

//		// ��ǰ�ƺ�����Ϊ����
//		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() == _TITLE::MOOD_TITLE )
//		{
//			state->PushString("");
//			return 1;
//		}
//
//		// ��ȡ��ǰ�ƺ�
//		const char* szCurTitle = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle();
//		/*if(strcmp(szCurTitle, "") != 0)
//		{*/
//			state->PushString( szCurTitle );
//		/*}*/
//		return 1;



		//INT TitleID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle();
		//state->PushInteger(TitleID);

	/*	_TITLE_	Title = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Title(TitleID);

		if(Title.bFlag == _TITLE_::INVALID_TITLE )
		{
			state->PushString("�п�");
		}
		else if(Title.bFlag == _TITLE_::ID_TITLE)
		{
			INT TitleTBIndex = Title.ID;
			if(TitleTBIndex>0)
			{
				DBC_DEFINEHANDLE(pTitleDBC, DBC_TITLE_NAME);
				const _DBC_TITLE_NAME* pTitle = (const _DBC_TITLE_NAME*)pTitleDBC->Search_LineNum_EQU(TitleTBIndex);
				state->PushString( pTitle->szTitle);
			}
		}
		else
		{
			state->PushString( Title.szTitleData );
		}*/


		LuaStack args(state);
		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: GetCurrentAgname Wrong Param1");
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: GetCurrentAgname Wrong Param2");
		}

		INT nType	= args[2].GetInteger();
		INT nTag	= args[3].GetInteger();	// �����ȡ�ַ���������

		if( nType < _TITLE::GUOJIA_TITLE || nType > _TITLE::WANFA_TITLE )
		{
			state->PushString( "��" );
			return 1;
		}

		// ȡ�ַ���
		if( 0 == nTag )
		{
			STRING szCurTitle = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle( nType );
			state->PushString( szCurTitle.c_str() );
		}
		// ȡid
		else
		{
			state->PushInteger( CDataPool::GetMe()->Title_GetCurTitle( nType ) );
		}

		return 1;
	}

	INT PlayerMySelf::AskChangeCurrentAgname(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: AskChangeCurrentAgname Wrong Param1");
		}
		INT nIndex = args[2].GetInteger();
		
		_TITLE_ Title = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Title(nIndex);
	
		if(Title.bFlag == _TITLE_::INVALID_TITLE )
		{
			CGameInterface::GetMe()->Title_UpdateCurTitle(Title.bType, 0);
		}
		else if(Title.bFlag == _TITLE_::ID_TITLE)
		{
			INT TitleTBIndex = Title.ID;
			if(TitleTBIndex>0)
			{
//				DBC_DEFINEHANDLE(pTitleDBC, DBC_TITLE_NAME);	// char_title.tab
//				const _DBC_TITLE_NAME* pTitle = (const _DBC_TITLE_NAME*)pTitleDBC->Search_LineNum_EQU(TitleTBIndex);
//				CGameInterface::GetMe()->Title_UpdateCurTitle(Title.bType, Title.ID);
			}
		}
		else
		{
			CGameInterface::GetMe()->Title_UpdateCurTitle( Title.bType, 0 );
		}
		
		return 0;
	}


	
	INT PlayerMySelf::SetNullAgname(LuaPlus::LuaState* state)
	{
		_TITLE_ Title;
		CGameInterface::GetMe()->Title_UpdateCurTitle(_TITLE::NO_TITLE, 0);
		return 0;
	}

	// ���³ƺ�״̬
	INT PlayerMySelf::UpdateTitleState(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if( !( args[2].IsInteger() ) )
		{
			KLThrow("LUA: UpdateTitleState Wrong Param1");
		}
		if( !( args[3].IsInteger() ) )
		{
			KLThrow("LUA: UpdateTitleState Wrong Param2");
		}
		if( !( args[4].IsInteger() ) )
		{
			KLThrow("LUA: UpdateTitleState Wrong Param3");
		}


		INT nType  = args[2].GetInteger();		// ����
		INT nIndex = args[3].GetInteger();		// ����
		INT nState = args[4].GetInteger();		// ״̬

		if( nType < _TITLE::GUOJIA_TITLE || nType > _TITLE::WANFA_TITLE )
			return 0;

		// ������Ч
		if( nIndex == -1 )
		{
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "δѡ��ƺ� ����" );
			return 0;
		}


		// ����״̬����
		enum 
		{
			TITLE_SHOW = 0,		// ��ʾ
			TITLE_HIDE,			// ����
		};


		CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if( !pCharacterData )
			return 0;


		// ��������ȡ����
		INT nTitleDataIndex = CDataPool::GetMe()->Title_GetTitle( nType, nIndex );
		_TITLE_ title = pCharacterData->Get_Title( nTitleDataIndex );


		// �ƺ� 
		STRING sTitle = "";

/*
		if( _TITLE_::ID_TITLE == title.bType )
		{
			// ����ȡ����
			DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab

			if( title.ID != -1 )
			{
				const _DBC_TITLE* pDbcTitle = (const _DBC_TITLE*)pTitleDBC->Search_LineNum_EQU( title.ID );
				if( pDbcTitle )
				{
					// Ů
					if( 0 == pCharacterData->Get_RaceID() )
						sTitle = pDbcTitle->szWomanTitle;
					// ��
					else if( 1 == pCharacterData->Get_RaceID() )
						sTitle = pDbcTitle->szManTitle;
				}
			}
		}
		else 
*/

		if( _TITLE_::STRING_TITLE == title.bFlag )
		{
			sTitle = title.szTitleData;
		}


		// ���͸��³ƺ�״̬����ʾ/���أ���Ϣ
		CGCharUpdateCurTitle Msg;

		Msg.setTitleType( nType );	// �ƺ�����
		if( 0 == nState )
			Msg.setTitleID( -1 );
		else if( 1 == nState )
			Msg.setTitleID( title.bArrId );
		Msg.setGuid( CObjectManager::GetMe()->GetMySelf()->GetServerGUID() );
		Msg.setTitleSize( 0 );

		CNetManager::GetMe()->SendPacket( &Msg );


		// Ӧ��
		pCharacterData->Set_CurTitle( sTitle.c_str(), nType );

		// ����״̬��ֻ������ʱ��һ�Σ�
		if( FALSE == nState )
			CObjectManager::GetMe()->GetMySelf()->SetBoardTitleState( nState, nType );

		return 0;
	}
	

	// �����ֶ��������Ե��� 
	INT PlayerMySelf::SendAskManualAttr(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: SendAskManualAttr[2] Wrong Param1");
		}

		if (!(args[3].IsInteger()))
		{
			KLThrow("LUA: SendAskManualAttr[3] Wrong Param1");
		}

		if (!(args[4].IsInteger()))
		{
			KLThrow("LUA: SendAskManualAttr[4] Wrong Param1");
		}

		if (!(args[5].IsInteger()))
		{
			KLThrow("LUA: SendAskManualAttr[5] Wrong Param1");
		}

		if (!(args[6].IsInteger()))
		{
			KLThrow("LUA: SendAskManualAttr[6] Wrong Param1");
		}


		CGReqManualAttr msg;
		
		int iStr = args[2].GetInteger();
//		int iSpr = args[3].GetInteger();
		int iCon = args[4].GetInteger();
		int iInt = args[5].GetInteger();
		int iDex = args[6].GetInteger();

		if((0 == iStr)&&
//			( 0 == iSpr)&&
			( 0 == iCon)&&
			( 0 == iInt)&&
			( 0 == iDex))
		{
			return 0;
		}

		msg.SetStr(iStr);
//		msg.SetSpr(iSpr);
		msg.SetCon(iCon);
		msg.SetInt(iInt);
		msg.SetDex(iDex);

		CNetManager::GetMe()->SendPacket(&msg);
	
		return 0;
	}
	
	//----------------------------------------------------------------------------------------------------------
	// ������
	//
	
	//----
	// ͬ��������(����������)
	INT	PlayerMySelf::SendAgreeJoinTeam(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: AskChangeCurrentAgname Wrong Param1");
		}
		int iTeamIndex = args[2].GetInteger();

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetInvite msg;
		
			//msg.SetGUID( CUIDataPool::GetMe()->GetTeamInvitorGUID());
			
			// �޸�֮���һ������
			GUID_t invitorGUID = CUIDataPool::GetMe()->GetTeamInvitorGUID(iTeamIndex);
			if(INVALID_GUID == invitorGUID)
			{
				return 0; 
			}

			msg.SetGUID(invitorGUID);
			msg.SetReturn(1);

			CNetManager::GetMe()->SendPacket(&msg);

			// ͬ�����һ�������, ��յ������Ķ���.
			CUIDataPool::GetMe()->ClearAllInviteTeam();
		}

		return 0;
	}

	// �ܾ��������(����������)
	INT PlayerMySelf::SendRejectJoinTeam(LuaPlus::LuaState* state)
	{
		// �õ���������
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: AskChangeCurrentAgname Wrong Param1");
		}
		int iTeamIndex = args[2].GetInteger();


		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetInvite msg;
		
			GUID_t invitorGUID = CUIDataPool::GetMe()->GetTeamInvitorGUID(iTeamIndex);
			if(INVALID_GUID == invitorGUID)
			{
				return 0; 
			}

			msg.SetGUID(invitorGUID);
			msg.SetReturn(0);
			CNetManager::GetMe()->SendPacket(&msg);

			// ɾ��һ���ܾ��Ķ���.
			CUIDataPool::GetMe()->EraseCurRejectTeam();
		}
		return 0;//

	}

		
	
	//-----
	// ͬ��������(��������������)
	INT	PlayerMySelf::SendAgreeJoinTeam_Apply(LuaPlus::LuaState* state)
	{
		// �õ���������
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: SendAgreeJoinTeam_Apply[2] Wrong Param1");
		}
		int iApplyIndex = args[2].GetInteger();

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetApply msg;
			GUID_t id = CUIDataPool::GetMe()->GetApplySourceGUID( iApplyIndex );
		
			if( INVALID_GUID == id )
			{
				return 0;
			}

			msg.SetSourGUID(id);
			msg.SetReturn(1);

			CNetManager::GetMe()->SendPacket(&msg);


			// �������ģ��
			TeamCacheInfo* pApplyInfo = const_cast<TeamCacheInfo*>( CUIDataPool::GetMe()->GetTeamOrGroup()->GetProposerByIndex( iApplyIndex ) );
			if( pApplyInfo )
			{
				pApplyInfo->m_UIModel.DestroyUIModel();
			}

		}

		return 0;//
	}

	// �ܾ��������(��������������)
	INT PlayerMySelf::SendRejectJoinTeam_Apply(LuaPlus::LuaState* state)
	{
		// �õ���������
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: SendRejectJoinTeam_Apply[2] Wrong Param1");
		}
		int iApplyIndex = args[2].GetInteger();

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetApply msg;
			GUID_t id = CUIDataPool::GetMe()->GetApplySourceGUID( iApplyIndex );
			
			if( INVALID_GUID == id )
			{
				return 0;
			}

			msg.SetSourGUID(id);
			msg.SetReturn(0);

			CNetManager::GetMe()->SendPacket(&msg);

			
			// �������ģ��
			TeamCacheInfo* pApplyInfo = const_cast<TeamCacheInfo*>( CUIDataPool::GetMe()->GetTeamOrGroup()->GetProposerByIndex( iApplyIndex ) );
			if( pApplyInfo )
			{
				pApplyInfo->m_UIModel.DestroyUIModel();
			}

		}

		return 0;//
	}



	//-----
	// ͬ��������(��Ա����ĳ�˼������)
	INT	PlayerMySelf::SendAgreeJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamLeaderRetInvite msg;

			// �õ���������
			LuaStack args(state);
			if (!(args[2].IsInteger()))
			{
				KLThrow("LUA: SendRejectJoinTeam_Apply[2] Wrong Param1");
			}
			int iIndex = args[2].GetInteger();

			GUID_t SId;
			GUID_t DId;
			if( CUIDataPool::GetMe()->GetMemberInviteGUID(iIndex, SId, DId) )
			{
				msg.SetSourGUID( SId );
				msg.SetDestGUID( DId );
				msg.SetReturn(1);

				CNetManager::GetMe()->SendPacket(&msg);
			}
		}
		return 0;
	}

	// �ܾ��������(��Ա����ĳ�˼������)
	INT PlayerMySelf::SendRejectJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamLeaderRetInvite msg;
		
			
			// �õ���������
			LuaStack args(state);
			if (!(args[2].IsInteger()))
			{
				KLThrow("LUA: SendRejectJoinTeam_Apply[2] Wrong Param1");
			}
			int iIndex = args[2].GetInteger();

			GUID_t SId;
			GUID_t DId;
			if(CUIDataPool::GetMe()->GetMemberInviteGUID(iIndex, SId, DId))
			{
				msg.SetSourGUID( SId );
				msg.SetDestGUID( DId );
				msg.SetReturn(0);
				CNetManager::GetMe()->SendPacket(&msg);
			}
		}
		return 0;
	}

	// ͬ�������Ӹ���ģʽ
	INT PlayerMySelf::SendAgreeTeamFollow(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGReturnTeamFollow Msg;

			Msg.SetReturn( TRUE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}
		return 0;
	}

	// �ܾ�������Ӹ���ģʽ
	INT PlayerMySelf::SendRefuseTeamFollow(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_You_Reject_Team_Follow");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��ܾ�������Ӹ���ģʽ��" );

			CGReturnTeamFollow Msg;
			Msg.SetReturn( FALSE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}
		return 0;
	}




	
	// �뿪����
	INT PlayerMySelf::LeaveTeam(LuaPlus::LuaState* state)					
	{

		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{
				CUIDataPool::GetMe()->LeaveTeam();
			}
		}
		return 0;
	}

	// ��ɢ����
	INT PlayerMySelf::DismissTeam(LuaPlus::LuaState* state)					
	{
		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{
				CUIDataPool::GetMe()->DismissTeam();
				SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();
			}
		}
		return 0;
	}

	// ɾ����ǰѡ�ж�Ա
	INT PlayerMySelf::KickTeamMember(LuaPlus::LuaState* state)			
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:KickTeamMember");
		}	

		INT iIndex = args[2].GetInteger();

		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{
				CUIDataPool::GetMe()->KickTeamMember(iIndex);
			}
		}
		return 0;
	}

	
	// ɾ����ǰѡ�ж�Ա
	INT PlayerMySelf::KickTeamMemberByIndex(LuaPlus::LuaState* state)			
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:KickTeamMemberByIndex");
		}	

		INT iIndex = args[2].GetInteger();
		
		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{

				CUIDataPool::GetMe()->KickTeamMember(iIndex);
			}
		}

		return 0;
	}


	
	//�Խ�����
	INT	PlayerMySelf::CreateTeamSelf(LuaPlus::LuaState* state)
	{
		//CEventSystem::GetMe()->PushEvent(GE_TEAM_MEMBER_INVITE, "sssss", "ddddddddd", (rand() % 5 + 1));

		// �Ѿ��ж����� 
		if( CUIDataPool::GetMe()->GetTeamOrGroup()->HasTeam() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_You_Can_Not_Create_Team");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "���Ѿ��ڶ������ˣ������Խ����顣" );
			return 0;
		}

		CGTeamInvite msg;
		msg.SetSourObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		msg.SetDestName(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}

		
	//�����ӳ���Ϣ
	INT	PlayerMySelf::AppointLeader(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:AppointLeader");
		}	

		INT iIndex = args[2].GetInteger();

		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{

				CUIDataPool::GetMe()->AppointLeader(iIndex);
			}
		}
		return 0;

	}

	// �Ƿ�����Ӹ���״̬
	INT	PlayerMySelf::InTeamFollowMode(LuaPlus::LuaState* state)
	{
		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		BOOL bMode = pCharData->Get_TeamFollowFlag();

		if( bMode )
		{
			state->PushBoolean(true);
		}
		else
		{
			state->PushBoolean(false);
		}

		return 1;
	}

	// ȡ������
	INT	PlayerMySelf::StopFollow(LuaPlus::LuaState* state)
	{
		CGStopTeamFollow Msg;
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}

	// ��Ӹ���
	INT PlayerMySelf::TeamFrame_AskTeamFollow(LuaPlus::LuaState* state)
	{
		// ������ڰ�̯״̬��������������
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_You_Stalling");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
			return 0;
		}

		CGAskTeamFollow Msg;
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}


	// �õ���Աguid
	INT PlayerMySelf::GetTeamMemberGUID(LuaPlus::LuaState* state)
	{

		// �õ���������
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA: Send Add Friend Team");
		}
		int iIndex = args[2].GetInteger();

		GUID_t MyGUID;
		GUID_t SelGUID;
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			const TeamMemberInfo* pInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex(iIndex);

			if(pInfo)
			{
				MyGUID  = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
				SelGUID = pInfo->m_GUID;

				if(MyGUID != SelGUID)
				{
					state->PushInteger(SelGUID);
					return 1;
				}
				
			}
				
		}

		state->PushInteger(-1);
		return 1;
	}


	//�޸����״̬����Ʒʰȡģʽ
	INT PlayerMySelf::ModifyTeamAllocationMode(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			KLThrow("LUA:Modify Team Allocation Mode error");
		}
		INT mMode = args[2].GetInteger();

		CGSetTeamDistribMode Msg;
		Msg.setMode(mMode);
		CNetManager::GetMe()->SendPacket( &Msg );

		return 0;
	}

	//������״̬����Ʒʰȡģʽ
	INT PlayerMySelf::GetTeamAllocationMode(LuaPlus::LuaState* state)
	{

		BYTE mMode = 0;
		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			mMode = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMode();
		}
		state->PushInteger(mMode);//����ʰȡ״̬
		return 1;
	}

	// ������
	//----------------------------------------------------------------------------------------------------------


	//��ý�ɫ��ǰ�Ի���NPC��id
	INT	PlayerMySelf::GetCurDialogNpcId(LuaPlus::LuaState*state)
	{
		int id = CUIDataPool::GetMe()->GetCurDialogNpcId();
		state->PushInteger(id);
		return 1;
	}
	
	// �õ���ҵĳƺ�
	INT	PlayerMySelf::GetCurTitle(LuaPlus::LuaState* state)
	{
		// 080401

		// ���鲻�ܳ����ڳƺŵ�λ��
//		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() == _TITLE::MOOD_TITLE )
//		{
//			state->PushString("");
//			return 1;
//		}
//
//		state->PushString( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle());
		
		return 0;
	}
	
	// �Ҽ��˵����
	INT PlayerMySelf::ShowMySelfContexMenu(LuaPlus::LuaState* state)
	{
		INT myId = CObjectManager::GetMe()->GetMySelf()->GetServerID();
		CGameInterface::GetMe()->Object_ShowContexMenu(myId, TRUE);
		return 0;
	}
	
	// ���ѡ���Լ�
	INT	PlayerMySelf::SelectMyselfAsTarget(LuaPlus::LuaState* state)
	{
		INT myId = CObjectManager::GetMe()->GetMySelf()->GetServerID();
		CGameInterface::GetMe()->Object_SelectAsMainTarget(myId, 1);
		return 0;
	}

	// ���ö�������
	INT	PlayerMySelf::SetSupperPassword(LuaPlus::LuaState* state)
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup(FALSE))
		{
			// ������ù����룬 ���޸����롣
			CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG);
		}
		else
		{
			// ������ù����룬 ���������롣
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup();
		}
		return 0;
	}


	//----------------------------------------------------------------------------------------------------------
	// ������������

	INT PlayerMySelf::Lua_PasswordGetData( LuaPlus::LuaState* state )
	{
		INT nPasswordData = CDataPool::GetMe()->Rich_GetPasswordData();


		//�Ѿ��󶨵�װ��
		if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_EQUIP)) ) > 0 )
			state->PushInteger( 1 );
		else
			state->PushInteger( 0 );

		//�ֿ�
		if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_BANK)) ) > 0 )
			state->PushInteger( 1 );
		else
			state->PushInteger( 0 );

		//���ʹ��
		if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_MONEY)) ) > 0 )
			state->PushInteger( 1 );
		else
			state->PushInteger( 0 );

		//��̯�͹���
		if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_STALL)) ) > 0 )
			state->PushInteger( 1 );
		else
			state->PushInteger( 0 );

		//���
		if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_GUILD)) ) > 0 )
			state->PushInteger( 1 );
		else
			state->PushInteger( 0 );

		return 5;
	}

	INT PlayerMySelf::Lua_SavePasswordSetup( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("Lua_SavePasswordSetup[1] param parameter error");
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("Lua_SavePasswordSetup[2] param parameter error");
		}
		if( !(args[4].IsInteger()) )
		{
			KLThrow("Lua_SavePasswordSetup[3] param parameter error");
		}
		if( !(args[5].IsInteger()) )
		{
			KLThrow("Lua_SavePasswordSetup[4] param parameter error");
		}
		if( !(args[6].IsInteger()) )
		{
			KLThrow("Lua_SavePasswordSetup[5] param parameter error");
		}


		INT nBindEquip	= args[2].GetInteger();	// �Ѿ��󶨵�װ��
		INT nDepot		= args[3].GetInteger();	// �ֿ�
		INT nGold		= args[4].GetInteger();	// ���ʹ��
		INT nStall		= args[5].GetInteger();	// ��̯�͹���
		INT nGuild		= args[6].GetInteger();	// ���
		
		INT nPasswordData = CDataPool::GetMe()->Rich_GetPasswordData();
		INT Original = nPasswordData;
		

		// �Ѿ��󶨵�װ��
		if( nBindEquip == 0 )
		{
			INT nPT =  1 << (MINORPASSWD_INFO::PROTECT_EQUIP);
			nPasswordData = ~(nPT) & nPasswordData;
		}
		else
		{
			INT nPT =  1 << (MINORPASSWD_INFO::PROTECT_EQUIP);
			nPasswordData = nPT | nPasswordData;
		}
		
		// �ֿ�
		if( nDepot == 0 )
		{
			INT nPT =  1 << (MINORPASSWD_INFO::PROTECT_BANK);
			nPasswordData = ~(nPT) & nPasswordData;
		}
		else
		{
			INT nPT =  1 << (MINORPASSWD_INFO::PROTECT_BANK);
			nPasswordData = nPT | nPasswordData;
		}

		// ���ʹ��
		if( nGold == 0 )
		{
			INT nPT =  1 << (MINORPASSWD_INFO::PROTECT_MONEY);
			nPasswordData = ~(nPT) & nPasswordData;
		}
		else
		{
			INT nPT =  1 << (MINORPASSWD_INFO::PROTECT_MONEY);
			nPasswordData = nPT | nPasswordData;
		}

		// ��̯�͹���
		if( nStall == 0 )
		{
			INT nPT =  1 << (MINORPASSWD_INFO::PROTECT_STALL);
			nPasswordData = ~(nPT) & nPasswordData;
		}
		else
		{
			INT nPT =  1 << (MINORPASSWD_INFO::PROTECT_STALL);
			nPasswordData = nPT | nPasswordData;
		}

		// ���
		if( nGuild == 0 )
		{
			INT nPT =  1 << (MINORPASSWD_INFO::PROTECT_GUILD);
			nPasswordData = ~(nPT) & nPasswordData;
		}
		else
		{
			INT nPT =  1 << (MINORPASSWD_INFO::PROTECT_GUILD);
			nPasswordData = nPT | nPasswordData;
		}


		if( Original != nPasswordData )
		{
			CGMinorPasswd Msg;
	
			CG_MINORPASSWD* pMinorPasswd = Msg.GetMinorPassword();
			
			pMinorPasswd->CleanUp();

			pMinorPasswd->m_Type = MREQT_SETPROTECTTYPE;		// ���ö������뱣������
			pMinorPasswd->m_nFlag = nPasswordData;

			CNetManager::GetMe()->SendPacket( &Msg );
		}
	
		return 0;
	}

	// ��ȡ��������״̬
	INT PlayerMySelf::Lua_PasswordTypeGetState( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("Lua_SavePasswordSetup[1] param parameter error");
		}

		INT nType = args[2].GetInteger();	// ����������

		if( nType < 0 || nType >= 5 )
		{
			state->PushInteger( -1 );
			return 1;
		}

		INT nPasswordData = CDataPool::GetMe()->Rich_GetPasswordData();

		switch( nType )
		{
		// �Ѿ��󶨵�װ��
		case 0:
			{
				if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_EQUIP)) ) > 0 )
					state->PushInteger( 1 );
				else
					state->PushInteger( 0 );

				break;
			};
		// �ֿ�
		case 1:
			{
				if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_BANK)) ) > 0 )
					state->PushInteger( 1 );
				else
					state->PushInteger( 0 );

				break;
			}
		// ���ʹ��
		case 2:
			{
				if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_MONEY)) ) > 0 )
					state->PushInteger( 1 );
				else
					state->PushInteger( 0 );

				break;
			}
		// ��̯�͹���
		case 3:
			{
				if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_STALL)) ) > 0 )
					state->PushInteger( 1 );
				else
					state->PushInteger( 0 );

				break;
			}
		// ���
		case 4:
			{
				if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_GUILD)) ) > 0 )
					state->PushInteger( 1 );
				else
					state->PushInteger( 0 );

				break;
			}
		}

		return 1;
	}



	// �����¼�����
	VOID PlayerMySelf::ProtectEventHandling( INT nType )
	{
		// ���ݲ�ͬ������������ͬ���¼�
		// ���ű��е����¼��ĵط�ͳһ��������һ�������У� ���˴�����
		switch( nType )
		{
		// �Ѿ��󶨵�װ��
		// MINORPASSWD_INFO::PROTECT_EQUIP
		case 0:
			{
				// 
				CEventSystem::GetMe()->PushEvent( GE_QUEST_INPUT_PASSWD_COMMAND, m_sQuestScript.c_str(), "", m_nQuestIndex );

				break;
			}
		// �ֿ�
		// MINORPASSWD_INFO::PROTECT_BANK
		case 1:
			{
//				Bank::OpenInputFrame();

				// ������Ϣ
				const char* password = CDataPool::GetMe()->Rich_GetInputPassword().c_str();
				int len = (int)strlen( password );
				
				CGBankAcquireList Msg;
				Msg.SetPassWord( password );
				Msg.SetPwdSize( len );
				
				CNetManager::GetMe()->SendPacket(&Msg);

				break;
			}
		// ���ʹ��	MINORPASSWD_INFO::PROTECT_MONEY
		case 2:
			{
				break;
			}
		// ��̯�͹���
		// MINORPASSWD_INFO::PROTECT_STALL
		case 3:
			{
				CEventSystem::GetMe()->PushEvent( GE_PACKET_OPEN_SALE );

				break;
			}
		// ������
		// MINORPASSWD_INFO::PROTECT_GUILD
		case 4:
			{
				// ���ýű��������������Ϣ
				CEventSystem::GetMe()->PushEvent( GE_INPUT_PASSWD_COMMAND );	// ͨ���������봥��
				break;
			}
		};
	}


	// ����������
	INT PlayerMySelf::Lua_CheckPasswdProcedure( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("Lua_CheckPasswdProcedure[1] param parameter error");
			return 0;
		}

		// ����
		INT nType =  args[2].GetInteger();

		// ��ȡ�������Ƿ��Ѿ�У���������
		BOOL bState = CDataPool::GetMe()->Rich_GetInputPasswdState( nType );	

		// ��Ҫ�� �������������ļ��������Ϣ�е��� Rich_SetInputPasswdState ����־�Ƿ��Ѽ����


		if( !bState )
		{
			// ��װ���Ĳ�����Ҫ��������
			if( 0 == nType )
			{
				m_nQuestIndex	= args[3].GetInteger();
				m_sQuestScript	= args[4].GetString();
			}

			// ��ʾ��������
			CEventSystem::GetMe()->PushEvent( GE_RICH_CHECK_PASSWORD, nType );
		}
		// �Ѿ�������ˣ� ������ʾ��������
		else
		{
			ProtectEventHandling( nType );
		}

		return 0;
	}

	// ������������������
	INT PlayerMySelf::Lua_InputPasswdProcedure( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("InputPasswdProcedure[1] param parameter error");
		}
		if( !(args[3].IsString()) )
		{
			KLThrow("InputPasswdProcedure[2] param parameter error");
		}

		// ����
		INT nType =  args[2].GetInteger();
		// ����
		STRING pInputPassword = args[3].GetString();
		
		// ������������뵽���ݳ���
		CDataPool::GetMe()->Rich_SetInputPassword( pInputPassword );	// ֻ����һ���Ϳ����ˣ� ��Ϊ���е����붼һ��
		
		ProtectEventHandling( nType );

		return 0;
	}


}	// namespace