#ifndef _GCWORLDTIME_H_
#define _GCWORLDTIME_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCWorldTime:public Packet
	{
	public:
		GCWorldTime(){};
		virtual ~GCWorldTime(){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;


		virtual PacketID_t		GetPacketID() const { return PACKET_GC_WORLDTIME ; }

		virtual UINT			GetPacketSize()const { return sizeof(m_Time) + sizeof(m_SystemTime);}

	public:
		time_t					GetSystemTime()
		{
			return m_SystemTime;
		}
		VOID					SetSystemTime(time_t time)
		{
			m_SystemTime = time;
		}

		WORLD_TIME				GetWorldTime()
		{
			return m_Time;
		}
		VOID					SetWorldTime(WORLD_TIME WtTime)
		{
			m_Time = WtTime;
		}

		SceneID_t				GetSceneID()
		{
			return m_SceneID;
		}
		VOID					SetSceneID(SceneID_t SceneID)
		{
			m_SceneID = SceneID;
		}
	private:
		WORLD_TIME				m_Time;
		time_t					m_SystemTime;
		SceneID_t				m_SceneID;
	};

	class GCWorldTimeFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCWorldTime() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_WORLDTIME ; }
		UINT		GetPacketMaxSize()const		{ return	sizeof(WORLD_TIME) + sizeof(time_t);}			
	};

	class GCWorldTimeHandler
	{
	public:
		static UINT Execute(GCWorldTime* pPacket,Player* pPlayer);
	};

};

using namespace Packets;


#endif