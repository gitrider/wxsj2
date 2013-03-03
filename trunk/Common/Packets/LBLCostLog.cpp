#include "stdafx.h" 
#include "LBLCostLog.h"


BOOL LBLCostLog::Read( SocketBillingInputStream& iStream ) 
{
__ENTER_FUNCTION
	//从Billing接收
	if(BL_COST_LOG == mType)
	{
		iStream.Read((CHAR*)(mSerial),sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH);
		USHORT zoneid = htons(mZoneID);
		iStream.Read((CHAR*)&mZoneID,sizeof(USHORT));
		//UINT uId1 = htonl(mWorldId);
		iStream.Read((CHAR*)&mWorldId,sizeof(UINT));
		//UINT uId2 = htonl(mServerId);
		iStream.Read((CHAR*)&mServerId,sizeof(UINT));
		//UINT uId3 = htonl(mSceneId);
		iStream.Read((CHAR*)&mSceneId,sizeof(UINT));
		//UINT uId4 = htonl(mUserGUID);
		iStream.Read((CHAR*)&mUserGUID,sizeof(UINT));
		//UINT uTime = htonl(mCostTime);
		iStream.Read((CHAR*)&mCostTime,sizeof(UINT));
		//UINT uYuanBao = htonl(mYuanBao);
		iStream.Read((CHAR*)&mYuanBao,sizeof(UINT));

		iStream.Read((CHAR*)(&mAccLength), sizeof(BYTE));
		iStream.Read((CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		iStream.Read((CHAR*)(&mCharNameLength),sizeof(BYTE));
		iStream.Read((CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
		//USHORT ulevel = htons(mUserLevel);
		iStream.Read((CHAR*)&mUserLevel,sizeof(USHORT));
		iStream.Read((CHAR*)(&mIPLength),sizeof(BYTE));
		iStream.Read((CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		iStream.Read((CHAR*)(&mOtherLength),sizeof(BYTE));
		iStream.Read((CHAR*)(mOtherInfo),sizeof(CHAR)*mOtherLength);
	}
	else
	{
		Assert(FALSE);
	}
	return TRUE;

__LEAVE_FUNCTION
	return FALSE;
}

BOOL LBLCostLog::Write( SocketBillingOutputStream& oStream )	const
{
__ENTER_FUNCTION
	//发送到Billing
	if(LB_COST_LOG == mType)
	{
		oStream.Write((CHAR*)(mSerial),sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH);
		USHORT zoneid = htons(mZoneID);
		oStream.Write((CHAR*)&zoneid,sizeof(USHORT));
		UINT uId1 = htonl(mWorldId);
		oStream.Write((CHAR*)&uId1,sizeof(UINT));
		UINT uId2 = htonl(mServerId);
		oStream.Write((CHAR*)&uId2,sizeof(UINT));
		UINT uId3 = htonl(mSceneId);
		oStream.Write((CHAR*)&uId3,sizeof(UINT));
		UINT uId4 = htonl(mUserGUID);
		oStream.Write((CHAR*)&uId4,sizeof(UINT));
		UINT uTime = htonl(mCostTime);
		oStream.Write((CHAR*)&uTime,sizeof(UINT));
		UINT uYuanBao = htonl(mYuanBao);
		oStream.Write((CHAR*)&uYuanBao,sizeof(UINT));

		oStream.Write((CHAR*)(&mAccLength), sizeof(BYTE));
		oStream.Write((CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		oStream.Write((CHAR*)(&mCharNameLength),sizeof(BYTE));
		oStream.Write((CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
		USHORT ulevel = htons(mUserLevel);
		oStream.Write((CHAR*)&ulevel,sizeof(USHORT));
		oStream.Write((CHAR*)(&mIPLength),sizeof(BYTE));
		oStream.Write((CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		oStream.Write((CHAR*)(&mOtherLength),sizeof(BYTE));
		oStream.Write((CHAR*)(mOtherInfo),sizeof(CHAR)*mOtherLength);
	}
	else
	{
		Assert(FALSE);
	}
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

UINT LBLCostLog::Execute( Player* pPlayer )
{
__ENTER_FUNCTION
	return LBLCostLogHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION
	return FALSE ;
}

VOID LBLCostLog::SetCostInfo(_COST_LOG* pLog)
{
__ENTER_FUNCTION
	Assert(pLog);

	mType = LB_COST_LOG;
	//帐户
	strncpy(mAccount,pLog->m_AccName,MAX_BILLACCOUNT);
	mAccount[MAX_BILLACCOUNT] = '\0';

	mAccLength = 0;
	for(INT i =0;i<MAX_BILLACCOUNT;i++)
	{
		if(mAccount[i]) mAccLength++;
	}

	//角色
	strncpy(mCharName,pLog->m_CharName,MAX_CHARACTER_NAME);
	mCharName[MAX_CHARACTER_NAME] = '\0';

	mCharNameLength = 0;
	for(INT i =0;i<MAX_CHARACTER_NAME;i++)
	{
		if(mCharName[i]) mCharNameLength++;
	}

	//IP
	strncpy(mIP,pLog->m_Host,MAX_BILLING_IP_SIZE);
	mIP[MAX_BILLING_IP_SIZE] = '\0';

	mIPLength = 0;
	for(INT i =0;i<MAX_BILLING_IP_SIZE;i++)
	{
		if(mIP[i]) mIPLength++;
	}

	//备注信息
	strncpy(mOtherInfo,pLog->m_OtherInfo,MAX_COST_OTHER_SIZE);
	mOtherInfo[MAX_COST_OTHER_SIZE] = '\0';

	mOtherLength = 0;
	for(INT i =0;i<MAX_COST_OTHER_SIZE;i++)
	{
		if(mOtherInfo[i]) mOtherLength++;
	}

	//消费序列号
	strncpy(mSerial,pLog->m_SerialKey,MAX_PRIZE_SERIAL_LENGTH);
	mSerial[MAX_PRIZE_SERIAL_LENGTH] = '\0';

	//World号
	mWorldId		= pLog->m_WorldId;
	//Server号
	mServerId		= pLog->m_ServerId;
	//Scene号
	mSceneId		= pLog->m_SceneId;
	//用户GUID
	mUserGUID		= pLog->m_UserGUID;
	//用户等级
	mUserLevel		= (USHORT)(pLog->m_UserLevel);
	//消费时间(自1970-01-01 的秒数)
	mCostTime		= pLog->m_CostTime;
	//消耗的元宝数
	mYuanBao		= pLog->m_YuanBao;
__LEAVE_FUNCTION
}