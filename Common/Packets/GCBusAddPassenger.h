// GCBusAddPassenger.h
// 
/////////////////////////////////////////////////////////////

#ifndef __GCBUSADDPASSENGER_H__
#define __GCBUSADDPASSENGER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCBusAddPassenger :
		public Packet
	{
	public:
		GCBusAddPassenger()
		{
			m_ObjID			= INVALID_ID;
			m_nIndex		= -1;
			m_nPassengerID	= INVALID_ID;
		}
		virtual ~GCBusAddPassenger()
		{
		}

		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BUSADDPASSENGER; }
		virtual UINT			GetPacketSize()const
		{
			return	sizeof(ObjID_t)
				+ sizeof(INT)
				+ sizeof(ObjID_t);
		}

	public:
		VOID Init(ObjID_t nObjID, INT nIndex, ObjID_t nPassengerID)
		{
			m_ObjID			= nObjID;
			m_nIndex		= nIndex;
			m_nPassengerID	= nPassengerID;
		}

		ObjID_t GetObjID(VOID)const
		{
			return m_ObjID;
		}

		INT GetIndex(VOID)const
		{
			return m_nIndex;
		}

		ObjID_t GetPassengerID(VOID)const
		{
			return m_nPassengerID;
		}

	private:
		ObjID_t			m_ObjID;			// ObjID
		INT				m_nIndex;			// 乘客列表的索引
		ObjID_t			m_nPassengerID;		// 乘客ID
	};


	class GCBusAddPassengerFactory :
		public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBusAddPassenger(); }
		PacketID_t	GetPacketID()const { return PACKET_GC_BUSADDPASSENGER; }
		UINT		GetPacketMaxSize()const
		{
			return	sizeof(ObjID_t)
				+ sizeof(INT)
				+ sizeof(ObjID_t);
		}
	};


	class GCBusAddPassengerHandler 
	{
	public:
		static UINT Execute(GCBusAddPassenger* pPacket, Player* pPlayer);
	};
}

using namespace Packets;

#endif // __GCBUSADDPASSENGER_H__
