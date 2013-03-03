/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/


#ifndef __GWIssueHorseFindMateInfo_H__
#define __GWIssueHorseFindMateInfo_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GameStruct_Query.h"

namespace Packets
{
	class GWIssueHorseFindMateInfo : public Packet
	{
	public:
		GWIssueHorseFindMateInfo(){}
		virtual					~GWIssueHorseFindMateInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_ISSUEHORSEFINDMATEINFO; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_HorseFindMateInfo) + sizeof(PlayerID_t);
		}

		VOID SetHorseFindMateInfo(const HorseFindMateInfo& value) {m_HorseFindMateInfo = value;}
		const HorseFindMateInfo& GetHorseFindMateInfo() const {return m_HorseFindMateInfo;}

		VOID SetPlayerID(PlayerID_t id) {m_PlayerID = id;}
		PlayerID_t GetPlayerID() {return m_PlayerID;}

	private:
		PlayerID_t	m_PlayerID;
		HorseFindMateInfo m_HorseFindMateInfo;
	};

	class GWIssueHorseFindMateInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GWIssueHorseFindMateInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_GW_ISSUEHORSEFINDMATEINFO; }
		UINT					GetPacketMaxSize() const {
			return sizeof(HorseFindMateInfo) + sizeof(PlayerID_t); 
		}
	};

	class GWIssueHorseFindMateInfoHandler
	{
	public:
		static UINT Execute( GWIssueHorseFindMateInfo* pPacket, Player* pPlayer );
	};

}



#endif // __CG_GUILD_H__
