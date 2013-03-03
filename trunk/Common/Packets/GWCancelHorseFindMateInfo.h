/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#ifndef __GWCancelHorseFindMateInfo_H__
#define __GWCancelHorseFindMateInfo_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GWCancelHorseFindMateInfo : public Packet
	{
	public:
		GWCancelHorseFindMateInfo(){}
		virtual					~GWCancelHorseFindMateInfo(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_CANCELHORSEFINDMATEINFO; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_GUID) + sizeof(m_PlayerID);
		}

		PET_GUID_t		GetGUID( VOID )const{ return m_GUID; }
		VOID			SetGUID( PET_GUID_t GUID ){ m_GUID = GUID; }

		VOID				SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t			GetPlayerID( ) { return m_PlayerID; }

	private:
		PlayerID_t		m_PlayerID;
		PET_GUID_t		m_GUID;							// ID

	};

	class GWCancelHorseFindMateInfoFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GWCancelHorseFindMateInfo(); }
		PacketID_t				GetPacketID() const { return PACKET_GW_CANCELHORSEFINDMATEINFO; }
		UINT					GetPacketMaxSize() const { return sizeof(PET_GUID_t) + sizeof(PlayerID_t); }
	};

	class GWCancelHorseFindMateInfoHandler
	{
	public:
		static UINT Execute( GWCancelHorseFindMateInfo* pPacket, Player* pPlayer );
	};

}



#endif // __CG_GUILD_H__
