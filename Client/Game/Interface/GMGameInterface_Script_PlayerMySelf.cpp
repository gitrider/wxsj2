
/**	GMGameInterface_Script_playerMySelf.cpp
 *	
 *	功能：	
 *
 *	修改记录：	
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
#include "CGTeamInvite.h"								// 邀请加入队伍消息				
#include "CGTeamRetInvite.h"							// 邀请加入队伍消息返回
#include "CGTeamRetApply.h"								// 回答某人申请加入队伍消息
#include "CGTeamLeaderRetInvite.h"						// 回答队员邀请加入队伍消息
#include "CGReqManualAttr.h"							// 申请手动添加点数.
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


	// 得到玩家的名字
	INT	PlayerMySelf::GetName(LuaPlus::LuaState* state)
	{
		state->PushString( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		return 1;
	}

	// 得到角色身上的buff的数量
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

		// 20100426 ModifyCodeBegin 改为如果找不到图标，就显示为空，不要用assert中断程序了
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
			// 20100429 ModifyCodeBegin 修正 根据playerimpactframe的buff的索引号1-10，获取buff技能tip的一些错误
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
		//角色死亡后复活按钮响应逻辑
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
		if( str == "LEVEL" )	//等级
		{
			state->PushInteger(pCharData->Get_Level());
			return 1;
		}
		else if( str == "HP" )					//生命值
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

		else if( str == "MP" )					//魔法值
		{
			state->PushInteger(pCharData->Get_MP());
			return 1;
		}
		else if( str == "PNEUMA" )					//元气值
		{
			state->PushInteger(pCharData->Get_Penuma());
			return 1;
		}
		else if( str == "EXP" )					//经验
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
		else if( str == "NEEDEXP" )				//升级经验
		{
			state->PushInteger(pCharData->Get_MaxExp());
			return 1;
		}
		else if( str == "STRIKEPOINT" )				//连击点
		{
			state->PushInteger(pCharData->Get_StrikePoint());
			return 1;
		}
		else if( str == "MONEY" )				//货币
		{
			state->PushInteger(pCharData->Get_Money());

			//同时输出转换为金、银、铜币的数值
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
		else if( str == "STR" )					//力量 力量
		{
			state->PushInteger(pCharData->Get_STR());
			return 1;
		}
//		else if( str == "SPR" )					//灵气 灵力
//		{
//			state->PushInteger(pCharData->Get_SPR());
//			return 1;
//		}
		else if( str == "CON" )					//体制 体制
		{
			state->PushInteger(pCharData->Get_CON());
			return 1;
		}
		else if( str == "INT" )					//定力 智力
		{
			state->PushInteger(pCharData->Get_INT());
			return 1;
		}								   
		else if( str == "DEX" )					//身法 敏捷
		{
			state->PushInteger(pCharData->Get_DEX());
			return 1;
		}								   
		else if( str == "POINT_REMAIN" )			//剩余点数
		{
			state->PushInteger(pCharData->Get_PointRemain());
			return 1;
		}	

//		else if( str == "ATT_PHYSICS" )			//物理攻击力
//		{
//			state->PushInteger(pCharData->Get_AttPhysics());
//			return 1;
//		}								   
//		else if( str == "DEF_PHYSICS" )			//物理防御力
//		{
//			state->PushInteger(pCharData->Get_DefPhysics());
//			return 1;
//		}

		else if( str == "ATT_NEAR" )			//近攻击力
		{
			state->PushInteger(pCharData->Get_AttNear());
			return 1;
		}
		else if( str == "DEF_NEAR" )			//近
		{
			state->PushInteger(pCharData->Get_DefNear());
			return 1;
		}

		else if( str == "ATT_FAR" )			//远攻击力
		{
			state->PushInteger(pCharData->Get_AttFar());
			return 1;
		}
		else if( str == "DEF_FAR" )			//远
		{
			state->PushInteger(pCharData->Get_DefFar());
			return 1;
		}

		else if( str == "ATT_MAGIC_NEAR" )			//魔法攻击力
		{
			state->PushInteger(pCharData->Get_AttMagic_Near());
			return 1;
		}								   
		else if( str == "DEF_MAGIC_NEAR" )			//魔法防御力
		{
			state->PushInteger(pCharData->Get_DefMagic_Near());
			return 1;
		}
		else if( str == "ATT_MAGIC_FAR" )			//魔法攻击力
		{
			state->PushInteger(pCharData->Get_AttMagic_Far());
			return 1;
		}								   
		else if( str == "DEF_MAGIC_FAR" )			//魔法防御力
		{
			state->PushInteger(pCharData->Get_DefMagic_Far());
			return 1;
		}
		else if( str == "MAXHP" )				//最大生命值
		{
			state->PushInteger(pCharData->Get_MaxHP());
			return 1;
		}								   
		else if( str == "MAXMP" )				//最大魔法值
		{			
			state->PushInteger(pCharData->Get_MaxMP());
			return 1;
		}
		else if( str == "MAXPNEUMA" )				//最大元气值
		{			
			state->PushInteger(pCharData->Get_MaxPneuma());
			return 1;
		}	
		else if( str == "RAGE" )				//怒气
		{			
			state->PushInteger(pCharData->Get_Rage());
			return 1;
		}								   
		else if( str == "MAXRAGE" )				//最大怒气
		{			
			state->PushInteger(pCharData->Get_MaxRage());
			return 1;
		}								   
		else if( str == "HP_RESPEED" )			//HP恢复速度  点/秒
		{			
			state->PushInteger(pCharData->Get_HPRespeed());
			return 1;
		}								   
		else if( str == "MP_RESPEED" )			//MP恢复速度  点/秒
		{
			state->PushInteger(pCharData->Get_MPRespeed());
			return 1;
		}
		else if( str == "PNEUMA_RESPEED" )			//元气恢复速度  点/秒
		{
			state->PushInteger(pCharData->Get_PenumaPercent());
			return 1;
		}	
		else if( str == "HIT" )					//命中率
		{
			state->PushInteger(pCharData->Get_Hit());
			return 1;
		}								   
		else if( str == "MISS" )					//闪避率
		{
			state->PushInteger(pCharData->Get_Miss());
			return 1;
		}								   
		else if( str == "CRITRATE" )				//会心率
		{
			state->PushInteger(pCharData->Get_CritRate());
			return 1;								   
		}	
		else if(str == "TOUGHNESS")                // 韧性
		{
			state->PushInteger(pCharData->Get_Toughness());
			return 1;	
		}
		else if( str == "MOVESPEED" )			//移动速度
		{
			state->PushNumber(pCharData->Get_MoveSpeed());
			return 1;								   
		}								   
		else if( str == "ATTACKSPEED" )			//攻击速度
		{
			state->PushInteger(pCharData->Get_AttackSpeed());
			return 1;
		}
		//金攻击
		else if( str == "ATTACKGOLD" )			
		{
			state->PushInteger(pCharData->Get_AttGold());
			return 1;								   
		}
		//金防御
		else if( str == "DEFENCEGOLD" )			
		{
			state->PushInteger(pCharData->Get_DefGold());
			return 1;								   
		}
		//火攻击
		else if( str == "ATTACKFIRE" )			
		{
			state->PushInteger(pCharData->Get_AttFire());
			return 1;								   
		}
		//火防御
		else if( str == "DEFENCEFIRE" )			
		{
			state->PushInteger(pCharData->Get_DefFire());
			return 1;								   
		}
		//水攻击
		else if( str == "ATTACKWATER" )			
		{
			state->PushInteger(pCharData->Get_AttWater());
			return 1;								   
		}
		//水防御
		else if( str == "DEFENCEWATER" )			
		{
			state->PushInteger(pCharData->Get_DefWater());
			return 1;								   
		}
		//木攻击
		else if( str == "ATTACKWOOD" )
		{
			state->PushInteger(pCharData->Get_AttWood());
			return 1;								   
		}
		// 木防御
		else if( str == "DEFENCEWOOD" )
		{
			state->PushInteger(pCharData->Get_DefWood());
			return 1;								   
		}
		//土攻击
		else if( str == "ATTACKSOIL" )
		{
			state->PushInteger(pCharData->Get_AttSoil());
			return 1;								   
		}
		// 土防御
		else if( str == "DEFENCESOIL" )
		{
			state->PushInteger(pCharData->Get_DefSoil());
			return 1;								   
		}
		// 阵营
		else if( str == "CAMP" )					
		{
			// 接口变动
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
		// 帮会
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
		else if( str == "PORTRAIT" )			//人物头像
		{
			
			// 得到玩家和自己的头像信息
			LPCSTR pImageName = NULL;
			DBC_DEFINEHANDLE(s_pFaceImageDBC, DBC_CHARACTER_FACE)	// char_face.tab
			//打开生物定义表
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
			//else // 默认状态不显示头像图标， 头像图标显示空
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
	
	// 得到技能总数 Nick 2007.11.23
	INT PlayerMySelf::GetSkillNum( LuaPlus::LuaState* state )
	{
		state->PushInteger( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_SkillNum() );
		return 1;
	}
	
	// 按索引得到角色的某项战斗技能的信息 Nick 2007.11.23
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

		if(str == "name")			// 名称
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszName);
			else
				state->PushString("");
		}
		else if(str == "learn")		// 是否学会
		{
			if(pSkill)
				state->PushBoolean(pSkill->m_bLeaned == TRUE);
			else
				state->PushBoolean(false);
		}
		else if(str == "iconname")	// 图标文件名 
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszIconName);
			else
				state->PushString("");
		}
		else if(str == "passivity")	// 是否是被动技能
		{	
			if(pSkill)
				state->PushInteger(pSkill->m_pDefine->m_nPassiveFlag);
		}
		else if(str == "explain")	// 详细解释
		{	
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszSkillExplainForInterface);
			else
				state->PushString("");
		}
		else if(str == "skilldata")	// 技能数据
		{	
			if(pSkill)
				state->PushString(pSkill->GetSkillDesc_Interface());
			else
				state->PushString("");
		}
		else if(str == "skilllevel")	// 技能等级 Nick 2007.11.23
		{	
			if(pSkill)
				state->PushInteger(-1);
		}

		return 1;
	}

	// 获得玩家的某项技能信息
	INT PlayerMySelf::GetSkillInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;

		INT	nSkillID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		const SCLIENT_SKILL* pSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(nSkillID);

		if(str == "name")			// 名称
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszName);
			else
				state->PushString("");
		}
		else if(str == "learn")		// 是否学会
		{
			if(pSkill)
				state->PushBoolean(pSkill->m_bLeaned == TRUE);
			else
				state->PushBoolean(false);
		}
		else if(str == "iconname")	// 图标文件名
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszIconName);
			else
				state->PushString("");
		}
		else if(str == "passivity")	// 是否是被动技能
		{	
			if(pSkill)
				state->PushInteger(pSkill->m_pDefine->m_nPassiveFlag);
		}
		else if(str == "explain")	// 详细解释
		{	
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszSkillExplainForInterface);
			else
				state->PushString("");
		}
		else if(str == "skilldata")	// 技能数据
		{	
			if(pSkill)
				state->PushString(pSkill->GetSkillDesc_Interface());
			else
				state->PushString("");
		}
		else if(str == "skilllevel")	// 技能等级 Nick 2007.11.23
		{	
			if(pSkill)
				state->PushInteger(-1);
		}

		return 1;
	}

	//获得玩家的某项心法信息
	INT PlayerMySelf::GetXinfaInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;

		INT	nXinfaID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		//const SCLIENT_XINFA* pXinfa = 
		//	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa(nXinfaID);

		//if(str == "name")			// 名称
		//{
		//	state->PushString(pXinfa->m_pDefine->pszName);
		//}
		//else if(str == "level")		// 等级
		//{
		//	state->PushInteger(pXinfa->m_nLevel);
		//}
		//else if(str == "iconname")	// 图标文件名
		//{
		//	state->PushString(pXinfa->m_pDefine->pszIconName);
		//}
		//else if(str == "explain")	// 详细解释
		//{	
		//	state->PushString(pXinfa->m_pDefine->pszDesc);
		//}
		return 1;
	}

	//获得玩家是否学过某项生活技能
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

	//获得玩家的某项生活技能信息
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

		if(str == "name")			// 名称
		{
			state->PushString(pLifeAbility->m_pDefine->szName);
		}
		else if(str == "level")		// 等级
		{
			state->PushInteger(pLifeAbility->m_nLevel);
		}
		else if(str == "maxlevel")	// 等级上限
		{
			state->PushInteger(pLifeAbility->m_pDefine->nLevelMax);
		}
		else if(str == "skillexp")	// 熟练度
		{
			state->PushInteger(pLifeAbility->m_nExp);
		}
		else if(str == "iconname")	// 图标文件名
		{
			state->PushString(pLifeAbility->m_pDefine->szIconName);
		}
		else if(str == "explain")	// 详细解释
		{	
			state->PushString(pLifeAbility->m_pDefine->szExplain);
		}
		else if(str == "popup")	// 是否弹出下级界面
		{	// 1.弹出合成界面，2.弹出镶嵌界面，-1.不弹界面
			state->PushInteger(pLifeAbility->m_pDefine->nPopup);
		}
		return 1;
	}

	// 角色请求学习生活技能
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
  
		//// 玩家生活技能数据
		//const SCLIENT_LIFEABILITY* pLifeAbility = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbilityID);

		//if(!pLifeAbility) 
		//	return 0;

		//// 用本地数据先判定是否满足升级条件
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
		//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"经验值不足");
		//	return 0;
		//}
		//if( pData->Get_Money() < CDataPool::GetMe()->StudyAbility_GetNeedMoney() )
		//{
		//	STRING strTemp = "";
		//	strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_Money_Not_Enough");
		//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "金钱不足" );
		//	return 0;
		//}
		//if( pLifeAbility->m_nExp < CDataPool::GetMe()->StudyAbility_GetSkillExp() )
		//{
		//	STRING strTemp = "";
		//	strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_Skill_Not_Enough");
		//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "技能熟练度不够" );
		//	return 0;
		//}
		////任务等级是否满足
		//if( CDataPool::GetMe()->StudyAbility_GetLevelLimit() > pData->Get_Level() )
		//{
		//	STRING strTemp = "";
		//	strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_Level_Not_Enough");
		//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
		//	//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "等级不够" );
		//	return 0;
		//}

		//向服务器发送升级请求
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
	
	//称号相关
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


	// 获取表中全部的称号数量
	INT PlayerMySelf::GetAllAgnameNum(LuaPlus::LuaState* state)
	{
		DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab
		UINT nRecordsNum = pTitleDBC->GetRecordsNum();	// 行数
		UINT nFieldsNum  = pTitleDBC->GetFieldsNum();	// 列数

		state->PushInteger( nRecordsNum );

		return 1;

	}
	// 分类保存表中数据
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
		UINT nRecordsNum = pTitleDBC->GetRecordsNum();	// 行数

		if( nIndex < 0 || nIndex > nRecordsNum )
			return 0;

		CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if( !pCharacterData )
			return 0;

		const _DBC_TITLE* pDbcTitle = (const _DBC_TITLE*)pTitleDBC->Search_LineNum_EQU( nIndex );
		if( pDbcTitle )
		{
			// 称号
			if( szFilter == "title" )
			{
				// 女
				if( 0 == pCharacterData->Get_RaceID() )
					state->PushString( pDbcTitle->szWomanTitle );
				// 男
				else if( 1 == pCharacterData->Get_RaceID() )
					state->PushString( pDbcTitle->szManTitle );
			}
			// 类型
			else if( szFilter == "type" )
			{
				state->PushInteger( pDbcTitle->nType );
			}
			// 称号时限
			else if( szFilter == "tiemer" )
			{
				// 从服务器传
				state->PushString( "" );
			}
			// 称号描述
			if( szFilter == "tip" )
			{
				state->PushString( pDbcTitle->szTitleDesc );
			}
			// 显示颜色
			else if( szFilter == "color" )
			{
				state->PushString( pDbcTitle->szColor );
			}
			// 默认是否显示
			else if( szFilter == "show" )
			{
				state->PushInteger( pDbcTitle->bShow );
			}
			// 获取途径
			else if( szFilter == "acquire" )
			{
				state->PushString( pDbcTitle->szAcquireDesc );
			}
			// 效果ID
			else if( szFilter == "effect" )
			{
				pDbcTitle->nEffectId;

				// 从 std_impact.tab 表中读数据

				state->PushString( " effect" );
			}
			// 回收原因
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
					// 女
					if( 0 == pCharacterData->Get_RaceID() )
						state->PushString( pDbcTitle->szWomanTitle );
					// 男
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

	// 获取信息
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


		// 根据类型取数据
		_TITLE_ title;

		INT nTitleDataIndex = CDataPool::GetMe()->Title_GetTitle( nType, nIndex );
		if( nTitleDataIndex != -1 )
			title = pCharacterData->Get_Title( nTitleDataIndex );


		// 字符操作
		if( _TITLE_::STRING_TITLE == title.bFlag )
		{
			// 称号
			if( szFilter == "title" )
			{
				state->PushString( title.szTitleData );
			}
			// 称号时限
			else if( szFilter == "tiemer" )
			{
				// 计算剩余时间
	//			INT tiem = pDbcTitle->nTimeLimit - title.nTime;
				INT tiem = title.nTime;

				// 无限期
				if( tiem == 0 )
				{
					state->PushString( "无限期" );
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
					_snprintf( szText, _MAX_PATH, "%d天%d小时%d分%d秒", day, hour, minute, second );

					// 从服务器传
					state->PushString( szText );
				}

			}
			// 其他需要读表的
			else if( title.ID != -1 )
			{
				// 读表取数据
				DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab
				const _DBC_TITLE* pDbcTitle = (const _DBC_TITLE*)pTitleDBC->Search_Index_EQU( title.ID );
				if( pDbcTitle )
				{
					// 称号描述
					if( szFilter == "tip" )
					{
						state->PushString( pDbcTitle->szTitleDesc );
					}
					// 显示颜色
					else if( szFilter == "color" )
					{
						state->PushString( pDbcTitle->szColor );
					}
					// 默认是否显示
					else if( szFilter == "show" )
					{
						state->PushInteger( pDbcTitle->bShow );
					}
					// 获取途径
					else if( szFilter == "acquire" )
					{
						state->PushString( pDbcTitle->szAcquireDesc );
					}
					// 效果ID
					else if( szFilter == "effect" )
					{
						pDbcTitle->nEffectId;

						// 从 std_impact.tab 表中读数据

						state->PushString( " effect" );
					}
					// 回收原因
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
		// 说明不是服务器发过来的， 只是显示用的， 读表 
		else
		{
			// 根据类型 和 索引来

			// 该类称号的数量
			INT titleNum = CDataPool::GetMe()->Title_GetTitleNum( nType );

			DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab
			UINT nRecordsNum = pTitleDBC->GetRecordsNum();	// 行数

			_DBC_TITLE* pDbcTitle = NULL;
			for( INT num = 0; num < nRecordsNum; ++num )
			{
				pDbcTitle = (_DBC_TITLE*)pTitleDBC->Search_LineNum_EQU( num );
				if( pDbcTitle && ( pDbcTitle->nType == nType ) )
				{
					if( titleNum == nIndex )
					{
						// 称号
						if( szFilter == "title" )
						{
							// 女
							if( 0 == pCharacterData->Get_RaceID() )
								state->PushString( pDbcTitle->szWomanTitle );
							// 男
							else if( 1 == pCharacterData->Get_RaceID() )
								state->PushString( pDbcTitle->szManTitle );
						}
						// 类型
						else if( szFilter == "type" )
						{
							state->PushInteger( pDbcTitle->nType );
						}
						// 称号时限
						else if( szFilter == "tiemer" )
						{
							// 从服务器传
							state->PushString( "" );
						}
						// 称号描述
						if( szFilter == "tip" )
						{
							state->PushString( pDbcTitle->szTitleDesc );
						}
						// 显示颜色
						else if( szFilter == "color" )
						{
							state->PushString( pDbcTitle->szColor );
						}
						// 默认是否显示
						else if( szFilter == "show" )
						{
							state->PushInteger( pDbcTitle->bShow );
						}
						// 获取途径
						else if( szFilter == "acquire" )
						{
							state->PushString( pDbcTitle->szAcquireDesc );
						}
						// 效果ID
						else if( szFilter == "effect" )
						{
							pDbcTitle->nEffectId;

							// 从 std_impact.tab 表中读数据

							state->PushString( " effect" );
						}
						// 回收原因
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

//		// 当前称号类型为心情
//		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() == _TITLE::MOOD_TITLE )
//		{
//			state->PushString("");
//			return 1;
//		}
//
//		// 获取当前称号
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
			state->PushString("尚空");
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
		INT nTag	= args[3].GetInteger();	// 标记是取字符还是索引

		if( nType < _TITLE::GUOJIA_TITLE || nType > _TITLE::WANFA_TITLE )
		{
			state->PushString( "无" );
			return 1;
		}

		// 取字符串
		if( 0 == nTag )
		{
			STRING szCurTitle = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle( nType );
			state->PushString( szCurTitle.c_str() );
		}
		// 取id
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

	// 更新称号状态
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


		INT nType  = args[2].GetInteger();		// 类型
		INT nIndex = args[3].GetInteger();		// 索引
		INT nState = args[4].GetInteger();		// 状态

		if( nType < _TITLE::GUOJIA_TITLE || nType > _TITLE::WANFA_TITLE )
			return 0;

		// 索引无效
		if( nIndex == -1 )
		{
			CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "未选择称号 ！！" );
			return 0;
		}


		// 操作状态类型
		enum 
		{
			TITLE_SHOW = 0,		// 显示
			TITLE_HIDE,			// 隐藏
		};


		CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if( !pCharacterData )
			return 0;


		// 根据类型取数据
		INT nTitleDataIndex = CDataPool::GetMe()->Title_GetTitle( nType, nIndex );
		_TITLE_ title = pCharacterData->Get_Title( nTitleDataIndex );


		// 称号 
		STRING sTitle = "";

/*
		if( _TITLE_::ID_TITLE == title.bType )
		{
			// 读表取数据
			DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab

			if( title.ID != -1 )
			{
				const _DBC_TITLE* pDbcTitle = (const _DBC_TITLE*)pTitleDBC->Search_LineNum_EQU( title.ID );
				if( pDbcTitle )
				{
					// 女
					if( 0 == pCharacterData->Get_RaceID() )
						sTitle = pDbcTitle->szWomanTitle;
					// 男
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


		// 发送更新称号状态（显示/隐藏）消息
		CGCharUpdateCurTitle Msg;

		Msg.setTitleType( nType );	// 称号类型
		if( 0 == nState )
			Msg.setTitleID( -1 );
		else if( 1 == nState )
			Msg.setTitleID( title.bArrId );
		Msg.setGuid( CObjectManager::GetMe()->GetMySelf()->GetServerGUID() );
		Msg.setTitleSize( 0 );

		CNetManager::GetMe()->SendPacket( &Msg );


		// 应用
		pCharacterData->Set_CurTitle( sTitle.c_str(), nType );

		// 设置状态（只在隐藏时调一次）
		if( FALSE == nState )
			CObjectManager::GetMe()->GetMySelf()->SetBoardTitleState( nState, nType );

		return 0;
	}
	

	// 发送手动调整属性点数 
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
	// 组队相关
	//
	
	//----
	// 同意加入队伍(有人邀请我)
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
			
			// 修改之后加一个参数
			GUID_t invitorGUID = CUIDataPool::GetMe()->GetTeamInvitorGUID(iTeamIndex);
			if(INVALID_GUID == invitorGUID)
			{
				return 0; 
			}

			msg.SetGUID(invitorGUID);
			msg.SetReturn(1);

			CNetManager::GetMe()->SendPacket(&msg);

			// 同意加入一个队伍后, 清空掉其他的队伍.
			CUIDataPool::GetMe()->ClearAllInviteTeam();
		}

		return 0;
	}

	// 拒绝加入队伍(有人邀请我)
	INT PlayerMySelf::SendRejectJoinTeam(LuaPlus::LuaState* state)
	{
		// 得到队伍索引
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

			// 删掉一个拒绝的队伍.
			CUIDataPool::GetMe()->EraseCurRejectTeam();
		}
		return 0;//

	}

		
	
	//-----
	// 同意加入队伍(有人申请加入队伍)
	INT	PlayerMySelf::SendAgreeJoinTeam_Apply(LuaPlus::LuaState* state)
	{
		// 得到队伍索引
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


			// 清掉界面模型
			TeamCacheInfo* pApplyInfo = const_cast<TeamCacheInfo*>( CUIDataPool::GetMe()->GetTeamOrGroup()->GetProposerByIndex( iApplyIndex ) );
			if( pApplyInfo )
			{
				pApplyInfo->m_UIModel.DestroyUIModel();
			}

		}

		return 0;//
	}

	// 拒绝加入队伍(有人申请加入队伍)
	INT PlayerMySelf::SendRejectJoinTeam_Apply(LuaPlus::LuaState* state)
	{
		// 得到队伍索引
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

			
			// 清掉界面模型
			TeamCacheInfo* pApplyInfo = const_cast<TeamCacheInfo*>( CUIDataPool::GetMe()->GetTeamOrGroup()->GetProposerByIndex( iApplyIndex ) );
			if( pApplyInfo )
			{
				pApplyInfo->m_UIModel.DestroyUIModel();
			}

		}

		return 0;//
	}



	//-----
	// 同意加入队伍(队员邀请某人加入队伍)
	INT	PlayerMySelf::SendAgreeJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamLeaderRetInvite msg;

			// 得到队伍索引
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

	// 拒绝加入队伍(队员邀请某人加入队伍)
	INT PlayerMySelf::SendRejectJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamLeaderRetInvite msg;
		
			
			// 得到队伍索引
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

	// 同意进入组队跟随模式
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

	// 拒绝进入组队跟随模式
	INT PlayerMySelf::SendRefuseTeamFollow(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_You_Reject_Team_Follow");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "你拒绝进入组队跟随模式。" );

			CGReturnTeamFollow Msg;
			Msg.SetReturn( FALSE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}
		return 0;
	}




	
	// 离开队伍
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

	// 解散队伍
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

	// 删除当前选中队员
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

	
	// 删除当前选中队员
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


	
	//自建队伍
	INT	PlayerMySelf::CreateTeamSelf(LuaPlus::LuaState* state)
	{
		//CEventSystem::GetMe()->PushEvent(GE_TEAM_MEMBER_INVITE, "sssss", "ddddddddd", (rand() % 5 + 1));

		// 已经有队伍了 
		if( CUIDataPool::GetMe()->GetTeamOrGroup()->HasTeam() )
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_PlayerMySelf_Info_You_Can_Not_Create_Team");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "你已经在队伍中了，不能自建队伍。" );
			return 0;
		}

		CGTeamInvite msg;
		msg.SetSourObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		msg.SetDestName(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}

		
	//任命队长消息
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

	// 是否处于组队跟随状态
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

	// 取消跟随
	INT	PlayerMySelf::StopFollow(LuaPlus::LuaState* state)
	{
		CGStopTeamFollow Msg;
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}

	// 组队跟随
	INT PlayerMySelf::TeamFrame_AskTeamFollow(LuaPlus::LuaState* state)
	{
		// 如果处于摆摊状态，不允许这样做
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			STRING strTemp = "";
			strTemp = NOCOLORMSGFUNC("GMAI_Base_Info_You_Stalling");
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
			//CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
			return 0;
		}

		CGAskTeamFollow Msg;
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}


	// 得到队员guid
	INT PlayerMySelf::GetTeamMemberGUID(LuaPlus::LuaState* state)
	{

		// 得到队伍索引
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


	//修改组队状态下物品拾取模式
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

	//获得组队状态下物品拾取模式
	INT PlayerMySelf::GetTeamAllocationMode(LuaPlus::LuaState* state)
	{

		BYTE mMode = 0;
		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			mMode = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMode();
		}
		state->PushInteger(mMode);//返回拾取状态
		return 1;
	}

	// 组队相关
	//----------------------------------------------------------------------------------------------------------


	//获得角色当前对话的NPC的id
	INT	PlayerMySelf::GetCurDialogNpcId(LuaPlus::LuaState*state)
	{
		int id = CUIDataPool::GetMe()->GetCurDialogNpcId();
		state->PushInteger(id);
		return 1;
	}
	
	// 得到玩家的称号
	INT	PlayerMySelf::GetCurTitle(LuaPlus::LuaState* state)
	{
		// 080401

		// 心情不能出现在称号的位置
//		if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() == _TITLE::MOOD_TITLE )
//		{
//			state->PushString("");
//			return 1;
//		}
//
//		state->PushString( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle());
		
		return 0;
	}
	
	// 右键菜单相关
	INT PlayerMySelf::ShowMySelfContexMenu(LuaPlus::LuaState* state)
	{
		INT myId = CObjectManager::GetMe()->GetMySelf()->GetServerID();
		CGameInterface::GetMe()->Object_ShowContexMenu(myId, TRUE);
		return 0;
	}
	
	// 左键选中自己
	INT	PlayerMySelf::SelectMyselfAsTarget(LuaPlus::LuaState* state)
	{
		INT myId = CObjectManager::GetMe()->GetMySelf()->GetServerID();
		CGameInterface::GetMe()->Object_SelectAsMainTarget(myId, 1);
		return 0;
	}

	// 设置二级密码
	INT	PlayerMySelf::SetSupperPassword(LuaPlus::LuaState* state)
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup(FALSE))
		{
			// 如果设置过密码， 就修改密码。
			CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG);
		}
		else
		{
			// 如果设置过密码， 就设置密码。
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup();
		}
		return 0;
	}


	//----------------------------------------------------------------------------------------------------------
	// 设置密码数据

	INT PlayerMySelf::Lua_PasswordGetData( LuaPlus::LuaState* state )
	{
		INT nPasswordData = CDataPool::GetMe()->Rich_GetPasswordData();


		//已经绑定的装备
		if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_EQUIP)) ) > 0 )
			state->PushInteger( 1 );
		else
			state->PushInteger( 0 );

		//仓库
		if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_BANK)) ) > 0 )
			state->PushInteger( 1 );
		else
			state->PushInteger( 0 );

		//金币使用
		if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_MONEY)) ) > 0 )
			state->PushInteger( 1 );
		else
			state->PushInteger( 0 );

		//摆摊和购买
		if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_STALL)) ) > 0 )
			state->PushInteger( 1 );
		else
			state->PushInteger( 0 );

		//帮会
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


		INT nBindEquip	= args[2].GetInteger();	// 已经绑定的装备
		INT nDepot		= args[3].GetInteger();	// 仓库
		INT nGold		= args[4].GetInteger();	// 金币使用
		INT nStall		= args[5].GetInteger();	// 摆摊和购买
		INT nGuild		= args[6].GetInteger();	// 帮会
		
		INT nPasswordData = CDataPool::GetMe()->Rich_GetPasswordData();
		INT Original = nPasswordData;
		

		// 已经绑定的装备
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
		
		// 仓库
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

		// 金币使用
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

		// 摆摊和购买
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

		// 帮会
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

			pMinorPasswd->m_Type = MREQT_SETPROTECTTYPE;		// 设置二级密码保护类型
			pMinorPasswd->m_nFlag = nPasswordData;

			CNetManager::GetMe()->SendPacket( &Msg );
		}
	
		return 0;
	}

	// 获取保护类型状态
	INT PlayerMySelf::Lua_PasswordTypeGetState( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("Lua_SavePasswordSetup[1] param parameter error");
		}

		INT nType = args[2].GetInteger();	// 保护的类型

		if( nType < 0 || nType >= 5 )
		{
			state->PushInteger( -1 );
			return 1;
		}

		INT nPasswordData = CDataPool::GetMe()->Rich_GetPasswordData();

		switch( nType )
		{
		// 已经绑定的装备
		case 0:
			{
				if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_EQUIP)) ) > 0 )
					state->PushInteger( 1 );
				else
					state->PushInteger( 0 );

				break;
			};
		// 仓库
		case 1:
			{
				if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_BANK)) ) > 0 )
					state->PushInteger( 1 );
				else
					state->PushInteger( 0 );

				break;
			}
		// 金币使用
		case 2:
			{
				if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_MONEY)) ) > 0 )
					state->PushInteger( 1 );
				else
					state->PushInteger( 0 );

				break;
			}
		// 摆摊和购买
		case 3:
			{
				if( ( nPasswordData & ( 1 << (MINORPASSWD_INFO::PROTECT_STALL)) ) > 0 )
					state->PushInteger( 1 );
				else
					state->PushInteger( 0 );

				break;
			}
		// 帮会
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



	// 保护事件处理
	VOID PlayerMySelf::ProtectEventHandling( INT nType )
	{
		// 根据不同的类型来处理不同的事件
		// 将脚本中调用事件的地方统一起来放在一个方法中， 供此处调用
		switch( nType )
		{
		// 已经绑定的装备
		// MINORPASSWD_INFO::PROTECT_EQUIP
		case 0:
			{
				// 
				CEventSystem::GetMe()->PushEvent( GE_QUEST_INPUT_PASSWD_COMMAND, m_sQuestScript.c_str(), "", m_nQuestIndex );

				break;
			}
		// 仓库
		// MINORPASSWD_INFO::PROTECT_BANK
		case 1:
			{
//				Bank::OpenInputFrame();

				// 发送消息
				const char* password = CDataPool::GetMe()->Rich_GetInputPassword().c_str();
				int len = (int)strlen( password );
				
				CGBankAcquireList Msg;
				Msg.SetPassWord( password );
				Msg.SetPwdSize( len );
				
				CNetManager::GetMe()->SendPacket(&Msg);

				break;
			}
		// 金币使用	MINORPASSWD_INFO::PROTECT_MONEY
		case 2:
			{
				break;
			}
		// 摆摊和购买
		// MINORPASSWD_INFO::PROTECT_STALL
		case 3:
			{
				CEventSystem::GetMe()->PushEvent( GE_PACKET_OPEN_SALE );

				break;
			}
		// 帮会操作
		// MINORPASSWD_INFO::PROTECT_GUILD
		case 4:
			{
				// 调用脚本发送请求帮会的信息
				CEventSystem::GetMe()->PushEvent( GE_INPUT_PASSWD_COMMAND );	// 通过输入密码触发
				break;
			}
		};
	}


	// 密码检测流程
	INT PlayerMySelf::Lua_CheckPasswdProcedure( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("Lua_CheckPasswdProcedure[1] param parameter error");
			return 0;
		}

		// 类型
		INT nType =  args[2].GetInteger();

		// 获取该类型是否已经校验过密码了
		BOOL bState = CDataPool::GetMe()->Rich_GetInputPasswdState( nType );	

		// 需要在 服务器传过来的检测密码消息中调用 Rich_SetInputPasswdState 来标志是否已检测了


		if( !bState )
		{
			// 对装备的操作需要保存数据
			if( 0 == nType )
			{
				m_nQuestIndex	= args[3].GetInteger();
				m_sQuestScript	= args[4].GetString();
			}

			// 显示密码输入
			CEventSystem::GetMe()->PushEvent( GE_RICH_CHECK_PASSWORD, nType );
		}
		// 已经检验过了， 不再显示密码输入
		else
		{
			ProtectEventHandling( nType );
		}

		return 0;
	}

	// 处理输入密码后的流程
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

		// 类型
		INT nType =  args[2].GetInteger();
		// 密码
		STRING pInputPassword = args[3].GetString();
		
		// 保存输入的密码到数据池中
		CDataPool::GetMe()->Rich_SetInputPassword( pInputPassword );	// 只保存一个就可以了， 因为所有的密码都一样
		
		ProtectEventHandling( nType );

		return 0;
	}


}	// namespace