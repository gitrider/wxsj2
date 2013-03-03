
/**	GCCountryInfoConfirm.cpp
 *
 *	���ܣ�	���������صĹ��ڹ��ҵ���Ϣ
 *
 *	�޸ļ�¼��	
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

	// ������
	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
		COUNTRY_CGW_COUNTRY_INFO* pReturn = pPacket->GetCountryInfo();
		if( !pReturn )
		{
			return PACKET_EXE_ERROR;
		}

		INT nCountryId = pReturn->m_CountryID;

		_COUNTRY country = pReturn->m_CountryInfo;


		CDataPool::GetMe()->Country_Set( (INT)_COUNTRY::COUNTRY_GUOWANG, country.m_szGuoWang );		//����
		CDataPool::GetMe()->Country_Set( (INT)_COUNTRY::COUNTRY_ZAIXIANG, country.m_szZaiXiang );	//����
		CDataPool::GetMe()->Country_Set( (INT)_COUNTRY::COUNTRY_YUANSHUAI, country.m_szYuanShuai );	//Ԫ˧
		CDataPool::GetMe()->Country_Set( (INT)_COUNTRY::COUNTRY_YUSHIDAFU, country.m_szYuShiDaFu );	//��ʷ���
		CDataPool::GetMe()->Country_Set( (INT)_COUNTRY::COUNTRY_BUTOU, country.m_szBuTou );			//��ͷ

		CEventSystem::GetMe()->PushEvent( GE_COUNTRY_MANAGER_INFO );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}


