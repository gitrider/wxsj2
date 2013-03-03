// GCTargetListAndHitFlags.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCSkillSuccessfulUse.h"

using namespace Packets;

Packets::GCSkillSuccessfulUse::GCSkillSuccessfulUse()
{
	Cleanup();
}

Packets::GCSkillSuccessfulUse::~GCSkillSuccessfulUse()
{

}


VOID Packets::GCSkillSuccessfulUse::Cleanup()
{
	m_nDataType					= INVALID_ID;			// �ǽ�ɫʹ�ü��ܻ�������֮����������ը�򼤻�
	m_nObjID					= INVALID_ID;			// ObjID�� ����ʹ���߻����ڼ���Ķ���
	m_nLogicCount				= INVALID_ID;			// �߼������� ����ʹ���߻����ڼ���Ķ�����߼�����
	m_nSkillOrSpecialObjDataID	= INVALID_ID;			// ���ܻ�����������ԴID
	m_posUser.m_fX				= 0;					// ʹ������������ڼ���Ķ��������
	m_posUser.m_fZ				= 0;
	m_nTargetID					= 0;					// Ŀ���ɫ����Ҫ��ʹ������Ҫ�����Ŀ��
	m_posTarget.m_fX			= 0;					// Ŀ�����꣬��Ҫ��ʹ������Ҫ�����λ��
	m_posTarget.m_fZ			= 0;
	m_fDir						= 0;					// ���ܵķ�����Ҫ��ʹ������Ҫ����ķ���
	memset( &m_HitFlagList, 0, sizeof(m_HitFlagList) );	// Ŀ�걻�������ı���б�һ����Ƕ�Ӧ�����Ŀ���б��е�һ��Ŀ�������������Ӧ��
	m_nTargetNum				= 0;					// Ӱ���Ŀ����Ŀ
	memset( m_aTargetList, 0, sizeof(m_aTargetList) );	// Ӱ���Ŀ��ID�б�

	m_SkillDataID				= INVALID_ID;			// ���ܵ���ԴID
	m_nTotalTime				= 0;					// ��ʱ��
	m_nImpactID					= INVALID_ID;			// Ч��ID //�ο�GameStruct_Impact.h��DIRECT_IMPACT_SEID
	m_nSkillID					= INVALID_ID;			// ���ܵ�ID
	m_bIsCriticalHit			= FALSE;
	m_bBeatBack					= FALSE;
	m_bBeatFly					= FALSE;
	m_BeatBackPos.m_fX			= 0;
	m_BeatBackPos.m_fZ			= 0;
	m_CloseToPos				= m_BeatBackPos;
	m_fBeatFlyDistance			= 5.0f;
	m_fAttackerAngle			= 0.0f;
	m_bCloseTo					= FALSE;
	m_DirtyFlags.ClearAllFlags();

}

BOOL GCSkillSuccessfulUse::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nDataType),	sizeof(m_nDataType));
	iStream.Read( (CHAR*)(&m_nObjID),		sizeof(m_nObjID));
	iStream.Read( (CHAR*)(&m_nLogicCount),	sizeof(m_nLogicCount));
	iStream.Read( (CHAR*)(&m_nSkillOrSpecialObjDataID), sizeof(m_nSkillOrSpecialObjDataID));
	iStream.Read( (CHAR*)(&m_posUser),		sizeof(m_posUser));
	iStream.Read( (CHAR*)(&m_nTargetID),	sizeof(m_nTargetID));
	iStream.Read( (CHAR*)(&m_posTarget),	sizeof(m_posTarget));
	iStream.Read( (CHAR*)(&m_fDir),			sizeof(m_fDir));
	iStream.Read( (CHAR*)(&m_nTargetNum),	sizeof(m_nTargetNum));
	iStream.Read( (CHAR*)(m_HitFlagList.GetFlags()), m_HitFlagList.GetByteSize());

	for(INT nIdx=0; m_nTargetNum>nIdx; ++nIdx)
	{
		iStream.Read( (CHAR*)(&m_aTargetList[nIdx]),   sizeof(m_aTargetList[nIdx]));
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GCSkillSuccessfulUse::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_nDataType), sizeof(m_nDataType));
	oStream.Write( (CHAR*)(&m_nObjID), sizeof(m_nObjID));
	oStream.Write( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	oStream.Write( (CHAR*)(&m_nSkillOrSpecialObjDataID), sizeof(m_nSkillOrSpecialObjDataID));
	oStream.Write( (CHAR*)(&m_posUser), sizeof(m_posUser));
	oStream.Write( (CHAR*)(&m_nTargetID), sizeof(m_nTargetID));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(m_posTarget));
	oStream.Write( (CHAR*)(&m_fDir), sizeof(m_fDir));
	oStream.Write( (CHAR*)(&m_nTargetNum), sizeof(m_nTargetNum));
	oStream.Write( (CHAR*)(m_HitFlagList.GetFlags()), m_HitFlagList.GetByteSize());

	for(INT nIdx=0; m_nTargetNum>nIdx; ++nIdx)
	{
		oStream.Write( (CHAR*)(&m_aTargetList[nIdx]),   sizeof(m_aTargetList[nIdx]));
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCSkillSuccessfulUse::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return 0;//GCTargetListAndHitFlagsHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


UINT Packets::GCSkillSuccessfulUse::GetBallIDByIndex( INT nIdx )
{
	if(0>nIdx || MAX_TARGET_LIST_SIZE<=nIdx)
	{
		return INVALID_ID;
	}

	return m_aTargetList[nIdx].m_uBallUniqueID;
}

VOID Packets::GCSkillSuccessfulUse::SetBallIDByIndex( INT nIdx, UINT uBallID )
{
	if(0<=nIdx && MAX_TARGET_LIST_SIZE>nIdx)
	{
		m_aTargetList[nIdx].m_uBallUniqueID = uBallID;
	}
}

ObjID_t Packets::GCSkillSuccessfulUse::GetTargetObjIDByIndex( INT nIdx ) const
{
	if(0>nIdx || MAX_TARGET_LIST_SIZE<=nIdx)
	{
		return INVALID_ID;
	}
	return m_aTargetList[nIdx].m_nTargetID;
}

VOID Packets::GCSkillSuccessfulUse::SetTargetObjIDByIndex( INT nIdx, ObjID_t nObjID )
{
	if(0<=nIdx && MAX_TARGET_LIST_SIZE>nIdx)
	{
		m_aTargetList[nIdx].m_nTargetID = nObjID;
	}
}

UINT Packets::GCSkillSuccessfulUse::GetPacketSize() const
{
	//GCTargetListAndHitFlags
	UINT nSize =	sizeof(m_nDataType) +
		sizeof(m_nObjID) +
		sizeof(m_nLogicCount) +
		sizeof(m_nSkillOrSpecialObjDataID) +
		sizeof(m_posUser) +
		sizeof(m_nTargetID) +
		sizeof(m_posTarget) +
		sizeof(m_fDir) +
		sizeof(m_HitFlagList) +
		sizeof(m_nTargetNum) +
		sizeof(m_aTargetList[0])*m_nTargetNum;

	//GCCharDirectImpact
	nSize +=

			sizeof(m_nImpactID)
		+	sizeof(m_nSkillID);

	//GCCharSkill_Send
	nSize +=	
		sizeof(INT) +
		sizeof(INT);

	return nSize;
}
