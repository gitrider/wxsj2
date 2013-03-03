// CGBankAcquireList.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGBANKACQUIRELIST_H__
#define __CGBANKACQUIRELIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGBankAcquireList : public Packet
	{
	public:
		CGBankAcquireList( )
		{
			m_BankID = 0;
            m_cPwdSize = 0;
            memset(&m_Password, 0, MAX_PWD);
		};
		virtual ~CGBankAcquireList( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_BANKACQUIRELIST; }
		virtual UINT			GetPacketSize()const { return	2*sizeof(BYTE)+m_cPwdSize;}

	public:
		BYTE					GetBankID(VOID) const {return m_BankID;};
		VOID					SetBankID(BYTE BankID) {m_BankID = BankID;};
        BYTE                    GetPwdSize() { return m_cPwdSize; };
        VOID                    SetPwdSize(BYTE cPwdSize) { m_cPwdSize = cPwdSize; }
        const CHAR*             GetPassWord() { return m_Password; }
        VOID                    SetPassWord(const CHAR* PassWord) { memcpy(&m_Password[0], PassWord, MAX_PWD); m_Password[MAX_PWD-1] = '\0';}

	private:
		BYTE					m_BankID;		        //成功与否
        BYTE                    m_cPwdSize;             
        CHAR                    m_Password[MAX_PWD];    //银行密码
	};

	class CGBankAcquireListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGBankAcquireList() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_BANKACQUIRELIST; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2 + MAX_PWD;};
	};

	class CGBankAcquireListHandler 
	{
	public:
		static UINT Execute( CGBankAcquireList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
