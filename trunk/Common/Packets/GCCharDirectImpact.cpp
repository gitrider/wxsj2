
// GCCharDirectImpact.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCCharDirectImpact.h"


namespace Packets
{
	BOOL GCCharDirectImpact::Read( SocketInputStream& iStream ) 
	{
	__ENTER_FUNCTION
		iStream.Read((CHAR *)&m_nReceiverID, sizeof(m_nReceiverID));			// Ŀ������ID
		iStream.Read((CHAR *)&m_nSenderID, sizeof(m_nSenderID));				// ���ܷ����ߵ�ID
		iStream.Read((CHAR *)&m_nSenderLogicCount, sizeof(m_nSenderLogicCount));// �����ߵ��߼�����
		iStream.Read((CHAR *)&m_nImpactID, sizeof(m_nImpactID));				// Ч��ID //�ο�GameStruct_Impact.h��DIRECT_IMPACT_SEID
		iStream.Read((CHAR *)&m_nSkillID, sizeof(m_nSkillID));					// ����ID
		iStream.Read((CHAR *)(&m_iDelayTime), sizeof(m_iDelayTime));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	BOOL GCCharDirectImpact::Write( SocketOutputStream& oStream )const
	{
	__ENTER_FUNCTION

		oStream.Write((CHAR const*)&m_nReceiverID, sizeof(m_nReceiverID));				// Ŀ������ID
		oStream.Write((CHAR const*)&m_nSenderID, sizeof(m_nSenderID));					// ���ܷ����ߵ�ID
		oStream.Write((CHAR const*)&m_nSenderLogicCount, sizeof(m_nSenderLogicCount));	// �����ߵ��߼�����
		oStream.Write((CHAR const*)&m_nImpactID, sizeof(m_nImpactID));					// Ч��ID //�ο�GameStruct_Impact.h��DIRECT_IMPACT_SEID
		oStream.Write((CHAR const*)&m_nSkillID, sizeof(m_nSkillID));					// ����ID
		oStream.Write((CHAR *)(&m_iDelayTime), sizeof(m_iDelayTime));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	UINT GCCharDirectImpact::Execute( Player* pPlayer )
	{
	__ENTER_FUNCTION

		return GCCharDirectImpactHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
	}
}
