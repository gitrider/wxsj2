// GCBusRemoveAllPassenger.h
// 
/////////////////////////////////////////////////////////////

#ifndef __GCBUSREMOVEALLPASSENGER_H__
#define __GCBUSREMOVEALLPASSENGER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCBusRemoveAllPassenger :
		public Packet
	{
	public:
		GCBusRemoveAllPassenger()
		{
			m_ObjID			= INVALID_ID;
		}
		virtual ~GCBusRemoveAllPassenger()
		{
		}

		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BUSREMOVEALLPASSENGER; }
		virtual UINT			GetPacketSize()const
		{
			return	sizeof(ObjID_t);
		}

	public:
		VOID Init(ObjID_t nObjID)
		{
			m_ObjID			= nObjID;
		}

		ObjID_t GetObjID(VOID)const
		{
			return m_ObjID;
		}

	private:
		ObjID_t			m_ObjID;		// ObjID
	};


	class GCBusRemoveAllPassengerFactory :
		public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBusRemoveAllPassenger(); }
		PacketID_t	GetPacketID()const { return PACKET_GC_BUSREMOVEALLPASSENGER; }
		UINT		GetPacketMaxSize()const
		{
			return	sizeof(ObjID_t);
		}
	};


	class GCBusRemoveAllPassengerHandler 
	{
	public:
		static UINT Execute(GCBusRemoveAllPassenger* pPacket, Player* pPlayer);
	};
}

using namespace Packets;

#endif // __GCBUSREMOVEALLPASSENGER_H__
