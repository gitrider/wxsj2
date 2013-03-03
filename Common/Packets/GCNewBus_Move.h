// GCNewBus_Move.h
// 
/////////////////////////////////////////////////////////////

#ifndef __GCNEWBUS_MOVE_H__
#define __GCNEWBUS_MOVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Bus.h"

namespace Packets
{
	class GCNewBus_Move :
		public Packet
	{
	public:
		GCNewBus_Move()
		{
			m_ObjID			= INVALID_ID;
			m_nDataID		= INVALID_ID;
			//m_posWorld;
			m_fDir			= -1.f;
			//m_posTarget;
			m_nPassengerCount	= 0;
			INT i;
			for(i = 0; i < DEF_BUS_MAX_PASSENGER_COUNT; i++)
			{
				m_anPassengerIDs[i] = INVALID_ID;
			}
		}
		virtual ~GCNewBus_Move()
		{
		}

		//公用继承接口
		virtual BOOL			Read(SocketInputStream& iStream);
		virtual BOOL			Write(SocketOutputStream& oStream)const;
		virtual UINT			Execute(Player* pPlayer);

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWBUS_MOVE; }
		virtual UINT			GetPacketSize()const
		{
			return	sizeof(ObjID_t)
				+ sizeof(INT)
				+ sizeof(WORLD_POS)
				+ sizeof(FLOAT)
				+ sizeof(WORLD_POS)
				+ sizeof(FLOAT)
				+ sizeof(INT)
				+ sizeof(ObjID_t) * m_nPassengerCount;
		}

	public:
		VOID Init(ObjID_t nObjID, INT nDataID, const WORLD_POS *pCurPos, FLOAT fDir, const WORLD_POS *pTargetPos, FLOAT fTargetPosHeight,
			INT nPassengerCount, ObjID_t *paPassengerIDs)
		{
			m_ObjID			= nObjID;
			m_nDataID		= nDataID;
			if(pCurPos != NULL)
			{
				m_posWorld		= *pCurPos;
			}
			m_fDir			= fDir;
			if(pTargetPos != NULL)
			{
				m_posTarget		= *pTargetPos;
			}
			m_fTargetHeight		= fTargetPosHeight;

			m_nPassengerCount	= nPassengerCount;
			if(m_nPassengerCount > 0 && paPassengerIDs)
			{
				memcpy(m_anPassengerIDs, paPassengerIDs, sizeof(ObjID_t) * nPassengerCount);
			}
		}

		ObjID_t GetObjID(VOID)const
		{
			return m_ObjID;
		}

		INT GetDataID(VOID)const
		{
			return m_nDataID;
		}

		const WORLD_POS *GetCurPos(VOID)const
		{
			return &m_posWorld;
		}

		FLOAT GetDir(VOID)const
		{
			return m_fDir;
		}

		const WORLD_POS *GetTargetPos(VOID)const
		{
			return &m_posTarget;
		}

		FLOAT GetTargetPosHeight(VOID)const
		{
			return m_fTargetHeight;
		}

		INT GetPassengerCount(VOID)const
		{
			return m_nPassengerCount;
		}

		const ObjID_t *GetPassengerIDList(VOID)const
		{
			return m_anPassengerIDs;
		}

		ObjID_t GetPassengerIDByIndex(INT nIndex)const
		{
			if(nIndex >= 0 && nIndex < m_nPassengerCount)
			{
				return m_anPassengerIDs[nIndex];
			}
			return INVALID_ID;
		}

	private:
		ObjID_t			m_ObjID;		// ObjID
		INT				m_nDataID;		// 数据ID
		WORLD_POS		m_posWorld;		// 位置
		FLOAT			m_fDir;			// 方向
		WORLD_POS		m_posTarget;	// 目标位置
		FLOAT			m_fTargetHeight;	// 目标点高度

		INT				m_nPassengerCount;		// 乘客数目
		ObjID_t			m_anPassengerIDs[DEF_BUS_MAX_PASSENGER_COUNT];	// 乘客列表
	};


	class GCNewBus_MoveFactory :
		public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCNewBus_Move(); }
		PacketID_t	GetPacketID()const { return PACKET_GC_NEWBUS_MOVE; }
		UINT		GetPacketMaxSize()const
		{
			return	sizeof(ObjID_t)
				+ sizeof(INT)
				+ sizeof(WORLD_POS)
				+ sizeof(FLOAT)
				+ sizeof(WORLD_POS)
				+ sizeof(FLOAT)
				+ sizeof(INT)
				+ sizeof(ObjID_t) * DEF_BUS_MAX_PASSENGER_COUNT;
		}
	};


	class GCNewBus_MoveHandler 
	{
	public:
		static UINT Execute(GCNewBus_Move* pPacket, Player* pPlayer);
	};
}

using namespace Packets;

#endif // __GCNEWBUS_MOVE_H__
