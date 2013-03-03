#ifndef _BLRET_ACCCHECK_H_
#define _BLRET_ACCCHECK_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

enum	AccCheckResult
{
	ACR_SUCCESS				=	0x01,			//�ɹ�
	ACR_NO_ACCOUNT			=	0x02,			//�˻�������
	ACR_WRONG_PASSWORD		=	0x03,			//�������
	ACR_USER_ONLINE			=	0x04,			//��ɫ�ڱ�������
	ACR_OTHER_ONLINE		=	0x05,			//��ɫ������������
	ACR_OTHER_ERROR			=	0x06,			//��������
	ACR_FORBIT				=	0x07,			//��ֹ��¼
	ACR_NO_POINT			=	0x08,			//û���㹻����
	ACR_REG_PASSPORT		=	0x09,			//��Ҫע��SOHU��PassPort
	ACR_ACT_ACCOUNT			=	0xA0,			//�ʺ�δ����		
	ACR_REG_WEB_PASSPORT	=	0xA1,			//����WEBע��
	ACR_WRONG_IP_ADDR		=	0xB1,			//IP��ַ����
	ACR_LOCK_BY_MOBILE		=	0xB2,			//�ʻ��Ѿ��ֻ�����
	ACR_NO_MIBAO			=	0xC1,			//�û�δ�����ܱ�������
	ACR_MIBAO_NOT_USE		=	0xC2,			//�û��������ܱ������ܵ�û��ʹ��
	ACR_MIBAO_ERR			=	0xC3,			//�ܱ������벻��
	ACR_MAC_ERR				=	0xD1,			//MAC��ַMD5ֵ����
	ACR_FATIGUE_ONLYONE		=	0xE1,			//�������û�ֻ��ͬʱ��¼һ���ʺ�(1���֤->n�ʺ�)
	ACR_NO_NEW_LICENSE		=	0xF1,			//�û���û�н����µ����Э��
};

namespace Packets
{



	class BLRetAccCheck : public BillPacket
	{
	public:
		BLRetAccCheck()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			memset(mSName,0,MAX_SERVER_NAME+1);

			mIsFatigue = 'N';
			mIsPhoneBind = 'N';
			mIsIPBind = 'N';
			mIsMiBaoBind = 'N';
			mIsMacBind = 'N';
			mIsRealNameBind = 'N';
			mIsInputNameBind = 'N';
			mAccTotalOnlineSecond = 0;
		};
		virtual ~BLRetAccCheck(){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_BL_RETACCCHECK;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+mAccLength+sizeof(BYTE);
		}

		public:
			
			const	CHAR*			GetAccount()	const;
			VOID					SetAccount(const CHAR*	pAccount);

			BYTE			GetResult() const;
			VOID					SetResult(BYTE result);

			const	PlayerID_t		GetPlayerID() const;
			VOID					SetPlayerID(PlayerID_t	pID);

			const CHAR*				GetServerIP() const
			{
				return mIP;
			}

			VOID					SetServerIP(const CHAR* pServerIP)
			{
				strncpy(mIP,pServerIP,MAX_BILLING_IP_SIZE);
			}

			CHAR					GetFatigueSign(){return mIsFatigue;}
			VOID					SetFatigueSign(CHAR cSign){mIsFatigue=cSign;}

			UINT					GetTotalOnlineTime(){return mAccTotalOnlineSecond;}
			VOID					SetTotalOnlineTime(UINT uTotalSecond)
			{
				mAccTotalOnlineSecond = uTotalSecond;
			}

			CHAR					GetPhoneBind(){return mIsPhoneBind;}
			VOID					SetPhoneBind(CHAR cSign){mIsPhoneBind=cSign;}

			CHAR					GetIPBind(){return mIsIPBind;}
			VOID					SetIPBind(CHAR cSign){mIsIPBind=cSign;}

			CHAR					GetMiBaoBind(){return mIsMiBaoBind;}
			VOID					SetMiBaoBind(CHAR cSign){mIsMiBaoBind=cSign;}

			CHAR					GetMacBind(){return mIsMacBind;}
			VOID					SetMacBind(CHAR cSign){mIsMacBind=cSign;}

			CHAR					GetRealNameBind(){return mIsRealNameBind;}
			VOID					SetRealNameBind(CHAR cSign){mIsRealNameBind=cSign;}

			CHAR					GetInputNameBind(){return mIsInputNameBind;}
			VOID					SetInputNameBind(CHAR cSign){mIsInputNameBind=cSign;}
		private:
			BYTE					mAccLength;					//�û�������
			CHAR					mAccount[MAX_BILLACCOUNT+1];//�û�����
			BYTE					mAccResult;					//��֤���
		
			//��ѡ���mAccResult = 0x01��0xF1 ʱ
			UINT					mCardPoint;					//�㿨��
			USHORT					mCardDay;					//ʣ������
			UINT					mScore;						//����
			CHAR					mIsFatigue;					//�Ƿ���������
			UINT					mAccTotalOnlineSecond;		//�û��ۼ�����ʱ��
			CHAR					mIsPhoneBind;				//�Ƿ��ֻ���
			CHAR					mIsIPBind;					//�Ƿ�IP��
			CHAR					mIsMiBaoBind;				//�Ƿ��ܱ���
			CHAR					mIsMacBind;					//�Ƿ�MAC��
			CHAR					mIsRealNameBind;			//�Ƿ�ʵ����
			CHAR					mIsInputNameBind;			//�Ƿ�����ʵ��

			//mAccResult = 0x05 ʱ
			USHORT					mWorldID;					//�û����ڷ�������
			BYTE					mIPLength;					//������IP����
			CHAR					mIP[MAX_BILLING_IP_SIZE+1];	//������IP��ַ
			BYTE					mServerNameLength;			//���������ֳ���
			CHAR					mSName[MAX_SERVER_NAME+1];	//����������

	};


	class BLRetAccCheckFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new BLRetAccCheck() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_BL_RETACCCHECK; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT+sizeof(BYTE)+
				sizeof(USHORT)+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE+sizeof(BYTE)+
				sizeof(CHAR)*MAX_SERVER_NAME;
		}
	};


	class BLRetAccCheckHandler 
	{
	public:
		static UINT Execute( BLRetAccCheck* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif