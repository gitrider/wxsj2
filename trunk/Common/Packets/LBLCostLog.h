#ifndef _LBL_COSTLOG_H_
#define _LBL_COSTLOG_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class LBLCostLog : public BillPacket
	{
	public:
		enum
		{
			LB_COST_LOG,
			BL_COST_LOG,
		};

		LBLCostLog()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			memset(mCharName,0,MAX_CHARACTER_NAME+1);
			memset(mSerial,0,MAX_PRIZE_SERIAL_LENGTH+1);
			memset(mOtherInfo,0,MAX_COST_OTHER_SIZE+1);

			mAccLength = mIPLength = mCharNameLength = mOtherLength = 0;

			mZoneID = 0;
			mWorldId = 0;		
			mServerId = 0;	
			mSceneId = 0;
			mUserGUID = INVALID_ID;		
			mUserLevel = 0;
			mCostTime = 0;		
			mYuanBao = 0;		

			mType = BL_COST_LOG;
			mResult = 0;
		};
		virtual ~LBLCostLog(){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LBL_COSTLOG;}
		virtual UINT			GetPacketSize() const 
		{
			UINT uSize = 0;
			if(LB_COST_LOG == mType)
			{
				uSize = sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH
					+sizeof(INT)+sizeof(USHORT)+sizeof(INT)+sizeof(INT)+sizeof(GUID_t)+sizeof(USHORT)+sizeof(LONG)+sizeof(INT)
					+sizeof(BYTE)+sizeof(CHAR)*mAccLength
					+sizeof(BYTE)+sizeof(CHAR)*mCharNameLength
					+sizeof(BYTE)+sizeof(CHAR)*mIPLength
					+sizeof(BYTE)+sizeof(CHAR)*mOtherLength;
			}
			else if(BL_COST_LOG == mType)
			{
				uSize = sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH
					+sizeof(BYTE);
			}
			else
			{
				Assert(FALSE);
			}

			return uSize;
		}

		public:
			USHORT					GetZoneID(){return mZoneID;}
			VOID					SetZoneID(USHORT ZoneID){mZoneID=ZoneID;}
			VOID					SetCostInfo(_COST_LOG* pLog);
			
			BYTE					GetResult(){return mResult;}
			BYTE					GetType(){return mType;}
			const CHAR*				GetSerial(){return mSerial;}
		private:
			BYTE					mType;
			
			CHAR					mSerial[MAX_PRIZE_SERIAL_LENGTH+1];			//�������к�
			USHORT					mZoneID;									//����
			INT						mWorldId;									//World��
			INT						mServerId;									//Server��
			INT						mSceneId;									//Scene��
			GUID_t					mUserGUID;									//�û�GUID
			USHORT					mUserLevel;									//�û��ȼ�
			LONG					mCostTime;									//����ʱ��(��1970-01-01 ������)
			INT						mYuanBao;									//���ĵ�Ԫ����
			
			BYTE					mAccLength;
			CHAR					mAccount[MAX_BILLACCOUNT+1];				//�ʺ�
			BYTE					mCharNameLength;
			CHAR					mCharName[MAX_CHARACTER_NAME+1];			//��ɫ
			BYTE					mIPLength;
			CHAR					mIP[MAX_BILLING_IP_SIZE+1];					//IP
			BYTE					mOtherLength;
			CHAR					mOtherInfo[MAX_COST_OTHER_SIZE+1];			//��ע��Ϣ

			BYTE					mResult;
	};

	class LBLCostLogFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBLCostLog() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LBL_COSTLOG; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH
				+sizeof(INT)+sizeof(USHORT)+sizeof(INT)+sizeof(INT)+sizeof(GUID_t)+sizeof(LONG)+sizeof(INT)
				+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT
				+sizeof(BYTE)+sizeof(CHAR)*MAX_CHARACTER_NAME
				+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE
				+sizeof(BYTE)+sizeof(CHAR)*MAX_COST_OTHER_SIZE
				+sizeof(BYTE)+sizeof(USHORT);
		}
	};


	class LBLCostLogHandler 
	{
	public:
		static UINT Execute( LBLCostLog* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif