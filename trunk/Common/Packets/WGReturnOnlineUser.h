
#ifndef __WGReturnOnlineUser_H__
#define __WGReturnOnlineUser_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Query.h"

namespace Packets
{
class WGReturnOnlineUser : public Packet
{
public:
	WGReturnOnlineUser( ){}
	virtual ~WGReturnOnlineUser( ){}

	//���ü̳нӿ�
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_WG_RETURNONLINEUSER; }
	virtual UINT			GetPacketSize()const { 
		return sizeof(PlayerID_t) + m_ReturnOnlineUsers.GetSize() 
			+ sizeof(m_bContinue) + sizeof(m_iReturnPostion);
	}
	
public:
	PlayerID_t			GetPlayerID(VOID) {return m_PlayerID;}
	VOID				SetPlayerID(PlayerID_t id) {m_PlayerID = id;}
	//ʹ�����ݽӿ�
	const OnlineUser_SameCity_PerPage& GetResults() const {return m_ReturnOnlineUsers;}
	VOID				SetResults(const OnlineUser_SameCity_PerPage& value){
		m_ReturnOnlineUsers = value;
	}

	VOID SetContinue(BOOL value) { m_bContinue = value; }
	BOOL GetContinue() const { return m_bContinue; }

	VOID SetReturnPostion(INT value) {m_iReturnPostion = value;}
	INT GetReturnPostion() const {return m_iReturnPostion;}


private:
	//����
	PlayerID_t			m_PlayerID;
	BOOL				m_bContinue;   // �Ƿ�����Ϣ
	OnlineUser_SameCity_PerPage m_ReturnOnlineUsers;
	INT					m_iReturnPostion; //��ѯ����Users�е����Index
};


class WGReturnOnlineUserFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new WGReturnOnlineUser() ; }
	PacketID_t	GetPacketID()const { return PACKET_WG_RETURNONLINEUSER; }
	UINT		GetPacketMaxSize()const {
		return sizeof(PlayerID_t) + sizeof(OnlineUser_SameCity_PerPage) 
			+ sizeof(BOOL) + sizeof(INT);
	}
};

class WGReturnOnlineUserHandler 
{
public:
	static UINT Execute( WGReturnOnlineUser* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif