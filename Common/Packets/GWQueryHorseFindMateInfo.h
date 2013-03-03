/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#ifndef __GWQueryHorseFindMateInfo_H__
#define __GWQueryHorseFindMateInfo_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GWQueryHorseFindMateInfo : public Packet
	{
	public:
		GWQueryHorseFindMateInfo(){}
		virtual					~GWQueryHorseFindMateInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_QUERYHORSEFINDMATEINFO; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_PlayerID) + sizeof(m_iMaxPostion) + sizeof(GUID_t);
		}

		VOID				SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t			GetPlayerID( ) { return m_PlayerID; }

		INT					GetMaxPostion(){return m_iMaxPostion;}
		VOID				SetMaxPostion(INT value){m_iMaxPostion = value;}

		VOID				SetPlayerGUID(GUID_t id) {m_PlayerGUID = id;}
		GUID_t				GetPlayerGUID() const {return m_PlayerGUID;}

	private:
		GUID_t				m_PlayerGUID;
		PlayerID_t				m_PlayerID;
		INT	m_iMaxPostion; //上次查询时在Users中的最大Index

	};

	class GWQueryHorseFindMateInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GWQueryHorseFindMateInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_GW_QUERYHORSEFINDMATEINFO; }
		UINT					GetPacketMaxSize() const { return sizeof(PlayerID_t) + sizeof(INT) + sizeof(GUID_t); }
	};

	class GWQueryHorseFindMateInfoHandler
	{
	public:
		static UINT Execute( GWQueryHorseFindMateInfo* pPacket, Player* pPlayer );
	};

}


#endif // __CG_GUILD_H__
