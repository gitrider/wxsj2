#ifndef _GC_EQUIPDECOMPOUNDRESULT_H_
#define _GC_EQUIPDECOMPOUNDRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCEquipDecompoundResult:	public Packet
	{
	public:
		GCEquipDecompoundResult(){};
		virtual		~GCEquipDecompoundResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EQUIPDECOMPOUNDRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(EQUIPDECOMPOUND_RESULT)+sizeof(BYTE); }

		EQUIPDECOMPOUND_RESULT	GetResult(){return m_Result;}
		VOID					SetResult(EQUIPDECOMPOUND_RESULT	Ret){m_Result	= Ret;}

		VOID					SetEquipPoint(BYTE index){m_EquipPoint = index;}
		BYTE					GetEquipPoint(){return m_EquipPoint;}
	private:

		EQUIPDECOMPOUND_RESULT		m_Result;
		BYTE						m_EquipPoint;		    //成功后，丢弃物品的EquipPoint
	};

	class GCEquipDecompoundResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipDecompoundResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_EQUIPDECOMPOUNDRESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(EQUIPDECOMPOUND_RESULT)+sizeof(BYTE); }			
	};

	class GCEquipDecompoundResultHandler
	{
	public:
		static UINT Execute(GCEquipDecompoundResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif