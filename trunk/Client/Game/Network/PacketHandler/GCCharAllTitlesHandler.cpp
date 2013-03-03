
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
		// ���İ��ƺŵ���Ϣ
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
		}	// �޸İ���Ա�ƺ�



		//================
		// �������еĳƺ�

		_TITLE_ titletmp;
		INT nType = 0;

		// ͳ������
		INT numCountry = 0;	// ��������
		INT numGuild = 0;	// �������
		INT numWanfa = 0;	// �淨����
		INT num = 0;		// ����
		

		// �������еĳƺ�
		for( INT i = 0; i < MAX_TITLE_SIZE; ++i )
		{
			nType = pTitle->m_TitleArray[i].m_iTitleType;

			if( nType < 1 || nType > 3)
				continue;

			// ��ֵ
			titletmp.bFlag	=	_TITLE_::STRING_TITLE;		// �������ַ�

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
			// ���ҳƺ�
			case _TITLE::GUOJIA_TITLE:
				{
					titletmp.bType	=	_TITLE::GUOJIA_TITLE;

					pCharacterData->Set_Title( num, titletmp );
					CDataPool::GetMe()->Title_SetTitle( nType, numCountry++, num );
					num++;
				}
				break;
			// ���ɳƺ�
			case _TITLE::BANGPAI_TITLE:
				{
					titletmp.bType	=	_TITLE::BANGPAI_TITLE;

					pCharacterData->Set_Title( num, titletmp );
					CDataPool::GetMe()->Title_SetTitle( nType, numGuild++, num );
					num++;
				}
				break;
			// ��ͨ�ƺ�
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

		// ����ÿ��ƺŵ�����
		CDataPool::GetMe()->Title_SetTitleNum( _TITLE::GUOJIA_TITLE, numCountry );
		CDataPool::GetMe()->Title_SetTitleNum( _TITLE::BANGPAI_TITLE, numGuild );
		CDataPool::GetMe()->Title_SetTitleNum( _TITLE::WANFA_TITLE, numWanfa );

		// ���óƺ�����
		pCharacterData->Set_TitleNum( num );


		// ��ǰ���ɳƺ�ID
		if( CDataPool::GetMe()->Title_GetCurTitle( _TITLE::BANGPAI_TITLE ) != pTitle->m_CurGuildTitle )
		{
/*
			INT titleId = pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_iTitleID;
			if( -1 != titleId )
			{
				const _DBC_TITLE* pDbcTitle = (const _DBC_TITLE*)pTitleDBC->Search_LineNum_EQU( titleId );
				if( pDbcTitle )
				{
					// Ů
					if( 0 == pCharacterData->Get_RaceID() )
						pCharacterData->Set_CurTitle( pDbcTitle->szWomanTitle, _TITLE::BANGPAI_TITLE );
					// ��
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
				// Ů
				//if( 0 == pCharacterData->Get_RaceID() )
				//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szFemaleName, _TITLE::BANGPAI_TITLE );
				//// ��
				//else if( 1 == pCharacterData->Get_RaceID() )
				//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szMaleName, _TITLE::BANGPAI_TITLE );
			}
		}
		// ��ǰ���ҳƺ�ID
		if( -1 != pTitle->m_CurCountryTitle )
		{
			// Ů
			//if( 0 == pCharacterData->Get_RaceID() )
			//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szFemaleName, _TITLE::GUOJIA_TITLE );
			//// ��
			//else if( 1 == pCharacterData->Get_RaceID() )
			//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szMaleName, _TITLE::GUOJIA_TITLE );
		}
		if( -1 != pTitle->m_CurNormalTitle )
		{
			// Ů
			//if( 0 == pCharacterData->Get_RaceID() )
			//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szFemaleName, _TITLE::WANFA_TITLE );
			//// ��
			//else if( 1 == pCharacterData->Get_RaceID() )
			//	pCharacterData->Set_CurTitle( pTitle->m_TitleArray[ pTitle->m_CurGuildTitle ].m_szMaleName, _TITLE::WANFA_TITLE );
		}


		// ���õ�ǰʹ�õĳƺ�
		CDataPool::GetMe()->Title_SetCurTitle( _TITLE::GUOJIA_TITLE, pTitle->m_CurCountryTitle );
		CDataPool::GetMe()->Title_SetCurTitle( _TITLE::BANGPAI_TITLE, pTitle->m_CurGuildTitle );
		CDataPool::GetMe()->Title_SetCurTitle( _TITLE::WANFA_TITLE, pTitle->m_CurNormalTitle );


		switch( type )
		{
		//�������гƺ���Ϣ
		case GCCharAllTitles::UPDATE_ALL:
			{
			}
			break;

		//���°��ɳƺ�����
		case GCCharAllTitles::UPDATE_BANGPAI_TITLE_NAME:
			break;

		//���µ�ǰ���ɳƺ�ID
		case GCCharAllTitles::UPDATE_CUR_BANGPAI_TITLE:
			{
			}
			break;
		//���µ�ǰ���ҳƺ�ID
		case GCCharAllTitles::UPDATE_CUR_GUOJIA_TITLE:
			{
			}
			break;
		//���µ�ǰ�淨�ƺ�ID
		case GCCharAllTitles::UPDATE_CUR_WANFA_TITLE:
			{
			}
			break;
		};

	}

	//�����������Ƿ����Զ�Ѱ·
	if( CGameInterface::GetMe() )
	{
		CGameInterface::GetMe()->CheckAutoMove();
	}


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}