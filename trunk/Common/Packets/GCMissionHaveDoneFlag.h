// GCMissionHaveDoneFlag.h
// 
//////////////////////////////////////////////////////

#ifndef __GCMISSIONHAVEDONEFLAG_H__
#define __GCMISSIONHAVEDONEFLAG_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCMissionHaveDoneFlag :
		public Packet
	{
	public:
		GCMissionHaveDoneFlag()
		{
			m_nMissionID = INVALID_ID;
			m_bHaveDown = FALSE;
		}
		virtual ~GCMissionHaveDoneFlag()
		{
		}

		//公用继承接口
		virtual BOOL Read(SocketInputStream& iStream);
		virtual BOOL Write(SocketOutputStream& oStream)const;
		virtual UINT Execute(Player* pPlayer);

		virtual PacketID_t GetPacketID()const
		{
			return PACKET_GC_MISSIONHAVEDONEFLAG;
		}

		virtual UINT GetPacketSize()const
		{
			return sizeof(m_nMissionID) + sizeof(m_bHaveDown);
		}

	public:
		BOOL Init(MissionID_t nMissionID, BOOL bHaveDownFlag)
		{
			m_nMissionID = nMissionID;
			m_bHaveDown = bHaveDownFlag;
			return TRUE;
		}

		MissionID_t GetMissionID(VOID)const
		{
			return m_nMissionID;
		}

		BOOL IsHaveDown(void)const
		{
			return m_bHaveDown;
		}

	private:
		MissionID_t m_nMissionID;
		BOOL m_bHaveDown;
	};

	class GCMissionHaveDoneFlagFactory :
		public PacketFactory 
	{
	public:
		Packet *CreatePacket()
		{
			return new GCMissionHaveDoneFlag();
		}

		PacketID_t GetPacketID()const
		{
			return PACKET_GC_MISSIONHAVEDONEFLAG;
		}

		UINT GetPacketMaxSize()const
		{
			return sizeof(MissionID_t) + sizeof(BOOL);
		}
	};

	class GCMissionHaveDoneFlagHandler 
	{
	public:
		static UINT Execute(GCMissionHaveDoneFlag* pPacket, Player* pPlayer);
	};
}

#endif	// __GCMISSIONHAVEDONEFLAG_H__
