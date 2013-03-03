#ifndef _GC_EQUIP_REFINING_H_
#define _GC_EQUIP_REFINING_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCEquipRefining : public Packet
	{
	public:
		GCEquipRefining(VOID);
		virtual ~GCEquipRefining(VOID);

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_EQUIP_REFINING ; }
		virtual UINT			GetPacketSize() const 
		{ 
			return  sizeof(BYTE);
		}

		VOID			SetResult(BYTE Result)		{ m_nResult = Result; }
		BYTE			GetResult()					{ return m_nResult; }

	private:
		BYTE			m_nResult;		//Á¶»¯½á¹û
	};

	class GCEquipRefiningFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCEquipRefining() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_EQUIP_REFINING; }
		UINT		GetPacketMaxSize() const { return	sizeof(BYTE);}
	};

	class GCEquipRefiningHandler
	{
	public:
		static UINT Execute( GCEquipRefining* pPacket, Player* pPlayer );
	};
}
using namespace Packets ;
#endif // _GC_EQUIP_REFINING_H_

