#ifndef _GC_EQUIPLEVELUPRESULT_H_
#define _GC_EQUIPLEVELUPRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCEquipLevelupResult:	public Packet
	{
	public:
		GCEquipLevelupResult(){};
		virtual		~GCEquipLevelupResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EQUIPLEVELUPRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(USEITEM_RESULT); }

		EQUIPLEVEUP_RESULT		GetResult(){return m_Result;}
		VOID					SetResult(EQUIPLEVEUP_RESULT	Ret){m_Result	= Ret;}
	private:

		EQUIPLEVEUP_RESULT		m_Result;

	};

	class GCEquipLevelupResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipLevelupResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_EQUIPLEVELUPRESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(USEGEM_RESULT); }			
	};

	class GCEquipLevelupResultHandler
	{
	public:
		static UINT Execute(GCEquipLevelupResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif