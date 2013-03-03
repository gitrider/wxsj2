#ifndef _LB_REGPASSPORT_H_
#define _LB_REGPASSPORT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class LBRegPassPort : public BillPacket
	{
	public:
		LBRegPassPort()
		{
			memset(mAccount,0,MAX_BILLACCOUNT+1);
			memset(mSuperPw,0,MAX_SUPER_CODE_SIZE+1);
			memset(mPwMd5,0,MAX_MD5_SIZE+1);
			memset(mIP,0,MAX_BILLING_IP_SIZE+1);
			memset(mEmail,0,MAX_EMAIL_SIZE+1);

			memset(mOtherInfo1,0,MAX_OTHER_INFO_SIZE+1);
			memset(mOtherInfo2,0,MAX_OTHER_INFO_SIZE+1);
			memset(mOtherInfo3,0,MAX_OTHER_INFO_SIZE+1);

			mAccLength = mSuperPwLength = mPwLength = mIPLength = mEmailLength = 0;
			mOtherInfo1Length = mOtherInfo2Length = mOtherInfo3Length = 0;
		};
		virtual ~LBRegPassPort(){};

		//公用继承接口
		virtual BOOL			Read( SocketBillingInputStream& iStream ) ;
		virtual BOOL			Write( SocketBillingOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual BPacketID_t		GetPacketID() const { return BPACKET_LB_REGPASSPORT;}
		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(CHAR)*mAccLength
				+sizeof(BYTE)+sizeof(CHAR)*mSuperPwLength
				+sizeof(BYTE)+sizeof(CHAR)*mPwLength
				+sizeof(BYTE)+sizeof(CHAR)*mIPLength
				+sizeof(BYTE)+sizeof(CHAR)*mEmailLength
				+sizeof(USHORT)+sizeof(CHAR)*mOtherInfo1Length
				+sizeof(USHORT)+sizeof(CHAR)*mOtherInfo2Length
				+sizeof(USHORT)+sizeof(CHAR)*mOtherInfo3Length;
		}

		public:
			//使用数据接口
			BYTE					GetAccLength()
			{
				return mAccLength;
			}

			const CHAR*				GetAccount()
			{
				return mAccount;
			}
			VOID					SetAccount(const CHAR* pAcc)
			{
				strncpy(mAccount,pAcc,MAX_BILLACCOUNT);
				mAccount[MAX_BILLACCOUNT] = '\0';

				mAccLength = 0;
				for(INT i =0;i<MAX_BILLACCOUNT;i++)
				{
					if(mAccount[i]) mAccLength++;
				}
			}

			BYTE					GetSuperPwLength()
			{
				return mSuperPwLength;
			}

			VOID					SetSuperPassWord(const CHAR* pSuperPw)
			{
				strncpy(mSuperPw,pSuperPw,MAX_SUPER_CODE_SIZE);
				mSuperPwLength = 0;
				for(INT i =0;i<MAX_SUPER_CODE_SIZE;i++)
				{
					if(mSuperPw[i]) mSuperPwLength++;
				}
			}

			BYTE					GetPwLength()
			{
				return mPwLength;
			}

			VOID					SetPassWord(const CHAR* pPw)
			{
				strncpy(mPwMd5,pPw,MAX_MD5_SIZE);
				mPwLength = 0;
				for(INT i =0;i<MAX_MD5_SIZE;i++)
				{
					if(mPwMd5[i]) mPwLength++;
				}
			}

			BYTE					GetIPLength()
			{
				return	mIPLength;
			}

			VOID					SetIPAddr(const CHAR* pIP)
			{
				strncpy(mIP,pIP,MAX_BILLING_IP_SIZE);
				mIPLength = 0;
				for(INT i =0;i<MAX_BILLING_IP_SIZE;i++)
				{
					if(mIP[i]) mIPLength++;
				}
			}

			BYTE					GetEmailLength()
			{
				return	mEmailLength;
			}

			VOID					SetEmail(const CHAR* pEmail)
			{
				strncpy(mEmail,pEmail,MAX_EMAIL_SIZE);
				mEmailLength = 0;
				for(INT i =0;i<MAX_EMAIL_SIZE;i++)
				{
					if(mEmail[i]) mEmailLength++;
				}
			}

			//额外信息接口暂不添加，需要时再添加吧
		private:
			BYTE					mAccLength;								//用户名长度
			CHAR					mAccount[MAX_BILLACCOUNT+1];			//用户名串
			BYTE					mSuperPwLength;							//超级密码长度
			CHAR					mSuperPw[MAX_SUPER_CODE_SIZE+1];		//超级密码明文
			BYTE					mPwLength;								//密码长度
			CHAR					mPwMd5[MAX_MD5_SIZE+1];					//密码md5
			BYTE					mIPLength;								//用户IP长度
			CHAR					mIP[MAX_BILLING_IP_SIZE+1];				//用户IP地址
			BYTE					mEmailLength;							//email长度
			CHAR					mEmail[MAX_EMAIL_SIZE+1];				//email
			//备用段，留待以后扩展使用
			USHORT					mOtherInfo1Length;						//额外信息1长度
			CHAR					mOtherInfo1[MAX_OTHER_INFO_SIZE+1];		//额外信息
			USHORT					mOtherInfo2Length;						//额外信息2长度
			CHAR					mOtherInfo2[MAX_OTHER_INFO_SIZE+1];		//额外信息
			USHORT					mOtherInfo3Length;						//额外信息3长度
			CHAR					mOtherInfo3[MAX_OTHER_INFO_SIZE+1];		//额外信息
	};


	class LBRegPassPortFactory : public BillPacketFactory 
	{
	public:
		BillPacket*		CreatePacket()		{ return new LBRegPassPort() ; }
		BPacketID_t		GetPacketID() const { return BPACKET_LB_REGPASSPORT; }
		UINT			GetPacketMaxSize() const
		{ 
			return sizeof(BYTE)+sizeof(CHAR)*MAX_BILLACCOUNT
				+sizeof(BYTE)+sizeof(CHAR)*MAX_SUPER_CODE_SIZE
				+sizeof(BYTE)+sizeof(CHAR)*MAX_MD5_SIZE
				+sizeof(BYTE)+sizeof(CHAR)*MAX_BILLING_IP_SIZE
				+sizeof(BYTE)+sizeof(CHAR)*MAX_EMAIL_SIZE
				+sizeof(USHORT)+sizeof(CHAR)*MAX_OTHER_INFO_SIZE
				+sizeof(USHORT)+sizeof(CHAR)*MAX_OTHER_INFO_SIZE
				+sizeof(USHORT)+sizeof(CHAR)*MAX_OTHER_INFO_SIZE;
		}
	};


	class LBRegPassPortHandler 
	{
	public:
		static UINT Execute( LBRegPassPort* pPacket, Player* pPlayer ) ;
	};

}

using namespace Packets ;


#endif