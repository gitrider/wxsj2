#ifndef _GCLEVELTOPLIST_H_
#define _GCLEVELTOPLIST_H_


#include "Type.h"
#include "Packet.h"
#include "GameStruct_TopList.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCTopList:public Packet
	{
	public:
		GCTopList(){};
		virtual ~GCTopList(){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_TOP_LIST; }
		virtual UINT			GetPacketSize() const { return m_TopList.GetSize(); }
		
	public:
		//使用数据接口
		WORLD_TOP_LIST*			GetTopList( ) { return &m_TopList; }
		VOID					SetTopList( WORLD_TOP_LIST* TopList ) { m_TopList = *TopList; }

	private:
		//数据
		WORLD_TOP_LIST		 m_TopList;

	};

	class GCTopListFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCTopList() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_TOP_LIST ; }
		UINT		GetPacketMaxSize()const		{ return	sizeof(WORLD_TOP_LIST); }			
	};

	class GCTopListHandler
	{
	public:
		static UINT Execute(GCTopList* pPacket,Player* pPlayer);
	};

};

using namespace Packets;


#endif