// GCBusRemovePassenger.h
// 
/////////////////////////////////////////////////////////////

#ifndef __GCBUSREMOVEPASSENGER_H__
#define __GCBUSREMOVEPASSENGER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCBusRemovePassenger :
		public Packet
	{
	public:
		GCBusRemovePassenger()
		{
			m_ObjID			= INVALID_ID;
			m_nPassengerID	= INVALID_ID;
		}
		virtual ~GCBusRemovePassenger()
		{
		}

		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BUSREMOVEPASSENGER; }
		virtual UINT			GetPacketSize()const
		{
			return	sizeof(ObjID_t)
				+ sizeof(ObjID_t);
		}

	public:
		VOID Init(ObjID_t nObjID, ObjID_t nPassengerID)
		{
			m_ObjID			= nObjID;
			m_nPassengerID	= nPassengerID;
		}

		ObjID_t GetObjID(VOID)const
		{
			return m_ObjID;
		}

		ObjID_t GetPassengerID(VOID)const
		{
			return m_nPassengerID;
		}

	private:
		ObjID_t			m_ObjID;			// ObjID
		ObjID_t			m_nPassengerID;		// 乘客ID
	};


	class GCBusRemovePassengerFactory :
		public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBusRemovePassenger(); }
		PacketID_t	GetPacketID()const { return PACKET_GC_BUSREMOVEPASSENGER; }
		UINT		GetPacketMaxSize()const
		{
			return	sizeof(ObjID_t)
				+ sizeof(ObjID_t);
		}
	};


	class GCBusRemovePassengerHandler 
	{
	public:
		static UINT Execute(GCBusRemovePassenger* pPacket, Player* pPlayer);
	};
}

using namespace Packets;

#endif // __GCBUSREMOVEPASSENGER_H__
