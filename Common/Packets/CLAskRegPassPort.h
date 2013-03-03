#ifndef __CLASKREGPASSPORT_H__
#define __CLASKREGPASSPORT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{


	class CLAskRegPassPort : public Packet 
	{
	public:
		CLAskRegPassPort( )
		{
			memset(szAccount,0,MAX_ACCOUNT+1);
			memset(szSuperPw,0,MAX_SUPER_CODE_SIZE+1);
			memset(szConfirmSuperPw,0,MAX_SUPER_CODE_SIZE+1);
			memset(szPwMd5,0,MAX_MD5_SIZE+1);
			memset(szConfirmPwMd5,0,MAX_MD5_SIZE+1);
			memset(szEmail,0,MAX_EMAIL_SIZE+1);
		};
		virtual ~CLAskRegPassPort( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CL_ASKREGPASSPORT ; }
		virtual UINT			GetPacketSize() const 
		{
			return 	sizeof(CHAR)*MAX_ACCOUNT+sizeof(CHAR)*MAX_SUPER_CODE_SIZE
				+sizeof(CHAR)*MAX_SUPER_CODE_SIZE+sizeof(CHAR)*MAX_MD5_SIZE
				+sizeof(CHAR)*MAX_MD5_SIZE+sizeof(CHAR)*MAX_EMAIL_SIZE;
		}

	public:
		//使用数据接口
		const	CHAR*			GetAccount()	const;
		VOID					SetAccount(CHAR*	pAccount);
		const	CHAR*			GetSuperPw()	const;
		VOID					SetSuperPw(CHAR*	pSuperPw);
		const	CHAR*			GetConfirmSuperPw()	const;
		VOID					SetConfirmSuperPw(CHAR*	pConfirmSuperPw);
		const	CHAR*			GetPwMd5()	const;
		VOID					SetPwMd5(CHAR*	pPassWord);
		const	CHAR*			GetConfirmPwMd5()	const;
		VOID					SetConfirmPwMd5(CHAR*	pConfirmPassWord);
		const	CHAR*			GetEmail() const;
		VOID					SetEmail(CHAR* pEmail);
	private:
		//数据
		CHAR					szAccount[MAX_ACCOUNT+1] ;					//用户名称
		CHAR					szSuperPw[MAX_SUPER_CODE_SIZE+1];			//用户设定的超级密码		明文
		CHAR					szConfirmSuperPw[MAX_SUPER_CODE_SIZE+1];	//用户设定的超级密码确认	明文
		CHAR					szPwMd5[MAX_MD5_SIZE+1];					//用户设定的游戏密码		MD5
		CHAR					szConfirmPwMd5[MAX_MD5_SIZE+1];				//用户设定的游戏密码确认	MD5
		CHAR					szEmail[MAX_EMAIL_SIZE+1];					//用户Email
	};

	class CLAskRegPassPortFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CLAskRegPassPort() ; }
		PacketID_t	GetPacketID()const { return PACKET_CL_ASKREGPASSPORT ; }
		UINT		GetPacketMaxSize() const
		{ 
			return 	sizeof(CHAR)*MAX_ACCOUNT+sizeof(CHAR)*MAX_SUPER_CODE_SIZE
					+sizeof(CHAR)*MAX_SUPER_CODE_SIZE+sizeof(CHAR)*MAX_MD5_SIZE
					+sizeof(CHAR)*MAX_MD5_SIZE+sizeof(CHAR)*MAX_EMAIL_SIZE;
		}
	};


	class CLAskRegPassPortHandler 
	{
	public:
		static UINT Execute( CLAskRegPassPort* pPacket, Player* pPlayer ) ;
	};

};

using namespace Packets ;


#endif