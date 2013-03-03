#ifndef _GC_EQUIPREMOULDRESULT_H_
#define _GC_EQUIPREMOULDRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCEquipRemouldResult:	public Packet
	{
	public:
		GCEquipRemouldResult(){};
		virtual		~GCEquipRemouldResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EQUIPREMOULDRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(EQUIPREMOULD_RESULT); }

		EQUIPREMOULD_RESULT		GetResult(){return m_Result;}
		VOID					SetResult(EQUIPREMOULD_RESULT	Ret){m_Result	= Ret;}
	private:

		EQUIPREMOULD_RESULT		m_Result;

	};

	class GCEquipRemouldResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipRemouldResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_EQUIPREMOULDRESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(EQUIPREMOULD_RESULT); }			
	};

	class GCEquipRemouldResultHandler
	{
	public:
		static UINT Execute(GCEquipRemouldResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif