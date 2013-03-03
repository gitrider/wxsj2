#ifndef _GC_EQUIPCANCELMAGICRESULT_H_
#define _GC_EQUIPCANCELMAGICRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCEquipCancelMagicResult:	public Packet
	{
	public:
		GCEquipCancelMagicResult(){};
		virtual		~GCEquipCancelMagicResult(){};

		// 公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;
		
		virtual PacketID_t		GetPacketID()const			{ return PACKET_GC_EQUIPCANCELMAGICRESULT; }
		virtual UINT			GetPacketSize()const		{ return sizeof(EQUIPCANCELMAGIC_RESULT); }

		VOID					setResult(EQUIPCANCELMAGIC_RESULT Result)		{ m_Result = Result; }
		EQUIPCANCELMAGIC_RESULT	getResult()					{ return m_Result; }

	private:
		EQUIPCANCELMAGIC_RESULT	m_Result;

	};

	class GCEquipCancelMagicResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipCancelMagicResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_EQUIPCANCELMAGICRESULT; }
		UINT		GetPacketMaxSize()const		{ return sizeof(EQUIPCANCELMAGIC_RESULT); }			
	};

	class GCEquipCancelMagicResultHandler
	{
	public:
		static UINT	Execute(GCEquipCancelMagicResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif