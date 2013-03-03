
/**	GCCountryInfoConfirm.cpp
 *
 *	功能：	服务器返回的关于国家的消息
 *
 *	修改记录：	
 *			
 *			
 *			
 */

#include "StdAfx.h"
#include "GCCountryInfoConfirm.h"
#include "GameStruct_Country.h"

#include "Procedure/GameProcedure.h"
#include "DataPool/GMDataPool.h"
#include "Event/GMEventSystem.h"



uint GCCountryInfoConfirmHandler::Execute( GCCountryInfoConfirm* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	// 主流程
	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
		COUNTRY_CGW_COUNTRY_INFO* pReturn = pPacket->GetCountryInfo();
		if( !pReturn )
		{
			return PACKET_EXE_ERROR;
		}

		INT nCountryId = pReturn->m_CountryID;

		_COUNTRY country = pReturn->m_CountryInfo;


		CDataPool::GetMe()->Country_Set( (INT)_COUNTRY::COUNTRY_GUOWANG, country.m_szGuoWang );		//国王
		CDataPool::GetMe()->Country_Set( (INT)_COUNTRY::COUNTRY_ZAIXIANG, country.m_szZaiXiang );	//宰相
		CDataPool::GetMe()->Country_Set( (INT)_COUNTRY::COUNTRY_YUANSHUAI, country.m_szYuanShuai );	//元帅
		CDataPool::GetMe()->Country_Set( (INT)_COUNTRY::COUNTRY_YUSHIDAFU, country.m_szYuShiDaFu );	//御史大夫
		CDataPool::GetMe()->Country_Set( (INT)_COUNTRY::COUNTRY_BUTOU, country.m_szBuTou );			//捕头

		CEventSystem::GetMe()->PushEvent( GE_COUNTRY_MANAGER_INFO );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


