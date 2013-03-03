// GCBankBegin.h
// 
// ���߿ͻ��������Ƿ�ɹ�
// 
//////////////////////////////////////////////////////

#ifndef __GCBANKBEGIN_H__
#define __GCBANKBEGIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCBankBegin : public Packet
	{
	public:
		GCBankBegin( )
		{
			m_BankID = 0;
			m_NPCID = 0;
		}
		virtual ~GCBankBegin( ){};

		//���ü̳нӿ�
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BANKBEGIN; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+ sizeof(ObjID_t) + sizeof(BOOL);}

	public:
		BYTE					GetBankID(VOID) const {return m_BankID;};
		VOID					SetBankID(BYTE BankID) {m_BankID = BankID;};

		ObjID_t					GetNPCObjID(VOID) const {return m_NPCID;};
		VOID					SetNPCObjID(ObjID_t NPCID) {m_NPCID = NPCID;};

        BOOL                    IsBankPwd() { return m_bHavePwd; };
        VOID                    SetBankPwdFlag (BOOL bHavePwd) { m_bHavePwd = bHavePwd; };


	private:
		BYTE					m_BankID;		//�ɹ����
		ObjID_t					m_NPCID;
        BOOL                    m_bHavePwd;     //�Ƿ�������
	};

	class GCBankBeginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBankBegin() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_BANKBEGIN; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+ sizeof(ObjID_t) + sizeof(BOOL);};
	};

	class GCBankBeginHandler 
	{
	public:
		static UINT Execute( GCBankBegin* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
