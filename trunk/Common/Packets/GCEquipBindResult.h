#ifndef _GC_EQUIPBINDRESULT_H_
#define _GC_EQUIPBINDRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCEquipBindResult:	public Packet
	{
	public:
		GCEquipBindResult(){};
		virtual		~GCEquipBindResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EQUIPBINDRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(EQUIPBIND_RESULT); }

		EQUIPBIND_RESULT		GetResult(){return m_Result;}
		VOID					SetResult(EQUIPBIND_RESULT	Ret){m_Result	= Ret;}
	private:

		EQUIPBIND_RESULT			m_Result;

	};

	class GCEquipBindResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipBindResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_EQUIPBINDRESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(EQUIPBIND_RESULT); }			
	};

	class GCEquipBindResultHandler
	{
	public:
		static UINT Execute(GCEquipBindResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif