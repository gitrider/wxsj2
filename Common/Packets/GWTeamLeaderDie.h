
/********************************************************************************
*	文件名：	CGTopList.h

*	创建人：	王勇鸷
*	创建时间：	2008 年 6 月 23 日	
*
*	功能说明：	队长死亡后通知GL（需要更改队长）
*	修改记录：
*********************************************************************************/
#ifndef _GW_TEAM_LEADER_DIE_H_
#define _GW_TEAM_LEADER_DIE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWTeamLeaderDie:	public Packet
	{
		public:
			GWTeamLeaderDie(){};
			virtual		~GWTeamLeaderDie(){};

			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GW_TEAM_LEADER_DIE ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t) ; }

	public :
		//数据接口
		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

	public :
		//数据
		GUID_t			m_SourGUID ;		//自己的GUID

	};

	class GWTeamLeaderDieFactory: public	PacketFactory
	{
		public:
			Packet*		CreatePacket() { return new GWTeamLeaderDie() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GW_TEAM_LEADER_DIE ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t); }			
	};

	class GWTeamLeaderDieHandler
	{
		public:
			static UINT Execute(GWTeamLeaderDie* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif