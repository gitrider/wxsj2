#ifndef _GC_EQUIPCHANGEPAITYPERESULT_H_
#define _GC_EQUIPCHANGEPAITYPERESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCEquipChangePaiTypeResult:	public Packet
	{
	public:
		GCEquipChangePaiTypeResult(){};
		virtual		~GCEquipChangePaiTypeResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EQUIPCHANGEPAITYPERESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(EQUIPCHANGEPAITTYPE_RESULT); }

		EQUIPCHANGEPAITTYPE_RESULT		GetResult(){return m_Result;}
		VOID					SetResult(EQUIPCHANGEPAITTYPE_RESULT	Ret){m_Result	= Ret;}
	private:

		EQUIPCHANGEPAITTYPE_RESULT			m_Result;

	};

	class GCEquipChangePaiTypeResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipChangePaiTypeResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_EQUIPCHANGEPAITYPERESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(EQUIPCHANGEPAITTYPE_RESULT); }			
	};

	class GCEquipChangePaiTypeResultHandler
	{
	public:
		static UINT Execute(GCEquipChangePaiTypeResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif