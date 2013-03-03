
/** GCGuildReturnHandler.cpp
 *
 *	功能：	帮会操作信息反馈
 *	
 *	修改记录：
 *			080308	GUILD_RETURN_INVIT
 *					GUILD_RETURN_CREATE_FAMILY
 *					GUILD_RETURN_ADD_FAMILY_USER
 *					GUILD_RETURN_REMOVE_FAMILY_USER
 *			080411	添加在创建成功返回后向服务器请求消息
 *			
 */

#include "StdAfx.h"
#include "GCGuildReturn.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "../../Interface/GMInterface_Script_Talk.h"
#include "..\..\DataPool\GMDP_CharacterData.h"




uint GCGuildReturnHandler::Execute( GCGuildReturn* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		_GUILD_RETURN* pReturn = pPacket->GetGuildReturn();
		CHAR szText[_MAX_PATH];

		STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("GCGuildReturnHandler_Info_Guild_Other_Operate");
		_snprintf(szText, _MAX_PATH, strTemp.c_str());
		//_snprintf(szText, _MAX_PATH, "其他帮会操作。");

 		switch( pReturn->m_ReturnType )
		{
		// 响应成功
		case GUILD_RETURN_RESPONSE:
			{
				STRING strTemp = "";
				strTemp = NOPARAMMSGFUNC("GCGuildReturnHandler_Info_Respound_Guild_Create");
				_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_SourName);	// "%s响应本帮会的建立。
			}
			break;

		// 创建成功
		case GUILD_RETURN_CREATE:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildReturnHandler_Info_Guild_Create_Success");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());	// "建立帮会成功。"

//				INT guildId = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
//				if( INVALID_ID != guildId )
//				{
//					CDataPool::GetMe()->Guild_AskInfo( guildId );
//				}
			}
			break;

		// 加入申请列表等待批准
		case GUILD_RETURN_JOIN:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildReturnHandler_Info_Apply_Success_Waite_For_Pass", pReturn->m_GuildName);
				_snprintf(szText, _MAX_PATH, strTemp.c_str());	// "你已经向%s帮会发出了申请，请等待该帮会处理。"
			}
			break;

		// 开除帮众
		case GUILD_RETURN_EXPEL:
			{
				STRING strTemp = "";
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//自己被踢出帮会
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();
					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);

					strTemp = NOPARAMMSGFUNC("GCGuildReturnHandler_Info_Kick_Guild_Member");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_SourName, pReturn->m_DestName);
				}
				else
				{
					//别人看到
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{
							//改变职位
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}

					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;

		// 拒绝申请
		case GUILD_RETURN_REJECT:
			{
				STRING strTemp = "";
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//自己的申请被拒绝了
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();
					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);

					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_Refuse_Member1");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_GuildName);
				}
				else
				{
					//别人看到
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{
							//改变职位
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_You_Kick_Other_Success");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_DestName);

					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;

		// 离开帮会
		case GUILD_RETURN_LEAVE:
			{
				//在数据池中查找这个玩家,改变职位
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{//是自己退出帮会
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();

					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_You_Leave2");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_GuildName);

					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);
				}
				else
				{//其他帮众看到某人退出帮会
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{//改变职位
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}
					
					//帮会频道
					//
					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;

		// 接收帮众
		case GUILD_RETURN_RECRUIT:
			{
				STRING strTemp = "";
				//在数据池中查找这个玩家,改变职位
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//改变职位
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)GUILD_POSITION_MEMBER;
						break;
					}
				}

				CDataPool::GetMe()->Guild_SetMaxMemNum(CDataPool::GetMe()->Guild_GetMaxMemNum()+1);

				//刷新界面
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);

				//被接收人
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					strTemp	= NOPARAMMSGFUNC("GMGameInterface_Script_Guild_New_Member2");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_GuildName);
				}

				//接收人
				if(pReturn->m_GUIDChanged == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_New_Member1");
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_DestName);
				}

				strTemp = COLORMSGFUNC("GMGameInterface_Script_Guild_Welcome_New_Member", pReturn->m_DestName, pReturn->m_GuildName);
				//添加帮会聊天消息
				ADD_GUILD_MSG(strTemp);
			}
			break;

		// 正式成立（人数达到）
		case GUILD_RETURN_FOUND:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildReturnHandler_Info_Guild_Set_Up");
				_snprintf(szText, _MAX_PATH, strTemp.c_str());	// "帮会正式成立。"
			}
			break;

		// 解散帮会
		case GUILD_RETURN_DISMISS:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("GCGuildReturnHandler_Info_Guild_Dismiss");
				_snprintf(szText, _MAX_PATH,  strTemp.c_str());	// "帮会解散。"
			}
			break;

		// 任免职务，升职
		case GUILD_RETURN_PROMOTE:
			{
				//输出字符串
				//在数据池中查找这个玩家,改变职位
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{
						//改变职位
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)pReturn->m_PosID;
						break;
					}
				}

				//你的帮会权限改变了。
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_Change_Position_Success2");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
				}
				else
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_Change_Position_Success1");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
				}

				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;

		// 任免职务，降职
		case GUILD_RETURN_DEMOTE:
			{
				//输出字符串
				//在数据池中查找这个玩家,改变职位
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//改变职位
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)pReturn->m_PosID;
						break;
					}
				}

				//你的帮会权限改变了。
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_Change_Position_Success2");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
				}
				else
				{
					STRING strTemp = "";
					strTemp = NOPARAMMSGFUNC("GMGameInterface_Script_Guild_Change_Position_Success1");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
				}

				
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;

		// 调整权限，授权
		case GUILD_RETURN_AUTHORIZE:
		// 调整权限，解除权限
		case GUILD_RETURN_DEPRIVE_AUTHORITY:
			{
			}
			break;

		// 禅让
		case GUILD_RETURN_DEMISE:
			{
				if(pReturn->m_GUIDChanged == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//你的权限改变了，恭喜你成为帮会帮主。
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Promote_Chief");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
					break;
				}

				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//操作成功你的权限改变了
					STRING strTemp = "";
					strTemp = NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_Change_OK");
					_snprintf(szText, _MAX_PATH, strTemp.c_str());
					break;
				}

				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUIDChanged)
					{
						//改变职位
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = GUILD_POSITION_CHIEFTAIN;
					}

					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{
						//改变职位
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = GUILD_POSITION_MEMBER;
					}
				}

				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;

		// 收到别人的加帮请求
