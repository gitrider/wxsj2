
/**	GMInterface_Script_Guild.cpp
 *	
 *	功能：	帮会相关的操作
 *	
 *	修改记录：
 *			080303	增加 邀请入帮 INT Guild::GuildInvite(LuaPlus::LuaState* state) 
 *					修改 FixGuildInfo， 实现修改帮会描述
 *			080305	DestoryFamily
 *					AgreeInvite
 *		`	080307	CharacterCheck
 *					CreateFamily
 *					CreateFamilyConfirm
 *					AdjustFamilyMember
 *					TransferMemberToFamily
 *					KickMemberFormFamily
 *					GetFamilyMembersInfo 获得家族的成员详细信息
 *			080314	修改 OpenMenu 
 *			080320	修改创建帮会流程， 先判断金钱
 *			080327	修改 AdjustMemberTitle 中CGCharUpdateCurTitle消息里的设置称号的部分
 *			
 */

#include "StdAfx.h"
#include "GIException.h"
#include "GITimeSystem.h"
#include "GIEventDefine.h"
#include "GIUISystem.h"
#include "GIUtil.h"
#include "LuaPlus.h"
#include "DB_Struct.h"
#include "GameDefine_Guild.h"
#include "GameStruct_Guild.h"
#include "Network\NetManager.h"
#include "DBC/GMDataBase.h"
#include "DataPool/GMDataPool.h"
#include "DataPool/GMDP_Struct_Guild.h"
#include "DataPool/GMDP_CharacterData.h"
#include "Object/Logic/Character/Obj_Character.h"
#include "Object/Logic/Character/Obj_PlayerNPC.h"
#include "Object/Logic/Character/Obj_PlayerMySelf.h"
#include "Object/Manager/ObjectManager.h"
#include "Event/GMEventSystem.h"
#include "Procedure/GameProcedure.h"

#include "CGGuild.h"
#include "CGGuildApply.h"
#include "CGGuildJoin.h"
#include "CGCharUpdateCurTitle.h"

#include "GMInterface_Script_Talk.h"
#include "GMInterface_Script_Guild.h"
#include "Input\GMInputSystem.h"




namespace SCRIPT_SANDBOX
{
	Guild Guild::s_Guild;
	LuaPlus::LuaObject* Guild::s_pMetaTable = NULL;

	enum
	{
		ERR_GUILD_ALREADYIN_MSG = 0,	//玩家已经在一个帮会中
		ERR_GUILD_NOTIN_MSG,			//玩家不在帮会中
		ERR_GUILD_NOPOW_MSG,			//你的权限不够
		ERR_GUILD_NOTHAVEASSCHIEF_MSG,	//没有副帮主

		ERR_GUILD_CREATE_LEVEL_TOO_LOW,	//创建等级过低
		ERR_GUILD_NAME_EMPTY,			//帮会名为空
		ERR_GUILD_NAME_INVALID,			//名字里含有敏感词
		ERR_GUILD_NAME_CANTUSE,			//名字是完全过滤表中的词
		ERR_GUILD_DESC_EMPTY,			//描述为空
		ERR_GUILD_DESC_INVALID,			//描述里含有敏感词
		ERR_GUILD_MONEY_NOT_ENOUGH,		//建立帮会金钱不足

		ERR_GUILD_JOIN_LEVEL_TOO_LOW,	//加入等级过低
		ERR_GUILD_POW_NORECRUIT,		//没有权限收人
		ERR_GUILD_POW_NOEXPEL,			//没有权限踢人
	};

	Guild::Guild()
	{
		m_MsgArray.resize(0);

		m_iMemberBak	= INVALID_ID;
		m_iFamilyNum	= 0;
		m_iCurFamilyNum	= INVALID_ID;
		m_iMemberSelect	= INVALID_ID;
	}

	Guild::~Guild()
	{
	}

	INT Guild::CreateGuild(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Guild CreateGuild Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Guild CreateGuild Wrong Param2");
			return -1;
		}

		STRING szGuildName = args[2].GetString();
		STRING szGuildDesc = args[3].GetString();

		if(!szGuildName.empty() && !szGuildDesc.empty())
		{
			//帮会创建资格检查
			if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level() < 20 )	// 级别小于20
			{
				ShowMsg(ERR_GUILD_CREATE_LEVEL_TOO_LOW);
				state->PushInteger(-1);
				return 1;
			}

			// 钱少了也不让建 555555555555
			if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() < 500000 )
			{
				ShowMsg(ERR_GUILD_MONEY_NOT_ENOUGH);
				state->PushInteger(-1);
				return 1;
			}

			if( INVALID_ID == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
			{

				if( !CharacterCheck( szGuildName ) )
				{
					ShowMsg(ERR_GUILD_NAME_INVALID);
					state->PushInteger(-1);
					return 1;
				}

				//敏感字符过滤
				if( !CGameProcedure::s_pUISystem->CheckStringFilter( szGuildDesc, FT_NAME ) )
				{
					ShowMsg(ERR_GUILD_DESC_INVALID);
					state->PushInteger(-1);
					return 1;
				}


				m_MsgArray.push_back(szGuildName);
				m_MsgArray.push_back(szGuildDesc);

				//检测通过
				state->PushInteger(1);
				//显示确认框
				CEventSystem::GetMe()->PushEvent( GE_GUILD_CREATE_CONFIRM, szGuildName.c_str() );
			}
			else
			{
				//已经拥有一个帮会MSG
				ShowMsg(ERR_GUILD_ALREADYIN_MSG);
				state->PushInteger(-1);
			}
		}
		else
		{
			if(szGuildName.empty())
			{
				ShowMsg(ERR_GUILD_NAME_EMPTY);
				state->PushInteger(-1);
			}

			if(szGuildDesc.empty())
			{
				ShowMsg(ERR_GUILD_DESC_EMPTY);
				state->PushInteger(-1);
			}
		}

		return 1;
	}

	// 创建家族
	INT	Guild::CreateFamily(LuaPlus::LuaState* state)
	{
		// 家族创建资格检查
		if( -1 == m_iMemberSelect )
		{
			CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "还没选择角色创建家族 ！！" );
			return 0;
		}


		// 是否加入了帮会
		INT nGuildId = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
		if( INVALID_ID != nGuildId )
		{
			// 客户端玩家权限检查
			if( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN )
			{

				// 成员小于3人无法建立
				if( m_AllMembers.size() >= 3 )
				{
					// 显示创建家族对话框
					CEventSystem::GetMe()->PushEvent( GE_GUILD_CREATE_FAMILY, m_AllMembers[m_iMemberSelect] );	// 通过选择的索引获取家族长名字
				}
				else
				{
					CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "帮会成员不足3人， 无法创建家族 ！！" );
					return 1;
				}
			}
			else
			{
				//权限不够
				ShowMsg( ERR_GUILD_NOPOW_MSG );
				return 1;
			}
		}
		else
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "你还未加入帮会 ！！" );
			return 1;
		}

		return 1;
	}


	INT Guild::CreateGuildConfirm(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild CreateGuildConfirm Wrong Param1");
			return -1;
		}

		INT confirmId = args[2].GetInteger();
		if(1 == confirmId && m_MsgArray.size() == 2) //create
		{

			//发送创建帮会信息包
			CGGuildApply pk;

			pk.SetGuildNameSize((BYTE)m_MsgArray[0].size());
			pk.SetGuildName((CHAR*)m_MsgArray[0].c_str());		// 
			pk.SetGuildDescSize((BYTE)m_MsgArray[1].size());
			pk.SetGuildDesc((CHAR*)m_MsgArray[1].c_str());

			CNetManager::GetMe()->SendPacket(&pk);
		}
		else if(confirmId == 2)	//destory
		{
			//发送帮会删除包
			CGGuild ck;
			ck.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_DISMISS;

			CNetManager::GetMe()->SendPacket(&ck);
		}
		else if(confirmId == 3) //quit
		{
			//发送退出帮会包
			CGGuild dk;
			
			dk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_LEAVE;
			GUILD_CGW_LEAVE* pLeave = (GUILD_CGW_LEAVE*)dk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_LEAVE);

			if(pLeave)
			{
				CNetManager::GetMe()->SendPacket(&dk);
			}
		}

		m_MsgArray.clear();
		return 0;
	}

	// 确认创建家族 
	INT Guild::CreateFamilyConfirm(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsString()) )
		{
			KLThrow("LUA: Guild CreateFamily Wrong Param1");
			return -1;
		}

		if( !(args[3].IsString()) )
		{
			KLThrow("LUA: Guild CreateFamily Wrong Param2");
			return -1;
		}

		STRING szFamilyName = args[2].GetString();
		STRING szFamilyDesc = args[3].GetString();

		if( !szFamilyName.empty() && !szFamilyDesc.empty() )
		{
			if( !CharacterCheck( szFamilyName ) )
			{
				ShowMsg(ERR_GUILD_NAME_INVALID);
				return 1;
			}

			//敏感字符过滤
			if( !CGameProcedure::s_pUISystem->CheckStringFilter( szFamilyDesc, FT_NAME ) )
			{
				ShowMsg(ERR_GUILD_DESC_INVALID);
				return 1;
			}


			// 发送创建家族的消息

			CGGuild pk;
			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_CREATE_FAMILY;

			GUILD_CGW_CREATE_FAMILY* pCreateFamily = (GUILD_CGW_CREATE_FAMILY*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_CREATE_FAMILY );
			if( pCreateFamily )
			{
				pCreateFamily->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();

				pCreateFamily->m_ShaikhIndex = m_AllMembers[m_iMemberSelect];
				pCreateFamily->m_ShaikhGuid = CDataPool::GetMe()->Guild_GetMemInfoByIndex(m_AllMembers[m_iMemberSelect])->m_Guid;

				pCreateFamily->m_cDescSize = (UCHAR)strlen( szFamilyDesc.c_str() );
				strncpy( pCreateFamily->m_szFamilyDesc, szFamilyDesc.c_str(), MAX_GUILD_FAMILY_DESC_SIZE * sizeof(CHAR) );
				pCreateFamily->m_szFamilyDesc[MAX_GUILD_FAMILY_DESC_SIZE] = 0 ;

				pCreateFamily->m_cNameSize = (UCHAR)strlen( szFamilyName.c_str() );
				strncpy( pCreateFamily->m_szFamilyName, szFamilyName.c_str(), MAX_GUILD_FAMILY_NAME_SIZE * sizeof(CHAR) );
				pCreateFamily->m_szFamilyName[MAX_GUILD_FAMILY_NAME_SIZE] = 0 ;


				CNetManager::GetMe()->SendPacket( &pk );
			}

		}

		return 0;
	}


	// 向World申请帮会详细信息
	INT Guild::AskGuildDetailInfo(LuaPlus::LuaState* state)
	{
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			CGGuild pk;

			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ASKINFO;

			GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_ASKINFO);
			if(pAskInfo)
			{
				pAskInfo->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
				pAskInfo->m_Type = GUILD_CGW_ASKINFO::GUILD_INFO;

				STRING pInputPassword = CDataPool::GetMe()->Rich_GetInputPassword( );

				pAskInfo->m_cPwdSize = (UCHAR)strlen( pInputPassword.c_str() );
				strncpy( pAskInfo->m_Password, pInputPassword.c_str(), MAX_PWD * sizeof(CHAR) );
				pAskInfo->m_Password[MAX_PWD] = 0 ;

				CNetManager::GetMe()->SendPacket(&pk);
			}
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}

		return 0;
	}

	// 向World申请帮会成员信息
	INT Guild::AskGuildMembersInfo(LuaPlus::LuaState* state)
	{
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			CGGuild pk;
			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ASKINFO;

			GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_ASKINFO);
			if(pAskInfo)
			{
				pAskInfo->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
				pAskInfo->m_Type = GUILD_CGW_ASKINFO::GUILD_MEMBER_INFO;
				
				STRING pInputPassword = CDataPool::GetMe()->Rich_GetInputPassword( );

				pAskInfo->m_cPwdSize = (UCHAR)strlen( pInputPassword.c_str() );
				strncpy( pAskInfo->m_Password, pInputPassword.c_str(), MAX_PWD * sizeof(CHAR) );
				pAskInfo->m_Password[MAX_PWD] = 0 ;

				CNetManager::GetMe()->SendPacket( &pk );
			}
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);

			CDataPool::GetMe()->Guild_ClearMemInfo();

			// 显示空界面
//			CEventSystem::GetMe()->PushEvent( GE_GUILD_SHOW_MEMBERINFO );
			CEventSystem::GetMe()->PushEvent( GE_UNITE_GF_SHOW, 1 );
		}

		return 0;
	}

	// 向World申请帮会职位信息
	INT Guild::AskGuildAppointPosInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild AskGuildAppointPosInfo Wrong Param1");
			return -1;
		}
		

		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			// 客户端玩家权限检查等
			if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_ASSIGN )	// 职务调动权
			{
				CGGuild pk;
				pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ASKINFO;

				GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_ASKINFO);
				if(pAskInfo)
				{
					pAskInfo->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
					pAskInfo->m_Type = GUILD_CGW_ASKINFO::GUILD_APPOINT_POS;	// 帮会中可任命的职位

					STRING pInputPassword = CDataPool::GetMe()->Rich_GetInputPassword( );

					pAskInfo->m_cPwdSize = (UCHAR)strlen( pInputPassword.c_str() );
					strncpy( pAskInfo->m_Password, pInputPassword.c_str(), MAX_PWD * sizeof(CHAR) );
					pAskInfo->m_Password[MAX_PWD] = 0 ;

					CNetManager::GetMe()->SendPacket(&pk);
				}

				m_iMemberBak = args[2].GetInteger();
			}
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}
		return 0;
	}

	// 帮会让位给别人
	INT Guild::ChangeGuildLeader(LuaPlus::LuaState* state)
	{
		return 0;
	}

	// 帮会删除
	INT Guild::DestoryGuild(LuaPlus::LuaState* state)
	{
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			//客户端玩家权限检查等
			if(CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN)
			{
				CEventSystem::GetMe()->PushEvent(GE_GUILD_DESTORY_CONFIRM, CDataPool::GetMe()->Guild_GetName());
			}
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}
		return 0;		
	}

	// 修改帮会信息（宗旨）
	INT Guild::FixGuildInfo( LuaPlus::LuaState* state )
	{
		LuaStack args(state);

		if( !(args[2].IsString()) )
		{
			KLThrow("LUA: Guild FixGuildInfo Wrong Param2");
			return -1;
		}
		if( !(args[3].IsString()) )
		{
			KLThrow("LUA: Guild FixGuildInfo Wrong Param3");
			return -1;
		}

		STRING strOp2 = args[2].GetString();
		STRING strOp3 = args[3].GetString();

		if( strOp2.empty() || strOp3.empty() ) 
			return 0;


		// 是否有帮会
		INT nGuild = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
		if( INVALID_ID != nGuild )
		{
			// 只有帮主可以修改
			if( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN )
			{
				if( strOp2 == "Desc" )
				{
					CGGuild pk;
					pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_MODIFY_DESC;	//

					GUILD_CGW_MODIFY_DESC* pModifyDesc = (GUILD_CGW_MODIFY_DESC*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_MODIFY_DESC );
					if( pModifyDesc )
					{
						pModifyDesc->m_DescSize = (UCHAR)strlen( strOp3.c_str() );

						strncpy( pModifyDesc->m_szGuildDesc, strOp3.c_str(), MAX_GUILD_DESC_SIZE * sizeof(CHAR) );
						pModifyDesc->m_szGuildDesc[MAX_GUILD_DESC_SIZE] = 0 ;

						CNetManager::GetMe()->SendPacket( &pk );
					}

				}
				
			}
		}

		return 1;
	}


	// 修改现有帮众职位
	INT Guild::AdjustMemberAuth(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			// 改变职位时的备份是要修改哪个玩家的职位信息
			m_iMemberBak = -1;
			KLThrow("LUA: Guild AdjustMemberAuth Wrong Param1");
			return -1;
		}

		INT lidx = args[2].GetInteger();

		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			// 客户端玩家权限检查等
			//是否是帮主
			if( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN )
//			if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_ASSIGN )	// 职务调动权
			{
				if( lidx >= 0 && CDataPool::GetMe()->Guild_GetAppointInfoByIndex(lidx) && m_iMemberBak >= 0 )
				{
					// 获得帮派可任命职位
					GuildAppointPos_t* pAppoint = CDataPool::GetMe()->Guild_GetAppointInfoByIndex(lidx);

					CGGuild pk;
					pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_APPOINT;	// 帮会任免

					GUILD_CGW_APPOINT* pAuth = (GUILD_CGW_APPOINT*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_APPOINT);

					if( pAuth && pAppoint && CDataPool::GetMe()->Guild_GetMemInfoByIndex(m_iMemberBak) )
					{
						// 发送职位变更包
						pAuth->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();		// 获取帮会ID
						pAuth->m_CandidateGUID = CDataPool::GetMe()->Guild_GetMemInfoByIndex(m_iMemberBak)->m_Guid;		// 通过索引获得帮众信息
						pAuth->m_NewPosition = (UCHAR)pAppoint->m_PosID;

						CNetManager::GetMe()->SendPacket(&pk);
					}
				}
			}
			else
			{
				//权限不够
				ShowMsg(ERR_GUILD_NOPOW_MSG);
			}
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}

		m_iMemberBak = -1;
		return 0;
	}

	// 修改帮会成员的称号
	INT Guild::AdjustMemberTitle(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild AdjustMemberAuth Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Guild AdjustMemberAuth Wrong Param2");
			return -1;
		}

		INT idx			= args[2].GetInteger();
		STRING sTitle	= args[3].GetString();


		// 检测输入的称号合法性
		if( !CharacterCheck( sTitle ) )
		{
			ShowMsg( ERR_GUILD_NAME_INVALID );
			return 1;
		}


		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			// 根据列表中选择的成员
			// 发送的参数： 帮会的ID、要修改的成员ID、称号
			
			// 还没确定怎么从列表中获取信息（索引 或 名称）


			// 根据索引获取成员数据
			GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);

			if( pMember && pMember->m_Guid != INVALID_ID )
			{
				// 权限判断
				if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_AUTHORIZE )	// 权限调整权 应该够了
				{

					// 更新称号
					CGCharUpdateCurTitle Msg;


					Msg.setTitleType( _TITLE::BANGPAI_TITLE );	// 帮派称号
//					Msg.setTitleData( 0 );
//					Msg.setTitle( sTitle.c_str() );
					Msg.setTitleID( CDataPool::GetMe()->Title_GetCurTitle( _TITLE::BANGPAI_TITLE ) );
					Msg.setGuid( pMember->m_Guid );


					Msg.setTitleSize( (UCHAR)strlen( sTitle.c_str() ) );
					Msg.setTitleName( sTitle.c_str() );

					CNetManager::GetMe()->SendPacket( &Msg );

				}
				else
					//权限不够
					ShowMsg( ERR_GUILD_POW_NORECRUIT );

			}
		}
		else
		{
			// 不在帮会MSG
			ShowMsg( ERR_GUILD_NOTIN_MSG );
		}

		return 0;
	}

	// 家族成员的信息
	INT	Guild::GetFamilyMembersInfo(LuaPlus::LuaState* state)
	{
		return 0;
	}

	//家族删除
	INT	Guild::DestoryFamilyConfirm(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild DestoryFamily Wrong Param1");
			return -1;
		}

		INT familyIdx = args[2].GetInteger();

		INT nFamilyNum = CDataPool::GetMe()->Guild_GetFamilyNum();
		if( familyIdx > nFamilyNum )
		{
			return 0;
		}

		STRING strFamilyName = "";

		GuildFamilyInfo* familyInfo = CDataPool::GetMe()->Guild_GetFamilyInfoByIndex( familyIdx );
		if( familyInfo )
		{
			strFamilyName = familyInfo->m_sFamilyName;
		}


		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			//客户端玩家权限检查等
			if(CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN)
			{
				// 保存选择的家族
				CDataPool::GetMe()->Guild_SetSelectFamilyId( familyIdx );

				CEventSystem::GetMe()->PushEvent( GE_GUILD_DESTORY_FAMILY_CONFIRM, CDataPool::GetMe()->Guild_GetName(), strFamilyName.c_str() );
			}
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}

		return 0;
	}
	INT	Guild::DestoryFamily(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		INT familyIdx = CDataPool::GetMe()->Guild_GetSelectFamilyId( );

		INT nFamilyNum = CDataPool::GetMe()->Guild_GetFamilyNum();
		if( familyIdx > nFamilyNum )
		{
			return 0;
		}


		GuildDetailInfo_t* guildInfo = CDataPool::GetMe()->Guild_GetDetailInfo();
		if( guildInfo )
		{
		}


		CGGuild pk;
		pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_FAMILY_DISMISS;	// 删除家族

		GUILD_CGW_FAMILY_DISMISS* pDestoryFamily = (GUILD_CGW_FAMILY_DISMISS*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_FAMILY_DISMISS );
		if( pDestoryFamily )
		{
			pDestoryFamily->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();;
			pDestoryFamily->m_nFamilyID	= familyIdx;

			CNetManager::GetMe()->SendPacket( &pk );
		}

		return 0;
	}

	// 添加或删除家族成员
	INT Guild::AdjustFamilyMember(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsString()) )
		{
			KLThrow("LUA: Guild CreateFamily Wrong Param1");
			return -1;
		}

		if( !(args[3].IsString()) )
		{
			KLThrow("LUA: Guild CreateFamily Wrong Param2");
			return -1;
		}

