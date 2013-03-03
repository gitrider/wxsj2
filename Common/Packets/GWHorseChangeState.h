/********************************************************************************
 *	创建人：	武文斌
 *	创建时间：	2008 年 5 月 20 日	
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#ifndef _GWHorseChangeState_h_
#define _GWHorseChangeState_h_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GWHorseChangeState : public Packet
	{
	public:
		GWHorseChangeState(){}
		virtual					~GWHorseChangeState(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_HORSECHANGESTATE; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_iStateChangeType)+sizeof(m_GUID)+sizeof(PlayerID_t);
		}

		VOID					SetStateChangeType(INT type) { m_iStateChangeType = type; }
		INT					GetStateChangeType() { return m_iStateChangeType; }

		PET_GUID_t		GetGUID( VOID )const{ return m_GUID; }
		VOID			SetGUID( PET_GUID_t GUID ){ m_GUID = GUID; }

		VOID				SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t			GetPlayerID( ) { return m_PlayerID; }

	private:
		PlayerID_t		m_PlayerID;
		PET_GUID_t			m_GUID;
		INT					m_iStateChangeType;

	};

	class GWHorseChangeStateFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GWHorseChangeState(); }
		PacketID_t				GetPacketID() const { return PACKET_GW_HORSECHANGESTATE; }
		UINT					GetPacketMaxSize() const { return sizeof(INT)+sizeof(PET_GUID_t)+sizeof(PlayerID_t); }
	};

	class GWHorseChangeStateHandler
	{
	public:
		static UINT Execute( GWHorseChangeState* pPacket, Player* pPlayer );
	};

}

#endif // __CG_GUILD_H__
