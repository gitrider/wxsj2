

#ifndef __WGLEVELTOPLIST_H__
#define __WGLEVELTOPLIST_H__

#include "Type.h"
#include "Packet.h"
#include "GameStruct_TopList.h"
#include "PacketFactory.h"


namespace Packets
{

class WGTopList : public Packet 
{
public:
	WGTopList( ){};
	virtual ~WGTopList( ){};

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_WG_TOP_LIST; }
	virtual UINT			GetPacketSize() const { return sizeof(PlayerID_t) + m_TopList.GetSize(); }
	
public:
	VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
	PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

	//使用数据接口
	WORLD_TOP_LIST*			GetTopList( ) { return &m_TopList; }
	VOID					SetTopList( WORLD_TOP_LIST* TopList ) { m_TopList = *TopList; }

private:	
	PlayerID_t				m_PlayerID;	//连接者

	//数据
	WORLD_TOP_LIST		 m_TopList;
};


class WGTopListFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new WGTopList(); }
	PacketID_t	GetPacketID() const { return PACKET_WG_TOP_LIST; }
	UINT		GetPacketMaxSize() const { return sizeof(WORLD_TOP_LIST) +sizeof(PlayerID_t); }
};


class WGTopListHandler 
{
public:
	static UINT Execute( WGTopList* pPacket, Player* pPlayer );
};

};

using namespace Packets;

#endif
