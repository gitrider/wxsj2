
/**	GCCountryReturn.cpp
 *
 *	功能：	服务器返回的关于国家的消息
 *
 *	修改记录：	
 *			
 *			
 *			
 */

#include "StdAfx.h"
#include "GCCountryReturn.h"

#include "Procedure/GameProcedure.h"
#include "Event/GMEventSystem.h"
#include "DataPool\GMDataPool.h"



uint GCCountryReturnHandler::Execute( GCCountryReturn* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	// 主流程
	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
		_COUNTRY_RETURN* pReturn = pPacket->GetCountryReturn();
		if( !pReturn )
		{
			return PACKET_EXE_ERROR;
		}
		

		pReturn->m_cCountry;	// 国家


		// 处理不同类型的事件
		switch( pReturn->m_ReturnType )
		{
		// 请求当国王
		case COUNTRY_RETURN_REQUEST_KINDOM:		// COUNTRY_RETURN_TYPE
			{
				break;
			}

		// 任免官职
		case COUNTRY_RETURN_APPOINT:
			{
				CDataPool::GetMe()->Country_Set( pReturn->m_cPosition, pReturn->m_szName );

				CEventSystem::GetMe()->PushEvent( GE_COUNTRY_MANAGER_ADJUST_AUTH );

				break;
			}

		// 罢免官职
		case COUNTRY_RETURN_REMOVE:
			{
				CDataPool::GetMe()->Country_Set( pReturn->m_cPosition, "" );

				CEventSystem::GetMe()->PushEvent( GE_COUNTRY_MANAGER_ADJUST_AUTH );

				break;
			}
		};

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
