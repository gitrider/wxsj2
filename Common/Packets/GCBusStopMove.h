// GCBusStopMove.h
// 
/////////////////////////////////////////////////////////////

#ifndef __GCBUSSTOPMOVE_H__
#define __GCBUSSTOPMOVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCBusStopMove :
		public Packet
	{
	public:
		GCBusStopMove()
		{
			m_ObjID			= INVALID_ID;
			//m_posWorld;
		}
		virtual ~GCBusStopMove()
		{
		}

		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BUSSTOPMOVE; }
		virtual UINT			GetPacketSize()const
		{
			return	sizeof(ObjID_t)
				+sizeof(WORLD_POS);
		}

	public:
		VOID Init(ObjID_t nObjID, const WORLD_POS *pCurPos)
		{
			m_ObjID			= nObjID;
			if(pCurPos != NULL)
			{
				m_posWorld		= *pCurPos;
			}
		}

		ObjID_t GetObjID(VOID)const
		{
			return m_ObjID;
		}

		const WORLD_POS *GetCurPos(VOID)const
		{
			return &m_posWorld;
		}

	private:
		ObjID_t			m_ObjID;		// ObjID
		WORLD_POS		m_posWorld;		// 位置
	};


	class GCBusStopMoveFactory :
		public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBusStopMove(); }
		PacketID_t	GetPacketID()const { return PACKET_GC_BUSSTOPMOVE; }
		UINT		GetPacketMaxSize()const
		{
			return	sizeof(ObjID_t)
				+sizeof(WORLD_POS);
		}
	};


	class GCBusStopMoveHandler 
	{
	public:
		static UINT Execute(GCBusStopMove* pPacket, Player* pPlayer);
	};
}

using namespace Packets;

#endif // __GCBUSSTOPMOVE_H__
