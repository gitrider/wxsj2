
/**	
 *	
 */

//GCCharAllTitlesHandler.cpp
#include "stdafx.h"
#include "GCCharAllTitles.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"
#include "Object\Logic\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "Object\GMObjectLoadQueue.h"
#include "Object\Logic\Character\Obj_playerMySelf.h"
#include "dbc\GMDataBase.h"
#include "Interface\GMGameInterface.h"


uint GCCharAllTitlesHandler::Execute( GCCharAllTitles* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{

		CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		if( !pCharacterData )
		{
			return PACKET_EXE_CONTINUE;
		}

		_TITLE* pTitle = pPacket->GetTitleInfo();
		if( !pTitle )
		{
			return PACKET_EXE_CONTINUE;
		}


		DBC_DEFINEHANDLE( pTitleDBC, DBC_TITLE );	// title_info.tab

		INT type = pPacket->GetType();


		//================
		// 更改帮会称号的消息
		if( type == GCCharAllTitles::UPDATE_BANGPAI_TITLE_NAME )
		{
			INT nGuildId = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
			if( nGuildId != -1 )
			{
//				GuildInfo_t* guildInfo = CDataPool::GetMe()->Guild_GetInfoByIndex( nGuildId );

				GuildMemberInfo_t* guildMemInfo = NULL;

				INT guildMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for( INT i = 0; i < guildMemNum; ++i )
				{
					guildMemInfo = CDataPool::GetMe()->Guild_GetMemInfoByIndex( i );
					if( guildMemInfo )
					{
						if( guildMemInfo->m_Guid == CObjectManager::GetMe()->GetMySelf()->GetServerGUID() )
						{
							guildMemInfo->m_szTitleName = pTitle->m_szOfficialTitleName;

							CEventSystem::GetMe()->PushEvent( GE_GUILD_UPDATE_MEMBERINFO );

							break;
						}
					}
				}	// for
				
			}
		}	// 修改帮会成员称号



		//================
		// 处理所有的称号

		_TITLE_ titletmp;
		INT nType = 0;

		// 统计数量
		INT numCountry = 0;	// 国家数量
		INT numGuild = 0;	// 帮会数量
		INT numWanfa = 0;	// 玩法数量
		INT num = 0;		// 总数
		

		// 处理所有的称号
		for( INT i = 0; i < MAX_TITLE_SIZE; ++i )
		{
			nType = pTitle->m_TitleArray[i].m_iTitleType;

			if( nType < 1 || nType > 3)
				continue;

			// 赋值
			titletmp.bFlag	=	_TITLE_::STRING_TITLE;		// 表明是字符

			titletmp.ID =	pTitle->m_TitleArray[i].m_iTitleID;
			memset( &titletmp.szTitleData, 0, MAX_CHARACTER_TITLE );
			/*if( 0 == pCharacterData->Get_RaceID() )
				memcpy( &titletmp.szTitleData, &( pTitle->m_TitleArray[i].m_szFemaleName), strlen( pTitle->m_TitleArray[i].m_szFemaleName ) );
			else if( 1 == pCharacterData->Get_RaceID() )
				memcpy( &titletmp.szTitleData, &( pTitle->m_TitleArray[i].m_szMaleName), strlen( pTitle->m_TitleArray[i].m_szMaleName ) );*/

			titletmp.bArrId	=	i;
			titletmp.nTime	=	pTitle->m_TitleArray[i].m_uTime;

			

			switch( nType )
			{
			// 国家称号
			case _TITLE::GUOJIA_TITLE:
				{
					titletmp.bType	=	_TITLE::GUOJIA_TITLE;

					pCharacterData->Set_Title( num, titletmp );
					CDataPool::GetMe()->Title_SetTitle( nType, numCountry++, num );
					num++;
				}
				break;
			// 帮派称号
			case _TITLE::BANGPAI_TITLE:
				{
					titletmp.bType	=	_TITLE::BANGPAI_TITLE;

					pCharacterData->Set_Title( num, titletmp );
					CDataPool::GetMe()->Title_SetTitle( nType, numGuild++, num );
					num++;
				}
				break;
			// 普通称号
			case _TITLE::WANFA_TITLE:
				{
					titletmp.bType	=	_TITLE::WANFA_TITLE;

					pCharacterData->Set_Title( num, titletmp );
					CDataPool::GetMe()->Title_SetTitle( nType, numWanfa++, num );
					num++;
				};
				break;
			};
		}

		// 设置每类称号的数量
		CDataPool::GetMe()->Title_SetTitleNum( _TITLE::GUOJIA_TITLE, numCountry );
		CDataPool::GetMe()->Title_SetTitleNum( _TITLE::BANGPAI_TITLE, numGuild );
		CDataPool::GetMe()->Title_SetTitleNum( _TITLE::WANFA_TITLE, numWanfa );

		// 设置称号总数
		pCharacterData->Set_TitleNum( num );


		// 当前帮派称号ID
		if( CDataPool::GetMe()->Title_GetCurTitle( _TITLE::BANGPAI_TITLE ) != pTitle->m_CurGuildTitle )
		{
/*
			INT titleId = pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_iTitleID;
			if( -1 != titleId )
			{
				const _DBC_TITLE* pDbcTitle = (const _DBC_TITLE*)pTitleDBC->Search_LineNum_EQU( titleId );
				if( pDbcTitle )
				{
					// 女
					if( 0 == pCharacterData->Get_RaceID() )
						pCharacterData->Set_CurTitle( pDbcTitle->szWomanTitle, _TITLE::BANGPAI_TITLE );
					// 男
					else if( 1 == pCharacterData->Get_RaceID() )
						pCharacterData->Set_CurTitle( pDbcTitle->szManTitle, _TITLE::BANGPAI_TITLE );
				}
			}
*/

			if( -1 == pTitle->m_CurGuildTitle )
			{
				pCharacterData->Set_CurTitle( "", _TITLE::BANGPAI_TITLE );
			}
			else
			{
				// 女
				//if( 0 == pCharacterData->Get_RaceID() )
				//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szFemaleName, _TITLE::BANGPAI_TITLE );
				//// 男
				//else if( 1 == pCharacterData->Get_RaceID() )
				//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szMaleName, _TITLE::BANGPAI_TITLE );
			}
		}
		// 当前国家称号ID
		if( -1 != pTitle->m_CurCountryTitle )
		{
			// 女
			//if( 0 == pCharacterData->Get_RaceID() )
			//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szFemaleName, _TITLE::GUOJIA_TITLE );
			//// 男
			//else if( 1 == pCharacterData->Get_RaceID() )
			//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szMaleName, _TITLE::GUOJIA_TITLE );
		}
		if( -1 != pTitle->m_CurNormalTitle )
		{
			// 女
			//if( 0 == pCharacterData->Get_RaceID() )
			//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szFemaleName, _TITLE::WANFA_TITLE );
			//// 男
			//else if( 1 == pCharacterData->Get_RaceID() )
			//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szMaleName, _TITLE::WANFA_TITLE );
		}


		// 设置当前使用的称号
		CDataPool::GetMe()->Title_SetCurTitle( _TITLE::GUOJIA_TITLE, pTitle->m_CurCountryTitle );
		CDataPool::GetMe()->Title_SetCurTitle( _TITLE::BANGPAI_TITLE, pTitle->m_CurGuildTitle );
		CDataPool::GetMe()->Title_SetCurTitle( _TITLE::WANFA_TITLE, pTitle->m_CurNormalTitle );


		switch( type )
		{
		//更新所有称号信息
		case GCCharAllTitles::UPDATE_ALL:
			{
			}
			break;

		//更新帮派称号名字
		case GCCharAllTitles::UPDATE_BANGPAI_TITLE_NAME:
			break;

		//更新当前帮派称号ID
		case GCCharAllTitles::UPDATE_CUR_BANGPAI_TITLE:
			{
			}
			break;
		//更新当前国家称号ID
		case GCCharAllTitles::UPDATE_CUR_GUOJIA_TITLE:
			{
			}
			break;
		//更新当前玩法称号ID
		case GCCharAllTitles::UPDATE_CUR_WANFA_TITLE:
			{
			}
			break;
		};

	}

	//在这里检查下是否在自动寻路
	if( CGameInterface::GetMe() )
	{
		CGameInterface::GetMe()->CheckAutoMove();
	}


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}