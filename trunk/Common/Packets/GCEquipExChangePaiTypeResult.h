#ifndef _GC_EQUIPEXCHANGEPAITYPERESULT_H_
#define _GC_EQUIPEXCHANGEPAITYPERESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCEquipExChangePaiTypeResult:	public Packet
	{
	public:
		GCEquipExChangePaiTypeResult(){};
		virtual		~GCEquipExChangePaiTypeResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EQUIPEXCHANGEPAITYPERESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(EQUIPEXCHANGEPAITTYPE_RESULT); }

		EQUIPEXCHANGEPAITTYPE_RESULT		GetResult(){return m_Result;}
		VOID					SetResult(EQUIPEXCHANGEPAITTYPE_RESULT	Ret){m_Result	= Ret;}
	private:

		EQUIPEXCHANGEPAITTYPE_RESULT			m_Result;
	};

	class GCEquipExChangePaiTypeResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipExChangePaiTypeResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_EQUIPEXCHANGEPAITYPERESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(EQUIPEXCHANGEPAITTYPE_RESULT); }			
	};

	class GCEquipExChangePaiTypeResultHandler
	{
	public:
		static UINT Execute(GCEquipExChangePaiTypeResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif