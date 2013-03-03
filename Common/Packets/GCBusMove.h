// GCBusMove.h
// 
/////////////////////////////////////////////////////////////

#ifndef __GCBUSMOVE_H__
#define __GCBUSMOVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCBusMove :
		public Packet
	{
	public:
		GCBusMove()
		{
			m_ObjID			= INVALID_ID;
			//m_posWorld;
			//m_posTarget;
		}
		virtual ~GCBusMove()
		{
		}

		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BUSMOVE; }
		virtual UINT			GetPacketSize()const
		{
			return	sizeof(ObjID_t)
				+ sizeof(WORLD_POS)
				+ sizeof(WORLD_POS)
				+ sizeof(FLOAT);
		}

	public:
		VOID Init(ObjID_t nObjID, const WORLD_POS *pCurPos, const WORLD_POS *pTargetPos, FLOAT fTargetHeight)
		{
			m_ObjID			= nObjID;
			if(pCurPos != NULL)
			{
				m_posWorld		= *pCurPos;
			}
			if(pTargetPos != NULL)
			{
				m_posTarget		= *pTargetPos;
			}
			m_fTargetHeight = fTargetHeight;
		}

		ObjID_t GetObjID(VOID)const
		{
			return m_ObjID;
		}

		const WORLD_POS *GetCurPos(VOID)const
		{
			return &m_posWorld;
		}

		const WORLD_POS *GetTargetPos(VOID)const
		{
			return &m_posTarget;
		}

		const FLOAT GetTargetHeight(VOID)const
		{
			return m_fTargetHeight;
		}

	private:
		ObjID_t			m_ObjID;			// ObjID
		WORLD_POS		m_posWorld;			// 位置
		WORLD_POS		m_posTarget;		// 目标位置
		FLOAT			m_fTargetHeight;	// 目标高度
	};


	class GCBusMoveFactory :
		public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBusMove(); }
		PacketID_t	GetPacketID()const { return PACKET_GC_BUSMOVE; }
		UINT		GetPacketMaxSize()const
		{
			return	sizeof(ObjID_t)
				+ sizeof(WORLD_POS)
				+ sizeof(WORLD_POS)
				+ sizeof(FLOAT);
		}
	};


	class GCBusMoveHandler 
	{
	public:
		static UINT Execute(GCBusMove* pPacket, Player* pPlayer);
	};
}

using namespace Packets;

#endif // __GCBUSMOVE_H__
