
/**	GCCountryReturn.cpp
 *
 *	���ܣ�	���������صĹ��ڹ��ҵ���Ϣ
 *
 *	�޸ļ�¼��	
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

	// ������
	if( CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain )
	{
		_COUNTRY_RETURN* pReturn = pPacket->GetCountryReturn();
		if( !pReturn )
		{
			return PACKET_EXE_ERROR;
		}
		

		pReturn->m_cCountry;	// ����


		// ����ͬ���͵��¼�
		switch( pReturn->m_ReturnType )
		{
		// ���󵱹���
		case COUNTRY_RETURN_REQUEST_KINDOM:		// COUNTRY_RETURN_TYPE
			{
				break;
			}

		// �����ְ
		case COUNTRY_RETURN_APPOINT:
			{
				CDataPool::GetMe()->Country_Set( pReturn->m_cPosition, pReturn->m_szName );

				CEventSystem::GetMe()->PushEvent( GE_COUNTRY_MANAGER_ADJUST_AUTH );

				break;
			}

		// �����ְ
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
