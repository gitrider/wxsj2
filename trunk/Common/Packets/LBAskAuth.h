#ifndef __LBASKAUTH_H__
#define __LBASKAUTH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class LBAskAuth : public Packet 
	{
	public:
		LBAskAuth( )
		{
			memset(szAccount,0,MAX_ACCOUNT+1);
			memset(szPassWord,0,MAX_MD5_SIZE+1);
			memset(szIP,0,MAX_BILLING_IP_SIZE+1);
			for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
			{
				memset(mAllMiBaoKey[i],0,MIBAOUNIT_NAME_LENGTH+1);
				memset(mAllMiBaoValue[i],0,MIBAOUNIT_VALUE_LENGTH+1);
			}
			memset(mMacAddr,0,MAX_MD5_SIZE+1);
		} ;
		virtual ~LBAskAuth( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_LB_ASKAUTH ; }
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(CHAR)*MAX_ACCOUNT+
					sizeof(CHAR)*MAX_MD5_SIZE+
					sizeof(CHAR)*MAX_BILLING_IP_SIZE+
					sizeof(CHAR)*MIBAOUNIT_NUMBER*MIBAOUNIT_NAME_LENGTH+
					sizeof(CHAR)*MIBAOUNIT_NUMBER*MIBAOUNIT_VALUE_LENGTH+
					sizeof(CHAR)*MAX_MD5_SIZE;
		}

	public:
		//使用数据接口
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(const CHAR*	pAccount);
		const	CHAR*			GetPassWord()	const;
		VOID					SetPassWord(const CHAR*	pPassWord);

		const	PlayerID_t		GetPlayerID() const;
		VOID					SetPlayerID(PlayerID_t	pID);

		const	CHAR*			GetIPAddr()		const;
		VOID					SetIPAddr(const CHAR* pIP);

		VOID					SetMiBaoKey(INT idx, INT row, INT column)
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)				return;
			if(row < 0 || row >= MIBAO_TABLE_ROW_MAX)			return;
			if(column < 0 || column >= MIBAO_TABLE_COLUMN_MAX)	return;

			tsnprintf(mAllMiBaoKey[idx],MIBAOUNIT_NAME_LENGTH+1,"%d%d",row+1,column+1);
		}
		VOID					ClearMiBaoKey(INT idx)
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)				return;
			tsnprintf(mAllMiBaoKey[idx],MIBAOUNIT_NAME_LENGTH+1,"%d%d",0,0);	//key为"00"，通知Billing玩家没有选择“使用密保卡”的选项
		}
		const	CHAR*			GetMiBaoKey(INT idx)	const
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)	return	NULL;
			return mAllMiBaoKey[idx];
		}

		VOID					SetMiBaoValue(INT idx, const CHAR* pValue)
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)	return;
			strncpy(mAllMiBaoValue[idx],pValue,MIBAOUNIT_VALUE_LENGTH);
		}

		const	CHAR*			GetMiBaoValue(INT idx)	const
		{
			if(idx < 0 || idx >= MIBAOUNIT_NUMBER)	return	NULL;
			return mAllMiBaoValue[idx];
		}

		const	CHAR*			GetMacAddr()	const;
		VOID					SetMacAddr(CHAR*	pMacMD5);
	private:
		//数据
		CHAR					szAccount[MAX_ACCOUNT+1] ;	//用户名称
		CHAR					szPassWord[MAX_MD5_SIZE+1];		//用户密码
		CHAR					szIP[MAX_BILLING_IP_SIZE+1];	//用户IP地址
		PlayerID_t				PlayerID;
		
		CHAR					mAllMiBaoKey[MIBAOUNIT_NUMBER][MIBAOUNIT_NAME_LENGTH+1];	//密保名
		CHAR					mAllMiBaoValue[MIBAOUNIT_NUMBER][MIBAOUNIT_VALUE_LENGTH+1];	//密保值
		CHAR					mMacAddr[MAX_MD5_SIZE+1];	//用户电脑的MAC地址MD5
	};

	class LBAskAuthFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new LBAskAuth() ; }
		PacketID_t	GetPacketID()const { return PACKET_LB_ASKAUTH ; }
		UINT		GetPacketMaxSize() const
		{ 
			return	sizeof(CHAR)*MAX_ACCOUNT+
				sizeof(CHAR)*MAX_MD5_SIZE+
				sizeof(CHAR)*MAX_BILLING_IP_SIZE+
				sizeof(CHAR)*MIBAOUNIT_NUMBER*MIBAOUNIT_NAME_LENGTH+
				sizeof(CHAR)*MIBAOUNIT_NUMBER*MIBAOUNIT_VALUE_LENGTH+
				sizeof(CHAR)*MAX_MD5_SIZE;
		}
	};


	class LBAskAuthHandler 
	{
	public:
		static UINT Execute( LBAskAuth* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif