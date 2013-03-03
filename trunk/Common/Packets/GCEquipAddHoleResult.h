#ifndef _GC_EQUIPADDHOLE_RESULT_H_
#define _GC_EQUIPADDHOLE_RESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"





namespace Packets
{
	class GCEquipAddHoleResult:public Packet
	{

	public:
		GCEquipAddHoleResult(){};
		virtual				~GCEquipAddHoleResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_EQUIPADDHOLERESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE);
		}

	public:

		VOID			setResult(BYTE	Ret){ m_Result = Ret;}
		BYTE			getResult(){return m_Result;}

	private:
		BYTE				m_Result;				//是否成功，不成功则是错误号	
	};


	class GCEquipAddHoleResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipAddHoleResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_EQUIPADDHOLERESULT; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE); }
	};


	class GCEquipAddHoleResultHandler
	{
	public:
		static UINT Execute( GCEquipAddHoleResult* pPacket, Player* pPlayer );
	};



}


using namespace Packets;

#endif