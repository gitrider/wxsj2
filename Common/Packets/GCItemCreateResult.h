#ifndef _GC_ITEMCREATERESULT_H_
#define _GC_ITEMCREATERESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCItemCreateResult:	public Packet
	{
	public:
		GCItemCreateResult(){};
		virtual		~GCItemCreateResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_ITEMCREATERESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(USEITEM_RESULT); }

		ITEMCREATE_RESULT		GetResult(){return m_Result;}
		VOID					SetResult(ITEMCREATE_RESULT	Ret){m_Result	= Ret;}
	private:

		ITEMCREATE_RESULT		m_Result;

	};

	class GCItemCreateResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCItemCreateResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_ITEMCREATERESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(ITEMCREATE_RESULT); }			
	};

	class GCItemCreateResultHandler
	{
	public:
		static UINT Execute(GCItemCreateResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif