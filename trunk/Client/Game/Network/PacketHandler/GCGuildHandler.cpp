
/** GCGuildHandler.cpp
 */

#include "StdAfx.h"
#include "GCGuild.h"

#include "Procedure\GameProcedure.h"
#include "Event\GMEventSystem.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "DataPool\GMDataPool.h"
#include "DataPool\GMDP_Struct_SystemTip.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"


uint GCGuildHandler::Execute( GCGuild* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR szText[_MAX_PATH * 10];

		switch( pPacket->GetGuildPacket()->m_uPacketType )
		{
		// 帮会列表
		case GUILD_PACKET_GC_LIST:
			{
				GuildInfo_t GuildInfo;

				GUILD_WGC_LIST* pList = (GUILD_WGC_LIST*)pPacket->GetGuildPacket()->GetPacket(GUILD_PACKET_GC_LIST);

				_snprintf(szText, _MAX_PATH * 10, "\n%10s%6s%6s%10s\n", "帮会名称", "ID", "人数", "状态");
				_snprintf(szText, _MAX_PATH * 10, "%s--------------------------------\n", szText);


				CDataPool::GetMe()->Guild_ClearAllInfo();

				for( INT i=0; i<pList->m_uGuildListCount; ++i )
				{
					_snprintf(szText, _MAX_PATH * 10, "%s%10s%6d%6d%10s\n",
						szText,
						pList->m_uGuild[i].m_GuildName,
						pList->m_uGuild[i].m_GuildID,
						(INT)pList->m_uGuild[i].m_uGuildUserCount,
						(pList->m_uGuild[i].m_uGuildStatus == GUILD_STATUS_NORMAL) ? "已成立" : "创建");


					GuildInfo.CleanUp();

					GuildInfo.m_GuildID			=	pList->m_uGuild[i].m_GuildID;			// 工会ID
					GuildInfo.m_GuildName		=	pList->m_uGuild[i].m_GuildName;			// 工会名
					GuildInfo.m_GuildDesc		=	pList->m_uGuild[i].m_GuildDesc;			// 工会宗旨
					GuildInfo.m_ChiefName		=	pList->m_uGuild[i].m_ChiefName;			// 帮主名
					GuildInfo.m_CityName		=	pList->m_uGuild[i].m_CityName;			// 城市名
					GuildInfo.m_uGuildStatus	=	pList->m_uGuild[i].m_uGuildStatus;		// 工会状态
					GuildInfo.m_bGuildLevel		=	pList->m_uGuild[i].m_bGuildLevel;		// 工会级别
					GuildInfo.m_uGuildUserCount	=	pList->m_uGuild[i].m_uGuildUserCount;	// 帮众数量
					GuildInfo.m_nFoundedTime	=	pList->m_uGuild[i].m_nFoundedTime;		// 成立时间

					CDataPool::GetMe()->Guild_SetInfoByIndex(i, &GuildInfo);
				}

				_snprintf(szText, _MAX_PATH * 10, "%s--------------------------------\n", szText);
				_snprintf(szText, _MAX_PATH * 10, "%s帮会总数：%6d", szText, pList->m_uGuildCount);


				CDataPool::GetMe()->Guild_SetInfoNum(pList->m_uGuildCount);

				CEventSystem::GetMe()->PushEvent( GE_GUILD_SHOW_LIST );
			}
			break;

		// 帮众列表
		case GUILD_PACKET_GC_MEMBER_LIST:
			{
				//帮众信息列表
				GUILD_WGC_MEMBER_LIST* pMemberList = (GUILD_WGC_MEMBER_LIST*)pPacket->GetGuildPacket()->GetPacket(GUILD_PACKET_GC_MEMBER_LIST);
				GuildMemberInfo_t GuildMemInfo;

				CDataPool::GetMe()->Guild_ClearMemInfo();
				
				for( INT i=0; i<pMemberList->m_uMemberCount; ++i )
				{
					GuildMemInfo.CleanUp();
					GuildMemInfo.m_szName			=	pMemberList->m_GuildMemberData[i].m_szName;
					GuildMemInfo.m_Guid				=	pMemberList->m_GuildMemberData[i].m_Guid;	
					GuildMemInfo.m_bLevel			=	pMemberList->m_GuildMemberData[i].m_bLevel;
					GuildMemInfo.m_bMenPaiID		=	pMemberList->m_GuildMemberData[i].m_bMenPaiID;
					GuildMemInfo.m_iCurContribute	=	pMemberList->m_GuildMemberData[i].m_iCurContribute;
					GuildMemInfo.m_iMaxContribute	=	pMemberList->m_GuildMemberData[i].m_iMaxContribute;
					GuildMemInfo.m_iJoinTime		=	pMemberList->m_GuildMemberData[i].m_iJoinTime;
					GuildMemInfo.m_iLogOutTime		=	pMemberList->m_GuildMemberData[i].m_iLogOutTime;
					GuildMemInfo.m_bIsOnline		=	pMemberList->m_GuildMemberData[i].m_bIsOnline;
					GuildMemInfo.m_bPosition		=	pMemberList->m_GuildMemberData[i].m_bPosition;
					GuildMemInfo.m_iFamilyID		=	pMemberList->m_GuildMemberData[i].m_iFamilyID;
					GuildMemInfo.m_iSex				=	pMemberList->m_GuildMemberData[i].m_iSex;				// 
					GuildMemInfo.m_szTitleName		=	pMemberList->m_GuildMemberData[i].m_szTitleName;		// 

					CDataPool::GetMe()->Guild_SetMemInfoByIndex(i, &GuildMemInfo);
				}

				GuildFamilyInfo FamilyInfo;
				for( INT i=0; i<pMemberList->m_uFamilyCount; ++i )
				{
					FamilyInfo.CleanUp();

					FamilyInfo.m_iFamilyID			= pMemberList->m_GuildFamilyData[i].m_iFamilyID;
					FamilyInfo.m_sFamilyChiefName	= pMemberList->m_GuildFamilyData[i].m_szShaikhName;
					FamilyInfo.m_sFamilyName		= pMemberList->m_GuildFamilyData[i].m_szFamilyName;
					FamilyInfo.m_sFamilyDesc		= pMemberList->m_GuildFamilyData[i].m_szFamilyDesc;
					FamilyInfo.m_iFamilyUserNum		= pMemberList->m_GuildFamilyData[i].m_iFamilyUserNum;

					for( INT j=0; j<FamilyInfo.m_iFamilyUserNum; ++j )
					{
						FamilyInfo.m_aFamilyUser[j] = pMemberList->m_GuildFamilyData[i].m_aFamilyUser[j];
					}

					CDataPool::GetMe()->Guild_SetFamilyMemInfoByIndex(i, &FamilyInfo);
				}


				CDataPool::GetMe()->Guild_SetMemInfoNum(pMemberList->m_uMemberCount);
				CDataPool::GetMe()->Guild_SetMaxMemNum(pMemberList->m_uMemberMax);
				CDataPool::GetMe()->Guild_SetDesc(pMemberList->m_GuildDesc);
				CDataPool::GetMe()->Guild_SetName(pMemberList->m_GuildName);
				CDataPool::GetMe()->Guild_SetCurPosition(pMemberList->m_bPosition);
				CDataPool::GetMe()->Guild_SetCurAccess(pMemberList->m_bAccess);
				CDataPool::GetMe()->Guild_SetFamilyNum(pMemberList->m_uFamilyCount);

//				CEventSystem::GetMe()->PushEvent( GE_GUILD_SHOW_MEMBERINFO );

			}
			break;

		// 帮会信息
		case GUILD_PACKET_GC_GUILD_INFO:
			{
				//帮派的详细信息
				GUILD_WGC_GUILD_INFO* pGuildDetailInfo = (GUILD_WGC_GUILD_INFO*)pPacket->GetGuildPacket()->GetPacket(GUILD_PACKET_GC_GUILD_INFO);
				GuildDetailInfo_t GuildMemInfo;

				CDataPool::GetMe()->Guild_ClearDetailInfo();

				GuildMemInfo.m_GuildName		=	pGuildDetailInfo->m_GuildName;
				GuildMemInfo.m_GuildCreator		=	pGuildDetailInfo->m_GuildCreator;
				GuildMemInfo.m_GuildChairMan	=	pGuildDetailInfo->m_GuildChairMan;
				GuildMemInfo.m_CityName			=	pGuildDetailInfo->m_CityName;
				GuildMemInfo.m_nLevel			=	pGuildDetailInfo->m_nLevel;
				GuildMemInfo.m_nPortSceneID		=	pGuildDetailInfo->m_nPortSceneID;			// 入口场景
				GuildMemInfo.m_MemNum			=	pGuildDetailInfo->m_MemNum;					// 人数
				GuildMemInfo.m_Longevity		=	pGuildDetailInfo->m_Longevity;				// 资历 
				GuildMemInfo.m_Contribute		=	pGuildDetailInfo->m_Contribute;				// 贡献度
				GuildMemInfo.m_Honor			=	pGuildDetailInfo->m_Honor;					// 人气
				GuildMemInfo.m_FoundedMoney		=	pGuildDetailInfo->m_FoundedMoney;			// 帮派资金
				GuildMemInfo.m_nIndustryLevel	=	pGuildDetailInfo->m_nIndustryLevel;			// 工业度
				GuildMemInfo.m_nAgrLevel		=	pGuildDetailInfo->m_nAgrLevel;				// 农业度
				GuildMemInfo.m_nComLevel		=	pGuildDetailInfo->m_nComLevel;				// 商业度
				GuildMemInfo.m_nDefLevel		=	pGuildDetailInfo->m_nDefLevel;				// 防卫度
				GuildMemInfo.m_nTechLevel		=	pGuildDetailInfo->m_nTechLevel;				// 科技度
				GuildMemInfo.m_nAmbiLevel		=	pGuildDetailInfo->m_nAmbiLevel;				// 扩张度
				
				CDataPool::GetMe()->Guild_SetDetailInfo(&GuildMemInfo);

//				CEventSystem::GetMe()->PushEvent(GE_GUILD_SHOW_DETAILINFO);

				// 打开帮会界面， 现改为打开整和界面
				CEventSystem::GetMe()->PushEvent( GE_UNITE_GF_SHOW, 1 );
			}
			break;

		// 帮会可任命信息
		case GUILD_PACKET_GC_APPOINT_INFO:
			{
				// 可任命的帮派职位
				GUILD_WGC_APPOINT_INFO* pGuildAppointInfo = (GUILD_WGC_APPOINT_INFO*)pPacket->GetGuildPacket()->GetPacket(GUILD_PACKET_GC_APPOINT_INFO);
				
				GuildAppointPos_t GuildAppointPos;
				CDataPool::GetMe()->Guild_ClearAppointInfo();
				
				for(INT i = 0; i<pGuildAppointInfo->iPosNum; i++)
				{
					GuildAppointPos.CleanUp();

					GuildAppointPos.m_PosID		= pGuildAppointInfo->m_PosList[i].m_PosID;
					GuildAppointPos.m_szPosName = pGuildAppointInfo->m_PosList[i].m_PosName;

					CDataPool::GetMe()->Guild_SetAppointInfoByIndex(i, &GuildAppointPos);
				}
				
				// 刷新界面
				CEventSystem::GetMe()->PushEvent(GE_GUILD_SHOW_APPOINTPOS);
			}
			break;

		// 收到别人的加帮请求
		case GUILD_PACKET_GC_CONFIRM_GUILD_INVITE:
			{
				GUILD_WGC_INVITE* pInvite = (GUILD_WGC_INVITE*)pPacket->GetGuildPacket()->GetPacket( GUILD_PACKET_GC_CONFIRM_GUILD_INVITE );

				// 保存ID到数据池中
				CDataPool::GetMe()->Guild_SetInviteGuildId( pInvite->m_GuildId );
				CDataPool::GetMe()->Guild_SetInviteGuildName( pInvite->m_GuildName );
				CDataPool::GetMe()->Guild_SetInviteGuildObjName( pInvite->m_InvitorName );

				STRING strTemp = "有人邀请你加入帮会。";
				ADDNEWDEBUGMSG( strTemp );

				// 向界面发送按钮闪烁
//				CGameProcedure::s_pEventSystem->PushEvent( GE_GUILD_NOTIFY_INTIVE, 1 );

				CDataPool::GetMe()->SystemTip_AddEventInfo( STT_GUILDINVITE, "有人邀请你加入帮会" );
				// 更新界面显示
				CEventSystem::GetMe()->PushEvent( GE_SHOW_SYSTEM_PROMPT );
			}
			break;

		// 收到修改称号的消息
		case GUILD_PACKET_GC_MODIFY_TITLE_NAME:
			{
				GUILD_WGC_MODIFY_TITLE_NAME* pTitle = (GUILD_WGC_MODIFY_TITLE_NAME*)pPacket->GetGuildPacket()->GetPacket( GUILD_PACKET_GC_MODIFY_TITLE_NAME );

				if( pTitle )
				{
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for( INT i = 0; i < iMemNum; ++i )
					{
						if( CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pTitle->m_Guid )
						{
							// 改变职位
							CDataPool::GetMe()->Guild_GetMemInfoByIndex( i )->m_szTitleName = pTitle->m_szTitleName;
							break;
						}
					}

					// 刷新界面
					CEventSystem::GetMe()->PushEvent( GE_GUILD_UPDATE_MEMBERINFO );
				}
			}
			break;


		default:
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

		//CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "帮会", szText, 0 );
		//ADDTALKMSG(szText);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
