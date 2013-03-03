/********************************************************************************
 *	�ļ�����	WGGuildReturn.h
 *	ȫ·����	d:\Prj\Common\Packets\WGGuildReturn.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 12 �� 12 ��	12:11
 *
 *	����˵����	�������ɹ��ķ���ֵ
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef __WG_GUILDRETURN_H__
#define __WG_GUILDRETURN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGGuildReturn:		public Packet
	{
	public:
		WGGuildReturn(){}
		virtual					~WGGuildReturn(){}

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_GUILDRETURN; }
		virtual UINT			GetPacketSize() const { return sizeof(m_PlayerID) + m_GuildReturn.GetPacketSize(); }

	public :
		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		VOID					SetGuildReturn( _GUILD_RETURN* pGuildReturn )
		{
			memcpy((void*)&m_GuildReturn, pGuildReturn, sizeof(m_GuildReturn));
		}

		_GUILD_RETURN*			GetGuildReturn() { return &m_GuildReturn; }

	private :
		PlayerID_t				m_PlayerID;	//������
		_GUILD_RETURN			m_GuildReturn;

	};

	class WGGuildReturnFactory:	public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new WGGuildReturn(); }
		PacketID_t				GetPacketID() const { return PACKET_WG_GUILDRETURN; }
		UINT					GetPacketMaxSize() const { return sizeof(PlayerID_t) + sizeof(_GUILD_RETURN); }
	};

	class WGGuildReturnHandler
	{
	public:
		static UINT				Execute(WGGuildReturn* pPacket, Player* pPlayer);
	};

}

using namespace Packets;

#endif // __WG_GUILDRETURN_H__
