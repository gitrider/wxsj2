
#ifndef __CGSearchOnlineUser_H__
#define __CGSearchOnlineUser_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGSearchOnlineUser : public Packet
{
public:
	CGSearchOnlineUser( ){}
	virtual ~CGSearchOnlineUser( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_SEARCHONLINEUSER ; }
	virtual UINT			GetPacketSize()const { return sizeof(INT)+sizeof(INT) ; }
	
public:
	//ʹ�����ݽӿ�
	INT					GetPostCode(VOID) { return m_PostCode; }
	VOID				SetPostCode(INT value) { m_PostCode = value; }
	INT					GetMaxPostion(){return m_iMaxPostion;}
	VOID				SetMaxPostion(INT value){m_iMaxPostion = value;}

private:
	//����
	INT					m_PostCode;	//��������ʱ��
	INT					m_iMaxPostion; //�ϴβ�ѯʱ��Users�е����Index
};


class CGSearchOnlineUserFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGSearchOnlineUser() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_SEARCHONLINEUSER; }
	UINT		GetPacketMaxSize()const { return sizeof(INT) + sizeof(INT); }
};

class CGSearchOnlineUserHandler 
{
public:
	static UINT Execute( CGSearchOnlineUser* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif