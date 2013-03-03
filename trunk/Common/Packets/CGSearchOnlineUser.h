
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

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_SEARCHONLINEUSER ; }
	virtual UINT			GetPacketSize()const { return sizeof(INT)+sizeof(INT) ; }
	
public:
	//使用数据接口
	INT					GetPostCode(VOID) { return m_PostCode; }
	VOID				SetPostCode(INT value) { m_PostCode = value; }
	INT					GetMaxPostion(){return m_iMaxPostion;}
	VOID				SetMaxPostion(INT value){m_iMaxPostion = value;}

private:
	//数据
	INT					m_PostCode;	//在线玩家邮编号
	INT					m_iMaxPostion; //上次查询时在Users中的最大Index
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