/*
		// 添加
		if(  )
		{
			// 显示确认对话框
		}
		// 删除
		else 
		{
			// 显示删除对话框
		}
*/

		return 0;
	}

	// 移动成员到家族
	INT Guild::TransferMemberToFamily(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild TransferMemberToFamily Wrong Param1");
			return -1;
		}

		if(!(args[3].IsInteger()))
		{
			KLThrow("LUA: Guild TransferMemberToFamily Wrong Param2");
			return -1;
		}


		INT familyIdx	= args[2].GetInteger();
		INT idx			= args[3].GetInteger();

		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);

		if( pMember && pMember->m_Guid != INVALID_ID )
		{
			// 发送家族添加成员消息
			CGGuild pk;
			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ADD_FAMILY_USER;

			GUILD_CGW_ADD_FAMILY_USER* pAddFamily = (GUILD_CGW_ADD_FAMILY_USER*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_ADD_FAMILY_USER );
			if( pAddFamily )
			{
				pAddFamily->m_cFamilyIndex = familyIdx;			// 家族id
				pAddFamily->m_iGuildIndex  = idx;				// 要添加的成员在帮会成员列表中的索引
				pAddFamily->m_TargetGuid   = pMember->m_Guid;	// 要添加的成员的 server id

				CNetManager::GetMe()->SendPacket( &pk );
			}
		}

		return 0;
	}

	// 从家族中踢出
	INT Guild::KickMemberFormFamily(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: Guild KickMemberFormFamily Wrong Param1");
			return -1;
		}

		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: Guild KickMemberFormFamily Wrong Param2");
			return -1;
		}

		INT familyIdx	= args[2].GetInteger();
		INT idx			= args[3].GetInteger();

		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);

		if( pMember && pMember->m_Guid != INVALID_ID )
		{
			// 发送家族删除成员消息
			CGGuild pk;
			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_REMOVE_FAMILY_USER;

			GUILD_CGW_REMOVE_FAMILY_USER* pRemoveFamily = (GUILD_CGW_REMOVE_FAMILY_USER*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_REMOVE_FAMILY_USER );
			if( pRemoveFamily )
			{
				pRemoveFamily->m_cFamilyIndex = familyIdx;
				pRemoveFamily->m_iGuildIndex  = idx;
				pRemoveFamily->m_TargetGuid   = pMember->m_Guid;

				CNetManager::GetMe()->SendPacket( &pk );
			}
		}

		return 0;
	}

	//家族邀请
	INT Guild::InviteMemberToFamily(LuaPlus::LuaState* state)
	{
		return 0;
	}

	//家族邀请确认
	INT Guild::FamilyInviteConfirm(LuaPlus::LuaState* state)
	{
		return 0;
	}

	INT Guild::GetFamilyMemberNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetFamilyMemberIndex Wrong Param1");
			return -1;
		}

		INT idx = args[2].GetInteger();

		INT nFamilyNum = CDataPool::GetMe()->Guild_GetFamilyNum();

		if( idx < 0 || idx > nFamilyNum )
			return 0;


		GuildFamilyInfo* familyInfo = CDataPool::GetMe()->Guild_GetFamilyInfoByIndex( idx );

		if( familyInfo )
		{
			state->PushInteger( familyInfo->m_iFamilyUserNum );
		}

		return 1;
	}

	//获取家族成员在成员列表中的索引
	INT Guild::GetFamilyMemberIndex(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetFamilyMemberIndex Wrong Param1");
			return -1;
		}

		if(!(args[3].IsInteger()))
		{
			KLThrow("LUA: Guild GetFamilyMemberIndex Wrong Param2");
			return -1;
		}


		INT idx			= args[2].GetInteger();
		INT nFamilyIdx	= args[3].GetInteger();

		INT nFamilyNum = CDataPool::GetMe()->Guild_GetFamilyNum();

		if( nFamilyIdx < 0 || nFamilyIdx > nFamilyNum )
			return 0;

		GuildFamilyInfo* familyInfo = CDataPool::GetMe()->Guild_GetFamilyInfoByIndex( nFamilyIdx );

		if( familyInfo )
		{
			if( idx >= 0 && idx < familyInfo->m_iFamilyUserNum )
			{
				state->PushInteger( familyInfo->m_aFamilyUser[idx] );
			}
		}

		return 1;
	}


	INT Guild::SetLuaFamilyNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild SetLuaFamilyNum Wrong Param1");
			return -1;
		}

		m_iCurFamilyNum = args[2].GetInteger();

		return 0;
	}
	INT Guild::GetLuaFamilyNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( INVALID_ID == m_iCurFamilyNum )
			m_iCurFamilyNum = 0;	// CDataPool::GetMe()->Guild_GetFamilyNum();

		state->PushInteger( m_iCurFamilyNum );

		return 1;
	}

	// 保存创建的家族窗口数据
	INT Guild::SetFamilyWindowData(LuaPlus::LuaState* state)
	{
		// 索引
		// 名称
		// 位置

		INT		index = 0;
		STRING	name = "";

		CDataPool::GetMe()->Guild_AddFamilyWindowData( index, name );
		
		

		return 0;
	}
	// 
	INT Guild::GetFamilyWindowData(LuaPlus::LuaState* state)
	{
		INT		index = 0;
		STRING	name = "";

		name = CDataPool::GetMe()->Guild_GetFamilyWindowData( index );

		state->PushString( name.c_str() );

		return 1;
	}
	// 删除保存的指定数据
	INT Guild::DelFamilyWindowData(LuaPlus::LuaState* state)
	{
		INT		index = 0;

		CDataPool::GetMe()->Guild_DelFamilyWindowData( index );

		return 0;
	}



	INT Guild::GetGuildInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetGuildInfo Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Guild GetGuildInfo Wrong Param2");
			return -1;
		}

		INT idx = args[2].GetInteger();
		STRING strOp = args[3].GetString();

		if( idx >= CDataPool::GetMe()->Guild_GetInfoNum() ) 
			return 0;

		if( strOp.empty() ) 
			return 0;


		// 帮会信息
		GuildInfo_t* pGuild = CDataPool::GetMe()->Guild_GetInfoByIndex(idx);

		if(pGuild && pGuild->m_GuildID != INVALID_ID)
		{
			if(strOp == "ID")
			{
				state->PushInteger(pGuild->m_GuildID);
			}
			else if(strOp == "ChiefName")
			{
				state->PushString(pGuild->m_ChiefName.c_str());
			}
			else if(strOp == "Name")
			{
				state->PushString(pGuild->m_GuildName.c_str());
			}
			else if(strOp == "Desc")
			{
				state->PushString(pGuild->m_GuildDesc.c_str());
			}
			else if(strOp == "CityName")
			{
				state->PushString(pGuild->m_CityName.c_str());
			}
			else if(strOp == "Status")
			{
				state->PushInteger((INT)pGuild->m_uGuildStatus);
			}
			else if(strOp == "Level")
			{
				state->PushInteger((INT)pGuild->m_bGuildLevel);
			}
			else if(strOp == "FoundTime")
			{
				STRING strTime;
				ConvertServerTime(pGuild->m_nFoundedTime, strTime);

				state->PushString(strTime.c_str());
			}
			else if(strOp == "Count")
			{
				state->PushInteger((INT)pGuild->m_uGuildUserCount);
			}
		}
		else
		{
			if(strOp == "ID")
			{
				state->PushInteger(0);
			}
			else if(strOp == "ChiefName")
			{
				state->PushString("");
			}
			else if(strOp == "Name")
			{
				state->PushString("");
			}
			else if(strOp == "Desc")
			{
				state->PushString("");
			}
			else if(strOp == "CityName")
			{
				state->PushString("");
			}
			else if(strOp == "Status")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Level")
			{
				state->PushInteger(0);
			}
			else if(strOp == "FoundTime")
			{
				state->PushString("");
			}
			else if(strOp == "Count")
			{
				state->PushInteger(0);
			}
		}

		return 1;
	}

	INT Guild::GetGuildNum(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->Guild_GetInfoNum());
		return 1;
	}

	INT Guild::GetMembersInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetMembersInfo Wrong Param1");
			return -1;
		}
		if(!(args[3].IsString()))
		{
			KLThrow("LUA: Guild GetMembersInfo Wrong Param2");
			return -1;
		}

		INT idx = args[2].GetInteger();
		STRING strOp = args[3].GetString();

		// 帮会成员信息
		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);
		
		if( pMember && strOp == "GUID" )
		{
			state->PushInteger( (INT)pMember->m_Guid );
		}
		else if( pMember && pMember->m_Guid != INVALID_ID )
		{
			if(strOp == "Name")
			{
				state->PushString(pMember->m_szName.c_str());
			}
			else if(strOp == "Level")
			{
				state->PushInteger(pMember->m_bLevel);
			}
			else if(strOp == "MenPai")
			{
				state->PushInteger(pMember->m_bMenPaiID);
			}
			else if(strOp == "CurCon")
			{
				state->PushInteger(pMember->m_iCurContribute);
			}
			else if(strOp == "MaxCon")
			{
				state->PushInteger(pMember->m_iMaxContribute);
			}
			else if(strOp == "JoinTime")
			{
				STRING strTime;
				ConvertServerTime(pMember->m_iJoinTime, strTime);

				state->PushString(strTime.c_str());
			}
			else if(strOp == "LogOutTime")
			{
				STRING strTime;
				ConvertServerTime(pMember->m_iLogOutTime, strTime);

				state->PushString(strTime.c_str());
			}
			else if(strOp == "Position")
			{
				state->PushInteger(pMember->m_bPosition);
			}
			else if(strOp == "Online")
			{
				state->PushInteger(pMember->m_bIsOnline);
			}
			else if(strOp == "ShowColor")
			{
				state->PushString(GetShowColor_For_Lua(idx).c_str());
			}
			else if(strOp == "Family")
			{
				// 所属家族
				GuildFamilyInfo* pFamily = CDataPool::GetMe()->Guild_GetFamilyInfoByIndex( pMember->m_iFamilyID );
				if( pFamily )
				{
					state->PushString( pFamily->m_sFamilyName.c_str() );
				}
				else
				{
					state->PushString( "无家族" );
				}
			}
			else if(strOp == "Title")
			{
				state->PushString( pMember->m_szTitleName.c_str() );
			}
			else if(strOp == "Sex")
			{
				state->PushInteger( pMember->m_iSex );
			}
		}

		return 1;
	}

	// 获得自己的帮派信息
	INT Guild::GetMyGuildInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Guild GetMyGuildInfo Wrong Param1");
			return -1;
		}
		STRING strOp = args[2].GetString();

		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			if(strOp == "Name")
			{
				state->PushString(CDataPool::GetMe()->Guild_GetName());
			}
			else if(strOp == "Desc")
			{
				state->PushString(CDataPool::GetMe()->Guild_GetDesc());
			}
			else if(strOp == "Appoint")
			{
				if(!(args[3].IsInteger()))
				{
					KLThrow("LUA: Guild GetMyGuildInfo Wrong Param2");
					return -1;
				}
				INT idx = args[3].GetInteger();

				GuildAppointPos_t* pAppoint = CDataPool::GetMe()->Guild_GetAppointInfoByIndex(idx);
				if(pAppoint && pAppoint->m_PosID != -1)
				{
					state->PushString(pAppoint->m_szPosName.c_str());
				}
			}
			else if(strOp == "Position")
			{
				INT nGuildPos = CDataPool::GetMe()->Guild_GetCurPosition();
				if(  nGuildPos >= 0 || nGuildPos < GUILD_POSITION_SIZE )
					state->PushInteger( nGuildPos );
				else
					state->PushInteger( -1 );
			}
		}
		else
		{
			if(strOp == "Name")
			{
				state->PushString("");
			}
			else if(strOp == "Desc")
			{
				state->PushString("还未创建帮会。");
			}
			else if(strOp == "Appoint")
			{
				state->PushString("");
			}
			else if(strOp == "Position")
			{
				state->PushInteger( -1 );
			}
		}

		return 1;
	}

	INT Guild::GetMyGuildDetailInfo(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		
		if(!(args[2].IsString()))
		{
			KLThrow("LUA: Guild GetMyGuildDetailInfo Wrong Param1");
			return -1;
		}

		STRING strOp = args[2].GetString();

		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			GuildDetailInfo_t* pDetail = CDataPool::GetMe()->Guild_GetDetailInfo();
			if( pDetail )
			{
				if(strOp == "Name")
				{
					state->PushString(pDetail->m_GuildName.c_str());
				}
				else if(strOp == "Creator")
				{
					state->PushString(pDetail->m_GuildCreator.c_str());
				}
				else if(strOp == "ChiefName")
				{
					state->PushString(pDetail->m_GuildChairMan.c_str());
				}
				else if(strOp == "CityName")
				{
					state->PushString(pDetail->m_CityName.c_str());
				}
				else if(strOp == "Level")
				{
					state->PushInteger((INT)pDetail->m_nLevel);
				}
				else if(strOp == "Scene")
				{
					state->PushInteger(pDetail->m_nPortSceneID);
				}
				else if(strOp == "MemNum")
				{
					state->PushInteger(pDetail->m_MemNum);
				}
				else if(strOp == "Lon")
				{
					state->PushInteger(pDetail->m_Longevity);
				}
				else if(strOp == "Con")
				{
					state->PushInteger(pDetail->m_Contribute);
				}
				else if(strOp == "Honor")
				{
					state->PushInteger(pDetail->m_Honor);
				}
				else if(strOp == "FMoney")
				{
					state->PushInteger(pDetail->m_FoundedMoney);
				}
				else if(strOp == "Ind")
				{
					state->PushInteger(pDetail->m_nIndustryLevel);
				}
				else if(strOp == "Agr")
				{
					state->PushInteger(pDetail->m_nAgrLevel);
				}
				else if(strOp == "Com")
				{
					state->PushInteger(pDetail->m_nComLevel);
				}
				else if(strOp == "Def")
				{
					state->PushInteger(pDetail->m_nDefLevel);
				}
				else if(strOp == "Tech")
				{
					state->PushInteger(pDetail->m_nTechLevel);
				}
				else if(strOp == "Ambi")
				{
					state->PushInteger(pDetail->m_nAmbiLevel);
				}
			}// end of if(pDetail)
		}
		else
		{
			if(strOp == "Name")
			{
				state->PushString("");
			}
			else if(strOp == "Creator")
			{
				state->PushString("");
			}
			else if(strOp == "ChiefName")
			{
				state->PushString("");
			}
			else if(strOp == "CityName")
			{
				state->PushString("");
			}
			else if(strOp == "Level")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Scene")
			{
				state->PushInteger(-1);
			}
			else if(strOp == "MemNum")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Lon")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Con")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Honor")
			{
				state->PushInteger(0);
			}
			else if(strOp == "FMoney")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Ind")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Agr")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Com")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Def")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Tech")
			{
				state->PushInteger(0);
			}
			else if(strOp == "Ambi")
			{
				state->PushInteger(0);
			}
		}

		return 1;
	}

	INT Guild::GetMyGuildPower(LuaPlus::LuaState* state)
	{
		const char* szOk = "1";
		const char* szFalse = "0";

		STRING strR = "";
		BYTE pow = CDataPool::GetMe()->Guild_GetCurAccess();
		
		strR += ( pow & GUILD_AUTHORITY_ASSIGN )	? szOk:szFalse;	// 职务调动权
		strR += ( pow & GUILD_AUTHORITY_AUTHORIZE )	? szOk:szFalse;	// 权限调整权
		strR += ( pow & GUILD_AUTHORITY_RECRUIT )	? szOk:szFalse;	// 接收帮众权
		strR += ( pow & GUILD_AUTHORITY_EXPEL )		? szOk:szFalse;	// 开除帮众权
		strR += ( pow & GUILD_AUTHORITY_DEMISE )	? szOk:szFalse;	// 禅让权
		strR += ( pow & GUILD_AUTHORITY_WITHDRAW )	? szOk:szFalse;	// 支取帮资权
		strR += ( pow & GUILD_AUTHORITY_DEPOSIT )	? szOk:szFalse;	// 存入金额权
		strR += ( pow & GUILD_AUTHORITY_LEAVE )		? szOk:szFalse;	// 离开帮会权
		strR += ( pow & GUILD_AUTHORITY_DIMISS )	? szOk:szFalse;	// 解散帮会权

        state->PushString(strR.c_str());

		return 1;
	}

	INT Guild::GetMembersNum(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetMembersNum Wrong Param1");
			return -1;
		}
		INT sel = args[2].GetInteger();

		enum
		{
			TOTAL_NUMBERS = 1,	//全部帮众，这个数值包含已经被踢出的原帮众的信息
			MAX_NUMBERS,		//最多帮众，不同帮会能包含的最大帮众数是不同的

			REAL_NUMBERS,		//真实帮众，真正的有效的数据，包含申请中的人和正式的人

			SHOW_MEMBERS_NUMBERS,	//Lua显示成员的list控件需要的数量
			SHOW_TRAINEES_NUMBERS,	//Lua显示预备的list控件需要的数量
			SHOW_FAMILY_NUMBERS,	
		};

		INT Num = 0;
		switch(sel) 
		{
		case TOTAL_NUMBERS:
			Num = CDataPool::GetMe()->Guild_GetMemInfoNum();
			break;
		case MAX_NUMBERS:
			Num = CDataPool::GetMe()->Guild_GetMaxMemNum();
			break;
		case REAL_NUMBERS:
			for(INT i = 0; i < CDataPool::GetMe()->Guild_GetMemInfoNum(); ++i)
			{
				GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(i);
				if(pMember && pMember->m_Guid != INVALID_ID &&
				   pMember->m_bIsOnline == 1 && pMember->m_bPosition > GUILD_POSITION_TRAINEE)	// 预备帮众，待批准
				Num++;
			}
			break;
		case SHOW_MEMBERS_NUMBERS:
			Num = (INT)m_AllMembers.size();
			break;
		case SHOW_TRAINEES_NUMBERS:
			Num = (INT)m_AllTrainees.size();
			break;
		// 帮会中家族的数量
		case SHOW_FAMILY_NUMBERS:
			Num = CDataPool::GetMe()->Guild_GetFamilyNum();
			break;

		default:
			Num = -1;
			break;
		};

		state->PushInteger(Num);
		return 1;
	}

	// Lua显示里list控件的位置
	INT Guild::GetShowMembersIdx(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetShowMembersIdx Wrong Param1");
			return -1;
		}
		INT showIdx = args[2].GetInteger();

		if( showIdx < 0 || showIdx >= (INT)( m_AllMembers.size() ) ) 
			return 0;

        state->PushInteger( m_AllMembers[showIdx] );
		return 1;
	}

	INT Guild::GetShowTraineesIdx(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild GetShowTraineesIdx Wrong Param1");
			return -1;
		}

		INT showIdx = args[2].GetInteger();

		if(showIdx < 0 || showIdx >= (INT)(m_AllTrainees.size())) return 0;

		state->PushInteger(m_AllTrainees[showIdx]);
		return 1;
	}

	// 加入帮会
	INT Guild::JoinGuild(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild JoinGuild Wrong Param1");
			return -1;
		}

		INT selGuild = args[2].GetInteger();
		
		// 已经加了就不能再申请了
		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "已经加入了帮会， 退出现有帮会才能申请 ！！" );
			return 0;
		}

		// 选择的有效性判断
		if( selGuild >= 0 && selGuild < CDataPool::GetMe()->Guild_GetInfoNum() )
		{
			GuildInfo_t* pGuild = CDataPool::GetMe()->Guild_GetInfoByIndex(selGuild);
			if( pGuild && pGuild->m_GuildID != INVALID_ID )
			{
				// 等级不小于10
				if( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level() < 10 )
				{
					ShowMsg(ERR_GUILD_JOIN_LEVEL_TOO_LOW);
					return 0;
				}

				INT nServerId = CObjectManager::GetMe()->GetMySelf()->GetServerID();

				//发送加入帮会的消息包
				CGGuildJoin pk;

				pk.SetGuildID( pGuild->m_GuildID );
				pk.SetObjectID( nServerId );

				CNetManager::GetMe()->SendPacket(&pk);
			}
		}
		return 0;
	}

	// 邀请入帮
	INT Guild::GuildInvite(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsString()) )
		{
			KLThrow("LUA: Guild GuildInvite Wrong Param1");
			return -1;
		}

		STRING strOp = args[2].GetString();


