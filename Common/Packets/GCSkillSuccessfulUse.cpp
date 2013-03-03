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
	m_nDataType					= INVALID_ID;			// 是角色使用技能还是陷阱之类的特殊对象爆炸或激活
	m_nObjID					= INVALID_ID;			// ObjID， 技能使用者或正在激活的对象
	m_nLogicCount				= INVALID_ID;			// 逻辑计数， 技能使用者或正在激活的对象的逻辑计数
	m_nSkillOrSpecialObjDataID	= INVALID_ID;			// 技能或特殊对象的资源ID
	m_posUser.m_fX				= 0;					// 使用者坐标或正在激活的对象的坐标
	m_posUser.m_fZ				= 0;
	m_nTargetID					= 0;					// 目标角色，主要是使用者需要面向的目标
	m_posTarget.m_fX			= 0;					// 目标坐标，主要是使用者需要面向的位置
	m_posTarget.m_fZ			= 0;
	m_fDir						= 0;					// 技能的方向，主要是使用者需要面向的方向
	memset( &m_HitFlagList, 0, sizeof(m_HitFlagList) );	// 目标被击中与否的标记列表，一个标记对应下面的目标列表中的一个目标对象，用索引对应。
	m_nTargetNum				= 0;					// 影响的目标数目
	memset( m_aTargetList, 0, sizeof(m_aTargetList) );	// 影响的目标ID列表

	m_SkillDataID				= INVALID_ID;			// 技能的资源ID
	m_nTotalTime				= 0;					// 总时间
	m_nImpactID					= INVALID_ID;			// 效果ID //参考GameStruct_Impact.h的DIRECT_IMPACT_SEID
	m_nSkillID					= INVALID_ID;			// 技能的ID
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
