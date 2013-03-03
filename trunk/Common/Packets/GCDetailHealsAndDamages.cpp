
#include "stdafx.h"
#include "GCDetailHealsAndDamages.h"

namespace Packets
{
	BOOL GCDetailHealsAndDamages::Read( SocketInputStream& iStream ) 
	{
	__ENTER_FUNCTION
		iStream.Read((CHAR*)m_DirtyFlags.GetFlags(), m_DirtyFlags.GetByteSize());
		iStream.Read((CHAR*)&m_nReceiverID, sizeof(m_nReceiverID));
		iStream.Read((CHAR*)&m_nSenderID, sizeof(m_nSenderID));
		iStream.Read((CHAR*)&m_nSenderLogicCount, sizeof(m_nSenderLogicCount));
		if(TRUE==IsHpModificationDirty())
		{
			iStream.Read((CHAR*)&m_nHpModification, sizeof(m_nHpModification));
		}
		if(TRUE==IsMpModificationDirty())
		{
			iStream.Read((CHAR*)&m_nMpModification, sizeof(m_nMpModification));
		}
		if(TRUE==IsPneumaModificationDirty())
		{
			iStream.Read((CHAR*)&m_nPneumaModification, sizeof(m_nPneumaModification));
		}
		if(TRUE==IsRageModificationDirty())
		{
			iStream.Read((CHAR*)&m_nRageModification, sizeof(m_nRageModification));
		}
		if(TRUE==IsStrikePointModificationDirty())
		{
			iStream.Read((CHAR*)&m_nStrikePointModification, sizeof(m_nStrikePointModification));
		}
		iStream.Read((CHAR*)&m_bIsCriticalHit, sizeof(m_bIsCriticalHit));
		//���˱��
		iStream.Read((CHAR*)&m_bBeatBack, sizeof(m_bBeatBack));
		//����λ��
		iStream.Read((CHAR*)&m_BeatBackPos, sizeof(m_BeatBackPos));
		//���ɱ��
		iStream.Read((CHAR*)&m_bBeatFly, sizeof(m_bBeatFly));
		iStream.Read((CHAR*)&m_CloseToPos, sizeof(m_CloseToPos));
		iStream.Read((CHAR*)&m_fBeatFlyDistance, sizeof(m_fBeatFlyDistance));
		iStream.Read((CHAR*)&m_fAttackerAngle, sizeof(m_fAttackerAngle));
		iStream.Read((CHAR*)&m_bCloseTo, sizeof(m_bCloseTo));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	BOOL GCDetailHealsAndDamages::Write( SocketOutputStream& oStream )const
	{
	__ENTER_FUNCTION
		oStream.Write((CHAR const*)m_DirtyFlags.GetFlags(), m_DirtyFlags.GetByteSize());
		oStream.Write((CHAR const*)&m_nReceiverID, sizeof(m_nReceiverID));
		oStream.Write((CHAR const*)&m_nSenderID, sizeof(m_nSenderID));
		oStream.Write((CHAR const*)&m_nSenderLogicCount, sizeof(m_nSenderLogicCount));
		if(TRUE==IsHpModificationDirty())
		{
			oStream.Write((CHAR const*)&m_nHpModification, sizeof(m_nHpModification));
		}
		if(TRUE==IsMpModificationDirty())
		{
			oStream.Write((CHAR const*)&m_nMpModification, sizeof(m_nMpModification));
		}
		if(TRUE==IsPneumaModificationDirty())
		{
			oStream.Write((CHAR const*)&m_nPneumaModification, sizeof(m_nPneumaModification));
		}
		if(TRUE==IsRageModificationDirty())
		{
			oStream.Write((CHAR const*)&m_nRageModification, sizeof(m_nRageModification));
		}
		if(TRUE==IsStrikePointModificationDirty())
		{
			oStream.Write((CHAR const*)&m_nStrikePointModification, sizeof(m_nStrikePointModification));
		}
		oStream.Write((CHAR const*)&m_bIsCriticalHit, sizeof(m_bIsCriticalHit));
		oStream.Write((CHAR*)&m_bBeatBack, sizeof(m_bBeatBack));
		//����λ��
		oStream.Write((CHAR*)&m_BeatBackPos, sizeof(m_BeatBackPos));
		oStream.Write((CHAR*)&m_bBeatFly, sizeof(m_bBeatFly));
		oStream.Write((CHAR*)&m_CloseToPos, sizeof(m_CloseToPos));
		oStream.Write((CHAR*)&m_fBeatFlyDistance, sizeof(m_fBeatFlyDistance));
		oStream.Write((CHAR*)&m_fAttackerAngle, sizeof(m_fAttackerAngle));
		oStream.Write((CHAR*)&m_bCloseTo, sizeof(m_bCloseTo));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	UINT GCDetailHealsAndDamages::Execute( Player* pPlayer )
	{
	__ENTER_FUNCTION

		return GCDetailHealsAndDamagesHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

}