//		if( CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN )
//		{
//			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "你的权限不能邀请入帮。邀请失败 ！！" );
//			return 0;
//		}


		CObject* obj = NULL;

		// 目标玩家有效性判断
		{
			// 通过名字， 判断是否有对名字的操作
			if( !strOp.empty() )
			{
				obj = CObjectManager::GetMe()->FindCharacterByName( strOp );
			}
			// 获取主目标
			else
			{
				obj = (CObject*)CObjectManager::GetMe()->GetMainTarget();
			}

			if( !obj )
			{
				CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "所邀请的玩家不存在。邀请失败 ！！" );
				return 0;
			}
		}
		
		// 玩家等级判断
		if( ( (CObject_Character*)obj )->GetCharacterData()->Get_Level() < 10 )
		{
			if( "" != strOp )
			{
				// PlayerName等级不足，无法加入帮会
			}
			else
			{
				// 所邀请的玩家等级不足10级，邀请失败
			}

			CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "所邀请的玩家等级不够10级。邀请失败 ！！" );
			return 0;
		}


		if( strOp.empty() )
		{
			CObject_Character* obj_char = (CObject_Character*)(CObjectManager::GetMe()->GetMainTarget());
			strOp = obj_char->GetCharacterData()->Get_Name();
		}

		CGGuild pk;
		pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_INVITE;	// 邀请

		GUILD_CGW_INVITE* pInvite = (GUILD_CGW_INVITE*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_INVITE );
		if( pInvite )
		{
			strncpy( pInvite->m_szTargetName, strOp.c_str(), MAX_CHARACTER_NAME * sizeof(CHAR) );
			pInvite->m_szTargetName[MAX_CHARACTER_NAME] = 0 ;

			pInvite->m_NameSize = (UCHAR)strlen( strOp.c_str() );

			CNetManager::GetMe()->SendPacket( &pk );

			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "邀请已经发出， 等待玩家回复。" );
		}

		return 0;
	}

	// 显示帮会相关的对话框
	INT Guild::ShowInputDialog(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: Guild ShowInputDialog Wrong Param1");
			return -1;
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: Guild ShowInputDialog Wrong Param2");
			return -1;
		}

		INT nOp		= args[2].GetInteger();
		INT nMember = args[3].GetInteger();


		switch( nOp )
		{
		// 邀请加入帮会
		case 0:
			{
				// 权限判断
				if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_RECRUIT )
				{
					// 显示对话框
					CEventSystem::GetMe()->PushEvent( GE_GUILD_INVITE_OBJECT );
				}
				else
				{
					//权限不够
					ShowMsg( ERR_GUILD_POW_NORECRUIT );
				}
			}
			break;

		// 修改成员称号
		case 1:
			{
				GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex( nMember );
				

				// 权限判断
				if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_AUTHORIZE )	// 权限调整权 应该够了
				{
					if( pMember )
					{
						// 显示对话框
						CEventSystem::GetMe()->PushEvent( GE_GUILD_MODIFY_TITLE, pMember->m_szName.c_str(), "", nMember );
					}
					else
					{
						CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "成员不存在 ！！" );
					}
				}
				else
				{
					//权限不够
					ShowMsg( ERR_GUILD_POW_NORECRUIT );
				}
			}
			break;
		};


		return 0;
	}

	// 收到邀请入帮消息后， 点同意或不同意后的处理
	INT Guild::AgreeInvite(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: Guild AgreeInvite Wrong Param2");
			return -1;
		}

		BOOL bOp = args[2].GetInteger();

		// 提示信息
		if( bOp )
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "你同意了邀请。" );
		}
		else
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, "你拒绝了邀请！" );
		}

		// GUILD_PACKET_CG_INVITE

		CGGuild pk;
		pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_INVITE_CONFIRM;	// 确认邀请

		GUILD_CGW_INVITE_CONFIRM* pInviteConfirm = (GUILD_CGW_INVITE_CONFIRM*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_INVITE_CONFIRM );
		if( pInviteConfirm )
		{
			pInviteConfirm->m_GuildId	= CDataPool::GetMe()->Guild_GetInviteGuildId( );	// 
			pInviteConfirm->m_AgreeInvite= bOp;		// 


			STRING sInviteName = CDataPool::GetMe()->Guild_GetInviteGuildObjName();

			strncpy( pInviteConfirm->m_InvitorName, sInviteName.c_str(), MAX_CHARACTER_NAME * sizeof(CHAR) );
			pInviteConfirm->m_InvitorName[MAX_CHARACTER_NAME] = 0 ;

			pInviteConfirm->m_InvitorNameSize = (UCHAR)strlen( sInviteName.c_str() );


			CNetManager::GetMe()->SendPacket( &pk );
		}

		return 0;
	}


	// 踢出帮会,拒绝申请
	INT Guild::KickGuild(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild KickGuild Wrong Param1");
			return -1;
		}

		INT idx = args[2].GetInteger();

		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);

		if(pMember && pMember->m_Guid != INVALID_ID)
		{
			// 开除帮众权
			if(CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_EXPEL)	
			{
				// 发送踢出帮会的消息包
				CGGuild pk;
				
				pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_EXPEL;
				GUILD_CGW_EXPEL* pExpel = (GUILD_CGW_EXPEL*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_EXPEL);
				if(pExpel)
				{
					pExpel->m_GuildUserGUID = pMember->m_Guid;
					CNetManager::GetMe()->SendPacket(&pk);
				}
			}
			else
			{
				//权限不够
				ShowMsg(ERR_GUILD_POW_NOEXPEL);
			}
		}

		return 0;
	}

	// 接纳会员
	INT Guild::RecruitGuild(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild RecruitGuild Wrong Param1");
			return -1;
		}

		INT idx = args[2].GetInteger();
		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);
		
		if( pMember && pMember->m_Guid != INVALID_ID && pMember->m_bPosition == GUILD_POSITION_TRAINEE )	// 预备帮众，待批准
		{
			// 权限判断
			if( CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_RECRUIT )
			{
				//发送招收会员的消息包
				CGGuild pk;
				pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_RECRUIT;

				GUILD_CGW_RECRUIT* pRecruit = (GUILD_CGW_RECRUIT*)pk.GetGuildPacket()->GetPacket( GUILD_PACKET_CG_RECRUIT );
				if( pRecruit )
				{
					pRecruit->m_ProposerGUID = pMember->m_Guid;
					CNetManager::GetMe()->SendPacket( &pk );
				}
			}
			else
			{
				//权限不够
				ShowMsg(ERR_GUILD_POW_NORECRUIT);
			}
		}

		return 0;
	}

	INT Guild::QuitGuild(LuaPlus::LuaState* state)
	{
		// 判断有效性
		if( INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild() )
		{
			if( CDataPool::GetMe()->Guild_GetCurAccess() & GUILD_AUTHORITY_LEAVE )	// 离开帮会权（这个权限有点多余）
			{
				// 帮会名称
				LPCTSTR pGuildName = CDataPool::GetMe()->Guild_GetName();
				// 显示退出确认框
				CEventSystem::GetMe()->PushEvent( GE_GUILD_QUIT_CONFIRM, pGuildName );
			}
			else
			{
				//权限不够
				ShowMsg( ERR_GUILD_NOPOW_MSG );
			}
		}
		return 0;
	}

	INT Guild::DemisGuild(LuaPlus::LuaState* state)
	{
		//是否有帮会
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			//是否是帮主
			if(CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN)
			{
				BOOL bAss = FALSE;
                //帮里是否有副帮主
				for(INT i = 0; i < CDataPool::GetMe()->Guild_GetMemInfoNum(); ++i)
				{
					GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(i);
					if(pMember && pMember->m_Guid != INVALID_ID)
					{
						if(pMember->m_bPosition == GUILD_POSITION_ASS_CHIEFTAIN)
						{
							bAss = TRUE;
							break;
						}
					}
				}

				if(bAss && CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_DEMISE)
				{
					//发送禅让包
					CGGuild pk;
					pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_DEMISE;
					GUILD_CGW_DEMISE* pDemis = (GUILD_CGW_DEMISE*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_DEMISE);

					if(pDemis)
					{
						CNetManager::GetMe()->SendPacket(&pk);
					}
				}

				//显示错误信息
				if(!bAss)
				{
					ShowMsg(ERR_GUILD_NOTHAVEASSCHIEF_MSG);
				}

				if(!(CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_DEMISE))
				{
					ShowMsg(ERR_GUILD_NOPOW_MSG);
				}
			}
		}
		return 0;
	}

	INT	Guild::PrepareMembersInfomation(LuaPlus::LuaState* state)
	{
		PerpareMembersBeforeShow();
		return 0;
	}

	VOID Guild::ShowMsg(INT msgType, BOOL bTalk, BOOL bTip)
	{
		STRING strMsg = "";

		switch(msgType) {
		case ERR_GUILD_ALREADYIN_MSG:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_AlreadyIn");
			break;
		case ERR_GUILD_NOTIN_MSG:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Not_Guild_Member");
			break;
		case ERR_GUILD_NOPOW_MSG:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_CantDo");
			break;
		case ERR_GUILD_NOTHAVEASSCHIEF_MSG:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Change_Demiss_No_Assistant_Chief");
			break;
		case ERR_GUILD_CREATE_LEVEL_TOO_LOW:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Level_Too_Low_To_Create");
			break;
		case ERR_GUILD_NAME_EMPTY:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Name_Empty_Create");
			break;
		case ERR_GUILD_NAME_INVALID:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Name_Invalid_Create");
			break;
		case ERR_GUILD_NAME_CANTUSE:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Name_CantUse_Create");
			break;
		case ERR_GUILD_DESC_EMPTY:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Desc_Empty_Create");
			break;
		case ERR_GUILD_DESC_INVALID:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Desc_Invalid_Create");
			break;
		case ERR_GUILD_MONEY_NOT_ENOUGH:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Money_Too_Low_To_Create");
			break;
		case ERR_GUILD_JOIN_LEVEL_TOO_LOW:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Level_Too_Low_To_Join");
			break;
		case ERR_GUILD_POW_NORECRUIT:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_No_Recruit");
			break;
		case ERR_GUILD_POW_NOEXPEL:
			strMsg = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_No_Expel");
			break;
		default:
			break;
		}
		
		m_MsgArray.clear();

		if(strMsg.empty()) return;
		if(!bTalk && !bTip) return;

		if(bTalk)
		{
			ADDTALKMSG(strMsg);
		}
		
		if(bTip)
		{
			CEventSystem::GetMe()->PushEvent( GE_PERSONAL_MESSAGE, strMsg.c_str() );
		}
	}

	VOID Guild::ConvertServerTime(INT dTime, STRING& strTime)
	{
		INT min,hour,day,month,year;

		min = dTime % 100;
		dTime /= 100;

		hour = dTime % 100;
		dTime /= 100;

		day = dTime % 100;
		dTime /= 100;

		month = dTime % 100;
		dTime /= 100;

		//year = 2000 + dTime % 100;
		year = dTime % 100;
		dTime /= 100;

		//CHAR szFormat[] = "%d年%d月%d日 %d:%d";
		//CHAR szFormat[] = "%d年%d月%d日";
		CHAR szFormat[] = "%02d-%02d-%02d";
		CHAR szDate[32] = {0};
		//_snprintf(szDate,sizeof(szDate), szFormat, year,month,day,hour,min);
		//_snprintf(szDate,sizeof(szDate), szFormat, year,month,day);
		_snprintf(szDate,sizeof(szDate),szFormat, month,day,year);

		strTime = szDate;
	}

	STRING Guild::GetShowColor_For_Lua(INT idx)
	{
		STRING strColor;

		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);
		if(pMember && pMember->m_Guid != INVALID_ID)
		{
			switch(pMember->m_bPosition) {
			case GUILD_POSITION_CHIEFTAIN:
			case GUILD_POSITION_ASS_CHIEFTAIN:
			case GUILD_POSITION_ELITE_MEMBER:
//			case GUILD_POSITION_COM:
//			case GUILD_POSITION_AGRI:
//			case GUILD_POSITION_INDUSTRY:
//			case GUILD_POSITION_HR:
				strColor = (pMember->m_bIsOnline > 0)?m_LuaShowColors.m_OnlineLeaderColor:m_LuaShowColors.m_LeaveLeaderColor;
				break;
			case GUILD_POSITION_TRAINEE:
				strColor = (pMember->m_bIsOnline > 0)?m_LuaShowColors.m_OnlineMemberColor:m_LuaShowColors.m_LeaveMemberColor;
				break;
			case GUILD_POSITION_MEMBER:
				strColor = (pMember->m_bIsOnline > 0)?m_LuaShowColors.m_OnlineTraineeColor:m_LuaShowColors.m_LeaveTraineeColor;
				break;
			default:
				break;
			}
		}

		return strColor;
	}


	// 保存每次成员列表中的选择
	INT Guild::SetMemberListSelect(LuaPlus::LuaState* state)
	{
		LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			KLThrow("LUA: Guild SetMemberListSelect Wrong Param1");
			return -1;
		}

		m_iMemberSelect = args[2].GetInteger();

		return 0;
	}

	INT Guild::OpenMenu(LuaPlus::LuaState* state)
	{
		LuaStack args(state);

		if( !(args[2].IsInteger()) )
		{
			KLThrow("LUA: Guild::OpenMenu Wrong Param1");
			return -1;
		}
		if( !(args[3].IsInteger()) )
		{
			KLThrow("LUA: Guild::OpenMenu Wrong Param2");
			return -1;
		}

		enum
		{
			JOIN_MENU = 0,	// 申请列表管理操作
			TRANSFER_MENU,	// 转移成员到家族
		};

		// 0	申请列表管理
		// 1	转移成员到家族
		INT nType = args[2].GetInteger();
		INT nMember = args[3].GetInteger();


		// 得到鼠标位置
		POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();

		// 保存参数
		std::vector< STRING > vParam;

		CHAR szMember[32]; _snprintf(szMember, 32, "%d", nMember);
		CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
		CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);

		switch( nType )
		{
		case JOIN_MENU:
			{
				vParam.push_back("GuildJoinMenu");

				vParam.push_back("-1");
				vParam.push_back( szMember );
				vParam.push_back(szXPos);
				vParam.push_back(szYPos);
			}
			break;
		case TRANSFER_MENU:
			{
				vParam.push_back("GuildTransferMenu");
				vParam.push_back("-1");
				vParam.push_back( szMember );
				vParam.push_back(szXPos);
				vParam.push_back(szYPos);

				// 获取当前的家族数量
				INT nFamilyNum = CDataPool::GetMe()->Guild_GetFamilyNum();

				CHAR szStr[32]; _snprintf(szStr, 32, "%d", nFamilyNum);
				vParam.push_back(szStr);

				GuildFamilyInfo* pFamily = NULL;
				// 添加到菜单中
				for( INT i = 0; i < nFamilyNum; ++i )
				{
					pFamily = CDataPool::GetMe()->Guild_GetFamilyInfoByIndex( i );
					if( pFamily )
					{
						vParam.push_back( pFamily->m_sFamilyName );
					}
				}

				// 标记加入家族结束
				vParam.push_back("-1");
			}
			break;
		};

		CEventSystem::GetMe()->PushEvent( GE_SHOW_CONTEXMENU, vParam );

		return 0;
	}


	VOID Guild::PerpareMembersBeforeShow()
	{
		MEMBER_TABLE	onlineLeaders;		//在线领导
		MEMBER_TABLE	leaveLeaders;		//离线领导

		MEMBER_TABLE	onlineMembers;		//在线成员
		MEMBER_TABLE	leaveMembers;		//离线成员

		MEMBER_TABLE	onlineTrainees;		//在线预备成员
		MEMBER_TABLE	leaveTrainees;		//离线预备成员


		//clear
		m_AllMembers.clear();
		m_AllTrainees.clear();

		for(INT i = 0; i < CDataPool::GetMe()->Guild_GetMemInfoNum(); ++i)
		{
			GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(i);
			if(pMember && pMember->m_Guid != INVALID_ID)
			{
				Name_Idx curMember;
				curMember.m_MemberIdx	= i;
				curMember.m_MemberName	= pMember->m_szName;
				curMember.m_Position	= pMember->m_bPosition;		// 职位

				switch(pMember->m_bPosition) 
				{
				// 帮主
				case GUILD_POSITION_CHIEFTAIN:
				// 副帮主
				case GUILD_POSITION_ASS_CHIEFTAIN:
				// 精英帮众
				case GUILD_POSITION_ELITE_MEMBER:
				// 家族长
				case GUILD_POSITION_FAMILY_BOSS:
					( pMember->m_bIsOnline > 0 ) ? onlineLeaders.push_back( curMember ) : leaveLeaders.push_back( curMember );
					break;

				// 预备帮众，待批准
				case GUILD_POSITION_TRAINEE:
					( pMember->m_bIsOnline > 0 ) ? onlineTrainees.push_back( curMember ) : leaveTrainees.push_back( curMember );
					break;

				// 普通帮众
				case GUILD_POSITION_MEMBER:
					( pMember->m_bIsOnline > 0 ) ? onlineMembers.push_back( curMember ) : leaveMembers.push_back( curMember );
					break;

				default:
					break;
				}
			}
		}


		//================
		// 领导排序

		// 在线
		std::stable_sort(onlineLeaders.begin(), onlineLeaders.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(onlineLeaders.begin(), onlineLeaders.end(), SimpleCmpFunc::less_position);
		// 离线
		std::stable_sort(leaveLeaders.begin(), leaveLeaders.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(leaveLeaders.begin(), leaveLeaders.end(), SimpleCmpFunc::less_position);


		//================
		// 正式帮众排序

		std::stable_sort(onlineMembers.begin(), onlineMembers.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(leaveMembers.begin(), leaveMembers.end(), SimpleCmpFunc::less_strcmp);

		// 生成所有成员表

		//0.在线领导
		for(UINT i=0; i<onlineLeaders.size(); ++i)
		{
			m_AllMembers.push_back(onlineLeaders.at(i).m_MemberIdx);
		}
		//1.在线成员
		for(UINT i=0; i<onlineMembers.size(); ++i)
		{
			m_AllMembers.push_back(onlineMembers.at(i).m_MemberIdx);
		}
		//2.离线领导
		for(UINT i=0; i<leaveLeaders.size(); ++i)
		{
			m_AllMembers.push_back(leaveLeaders.at(i).m_MemberIdx);
		}
		//3.离线成员
		for(UINT i=0; i<leaveMembers.size(); ++i)
		{
			m_AllMembers.push_back(leaveMembers.at(i).m_MemberIdx);
		}



		//================
		// 预备帮众排序

		std::stable_sort(onlineTrainees.begin(), onlineTrainees.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(leaveTrainees.begin(), leaveTrainees.end(), SimpleCmpFunc::less_strcmp);

		//生成所有预备成员表

		//0.在线预备成员
		for(UINT i=0; i<onlineTrainees.size(); ++i)
		{
			m_AllTrainees.push_back(onlineTrainees.at(i).m_MemberIdx);
		}
		//1.离线预备成员
		for(UINT i=0; i<leaveTrainees.size(); ++i)
		{
			m_AllTrainees.push_back(leaveTrainees.at(i).m_MemberIdx);
		}

	}



	// 字符合法检测
	BOOL Guild::CharacterCheck( const STRING& str )
	{
		// 完全匹配过滤
		if( !CGameProcedure::s_pUISystem->CheckStringFullCompare( str, "guild" ) )
		{
			return FALSE;
		}
        
		// 敏感字符过滤
		if( !CGameProcedure::s_pUISystem->CheckStringFilter( str, FT_NAME ) )
		{
			return FALSE;
		}

		// 名称非法字符过滤
		if( !KLU_CheckStringValid( str.c_str() ) )
		{
			return FALSE;
		}


		return TRUE;
	}


//  SimpleCmpFunc------------------------------------------------------------------
	BOOL SimpleCmpFunc::less_strcmp(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2)
	{
		if( 0 <= strcmp(s1.m_MemberName.c_str(),s2.m_MemberName.c_str()))
			return FALSE;
		else
			return TRUE;
	}

	BOOL SimpleCmpFunc::less_strcoll(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2)
	{
		if( 0 <= strcoll(s1.m_MemberName.c_str(),s2.m_MemberName.c_str()))
			return FALSE;
		else
			return TRUE;
	}

	BOOL SimpleCmpFunc::less_position(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2)
	{
		if(s1.m_Position <= s2.m_Position)
			return FALSE;
		else
			return TRUE;
	}
	
}