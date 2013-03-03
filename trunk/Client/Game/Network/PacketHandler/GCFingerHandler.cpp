
/**	
 *
 *	功能：	搜索返回结果的处理
 *
 *	修改记录：
 *			080318	添加实现
 *			
 *			
 */

#include "StdAfx.h"
#include "GCFinger.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDataPool.h"
#include "Event/GMEventSystem.h"


uint GCFingerHandler::Execute(GCFinger* pPacket, Player* pPlayer)
{
__ENTER_FUNCTION

	// 判断主流程
	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
//		CHAR szText[_MAX_PATH * 10];

		GC_FINGER* pFinger = pPacket->GetFinger();

//		pFinger->m_FingerPlayerList	// RETURN_FINGER_PLAYERLIST
		
		if( pFinger )
		{
			switch( pFinger->m_Type )	// FINGER_RETURN_TYPE
			{
			// 搜索到的玩家列表
			case FRET_PLAYERLIST:
				{
					INT nPlayerCount = pFinger->m_FingerPlayerList.GetPlayerCount();	// 数量
					FINGER_PLAYER_INFO* pPlayerInfo = NULL;

					FingerPlayerInfo_t  FingerPlayerInfo;

					// 清空数据池
					CDataPool::GetMe()->Finger_ClearInfo();

					//
					INT nCount = 0;

					for( INT i = 0; i < nPlayerCount; ++i )
					{
						pPlayerInfo = pFinger->m_FingerPlayerList.GetPlayerInfo( i );

						if( pPlayerInfo )
						{
							FingerPlayerInfo.CleanUp();

							FingerPlayerInfo.m_nGUID		= pPlayerInfo->GetGUID();			// GUID
							FingerPlayerInfo.m_sName		= pPlayerInfo->GetName();			// 名字
							FingerPlayerInfo.m_bOnlineFlag	= pPlayerInfo->GetOnlineFlag();		// 是否在线
							FingerPlayerInfo.m_nLevel		= pPlayerInfo->GetLevel();			// 角色等级
							FingerPlayerInfo.m_nSex			= pPlayerInfo->GetSex();			// 性别
							FingerPlayerInfo.m_nMenPai		= pPlayerInfo->GetMenPai();			// 门派 MENPAI_ATTRIBUTE
							FingerPlayerInfo.m_nGuildID		= pPlayerInfo->GetGuildID();		// 帮会ID（用于发给服务端）
							FingerPlayerInfo.m_sGuildName	= pPlayerInfo->GetGuildName();		// 帮会名称（用于 Server 发给客户端）
							FingerPlayerInfo.m_nPortrait	= pPlayerInfo->GetPortrait();		// 头像

							// 保存到数据池中
							CDataPool::GetMe()->Finger_SetPlayerInfoByIndex( i, &FingerPlayerInfo );

							nCount++;
						}
					}

					CDataPool::GetMe()->Finger_SetPlayerInfoNum( nCount );

//					FINGER_PLAYER_INFO* pLastPlayerInfo = pFinger->m_FingerPlayerList.GetLastPlayerInfo();
//					INT nFingerFlag = pFinger->m_FingerPlayerList.GetFingerFlag();		// 是否查找过全部范围(没有下一页)
//					INT nFingerPosition = pFinger->m_FingerPlayerList.GetPosition();	// 当前搜索位置

					CEventSystem::GetMe()->PushEvent( GE_OPEN_FRIEND_SEARCH_REASON );
				}
				break;
			
			//
			case FRET_ERR_START:
				{
				}
				break;

			// 没有任何匹配结果
			case FRET_ERR_NORESULT:
				{
					CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, "没有找到匹配的结果。" );
				}
				break;

			// 
			case FRET_NONE:
				{
				}
				break;
			};
		}									

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