//		case GUILD_PACKET_GC_CONFIRM_GUILD_INVITE:
//
//				if( pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID() )
//				{
//					// 保存ID到数据池中
//					CDataPool::GetMe()->Guild_SetInviteGuildInfo( pReturn->m_GuildID );
//					CDataPool::GetMe()->Guild_SetInviteInfo( pReturn->m_GUID );
//
//					// 向界面发送按钮闪烁
//					CGameProcedure::s_pEventSystem->PushEvent( GE_GUILD_NOTIFY_INTIVE, 1 );
//				}
//				// 别人看到
//				else
//				{
//				}
//
//			break;

		// 邀请答复后的处理
		case GUILD_RETURN_INVIT:
//			if( pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID() )
			{
				CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Guild( pReturn->m_GuildID );
				pReturn->m_GuildName;

//				STRING strTemp = "你同意了加入";
//				strTemp += pReturn->m_GuildName;
//				strTemp += "帮会。";

				STRING strTemp = "";
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
			}
			break;

		// 创建家族
		case GUILD_RETURN_CREATE_FAMILY:
			{
				pReturn->m_cFamilyID;
				pReturn->m_FamilyNameSize;
				pReturn->m_FamilyDescSize;
				pReturn->m_FamilyName;
				pReturn->m_FamilyDesc;

				STRING strTemp = pReturn->m_FamilyName;
				strTemp += "家族创建成功。";
				_snprintf(szText, _MAX_PATH, strTemp.c_str());

				//刷新界面
				CEventSystem::GetMe()->PushEvent( GE_GUILD_UPDATE_MEMBERINFO, 0 );
			}
			break;
		// 创建失败
		case GUILD_RETURN_CREATE_FAILED:
			{
				STRING strTemp = "家族创建失败！";
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
			}
			break;
		// 添加家族成员
		case GUILD_RETURN_ADD_FAMILY_USER:
			{
				pReturn->m_cFamilyID;
				pReturn->m_SourNameSize;
				pReturn->m_SourName;

				STRING strTemp = "添加成员";
				strTemp += pReturn->m_SourName;
				strTemp += "成功。";
				_snprintf(szText, _MAX_PATH, strTemp.c_str());

				//刷新界面
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;
		// 删除家族成员
		case GUILD_RETURN_REMOVE_FAMILY_USER:
			{
				pReturn->m_cFamilyID;
				pReturn->m_SourNameSize;
				pReturn->m_SourName;

				STRING strTemp = "删除成员";
				strTemp += pReturn->m_SourName;
				strTemp += "成功。";
				_snprintf(szText, _MAX_PATH, strTemp.c_str());

				//刷新界面
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;

		// 修改帮会宗旨
		case GUILD_RETURN_MODIFY_DESC:
			{
				pReturn->m_SourName;

				STRING strTemp = "帮会宗旨已修改为：";
				strTemp += pReturn->m_SourName;
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
			}
			break; 

		// 解散家族
		case GUILD_RETURN_FAMILY_DISMISS:
			{
				pReturn->m_cFamilyID;

				STRING strTemp = "帮会中家族已解散。";
				strTemp += pReturn->m_SourName;
				_snprintf(szText, _MAX_PATH, strTemp.c_str());
			}
			break;

		// 支取金额
		case GUILD_RETURN_WITHDRAW:
		// 存入金额
		case GUILD_RETURN_DEPOSIT:
		default:
			Assert(FALSE);
			return 0;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_PERSONAL_MESSAGE, szText );
//		CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "帮会", szText, 0 );

		// 在显示创建成功后再提示
		if( GUILD_RETURN_CREATE == pReturn->m_ReturnType )
		{
			CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "如果成立后3天内帮会人数未" );
			CEventSystem::GetMe()->PushEvent( GE_SYSTEM_MESSAGE, "达到10人则自动解散。" );
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

