/********************************************************************************
 *	�ļ�����	GWStopTeamFollow.h
 *	ȫ·����	d:\Prj\Common\Packets\GWStopTeamFollow.h
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	23:22
 *
 *	����˵����	֪�� world ĳ����Աֹͣ�˸��棬����Ƕӳ�����ȫ��ȡ��
 *	�޸ļ�¼��
*********************************************************************************/

#ifndef _GW_STOPTEAMFOLLOW_H_
#define _GW_STOPTEAMFOLLOW_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWStopTeamFollow:	public Packet
	{
	public:
		GWStopTeamFollow(){}
		virtual		~GWStopTeamFollow(){}

		//���ýӿ�
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_STOP_TEAMFOLLOW; }
		virtual UINT			GetPacketSize()const { return sizeof(GUID_t); }

	public :
		VOID					SetGUID( GUID_t guid ) { m_GUID = guid; }
		GUID_t					GetGUID( ) { return m_GUID; }

	private :
		GUID_t					m_GUID;	// ֹͣ����״̬����� GUID

	};

	class GWStopTeamFollowFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWStopTeamFollow(); }
		PacketID_t	GetPacketID() const	 { return PACKET_GW_STOP_TEAMFOLLOW; }
		UINT		GetPacketMaxSize() const { return sizeof(GUID_t); }			
	};

	class GWStopTeamFollowHandler
	{
	public:
		static UINT Execute(GWStopTeamFollow* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _GW_STOPTEAMFOLLOW_H_
